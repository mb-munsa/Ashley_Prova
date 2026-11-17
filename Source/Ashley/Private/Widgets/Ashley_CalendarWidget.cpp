// Ashley


#include "Widgets/Ashley_CalendarWidget.h"

#include "Components/Image.h"
#include "Components/CanvasPanelSlot.h"
#include "Blueprint/WidgetLayoutLibrary.h"
#include "Engine/Canvas.h"
#include "Engine/Texture2D.h"
#include "Engine/TextureRenderTarget2D.h"
#include "Materials/MaterialInstanceDynamic.h"
#include "Kismet/KismetRenderingLibrary.h"
#include "Kismet/KismetMaterialLibrary.h"
#include "UObject/ConstructorHelpers.h"
#include <Kismet/GameplayStatics.h>

#include "MyBlueprintFunctionLibrary.h"
#include "Core/Ashley_PlayerController.h"
#include "Interaction/Ashley_MinivanPuzzle.h"

#include "DebugHelper.h"

void UAshley_CalendarWidget::NativeConstruct()
{
	Super::NativeConstruct();

	if (!BaseImage)
	{
		UE_LOG(LogTemp, Error, TEXT("%s: BaseImage non assegnata"), *GetName());
		return;
	}

	const int32 Width = BaseImage->GetSizeX();
	const int32 Height = BaseImage->GetSizeY();

	// Materiale della carta: base + soluzione, mascherati da quello che viene grattato
	RenderTex = UKismetRenderingLibrary::CreateRenderTarget2D(this, Width, Height, RTF_RGBA8, FLinearColor(0.f, 0.f, 0.f, 0.f));
	BrushMaterial = UKismetMaterialLibrary::CreateDynamicMaterialInstance(this, BrushMaterialBase);
	PaperMaterial = UKismetMaterialLibrary::CreateDynamicMaterialInstance(this, PaperMaterialBase);

	PaperMaterial->SetTextureParameterValue(TEXT("BaseTex"), BaseImage);
	PaperMaterial->SetTextureParameterValue(TEXT("HiddenTex"), ForegroundImage);
	PaperMaterial->SetTextureParameterValue(TEXT("MaskTex"), RenderTex);
	Background->SetBrushFromMaterial(PaperMaterial);

	// Disegno il testo su un RT temporaneo per ricavare i pixel da scoprire
	TempRT = UKismetRenderingLibrary::CreateRenderTarget2D(this, Width, Height, RTF_RGBA16f, FLinearColor(1.f, 1.f, 1.f, 0.f));

	UCanvas* Canvas;
	FVector2D Size;
	FDrawToRenderTargetContext Context;
	UKismetRenderingLibrary::BeginDrawCanvasToRenderTarget(this, TempRT, Canvas, Size, Context);
	Canvas->K2_DrawTexture(ForegroundImage, FVector2D::ZeroVector, Size, FVector2D::ZeroVector, FVector2D::UnitVector, FLinearColor::White, BLEND_Opaque);
	UKismetRenderingLibrary::EndDrawCanvasToRenderTarget(this, Context);

	UMyBlueprintFunctionLibrary::CountWhitePixels(this, TempRT, TextUVs, 170.f);

	GetWorld()->GetTimerManager().SetTimer(TimerHandle_CheckWin, this, &ThisClass::CheckWin, CheckWinInterval, true);

	PlayerController = Cast<AAshley_PlayerController>(UGameplayStatics::GetPlayerController(this, 0));
}

void UAshley_CalendarWidget::NativeDestruct()
{
	if (UWorld* World = GetWorld())
	{
		World->GetTimerManager().ClearTimer(TimerHandle_CheckWin);
		World->GetTimerManager().ClearTimer(TimerHandle_NotifyWin);
	}

	Super::NativeDestruct();
}

void UAshley_CalendarWidget::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);

	// TODO: debug temporaneo, rimuovere
	{
		FVector2D DebugPosition;
		const bool bDebugInside = UMyBlueprintFunctionLibrary::GetNormalisedMousePositionInGeometry(this, Background->GetTickSpaceGeometry(), DebugPosition);
		Debug::Print(FString::Printf(TEXT("Calendar | Activated %d | PC %d | Mouse %d | Inside %d (%.2f, %.2f) | RenderTex %d | Brush %d | BrushSize %.1f"),
			Activated, PlayerController != nullptr, PlayerController && PlayerController->IsMousePressed(),
			bDebugInside, DebugPosition.X, DebugPosition.Y, RenderTex != nullptr, BrushMaterial != nullptr, BrushSize), 7741, FColor::Yellow);
	}

	if (bIsZooming)
	{
		TickZoom(Background->GetTickSpaceGeometry().GetLocalSize(), InDeltaTime);
		return;
	}

	if (!Activated || !PlayerController || !PlayerController->IsMousePressed())
	{
		bIsStroking = false;
		return;
	}

	FVector2D Position;
	if (!UMyBlueprintFunctionLibrary::GetNormalisedMousePositionInGeometry(this, Background->GetTickSpaceGeometry(), Position))
	{
		bIsStroking = false;
		return;
	}

	DrawLocation = Position;

	// Un nuovo tratto parte da qui, senza collegarsi al punto in cui è finito il precedente
	const bool bStrokeStart = !bIsStroking;
	if (bStrokeStart)
	{
		PrevLocation = DrawLocation;
		bIsStroking = true;
		DistanceSinceStamp = 0.0;
		SmoothedSpeed = 0.0;
	}

	const double Distance = FVector2D::Distance(DrawLocation, PrevLocation);

	// Velocità mediata: il mouse non si aggiorna ad ogni frame e quella grezza oscillerebbe troppo
	SmoothedSpeed = FMath::FInterpTo(SmoothedSpeed, Distance / FMath::Max((double)InDeltaTime, UE_KINDA_SMALL_NUMBER), (double)InDeltaTime, 10.0);

	if (bStrokeStart || Distance > 0.0)
	{
		if (UCanvasPanelSlot* PencilSlot = UWidgetLayoutLibrary::SlotAsCanvasSlot(PencilImage))
			PencilSlot->SetPosition(UWidgetLayoutLibrary::GetMousePositionOnViewport(this));

		DrawBrush(bStrokeStart);
	}

	PrevLocation = DrawLocation;
}

void UAshley_CalendarWidget::DrawBrush(bool bStrokeStart)
{
	const double Distance = FVector2D::Distance(DrawLocation, PrevLocation);

	// Il brush è additivo: più timbri per tratto = più rivelazione. Più il mouse è veloce, più i timbri sono fitti
	const double SpeedAlpha = FMath::Clamp(SmoothedSpeed / FMath::Max(FastStrokeSpeed, UE_KINDA_SMALL_NUMBER), 0.0, 1.0);
	const double StampsPerUnit = FMath::Lerp(1.0 / FMath::Max(SlowStrokeSpacing, UE_KINDA_SMALL_NUMBER), 1.0 / FMath::Max(FastStrokeSpacing, UE_KINDA_SMALL_NUMBER), SpeedAlpha);

	// Limite per i salti molto lunghi: allargo i timbri invece di tagliare il tratto
	const double Spacing = FMath::Max(1.0 / StampsPerUnit, Distance / 128.0);

	double NextStamp = FMath::Max(Spacing - DistanceSinceStamp, 0.0);

	if (!bStrokeStart && NextStamp > Distance)
	{
		DistanceSinceStamp += Distance;
		return;
	}

	UCanvas* Canvas;
	FVector2D Size;
	FDrawToRenderTargetContext Context;
	UKismetRenderingLibrary::BeginDrawCanvasToRenderTarget(this, RenderTex, Canvas, Size, Context);

	if (bStrokeStart)
	{
		DrawPoint(Canvas, Size, DrawLocation);
	}

	while (NextStamp <= Distance && Distance > 0.0)
	{
		DrawPoint(Canvas, Size, FMath::Lerp(PrevLocation, DrawLocation, NextStamp / Distance));
		NextStamp += Spacing;
	}

	DistanceSinceStamp = Distance - (NextStamp - Spacing);

	UKismetRenderingLibrary::EndDrawCanvasToRenderTarget(this, Context);

	bMaskDirty = true;
}

void UAshley_CalendarWidget::DrawPoint(UCanvas* Canvas, const FVector2D& Size, const FVector2D& Location)
{
	const FVector2D ScreenPosition = Size * Location - FVector2D(BrushSize / 2.0);
	Canvas->K2_DrawMaterial(BrushMaterial, ScreenPosition, FVector2D(BrushSize), FVector2D::ZeroVector);
}

void UAshley_CalendarWidget::CheckWin()
{
	// Senza pixel da scoprire la percentuale sarebbe NaN e il controllo sotto darebbe vittoria
	if (TextUVs.IsEmpty())
	{
		UE_LOG(LogTemp, Error, TEXT("%s: nessun pixel del testo trovato in ForegroundImage"), *GetName());
		GetWorld()->GetTimerManager().ClearTimer(TimerHandle_CheckWin);
		return;
	}

	// Rileggo la maschera solo se è cambiata
	if (!Activated || !bMaskDirty) return;

	bMaskDirty = false;

	const float Completed = UMyBlueprintFunctionLibrary::CountCompletedPercentage(this, RenderTex, TextUVs);

	if (Completed < WinPercentage)
	{
		Debug::Print(FString::SanitizeFloat(Completed));
		return;
	}

	Debug::Print("WIIIIN");

	Activated = false;
	GetWorld()->GetTimerManager().ClearTimer(TimerHandle_CheckWin);
	PencilImage->SetVisibility(ESlateVisibility::Collapsed);

	// Chiudere il widget ora cancellerebbe NotifyWin
	if (IsValid(PuzzleManager)) PuzzleManager->SetExitBlocked(true);

	RevealText();

	ZoomElapsed = 0.f;
	bIsZooming = true;
}

void UAshley_CalendarWidget::RevealText()
{
	UCanvas* Canvas;
	FVector2D Size;
	FDrawToRenderTargetContext Context;
	UKismetRenderingLibrary::BeginDrawCanvasToRenderTarget(this, RenderTex, Canvas, Size, Context);

	// Il brush è debole e irregolare: serve una griglia fitta per coprire tutta l'area del testo
	const FBox2D TextBounds(TextUVs);
	const double GridStep = FMath::Max(BrushSize * 0.1, 1.0);

	for (double Y = TextBounds.Min.Y; Y <= TextBounds.Max.Y; Y += GridStep)
	{
		for (double X = TextBounds.Min.X; X <= TextBounds.Max.X; X += GridStep)
		{
			DrawPoint(Canvas, Size, FVector2D(X, Y) / Size);
		}
	}

	UKismetRenderingLibrary::EndDrawCanvasToRenderTarget(this, Context);
}

void UAshley_CalendarWidget::TickZoom(const FVector2D& LocalSize, float InDeltaTime)
{
	// I widget ticcano anche in pausa, i timer no: lo zoom si ferma con il resto del gioco
	if (UGameplayStatics::IsGamePaused(this)) return;

	ZoomElapsed = FMath::Min(ZoomElapsed + InDeltaTime, ZoomDuration);

	const float Alpha = ZoomDuration > 0.f ? FMath::InterpEaseInOut(0.f, 1.f, ZoomElapsed / ZoomDuration, 2.f) : 1.f;
	const float Scale = FMath::Lerp(1.f, ZoomScale, Alpha);

	// Scalo dal centro e trasli per portare ZoomTargetUV al centro del widget
	const FVector2D Offset = (FVector2D(0.5, 0.5) - ZoomTargetUV) * LocalSize * Scale * Alpha;
	Background->SetRenderTransform(FWidgetTransform(Offset, FVector2D(Scale, Scale), FVector2D::ZeroVector, 0.f));

	if (ZoomElapsed < ZoomDuration) return;

	bIsZooming = false;

	// Con durata 0 SetTimer annullerebbe il timer e la vittoria non arriverebbe mai
	GetWorld()->GetTimerManager().SetTimer(TimerHandle_NotifyWin, this, &ThisClass::NotifyWin, FMath::Max(ZoomHoldDuration, 0.01f), false);
}

void UAshley_CalendarWidget::NotifyWin()
{
	if (!IsValid(PuzzleManager)) return;

	// NotifyWinToPuzzle è implementata in BP_Calendar
	if (UFunction* NotifyFunction = PuzzleManager->FindFunction(TEXT("NotifyWinToPuzzle")))
	{
		PuzzleManager->ProcessEvent(NotifyFunction, nullptr);
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("%s: NotifyWinToPuzzle non trovata in %s"), *GetName(), *PuzzleManager->GetName());
	}

	// Se il puzzle è ancora vivo la vittoria non l'ha chiuso: il giocatore deve poter uscire
	if (IsValid(PuzzleManager)) PuzzleManager->SetExitBlocked(false);
}
