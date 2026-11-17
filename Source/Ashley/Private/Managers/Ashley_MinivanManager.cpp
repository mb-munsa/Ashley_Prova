// Ashley


#include "Managers/Ashley_MinivanManager.h"
#include "Managers/Ashley_WidgetSystem.h"
#include "Managers/Ashley_EventManagerSystem.h"

#include "Core/Ashley_PlayerController.h"
#include "Core/Ashley_FixedCameraController.h"
#include "Components/Ashley_CameraManager.h"
#include "Components/Ashley_InventoryComponent.h"
#include "Characters/Players/Ashley_JournalistPlayer.h"
#include "InvestigationBoard/Ashley_InvestigationBoardManager.h"
#include "Interaction/Ashley_MinivanPuzzle.h"
#include "Interaction/Ashley_MinivanSelector.h"
#include "DataAssets/Ashley_PickupDA.h"
#include "DataAssets/Ashley_InputConfigDA.h"

#include "Blueprint/UserWidget.h"
#include <Kismet/GameplayStatics.h>

#include "DebugHelper.h"

AAshley_MinivanManager::AAshley_MinivanManager()
{
	PrimaryActorTick.bCanEverTick = false;

	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("DefaultSceneRoot"));
}

void AAshley_MinivanManager::BeginPlay()
{
	Super::BeginPlay();

	PlayerController = Cast<AAshley_PlayerController>(UGameplayStatics::GetPlayerController(this, 0));
	if (!PlayerController) return;

	PlayerController->SetMappingContextByName(EAshley_InputMappingContext::MOUSE_PUZZLE);
	PlayerController->SetShowMouseCursor(true);
	SetGameAndUIInputMode();

	// Chiudendo pausa/inventario il WidgetSystem rimette Game+UI col cursore invece di GameOnly
	UGameplayStatics::GetGameInstance(this)->GetSubsystem<UAshley_WidgetSystem>()->SetCursorModeOnResume(true);

	JournalistPlayer = Cast<AAshley_JournalistPlayer>(UGameplayStatics::GetPlayerCharacter(this, 0));
	if (!JournalistPlayer) return;

	UGameInstance* GameInstance = UGameplayStatics::GetGameInstance(this);
	GameInstance->GetSubsystem<UAshley_EventManagerSystem>()->OnItemSelected.AddDynamic(this, &ThisClass::HandleOnItemSelected);
	GameInstance->GetSubsystem<UAshley_WidgetSystem>()->OnInventoryClosed.AddDynamic(this, &ThisClass::OnInventoryClose);

	if (PuzzleSelector)
	{
		PuzzleSelector->OnSelectorClicked.AddDynamic(this, &ThisClass::OnPuzzleSelectorClicked);
		PuzzleSelector->SetCollisionEnabled(CurrentView == EAshley_MinivanView::DeskView);
	}

	if (!BackButtonClass)
	{
		UE_LOG(LogTemp, Error, TEXT("%s: BackButtonClass non assegnata, nessun bottone Back"), *GetName());
		return;
	}

	BackButton = CreateWidget<UUserWidget>(GetWorld(), BackButtonClass);
	if (!BackButton) return;

	// WBP_MenuButton non ha un parent C++: Text e OnButtonClicked sono variabili BP, le raggiungo via reflection
	if (FTextProperty* TextProperty = FindFProperty<FTextProperty>(BackButton->GetClass(), TEXT("Text")))
	{
		TextProperty->SetPropertyValue_InContainer(BackButton, NSLOCTEXT("Ashley_MinivanManager", "BackButton", "Back"));
	}

	BackButton->SetVisibility(ESlateVisibility::Collapsed);
	BackButton->AddToViewport(1000);

	if (FMulticastDelegateProperty* ClickedProperty = FindFProperty<FMulticastDelegateProperty>(BackButton->GetClass(), TEXT("OnButtonClicked")))
	{
		FScriptDelegate ClickedDelegate;
		ClickedDelegate.BindUFunction(this, GET_FUNCTION_NAME_CHECKED(AAshley_MinivanManager, OnBackButton));
		ClickedProperty->AddDelegate(MoveTemp(ClickedDelegate), BackButton);
	}
}

void AAshley_MinivanManager::SwitchToView(EAshley_MinivanView NewScene)
{
	CurrentView = NewScene;
	UAshley_CameraManager* CameraManager = PlayerController->GetCameraManager();

	if (PuzzleSelector)
		PuzzleSelector->SetCollisionEnabled(CurrentView == EAshley_MinivanView::DeskView);

	if (CurrentView == EAshley_MinivanView::GeneralView)
	{
		CameraManager->SetCurrentCamera(SelectionCamera);
		if (BackButton) BackButton->SetVisibility(ESlateVisibility::Collapsed);
		if (DeskSelector) DeskSelector->SetCollisionEnabled(true);
		if (BoardSelector) BoardSelector->SetCollisionEnabled(true);
		return;
	}

	switch (CurrentView)
	{
	case EAshley_MinivanView::DeskView:
	case EAshley_MinivanView::LaptopView:
		CameraManager->SetCurrentCamera(DeskCamera);
		break;
	case EAshley_MinivanView::PuzzleView:
		CameraManager->SetCurrentCamera(PuzzleCamera);
		break;
	case EAshley_MinivanView::BoardView:
		// Nel BP questa vista non cambia camera
		Debug::Print("BOARD");
		BoardManager->BuildBoard();
		break;
	default:
		break;
	}

	if (BackButton) BackButton->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
	if (DeskSelector) DeskSelector->SetCollisionEnabled(false);
	if (BoardSelector) BoardSelector->SetCollisionEnabled(false);
}

void AAshley_MinivanManager::StartPuzzle(AAshley_MinivanPuzzle* Puzzle)
{
	SwitchToView(EAshley_MinivanView::PuzzleView);
	Debug::Print("Starting puzzle");
	Puzzle->StartPuzzleEvent();
}

void AAshley_MinivanManager::ExitPuzzle(AAshley_MinivanPuzzle* Puzzle)
{
	Debug::Print("Exiting puzzle");
	Puzzle->ExitPuzzleEvent();
	SwitchToView(EAshley_MinivanView::DeskView);
}

void AAshley_MinivanManager::CompletePuzzle(AAshley_MinivanPuzzle* Puzzle)
{
	Puzzle->CompletePuzzle();
	ExitPuzzle(Puzzle);

	CurrentPuzzle->Destroy();
	CurrentPuzzle = nullptr;
}

void AAshley_MinivanManager::OnPuzzleSelectorClicked(AAshley_MinivanSelector* Selector)
{
	UAshley_WidgetSystem* WidgetSystem = UGameplayStatics::GetGameInstance(this)->GetSubsystem<UAshley_WidgetSystem>();

	bAwaitingPuzzleItem = true;

	if (!WidgetSystem->IsInventoryOpen())
		WidgetSystem->ToggleInventory();
}

void AAshley_MinivanManager::HandleOnItemSelected(FName ItemName)
{
	// Reagisco solo se l'inventario è stato aperto dal PuzzleSelector; un item che non è un puzzle viene ignorato
	if (!bAwaitingPuzzleItem) return;
	if (!NamePuzzleMap.Contains(ItemName)) return;

	bAwaitingPuzzleItem = false;

	if (IsValid(CurrentPuzzle))
		PutPuzzleBackInInventory();

	FActorSpawnParameters SpawnParams;
	SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
	CurrentPuzzle = GetWorld()->SpawnActor<AAshley_MinivanPuzzle>(NamePuzzleMap[ItemName], PuzzleSpawnPoint->GetActorTransform(), SpawnParams);

	UGameplayStatics::GetGameInstance(this)->GetSubsystem<UAshley_WidgetSystem>()->ToggleInventory();
	JournalistPlayer->GetInventory()->DeleteObjectInventory(ItemName);

	PlayerController->SetShowMouseCursor(true);

	CurrentPuzzle->OnPuzzleClicked.AddDynamic(this, &ThisClass::StartPuzzle);
	CurrentPuzzle->OnPuzzleCompleted.AddDynamic(this, &ThisClass::CompletePuzzle);

	// La vista cambia solo ora che è stato scelto un puzzle
	SwitchToView(PuzzleSelector->GetSceneToFocus());
}

void AAshley_MinivanManager::PutPuzzleBackInInventory()
{
	CurrentPuzzle->OnPuzzleClicked.Clear();
	CurrentPuzzle->OnPuzzleCompleted.Clear();

	JournalistPlayer->GetInventory()->PutObjectInventory(CurrentPuzzle->GetPickupName(), EPickupType::OBJECT, 1);

	CurrentPuzzle->Destroy();
	CurrentPuzzle = nullptr;
}

void AAshley_MinivanManager::OnInventoryClose()
{
	// Inventario chiuso senza scegliere un puzzle
	const bool bClosedWithoutChoosing = bAwaitingPuzzleItem;
	bAwaitingPuzzleItem = false;

	// Se c'è già un puzzle sulla scrivania vado comunque alla sua vista, come se fosse stato scelto
	if (bClosedWithoutChoosing && IsValid(CurrentPuzzle))
		SwitchToView(PuzzleSelector->GetSceneToFocus());
}

void AAshley_MinivanManager::OnBackButton()
{
	Debug::Print("Back button clicked");

	// Con l'inventario aperto, Back chiude solo l'inventario
	UAshley_WidgetSystem* WidgetSystem = UGameplayStatics::GetGameInstance(this)->GetSubsystem<UAshley_WidgetSystem>();
	if (WidgetSystem->IsInventoryOpen())
	{
		WidgetSystem->ToggleInventory();
		return;
	}

	switch (CurrentView)
	{
	case EAshley_MinivanView::DeskView:
	case EAshley_MinivanView::LaptopView:
		SwitchToView(EAshley_MinivanView::GeneralView);
		break;
	case EAshley_MinivanView::PuzzleView:
		if (IsValid(CurrentPuzzle) && CurrentPuzzle->IsExitBlocked()) break;
		SwitchToView(EAshley_MinivanView::DeskView);
		if (IsValid(CurrentPuzzle)) CurrentPuzzle->ExitPuzzleEvent();
		break;
	case EAshley_MinivanView::BoardView:
		BoardManager->HideBoard();
		SwitchToView(EAshley_MinivanView::GeneralView);
		break;
	default:
		break;
	}
}

void AAshley_MinivanManager::SetGameAndUIInputMode()
{
	FInputModeGameAndUI InputMode;
	InputMode.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);
	InputMode.SetHideCursorDuringCapture(false);
	PlayerController->SetInputMode(InputMode);
}


TArray<FName> AAshley_MinivanManager::GetItemNames() const
{
	TArray<FName> IDs;
	if (ItemData)
	{
		IDs = ItemData->GetIDsValids();
	}
	return IDs;
}
