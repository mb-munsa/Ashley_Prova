// Ashley

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "Tags_Enums_Structs/Ashley_GameplayTags.h"

#include "Ashley_InputConfigDA.generated.h"

class UInputAction;
class UInputMappingContext;

UENUM(BlueprintType)
enum class EAshley_InputMappingContext : uint8
{
	GAMEPLAY UMETA(DisplayName = "Standard Gameplay"),
	DIALOGUE UMETA(DisplayName = "Dialogue"),
	MOUSE_PUZZLE UMETA(DisplayName = "Puzzle mouse controlled"),
	CUTSCENE UMETA(DisplayName = "Cutscenes"),
	CARRYING_OBJECT UMETA(DisplayName = "Carrying object in hand"),
	EXAMINING_OBJECT UMETA(DisplayName = "Examining object")
};

USTRUCT(BlueprintType)
struct FAshley_InputActionConfig
{
	GENERATED_BODY()

public:

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (Categories = "Ashley.InputAction"))
	FGameplayTag InputTag;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	UInputAction* InputAction = nullptr;
};

USTRUCT(BlueprintType)
struct FAshley_MappingContext
{
	GENERATED_BODY()

public:

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	EAshley_InputMappingContext InputContextName;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	UInputMappingContext* MappingContext = nullptr;
};

UCLASS()
class ASHLEY_API UAshley_InputConfigDA : public UDataAsset
{
	GENERATED_BODY()
	
public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	UInputMappingContext* DefaultMappingContext;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TArray<FAshley_MappingContext> MappingContexts;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (TitleProperty = "InputTag"))
	TArray<FAshley_InputActionConfig> NativeInputActions;

	UFUNCTION()
	UInputAction* FindNativeInputActionByTag(const FGameplayTag& InInputTag) const;

	UFUNCTION()
	UInputMappingContext* GetMappingContextByName(const EAshley_InputMappingContext& Name) const;

};
