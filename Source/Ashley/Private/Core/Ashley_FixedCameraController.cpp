// Ashley


#include "Core/Ashley_FixedCameraController.h"
#include "Core/Ashley_PlayerController.h"
#include "Components/Ashley_CameraManager.h"
#include "Characters/Players/Ashley_CharacterPlayer.h"
#include "Tags_Enums_Structs/Ashley_GameplayTags.h"

#include "Kismet/GameplayStatics.h"
#include "Components/BoxComponent.h"
#include "Components/ArrowComponent.h"
#include "CineCameraComponent.h"
#include <Kismet/KismetMathLibrary.h>

#include "DebugHelper.h"

AAshley_FixedCameraController::AAshley_FixedCameraController()
{
	PrimaryActorTick.bCanEverTick = false;

	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("DefaultSceneRoot"));

	TriggerVolume = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxVolume"));
	TriggerVolume->SetupAttachment(RootComponent);

	Camera = CreateDefaultSubobject<UCineCameraComponent>(TEXT("Camera"));
	Camera->SetupAttachment(RootComponent);

    DirectionalArrow = CreateDefaultSubobject<UArrowComponent>(TEXT("DirectionalArrow"));
    DirectionalArrow->SetupAttachment(RootComponent);
}

float AAshley_FixedCameraController::GetBlendTime() const
{
	return BlendTime;
}

FVector AAshley_FixedCameraController::GetForwardDirection() const
{
    FVector Forward = DirectionalArrow->GetForwardVector();
    FVector Normalized = FVector(Forward.X, Forward.Y, 0.0L);
    Normalized.Normalize();
    return Normalized;
}

FVector AAshley_FixedCameraController::GetRightDirection() const
{
    FVector Right = DirectionalArrow->GetRightVector();
    FVector Normalized = FVector(Right.X, Right.Y, 0.0L);
    Normalized.Normalize();
    return Normalized;
}

FVector AAshley_FixedCameraController::GetCurrentForwardDirection() const
{
    FVector Forward =  Camera->GetForwardVector();
    FVector Normalized = FVector(Forward.X, Forward.Y, 0.0L);
    Normalized.Normalize();
    return Normalized;
}

FVector AAshley_FixedCameraController::GetCurrentRightDirection() const
{
    FVector Right = Camera->GetRightVector();
    FVector Normalized = FVector(Right.X, Right.Y, 0.0L);
    Normalized.Normalize();
    return Normalized;
}

bool AAshley_FixedCameraController::GetShouldTrackPlayer() const
{
    return bShouldTrackPlayer;
}

void AAshley_FixedCameraController::FollowPlayer(AActor* ActorToFollow, float DeltaTime)
{
    FRotator CurrentRotation = Camera->GetRelativeRotation();
    FRotator TargetRotation = UKismetMathLibrary::FindLookAtRotation(Camera->GetComponentLocation(), ActorToFollow->GetActorLocation());

    FRotator DeltaRot = (TargetRotation - InitialRotation).GetNormalized();
    DeltaRot.Pitch = FMath::Clamp(DeltaRot.Pitch, -MaxDelta, MaxDelta);
    DeltaRot.Yaw = FMath::Clamp(DeltaRot.Yaw, -MaxDelta, MaxDelta);
    DeltaRot.Roll = 0.f;

    TargetRotation = InitialRotation + DeltaRot;

    FRotator NewRotation = FMath::RInterpTo(CurrentRotation, TargetRotation, DeltaTime, 1.5f); // TODO: mettere speed come paramet

    Camera->SetRelativeRotation(NewRotation);
}

void AAshley_FixedCameraController::SnapToPlayer(AActor* PlayerChar)
{
    FRotator CurrentRotation = Camera->GetRelativeRotation();
    FRotator TargetRotation = UKismetMathLibrary::FindLookAtRotation(Camera->GetComponentLocation(), PlayerChar->GetActorLocation());

    FRotator DeltaRot = (TargetRotation - InitialRotation).GetNormalized();
    DeltaRot.Pitch = FMath::Clamp(DeltaRot.Pitch, -MaxDelta, MaxDelta);
    DeltaRot.Yaw = FMath::Clamp(DeltaRot.Yaw, -MaxDelta, MaxDelta);
    DeltaRot.Roll = 0.f;

    TargetRotation = InitialRotation + DeltaRot;

    Camera->SetRelativeRotation(TargetRotation);
}

void AAshley_FixedCameraController::BeginPlay()
{
	Super::BeginPlay();

    // Aggiunto tutti i Box come trigger
    TArray<UBoxComponent*> TriggerVolumes;
    this->GetComponents<UBoxComponent>(TriggerVolumes);

    for (const auto& trigger : TriggerVolumes)
    {
        trigger->OnComponentBeginOverlap.AddDynamic(this, &AAshley_FixedCameraController::OnBoxOverlap);
        trigger->OnComponentEndOverlap.AddDynamic(this, &AAshley_FixedCameraController::OnBoxEndOverlap);
    }

    InitialRotation = Camera->GetRelativeRotation();
    while (InitialRotation.Yaw > 180)
        InitialRotation.Yaw -= 360;
    while (InitialRotation.Pitch > 180)
        InitialRotation.Pitch -= 360;
}

void AAshley_FixedCameraController::ResetCamera()
{
    Camera->SetRelativeRotation(InitialRotation);
}

UCineCameraComponent* AAshley_FixedCameraController::GetCameraComponent()
{
    return Camera;
}

void AAshley_FixedCameraController::OnBoxOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (Cast<AAshley_CharacterPlayer>(OtherActor))
		UAshley_CameraManager::Get(this)->AddToCameraStack(this);
}

bool AAshley_FixedCameraController::IsActorInsideTriggers(const AActor* Actor) const
{
    TArray<UBoxComponent*> TriggerVolumes;
    GetComponents<UBoxComponent>(TriggerVolumes);

    for (const UBoxComponent* Trigger : TriggerVolumes)
    {
        if (Trigger->IsOverlappingActor(Actor)) return true;
    }
    return false;
}

void AAshley_FixedCameraController::OnBoxEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
    if (!Cast<AAshley_CharacterPlayer>(OtherActor)) return;

    // The player can overlap with several components / boxes:
    // leave the stack only once no part of the player is inside any of them
    if (!IsActorInsideTriggers(OtherActor))
    {
        UAshley_CameraManager::Get(this)->RemoveFromCameraStack(this);

        if (bDestroyOnExit) this->Destroy();
    }
}

// Editor function to make sure that only one camera can be flagged as the first camera

#if WITH_EDITOR
#include "Editor.h"

void AAshley_FixedCameraController::PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent)
{
    Super::PostEditChangeProperty(PropertyChangedEvent);

    FName PropertyName = (PropertyChangedEvent.Property != nullptr)
        ? PropertyChangedEvent.Property->GetFName()
        : NAME_None;

    if (PropertyName == GET_MEMBER_NAME_CHECKED(AAshley_FixedCameraController, IsFirstCamera) && IsFirstCamera)
    {
        // Find all other actors of this type
        UWorld* World = GetWorld();
        if (World)
        {
            TArray<AActor*> FoundActors;
            UGameplayStatics::GetAllActorsOfClass(World, AAshley_FixedCameraController::StaticClass(), FoundActors);

            for (AActor* Actor : FoundActors)
            {
                AAshley_FixedCameraController* Other = Cast<AAshley_FixedCameraController>(Actor);
                if (Other && Other != this && Other->IsFirstCamera)
                {
                    // Clear the flag on the other actor
                    Other->Modify();
                    Other->IsFirstCamera = false;

                    UE_LOG(LogTemp, Warning, TEXT("Unsetting Initial flag on %s because %s was set as Initial"),
                        *Other->GetName(), *GetName());
                }
            }
        }
    }
}
#endif
