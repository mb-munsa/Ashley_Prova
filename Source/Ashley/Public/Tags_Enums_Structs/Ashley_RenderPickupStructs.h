#pragma once
#include "CoreMinimal.h"
#include "Engine/StaticMesh.h"

#include "Ashley_RenderPickUpStructs.generated.h"


USTRUCT(BlueprintType)
struct FMaterialBySlot
{
    GENERATED_BODY()

public:

    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    int SlotNumber = 0;

    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    UMaterialInterface* Material = nullptr;
};

USTRUCT(BlueprintType)
struct FRenderPickup
{
    GENERATED_BODY()

public:

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    UStaticMesh* Mesh = nullptr;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    bool bNeedsMaterialAssigned = false;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (EditCondition = "bNeedsMaterialAssigned", EditConditionHides))
    TArray<FMaterialBySlot> MaterialsBySlot;
};
