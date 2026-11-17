// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/Ashley_CharacterBase.h"

// Sets default values
AAshley_CharacterBase::AAshley_CharacterBase()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

}

//void AAshley_CharacterBase::Interface_Interaction_Implementation(bool Enters, AActor* InteractActor, FName ID_Name)
//{
//}

// Called when the game starts or when spawned
void AAshley_CharacterBase::BeginPlay()
{
	Super::BeginPlay();
	
}




