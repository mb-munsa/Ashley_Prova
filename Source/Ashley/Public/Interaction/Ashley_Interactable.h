// Ashley

#pragma once

#include "CoreMinimal.h"
#include "Interaction/Ashley_ActorInteract.h"
#include "Widgets/Ashley_ChoiceMenu.h"
#include "Ashley_Interactable.generated.h"

class UAshley_EventManagerSystem;
class UAshley_WidgetSystem;
class AAshley_FixedCameraController;

struct FDialogueChoice;

/**
 * Classe generica per interactable che mostrano un dialogo
 */
UCLASS()
class ASHLEY_API AAshley_Interactable : public AAshley_ActorInteract
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AAshley_Interactable();

	virtual void Interact_Implementation() override;

	UFUNCTION(BlueprintCallable)
	virtual void OpenInventory();

	UFUNCTION(BlueprintCallable)
	virtual void OpenChoiceMenu(TArray<FDialogueChoice> ChoicesToShow);

	UFUNCTION(BlueprintCallable)
	void HideChoiceMenu();

	UFUNCTION(BlueprintImplementableEvent)
	void OnChoiceSelected(int ChoiceIndex);

	UFUNCTION()
	virtual void OnInventoryClosed();

	UFUNCTION(BlueprintImplementableEvent)
	void OnItemSelected(FName ItemName);

	UFUNCTION(BlueprintCallable)
	void SetZoomInCamera();

	UFUNCTION(BlueprintCallable)
	void ResetCamera();

protected:

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Interactable")
	UStaticMeshComponent* Mesh;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data|Dialogues")
	bool bShouldTriggerDialogue;

	// TODO: migliorare, creare lista e prenderli da DataAsset
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data|Dialogues")
	FString Dialogue;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data|Dialogues")
	bool bIsDialogueOneTime;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data|Dialogues")
	bool bDisableAfterDialogue;

	bool bHasAlreadyShownDialogue = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data|Camera")
	TObjectPtr<AAshley_FixedCameraController> ZoomInCamera;

	TObjectPtr<AAshley_FixedCameraController> LastCamera;

	UAshley_WidgetSystem* WidgetManager;

	UAshley_EventManagerSystem* EventManager;

	TObjectPtr<UAshley_ChoiceMenu> ChoiceMenu;


	// TODO: aggiungere possibilità di muovere telecamera,
	//  riprodurre suoni, etc?
};
