// Ashley

#pragma once

#include "CoreMinimal.h"
#include "Characters/Ashley_CharacterBase.h"
#include "Ashley_NPC.generated.h"

class AAshley_CharacterPlayer;
class AAshley_FixedCameraController;
class UAshley_InteractionPrompt;
class UAshley_GenericDialogue;
class UAshley_DialogueNode;
class UBoxComponent;

/**
 * 
 */
UCLASS()
class ASHLEY_API AAshley_NPC : public AAshley_CharacterBase, public IAshley_InteractInterface
{
	GENERATED_BODY()

public:
	AAshley_NPC();

	virtual void Interact_Implementation() override;

	virtual FString GetInteractMessage() const override;

	// TODO: rimuovere
	UFUNCTION(BlueprintNativeEvent)
	void GoToCheckElectricalPanel();
	virtual void GoToCheckElectricalPanel_Implementation() {}

	virtual void ShowInteractionPrompt() const;

	virtual void HideInteractionPrompt() const;

protected:
	virtual void BeginPlay() override;

	UFUNCTION(BlueprintCallable)
	virtual void StartDialogue(UAshley_DialogueNode* DialogueHead);

	UFUNCTION(BlueprintCallable)
	virtual void EndDialogue();

	UFUNCTION(BlueprintCallable)
	virtual void OnDialogueEnded();

	UFUNCTION(BlueprintImplementableEvent)
	void OnDialogueEndedEvent();

	UFUNCTION()
	virtual void OnBoxInteractOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	UFUNCTION()
	virtual void OnBoxInteractEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data|Collision")
	bool bIgnoreCollision;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category = "Data|Collision")
	TObjectPtr<UBoxComponent> BoxInteract;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category = "Data")
	TObjectPtr<UAudioComponent> AudioComponent;

	UPROPERTY()
	TObjectPtr<AAshley_CharacterPlayer> CharacterInRange;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data|Interaction")
	bool bIsSitting;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data|Interaction")
	bool bIsTalking;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data|Interaction")
	bool bIsOpenToTalk;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data|Interaction")
	TObjectPtr<AAshley_FixedCameraController> CameraDialogue;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data|Interaction")
	TObjectPtr<UAshley_InteractionPrompt> InteractionWidget;
};
