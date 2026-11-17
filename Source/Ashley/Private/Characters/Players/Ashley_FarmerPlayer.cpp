// Ashley


#include "Characters/Players/Ashley_FarmerPlayer.h"

#include "Components/Ashley_HeartRateComponent.h"
#include "Components/Ashley_InventoryComponent.h"
#include "Core/Ashley_PlayerController.h"
#include "Interaction/Items/Ashley_Torch.h"
#include "Episode02/Ashley_GhostSpawner.h"

#include "Components/AudioComponent.h"
#include <Kismet/KismetSystemLibrary.h>
#include <Kismet/GameplayStatics.h>
#include "DebugHelper.h"
#include <Managers/Ashley_WidgetSystem.h>
#include "Components/Ashley_CameraManager.h"
#include <Managers/Ashley_EventManagerSystem.h>

AAshley_FarmerPlayer::AAshley_FarmerPlayer()
{
	HeartRateComponent = CreateDefaultSubobject<UAshley_HeartRateComponent>(TEXT("HeartRateComponent"));
	//HeartRateComponent->RegisterComponent();

	AudioSourceHeart = CreateDefaultSubobject<UAudioComponent>(TEXT("AudioComponentHeart"));
	AudioSourceHeart->SetupAttachment(RootComponent);

	AudioSourceBreath = CreateDefaultSubobject<UAudioComponent>(TEXT("AudioComponentBreath"));
	AudioSourceBreath->SetupAttachment(RootComponent);
}

void AAshley_FarmerPlayer::BeginPlay()
{
	Super::BeginPlay();

	HeartRateComponent->AudioComponentHeartbeat = AudioSourceHeart;
	HeartRateComponent->AudioComponentBreathing = AudioSourceBreath;

	// TODO: rimuovere
	UGameplayStatics::GetGameInstance(this)->GetSubsystem<UAshley_WidgetSystem>()->SetDiaryUI(DiaryBP);
	GetInventory()->PutObjectInventory(HeartPillID, EPickupType::OBJECT, HeartPillsInitialNumber);

	HeartRateComponent->ResetHeartRate();

	// Mi registro agli eventi
	auto EventManager = UGameplayStatics::GetGameInstance(this)->GetSubsystem<UAshley_EventManagerSystem>();
	EventManager->OnEventTriggered.AddDynamic(this, &AAshley_FarmerPlayer::OnPlayerEventTriggered);
}

void AAshley_FarmerPlayer::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);

	// Blocco inizialmente lo sprint del farmer
	PlayerControllerRef->ToggleSprintBlock(true);

	// Attivo il suono delle pagine al cambio telecamera
	PlayerControllerRef->GetCameraManager()->ActivateSoundOnCameraSwitch(true);
}

void AAshley_FarmerPlayer::FarmerTriggerScare(bool bShouldBeScared)
{
	HeartRateComponent->TriggerScare(bShouldBeScared);
}

void AAshley_FarmerPlayer::FarmerHeartAttack()
{
	// Resetto il cuore
	HeartRateComponent->ResetHeartRate();
	HideObjectsInHand(true);

	FName StartingSection = FName("Default");

	// Mando l'animazione di heartAttack
	AnimInstance->Montage_Play(HeartAttackMontage);
	AnimInstance->Montage_JumpToSection(StartingSection, HeartAttackMontage);

	// Blocco gli input/movimenti del player
	PlayerControllerRef->ToggleInputBlock(true);

	// Forzo la scomparsa del fantasma
	GhostSpawner->StopGhost();

	// Bind delegate to handle when montage ends
	//FOnMontageEnded EndDelegate;
	//EndDelegate.BindUObject(this, &AAshley_FarmerPlayer::OnHeartAttackAnimEnded);
	//AnimInstance->Montage_SetEndDelegate(EndDelegate, HeartAttackMontage);
}

void AAshley_FarmerPlayer::LightFire()
{
	if (AnimInstance && LightFireMontage)
	{
		HideObjectsInHand(true);
		AnimInstance->Montage_Play(LightFireMontage, 1.0f);
		PlayerControllerRef->ToggleInputBlock(true);
	}
}

void AAshley_FarmerPlayer::Die()
{
	// TODO: 
	HideObjectsInHand(true);
	APlayerController* PlayerController = UGameplayStatics::GetPlayerController(GetWorld(), 0);
	UKismetSystemLibrary::QuitGame(GetWorld(), PlayerController, EQuitPreference::Quit, true);
}

void AAshley_FarmerPlayer::SetGhostSpawner(AAshley_GhostSpawner* Spawner)
{
	GhostSpawner = Spawner;
}

void AAshley_FarmerPlayer::SetScarecrowSpawner(AAshley_ScarecrowSpawner* Spawner)
{
}

void AAshley_FarmerPlayer::OnHeartAttackAnimEnded()
{
	if (InventoryComponent->GetItemCount(HeartPillID) > 0)
	{
		// Consumo una pillola
		InventoryComponent->DeleteObjectInventory(HeartPillID);

		// Mando l'animazione di heartAttack
		FName StartingSection = FName("Pill");
		AnimInstance->Montage_Play(HeartAttackMontage);
		AnimInstance->Montage_JumpToSection(StartingSection, HeartAttackMontage);
	}
	else Die();
}

void AAshley_FarmerPlayer::Sprint(bool bSpring)
{
	Super::Sprint(bSpring);

	HeartRateComponent->StartRunning(bSpring);
}

// Gestione torcia

// Alla fine della prima sequence equippo la torcia
void AAshley_FarmerPlayer::OnPlayerEventTriggered(FGameplayTag EventName, FString EventParameter)
{
	if (EventName == EventEquipTorch)
	{
		TorchCarried->EquipTorch();
		ToggleTorch(false);
		return;
	}

	if (EventName != PlayerEnteredHouseEvent) return;

	bool bIsExit = EventParameter == "Exit";

	// Cambio stato
	if ((!bHasTorchEquipped && bIsExit || bHasTorchEquipped && !bIsExit))
	{
		ToggleTorch(bIsExit);
	}
}
