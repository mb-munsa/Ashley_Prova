// Ashley


#include "Managers/Ashley_LevelDialogueManager.h"
#include "Interaction/Items/Ashley_Pickup.h"

#include "DebugHelper.h"
#include "Characters/Players/Ashley_CharacterPlayer.h"
#include "Core/Ashley_PlayerController.h"

#include "Components/Ashley_InventoryComponent.h"
#include <Managers/Ashley_WidgetSystem.h>
#include <Kismet/GameplayStatics.h>

// Sets default values
AAshley_LevelDialogueManager::AAshley_LevelDialogueManager()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
}

// Called when the game starts or when spawned
void AAshley_LevelDialogueManager::BeginPlay()
{
	Super::BeginPlay();

	AAshley_Pickup::OnItemPickup.AddUObject(this, &AAshley_LevelDialogueManager::OnPlayerItemPickup);
}

void AAshley_LevelDialogueManager::OnPlayerItemPickup(FName ItemID)
{
	if (Character == nullptr)
	{
		Player = Cast<AAshley_PlayerController>(GetWorld()->GetFirstPlayerController());
		Character = Cast<AAshley_CharacterPlayer>(Player->GetCharacter());
	}

	auto Inventory = Character->GetInventory();

	//Debug::Print(FString::Printf(TEXT("Picked up %s"), *(ItemID.ToString())));

	// Se ho già visto questo item, riproduco solo il dialogo generico
	if (ItemsAlreadySeen.Contains(ItemID))
	{
		auto Text = PickupDialogueDA->GetGenericDialogueForPickup(ItemID);
		UGameplayStatics::GetGameInstance(this)->GetSubsystem<UAshley_WidgetSystem>()->SpawnInfoText(Text.ToString(), 8.0f, false);
	}
	// altrimenti, cerco il dialogo specifico, a seconda della situazione attuale
	else
	{
		auto Text = PickupDialogueDA->GetDialogueForPickup(ItemID, Inventory);
		UGameplayStatics::GetGameInstance(this)->GetSubsystem<UAshley_WidgetSystem>()->SpawnInfoText(Text.ToString(), 8.0f, false);
		ItemsAlreadySeen.Add(ItemID);
	}

	//Debug::Print(FString::Printf(TEXT("Dialogue %s"), *(Text.ToString())));
}

