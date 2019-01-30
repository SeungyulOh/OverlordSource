// Fill out your copyright notice in the Description page of Project Settings.

#include "ProjectR.h"
#include "ArenaPlayerController.h"
#include "ProjectRGameMode.h"
#include "GlobalIntegrated.h"
#include "GameInfo/PlayStateInfo.h"
#include "Character/BaseAIController.h"
#include "Character/Components/SkillAreaIndicatorMeshComponent.h"
#include "Character/Components/AnimStateComponent.h"
#include "Character/Components/FunctionalComponents/SkillEntityComponent.h"
#include "Character/Components/FunctionalComponents/BattleEntityComponent.h"
#include "Character/Components/FunctionalComponents/EntityRenderComponent.h"
#include "Character/Components/SelectionIndicatorMeshComponent.h"
#include "Character/Components/WidgetDecoratorComponent.h"
#include "Character/AnimState/IAnimState.h"
#include "Character/Components/AvoidanceComponent.h"
#include "UtilFunctionIntegrated.h"
#include "Object/BaseCameraActor.h"
#include "CustomStructures/SharedStructures/CharacterBattleInfo.h"
#include "Skill_V2/SharedSkill/ISkill_V2.h"
#include "Network/HttpRequestClient.h"
#include "Input/NormalTouchInput.h"
#include "LeaderControlHelper.h"
#include "UI/UIDefs.h"
#include "UI/RWidgetManager.h"
#include "UI/UI_Battle/RCampaignHUD_ThreeControl.h"
#include "UI/UI_Battle/RSkillIcon.h"
#include "UI/UI_Battle/RAttackIcon.h"//temp
#include "UI/InGameHUD/UP_InGameHUD_Arena.h"
#include "UI/Common/UC_Particle.h"


#define DPAD_DEBUG 0

AArenaPlayerController::AArenaPlayerController(const FObjectInitializer& ObjectInitializer)
	: Super( ObjectInitializer )
{
}