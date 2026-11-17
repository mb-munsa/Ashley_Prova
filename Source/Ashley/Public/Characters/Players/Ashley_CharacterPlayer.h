// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Characters/Ashley_CharacterBase.h"
#include "Interaction/Ashley_ActorInteract.h"

#include "Ashley_CharacterPlayer.generated.h"

class UInputMappingContext;

class UAshley_DialogueComponent;
class UAshley_InventoryComponent;
class UAshley_InventoryUI;
class AAshley_PlayerController;
class AAshley_QuestManager;
class AAshley_CarryItem;
class AAshley_Pickup;
class AAshley_Torch;
class AAshley_ItemShowcase;
class UCineCameraComponent;
struct FInventoryInfo;

UCLASS(Abstract)
class ASHLEY_API AAshley_CharacterPlayer : public AAshley_CharacterBase
{
	GENERATED_BODY()

public:

	AAshley_CharacterPlayer();
	virtual void PossessedBy(AController* NewController) override;

	UFUNCTION(BlueprintCallable, Category = "Interact")
	void Interact();

	UFUNCTION(BlueprintCallable)
	void HideObjectsInHand(bool bHide);

	UFUNCTION(BlueprintCallable, Category = "Interact")
	void ToggleActorActive(bool bActive);

	UFUNCTION(BlueprintCallable, Category = "Interact")
	void InteractAreaTriggered(bool bEntered, TScriptInterface<IAshley_InteractInterface> Interactable);

	UFUNCTION(BlueprintCallable, Category = "Interact")
	void PickupItem(FName PickupItem, FInventoryInfo InventoryInfo, int StackSize = 1);

	UFUNCTION(BlueprintCallable, Category = "Interact")
	void StopExaminingObject();

	UFUNCTION(BlueprintCallable, Category = "Interact")
	void RotateShowcaseItem(FVector2D Delta);

	UFUNCTION(BlueprintCallable, Category = "Interact")
	void CarryItem(AAshley_CarryItem* ItemToCarry);

	UFUNCTION(BlueprintCallable, Category = "Interact")
	void DropItem();

	UFUNCTION(BlueprintCallable, Category = "Interact")
	void UseCarriedItem();

	UFUNCTION(BlueprintCallable, Category = "Movement")
	virtual void Sprint(bool bSpring);

	UFUNCTION(BlueprintCallable)
	virtual TArray<FItemStack> GetItemsInInventory();

	UFUNCTION(BlueprintCallable)
	virtual TArray<FItemStack> GetCollectables();

	UFUNCTION()
	virtual AAshley_QuestManager* GetQuestManager();

	UFUNCTION(BlueprintCallable)
	virtual UAshley_InventoryComponent* GetInventory();

	bool IsShowcaseShowing() const { return bIsShowcaseShowing; }

	// TODO: migliorare con classe apposta
	void PutObjectInHand(AActor* Object);

	void PutObjectInOffHand(AActor* ObjectToAttach);

	// Torch
	void EquipTorchInHand(AAshley_Torch* Torch);

	void ToggleTorch(bool bIsTorchOn);

	void SetTorch(AAshley_Torch* Torch);


	//INTERFACES

	//virtual void Interface_Interaction_Pickup_Implementation(bool Enters, AActor* InteractActor, FName ID_Name, bool bIsCollectable, int32 NumKeyPuzzle) override;
	//virtual void Interface_Interaction_Pickup_Implementation(bool Enters, AActor* InteractActor, FName ID_Name, FInventoryInfo InventoryInfo) override;
	//virtual void Interface_Interaction_Openable_Key_Implementation(bool Enters, AActor* InteractActor, bool bClosed, int32 Key) override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data|Movement")
	float MaxVelocityWalk;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data|Movement")
	float MaxVelocityRun;

protected:
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;

	virtual void OpenItemShowcase();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data|Trace")
	float TraceDistance;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data|Trace")
	float Radius;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Data|Inventory")
	UAshley_InventoryComponent* InventoryComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Data|Inventory")
	AAshley_QuestManager* QuestManager;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data|Inventory")
	AAshley_ItemShowcase* ItemShowcase;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data|Inventory")
	TSubclassOf<UAshley_InventoryUI> InventoryBP;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	AAshley_PlayerController* PlayerControllerRef;

	UPROPERTY()
	AAshley_CarryItem* ItemCarried;

	UPROPERTY()
	AAshley_Torch* TorchCarried;

	UPROPERTY()
	UAnimInstance* AnimInstance;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Data|Character")
	bool bIsShowcaseShowing;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Data|Character")
	bool bHasTorchEquipped;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Data|Dialogue")
	UAshley_DialogueComponent* DialogueComponent;

	UPROPERTY()
	bool bShouldTrace;

	UPROPERTY()
	bool bCanInteract;

	UPROPERTY()
	float LastCameraFocus;

	UPROPERTY()
	UCineCameraComponent* LastCamera;

	TQueue<FName> ItemsToShowcase;

	UPROPERTY()
	TScriptInterface<IAshley_InteractInterface> CurrentInteractable;

	UPROPERTY()
	AActor* ObjectInHand;

	UPROPERTY()
	TArray<TScriptInterface<IAshley_InteractInterface>> InteractablesInRange;
};
