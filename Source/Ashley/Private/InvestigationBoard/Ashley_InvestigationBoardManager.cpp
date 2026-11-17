// Ashley


#include "InvestigationBoard/Ashley_InvestigationBoardManager.h"
#include "InvestigationBoard/Ashley_InvestigationBoardSubsystem.h"

#include <Components/Ashley_InventoryComponent.h>

#include "DebugHelper.h"
#include <Characters/Players/Ashley_CharacterPlayer.h>

// Sets default values
AAshley_InvestigationBoardManager::AAshley_InvestigationBoardManager()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AAshley_InvestigationBoardManager::BeginPlay()
{
	Super::BeginPlay();
}

void AAshley_InvestigationBoardManager::BuildBoard()
{
    OpenBoard(NAME_None);
}

// Instanzio il widget se non e' presente, scelgo la board e la ricostruisco
// dallo stato salvato (o la calcolo da zero se non c'e' uno stato)
void AAshley_InvestigationBoardManager::OpenBoard(FName BoardId)
{
    EnsureWidget();
    if (!BoardWidget) return;

    const TArray<UInvestigationBoardDefinition*> UnlockedBoards = GetUnlockedBoards();
    if (!ensureMsgf(UnlockedBoards.Num() > 0, TEXT("No unlocked investigation board"))) return;

    UInvestigationBoardDefinition* Board = FindBoard(BoardId);

    if (!Board || !UnlockedBoards.Contains(Board))
    {
        UAshley_InvestigationBoardSubsystem* Subsystem = GetBoardSubsystem();
        Board = Subsystem ? FindBoard(Subsystem->LastActiveBoardId) : nullptr;
    }

    if (!Board || !UnlockedBoards.Contains(Board))
        Board = UnlockedBoards[0];

    // Prima mostro il widget: le connessioni vengono disegnate solo dopo il layout
    ShowBoard();

    BoardWidget->SetBoards(UnlockedBoards, Board->GetBoardId());
    SetActiveBoard(Board);
}

void AAshley_InvestigationBoardManager::SwitchBoard(FName BoardId)
{
    if (!BoardWidget || BoardId == ActiveBoardId) return;

    UInvestigationBoardDefinition* Board = FindBoard(BoardId);
    if (!Board || !IsBoardUnlocked(Board)) return;

    SetActiveBoard(Board);
}

void AAshley_InvestigationBoardManager::SetActiveBoard(UInvestigationBoardDefinition* Board)
{
    const FName NewBoardId = Board->GetBoardId();

    if (NewBoardId != ActiveBoardId)
        BoardWidget->ResetView();

    ActiveDefinition = Board;
    ActiveBoardId = NewBoardId;

    PopulateActiveBoard();
    BoardWidget->SetActiveTab(ActiveBoardId);

    if (UAshley_InvestigationBoardSubsystem* Subsystem = GetBoardSubsystem())
    {
        Subsystem->LastActiveBoardId = ActiveBoardId;
        Subsystem->SaveToDisk();
    }
}

void AAshley_InvestigationBoardManager::PopulateActiveBoard()
{
    FInvestigationBoardState& State = GetActiveStateMutable();

    BoardWidget->ClearBoard();

    // 1. Ripristino i post-it gia' rivelati (solo widget, lo stato non cambia)
    for (const FInvestigationPostIt& PostIt : ActiveDefinition->PostIts)
    {
        if (State.VisiblePostIts.Contains(PostIt.Id))
            BoardWidget->AddPostId(PostIt);
    }

    // 2. Prima apertura: calcolo completo. Altrimenti solo i post-it nuovi
    //    (es. oggetti raccolti mentre la bacheca era chiusa), animati
    RefreshReveals(State.bInitialized);
    State.bInitialized = true;

    // 3. Connessioni: disegnate dal widget appena i pin hanno un layout valido
    for (const FInvestigationConnection& Connection : State.Connections)
        BoardWidget->AddConnection(Connection.A, Connection.B);
}

void AAshley_InvestigationBoardManager::RefreshReveals(bool bAnimate)
{
    for (const FInvestigationPostIt& PostIt : ActiveDefinition->PostIts)
    {
        if (IsPostItVisible(PostIt.Id)) continue;

        if (ShouldRevealPostIt(PostIt))
            RevealPostIt(PostIt, bAnimate);
    }
}

void AAshley_InvestigationBoardManager::EnsureWidget()
{
    if (BoardWidget) return;
    if (!ensure(BoardWidgetClass)) return;

    BoardWidget = CreateWidget<UAshley_InvestigationBoardWidget>(GetWorld(), BoardWidgetClass);
    if (!BoardWidget) return;

    BoardWidget->AddToViewport(500);
    BoardWidget->OnPostItsConnected.AddDynamic(this, &ThisClass::TryConnect);
    BoardWidget->OnBoardSelected.AddDynamic(this, &ThisClass::SwitchBoard);
}

bool AAshley_InvestigationBoardManager::ShouldRevealPostIt(const FInvestigationPostIt& PostIt)
{
    if (PostIt.RequiredInventoryItem.IsNone() && !PostIt.bRequireDeduction) return true;

	// Query inventory/game state here.
    UAshley_InventoryComponent* Inventory = GetPlayerInventory();

    if (!PostIt.RequiredInventoryItem.IsNone() && Inventory && Inventory->IsItemPresent(PostIt.RequiredInventoryItem)) return true;

    const FInvestigationBoardState& State = GetActiveState();

    int32 MatchedConnections = 0;
    for (const FInvestigationConnection& Required : PostIt.RequiredConnections)
    {
        const bool bFound = State.Connections.Contains(Required);

        if (bFound)
        {
            ++MatchedConnections;
        }
    }

    // Se abbastanza connessioni sono state trovate
    return (MatchedConnections >= PostIt.RequiredConnectionCount);
}

// Mostro il widget
void AAshley_InvestigationBoardManager::ShowBoard()
{
    if (BoardWidget)
        BoardWidget->SetVisibility(ESlateVisibility::Visible);
}

// Nascondo widget e salvo lo stato
void AAshley_InvestigationBoardManager::HideBoard()
{
    if (BoardWidget)
        BoardWidget->SetVisibility(ESlateVisibility::Collapsed);

    if (UAshley_InvestigationBoardSubsystem* Subsystem = GetBoardSubsystem())
        Subsystem->SaveToDisk();
}

// Boards

TArray<UInvestigationBoardDefinition*> AAshley_InvestigationBoardManager::GetAllBoards() const
{
    TArray<UInvestigationBoardDefinition*> Result;

    for (UInvestigationBoardDefinition* Board : Boards)
    {
        if (Board)
            Result.AddUnique(Board);
    }

    if (Definition)
        Result.AddUnique(Definition);

    return Result;
}

TArray<UInvestigationBoardDefinition*> AAshley_InvestigationBoardManager::GetUnlockedBoards()
{
    TArray<UInvestigationBoardDefinition*> Result = GetAllBoards();

    Result.RemoveAll([this](const UInvestigationBoardDefinition* Board)
    {
        return !IsBoardUnlocked(Board);
    });

    return Result;
}

UInvestigationBoardDefinition* AAshley_InvestigationBoardManager::FindBoard(FName BoardId) const
{
    if (BoardId.IsNone()) return nullptr;

    for (UInvestigationBoardDefinition* Board : GetAllBoards())
    {
        if (Board->GetBoardId() == BoardId)
            return Board;
    }

    return nullptr;
}

bool AAshley_InvestigationBoardManager::IsBoardUnlocked(const UInvestigationBoardDefinition* Board)
{
    if (!Board) return false;
    if (Board->bUnlockedByDefault) return true;

    // Una board gia' aperta in passato resta sbloccata
    const UAshley_InvestigationBoardSubsystem* Subsystem = GetBoardSubsystem();
    const FInvestigationBoardState* SavedState = Subsystem ? Subsystem->FindState(Board->GetBoardId()) : nullptr;
    if (SavedState && SavedState->bInitialized) return true;

    UAshley_InventoryComponent* Inventory = GetPlayerInventory();

    return !Board->RequiredInventoryItem.IsNone()
        && Inventory
        && Inventory->IsItemPresent(Board->RequiredInventoryItem);
}

// State

const FInvestigationBoardState& AAshley_InvestigationBoardManager::GetActiveState() const
{
    static const FInvestigationBoardState EmptyState;

    const UAshley_InvestigationBoardSubsystem* Subsystem = GetBoardSubsystem();
    const FInvestigationBoardState* State = Subsystem ? Subsystem->FindState(ActiveBoardId) : nullptr;

    return State ? *State : EmptyState;
}

FInvestigationBoardState& AAshley_InvestigationBoardManager::GetActiveStateMutable()
{
    UAshley_InvestigationBoardSubsystem* Subsystem = GetBoardSubsystem();
    check(Subsystem);

    // Riferimento diretto allo stato salvato: le modifiche non vanno ricopiate
    return Subsystem->GetOrCreateState(ActiveBoardId);
}

UAshley_InvestigationBoardSubsystem* AAshley_InvestigationBoardManager::GetBoardSubsystem() const
{
    const UGameInstance* GameInstance = GetGameInstance();
    return GameInstance ? GameInstance->GetSubsystem<UAshley_InvestigationBoardSubsystem>() : nullptr;
}

UAshley_InventoryComponent* AAshley_InvestigationBoardManager::GetPlayerInventory()
{
    if (PlayerInventory == nullptr)
    {
        const APlayerController* PlayerController = GetWorld()->GetFirstPlayerController();
        auto Character = PlayerController ? Cast<AAshley_CharacterPlayer>(PlayerController->GetCharacter()) : nullptr;

        if (Character)
            PlayerInventory = Character->GetInventory();
    }

    return PlayerInventory;
}

bool AAshley_InvestigationBoardManager::IsPostItVisible(FName PostItId) const
{
    return GetActiveState().VisiblePostIts.Contains(PostItId);
}

bool AAshley_InvestigationBoardManager::IsConnected(FName A, FName B) const
{
    FInvestigationConnection CandidateConnection = FInvestigationConnection(A, B);
    return GetActiveState().Connections.Contains(CandidateConnection);
}

bool AAshley_InvestigationBoardManager::IsDeductionDiscovered(FName DeductionId) const
{
    return GetActiveState().DiscoveredDeductions.Contains(DeductionId);
}

// Actions

void AAshley_InvestigationBoardManager::RevealPostIt(const FInvestigationPostIt& PostIt, bool bAnimate)
{
    GetActiveStateMutable().VisiblePostIts.Add(PostIt.Id);
    BoardWidget->AddPostId(PostIt, bAnimate);

    OnPostItRevealed.Broadcast(PostIt.Id);
}

void AAshley_InvestigationBoardManager::DrawConnection(FName A, FName B)
{
    // La aggiungo alla stato corrente
    GetActiveStateMutable().Connections.Add(FInvestigationConnection(A, B));

    // Visualizzo il nuovo collegamento
    BoardWidget->AddConnection(A, B);

    OnConnectionAdded.Broadcast(FInvestigationConnection(A, B));
}

void AAshley_InvestigationBoardManager::DiscoverDeduction(const FInvestigationPostIt& Deduction)
{
    RevealPostIt(Deduction, true);
    GetActiveStateMutable().DiscoveredDeductions.Add(Deduction.Id);
    OnDeductionDiscovered.Broadcast(Deduction.Id);

    TArray<FInvestigationConnection> FoundConnections;
    for (const FInvestigationConnection& Required : Deduction.RequiredConnections)
    {
        const bool bFound = IsConnected(Required.A, Required.B);

        if (bFound)
            FoundConnections.Add(Required);
    }

    FTimerHandle TimerHandler;
    const FName DeductionBoardId = ActiveBoardId;

    // Le collego alla deduzione
    GetWorld()->GetTimerManager().SetTimer(
        TimerHandler,
        FTimerDelegate::CreateWeakLambda(this, [this, Deduction, FoundConnections, DeductionBoardId]()
        {
            // Nel frattempo il giocatore ha cambiato board
            if (ActiveBoardId != DeductionBoardId)
                return;

            for (const FInvestigationConnection& Required : FoundConnections)
            {
                if (!IsConnected(Required.A, Required.B))
                    continue;

                if (!IsConnected(Required.A, Deduction.Id))
                    DrawConnection(Required.A, Deduction.Id);

                if (!IsConnected(Required.B, Deduction.Id))
                    DrawConnection(Required.B, Deduction.Id);
            }

            if (UAshley_InvestigationBoardSubsystem* Subsystem = GetBoardSubsystem())
                Subsystem->SaveToDisk();
        }),
        0.25f,
        false
    );

}

void AAshley_InvestigationBoardManager::TryConnect(FName A, FName B)
{
    if (!ActiveDefinition) return;

    FInvestigationConnection CandidateConnection = FInvestigationConnection(A, B);

    // Se non e' una connessione prevista, la ignoro
    if (!ActiveDefinition->Connections.Contains(CandidateConnection)) return;

    // Se e' gia' presente questa connessione, la ignoro
    if (IsConnected(A, B)) return;

    // La aggiungo alla stato corrente e visualizzo il nuovo collegamento
    DrawConnection(A, B);

    // Se questa connessione contribuisce a sbloccare delle deduzioni
    for (const FInvestigationPostIt& PostIt : ActiveDefinition->PostIts)
    {
        if (PostIt.bRequireDeduction &&
            !IsPostItVisible(PostIt.Id) &&
            ShouldRevealPostIt(PostIt))
        {
            DiscoverDeduction(PostIt);
        }
    }

    if (UAshley_InvestigationBoardSubsystem* Subsystem = GetBoardSubsystem())
        Subsystem->SaveToDisk();
}
