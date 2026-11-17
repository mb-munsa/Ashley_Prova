// Ashley


#include "Components/Ashley_CameraManager.h"

#include "Managers/Ashley_AudioManager.h"
#include "Core/Ashley_FixedCameraController.h"
#include <Core/Ashley_PlayerController.h>
#include <Characters/Players/Ashley_CharacterPlayer.h>
#include <Kismet/GameplayStatics.h>

#include "DebugHelper.h"

// Sets default values for this component's properties
UAshley_CameraManager::UAshley_CameraManager() 
{
	PrimaryComponentTick.bCanEverTick = true;
}

UAshley_CameraManager* UAshley_CameraManager::Get(const UObject* WorldContextObject)
{
	auto PlayerController = Cast<AAshley_PlayerController>(UGameplayStatics::GetPlayerController(WorldContextObject, 0));
	return PlayerController ? PlayerController->GetCameraManager() : nullptr;
}

// Called when the game starts
void UAshley_CameraManager::BeginPlay()
{
	Super::BeginPlay();

	CurrentCamera = nullptr;

	PlayerControllerRef = Cast<AAshley_PlayerController>(UGameplayStatics::GetPlayerController(GetWorld(), 0));
	CharacterRef = Cast<AAshley_CharacterPlayer>(PlayerControllerRef->GetCharacter());

	FindAndAssignFirstCamera();
}

// Called every frame
void UAshley_CameraManager::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// Follow player
	if (bIsFollowingPlayer && IsValid(CurrentCamera))
	{
		//Debug::Print(TEXT("CameraManager -> FollowPlayer !!"));
		CurrentCamera->FollowPlayer(CharacterRef, DeltaTime);

		CurrentForwardVector = CurrentCamera->GetCurrentForwardDirection();
		CurrentRightVector = CurrentCamera->GetCurrentRightDirection();
	}
}

void UAshley_CameraManager::FindAndAssignFirstCamera()
{
	// We find all the cameras in the level, and set the initial one
	TArray<AActor*> FoundActors;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), AAshley_FixedCameraController::StaticClass(), FoundActors);

	AAshley_FixedCameraController* FirstCamera = nullptr;

	for (AActor* Actor : FoundActors)
	{
		auto Camera = Cast<AAshley_FixedCameraController>(Actor);
		if (Camera && Camera->IsFirstCamera)
		{
			FirstCamera = Camera;
			break;
		}
	}

	if (FirstCamera == nullptr) Debug::Print(TEXT("Nessuna camera impostata come iniziale!!"));
	else AddToCameraStack(FirstCamera);
}

//void UAshley_CameraManager::SetPreviousCamera()
//{
//	CurrentCamera = nullptr;
//	SetCurrentCamera(PreviousCamera, PreviousCamera->GetShouldTrackPlayer());
//}

void UAshley_CameraManager::SetCurrentCamera(AAshley_FixedCameraController* NewCamera)
{
	if (CurrentCamera == NewCamera) return;

	if (!IsValid(NewCamera)) return;

	// Resetto la camera precedente, se esiste
	if (IsValid(CurrentCamera)) CurrentCamera->ResetCamera();

	// Suono di cambio camera
	if (CameraChangeSound != nullptr && bShouldPlaySoundOnCameraSwitch)
		UGameplayStatics::GetGameInstance(this)->GetSubsystem<UAshley_AudioManager>()->PlaySoundEffect(CameraChangeSound);

	CurrentCamera = NewCamera;
	bIsFollowingPlayer = CurrentCamera->GetShouldTrackPlayer();

	if (bIsFollowingPlayer) NewCamera->SnapToPlayer(CharacterRef);

	PlayerControllerRef->SetViewTargetWithBlend(NewCamera, NewCamera->GetBlendTime());

	CurrentForwardVector = NewCamera->GetForwardDirection();
	CurrentRightVector = NewCamera->GetRightDirection();
}

void UAshley_CameraManager::RemoveFromCameraStack(AAshley_FixedCameraController* CameraToRemove)
{
	// Level change: actors are being destroyed and their overlaps cleared, nothing to switch to
	if (GetWorld()->bIsTearingDown) return;

	// Camera neither in the stack nor active (e.g. its begin overlap was missed): nothing to restore.
	// A camera set directly (e.g. a pickup zoom-in) is not in the stack but still active: fall through.
	if (CameraStack.Remove(CameraToRemove) == 0 && CurrentCamera != CameraToRemove) return;

	// Drop any camera destroyed in the meantime
	CameraStack.RemoveAll([](const AAshley_FixedCameraController* Camera) { return !IsValid(Camera); });

	// Nothing left to switch to: keep the current camera as the view (other systems,
	// e.g. the item showcase, expect GetCurrentCamera() to never be null)
	if (CameraStack.Num() > 0)
		SetCurrentCamera(CameraStack.Last());
}

void UAshley_CameraManager::AddToCameraStack(AAshley_FixedCameraController* CameraToAdd)
{
	if (GetWorld()->bIsTearingDown || !IsValid(CameraToAdd)) return;

	// The same camera can be added both by its overlap and manually (e.g. after a teleport):
	// keep a single entry and move it to the top
	CameraStack.Remove(CameraToAdd);
	CameraStack.Add(CameraToAdd);

	SetCurrentCamera(CameraToAdd);
}

AAshley_FixedCameraController* UAshley_CameraManager::GetCurrentCamera()
{
	return CurrentCamera;
}

FVector UAshley_CameraManager::GetCurrentForward() const
{
	return CurrentForwardVector;
}

FVector UAshley_CameraManager::GetCurrentRight() const
{
	return CurrentRightVector;
}
