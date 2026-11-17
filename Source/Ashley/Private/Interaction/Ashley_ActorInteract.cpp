// Fill out your copyright notice in the Description page of Project Settings.


#include "Interaction/Ashley_ActorInteract.h"

#include "Components/BoxComponent.h"
#include "Interaction/Ashley_InteractInterface.h"
#include "Kismet/GameplayStatics.h"

#include "Characters/Players/Ashley_CharacterPlayer.h"
#include "Components/Ashley_InteractionPrompt.h"

#include "DebugHelper.h"


// Sets default values
AAshley_ActorInteract::AAshley_ActorInteract()
{
	PrimaryActorTick.bCanEverTick = true;
	PrimaryActorTick.bStartWithTickEnabled = true;

	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("DefaultSceneRoot"));

	BoxInteract = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxInteract"));
	BoxInteract->SetupAttachment(RootComponent);

	InteractionWidget = CreateDefaultSubobject<UAshley_InteractionPrompt>(TEXT("InteractionPrompt"));
	InteractionWidget->SetupAttachment(RootComponent);
	
	bIgnoreCollision = false;
}

void AAshley_ActorInteract::Interact_Implementation() {}

void AAshley_ActorInteract::ShowInteractionPrompt() const
{
	InteractionWidget->Show();
}

void AAshley_ActorInteract::HideInteractionPrompt() const
{
	InteractionWidget->Hide();
}

// Called when the game starts or when spawned
void AAshley_ActorInteract::BeginPlay()
{
	Super::BeginPlay();
	BoxInteract->OnComponentBeginOverlap.AddDynamic(this, &AAshley_ActorInteract::OnBoxInteractOverlap);
	BoxInteract->OnComponentEndOverlap.AddDynamic(this, &AAshley_ActorInteract::OnBoxInteractEndOverlap);

	InteractionWidget->SetText(GetInteractMessage());
}

FString AAshley_ActorInteract::GetInteractMessage() const
{
	return "Interact";
}

bool AAshley_ActorInteract::IsInteractable() const
{
	return !bIgnoreCollision;
}

void AAshley_ActorInteract::ToggleInteraction(bool bShoulEnable)
{
	bIgnoreCollision = !bShoulEnable;
	
	if (CharacterInRange)
		CharacterInRange->InteractAreaTriggered(bShoulEnable, this);
}

void AAshley_ActorInteract::PlaySoundIndex(int32 Index)
{
	 if (!SoundList.IsValidIndex(Index))
	{
		UE_LOG(LogTemp, Warning, TEXT("PlaySoundAtIndex: Index %d not valid"), Index);
		return;
	}

	USoundBase* SoundToPlay = SoundList[Index];
	if (!SoundToPlay)
	{
		UE_LOG(LogTemp, Warning, TEXT("PlaySoundAtIndex: index %d null"), Index);
		return;
	}

	UGameplayStatics::PlaySoundAtLocation(this,	SoundToPlay, GetActorLocation()); 
}

void AAshley_ActorInteract::OnBoxInteractOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (bIgnoreCollision) return;

	CharacterInRange = Cast<AAshley_CharacterPlayer>(OtherActor);

	if (CharacterInRange)
		CharacterInRange->InteractAreaTriggered(true, this);
}

void AAshley_ActorInteract::OnBoxInteractEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (bIgnoreCollision) return;

	CharacterInRange = Cast<AAshley_CharacterPlayer>(OtherActor);

	if (CharacterInRange)
	{
		CharacterInRange->InteractAreaTriggered(false, this);
		CharacterInRange = nullptr;
	}
}
