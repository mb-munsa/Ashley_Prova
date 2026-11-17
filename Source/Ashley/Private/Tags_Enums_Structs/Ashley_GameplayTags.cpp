// Ashley


#include "Tags_Enums_Structs/Ashley_GameplayTags.h"

namespace AshleyGameplayTags
{
	// Input Actions Tags
	UE_DEFINE_GAMEPLAY_TAG(Ashley_InputAction, "Ashley.InputAction");
	UE_DEFINE_GAMEPLAY_TAG(Ashley_InputAction_Look, "Ashley.InputAction.Look");
	UE_DEFINE_GAMEPLAY_TAG(Ashley_InputAction_MousePress, "Ashley.InputAction.MousePress");

	UE_DEFINE_GAMEPLAY_TAG(Ashley_InputAction_Move, "Ashley.InputAction.Move");
	UE_DEFINE_GAMEPLAY_TAG(Ashley_InputAction_Run, "Ashley.InputAction.Run");
	UE_DEFINE_GAMEPLAY_TAG(Ashley_InputAction_Jump, "Ashley.InputAction.Jump");
	UE_DEFINE_GAMEPLAY_TAG(Ashley_InputAction_Interact, "Ashley.InputAction.Interact");
	UE_DEFINE_GAMEPLAY_TAG(Ashley_InputAction_Drop, "Ashley.InputAction.Drop");
	UE_DEFINE_GAMEPLAY_TAG(Ashley_InputAction_Inventory, "Ashley.InputAction.Inventory");
	UE_DEFINE_GAMEPLAY_TAG(Ashley_InputAction_Esc, "Ashley.InputAction.Esc");

	UE_DEFINE_GAMEPLAY_TAG(Ashley_InputAction_ExitPuzzle, "Ashley.InputAction.ExitPuzzle");
	UE_DEFINE_GAMEPLAY_TAG(Ashley_InputAction_StopExaminingObject, "Ashley.InputAction.StopExaminingObject");
	UE_DEFINE_GAMEPLAY_TAG(Ashley_InputAction_RotateShowcase, "Ashley.InputAction.RotateShowcase");

	UE_DEFINE_GAMEPLAY_TAG(Ashley_InputAction_SkipCutscene, "Ashley.InputAction.SkipCutscene");
	UE_DEFINE_GAMEPLAY_TAG(Ashley_InputAction_SkipDialogue, "Ashley.InputAction.SkipDialogue");

	// END Input Actions Tags

	// Character Tags
	UE_DEFINE_GAMEPLAY_TAG(Ashley_Character_Player, "Ashley.Character.Player");
	// END Character Tags

	// GameplayEvents Tags
	UE_DEFINE_GAMEPLAY_TAG(Ashley_Events_PlayerEnterHouse, "Ashley.Events.PlayerEnterHouse");
	UE_DEFINE_GAMEPLAY_TAG(Ashley_Events_PlaySequence, "Ashley.Events.PlaySequence");
	UE_DEFINE_GAMEPLAY_TAG(Ashley_Events_EndSequence, "Ashley.Events.EndSequence");
	UE_DEFINE_GAMEPLAY_TAG(Ashley_Events_EquipTorch, "Ashley.Events.EquipTorch");
	UE_DEFINE_GAMEPLAY_TAG(Ashley_Events_LevelEnd, "Ashley.Events.LevelEnd");
	UE_DEFINE_GAMEPLAY_TAG(Ashley_Events_ShowcaseClosed, "Ashley.Events.ShowcaseClosed");

	//END

	// Cutscenes
	//Farmer
	UE_DEFINE_GAMEPLAY_TAG(Ashley_Cutscene_Farmer_Telephone, "Ashley.Cutscene.Farmer.Telephone");
	UE_DEFINE_GAMEPLAY_TAG(Ashley_Cutscene_Farmer_BlackHole, "Ashley.Cutscene.Farmer.BlackHole");
	UE_DEFINE_GAMEPLAY_TAG(Ashley_Cutscene_Farmer_WifeSuicide, "Ashley.Cutscene.Farmer.WifeSuicide");
	UE_DEFINE_GAMEPLAY_TAG(Ashley_Cutscene_Farmer_GarageDance, "Ashley.Cutscene.Farmer.GarageDance");
	UE_DEFINE_GAMEPLAY_TAG(Ashley_Cutscene_Farmer_WifeQuestStart, "Ashley.Cutscene.Farmer.WifeQuestStart");
	UE_DEFINE_GAMEPLAY_TAG(Ashley_Cutscene_Farmer_WifeQuestEnd, "Ashley.Cutscene.Farmer.WifeQuestEnd");
	//EvanApartment
	UE_DEFINE_GAMEPLAY_TAG(Ashley_Cutscene_EvanApartment_DoormanOnStairs, "Ashley.Cutscene.EvanApartment.DoormanOnStairs");
	UE_DEFINE_GAMEPLAY_TAG(Ashley_Cutscene_EvanApartment_Blackout, "Ashley.Cutscene.EvanApartment.Blackout");
	UE_DEFINE_GAMEPLAY_TAG(Ashley_Cutscene_EvanApartment_BreakingWindow, "Ashley.Cutscene.EvanApartment.BreakingWindow");
	UE_DEFINE_GAMEPLAY_TAG(Ashley_Cutscene_EvanApartment_DoormanCapture, "Ashley.Cutscene.EvanApartment.DoormanCapture");
	UE_DEFINE_GAMEPLAY_TAG(Ashley_Cutscene_EvanApartment_BlockHallway, "Ashley.Cutscene.EvanApartment.BlockHallway");

	//END

	// GameState Flags
	UE_DEFINE_GAMEPLAY_TAG(Ashley_GameState_FirstResetDone, "Ashley.GameState.FirstResetDone");

	// Speaking voices Tags
	UE_DEFINE_GAMEPLAY_TAG(Ashley_Voice_Narrator, "Ashley.Voice.Narrator");
	UE_DEFINE_GAMEPLAY_TAG(Ashley_Voice_Alex, "Ashley.Voice.Alex");
	UE_DEFINE_GAMEPLAY_TAG(Ashley_Voice_Doorman, "Ashley.Voice.Doorman");
	UE_DEFINE_GAMEPLAY_TAG(Ashley_Voice_Neighbour, "Ashley.Voice.Neighbour");

}