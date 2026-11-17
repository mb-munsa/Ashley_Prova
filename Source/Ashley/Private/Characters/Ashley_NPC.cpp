// Ashley


#include "Characters/Ashley_NPC.h"

#include "Components/BoxComponent.h"
#include "Components/AudioComponent.h"

#include "Characters/Players/Ashley_CharacterPlayer.h"
#include "Managers/Ashley_WidgetSystem.h"
#include "DataAssets/Ashley_GenericDialogue.h"
#include "DataAssets/Ashley_InputConfigDA.h"
#include "Components/Ashley_CameraManager.h"
#include "Components/Ashley_InteractionPrompt.h"
#include "Core/Ashley_FixedCameraController.h"
#include "Core/Ashley_PlayerController.h"
#include "Core/Ashley_DialogueComponent.h"

#include "DebugHelper.h"
#include <Kismet/GameplayStatics.h>


// Sets default values
AAshley_NPC::AAshley_NPC()
{
	PrimaryActorTick.bCanEverTick = true;
	PrimaryActorTick.bStartWithTickEnabled = true;

	//RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("DefaultSceneRoot"));

	BoxInteract = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxInteract"));
	BoxInteract->SetupAttachment(RootComponent);

	//AudioComponent = CreateDefaultSubobject<UAudioComponent>(TEXT("AudioSource"));
	//AudioComponent->SetupAttachment(RootComponent);

	InteractionWidget = CreateDefaultSubobject<UAshley_InteractionPrompt>(TEXT("InteractionPrompt"));
	InteractionWidget->SetupAttachment(RootComponent);

	bIgnoreCollision = false;
}

void AAshley_NPC::BeginPlay()
{
	Super::BeginPlay();
	BoxInteract->OnComponentBeginOverlap.AddDynamic(this, &AAshley_NPC::OnBoxInteractOverlap);
	BoxInteract->OnComponentEndOverlap.AddDynamic(this, &AAshley_NPC::OnBoxInteractEndOverlap);
	InteractionWidget->SetText(GetInteractMessage());
}

FString AAshley_NPC::GetInteractMessage() const
{
	return "Talk";
}

void AAshley_NPC::ShowInteractionPrompt() const
{
	InteractionWidget->Show();
}

void AAshley_NPC::HideInteractionPrompt() const
{
	InteractionWidget->Hide();
}

void AAshley_NPC::StartDialogue(UAshley_DialogueNode* DialogueHead)
{
	bIsTalking = true;

	auto PlayerController = Cast<AAshley_PlayerController>(UGameplayStatics::GetPlayerController(GetWorld(), 0));
	auto Character = Cast<AAshley_CharacterPlayer>(PlayerController->GetCharacter());
	
	if (CameraDialogue)
		PlayerController->GetCameraManager()->AddToCameraStack(CameraDialogue);

	auto DialogueComponent = Character->GetComponentByClass<UAshley_DialogueComponent>();
	DialogueComponent->Play(DialogueHead);
	DialogueComponent->OnDialogueEnded.AddDynamic(this, &AAshley_NPC::OnDialogueEnded);

	PlayerController->SetMappingContextByName(EAshley_InputMappingContext::DIALOGUE);
	InteractionWidget->Hide();
}

void AAshley_NPC::EndDialogue()
{
	bIsTalking = false;

	auto PlayerController = Cast<AAshley_PlayerController>(UGameplayStatics::GetPlayerController(GetWorld(), 0));
	auto Character = Cast<AAshley_CharacterPlayer>(PlayerController->GetCharacter());

	if (CameraDialogue)
		PlayerController->GetCameraManager()->RemoveFromCameraStack(CameraDialogue);

	PlayerController->SetMappingContextByName(EAshley_InputMappingContext::GAMEPLAY);
	auto DialogueComponent = Character->GetComponentByClass<UAshley_DialogueComponent>();
	DialogueComponent->OnDialogueEnded.RemoveDynamic(this, &AAshley_NPC::OnDialogueEnded);

	OnDialogueEndedEvent();
	if (CharacterInRange) InteractionWidget->Show();
}

void AAshley_NPC::OnDialogueEnded()
{
	if (bIsTalking)
		EndDialogue();
}

void AAshley_NPC::OnBoxInteractOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (bIgnoreCollision) return;

	CharacterInRange = Cast<AAshley_CharacterPlayer>(OtherActor);

	if (CharacterInRange)
		CharacterInRange->InteractAreaTriggered(true, this);
}

void AAshley_NPC::OnBoxInteractEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (bIgnoreCollision) return;

	CharacterInRange = Cast<AAshley_CharacterPlayer>(OtherActor);

	if (CharacterInRange)
	{
		CharacterInRange->InteractAreaTriggered(false, this);
		CharacterInRange = nullptr;
	}
}

void AAshley_NPC::Interact_Implementation()
{
	//if (bIsOpenToTalk)
	//{
	//	bIsTalking = !bIsTalking;

	//	Debug::Print("TALK TO ME");

	//	if (bIsTalking)
	//	{
	//		auto PlayerController = Cast<AAshley_PlayerController>(CharacterInRange->GetController());
	//		PlayerController->GetCameraManager()->AddToCameraStack(CameraDialogue);

	//		MainDialogue->PlayDialogue(GetWorld()->GetGameInstance(), 0);

	//		// TODO: modificare context mapping ( con solo skip dialogue )

	//		PlayerController->ToggleInputBlock(true);
	//	}
	//	else
	//	{
	//		auto PlayerController = Cast<AAshley_PlayerController>(CharacterInRange->GetController());
	//		PlayerController->GetCameraManager()->RemoveFromCameraStack(CameraDialogue);

	//		PlayerController->ToggleInputBlock(false);
	//	}

	//	// Start dialogue -> blocco movimenti giocatore
	//}
}
