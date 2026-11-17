// Ashley


#include "Core/Ashley_QuestObject.h"

#include "DataAssets/Ashley_QuestDA.h"
#include "DataAssets/Ashley_PickupDA.h"

// Sets default values
AAshley_QuestObject::AAshley_QuestObject()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	//Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	//Mesh->SetupAttachment(RootComponent);

	// Configuro il layer per essere interagibile dal player
	Mesh->SetCollisionResponseToChannel(ECC_GameTraceChannel1, ECR_Block);
}

// Called when the game starts or when spawned
void AAshley_QuestObject::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void AAshley_QuestObject::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AAshley_QuestObject::Interact_Implementation() {}

TArray<FName> AAshley_QuestObject::GetItemsNames() const
{
    TArray<FName> ItemsNames;

    if (PickupsData)
    {
        for (const auto& PickUp : PickupsData->PickupItemsData)
        {
            ItemsNames.Add(PickUp.ItemID);
        }
    }

    return ItemsNames;
}

TArray<FName> AAshley_QuestObject::GetQuestNames() const
{
    TArray<FName> QuestNames;

    if (QuestsData)
    {
        for (const auto& Quest : QuestsData->Quests)
        {
            QuestNames.Add(Quest.QuestID);
        }
    }

    return QuestNames;
}

TArray<FString> AAshley_QuestObject::GetQuestSteps() const
{
    TArray<FString> QuestSteps;

    if (QuestsData && QuestName != NAME_None)
    {
        for (const auto& Quest : QuestsData->Quests)
        {
            if (Quest.QuestID == QuestName)
            {
                for (const auto& Step : Quest.QuestSteps)
                    QuestSteps.Add(Step.StepTitle);
            }
        }
        return QuestSteps;
    }

    return TArray<FString>();
}
