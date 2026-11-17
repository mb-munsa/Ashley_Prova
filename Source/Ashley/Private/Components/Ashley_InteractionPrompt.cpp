// Ashley


#include "Components/Ashley_InteractionPrompt.h"

#include "Kismet/GameplayStatics.h"
#include "GameFramework/PlayerController.h"
#include "Camera/PlayerCameraManager.h"
#include <Widgets/Ashley_InfoText.h>


//UAshley_InteractionPrompt::UAshley_InteractionPrompt()
//{
//	PrimaryComponentTick.bCanEverTick = true;
//}

void UAshley_InteractionPrompt::BeginPlay()
{
    Super::BeginPlay();
    Hide();
}

void UAshley_InteractionPrompt::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
    Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (!bIsShowing) return;

    APlayerController* PlayerController = UGameplayStatics::GetPlayerController(this, 0);
    if (!PlayerController)
    {
        return;
    }

    // Get the player's camera location
    FVector CameraLocation;
    FRotator CameraRotation;
    PlayerController->GetPlayerViewPoint(CameraLocation, CameraRotation);

    // Component location
    const FVector ComponentLocation = GetComponentLocation();

    // Direction from the component towards the camera
    FVector DirectionToCamera = CameraLocation - ComponentLocation;

    // Ignore vertical difference so we only rotate around yaw
    DirectionToCamera.Z = 0.0f;

    if (DirectionToCamera.IsNearlyZero())
    {
        return;
    }

    // Calculate the yaw needed to face the camera
    const FRotator TargetRotation = DirectionToCamera.Rotation();

    // Keep the component's current pitch and roll
    FRotator NewRotation = GetComponentRotation();
    NewRotation.Yaw = TargetRotation.Yaw;

    SetWorldRotation(NewRotation);

}

void UAshley_InteractionPrompt::Show()
{
	this->SetVisibility(true);

	bIsShowing = true;
}

void UAshley_InteractionPrompt::Hide()
{
	this->SetVisibility(false);

	bIsShowing = false;
}

void UAshley_InteractionPrompt::SetText(const FString& TextToDisplay) const
{
    UUserWidget* CurrentWidget = GetWidget();

    if (!CurrentWidget)
    {
        return;
    }

    UAshley_InfoText* InteractionWidget = Cast<UAshley_InfoText>(CurrentWidget);

    if (!InteractionWidget)
    {
        return;
    }

    InteractionWidget->SetTextToDisplay(TextToDisplay);
}


