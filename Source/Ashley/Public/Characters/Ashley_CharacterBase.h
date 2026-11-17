// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Interaction/Ashley_InteractInterface.h"

#include "Ashley_CharacterBase.generated.h"

UCLASS(Abstract)
class ASHLEY_API AAshley_CharacterBase : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AAshley_CharacterBase();

	//INTERFACES
	//virtual void Interface_Interaction_Implementation(bool Enters, AActor* InteractActor, FName ID_Name);

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

};
