#pragma once

#include "CoreMinimal.h"
#include "Ashley_WidgetData.generated.h"


UENUM(BlueprintType)
enum class EWidgetType : uint8
{
	HUD,
	INTERACTION_PROMPT,
	INFO_TEXT,
	INVENTORY,
	PAUSE_MENU,
	NOTIFICATION
};

static TMap<EWidgetType, int16> WidgetZOrders =
{
	{EWidgetType::HUD, 1},
	{EWidgetType::INTERACTION_PROMPT, 200},
	{EWidgetType::INFO_TEXT, 300},
	{EWidgetType::INVENTORY, 400},
	{EWidgetType::PAUSE_MENU, 500},
	{EWidgetType::NOTIFICATION, 600}
};
