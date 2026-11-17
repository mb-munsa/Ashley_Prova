// Ashley


#include "Interaction/Ashley_Door.h"
#include "Components/StaticMeshComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "Characters/Ashley_CharacterBase.h"

#include "DebugHelper.h"

AAshley_Door::AAshley_Door()
{
	PrimaryActorTick.bCanEverTick = false; // nessun Tick
}

void AAshley_Door::SetDoorDirection(AActor* OtherActor)
{
	Angle = OpenRotation;

	FVector CharacterLocation = OtherActor->GetActorLocation();
	FVector DoorLocation = GetActorLocation();
	FVector PositionCharacterRelative = CharacterLocation - DoorLocation;
	PositionCharacterRelative.Normalize();
	FVector DoorFV = GetActorForwardVector();

	bDoorDirection = (FVector::DotProduct(PositionCharacterRelative, DoorFV) >= 0);

	Angle = bDoorDirection ? (InitialOpenableRotation - Angle) : (InitialOpenableRotation + Angle);
}

void AAshley_Door::NotifyActorBeginOverlap(AActor* OtherActor)
{
	Super::NotifyActorBeginOverlap(OtherActor);

	if (CurrentState != EState::OPEN) SetDoorDirection(OtherActor);
}
