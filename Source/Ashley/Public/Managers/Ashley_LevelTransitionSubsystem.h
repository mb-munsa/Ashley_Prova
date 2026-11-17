// Ashley

#pragma once

#include "CoreMinimal.h"
#include "Containers/Ticker.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "Tags_Enums_Structs/Ashley_MenuPickusStructs.h"

#include "Ashley_LevelTransitionSubsystem.generated.h"

class UAshley_InventoryComponent;

/**
 * Gestisce il cambio livello: fade a nero, caricamento della mappa e fade in.
 * Conserva inoltre l'inventario del player, che altrimenti verrebbe distrutto con il pawn.
 */
UCLASS()
class ASHLEY_API UAshley_LevelTransitionSubsystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()

public:

	virtual void Initialize(FSubsystemCollectionBase& Collection) override;
	virtual void Deinitialize() override;

	UFUNCTION(BlueprintCallable)
	void TravelToLevel(TSoftObjectPtr<UWorld> Level, float FadeDuration = 1.0f);

	// Chiamata dall'inventario del player a inizio livello
	void RestoreInventory(UAshley_InventoryComponent* Inventory);

protected:

	void OnPostLoadMap(UWorld* LoadedWorld);

	void SaveInventory();

	// Ticker e non timer del mondo, che in pausa si fermerebbe
	FTSTicker::FDelegateHandle FadeTickerHandle;

	float CurrentFadeDuration = 0.0f;

	bool bIsTraveling = false;

	UPROPERTY()
	TArray<FItemStack> SavedItems;

	UPROPERTY()
	TArray<FItemStack> SavedCollectables;
};
