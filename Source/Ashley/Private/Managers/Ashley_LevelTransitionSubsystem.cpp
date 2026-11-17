// Ashley


#include "Managers/Ashley_LevelTransitionSubsystem.h"

#include "Camera/PlayerCameraManager.h"
#include "Characters/Players/Ashley_CharacterPlayer.h"
#include "Components/Ashley_InventoryComponent.h"

#include <Kismet/GameplayStatics.h>

void UAshley_LevelTransitionSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);

	FCoreUObjectDelegates::PostLoadMapWithWorld.AddUObject(this, &UAshley_LevelTransitionSubsystem::OnPostLoadMap);
}

void UAshley_LevelTransitionSubsystem::Deinitialize()
{
	FCoreUObjectDelegates::PostLoadMapWithWorld.RemoveAll(this);
	FTSTicker::GetCoreTicker().RemoveTicker(FadeTickerHandle);

	Super::Deinitialize();
}

void UAshley_LevelTransitionSubsystem::TravelToLevel(TSoftObjectPtr<UWorld> Level, float FadeDuration)
{
	if (bIsTraveling) return;

	if (Level.IsNull())
	{
		UE_LOG(LogTemp, Warning, TEXT("TravelToLevel called without a valid level"));
		return;
	}

	bIsTraveling = true;
	CurrentFadeDuration = FadeDuration;

	UWorld* World = GetWorld();

	if (APlayerController* PlayerController = World->GetFirstPlayerController())
	{
		PlayerController->DisableInput(PlayerController);

		if (PlayerController->PlayerCameraManager)
			PlayerController->PlayerCameraManager->StartCameraFade(0.0f, 1.0f, FadeDuration, FLinearColor::Black, false, true);
	}

	// Carico il livello solo a fade finito, cosi' non distruggo il mondo durante il broadcast di un evento
	FadeTickerHandle = FTSTicker::GetCoreTicker().AddTicker(
		FTickerDelegate::CreateWeakLambda(this, [this, Level](float DeltaTime)
		{
			SaveInventory();
			UGameplayStatics::OpenLevelBySoftObjectPtr(GetWorld(), Level);
			return false;
		}),
		FadeDuration
	);
}

void UAshley_LevelTransitionSubsystem::OnPostLoadMap(UWorld* LoadedWorld)
{
	if (!bIsTraveling) return;

	bIsTraveling = false;

	if (!LoadedWorld) return;

	APlayerController* PlayerController = LoadedWorld->GetFirstPlayerController();
	if (PlayerController && PlayerController->PlayerCameraManager)
		PlayerController->PlayerCameraManager->StartCameraFade(1.0f, 0.0f, CurrentFadeDuration, FLinearColor::Black);
}

void UAshley_LevelTransitionSubsystem::SaveInventory()
{
	SavedItems.Reset();
	SavedCollectables.Reset();

	APlayerController* PlayerController = GetWorld()->GetFirstPlayerController();
	if (!PlayerController) return;

	AAshley_CharacterPlayer* Character = Cast<AAshley_CharacterPlayer>(PlayerController->GetCharacter());
	if (!Character) return;

	SavedItems = Character->GetInventory()->GetAllItems();
	SavedCollectables = Character->GetInventory()->GetCollectables();
}

void UAshley_LevelTransitionSubsystem::RestoreInventory(UAshley_InventoryComponent* Inventory)
{
	for (const FItemStack& Item : SavedItems)
	{
		Inventory->PutObjectInventory(Item.ItemID, EPickupType::OBJECT, Item.StackSize);
	}

	for (const FItemStack& Collectable : SavedCollectables)
	{
		Inventory->RestoreCollectable(Collectable.ItemID);
	}

	SavedItems.Reset();
	SavedCollectables.Reset();
}
