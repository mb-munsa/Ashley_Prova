// Ashley


#include "Core/Ashley_TriggerCollider.h"

#include "Components/BoxComponent.h"
#include <Kismet/GameplayStatics.h>

#include <LevelSequenceActor.h>
#include <LevelSequencePlayer.h>
#include <MovieSceneSequencePlayer.h>

#include "Characters/Players/Ashley_CharacterPlayer.h"
#include "Core/Ashley_PlayerController.h"
#include <Managers/Ashley_WidgetSystem.h>
#include <Managers/Ashley_EventManagerSystem.h>

// Sets default values
AAshley_TriggerCollider::AAshley_TriggerCollider()
{
	PrimaryActorTick.bCanEverTick = false;

	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("DefaultSceneRoot"));

	TriggerVolume = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxVolume"));
	TriggerVolume->SetupAttachment(RootComponent);
}

// Called when the game starts or when spawned
void AAshley_TriggerCollider::BeginPlay()
{
	Super::BeginPlay();

	// Aggiunto tutti i Box come trigger
	TArray<UBoxComponent*> TriggerVolumes;
	this->GetComponents<UBoxComponent>(TriggerVolumes);

	for (const auto& trigger : TriggerVolumes)
	{
		trigger->OnComponentBeginOverlap.AddDynamic(this, &AAshley_TriggerCollider::OnBoxOverlap);
		trigger->OnComponentHit.AddDynamic(this, &AAshley_TriggerCollider::OnComponentHit);
	}
}

void AAshley_TriggerCollider::OnBoxOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (!bIsActive) return;

	AAshley_CharacterPlayer* Player = Cast<AAshley_CharacterPlayer>(OtherActor);

	if (!Player) return;

	OnColliderTrigger.Broadcast();

	if (bShouldTriggerEvent && (!bHasAlreadySentEvent || !bIsEventOneShot))
	{
		UGameplayStatics::GetGameInstance(this)->GetSubsystem<UAshley_EventManagerSystem>()->BroadcastEventTriggered(EventName, EventParam);
		bHasAlreadySentEvent = true;
	}

	// TODO: generalizzare questo in un component (vedi Interactable)
	if (bShouldShowDialogue && (!bHasAlreadyShownDialogue || !bIsDialogueOneTime))
	{
		auto PlayerController = Cast<AAshley_PlayerController>(Player->GetController());
		UGameplayStatics::GetGameInstance(this)->GetSubsystem<UAshley_WidgetSystem>()->SpawnInfoText(DialogueText, 2.5f);
		bHasAlreadyShownDialogue = true;
	}
}

void AAshley_TriggerCollider::OnComponentHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	if (!bIsActive) return;

	AAshley_CharacterPlayer* Player = Cast<AAshley_CharacterPlayer>(OtherActor);

	if (!Player) return;

	OnColliderTrigger.Broadcast();

	if (bShouldTriggerEvent && (!bHasAlreadySentEvent || !bIsEventOneShot))
	{
		UGameplayStatics::GetGameInstance(this)->GetSubsystem<UAshley_EventManagerSystem>()->BroadcastEventTriggered(EventName, EventParam);
		bHasAlreadySentEvent = true;
	}

	// TODO: generalizzare questo in un component (vedi Interactable)
	if (bShouldShowDialogue && (!bHasAlreadyShownDialogue || !bIsDialogueOneTime))
	{
		auto PlayerController = Cast<AAshley_PlayerController>(Player->GetController());
		UGameplayStatics::GetGameInstance(this)->GetSubsystem<UAshley_WidgetSystem>()->SpawnInfoText(DialogueText, 2.5f);
		bHasAlreadyShownDialogue = true;
	}
}

void AAshley_TriggerCollider::SetActive(bool bSetActive)
{
	this->bIsActive = bSetActive;

	this->SetActorEnableCollision(bIsActive);
}