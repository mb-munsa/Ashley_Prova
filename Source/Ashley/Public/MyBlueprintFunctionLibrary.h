// Ashley

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "GameplayTagContainer.h"
#include "Layout/Geometry.h" 
#include "MyBlueprintFunctionLibrary.generated.h"

/**
 * 
 */
UCLASS()
class ASHLEY_API UMyBlueprintFunctionLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

	public:
	UFUNCTION(BlueprintPure, Category = "GameplayTags")
	static bool TryRequestGameplayTag(FName TagName, FGameplayTag& Tag);

	UFUNCTION(BlueprintPure, Category = "Input")
	static bool GetNormalisedMousePositionInGeometry(UObject* WorldContextObject, FGeometry Geometry, FVector2D& Position);

	UFUNCTION(BlueprintPure, Category = "Rendering")
	static bool CountWhitePixels(UObject* WorldContextObject, UTextureRenderTarget2D* Texture, TArray<FVector2D> &Pixels, float Threshold = 200);

	UFUNCTION(BlueprintPure, Category = "Rendering")
	static float CountCompletedPercentage(UObject* WorldContextObject, UTextureRenderTarget2D* Texture, TArray<FVector2D> Pixels);
	
};
