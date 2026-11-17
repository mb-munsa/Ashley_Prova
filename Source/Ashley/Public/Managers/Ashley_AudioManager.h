// Ashley

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "Ashley_AudioManager.generated.h"

/**
 * 
 */
UCLASS()
class ASHLEY_API UAshley_AudioManager : public UGameInstanceSubsystem
{
	GENERATED_BODY()

public:
    virtual void Initialize(FSubsystemCollectionBase& Collection) override;
    virtual void Deinitialize() override;

    void OnPostLoadMap(UWorld* LoadedWorld);

    UFUNCTION(BlueprintCallable)
    bool PlaySoundEffect(USoundBase* SFX, float Volume = 1, bool bPlayWhilePaused = false);

private:
    UAudioComponent* CreateNewComponent();

    void SpawnPool(UWorld* CurrentWorld);

    UPROPERTY()
    AActor* OwnerActor;

    UPROPERTY()
    TArray<UAudioComponent*> Pool;

    int PoolSize = 16;
};