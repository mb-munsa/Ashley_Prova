// Ashley

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Ashley_CalendarWidget.generated.h"

class UImage;
class UCanvas;
class UMaterialInterface;
class UMaterialInstanceDynamic;
class UTexture2D;
class UTextureRenderTarget2D;
class AAshley_PlayerController;
class AAshley_MinivanPuzzle;

// Puzzle del calendario: il giocatore "gratta" la carta col mouse per rivelare la soluzione
UCLASS()
class ASHLEY_API UAshley_CalendarWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	// BP_Calendar che ha aperto il widget, notificato alla vittoria
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Calendar", meta = (ExposeOnSpawn = true))
	TObjectPtr<AAshley_MinivanPuzzle> PuzzleManager;

	// Stesso nome della variabile del vecchio WBP, così i nodi Set Activated di BP_Calendar la ritrovano
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Calendar")
	bool Activated = true;

protected:
	virtual void NativeConstruct() override;
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;
	virtual void NativeDestruct() override;

	UFUNCTION()
	void CheckWin();

	void DrawBrush(bool bStrokeStart);

	void DrawPoint(UCanvas* Canvas, const FVector2D& Size, const FVector2D& Location);

	void NotifyWin();

	// Scopre tutto il testo, compresa la parte che il giocatore non ha grattato
	void RevealText();

	void TickZoom(const FVector2D& LocalSize, float InDeltaTime);

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UImage> Background;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UImage> PencilImage;

	// Immagine della carta
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Calendar")
	TObjectPtr<UTexture2D> BaseImage;

	// Testo in bianco da scoprire per vincere
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Calendar")
	TObjectPtr<UTexture2D> ForegroundImage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Calendar")
	TObjectPtr<UMaterialInterface> PaperMaterialBase;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Calendar")
	TObjectPtr<UMaterialInterface> BrushMaterialBase;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Calendar")
	double BrushSize = 0.0;

	// Distanza tra i timbri a mouse lento, in UV: più è alta, più il grattare lento rivela poco
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Calendar")
	double SlowStrokeSpacing = 0.06;

	// Distanza tra i timbri a mouse veloce, in UV: più è bassa, più i tratti lunghi rivelano
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Calendar")
	double FastStrokeSpacing = 0.006;

	// Velocità del mouse, in UV al secondo, a cui si usa FastStrokeSpacing
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Calendar")
	double FastStrokeSpeed = 1.0;

	// Percentuale del testo da scoprire per vincere
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Calendar")
	float WinPercentage = 0.9f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Calendar")
	float CheckWinInterval = 2.f;

	// Punto della carta da portare al centro dopo la vittoria, in UV
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Calendar")
	FVector2D ZoomTargetUV = FVector2D(0.374, 0.699);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Calendar")
	float ZoomScale = 3.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Calendar")
	float ZoomDuration = 1.2f;

	// Tempo per leggere il testo prima di notificare la vittoria
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Calendar")
	float ZoomHoldDuration = 2.f;

	UPROPERTY(Transient)
	TObjectPtr<UMaterialInstanceDynamic> PaperMaterial;

	UPROPERTY(Transient)
	TObjectPtr<UMaterialInstanceDynamic> BrushMaterial;

	// Maschera di quello che è stato grattato
	UPROPERTY(Transient)
	TObjectPtr<UTextureRenderTarget2D> RenderTex;

	UPROPERTY(Transient)
	TObjectPtr<UTextureRenderTarget2D> TempRT;

	UPROPERTY()
	TObjectPtr<AAshley_PlayerController> PlayerController;

	// UV normalizzate dei pixel del testo
	TArray<FVector2D> TextUVs;

	FVector2D DrawLocation = FVector2D::ZeroVector;
	FVector2D PrevLocation = FVector2D::ZeroVector;

	// True mentre il mouse è premuto dentro l'immagine
	bool bIsStroking = false;

	// Distanza percorsa dall'ultimo timbro, in UV
	double DistanceSinceStamp = 0.0;

	// Velocità del mouse mediata, in UV al secondo
	double SmoothedSpeed = 0.0;

	// True se la maschera è cambiata dall'ultimo CheckWin
	bool bMaskDirty = false;

	// True dalla vittoria alla fine dello zoom
	bool bIsZooming = false;

	float ZoomElapsed = 0.f;

	FTimerHandle TimerHandle_CheckWin;
	FTimerHandle TimerHandle_NotifyWin;
};
