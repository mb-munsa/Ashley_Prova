// Ashley

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "Ashley_WidgetSystemConfigDA.generated.h"

class UAshley_InfoText;
class UAshley_PauseMenu;
class UAshley_InventoryUI;
class UAshley_SkipCutscene;
class UAshley_BaseHUD;
class UAshley_CollectableNotification;


UCLASS()
class ASHLEY_API UAshley_WidgetSystemConfigDA : public UDataAsset
{
	GENERATED_BODY()

public:
	//UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Widgets Prefabs")
	//TSubclassOf<UAshley_InfoText> InfoTextBP;

	//UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Widgets Prefabs")
	//TSubclassOf<UAshley_InfoText> InteractionPromptBP;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Widgets Prefabs")
	TSubclassOf<UAshley_PauseMenu> PauseMenuBP;

	//UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Widgets Prefabs")
	//TSubclassOf<UAshley_InventoryUI> InventoryBP;

	//UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Widgets Prefabs")
	//TSubclassOf<UAshley_SkipCutscene> SkipCutsceneBP;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Widgets Prefabs")
	TSubclassOf<UAshley_BaseHUD> BaseHUDBP;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Widgets Prefabs")
	TSubclassOf<UAshley_CollectableNotification> CollectableNotificationBP;
};
