// Ashley


#include "Interaction/Ashley_Interactable.h"
#include "Core/Ashley_PlayerController.h"
#include "Core/Ashley_FixedCameraController.h"

#include "Characters/Players/Ashley_CharacterPlayer.h"

#include "Kismet/GameplayStatics.h"
#include <Managers/Ashley_WidgetSystem.h>
#include <Managers/Ashley_EventManagerSystem.h>
#include <Components/Ashley_CameraManager.h>

#include "DataAssets/Ashley_DialogueNode.h"
#include "Widgets/Ashley_ChoiceMenu.h"

#include "DebugHelper.h"

AAshley_Interactable::AAshley_Interactable()
{
	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	Mesh->SetupAttachment(RootComponent);

	// Configuro il layer per essere interagibile dal player
	Mesh->SetCollisionResponseToChannel(ECC_GameTraceChannel1, ECR_Block);
	Mesh->SetRenderCustomDepth(true);
	Mesh->SetCustomDepthStencilValue(0);
}

void AAshley_Interactable::Interact_Implementation()
{
	if (bShouldTriggerDialogue)
	{
		if (!bIsDialogueOneTime || !bHasAlreadyShownDialogue)
		{
			UGameplayStatics::GetGameInstance(this)->GetSubsystem<UAshley_WidgetSystem>()->SpawnInfoText(Dialogue);
			bHasAlreadyShownDialogue = true;

			if (bDisableAfterDialogue) bIgnoreCollision = true;
		}
	}
}

void AAshley_Interactable::OpenInventory()
{
	WidgetManager = UGameplayStatics::GetGameInstance(this)->GetSubsystem<UAshley_WidgetSystem>();
	WidgetManager->OnInventoryClosed.AddDynamic(this, &AAshley_Interactable::OnInventoryClosed);

	EventManager = UGameplayStatics::GetGameInstance(this)->GetSubsystem<UAshley_EventManagerSystem>();
	EventManager->OnItemSelected.AddDynamic(this, &AAshley_Interactable::OnItemSelected);

	WidgetManager->ToggleInventory();
}

void AAshley_Interactable::OnInventoryClosed()
{
	// unbind callbacks
	EventManager->OnItemSelected.Remove(this, FName("OnItemSelected"));
	WidgetManager->OnInventoryClosed.Remove(this, FName("OnInventoryClosed"));
}

void AAshley_Interactable::OpenChoiceMenu(TArray<FDialogueChoice> ChoicesToShow)
{
	WidgetManager = UGameplayStatics::GetGameInstance(this)->GetSubsystem<UAshley_WidgetSystem>();
	ChoiceMenu = WidgetManager->ShowDialogueChoices(ChoicesToShow);

	auto PlayerController = Cast<AAshley_PlayerController>(UGameplayStatics::GetPlayerController(GetWorld(), 0));
	PlayerController->ToggleInputBlock(true);

	// Mi collego all'evento di callback di scelta
	ChoiceMenu->OnChoiceSelected.AddDynamic(this, &AAshley_Interactable::OnChoiceSelected);
}

void AAshley_Interactable::HideChoiceMenu()
{
	WidgetManager = UGameplayStatics::GetGameInstance(this)->GetSubsystem<UAshley_WidgetSystem>();
	WidgetManager->HideChoiceMenu();

	auto PlayerController = Cast<AAshley_PlayerController>(UGameplayStatics::GetPlayerController(GetWorld(), 0));
	PlayerController->ToggleInputBlock(false);

	ChoiceMenu->OnChoiceSelected.RemoveDynamic(this, &AAshley_Interactable::OnChoiceSelected);
}

void AAshley_Interactable::SetZoomInCamera()
{
	if (!ZoomInCamera) return;

	UAshley_CameraManager* CameraManager = UAshley_CameraManager::Get(this);
	LastCamera = CameraManager->GetCurrentCamera();
	CameraManager->SetCurrentCamera(ZoomInCamera);
}

void AAshley_Interactable::ResetCamera()
{
	if (LastCamera)
		UAshley_CameraManager::Get(this)->SetCurrentCamera(LastCamera);
}
