// Ashley

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Ashley_ItemShowcase.generated.h"

class UAshley_PickupDA;
class UWidgetComponent;
class UCineCameraComponent;

USTRUCT(BlueprintType)
struct FItemMeshPair
{
	GENERATED_BODY()

public:

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Data", meta = (GetOptions = "GetValidIDs"))
	FName ItemName;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Data")
	UStaticMeshComponent* MeshComponent = nullptr;

	// Trasform iniziale della mesh, usata per resettare la rotazione a ogni ShowItem
	UPROPERTY()
	FTransform InitialRelativeTransform;
};

UCLASS()
class ASHLEY_API AAshley_ItemShowcase : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AAshley_ItemShowcase();

	UFUNCTION(BlueprintCallable)
	void ShowItem(FName ItemName);

	UFUNCTION(BlueprintCallable)
	void CloseShowcase();

	UFUNCTION(BlueprintCallable)
	void SetOverscan(float OverscanAmount);

	// Ruota l'oggetto mostrato attorno al suo centro, sugli assi della camera (X = yaw, Y = pitch)
	UFUNCTION(BlueprintCallable)
	void RotateItem(FVector2D Delta);

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

#if WITH_EDITOR
	UFUNCTION()
	TArray<FName> GetValidIDs() const;
#endif

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data")
	TArray<FItemMeshPair> ItemMap;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data")
	TSoftObjectPtr<UAshley_PickupDA> PickupDataAsset;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data")
	UWidgetComponent* CameraOverlay;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Variables")
	TObjectPtr<UCineCameraComponent> Camera;

	// Gradi di rotazione per unita' di input
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Rotation")
	float RotationSpeed = 1.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Rotation")
	bool bInvertPitch = false;

	UPROPERTY(Transient)
	TObjectPtr<UStaticMeshComponent> CurrentMesh;
};
