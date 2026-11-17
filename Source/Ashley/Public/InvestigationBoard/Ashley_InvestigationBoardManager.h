// Ashley

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Ashley_InvestigationConnection.h"
#include "Ashley_InvestigationPostIt.h"
#include "Ashley_InvestigationBoardDefinition.h"
#include "UI/Ashley_InvestigationBoardWidget.h"

#include "Ashley_InvestigationBoardManager.generated.h"

class UAshley_InvestigationBoardSubsystem;

DECLARE_MULTICAST_DELEGATE_OneParam(FOnPostItRevealed, FName);

DECLARE_MULTICAST_DELEGATE_OneParam(FOnConnectionAdded, FInvestigationConnection);

DECLARE_MULTICAST_DELEGATE_OneParam(FOnDeductionDiscovered, FName);

UCLASS()
class ASHLEY_API AAshley_InvestigationBoardManager : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AAshley_InvestigationBoardManager();

	FOnPostItRevealed OnPostItRevealed;
	FOnConnectionAdded OnConnectionAdded;
	FOnDeductionDiscovered OnDeductionDiscovered;

	// Apre la board usata per ultima (o la prima disponibile)
	UFUNCTION(BlueprintCallable)
	void BuildBoard();

	// Apre la board indicata. Se None, quella usata per ultima (o la prima disponibile)
	UFUNCTION(BlueprintCallable)
	void OpenBoard(FName BoardId = NAME_None);

	// Passa a un'altra board mentre la bacheca e' aperta
	UFUNCTION(BlueprintCallable)
	void SwitchBoard(FName BoardId);

	UFUNCTION(BlueprintCallable)
	void ShowBoard();

	UFUNCTION(BlueprintCallable)
	void HideBoard();

	const FInvestigationBoardState& GetActiveState() const;

	bool IsPostItVisible(FName PostItId) const;
	bool IsConnected(FName A, FName B) const;
	bool IsDeductionDiscovered(FName DeductionId) const;

	void RevealPostIt(const FInvestigationPostIt& PostIt, bool bAnimate = false);
	void DrawConnection(FName A, FName B);
	void DiscoverDeduction(const FInvestigationPostIt& Deduction);

	UFUNCTION()
	void TryConnect(FName A, FName B);
	// se riesce -> chiama sul WBP show connection (1,2) e opzionalmente mostra deduction


protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	void EnsureWidget();

	// Svuota il widget e lo ricostruisce dallo stato della board attiva
	void PopulateActiveBoard();

	// Rivela i post-it non ancora visibili che soddisfano le condizioni
	void RefreshReveals(bool bAnimate);

	void SetActiveBoard(UInvestigationBoardDefinition* Board);

	bool ShouldRevealPostIt(const FInvestigationPostIt& PostIt);

	bool IsBoardUnlocked(const UInvestigationBoardDefinition* Board);

	TArray<UInvestigationBoardDefinition*> GetAllBoards() const;
	TArray<UInvestigationBoardDefinition*> GetUnlockedBoards();
	UInvestigationBoardDefinition* FindBoard(FName BoardId) const;

	FInvestigationBoardState& GetActiveStateMutable();

	UAshley_InvestigationBoardSubsystem* GetBoardSubsystem() const;
	class UAshley_InventoryComponent* GetPlayerInventory();

	UPROPERTY()
	TObjectPtr<UAshley_InvestigationBoardWidget> BoardWidget;

	UPROPERTY()
	TObjectPtr<class UAshley_InventoryComponent> PlayerInventory;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data")
	TSubclassOf<UAshley_InvestigationBoardWidget> BoardWidgetClass;

	// Tutte le board disponibili, nell'ordine in cui compaiono i tab
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data")
	TArray<TObjectPtr<UInvestigationBoardDefinition>> Boards;

	// Legacy: board singola, aggiunta in coda a Boards se non gia' presente
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data")
	TObjectPtr<UInvestigationBoardDefinition> Definition;

	// Board attualmente mostrata
	UPROPERTY()
	TObjectPtr<UInvestigationBoardDefinition> ActiveDefinition;

	FName ActiveBoardId;
};
