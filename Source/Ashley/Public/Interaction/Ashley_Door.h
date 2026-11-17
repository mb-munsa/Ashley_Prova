// Ashley

#pragma once

#include "CoreMinimal.h"
#include "Interaction/Ashley_Openable.h"
#include "Ashley_Door.generated.h"

/**
 * 
 */
UCLASS()
class ASHLEY_API AAshley_Door : public AAshley_Openable
{
	GENERATED_BODY()

	public:
		AAshley_Door();
		
		// Override dell'interazione della classe base
		virtual void NotifyActorBeginOverlap(AActor* OtherActor) override;

	protected:

		UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Door|State")
		bool bDoorDirection;
		
		UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Door|Rotation")
		float Angle = 90.f;

		UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Door|Rotation")
		float OpenRotation = -90.f;
	
private:
		UFUNCTION()
		void SetDoorDirection(AActor* OtherActor );
	};

