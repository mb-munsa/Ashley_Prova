// Ashley


#include "Interaction/Ashley_Openable.h"

#include "DataAssets/Ashley_PickupDA.h"

#include "Core/Ashley_TriggerCollider.h"
#include "Characters/Players/Ashley_CharacterPlayer.h"
#include "Components/Ashley_InventoryComponent.h"
#include "Components/Ashley_InteractionPrompt.h"

#include "DebugHelper.h"
#include <Kismet/GameplayStatics.h>
#include <Managers/Ashley_EventManagerSystem.h>

AAshley_Openable::AAshley_Openable()
{
	MeshBase = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshBase"));
	MeshBase->SetupAttachment(RootComponent);

	MeshOpenable = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshOpenable"));
	MeshOpenable->SetupAttachment(RootComponent);

	MeshOpenable->SetRenderCustomDepth(true);
	MeshOpenable->SetCustomDepthStencilValue(0);

	MeshBase->SetRenderCustomDepth(true);
	MeshBase->SetCustomDepthStencilValue(0);

	// Configuro il layer per essere interagibile dal player
	MeshBase->SetCollisionResponseToChannel(ECC_GameTraceChannel1, ECR_Block);
	MeshOpenable->SetCollisionResponseToChannel(ECC_GameTraceChannel1, ECR_Block);

	InitialState = EState::CLOSED;
	CurrentState = EState::CLOSED;
	bIsThereAPickup = false;
	bCanBeClosed = true;
	Name_ID = "";

	Pickup = FInventoryInfo();
}

#if WITH_EDITOR
void AAshley_Openable::PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent)
{

	Super::PostEditChangeProperty(PropertyChangedEvent);

	FName PropertyName = (PropertyChangedEvent.Property != nullptr)
		? PropertyChangedEvent.Property->GetFName()
		: NAME_None;

	if (bIsThereAPickup)
	{
		if (PropertyName == GET_MEMBER_NAME_CHECKED(AAshley_Openable, Name_ID))
		{
			UAshley_PickupDA* LoadedAsset;
			if (PickupDataAsset.IsNull())
			{
				UE_LOG(LogTemp, Warning, TEXT("PickupDataAsset is not assigned in %s"), *GetName());
				return;
			}

			if (PickupDataAsset.IsValid()) LoadedAsset = PickupDataAsset.Get();
			else LoadedAsset = PickupDataAsset.LoadSynchronous();

			if (LoadedAsset)
			{
				Pickup = LoadedAsset->GetInventoryStructByID(Name_ID);
			}
		}
	}
}
#endif

// Not wrapped in WITH_EDITOR: this is a UFUNCTION, so UHT generates an exec thunk in every build configuration.
TArray<FName> AAshley_Openable::GetValidIDs() const
{
	//if (bIsThereAPickup)
	UAshley_PickupDA* LoadedAsset;
	if (PickupDataAsset.IsValid())
	{
		LoadedAsset = PickupDataAsset.Get();
	}
	else
	{
		LoadedAsset = PickupDataAsset.LoadSynchronous();
	}
	TArray<FName> IDs;
	if (LoadedAsset)
	{
		IDs = LoadedAsset->GetIDsValids();
	}
	return IDs;
	//return TArray<FName>();
}

void AAshley_Openable::BeginPlay()
{
	Super::BeginPlay();

	InitialOpenableRotation = MeshOpenable->GetRelativeRotation().GetComponentForAxis(EAxis::Z);

	if (InitialState == EState::OPEN) SwitchToOpen();
}

void AAshley_Openable::GivePickup()
{
	if (CharacterInRange == nullptr) return;

	UAshley_PickupDA* LoadedAsset = PickupDataAsset.LoadSynchronous();

	if (!LoadedAsset)
	{
		UE_LOG(LogTemp, Warning, TEXT("PickupDataAsset is not assigned in %s"), *GetName());
		return;
	}

	auto InventoryInfo = LoadedAsset->GetInventoryStructByID(Name_ID);
	CharacterInRange->PickupItem(Name_ID, InventoryInfo, StackSize);
	bIsThereAPickup = false;

	PickupDataAsset.Reset();
}

void AAshley_Openable::NotifyActorEndOverlap(AActor* OtherActor)
{
	Super::NotifyActorEndOverlap(OtherActor);

	if (bShouldAutoclose && CurrentState == EState::OPEN)
	{
		auto Character = Cast<AAshley_CharacterPlayer>(OtherActor);
		if (/*!bIgnoreCollision &&*/ Character)
		{
			OpenCloseDoor(false);
			CurrentState = EState::CLOSED;
		}
	}
}

void AAshley_Openable::Interact_Implementation()
{
	//Debug::Print("INTERACT PRESSED");
	if (!IsInteractable()) return;

	auto Character = Cast<AAshley_CharacterPlayer>(GetWorld()->GetFirstPlayerController()->GetCharacter());

	switch (CurrentState)
	{
		case EState::OPEN:

			SwitchToClosed();
			break;

		case EState::CLOSED:

			switch (LockType)
			{
				case ELockType::LOCKED_KEY:

					if (Character->GetInventory()->UseItem(Key))
						SwitchToOpen();
					else
					{
						if (ClosedDoorDialogue) ClosedDoorDialogue->PlayDialogue(GetGameInstance());
						Debug::Print("Serve una chiave!");
					}
					break;

				case ELockType::UNLOCKED:

					SwitchToOpen();
					break;

				case ELockType::LOCKED:
					if (ClosedDoorDialogue) ClosedDoorDialogue->PlayDialogue(GetGameInstance());
					break;
			}
	}
}

void AAshley_Openable::ResetState()
{
	if (CurrentState == InitialState) return;

	else if (CurrentState == EState::CLOSED) SwitchToOpen();
	else SwitchToClosed();
}

void AAshley_Openable::SwitchToClosed()
{
	if (CurrentState != EState::OPEN) return;

	// If the door cannot be closed, re-enable interaction
	if (!bCanBeClosed) ToggleInteraction(true);

	OpenCloseDoor(false);
	CurrentState = EState::CLOSED;
	InteractionWidget->SetText(GetInteractMessage());
}

void AAshley_Openable::SwitchToOpen()
{
	if (CurrentState == EState::OPEN) return;

	OpenCloseDoor(true);
	if (bIsThereAPickup) GivePickup();

	// If the door cannot be closed, disable interaction
	if (!bCanBeClosed) ToggleInteraction(false);

	CurrentState = EState::OPEN;

	// Play dialogue on open, only first time
	if (OpenDoorDialogue && !bFirstOpenDone)
		OpenDoorDialogue->PlayDialogue(GetGameInstance());

	if (bShouldTriggerEvent && !bFirstOpenDone)
	{
		auto EventManager = UGameplayStatics::GetGameInstance(this)->GetSubsystem<UAshley_EventManagerSystem>();
		EventManager->BroadcastEventTriggered(EventName, EventParam);
	}
	InteractionWidget->SetText(GetInteractMessage());

	bFirstOpenDone = true;
}

FString AAshley_Openable::GetInteractMessage() const
{
	return CurrentState == EState::OPEN ? "Close" : "Open";
}

