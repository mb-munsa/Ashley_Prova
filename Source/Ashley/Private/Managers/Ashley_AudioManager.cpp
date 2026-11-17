// Ashley


#include "Managers/Ashley_AudioManager.h"

#include "Components/AudioComponent.h"

#include "DebugHelper.h"

void UAshley_AudioManager::Initialize(FSubsystemCollectionBase& Collection)
{
    Super::Initialize(Collection);

    UWorld* World = GetGameInstance()->GetWorld();
    if (!World) return;

    // Manually bind to level transition events
    FCoreUObjectDelegates::PostLoadMapWithWorld.AddUObject(this, &UAshley_AudioManager::OnPostLoadMap);

    // Spawno per testare direttamente il livello senza caricamenti
#if WITH_EDITOR
    SpawnPool(World);
#endif
}

void UAshley_AudioManager::OnPostLoadMap(UWorld* LoadedWorld)
{
    //Debug::Print("Post load new level");

    if (OwnerActor)
        OwnerActor->Destroy();
    
    SpawnPool(LoadedWorld);
}

void UAshley_AudioManager::SpawnPool(UWorld* CurrentWorld)
{
    Pool.Reset();

    // Actor contenente la pool di audioComponents
    FActorSpawnParameters Params;
    OwnerActor = CurrentWorld->SpawnActor<AActor>(AActor::StaticClass(), Params);
    OwnerActor->SetActorHiddenInGame(true);

    // Inizializzo la pool
    for (int32 i = 0; i < PoolSize; i++)
    {
        Pool.Add(CreateNewComponent());
    }
}

UAudioComponent* UAshley_AudioManager::CreateNewComponent()
{
    UAudioComponent* AC = NewObject<UAudioComponent>(OwnerActor);
    AC->bAutoActivate = false;
    AC->bAutoDestroy = false;
    AC->RegisterComponent();
    return AC;
}

bool UAshley_AudioManager::PlaySoundEffect(USoundBase* SFX, float Volume, bool bPlayWhilePaused)
{
    for (auto audioComponent : Pool)
    {
        if (!audioComponent->IsPlaying())
        {
            audioComponent->SetSound(SFX);
            audioComponent->SetVolumeMultiplier(Volume);
            // I suoni UI non vengono messi in pausa quando il gioco è in pausa.
            // Va reimpostato ad ogni Play perché i componenti della pool vengono riutilizzati.
            audioComponent->bIsUISound = bPlayWhilePaused;
            audioComponent->Play();
            return true;
        }
    }
    return false;
}

void UAshley_AudioManager::Deinitialize()
{
    if (OwnerActor)
    {
        OwnerActor->Destroy();
        OwnerActor = nullptr;
    }
    Pool.Empty();
    Super::Deinitialize();
}