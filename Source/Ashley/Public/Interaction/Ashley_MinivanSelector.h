// Ashley

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"

#include <Interaction/Ashley_ClickableInterface.h>
#include "Managers/Ashley_MinivanManager.h"

#include "Ashley_MinivanSelector.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnSelectorClicked, AAshley_MinivanSelector*, Selector);

UCLASS()
class ASHLEY_API AAshley_MinivanSelector : public AActor, public IAshley_ClickableInterface
{
	GENERATED_BODY()

public:
	AAshley_MinivanSelector();

	virtual void Click_Implementation() override;

	UFUNCTION(BlueprintCallable)
	void SetCollisionEnabled(bool bEnabled);

	EAshley_MinivanView GetSceneToFocus() const { return SceneToFocus; }

	UPROPERTY(BlueprintAssignable, BlueprintCallable)
	FOnSelectorClicked OnSelectorClicked;

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data|Interactable")
	UStaticMeshComponent* Mesh;

	// Se assegnato, al click passo il Manager alla vista SceneToFocus
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data|Interactable")
	TObjectPtr<AAshley_MinivanManager> Manager;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data|Interactable")
	EAshley_MinivanView SceneToFocus = EAshley_MinivanView::GeneralView;
};
