// Ashley


#include "MyBlueprintFunctionLibrary.h"
#include "NativeGameplayTags.h"
#include <Blueprint/SlateBlueprintLibrary.h>
#include <Blueprint/WidgetLayoutLibrary.h>
#include "Engine/TextureRenderTarget2D.h"
#include <Kismet/KismetRenderingLibrary.h>

#include "DebugHelper.h"

bool UMyBlueprintFunctionLibrary::TryRequestGameplayTag(FName TagName, FGameplayTag& Tag)
{
	Tag = UGameplayTagsManager::Get().RequestGameplayTag(TagName, false);
	return Tag.IsValid();
}

bool UMyBlueprintFunctionLibrary::GetNormalisedMousePositionInGeometry(UObject* WorldContextObject, FGeometry Geometry, FVector2D& Position)
{
	FVector2D pixelPosition;
	FVector2D viewportMinPosition;
	FVector2D viewportMaxPosition;

	// Get the top left and bottom right viewport positions
	USlateBlueprintLibrary::LocalToViewport(WorldContextObject, Geometry, FVector2D(0, 0), pixelPosition, viewportMinPosition);
	USlateBlueprintLibrary::LocalToViewport(WorldContextObject, Geometry, Geometry.GetLocalSize(), pixelPosition, viewportMaxPosition);

	// Get the mouse's current position in the viewport
	FVector2D mousePosition = UWidgetLayoutLibrary::GetMousePositionOnViewport(WorldContextObject);

	// Determine where the mouse is relative to the widget geometry
	Position = (mousePosition - viewportMinPosition) / (viewportMaxPosition - viewportMinPosition);

	// Calculate if the mouse is inside the geometry or not
	return FMath::Min(Position.X, Position.Y) >= 0.f && FMath::Max(Position.X, Position.Y) <= 1.f;
}

bool UMyBlueprintFunctionLibrary::CountWhitePixels(UObject* WorldContextObject, UTextureRenderTarget2D* Texture, TArray<FVector2D> &Pixels, float Threshold)
{
	TArray<FColor> ColorPixels;
	UKismetRenderingLibrary::ReadRenderTarget(WorldContextObject, Texture, ColorPixels);

	Pixels.Empty();

	int32 Width = Texture->SizeX;
	int32 Height = Texture->SizeY;

	for (int32 y = 0; y < Height; ++y)
	{
		for (int32 x = 0; x < Width; ++x)
		{
			const FColor& Pixel = ColorPixels[y * Width + x];

			if (Pixel.R > Threshold)
			{
				FVector2D PixelCoord(x, y);
				Pixels.Add(PixelCoord);
			}
		}
	}

	//Debug::Print(TEXT("Counted pixels.. %d,"), Pixels.Num());

	return true;
}

float UMyBlueprintFunctionLibrary::CountCompletedPercentage(UObject* WorldContextObject, UTextureRenderTarget2D* Texture, TArray<FVector2D> PixelCoords)
{
	int32 RevealedCount = 0;
	TArray<FColor> RevealPixels;
	UKismetRenderingLibrary::ReadRenderTarget(WorldContextObject, Texture, RevealPixels);

	for (const FVector2D& Coord : PixelCoords)
	{
		if (RevealPixels[Coord.Y * Texture->SizeX + Coord.X].R > 100)
		{
			RevealedCount++;
		}
	}

	//Debug::Print(FString::Printf(TEXT("RevealedCount %d,"), RevealedCount));
	//Debug::Print(FString::Printf(TEXT("PixelCoords.Num() %d,"), PixelCoords.Num()));

	float RevealedPercentage = (float)RevealedCount / (float)PixelCoords.Num();
	return RevealedPercentage;
}
