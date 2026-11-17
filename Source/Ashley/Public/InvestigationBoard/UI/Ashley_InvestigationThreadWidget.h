#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Ashley_InvestigationThreadWidget.generated.h"

class UImage;

UCLASS()
class ASHLEY_API UAshley_InvestigationThreadWidget : public UUserWidget
{
    GENERATED_BODY()

public:
    void SetEndpoints(FVector2D Start, FVector2D End);

    // Multiplies the thread texture color
    void SetTint(const FLinearColor& Tint);

protected:

    void Update(FVector2D Start, FVector2D End);

    UPROPERTY(meta = (BindWidget))
    UImage* ThreadImage;

private:
    FVector2D StartPoint = FVector2D::ZeroVector;
    FVector2D EndPoint = FVector2D::ZeroVector;
};