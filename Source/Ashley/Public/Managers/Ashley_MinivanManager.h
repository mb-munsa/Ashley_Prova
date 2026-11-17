// Ashley

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Ashley_MinivanManager.generated.h"

class AAshley_PlayerController;
class AAshley_JournalistPlayer;
class AAshley_FixedCameraController;
class AAshley_InvestigationBoardManager;
class AAshley_MinivanPuzzle;
class AAshley_MinivanSelector;
class UAshley_PickupDA;
class UUserWidget;

// Same order as the EN_MinivanScene Blueprint enum
UENUM(BlueprintType)
enum class EAshley_MinivanView : uint8
{
	GeneralView,
	DeskView,
	PuzzleView,
	BoardView,
	LaptopView
};

UCLASS()
class ASHLEY_API AAshley_MinivanManager : public AActor
{
	GENERATED_BODY()

public:
	AAshley_MinivanManager();

	UFUNCTION(BlueprintCallable)
	void SwitchToView(EAshley_MinivanView NewScene);

	UFUNCTION(BlueprintCallable)
	void StartPuzzle(AAshley_MinivanPuzzle* Puzzle);

	UFUNCTION(BlueprintCallable)
	void ExitPuzzle(AAshley_MinivanPuzzle* Puzzle);

protected:
	virtual void BeginPlay() override;

	// Spawno il puzzle corretto dall'inventario
	UFUNCTION()
	void HandleOnItemSelected(FName ItemName);

	UFUNCTION()
	void OnInventoryClose();

	// Apro l'inventario e aspetto che venga selezionato un item presente in NamePuzzleMap
	UFUNCTION()
	void OnPuzzleSelectorClicked(AAshley_MinivanSelector* Selector);

	UFUNCTION()
	void OnBackButton();

	UFUNCTION()
	void CompletePuzzle(AAshley_MinivanPuzzle* Puzzle);

	void PutPuzzleBackInInventory();

	void SetGameAndUIInputMode();

	UFUNCTION()
	TArray<FName> GetItemNames() const;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Minivan|Cameras")
	TObjectPtr<AAshley_FixedCameraController> SelectionCamera;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Minivan|Cameras")
	TObjectPtr<AAshley_FixedCameraController> DeskCamera;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Minivan|Cameras")
	TObjectPtr<AAshley_FixedCameraController> PuzzleCamera;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Minivan|Cameras")
	TObjectPtr<AAshley_FixedCameraController> BoardCamera;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Minivan|Selectors")
	TObjectPtr<AAshley_MinivanSelector> DeskSelector;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Minivan|Selectors")
	TObjectPtr<AAshley_MinivanSelector> BoardSelector;

	// Attivo solo nella DeskView
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Minivan|Selectors")
	TObjectPtr<AAshley_MinivanSelector> PuzzleSelector;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Minivan|Board")
	TObjectPtr<AAshley_InvestigationBoardManager> BoardManager;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Minivan|Puzzle")
	TObjectPtr<AActor> PuzzleSpawnPoint;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Minivan|Puzzle")
	TObjectPtr<UAshley_PickupDA> ItemData;

	// Item dell'inventario -> puzzle da spawnare
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Minivan|Puzzle", meta = (GetKeyOptions = "GetItemNames"))
	TMap<FName, TSubclassOf<AAshley_MinivanPuzzle>> NamePuzzleMap;

	// WBP_MenuButton
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Minivan|UI")
	TSubclassOf<UUserWidget> BackButtonClass;

	UPROPERTY(BlueprintReadOnly, Category = "Minivan|UI")
	TObjectPtr<UUserWidget> BackButton;

	UPROPERTY(BlueprintReadOnly, Category = "Minivan|Puzzle")
	TObjectPtr<AAshley_MinivanPuzzle> CurrentPuzzle;

	UPROPERTY(BlueprintReadOnly, Category = "Minivan")
	EAshley_MinivanView CurrentView = EAshley_MinivanView::GeneralView;

	// True tra il click sul PuzzleSelector e la selezione dell'item (o la chiusura dell'inventario)
	UPROPERTY(BlueprintReadOnly, Category = "Minivan|Puzzle")
	bool bAwaitingPuzzleItem = false;

	UPROPERTY()
	TObjectPtr<AAshley_PlayerController> PlayerController;

	UPROPERTY()
	TObjectPtr<AAshley_JournalistPlayer> JournalistPlayer;
};
