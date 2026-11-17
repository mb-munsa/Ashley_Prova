// Ashley

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Ashley_DamageVignette.generated.h"

class UImage;

/**
 * 
 */
UCLASS()
class ASHLEY_API UAshley_DamageVignette : public UUserWidget
{
	GENERATED_BODY()

public:

	virtual void NativeConstruct() override;

	UFUNCTION(BlueprintCallable)
	void SetIntensity(float NewIntensity);

	UFUNCTION(BlueprintCallable)
	void SetColor(FLinearColor NewColor);

	UFUNCTION(BlueprintCallable)
	void SetOpacity(float NewOpacity);
	
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	UImage* VignetteImage;

protected:

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UMaterialInstanceDynamic> VignetteMID;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<UMaterial> VignetteMaterial;

};
