// Ashley


#include "Characters/Players/Ashley_CharacterPlayer.h"
#include "Core/Ashley_PlayerController.h"
#include "Components/ArrowComponent.h"
#include "Components/WidgetComponent.h"
#include "Components/Ashley_InventoryComponent.h"
#include "Components/Ashley_CameraManager.h"
#include "Core/Ashley_FixedCameraController.h"
#include "Core/Ashley_DialogueComponent.h"
#include "Interaction/Items/Ashley_CarryItem.h"
#include "Interaction/Items/Ashley_ItemShowcase.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Tags_Enums_Structs/Ashley_MenuPickusStructs.h"
#include "DataAssets/Ashley_InputConfigDA.h"

#include "DebugHelper.h"
#include "Kismet/GameplayStatics.h"
#include "Managers/Ashley_QuestManager.h"
#include <Managers/Ashley_WidgetSystem.h>
#include <Managers/Ashley_EventManagerSystem.h>
#include "Tags_Enums_Structs/Ashley_GameplayTags.h"
#include <Interaction/Items/Ashley_Pickup.h>
#include "Interaction/Items/Ashley_Torch.h"
#include "CineCameraComponent.h"


AAshley_CharacterPlayer::AAshley_CharacterPlayer()
{
	PrimaryActorTick.bCanEverTick = true;

	InventoryComponent = CreateDefaultSubobject<UAshley_InventoryComponent>(TEXT("InventoryComponent"));
	DialogueComponent = CreateDefaultSubobject<UAshley_DialogueComponent>(TEXT("DialogueComponent"));
	///InventoryComponent->RegisterComponent();

	MaxVelocityRun = 600.f;
	MaxVelocityWalk = 300.f;

	TraceDistance = 300.f;
	Radius = 30.f;
}

void AAshley_CharacterPlayer::BeginPlay()
{
	Super::BeginPlay();

	bShouldTrace = false;
	bHasTorchEquipped = false;
	CurrentInteractable = nullptr;

	USkeletalMeshComponent* MeshComp = GetMesh();
	AnimInstance = MeshComp ? MeshComp->GetAnimInstance() : nullptr;

	TArray<AActor*> OutActors;
	UGameplayStatics::GetAllActorsOfClass(this, AAshley_ItemShowcase::StaticClass(), OutActors);

	if (!OutActors.IsEmpty())
		ItemShowcase = Cast<AAshley_ItemShowcase>(OutActors[0]);

	GetCharacterMovement()->MaxWalkSpeed = MaxVelocityWalk;
}

void AAshley_CharacterPlayer::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);

	PlayerControllerRef = Cast<AAshley_PlayerController>(NewController);
	checkf(PlayerControllerRef, TEXT("PlayerController not found"));

	// Setto riferimento del personaggio e del suo inventario
	UGameplayStatics::GetGameInstance(this)->GetSubsystem<UAshley_WidgetSystem>()->SetPlayerRef(PlayerControllerRef);
	UGameplayStatics::GetGameInstance(this)->GetSubsystem<UAshley_WidgetSystem>()->SetInventoryUI(InventoryBP);

	// TODO: migliorare, tramite level blueprint?
	TArray<AActor*> OutActors;
	UGameplayStatics::GetAllActorsOfClass(this, AAshley_QuestManager::StaticClass(), OutActors);

	if (!OutActors.IsEmpty())
	{
		QuestManager = Cast<AAshley_QuestManager>(OutActors[0]);
		// TODO: temp
		//QuestManager->AdvanceQuest(FName("FindCamera"));
	}
}

void AAshley_CharacterPlayer::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (bShouldTrace)
	{
		// Sposto lo start della trace, per non interagire con oggetti dietro di me
		float Offset = Radius / 2;
		//auto MyCharacterMesh = GetMesh();
		FVector Start = this->GetActorLocation() + GetActorForwardVector() * Offset;
		FVector End = Start + (GetActorForwardVector() * TraceDistance);

		TArray<FHitResult> HitResult;

		bool bHit = GetWorld()->SweepMultiByChannel(HitResult, Start, End, FQuat::Identity, ECC_GameTraceChannel1, FCollisionShape::MakeSphere(Radius));
		
		// DEBUG
		//DrawDebugSphere(
		//	GetWorld(),
		//	Start,
		//	Radius,
		//	16,
		//	FColor::Green,
		//	false,
		//	2.0f
		//);

		//// Draw end sphere
		//DrawDebugSphere(
		//	GetWorld(),
		//	End,
		//	Radius,
		//	16,
		//	FColor::Green,
		//	false,
		//	2.0f
		//);

		//// Draw line between them
		//DrawDebugLine(
		//	GetWorld(),
		//	Start,
		//	End,
		//	FColor::Green,
		//	false,
		//	2.0f,
		//	0,
		//	1.0f
		//);

		//for (const FHitResult& Hit : HitResult)
		//{
		//	DrawDebugPoint(
		//		GetWorld(),
		//		Hit.ImpactPoint,
		//		10.f,
		//		FColor::Red,
		//		false,
		//		2.0f
		//	);
		//}
		

		if (bHit)
		{
			for (auto& Hit : HitResult)
			{
				AActor* HitActor = Hit.GetActor();
				if (HitActor->Implements<UAshley_InteractInterface>() && 
					InteractablesInRange.Contains(HitActor) &&
					Cast<IAshley_InteractInterface>(HitActor)->IsInteractable())
				{
					bCanInteract = true;

					// Se l'oggetto non è cambiato non devo fare nulla
					if (CurrentInteractable == HitActor) return;

					// Nascondo il prompt dell'oggetto precedente, se non l'ho già fatto
					if (CurrentInteractable) CurrentInteractable->HideInteractionPrompt();

					CurrentInteractable.SetObject(HitActor);
					CurrentInteractable.SetInterface(Cast<IAshley_InteractInterface>(HitActor));

					//CurrentInteractable->GetInteractMessage();
					CurrentInteractable->ShowInteractionPrompt();

					return;
				}
			}
			// Sto guardando un interactable, ma non sono ancora entrato nella sua area di interact
			UGameplayStatics::GetGameInstance(this)->GetSubsystem<UAshley_WidgetSystem>()->HideInteractionPrompt();
			bCanInteract = false;
			CurrentInteractable = nullptr;
		}
		// Non sto guardando nessuno degli interactables
		else
		{
			UGameplayStatics::GetGameInstance(this)->GetSubsystem<UAshley_WidgetSystem>()->HideInteractionPrompt();
			bCanInteract = false;
			CurrentInteractable = nullptr;
		}
	}
}

void AAshley_CharacterPlayer::PutObjectInHand(AActor* ObjectToAttach)
{
	ObjectInHand = ObjectToAttach;

	USkeletalMeshComponent* SkeletalMeshComp = GetMesh();

	ObjectToAttach->AttachToComponent(
		SkeletalMeshComp,
		FAttachmentTransformRules::SnapToTargetNotIncludingScale,
		FName("RightHandSocket")
	);
}

void AAshley_CharacterPlayer::EquipTorchInHand(AAshley_Torch* Torch)
{
	TorchCarried = Torch;
	PutObjectInHand(Torch);
	bHasTorchEquipped = true;
}

void AAshley_CharacterPlayer::ToggleTorch(bool bIsTorchOn)
{
	bHasTorchEquipped = bIsTorchOn;
	if (bIsTorchOn) TorchCarried->TurnOn();
	else TorchCarried->TurnOff();
}

void AAshley_CharacterPlayer::SetTorch(AAshley_Torch* Torch)
{
	TorchCarried = Torch;
}

void AAshley_CharacterPlayer::PutObjectInOffHand(AActor* ObjectToAttach)
{
	USkeletalMeshComponent* SkeletalMeshComp = GetMesh();

	ObjectToAttach->AttachToComponent(
		SkeletalMeshComp,
		FAttachmentTransformRules::SnapToTargetNotIncludingScale,
		FName("LeftHandSocket")
	);
}

void AAshley_CharacterPlayer::Interact()
{
	//Debug::Print("Interact pressed");
	if (bCanInteract && CurrentInteractable) 
		IAshley_InteractInterface::Execute_Interact(CurrentInteractable.GetObject());
}

void AAshley_CharacterPlayer::HideObjectsInHand(bool bHide)
{
	if (ItemCarried) ItemCarried->SetActorHiddenInGame(bHide);
	if (ObjectInHand) ObjectInHand->SetActorHiddenInGame(bHide);
}

void AAshley_CharacterPlayer::ToggleActorActive(bool bActive)
{
	// Hides visible components
	SetActorHiddenInGame(!bActive);

	// Disables collision components
	SetActorEnableCollision(bActive);

	// Stops the Actor from ticking
	SetActorTickEnabled(bActive);
}

void AAshley_CharacterPlayer::InteractAreaTriggered(bool bEntered, TScriptInterface<IAshley_InteractInterface> Interactable)
{
	// Quando entro nell'area di un interactable
	//  lo aggiungo alla lista
	if (bEntered)
	{
		bShouldTrace = true;
		InteractablesInRange.Add(Interactable);

		//Debug::Print(FString::Printf(TEXT("Interactablesin range %d,"), InteractablesInRange.Num()));
	}
	else
	{
		// Quando esco dalla sua area smetto di considerarlo,
		//  e se non ho altri interactables in range, smetto di cercare con il trace
		InteractablesInRange.Remove(Interactable);
		Interactable->HideInteractionPrompt();

		if (InteractablesInRange.IsEmpty())
		{
			bShouldTrace = false;
			bCanInteract = false;
			CurrentInteractable = nullptr;
			//UGameplayStatics::GetGameInstance(this)->GetSubsystem<UAshley_WidgetSystem>()->HideInteractionPrompt();
		}
	}
}

void AAshley_CharacterPlayer::PickupItem(FName PickupItem, FInventoryInfo InventoryInfo, int StackSize)
{
	InventoryComponent->PutObjectInventoryWithInfo(PickupItem, InventoryInfo, StackSize);

	ItemsToShowcase.Enqueue(PickupItem);

	if (!bIsShowcaseShowing) OpenItemShowcase();
}

void AAshley_CharacterPlayer::OpenItemShowcase()
{
	if (!bIsShowcaseShowing)
	{
		// zoom dell'oggetto
		auto CurrCamera = PlayerControllerRef->GetCameraManager()->GetCurrentCamera();
		LastCamera = CurrCamera->GetCameraComponent();
		auto CameraLocation = LastCamera->GetComponentLocation();

		// Attivo showcase
		ItemShowcase->SetActorLocation(CameraLocation + (LastCamera->GetForwardVector() * 50));
		ItemShowcase->SetActorRotation(LastCamera->GetComponentRotation());
		//ItemShowcase->SetOverscan(LastCamera->Overscan);

		// metto in pausa, disabilito input normali
		FTimerHandle TimerHandle;
		GetWorldTimerManager().SetTimer(TimerHandle, [this]()
			{
				PlayerControllerRef->SetPause(true);
			}, 0.1f, false);
		PlayerControllerRef->SetMappingContextByName(EAshley_InputMappingContext::EXAMINING_OBJECT);
	}

	bIsShowcaseShowing = true;

	FName ItemToShow;
	ItemsToShowcase.Dequeue(ItemToShow);
	ItemShowcase->ShowItem(ItemToShow);

	// TODO: spostare dialogo qui
	//OnPlayerItemPickup(ItemToShow);
	AAshley_Pickup::OnItemPickup.Broadcast(ItemToShow);
}

void AAshley_CharacterPlayer::RotateShowcaseItem(FVector2D Delta)
{
	if (!bIsShowcaseShowing || !IsValid(ItemShowcase)) return;

	ItemShowcase->RotateItem(Delta);
}

void AAshley_CharacterPlayer::StopExaminingObject()
{
	//Debug::Print("Arrivato stop!");

	if (!ItemsToShowcase.IsEmpty())
	{
		OpenItemShowcase();
		return;
	}

	ItemShowcase->CloseShowcase();

	// Ripristino l'ultima camera
	if (IsValid(LastCamera) && IsValid(LastCamera->GetOwner()))
		PlayerControllerRef->SetViewTarget(LastCamera->GetOwner());
	PlayerControllerRef->SetPause(false);
	PlayerControllerRef->SetMappingContextByName(EAshley_InputMappingContext::GAMEPLAY);

	bIsShowcaseShowing = false;

	UGameplayStatics::GetGameInstance(this)->GetSubsystem<UAshley_EventManagerSystem>()->BroadcastEventTriggered(AshleyGameplayTags::Ashley_Events_ShowcaseClosed, FString());
}

void AAshley_CharacterPlayer::CarryItem(AAshley_CarryItem* ItemToCarry)
{
	ItemCarried = ItemToCarry;
	InventoryComponent->AddCarriedItem(ItemCarried->GetID());
	PutObjectInOffHand(ItemCarried);

	// Modifico inputMapping per permettere il drop dell'item
	PlayerControllerRef->AddMappingContextByName(EAshley_InputMappingContext::CARRYING_OBJECT);
}

void AAshley_CharacterPlayer::DropItem()
{
	if (!IsValid(ItemCarried)) return;

	InventoryComponent->RemoveCarriedItem();
	// Lascio cadere l'oggetto
	FDetachmentTransformRules rules(EDetachmentRule::KeepWorld, true);
	ItemCarried->DetachFromActor(rules);
	ItemCarried->Drop();
	ItemCarried = nullptr;

	// Modifico inputMapping per permettere la corsa
	PlayerControllerRef->RemoveMappingContextByName(EAshley_InputMappingContext::CARRYING_OBJECT);
}

void AAshley_CharacterPlayer::UseCarriedItem()
{
	if (!IsValid(ItemCarried)) return;

	InventoryComponent->RemoveCarriedItem();

	if (ItemCarried->IsDestroyOnUse())
	{
		FDetachmentTransformRules rules(EDetachmentRule::KeepWorld, true);
		ItemCarried->DetachFromActor(rules);
		ItemCarried->Destroy();
		ItemCarried = nullptr;

		PlayerControllerRef->RemoveMappingContextByName(EAshley_InputMappingContext::CARRYING_OBJECT);
	}
	else
	{
		DropItem();
	}
}

void AAshley_CharacterPlayer::Sprint(bool bSprint)
{
	if (bSprint)
	{
		GetCharacterMovement()->MaxWalkSpeed = MaxVelocityRun;
	}
	else GetCharacterMovement()->MaxWalkSpeed = MaxVelocityWalk;
}

TArray<FItemStack> AAshley_CharacterPlayer::GetItemsInInventory()
{
	return InventoryComponent->GetAllItems();
}

TArray<FItemStack> AAshley_CharacterPlayer::GetCollectables()
{
	return InventoryComponent->GetCollectables();
}

AAshley_QuestManager* AAshley_CharacterPlayer::GetQuestManager()
{
	return QuestManager;
}

UAshley_InventoryComponent* AAshley_CharacterPlayer::GetInventory()
{
	return InventoryComponent;
}

