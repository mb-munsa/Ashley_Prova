// Ashley

#pragma once

#include "NativeGameplayTags.h"

namespace AshleyGameplayTags
{
	// Input Actions Tags
	ASHLEY_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Ashley_InputAction);
	ASHLEY_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Ashley_InputAction_Look);
	ASHLEY_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Ashley_InputAction_MousePress);

	ASHLEY_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Ashley_InputAction_Move);
	ASHLEY_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Ashley_InputAction_Run);
	ASHLEY_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Ashley_InputAction_Jump);
	ASHLEY_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Ashley_InputAction_Interact);
	ASHLEY_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Ashley_InputAction_Drop);
	ASHLEY_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Ashley_InputAction_Inventory);
	ASHLEY_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Ashley_InputAction_Esc);

	ASHLEY_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Ashley_InputAction_ExitPuzzle);
	ASHLEY_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Ashley_InputAction_StopExaminingObject);
	ASHLEY_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Ashley_InputAction_RotateShowcase);

	ASHLEY_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Ashley_InputAction_SkipCutscene);
	ASHLEY_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Ashley_InputAction_SkipDialogue);
	// END Input Actions Tags

	// Character Tags
	ASHLEY_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Ashley_Character_Player);
	// END Character Tags

	// GameplayEvents Tags
	ASHLEY_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Ashley_Events_PlayerEnterHouse);
	ASHLEY_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Ashley_Events_PlaySequence);
	ASHLEY_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Ashley_Events_EndSequence);
	ASHLEY_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Ashley_Events_EquipTorch);
	ASHLEY_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Ashley_Events_LevelEnd);
	ASHLEY_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Ashley_Events_ShowcaseClosed);

	// END Character Tags

	// Cutscenes Tags
	//Farmer
	ASHLEY_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Ashley_Cutscene_Farmer_Telephone);
	ASHLEY_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Ashley_Cutscene_Farmer_BlackHole);
	ASHLEY_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Ashley_Cutscene_Farmer_WifeSuicide);
	ASHLEY_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Ashley_Cutscene_Farmer_GarageDance);
	ASHLEY_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Ashley_Cutscene_Farmer_WifeQuestStart);
	ASHLEY_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Ashley_Cutscene_Farmer_WifeQuestEnd);
	//EvanApartment
	ASHLEY_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Ashley_Cutscene_EvanApartment_DoormanOnStairs);
	ASHLEY_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Ashley_Cutscene_EvanApartment_Blackout);
	ASHLEY_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Ashley_Cutscene_EvanApartment_BreakingWindow);
	ASHLEY_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Ashley_Cutscene_EvanApartment_DoormanCapture);
	ASHLEY_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Ashley_Cutscene_EvanApartment_BlockHallway);
	// END

	// GameState Flags
	ASHLEY_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Ashley_GameState_FirstResetDone);

	// Speaking voices Tags
	ASHLEY_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Ashley_Voice_Narrator);
	ASHLEY_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Ashley_Voice_Alex);
	ASHLEY_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Ashley_Voice_Doorman);
	ASHLEY_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Ashley_Voice_Neighbour);
}