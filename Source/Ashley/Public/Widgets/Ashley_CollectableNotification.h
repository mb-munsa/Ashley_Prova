// Ashley

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Ashley_CollectableNotification.generated.h"

class UImage;
class UWidgetAnimation;
class UAshley_PickupDA;

/**
 *
 */
UCLASS()
class ASHLEY_API UAshley_CollectableNotification : public UUserWidget
{
	GENERATED_BODY()

public:

	// Ritorna false se l'item non ha un'icona da mostrare
	UFUNCTION()
	bool SetItem(FName ItemID);

	UFUNCTION()
	void PlayShowAnimation();

	// Ritorna la durata dell'animazione
	UFUNCTION()
	float PlayHideAnimation();

protected:

	UPROPERTY(meta = (BindWidget))
	UImage* Icon;

	// Fade in: la stessa animazione viene riprodotta al contrario per il fade out
	UPROPERTY(Transient, meta = (BindWidgetAnim))
	UWidgetAnimation* ShowAnimation;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data|Inventory")
	TArray<UAshley_PickupDA*> ItemsDataAsset;
};
