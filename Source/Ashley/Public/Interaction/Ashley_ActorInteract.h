// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"

#include "Interaction/Ashley_InteractInterface.h"

#include "Ashley_ActorInteract.generated.h"

class UBoxComponent;
class AAshley_CharacterPlayer;
class UAshley_InteractionPrompt;

UCLASS(Blueprintable)
class ASHLEY_API AAshley_ActorInteract : public AActor, public IAshley_InteractInterface
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AAshley_ActorInteract();

public:

	virtual void Interact_Implementation() override;

	virtual void ShowInteractionPrompt() const;

	virtual void HideInteractionPrompt() const;
	
	virtual FString GetInteractMessage() const;

	virtual bool IsInteractable() const;

	UFUNCTION(BlueprintCallable)
	void ToggleInteraction(bool bShoulEnable);

	UFUNCTION(BlueprintCallable)
	void PlaySoundIndex(int32 Index);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data|Sound")
	TArray<USoundBase*> SoundList;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data|Collision")
	TObjectPtr<UBoxComponent> BoxInteract;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data|Interaction")
	TObjectPtr<UAshley_InteractionPrompt> InteractionWidget;

	UFUNCTION()
	virtual void OnBoxInteractOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	UFUNCTION()
	virtual void OnBoxInteractEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);
		
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data|Collision")
	bool bIgnoreCollision;

	UPROPERTY()
	TObjectPtr<AAshley_CharacterPlayer> CharacterInRange;

};
