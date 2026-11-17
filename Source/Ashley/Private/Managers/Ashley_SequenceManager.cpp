// Ashley


#include "Managers/Ashley_SequenceManager.h"
#include "Managers/Ashley_WidgetSystem.h"

#include <LevelSequenceActor.h>
#include <LevelSequencePlayer.h>
#include <MovieSceneSequencePlayer.h>

#include "Core/Ashley_TriggerCollider.h"
#include "Core/Ashley_PlayerController.h"
#include "DataAssets/Ashley_SequencesDA.h"
#include "DataAssets/Ashley_InputConfigDA.h"

#include <Kismet/GameplayStatics.h>
#include <Managers/Ashley_EventManagerSystem.h>

// Sets default values
AAshley_SequenceManager::AAshley_SequenceManager()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

}

// Called when the game starts or when spawned
void AAshley_SequenceManager::BeginPlay()
{
	Super::BeginPlay();

    auto EventManager = UGameplayStatics::GetGameInstance(this)->GetSubsystem<UAshley_EventManagerSystem>();
    EventManager->OnEventTriggered.AddDynamic(this, &AAshley_SequenceManager::OnPlayerEventTriggered);
    
    for (int i = 0; i < LevelSequences->Sequences.Num(); i++)
    {
        hasSequenceBeenShown.Add(LevelSequences->Sequences[i].SequenceName, false);
    }
}

void AAshley_SequenceManager::OnPlayerEventTriggered(FGameplayTag EventName, FString EventParameter)
{
    if (EventName.GetTagName() == "Ashley.Events.PlaySequence")
    {
        FGameplayTag SequenceName = FGameplayTag::RequestGameplayTag(FName(*EventParameter));
        PlaySequence(SequenceName);
    }
    else if (EventName.GetTagName() == "Ashley.Event.SkipSequence")
    {
        SkipSequence();
    }
}

void AAshley_SequenceManager::PlaySequence(FGameplayTag SequenceName)
{
    FSequence* SequenceToPlay = LevelSequences->Sequences.FindByKey(SequenceName);

    // Non esiste la sequence con questo nome
    if (SequenceToPlay == nullptr) return;

    // Se ho mostrato la sequence esco
    if (hasSequenceBeenShown[SequenceName]) return;

    // Per sicurezza nascondo eventuali infoText
    UGameplayStatics::GetGameInstance(this)->GetSubsystem<UAshley_WidgetSystem>()->EndDialogue();

    // Setto la sequenza come mostrata
    hasSequenceBeenShown[SequenceName] = true;
    auto& LevelSequence = SequenceToPlay->SequenceObject;

    if (!LevelSequence)
    {
        UE_LOG(LogTemp, Warning, TEXT("LevelSequence not loaded"));
        return;
    }

    // disabilita player e input
    //  e riabilita dopo tot
    if (!PlayerController)
    {
        PlayerController = Cast<AAshley_PlayerController>(UGameplayStatics::GetPlayerController(GetWorld(), 0));
    }

    PlayerController->SetMappingContextByName(EAshley_InputMappingContext::CUTSCENE);
    PlayerController->ToggleInputBlock(true);
    PlayerController->ToggleHidePlayer(true);

    ALevelSequenceActor* SequenceActor = nullptr;

    FMovieSceneSequencePlaybackSettings Settings;
    Settings.bAutoPlay = false;
    //Settings.LoopCount = 0; // 0 = play once

    Player = ULevelSequencePlayer::CreateLevelSequencePlayer(
            GetWorld(),
            LevelSequence,
            Settings,
            SequenceActor
        );

    if (!Player) return;

    CurrentSequenceName = SequenceName;

    // Send EndSequence only once the player has really finished (after it restored
    //  the pre-cutscene view target)
    Player->OnFinished.AddUniqueDynamic(this, &AAshley_SequenceManager::OnSequencePlayerFinished);
    Player->Play();
}

void AAshley_SequenceManager::OnSequencePlayerFinished()
{
    if (Player) Player->OnFinished.RemoveDynamic(this, &AAshley_SequenceManager::OnSequencePlayerFinished);

    OnSequenceEnd(CurrentSequenceName);
}

void AAshley_SequenceManager::OnSequenceEnd(FGameplayTag SequenceName)
{
    PlayerController->ToggleInputBlock(false);
    PlayerController->ToggleHidePlayer(false);

    PlayerController->SetMappingContextByName(EAshley_InputMappingContext::GAMEPLAY);

    // Per sicurezza nascondo eventuali infoText
    UGameplayStatics::GetGameInstance(this)->GetSubsystem<UAshley_WidgetSystem>()->EndDialogue();
    UGameplayStatics::GetGameInstance(this)->GetSubsystem<UAshley_EventManagerSystem>()->BroadcastEventTriggered(
            FGameplayTag::RequestGameplayTag(FName("Ashley.Events.EndSequence")), SequenceName.ToString());
}

void AAshley_SequenceManager::SkipSequence()
{
    // No sequence playing (callback already consumed or never bound)
    if (!Player || !Player->OnFinished.IsAlreadyBound(this, &AAshley_SequenceManager::OnSequencePlayerFinished)) return;

    // Unbind first so the end event is not sent twice, then stop: GoToEndAndStop
    //  restores the sequence state synchronously, before OnSequenceEnd runs
    Player->OnFinished.RemoveDynamic(this, &AAshley_SequenceManager::OnSequencePlayerFinished);
    Player->GoToEndAndStop();

    OnSequenceEnd(CurrentSequenceName);
}

