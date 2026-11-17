#pragma once

#include "Engine/StaticMesh.h"

#include "Ashley_MenuPickusStructs.generated.h"

UENUM(BlueprintType)
enum class EPickupType : uint8
{
    COLLECTABLE UMETA(DisplayName = "Collectable"),
    OBJECT UMETA(DisplayName = "Object")
};

USTRUCT(BlueprintType)
struct FInventoryInfo
{
    GENERATED_BODY()

public:

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    EPickupType ItemMainType = EPickupType::OBJECT;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FText ItemDescription;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FText ItemName;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    UTexture2D* Icon = nullptr;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    bool CanBeStacked = true;
};

USTRUCT(BlueprintType)
struct FItemStack
{
    GENERATED_BODY()

public:

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    int StackSize = 1;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FName ItemID;
};

