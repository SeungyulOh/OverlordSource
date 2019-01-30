// Fill out your copyright notice in the Description page of Project Settings.

#include "ProjectR.h"
#include "RSkillSet_New.h"

#include "GameInfo/PlayStateInfo.h"

#include "UI/UI_Battle/RSkillIcon.h"
#include "UI/UI_Battle/RAttackIcon.h"
#include "UI/UI_Battle/Component/RFocusAttackIcon.h"
#include "UI/UI_Battle/Component/RSearchTargetIcon.h"
#include "UI/UI_Battle/Component/RMeetingIcon.h"
#include "UI/UI_Battle/RCampaignHUD_ThreeControl.h"
#include "UI/UI_Battle/InGameHeroUI/HeroUIforBase.h"
#include "UI/UI_Battle/InGameHeroUI/HeroUIforCrew.h"
#include "UI/UI_Battle/InGameHeroUI/HeroUIforLeader.h"
#include "UI/UI_Battle/RCancelIcon.h"

#include "UI/InGameHUD/SkillIcon_New.h"
#include "RSpiritIcon.h"

#include "UtilFunctionIntegrated.h"

#include "ContentGameMode/Controllers/BasePlayerController.h"
#include "ContentGameMode/Controllers/StagePlayerController.h"

#include "Global/InputManager.h"
#include "Global/RealTimeModManager.h"

#include "Runtime/Engine/Classes/Engine/UserInterfaceSettings.h"
#include "Runtime/UMG/Public/Components/SafeZone.h"
#include "Blueprint/WidgetLayoutLibrary.h"

#include "Network/RTSManager.h"

#include "Skill_V2/SharedSkill/ISkill_V2.h"

#include "UI/UI_Battle/RVirtualJoystick.h"

#include "DirectionTrackManager.h"

#include "Character/Components/SkillAreaIndicatorMeshComponent.h"




void URSkillSet_New::NativeConstruct()
{
	Super::NativeConstruct();

	/*variables settings*/
	Variables.Prepare();
	/*variables settings End*/

	/*Blackboard Settings*/
	Blackboard.PlayStateInfo = RGAMEINSTANCE(this)->PlayStateInfo;
	Blackboard.BlackBoardArray.Init(false, (int32)ESkillIconSeq::MAX);
	Blackboard.CurrentTouchPos = FVector2D::ZeroVector;
	Blackboard.InitialTouchPos = FVector2D::ZeroVector;
	Blackboard.bCrewDeadState = false;
	/*Blackboard Settings End*/

	/*FSM Settings*/
	FSM.variables = &Variables;
	FSM.blackboard = &Blackboard;
	FSM.Prepare();
	/*FSM Settings End*/

	/*Renderer Settings*/
	Renderer.variables = &Variables;
	Renderer.blackboard = &Blackboard;
	Renderer.StateMachine = &FSM;
	Renderer.Prepare();
	/*Renderer Settings End*/


	/*Binding Delegates*/
	RGAMEINSTANCE(this)->InputManager->OnInputTouchBeginDelegate.RemoveDynamic(this, &URSkillSet_New::TouchBegin);
	RGAMEINSTANCE(this)->InputManager->OnInputTouchBeginDelegate.AddDynamic(this, &URSkillSet_New::TouchBegin);

	RGAMEINSTANCE(this)->InputManager->OnInputTouchOverDelegate.RemoveDynamic(this, &URSkillSet_New::TouchOver);
	RGAMEINSTANCE(this)->InputManager->OnInputTouchOverDelegate.AddDynamic(this, &URSkillSet_New::TouchOver);

	RGAMEINSTANCE(this)->InputManager->OnInputTouchEndDelegate.RemoveDynamic(this, &URSkillSet_New::TouchEnd);
	RGAMEINSTANCE(this)->InputManager->OnInputTouchEndDelegate.AddDynamic(this, &URSkillSet_New::TouchEnd);

	RGAMEINSTANCE(this)->EventManager->OnCrewDead.RemoveDynamic(this, &URSkillSet_New::OnCrewDead);
	RGAMEINSTANCE(this)->EventManager->OnCrewDead.AddDynamic(this, &URSkillSet_New::OnCrewDead);
	/*Binding Delegates End*/
}

void URSkillSet_New::NativeDestruct()
{
	Super::NativeDestruct();
}



void URSkillSet_New::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);
	
	// update == false means that state transition happened
	if(!FSM.Update())
		Renderer.Render();
}



void URSkillSet_New::TouchBegin(ETouchIndex::Type Intype, FVector Inlocation)
{
	FIntPoint CheckPos(Inlocation.X, Inlocation.Y);
	for (int32 i = 0 ; i < Variables.SkillIconPosArray.Num() ; ++i)
	{
		if (Variables.SkillIconPosArray[i].Contains(CheckPos))
		{
			if (Variables.SkillIconArray.IsValidIndex(i))
			{
				if (!Variables.SkillIconArray[i]->IsVisible())
					continue;

				//if playalone mode
				UBaseStageInfo* BaseStageInfo = UGamePlayBluePrintFunction::GetBaseStageInfo();
				if (BaseStageInfo && BaseStageInfo->bPlayAlone && i >= (int32)ESkillIconSeq::CREW1_ACTIVE3)
					return;

				USkillIcon_New* skillicon = Variables.SkillIconArray[i];
				if (skillicon)
				{
					if (skillicon->Blackboard.CurrentCoolTime > 0)
						return;

					if (skillicon->Blackboard.CurrentState == EIconState::LOCK || 
						skillicon->Blackboard.CurrentState == EIconState::DEAD || 
						skillicon->Blackboard.CurrentState == EIconState::ABNORMAL)
						return;

					Blackboard.ClearBlackboardData();
					Blackboard.SetBlackboardData((ESkillIconSeq)i, true);
					Blackboard.InitialTouchPos = *(FVector2D*)&Inlocation;
					Blackboard.CurrentTouchPos = Blackboard.InitialTouchPos;
				}
			}				
		}
	}
}

void URSkillSet_New::TouchOver(ETouchIndex::Type Intype, FVector Inlocation)
{
	Blackboard.CurrentTouchPos = *(FVector2D*)&Inlocation;
}

void URSkillSet_New::TouchEnd(ETouchIndex::Type Intype, FVector Inlocation)
{
	ESkillIconSeq CurrentSkillIconSeq = FSM.GetCurrentSkillIconSeq();
	if (CurrentSkillIconSeq != ESkillIconSeq::MAX)
		Blackboard.SetBlackboardData(CurrentSkillIconSeq, false);
}

void URSkillSet_New::OnCrewDead(int32 SlotIdx)
{
	int32 RestIdx = SlotIdx % 2 == 1 ? SlotIdx + 1 : SlotIdx - 1;
	FHeroSlotInfo slotinfo = Blackboard.PlayStateInfo->GetSelectedCharacterSlot(RestIdx);
	if (slotinfo.SlotSpawnState == EHeroSpawnStateEnum::VE_Dead)
	{
		ECharacterRoleEnum CharacterRole = SlotIdx <= ROLE_CREW1_REST_INDEX ? ECharacterRoleEnum::VE_Crew1 : ECharacterRoleEnum::VE_Crew2;

		if (CharacterRole == ECharacterRoleEnum::VE_Crew1)
			Variables.SkillIconArray[(int32)ESkillIconSeq::CREW1_ACTIVE3]->Blackboard.SetBlackboardData(EIconState::DEAD, true);
		else
			Variables.SkillIconArray[(int32)ESkillIconSeq::CREW2_ACTIVE3]->Blackboard.SetBlackboardData(EIconState::DEAD, true);

		return;
	}

	Blackboard.SetDeadData_OnBlackboard(true , SlotIdx);
}

class UISkill_V2* URSkillSet_New::GetCurrentSkill()
{
	USkillIcon_New* skillicon = FSM.CurrentState->SelectedSkillIcon;
	if (skillicon->IsValidLowLevel())
	{
		USkillEntityComponent* skillentitycomp = UCharacterHelpFunctionLibrary::GetSkillEntityComponent(skillicon->Blackboard.OwnerCharacter.Get());
		if (skillentitycomp)
		{
			UISkill_V2* skill = skillentitycomp->GetSkillByCategory_V2(skillicon->Blackboard.SkillCategory);
			if (skill->IsValidLowLevel())
				return skill;
		}
	}
	return nullptr;
}

void FRenderer::Prepare()
{
	UBaseStageInfo* BaseStageInfo = UGamePlayBluePrintFunction::GetBaseStageInfo();
	if (BaseStageInfo->bPlayAlone)
	{
		for (size_t i = 0; i < variables->SkillIconArray.Num(); ++i)
		{
			if (variables->SkillIconArray.IsValidIndex(i))
			{
				if (i == (size_t)ESkillIconSeq::LEADER_ACTIVE1)
					variables->SkillIconArray[i]->Renderer.Prepare(blackboard->PlayStateInfo->GetSelectedCharacterSlot(ROLE_LEADER_INDEX), ESkillCategoryTypeEnum::VE_ActiveSkill01);
				else if (i == (size_t)ESkillIconSeq::LEADER_ACTIVE2)
					variables->SkillIconArray[i]->Renderer.Prepare(blackboard->PlayStateInfo->GetSelectedCharacterSlot(ROLE_LEADER_INDEX), ESkillCategoryTypeEnum::VE_ActiveSkill02);
				else if (i == (size_t)ESkillIconSeq::LEADER_ACTIVE3)
					variables->SkillIconArray[i]->Renderer.Prepare(blackboard->PlayStateInfo->GetSelectedCharacterSlot(ROLE_LEADER_INDEX), ESkillCategoryTypeEnum::VE_ActiveSkill03);
				else
					variables->SkillIconArray[i]->SetVisibility(ESlateVisibility::Collapsed);
			}
		}

		if (variables->BG_3Combine)
			variables->BG_3Combine->SetVisibility(ESlateVisibility::Collapsed);
		if (variables->Crew_SpiritBoxSwitcher)
			variables->Crew_SpiritBoxSwitcher->SetActiveWidgetIndex(1);
		if (variables->Spirit1SkillIcon)
			variables->Spirit1SkillIcon->Renderer.Prepare(blackboard->PlayStateInfo->GetSelectedCharacterSlot(ROLE_CREW1_REST_INDEX));
		if (variables->Spirit2SkillIcon)
			variables->Spirit2SkillIcon->Renderer.Prepare(blackboard->PlayStateInfo->GetSelectedCharacterSlot(ROLE_CREW2_REST_INDEX));
		if (variables->FocusAttackIcon)
			variables->FocusAttackIcon->SetVisibility(ESlateVisibility::Collapsed);
		if (variables->MeetingIcon)
			variables->MeetingIcon->SetVisibility(ESlateVisibility::Collapsed);
	}
	else
	{
		if (URTSManager::GetInstancePtr()->IsConnectedMode(EPVP::CS))
		{
			for (size_t i = 0; i < variables->SkillIconArray.Num(); ++i)
			{
				if (variables->SkillIconArray.IsValidIndex(i))
				{
					if (i == (size_t)ESkillIconSeq::LEADER_ACTIVE1)
						variables->SkillIconArray[i]->Renderer.Prepare(blackboard->PlayStateInfo->GetSelectedCharacterSlot(ROLE_LEADER_INDEX), ESkillCategoryTypeEnum::VE_ActiveSkill01);
					else if (i == (size_t)ESkillIconSeq::LEADER_ACTIVE2)
						variables->SkillIconArray[i]->Renderer.Prepare(blackboard->PlayStateInfo->GetSelectedCharacterSlot(ROLE_LEADER_INDEX), ESkillCategoryTypeEnum::VE_ActiveSkill02);
					else if (i == (size_t)ESkillIconSeq::LEADER_ACTIVE3)
						variables->SkillIconArray[i]->Renderer.Prepare(blackboard->PlayStateInfo->GetSelectedCharacterSlot(ROLE_LEADER_INDEX), ESkillCategoryTypeEnum::VE_ActiveSkill03);
					/*else if (i == (size_t)ESkillIconSeq::CREW1_ACTIVE3)
						variables->SkillIconArray[i]->Renderer.Prepare(blackboard->PlayStateInfo->GetSelectedCharacterSlot(ROLE_CREW1_BATTLE_INDEX), ESkillCategoryTypeEnum::VE_ActiveSkill03);
					else if (i == (size_t)ESkillIconSeq::CREW2_ACTIVE3)
						variables->SkillIconArray[i]->Renderer.Prepare(blackboard->PlayStateInfo->GetSelectedCharacterSlot(ROLE_CREW2_BATTLE_INDEX), ESkillCategoryTypeEnum::VE_ActiveSkill03);*/
					else if (i == (size_t)ESkillIconSeq::CREW1_ACTIVE2)
						variables->SkillIconArray[i]->Renderer.Prepare(blackboard->PlayStateInfo->GetSelectedCharacterSlot(ROLE_CREW1_BATTLE_INDEX), ESkillCategoryTypeEnum::VE_ActiveSkill02);
					else if (i == (size_t)ESkillIconSeq::CREW2_ACTIVE2)
						variables->SkillIconArray[i]->Renderer.Prepare(blackboard->PlayStateInfo->GetSelectedCharacterSlot(ROLE_CREW2_BATTLE_INDEX), ESkillCategoryTypeEnum::VE_ActiveSkill02);
					else
						variables->SkillIconArray[i]->SetVisibility(ESlateVisibility::Collapsed);
				}
			}
			if (variables->BG_3Combine)
				variables->BG_3Combine->SetVisibility(ESlateVisibility::Collapsed);
		}
		else
		{
			for (size_t i = 0; i < variables->SkillIconArray.Num(); ++i)
			{
				if (variables->SkillIconArray.IsValidIndex(i))
				{
					if (i == (size_t)ESkillIconSeq::LEADER_ACTIVE1)
						variables->SkillIconArray[i]->Renderer.Prepare(blackboard->PlayStateInfo->GetSelectedCharacterSlot(ROLE_LEADER_INDEX), ESkillCategoryTypeEnum::VE_ActiveSkill01);
					else if (i == (size_t)ESkillIconSeq::LEADER_ACTIVE2)
						variables->SkillIconArray[i]->Renderer.Prepare(blackboard->PlayStateInfo->GetSelectedCharacterSlot(ROLE_LEADER_INDEX), ESkillCategoryTypeEnum::VE_ActiveSkill02);
					else if (i == (size_t)ESkillIconSeq::LEADER_ACTIVE3)
						variables->SkillIconArray[i]->Renderer.Prepare(blackboard->PlayStateInfo->GetSelectedCharacterSlot(ROLE_LEADER_INDEX), ESkillCategoryTypeEnum::VE_ActiveSkill03);
					else if (i == (size_t)ESkillIconSeq::CREW1_ACTIVE3)
						variables->SkillIconArray[i]->Renderer.Prepare(blackboard->PlayStateInfo->GetSelectedCharacterSlot(ROLE_CREW1_BATTLE_INDEX), ESkillCategoryTypeEnum::VE_ActiveSkill03);
					else if (i == (size_t)ESkillIconSeq::CREW2_ACTIVE3)
						variables->SkillIconArray[i]->Renderer.Prepare(blackboard->PlayStateInfo->GetSelectedCharacterSlot(ROLE_CREW2_BATTLE_INDEX), ESkillCategoryTypeEnum::VE_ActiveSkill03);
					else if (i == (size_t)ESkillIconSeq::CREW1_ACTIVE2)
						variables->SkillIconArray[i]->Renderer.Prepare(blackboard->PlayStateInfo->GetSelectedCharacterSlot(ROLE_CREW1_REST_INDEX), ESkillCategoryTypeEnum::VE_ActiveSkill02);
					else if (i == (size_t)ESkillIconSeq::CREW2_ACTIVE2)
						variables->SkillIconArray[i]->Renderer.Prepare(blackboard->PlayStateInfo->GetSelectedCharacterSlot(ROLE_CREW2_REST_INDEX), ESkillCategoryTypeEnum::VE_ActiveSkill02);
				}
			}
			if (variables->BG_3Combine)
				variables->BG_3Combine->SetVisibility(ESlateVisibility::Collapsed);
		}
	}
	
}

void FRenderer::Render()
{
	if (blackboard->PlayStateInfo)
	{
		AActor* Leader = blackboard->PlayStateInfo->GetBattleActiveCrewActor(ECharacterRoleEnum::VE_Leader);
		if (IsValid(Leader))
		{
			for (size_t i = 0; i < variables->SkillIconArray.Num(); ++i)
			{
				if (i == (int32)ESkillIconSeq::CREW1_ACTIVE2 || i == (int32)ESkillIconSeq::CREW2_ACTIVE2)
					continue;

				AActor* TargetActor = variables->SkillIconArray[i]->Blackboard.OwnerCharacter.Get();
				if (IsValid(TargetActor))
				{
					if (UUtilFunctionLibrary::IsInUnActableState(TargetActor))
					{
						variables->SkillIconArray[i]->Blackboard.SetBlackboardData(EIconState::ABNORMAL, true);
						
						if(TargetActor == Leader)
							blackboard->ClearBlackboardData();
					}
					else
						variables->SkillIconArray[i]->Blackboard.SetBlackboardData(EIconState::ABNORMAL, false);
				}
			}
		}
	}

	if (StateMachine)
	{
		EStateEnum CurrentState = StateMachine->GetCurrentState();
		switch (CurrentState)
		{
			case EStateEnum::IDLE :
			{
				if (variables->SkillJoystick->GetVisibility() != ESlateVisibility::Collapsed)
					variables->SkillJoystick->SetVisibility(ESlateVisibility::Collapsed);

				ChangeOpacityExceptFor(nullptr, FLinearColor(1.f, 1.f, 1.f, 1.f));
				break;
			}

			case EStateEnum::LEADERSKILL:
			case EStateEnum::CREWSKILL3:
			case EStateEnum::CREWSKILL2:
			case EStateEnum::CREWDROP:
			{

				if (variables->SkillJoystick)
				{
					if (variables->SkillJoystick->GetVisibility() != ESlateVisibility::SelfHitTestInvisible)
						variables->SkillJoystick->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
				}

				ESkillIconSeq skillidx = StateMachine->GetCurrentSkillIconSeq();
				ChangeOpacityExceptFor(variables->SkillIconArray[(int32)skillidx], FLinearColor(1.f, 1.f, 1.f, 0.4));
				break;
			}
		}
	}
}

void FRenderer::ChangeOpacityExceptFor(USkillIcon_New* InException , FLinearColor InColor)
{
	for (size_t i = 0; i < variables->SkillIconArray.Num(); ++i)
	{
		if (variables->SkillIconArray[i] == InException)
			continue;

		variables->SkillIconArray[i]->SetColorAndOpacity(InColor);
	}

	if (variables->AttackIcon)
		variables->AttackIcon->SetColorAndOpacity(InColor);

	if (variables->FocusAttackIcon)
		variables->FocusAttackIcon->SetColorAndOpacity(InColor);

	if (variables->SearchTargetIcon)
		variables->SearchTargetIcon->SetColorAndOpacity(InColor);

	if (variables->MeetingIcon)
		variables->MeetingIcon->SetColorAndOpacity(InColor);
}

void FStateMachine::Prepare()
{
	UUserInputState* IdleState = NewObject<UIdle_UserInputState>();
	IdleState->SetBlackboard(blackboard);
	IdleState->SetVariables(variables);
	StateMap.Emplace(EStateEnum::IDLE, IdleState);

	UUserInputState* LeaderSkillState = NewObject<ULeaderSkill_UserInputState>();
	LeaderSkillState->SetBlackboard(blackboard);
	LeaderSkillState->SetVariables(variables);
	StateMap.Emplace(EStateEnum::LEADERSKILL, LeaderSkillState);

	UUserInputState* CrewSkillState = NewObject<UCrewSkill3_UserInputState>();
	CrewSkillState->SetBlackboard(blackboard);
	CrewSkillState->SetVariables(variables);
	StateMap.Emplace(EStateEnum::CREWSKILL3, CrewSkillState);

	UUserInputState* CrewSkill2State = NewObject<UCrewSkill2_UserInputState>();
	CrewSkill2State->SetBlackboard(blackboard);
	CrewSkill2State->SetVariables(variables);
	StateMap.Emplace(EStateEnum::CREWSKILL2, CrewSkill2State);

	UUserInputState* CrewDeadState = NewObject<UCrewDead_UserInputState>();
	CrewDeadState->SetBlackboard(blackboard);
	CrewDeadState->SetVariables(variables);
	StateMap.Emplace(EStateEnum::CREWDEAD, CrewDeadState);

	UUserInputState* CrewDropState = NewObject<UCrewDrop_UserInputState>();
	CrewDropState->SetBlackboard(blackboard);
	CrewDropState->SetVariables(variables);
	StateMap.Emplace(EStateEnum::CREWDROP, CrewDropState);

	CurrentState = IdleState;
	
}

bool FStateMachine::Update()
{
	if (!CurrentState->IsValidLowLevel())
		return false;

	 EStateEnum NextStateEnum = CurrentState->Update();
	 EStateEnum CurrentStateEnum = *(StateMap.FindKey(CurrentState));

	if (NextStateEnum == CurrentStateEnum || NextStateEnum == EStateEnum::END)
		return false;

	//transition
	if (StateMap.Contains(NextStateEnum))
	{
		if (CurrentState)
			CurrentState->Leave();

		CurrentState = StateMap[NextStateEnum];
		for (size_t i = 0; i < blackboard->BlackBoardArray.Num(); ++i)
		{
			if (blackboard->BlackBoardArray[i] == true)
			{
				CurrentState->SetSkillIconSeq((ESkillIconSeq)i);
				if (i == (int32)ESkillIconSeq::LEADER_ACTIVE1 || i == (int32)ESkillIconSeq::LEADER_ACTIVE2 || i == (int32)ESkillIconSeq::LEADER_ACTIVE3)
					CurrentState->SetOwnerRole(ECharacterRoleEnum::VE_Leader);
				else if(i == (int32)ESkillIconSeq::CREW1_ACTIVE2 || i == (int32)ESkillIconSeq::CREW1_ACTIVE3)
					CurrentState->SetOwnerRole(ECharacterRoleEnum::VE_Crew1);
				else if(i == (int32)ESkillIconSeq::CREW2_ACTIVE2 || i == (int32)ESkillIconSeq::CREW2_ACTIVE3)
					CurrentState->SetOwnerRole(ECharacterRoleEnum::VE_Crew2);
				break;
			}
		}
		CurrentState->Enter();
	}
	return true;
}

void UIdle_UserInputState::Enter()
{
// 	URGameInstance* GameInstance = RGAMEINSTANCE(GEngine);
// 	checkf(GameInstance != nullptr, TEXT("ULeaderControlHelper[RGameInstance Is Empty]"));
// 	AStagePlayerController* BasePC = Cast<AStagePlayerController>(UUtilFunctionLibrary::GetBasePlayerController(GameInstance->GetWorld()));
// 	if (!IsValid(BasePC))
// 		return;
// 
// 	BasePC->ActorSettingOnTheCamera(BasePC->GetLeaderCharacter());
}

void UIdle_UserInputState::Leave()
{

}

EStateEnum UIdle_UserInputState::Update()
{
	return CheckBlackboardData();
}

void FVariables::Prepare()
{
	/*Set SkillIcon Positions*/
	InitUIPos();
}

void FVariables::InitUIPos()
{
	SkillIconPosArray.Reset(7);

	URSkillSet_New* SkillSet = Cast<URSkillSet_New>(SkillIconArray[0]->GetParentWidget());

	UCanvasPanelSlot* StartCanvasSlot = nullptr;
	FVector2D ParentPos = UUIFunctionLibrary::GetWidgetPos(SkillSet);
	UE_LOG(LogClass, Log, TEXT("ParentPost, x : %f , y : %f"), ParentPos.X, ParentPos.Y);
	float viewScale = UWidgetLayoutLibrary::GetViewportScale(SkillSet->GetWorld());

	const FVector2D viewportSize = FVector2D(GEngine->GameViewport->Viewport->GetSizeXY());
	viewScale = GetDefault<UUserInterfaceSettings>(UUserInterfaceSettings::StaticClass())->GetDPIScaleBasedOnSize(FIntPoint(viewportSize.X, viewportSize.Y));

	for (auto Element : SkillIconArray)
	{
		StartCanvasSlot = UWidgetLayoutLibrary::SlotAsCanvasSlot(Element);
		if (StartCanvasSlot)
		{
			int32		left = (StartCanvasSlot->GetPosition().X + ParentPos.X) * viewScale;
			int32		top = (StartCanvasSlot->GetPosition().Y + ParentPos.Y) * viewScale;
			int32		right = left + (StartCanvasSlot->GetSize().X * viewScale);
			int32		bottom = top + (StartCanvasSlot->GetSize().Y * viewScale);

			FIntRect Rect(left - safe_margin.Left, top - safe_margin.Top, right - safe_margin.Right, bottom - safe_margin.Bottom);
			SkillIconPosArray.Emplace(Rect);
		}
	}

	/*Spirits*/
	StartCanvasSlot = UWidgetLayoutLibrary::SlotAsCanvasSlot(Crew_SpiritBoxSwitcher);
	if (StartCanvasSlot)
	{
		int32		left = (int32)(StartCanvasSlot->GetPosition().X + ParentPos.X) * viewScale;
		int32		top = (int32)(StartCanvasSlot->GetPosition().Y + ParentPos.Y) * viewScale;
		int32		Height = StartCanvasSlot->GetSize().Y * 0.5f * viewScale;

		int32		right = (int32)(left + (StartCanvasSlot->GetSize().X * viewScale));
		int32		bottom = top + Height;


		FIntRect Rect(left - safe_margin.Left, top - safe_margin.Top, right - safe_margin.Right, bottom - safe_margin.Bottom);
		SkillIconPosArray.Emplace(Rect);

		Rect.Min.Y = top + Height;
		Rect.Max.Y = top + Height + Height;
		SkillIconPosArray.Emplace(Rect);
	}
}

void FBlackboard_Skillset::SetBlackboardData(ESkillIconSeq Indata, bool Inbool)
{
	if (BlackBoardArray.IsValidIndex((int32)Indata))
	{
		BlackBoardArray[(int32)Indata] = Inbool;
	}
}



void FBlackboard_Skillset::ClearBlackboardData()
{
	BlackBoardArray.Init(false, (int32)ESkillIconSeq::MAX);
	bCrewDeadState = false;
	bFingerOnCancelButton = false;
}

void FBlackboard_Skillset::SetDeadData_OnBlackboard(bool isDead, int32 slotIdx)
{
	bCrewDeadState = isDead;
	SlotIdx = slotIdx;
}

void UUserInputState::Enter()
{
	SelectedSkillIcon = variables->SkillIconArray[(int32)SkillIconSeq];
	if (!SelectedSkillIcon->IsValidLowLevel() || !SelectedSkillIcon->isDPadEnable())
		return;

	USkillEntityComponent* SkillEntityComponent = UCharacterHelpFunctionLibrary::GetSkillEntityComponent(SelectedSkillIcon->Blackboard.OwnerCharacter.Get());
	if (!SkillEntityComponent)
		return;
	UISkill_V2* skill = SkillEntityComponent->GetSkillByCategory_V2(SelectedSkillIcon->Blackboard.SkillCategory);
	if (!skill)
		return;

	SkillEntityComponent->SetNeedTargetSkillID(skill->GetSkillID());
	SkillEntityComponent->SetVisibleSkillArea_V2(true);
	//check if this skill need to use skill Dpad
	if (variables->SkillJoystick->IsValidLowLevel())
	{
		variables->SkillJoystick->InitTouchPos(blackboard->InitialTouchPos.X, blackboard->InitialTouchPos.Y);

		if (SelectedSkillIcon->IsValidLowLevel())
		{
			UCanvasPanelSlot* StartCanvasSlot = nullptr;
			if(SkillIconSeq == ESkillIconSeq::CREW1_ACTIVE2 || SkillIconSeq == ESkillIconSeq::CREW2_ACTIVE2)
				StartCanvasSlot = UWidgetLayoutLibrary::SlotAsCanvasSlot(variables->Crew_SpiritBoxSwitcher);
			else
				StartCanvasSlot = UWidgetLayoutLibrary::SlotAsCanvasSlot(SelectedSkillIcon);

			FVector2D StartPosition = StartCanvasSlot->GetPosition() /*+ (StartCanvasSlot->GetSize() * 0.5f)*/;
			FVector2D Size = StartCanvasSlot->GetSize();
			StartPosition += (Size * 0.5f);
			variables->SkillJoystick->ComputePosition(StartPosition);
		}
	}

	variables->CancelIcon->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
}

void UUserInputState::Leave()
{
	if (SelectedSkillIcon)
	{
		USkillEntityComponent* SkillEntityComponent = UCharacterHelpFunctionLibrary::GetSkillEntityComponent(SelectedSkillIcon->Blackboard.OwnerCharacter.Get());
		if (IsValid(SkillEntityComponent))
		{
			SkillEntityComponent->SetVisibleSkillArea_V2(false);
			variables->CancelIcon->SetVisibility(ESlateVisibility::Collapsed);
			if (blackboard->bFingerOnCancelButton)
				return;

			UBattleEntityComponent* BattleEntityComponent = UCharacterHelpFunctionLibrary::GetBattleEntityComponent(SelectedSkillIcon->Blackboard.OwnerCharacter.Get());
			if (BattleEntityComponent)
			{
				if (BattleEntityComponent->IsInUnActableState())
					return;
			}

			UISkill_V2* skill = SkillEntityComponent->GetSkillByCategory_V2(SelectedSkillIcon->Blackboard.SkillCategory);

			bool isSucessCast = SkillEntityComponent->CastInputSkill(skill->GetSkillID(), SelectedSkillIcon->Blackboard.OwnerCharacter.Get(), skill->GetTarget(), skill->GetTargetLocation());
			if (isSucessCast)
			{
// 				if (SelectedSkillIcon->Blackboard.SkillCategory == ESkillCategoryTypeEnum::VE_ActiveSkill03)
// 				{
// 					variables->SkillIconArray[(int32)ESkillIconSeq::CREW1_ACTIVE3]->Blackboard.SetBlackboardData(EIconState::COOL, true);
// 					variables->SkillIconArray[(int32)ESkillIconSeq::CREW2_ACTIVE3]->Blackboard.SetBlackboardData(EIconState::COOL, true);
// 					variables->SkillIconArray[(int32)ESkillIconSeq::LEADER_ACTIVE3]->Blackboard.SetBlackboardData(EIconState::COOL, true);
// 				}
// 				else
// 					SelectedSkillIcon->Blackboard.SetBlackboardData(EIconState::COOL, true);
			}

		}

		
	}
}

EStateEnum UUserInputState::Update()
{
	USkillEntityComponent* SkillEntityComponent = UCharacterHelpFunctionLibrary::GetSkillEntityComponent(SelectedSkillIcon->Blackboard.OwnerCharacter.Get());
	if (!SkillEntityComponent)
		return EStateEnum::END;
	UISkill_V2* skill = SkillEntityComponent->GetSkillByCategory_V2(SelectedSkillIcon->Blackboard.SkillCategory);
	if (!skill)
		return EStateEnum::END;

	if (!SelectedSkillIcon->isDPadEnable())
	{
		skill->SetTargetLocation(SelectedSkillIcon->Blackboard.OwnerCharacter.Get()->GetActorLocation());
		skill->SetTarget(nullptr);

		blackboard->SetBlackboardData(SkillIconSeq, false);
		return CheckBlackboardData();
	}

	ESkillTargetTypeEnum skilltype = skill->GetSkillInfo()->TargetType;
	ESearchShapeTypeEnum effectShape = skill->GetSkillInfo()->EffectShape;
	switch (skilltype)
	{
	case ESkillTargetTypeEnum::VE_Target:
	{
		if (variables->SkillJoystick->IsValidLowLevel())
		{
			variables->SkillJoystick->SetComaparedPos(blackboard->CurrentTouchPos.X, blackboard->CurrentTouchPos.Y);
			bool isInDeadZone = variables->SkillJoystick->EventTouchMove();

			FVector2D DPadSkillValue = RGAMEINSTANCE(this)->InputManager->GetDPadSkillMoveValue();
			FVector MoveVec = FVector(DPadSkillValue.X, DPadSkillValue.Y, 0);
			MoveVec.Normalize();

			FRotator MapRot = RGAMEINSTANCE(this)->CameraActor->GetActorRotation();
			MapRot.Pitch = 0.0f;
			MapRot.Roll = 0.0f;
			MoveVec = MapRot.RotateVector(MoveVec);

			float SkillRange = skill->GetSkillInfo()->CastingRange;
			MoveVec.X = MoveVec.X * SkillRange * variables->SkillJoystick->JoyStickScale;
			MoveVec.Y = MoveVec.Y * SkillRange * variables->SkillJoystick->JoyStickScale;
			MoveVec.Z = 0;

 			FVector SkillLocation = UMapFunctionLibrary::GetNavAgentLocation(SelectedSkillIcon->Blackboard.OwnerCharacter.Get());
 			if (!isInDeadZone)
 				SkillLocation += MoveVec;
 
			TArray<AActor*> FoundTargets;
			auto FindTargetFromSourceLocation = [=](TArray<AActor*>& outActors, FVector2D SourceVector) {
				UBaseStageInfo* StageInfo = UGamePlayBluePrintFunction::GetBaseStageInfo();
				if (!StageInfo)
					return;

				FSkillTableInfo_V2* CurrentSkillTable = skill->GetSkillInfo();
				if (!CurrentSkillTable)
					return;

				FVector CasterLocation = UMapFunctionLibrary::GetNavAgentLocation(SelectedSkillIcon->Blackboard.OwnerCharacter.Get());
				AProjectRGameMode* ProjectRGameMode = Cast<AProjectRGameMode>(UGameplayStatics::GetGameMode(RGAMEINSTANCE(GEngine)->GetWorld()));
				

				if (CurrentSkillTable->TargetApply == EFriendshipTypeEnum::VE_Enemy)
				{
					TArray<TWeakObjectPtr<AActor>> OutTargets;
					ProjectRGameMode->ObjectHelper.GetEnemyList(OutTargets, SelectedSkillIcon->Blackboard.OwnerCharacter.Get());

					for (size_t i = 0; i < OutTargets.Num(); ++i)
					{
						if (OutTargets[i] != nullptr)
						{
							UBattleEntityComponent* BattleEntityComponent = UCharacterHelpFunctionLibrary::GetBattleEntityComponent(OutTargets[i].Get());
							if (BattleEntityComponent)
							{
								float UnitRadius = BattleEntityComponent->GetBattleInfo()->CharacterProperty.UnitRadius;
								FVector2D MonsterPos = FVector2D(OutTargets[i]->GetActorLocation().X, OutTargets[i]->GetActorLocation().Y);
								FVector2D SecondVec = MonsterPos;
								MonsterPos -= SourceVector;
								SecondVec -= FVector2D(CasterLocation.X, CasterLocation.Y);
								if (MonsterPos.Size() < SkillRange + UnitRadius || 
									SecondVec.Size() < SkillRange + UnitRadius)
									outActors.Emplace(OutTargets[i].Get());
							}
						}
					}
				}
				else if (CurrentSkillTable->TargetApply == EFriendshipTypeEnum::VE_FriendParty ||
					CurrentSkillTable->TargetApply == EFriendshipTypeEnum::VE_Friend)
				{
					TArray<TWeakObjectPtr<AActor>> OutTargets;
					ProjectRGameMode->ObjectHelper.GetFriendList(OutTargets, SelectedSkillIcon->Blackboard.OwnerCharacter.Get());

					for (size_t i = 0; i < OutTargets.Num(); ++i)
					{
						UBattleEntityComponent* BattleEntityComponent = UCharacterHelpFunctionLibrary::GetBattleEntityComponent(OutTargets[i].Get());
						if (BattleEntityComponent)
						{
							float UnitRadius = BattleEntityComponent->GetBattleInfo()->CharacterProperty.UnitRadius;
							FVector2D MonsterPos = FVector2D(OutTargets[i]->GetActorLocation().X, OutTargets[i]->GetActorLocation().Y);
							FVector2D SecondVec = MonsterPos;
							MonsterPos -= SourceVector;
							SecondVec -= FVector2D(CasterLocation.X, CasterLocation.Y);
							if (MonsterPos.Size() < SkillRange + UnitRadius ||
								SecondVec.Size() < SkillRange + UnitRadius)
								outActors.Emplace(OutTargets[i].Get());
						}
					}
				}
			};

			FindTargetFromSourceLocation(FoundTargets, FVector2D(SkillLocation.X, SkillLocation.Y));
			

			Algo::Sort(FoundTargets, [=](AActor* InSource , AActor* InTarget) {
				FVector2D SourceLocation = FVector2D(SkillLocation.X, SkillLocation.Y);
				FVector2D TargetLocation1 = FVector2D(InSource->GetActorLocation().X, InSource->GetActorLocation().Y);
				FVector2D TargetLocation2 = FVector2D(InTarget->GetActorLocation().X, InTarget->GetActorLocation().Y);
				FVector2D Dir1 = SourceLocation - TargetLocation1;
				FVector2D Dir2 = SourceLocation - TargetLocation2;
				float Dist1 = Dir1.Size();
				float Dist2 = Dir2.Size();

				return Dist1 < Dist2;
			});
			

			if (FoundTargets.IsValidIndex(0))
			{
				FVector Location = SelectedSkillIcon->Blackboard.OwnerCharacter->GetActorLocation();
				FVector TargetLocation = FoundTargets[0]->GetActorLocation();
				FVector Dir = TargetLocation - Location;
				float Dist = Dir.Size();

				float UnitRadius = 0.f;
				UBattleEntityComponent* BattleEntityComponent = UCharacterHelpFunctionLibrary::GetBattleEntityComponent(FoundTargets[0]);
				if (BattleEntityComponent)
				{
					UnitRadius = BattleEntityComponent->GetBattleInfo()->CharacterProperty.UnitRadius;
				}

				if (Dist <= SkillRange + UnitRadius)
				{
					skill->SetTargetLocation(FVector::ZeroVector);
					skill->SetTarget(FoundTargets[0]);

					TArray< USkillAreaIndicatorMeshComponent* >	TargetComponentArray = UCharacterHelpFunctionLibrary::GetSkillAreaArray(SelectedSkillIcon->Blackboard.OwnerCharacter.Get());
					for (auto& Element : TargetComponentArray)
					{
						bool IsPickArea = (Element && Element->GetSkillAreaMeshType() == ESkillAreaMeshTypeEnum::VE_SkillPickArea);
						if (IsPickArea == false)
							continue;

						
						if (Element->GetSkillShapeType() == ESearchShapeTypeEnum::VE_Line)
						{
							Element->ShowSkillArea(Dist, 60.f);
							Dir = Dir.GetSafeNormal2D();

							float Degree = UUtilFunctionLibrary::GetBetweenDegree(Dir, FVector(1.f, 0.f, 0.f));
							FRotator rot = FRotator(0.f, -Degree, 0.f);
							if (!rot.ContainsNaN())
							{
								Element->SetWorldRotation(rot);
								Element->SkillAreaMeshUpdate(Location);
							}
						}
					}
				}
				else
				{
					TArray< USkillAreaIndicatorMeshComponent* >	TargetComponentArray = UCharacterHelpFunctionLibrary::GetSkillAreaArray(SelectedSkillIcon->Blackboard.OwnerCharacter.Get());
					for (auto& Element : TargetComponentArray)
					{
						bool IsPickArea = (Element && Element->GetSkillAreaMeshType() == ESkillAreaMeshTypeEnum::VE_SkillPickArea);
						if (IsPickArea == false)
							continue;

						if (Element->GetSkillShapeType() == ESearchShapeTypeEnum::VE_Line)
							Element->SetVisibility(false);

						Element->SkillAreaMeshUpdate(Location);
					}
				}
			}
			else
			{
				TArray< USkillAreaIndicatorMeshComponent* >	TargetComponentArray = UCharacterHelpFunctionLibrary::GetSkillAreaArray(SelectedSkillIcon->Blackboard.OwnerCharacter.Get());
				for (auto& Element : TargetComponentArray)
				{
					bool IsPickArea = (Element && Element->GetSkillAreaMeshType() == ESkillAreaMeshTypeEnum::VE_SkillPickArea);
					if (IsPickArea == false)
						continue;

					if (Element->GetSkillShapeType() == ESearchShapeTypeEnum::VE_Line)
						Element->SetVisibility(false);

					FVector Location = SelectedSkillIcon->Blackboard.OwnerCharacter->GetActorLocation();
					Element->SkillAreaMeshUpdate(Location);
				}
			}
		}

		break;
	}
	case ESkillTargetTypeEnum::VE_Me:
	{
		if (variables->SkillJoystick->IsValidLowLevel())
		{
			FVector SkillLocation = FVector::ZeroVector;
			bool PossibleToMoveDpad = effectShape == ESearchShapeTypeEnum::VE_Box || effectShape == ESearchShapeTypeEnum::VE_Sector;
			if (PossibleToMoveDpad)
			{
				variables->SkillJoystick->SetComaparedPos(blackboard->CurrentTouchPos.X, blackboard->CurrentTouchPos.Y);
				bool isInDeadZone = variables->SkillJoystick->EventTouchMove();

				FVector2D DPadSkillValue = RGAMEINSTANCE(this)->InputManager->GetDPadSkillMoveValue();
				FVector MoveVec = FVector(DPadSkillValue.X, DPadSkillValue.Y, 0);
				MoveVec.Normalize();

				FRotator MapRot = RGAMEINSTANCE(this)->CameraActor->GetActorRotation();
				MapRot.Pitch = 0.0f;
				MapRot.Roll = 0.0f;
				MoveVec = MapRot.RotateVector(MoveVec);

				float SkillRange = skill->GetSkillInfo()->CastingRange;
				MoveVec.X = MoveVec.X * SkillRange * variables->SkillJoystick->JoyStickScale;
				MoveVec.Y = MoveVec.Y * SkillRange * variables->SkillJoystick->JoyStickScale;
				MoveVec.Z = 0;

				SkillLocation = UMapFunctionLibrary::GetNavAgentLocation(SelectedSkillIcon->Blackboard.OwnerCharacter.Get());
				if (!isInDeadZone)
					SkillLocation += MoveVec;
				else
				{
					FVector ForwardVector = SelectedSkillIcon->Blackboard.OwnerCharacter->GetActorForwardVector() * -1.f;
					ForwardVector.Z = 0;
					ForwardVector = ForwardVector.GetSafeNormal();

					ForwardVector.X = ForwardVector.X * SkillRange * variables->SkillJoystick->JoyStickScale;
					ForwardVector.Y = ForwardVector.Y * SkillRange * variables->SkillJoystick->JoyStickScale;
					SkillLocation += ForwardVector;
				}
			}
			else
			{
				variables->SkillJoystick->InitTouchPos(0.f, 0.f);
				variables->SkillJoystick->SetComaparedPos(0.f, 0.f);
				variables->SkillJoystick->EventTouchMove();
				SkillLocation = UMapFunctionLibrary::GetNavAgentLocation(SelectedSkillIcon->Blackboard.OwnerCharacter.Get());
			}

			skill->SetTargetLocation(SkillLocation);
			skill->SetTarget(nullptr);

			TArray< USkillAreaIndicatorMeshComponent* >	TargetComponentArray = UCharacterHelpFunctionLibrary::GetSkillAreaArray(SelectedSkillIcon->Blackboard.OwnerCharacter.Get());
			for (auto& Element : TargetComponentArray)
			{
				bool IsPickArea = (Element && Element->GetSkillAreaMeshType() == ESkillAreaMeshTypeEnum::VE_SkillPickArea);
				if (IsPickArea == false)
				{
					if (!PossibleToMoveDpad)
					{
						FVector CasterLocation = UMapFunctionLibrary::GetNavAgentLocation(SelectedSkillIcon->Blackboard.OwnerCharacter.Get());
						Element->SkillAreaMeshUpdate(CasterLocation);
					}
					continue;
				}
				

				if (!PossibleToMoveDpad)
					Element->SetVisibility(false);
				else if (effectShape == ESearchShapeTypeEnum::VE_Sector || 
					effectShape == ESearchShapeTypeEnum::VE_Box)
				{
					FVector Location = SelectedSkillIcon->Blackboard.OwnerCharacter->GetActorLocation();
					ACharacter* Character = Cast<ACharacter>(SelectedSkillIcon->Blackboard.OwnerCharacter);
					if (Character)
					{
						float height = Character->GetDefaultHalfHeight();
						Location.Z -= height;
					}
					FVector TargetLocation = SkillLocation;

					FVector2D Location2D = FVector2D(Location.X, Location.Y);
					FVector2D TargetLocation2D = FVector2D(TargetLocation.X, TargetLocation.Y);

					FVector2D Dir = TargetLocation2D - Location2D;
					Dir = Dir.GetSafeNormal();

					float cosvalue = FVector2D::DotProduct(Dir, FVector2D(0.f, -1.f));
					float angle = 0.f;
					if (Dir.X > 0)
						angle = FMath::Acos(cosvalue);
					else
						angle = 2 * PI - FMath::Acos(cosvalue);

					Element->SetWorldRotation(FQuat(FVector(0.f, 0.f, 1.f), angle - PI * 0.5f));
					Element->SkillAreaMeshUpdate(TargetLocation);
					Element->SetVisibility(true);
				}
				else
				{
					Element->SetVisibility(true);
					Element->SkillAreaMeshUpdate(SkillLocation);
				}


			}
		}

		break;
	}
	case ESkillTargetTypeEnum::VE_TargetLocation:
	{
		if (variables->SkillJoystick->IsValidLowLevel())
		{
			variables->SkillJoystick->SetComaparedPos(blackboard->CurrentTouchPos.X, blackboard->CurrentTouchPos.Y);
			bool isInDeadZone = variables->SkillJoystick->EventTouchMove();

			FVector2D DPadSkillValue = RGAMEINSTANCE(this)->InputManager->GetDPadSkillMoveValue();
			FVector MoveVec = FVector(DPadSkillValue.X, DPadSkillValue.Y, 0);
			MoveVec.Normalize();

			FRotator MapRot = RGAMEINSTANCE(this)->CameraActor->GetActorRotation();
			MapRot.Pitch = 0.0f;
			MapRot.Roll = 0.0f;
			MoveVec = MapRot.RotateVector(MoveVec);

			float SkillRange = skill->GetSkillInfo()->CastingRange;
			MoveVec.X = MoveVec.X * SkillRange * variables->SkillJoystick->JoyStickScale;
			MoveVec.Y = MoveVec.Y * SkillRange * variables->SkillJoystick->JoyStickScale;
			MoveVec.Z = 0;

			FVector SkillLocation = UMapFunctionLibrary::GetNavAgentLocation(SelectedSkillIcon->Blackboard.OwnerCharacter.Get());
			if (!isInDeadZone)
				SkillLocation += MoveVec;

			skill->SetTargetLocation(SkillLocation);
			skill->SetTarget(nullptr);

			TArray< USkillAreaIndicatorMeshComponent* >	TargetComponentArray = UCharacterHelpFunctionLibrary::GetSkillAreaArray(SelectedSkillIcon->Blackboard.OwnerCharacter.Get());
			for (auto& Element : TargetComponentArray)
			{
				bool IsPickArea = (Element && Element->GetSkillAreaMeshType() == ESkillAreaMeshTypeEnum::VE_SkillPickArea);
				if (IsPickArea == false)
					continue;

				Element->SetVisibility(true);
				Element->SkillAreaMeshUpdate(SkillLocation);
			}
		}
		break;
	}
	}


	return CheckBlackboardData();
}

EStateEnum UUserInputState::CheckBlackboardData()
{
	if (blackboard->bCrewDeadState == true)
		return EStateEnum::CREWDEAD;
	else if (blackboard->BlackBoardArray[(int32)ESkillIconSeq::LEADER_ACTIVE1] == true)
		return EStateEnum::LEADERSKILL;
	else if (blackboard->BlackBoardArray[(int32)ESkillIconSeq::LEADER_ACTIVE2] == true)
		return EStateEnum::LEADERSKILL;
	else if (blackboard->BlackBoardArray[(int32)ESkillIconSeq::LEADER_ACTIVE3] == true)
		return EStateEnum::LEADERSKILL;
	else if (blackboard->BlackBoardArray[(int32)ESkillIconSeq::CREW1_ACTIVE3] == true)
		return EStateEnum::CREWSKILL3;
	else if (blackboard->BlackBoardArray[(int32)ESkillIconSeq::CREW2_ACTIVE3] == true)
		return EStateEnum::CREWSKILL3;
	else if (UUtilFunctionLibrary::GetDirectionTrackManager()->GetTopEffect() < EDirectionEffectTypeEnum::DE_Battle_CrewChangeEffect)
	{
		blackboard->ClearBlackboardData();
		return EStateEnum::IDLE;
	}
	else if (blackboard->BlackBoardArray[(int32)ESkillIconSeq::CREW1_ACTIVE2] == true)
	{
		USkillIcon_New* skillicon = variables->SkillIconArray[(int32)ESkillIconSeq::CREW1_ACTIVE2];
		if (IsValid(skillicon))
		{
			if(skillicon->Blackboard.bColosseumMode)
				return EStateEnum::CREWDROP;
			else
				return EStateEnum::CREWSKILL2;
		}
	}
	else if (blackboard->BlackBoardArray[(int32)ESkillIconSeq::CREW2_ACTIVE2] == true)
	{
		USkillIcon_New* skillicon = variables->SkillIconArray[(int32)ESkillIconSeq::CREW2_ACTIVE2];
		if (IsValid(skillicon))
		{
			if (skillicon->Blackboard.bColosseumMode)
				return EStateEnum::CREWDROP;
			else
				return EStateEnum::CREWSKILL2;
		}
	}
	
	
	return EStateEnum::IDLE;
}

void ULeaderSkill_UserInputState::Enter()
{
	Super::Enter();

	SelectedSkillIcon->PlayAni(TEXT("Click_Pressed"));
}

void ULeaderSkill_UserInputState::Leave()
{
	Super::Leave();

	SelectedSkillIcon->PlayAni(TEXT("Click_Released"));
}

EStateEnum ULeaderSkill_UserInputState::Update()
{
	return Super::Update();
}

void UCrewSkill3_UserInputState::Enter()
{
	Super::Enter();

	URGameInstance* GameInstance = RGAMEINSTANCE(GEngine);
	checkf(GameInstance != nullptr, TEXT("ULeaderControlHelper[RGameInstance Is Empty]"));
	AStagePlayerController* BasePC = Cast<AStagePlayerController>(UUtilFunctionLibrary::GetBasePlayerController(GameInstance->GetWorld()));
	if (!IsValid(BasePC))
		return;

	BasePC->ActorSettingOnTheCamera(SelectedSkillIcon->Blackboard.OwnerCharacter.Get() , 0.1f , 0.1f);
	SelectedSkillIcon->PlayAni(TEXT("Click_Pressed"));
}

void UCrewSkill3_UserInputState::Leave()
{
	Super::Leave();

	URGameInstance* GameInstance = RGAMEINSTANCE(GEngine);
	checkf(GameInstance != nullptr, TEXT("ULeaderControlHelper[RGameInstance Is Empty]"));
	AStagePlayerController* BasePC = Cast<AStagePlayerController>(UUtilFunctionLibrary::GetBasePlayerController(GameInstance->GetWorld()));
	if (!IsValid(BasePC))
		return;

	BasePC->ActorSettingOnTheCamera(BasePC->GetLeaderCharacter() , 0.1f, 0.1f);
	SelectedSkillIcon->PlayAni(TEXT("Click_Released"));
}

EStateEnum UCrewSkill3_UserInputState::Update()
{
	return Super::Update();
}

void UCrewSkill2_UserInputState::Enter()
{
	URGameInstance* GameInstance = RGAMEINSTANCE(GEngine);
	checkf(GameInstance != nullptr, TEXT("ULeaderControlHelper[RGameInstance Is Empty]"));
	AStagePlayerController* BasePC = Cast<AStagePlayerController>(UUtilFunctionLibrary::GetBasePlayerController(GameInstance->GetWorld()));
	if (!IsValid(BasePC))
		return;

	SelectedSkillIcon = variables->SkillIconArray[(int32)SkillIconSeq];
	SelectedSkillIcon->PlayAni(TEXT("Click_Pressed"));
	variables->CancelIcon->SetVisibility(ESlateVisibility::SelfHitTestInvisible);

	/*Skill DPad Settings*/
	if (variables->SkillJoystick->IsValidLowLevel())
	{
		variables->SkillJoystick->InitTouchPos(blackboard->InitialTouchPos.X, blackboard->InitialTouchPos.Y);

		if (SelectedSkillIcon->IsValidLowLevel())
		{
			UCanvasPanelSlot* StartCanvasSlot = nullptr;
			StartCanvasSlot = UWidgetLayoutLibrary::SlotAsCanvasSlot(variables->Crew_SpiritBoxSwitcher);

			FVector2D StartPosition = StartCanvasSlot->GetPosition() /*+ (StartCanvasSlot->GetSize() * 0.5f)*/;
			FVector2D Size = StartCanvasSlot->GetSize();
			
			StartPosition.X += Size.X * 0.5f;
			StartPosition.Y += Size.Y * 0.25f;
			if (SkillIconSeq == ESkillIconSeq::CREW2_ACTIVE2)
				StartPosition.Y += Size.Y * 0.5f;

			variables->SkillJoystick->ComputePosition(StartPosition);
		}
	}
	/*Skill DPad Settings End*/

	/*Slot Info Settings*/
	if (SkillIconSeq == ESkillIconSeq::CREW1_ACTIVE2)
		SlotInfo = blackboard->PlayStateInfo->GetRestActiveCrewSlot(ECharacterRoleEnum::VE_Crew1);
	else
		SlotInfo = blackboard->PlayStateInfo->GetRestActiveCrewSlot(ECharacterRoleEnum::VE_Crew2);
	/*Slot Info Settings End*/


	/*Dummy Actor Spawn*/
	FCharacterTableInfo* CharacterInfo = URenewal_BattleFunctionLibrary::GetCharacterTableInfo(ECharacterCategoryTypeEnum::VE_UICharacter, SlotInfo->HeroIDName);
	if (CharacterInfo == nullptr)
		return;

	FSkillTableInfo_V2* SkillTable = UUtilFunctionLibrary::GetSkillTableFromHeroID(SlotInfo->HeroIDName);
	if (!SkillTable)
		return;

	FActorSpawnParameters SpawnParams;
	SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;

	UClass* SpawnedHeroClass = nullptr;
	if (GameInstance->GameEnvironmentVar.LobbyHeroToManagement)
		SpawnedHeroClass = CharacterInfo->GetLobbyHeroBlueprintClass();
	if (!IsValid(SpawnedHeroClass))
		SpawnedHeroClass = CharacterInfo->GetHeroBlueprintClass();


	FTransform SpawnTransform = BasePC->GetLeaderCharacter()->GetActorTransform();

	DummyActor = GameInstance->GetWorld()->SpawnActor<ABattleBaseCharacter>(SpawnedHeroClass, SpawnTransform, SpawnParams);
	DummyActor->SetActorHiddenInGame(true);
	/*Dummy Actor Spawn End*/


	auto SkillEntityComponent = UCharacterHelpFunctionLibrary::GetSkillEntityComponent(BasePC->GetLeaderCharacter());
	if (false == IsValid(SkillEntityComponent))
		return;

	/*SkillIndicator Mesh Settings*/
	SkillEntityComponent->SetVisibleCrewSpawnArea(true, SkillTable->ShapeScope[0]);
	/*SkillIndicator Mesh Settings End*/

	/*Camera Settings*/
	if(!URealTimeModManager::IsRealTimeGameMode())
		BasePC->ActorSettingOnTheCamera(DummyActor , 0.1f, 0.1f);
	/*Camera Settings End*/

	/*Direction Track*/
	UUtilFunctionLibrary::GetDirectionTrackManager()->Push_DirectionEffect(EDirectionEffectTypeEnum::DE_Battle_CrewChangeEffect);
}

void UCrewSkill2_UserInputState::Leave()
{
	URGameInstance* GameInstance = RGAMEINSTANCE(GEngine);
	checkf(GameInstance != nullptr, TEXT("ULeaderControlHelper[RGameInstance Is Empty]"));
	AStagePlayerController* BasePC = Cast<AStagePlayerController>(UUtilFunctionLibrary::GetBasePlayerController(GameInstance->GetWorld()));
	if (!IsValid(BasePC))
		return;

	auto SkillEntityComponent = UCharacterHelpFunctionLibrary::GetSkillEntityComponent(BasePC->GetLeaderCharacter());
	if (false == IsValid(SkillEntityComponent))
		return;

	SelectedSkillIcon->PlayAni(TEXT("Click_Released"));

	/*Destroy DummyActor*/
	if (DummyActor->IsValidLowLevel())
		DummyActor->Destroy();
	/*Destroy DummyActor End*/
	
	/*Skill Indicator Mesh Settings*/
	SkillEntityComponent->SetVisibleSkillArea_V2(false);
	/*Skill Indicator Mesh Settings End*/

	/*Camera Settings*/
	if (!URealTimeModManager::IsRealTimeGameMode())
		BasePC->ActorSettingOnTheCamera(BasePC->GetLeaderCharacter() , 0.1f, 0.1f);
	/*Camera Settings End*/
		

	/*Direction Track*/
	UUtilFunctionLibrary::GetDirectionTrackManager()->Pop_DirectionEffect(EDirectionEffectTypeEnum::DE_Battle_CrewChangeEffect);
	UUtilFunctionLibrary::GetDirectionTrackManager()->Push_DirectionEffect(EDirectionEffectTypeEnum::DE_Battle_Vignette);
	/*Direction Track*/

	variables->CancelIcon->SetVisibility(ESlateVisibility::Collapsed);
	if (blackboard->bFingerOnCancelButton)
		return;

	/*Crew CHange Possibility check*/
	UMapFunctionLibrary::AdjustActorLocation(BasePC->GetLeaderCharacter(), SkillLocation);
	bool bSpawnable = UMapFunctionLibrary::isTargetPointonNavMesh(SkillLocation); //UMapFunctionLibrary::IsValidLocation(SkillLocation) && UMapFunctionLibrary::CanMoveToTargetPosition(BasePC->GetLeaderCharacter(), SkillLocation) && UAIFunctionLibrary::CanMoveToTargetPosition(BasePC->GetLeaderCharacter(), SkillLocation);
	if (bSpawnable == false)
	{
		FText text = UUtilFunctionLibrary::GetLocalizedString(EStringTableType::VE_UI, FName(TEXT("UI_HeroChoice_Warning01")));
		text = FText::FromString(TEXT("Impossible to spawn!!"));

		UUIFunctionLibrary::ShowCommonPopupSplash(UUtilFunctionLibrary::GetLocalizedString(EStringTableType::VE_UI, COMMONSTRING_NOTIFY), text);
		return;
	}
	else /*Do Change Crew*/
	{
		BasePC->PlayCameraShake(nullptr, BasePC->CameraShakeIntensity);

		if (!URealTimeModManager::IsRealTimeGameMode())	//
		{
			CrewChange_BeforeSpawn();
			AActor* SpawnedCharacter = CrewChange_Spawn();
			if (IsValid(SpawnedCharacter))
			{
				CrewChange_AfterSpawn_Cam(SpawnedCharacter);
				CrewChange_AfterSpawn_Skill(SpawnedCharacter);
				CrewChange_AfterSpawn_Anim(OwnerRole);
			}
		}
		else
		{
			FVector TargetLocation = FVector::ZeroVector;
			if (!isInDeadZone)
				TargetLocation = SkillLocation;
			else
			{
				AActor* Actor = blackboard->PlayStateInfo->GetBattleActiveCrewActor(OwnerRole);
				if(Actor->IsValidLowLevel())
					TargetLocation = Actor->GetActorLocation();
			}
				

			CrewChange_BeforeSpawn();

			if (SlotInfo->CharacterRole == ECharacterRoleEnum::VE_Crew1)
				GameInstance->RealTimeModManager->ChangeCrew(ESkillOrigin::VE_Crew1, TargetLocation, FVector(1.f, 0.f, 0.f));
			else if (SlotInfo->CharacterRole == ECharacterRoleEnum::VE_Crew2)
				GameInstance->RealTimeModManager->ChangeCrew(ESkillOrigin::VE_Crew2, TargetLocation, FVector(1.f, 0.f, 0.f));
		}
	}

}

EStateEnum UCrewSkill2_UserInputState::Update()
{
	URGameInstance* GameInstance = RGAMEINSTANCE(GEngine);
	checkf(GameInstance != nullptr, TEXT("ULeaderControlHelper[RGameInstance Is Empty]"));
	AStagePlayerController* BasePC = Cast<AStagePlayerController>(UUtilFunctionLibrary::GetBasePlayerController(GameInstance->GetWorld()));
	if (!IsValid(BasePC))
		return EStateEnum::END;

	FCharacterTableInfo* CharacterInfo = URenewal_BattleFunctionLibrary::GetCharacterTableInfo(ECharacterCategoryTypeEnum::VE_UICharacter, SlotInfo->HeroIDName);
	if (CharacterInfo == nullptr)
		return EStateEnum::END;

	FSkillTableInfo_V2* SkillTable = UUtilFunctionLibrary::GetSkillTableFromHeroID(SlotInfo->HeroIDName);
	if (!SkillTable)
		return EStateEnum::END;

	if (variables->SkillJoystick->IsValidLowLevel())
	{
		variables->SkillJoystick->SetComaparedPos(blackboard->CurrentTouchPos.X, blackboard->CurrentTouchPos.Y);
		isInDeadZone = variables->SkillJoystick->EventTouchMove();

		FVector2D DPadSkillValue = RGAMEINSTANCE(this)->InputManager->GetDPadSkillMoveValue();
		FVector MoveVec = FVector(DPadSkillValue.X, DPadSkillValue.Y, 0);
		MoveVec.Normalize();

		FRotator MapRot = RGAMEINSTANCE(this)->CameraActor->GetActorRotation();
		MapRot.Pitch = 0.0f;
		MapRot.Roll = 0.0f;
		MoveVec = MapRot.RotateVector(MoveVec);

		float SkillRange = 500.f;
		MoveVec.X = MoveVec.X * SkillRange * variables->SkillJoystick->JoyStickScale;
		MoveVec.Y = MoveVec.Y * SkillRange * variables->SkillJoystick->JoyStickScale;
		MoveVec.Z = 0;

		SkillLocation = UMapFunctionLibrary::GetNavAgentLocation(BasePC->GetLeaderCharacter());
		if (!isInDeadZone)
			SkillLocation += MoveVec;

		TArray< USkillAreaIndicatorMeshComponent* >	TargetComponentArray = UCharacterHelpFunctionLibrary::GetSkillAreaArray(BasePC->GetLeaderCharacter());
		for (auto& Element : TargetComponentArray)
		{
			bool IsPickArea = (Element && Element->GetSkillAreaMeshType() == ESkillAreaMeshTypeEnum::VE_SkillPickArea);
			if (IsPickArea == false)
				continue;

			ESearchShapeTypeEnum SkillShapeType = Element->GetSkillShapeType();
			if (SkillShapeType == ESearchShapeTypeEnum::VE_Circle || SkillShapeType == ESearchShapeTypeEnum::VE_Cylinder)
				Element->SkillAreaMeshUpdate(SkillLocation);
		}
	}

	//dummyActor Settings
	if (IsValid(DummyActor))
	{
		if(!isInDeadZone)
			DummyActor->SetActorHiddenInGame(false);

		UCapsuleComponent* CapsuleComp = UCharacterHelpFunctionLibrary::GetCapsuleComponent(DummyActor);
		if (IsValid(CapsuleComp))
		{
			float ScaledHeight = CapsuleComp->GetUnscaledCapsuleHalfHeight();
			FVector NewLocation = SkillLocation + FVector(0.f, 0.f, ScaledHeight);
			DummyActor->SetActorLocation(NewLocation);
		}

		UMaterialInstance* MaterialInstance = nullptr;
		bool bSpawnable = UMapFunctionLibrary::isTargetPointonNavMesh(SkillLocation);//UMapFunctionLibrary::IsValidLocation(SkillLocation) && UMapFunctionLibrary::CanMoveToTargetPosition(BasePC->GetLeaderCharacter(), SkillLocation) && UAIFunctionLibrary::CanMoveToTargetPosition(BasePC->GetLeaderCharacter(), SkillLocation);
		if (bSpawnable)
			MaterialInstance = UUtilFunctionLibrary::GetMaterialInstanceFromTable(FName(TEXT("Crew_Spawn_possible")));
		else
			MaterialInstance = UUtilFunctionLibrary::GetMaterialInstanceFromTable(FName(TEXT("Crew_Spawn_Impossible")));



		USkeletalMeshComponent* SkeletalMeshComponent = DummyActor->FindComponentByClass<USkeletalMeshComponent>();
		int32 MaterialSlotNum = SkeletalMeshComponent->GetNumMaterials();
		for (int32 i = 0; i < MaterialSlotNum; ++i)
		{
			//UMaterialInstanceDynamic* MID = UMaterialInstanceDynamic::Create(MaterialInstance, SkeletalMeshComponent);
			SkeletalMeshComponent->SetMaterial(i, MaterialInstance);
		}
	}

	return CheckBlackboardData();
}

void UCrewSkill2_UserInputState::CrewChange_BeforeSpawn()
{
	FHeroSlotInfo* BattleSlot = blackboard->PlayStateInfo->GetBattleActiveCrewSlot(OwnerRole);
	FHeroSlotInfo* RestSlot = SlotInfo;

	AActor* BattleActor = blackboard->PlayStateInfo->GetBattleActiveCrewActor(OwnerRole);
	if (BattleActor->IsValidLowLevel())
	{
		UBattleEntityComponent* BattleComp = UCharacterHelpFunctionLibrary::GetBattleEntityComponent(BattleActor);
		if (BattleComp)
			BattleSlot->PrevHeroHP = BattleComp->GetCurrentHPPercent();
	}
}

AActor* UCrewSkill2_UserInputState::CrewChange_Spawn()
{
	URGameInstance* GameInstance = RGAMEINSTANCE(GEngine);
	checkf(GameInstance != nullptr, TEXT("ULeaderControlHelper[RGameInstance Is Empty]"));
	AStagePlayerController* BasePC = Cast<AStagePlayerController>(UUtilFunctionLibrary::GetBasePlayerController(GameInstance->GetWorld()));

	AActor* SpawnedCharacter = nullptr;
	int32 Targetidx = blackboard->PlayStateInfo->GetRestActiveCrewIdx(OwnerRole);
	if (Targetidx != INDEX_NONE)
	{
		if (!isInDeadZone)
			SpawnedCharacter = BasePC->SpawnHeroAtLocation(Targetidx, SkillLocation, FRotator::ZeroRotator, true);
		else
		{
			AActor* BattleActor = blackboard->PlayStateInfo->GetBattleActiveCrewActor(OwnerRole);
			if (IsValid(BattleActor))
			{
				FVector TargetLocation = BattleActor->GetActorLocation();
				SpawnedCharacter = BasePC->SpawnHeroAtLocation(Targetidx, TargetLocation, FRotator::ZeroRotator, true);
			}
		}
	}

	return SpawnedCharacter;
}

void UCrewSkill2_UserInputState::CrewChange_AfterSpawn_Cam(AActor* SpawnedActor)
{
	//Camera Pingpoing start
	APlayerCameraManager* PCM = UGameplayStatics::GetPlayerCameraManager(RGAMEINSTANCE(this)->GetWorld(), 0);
	ABaseCameraActor* BaseCam = Cast<ABaseCameraActor>(PCM->GetViewTarget());
	if (IsValid(BaseCam))
	{
		BaseCam->MoveStageCamera(SpawnedActor->GetActorLocation(), 0.f);
		BaseCam->PingPongCameraForward(SpawnedActor->GetActorLocation(), 1.f);

		BaseCam->AutoZoomInOutCameraByPercent(
			true,//bStartZoomIn, 
			0.5f,//ZoomPercent, 
			0.15f,//StartTime, 
			0.0f,//DurationTime, 
			0.5f,//EndTime, 
			nullptr, nullptr);
	}
}

void UCrewSkill2_UserInputState::CrewChange_AfterSpawn_Skill(AActor* SpawnedActor)
{
	auto SkillEntComp = UCharacterHelpFunctionLibrary::GetSkillEntityComponent(SpawnedActor);
	auto BattleEntComp = UCharacterHelpFunctionLibrary::GetBattleEntityComponent(SpawnedActor);

	if (IsValid(SkillEntComp) && IsValid(BattleEntComp))
	{
		UISkill_V2* TempSkill = SkillEntComp->GetSkillByCategory_V2(ESkillCategoryTypeEnum::VE_ActiveSkill02);
		if (IsValid(TempSkill))
		{
			AActor* CurTarget = nullptr;
			if (TempSkill != nullptr && TempSkill->GetSkillInfo()->TargetType != ESkillTargetTypeEnum::VE_Target)
				CurTarget = SpawnedActor;
			else
				CurTarget = BattleEntComp->GetCurrentAttackTarget();

			if (!IsValid(CurTarget))
			{
				UCharacterBattleInfo* BattleInfo = BattleEntComp->GetBattleInfo();
				float AttackRange = BattleInfo->GetCharacterFinalProperty< float >(ECharacterPropertyTypeEnum::VE_AttackRange);

				BattleEntComp->SetCurrentAttackTarget(URenewal_BattleFunctionLibrary::FindEnemy(SpawnedActor, SpawnedActor->GetActorLocation(), ESearchShapeTypeEnum::VE_Circle, AttackRange, EEnemySearchTargetTypeEnum::VE_Close));
				CurTarget = BattleEntComp->GetCurrentAttackTarget();
			}

			if (CurTarget)
			{
				FVector Location = UMapFunctionLibrary::GetNavAgentLocation(CurTarget);
				SkillEntComp->CastInputSkill(TempSkill->GetSkillID(), SpawnedActor, CurTarget, Location);
			}
			

			FHeroSlotInfo* prevSlot = blackboard->PlayStateInfo->GetBattleActiveCrewSlot(OwnerRole);
			if (prevSlot)
			{
				prevSlot->DespawnAll();
				prevSlot->CharacterCrewState = ECharacterCrewStateEnum::VE_Rest;
			}
			SlotInfo->CharacterCrewState = ECharacterCrewStateEnum::VE_Battle;

			/*Set SkillIcon blackboard Data*/
			if (prevSlot)
			{
				SelectedSkillIcon->Renderer.Prepare(*prevSlot, ESkillCategoryTypeEnum::VE_ActiveSkill02);
				SelectedSkillIcon->Blackboard.MaxCoolTime = TempSkill->GetSkillInfo()->CoolTime;
				SelectedSkillIcon->Blackboard.CurrentCoolTime = SelectedSkillIcon->Blackboard.MaxCoolTime;
				SelectedSkillIcon->Blackboard.SetBlackboardData(EIconState::COOL, true);

				if (SkillIconSeq == ESkillIconSeq::CREW1_ACTIVE2)
				{
					variables->SkillIconArray[(int32)ESkillIconSeq::CREW1_ACTIVE3]->Blackboard.SetBlackboardData(EIconState::LOCK, false);
					float IncreaseCoolTime = variables->SkillIconArray[(int32)ESkillIconSeq::CREW1_ACTIVE3]->Blackboard.CurrentCoolTime;
					variables->SkillIconArray[(int32)ESkillIconSeq::CREW1_ACTIVE3]->Renderer.Prepare(*SlotInfo, ESkillCategoryTypeEnum::VE_ActiveSkill03);
					variables->SkillIconArray[(int32)ESkillIconSeq::CREW1_ACTIVE3]->IncreaseCoolTime(IncreaseCoolTime);
				}
				else if (SkillIconSeq == ESkillIconSeq::CREW2_ACTIVE2)
				{
					variables->SkillIconArray[(int32)ESkillIconSeq::CREW2_ACTIVE3]->Blackboard.SetBlackboardData(EIconState::LOCK, false);
					float IncreaseCoolTime = variables->SkillIconArray[(int32)ESkillIconSeq::CREW2_ACTIVE3]->Blackboard.CurrentCoolTime;
					variables->SkillIconArray[(int32)ESkillIconSeq::CREW2_ACTIVE3]->Renderer.Prepare(*SlotInfo, ESkillCategoryTypeEnum::VE_ActiveSkill03);
					variables->SkillIconArray[(int32)ESkillIconSeq::CREW2_ACTIVE3]->IncreaseCoolTime(IncreaseCoolTime);
				}
			}
		}
	}
}

void UCrewSkill2_UserInputState::CrewChange_AfterSpawn_Anim(ECharacterRoleEnum InRole)
{
	URCampaignHUD_ThreeControl* ThreeGameHUD = Cast<URCampaignHUD_ThreeControl>(UUIFunctionLibrary::GetInGameHUD());
	if (IsValid(ThreeGameHUD))
	{
		FString aniNameString = (InRole == ECharacterRoleEnum::VE_Crew1) ? TEXT("Crew1Change") : TEXT("Crew2Change");
		ThreeGameHUD->PlayAni(aniNameString);
		for (size_t i = 0; i < ThreeGameHUD->HeroIconArray.Num(); ++i)
		{
			FHeroSlotInfo slotinfo = ThreeGameHUD->HeroIconArray[i]->GetHeroSlotInfo();
			if (slotinfo.CharacterRole == InRole)
				ThreeGameHUD->HeroIconArray[i]->PlayAni(TEXT("CrewChange"));
		}
	}
}

void UCrewDead_UserInputState::Enter()
{
	/*Direction Track*/
	UUtilFunctionLibrary::GetDirectionTrackManager()->Push_DirectionEffect(EDirectionEffectTypeEnum::DE_Battle_CrewChangeEffect);
}

void UCrewDead_UserInputState::Leave()
{
	URGameInstance* GameInstance = RGAMEINSTANCE(GEngine);
	checkf(GameInstance != nullptr, TEXT("ULeaderControlHelper[RGameInstance Is Empty]"));
	AStagePlayerController* BasePC = Cast<AStagePlayerController>(UUtilFunctionLibrary::GetBasePlayerController(GameInstance->GetWorld()));
	if (!IsValid(BasePC))
		return;

	/*Camera Settings*/
	BasePC->ActorSettingOnTheCamera(BasePC->GetLeaderCharacter(), 0.1f, 0.1f);
	/*Camera Settings End*/


	/*Direction Track*/
	UUtilFunctionLibrary::GetDirectionTrackManager()->Pop_DirectionEffect(EDirectionEffectTypeEnum::DE_Battle_CrewChangeEffect);
	UUtilFunctionLibrary::GetDirectionTrackManager()->Push_DirectionEffect(EDirectionEffectTypeEnum::DE_Battle_Vignette);
	/*Direction Track*/

	/*Crew CHange Possibility check*/
	UMapFunctionLibrary::AdjustActorLocation(BasePC->GetLeaderCharacter(), SkillLocation);
	/*Do Change Crew*/
	BasePC->PlayCameraShake(nullptr, BasePC->CameraShakeIntensity);

	if (!URealTimeModManager::IsRealTimeGameMode())	//
	{
		int32 RestIdx = blackboard->SlotIdx % 2 == 1 ? blackboard->SlotIdx + 1 : blackboard->SlotIdx - 1;
		AActor* SpawnedCharacter = BasePC->SpawnHeroAtLocation(RestIdx, SkillLocation, FRotator::ZeroRotator, true);

		if (IsValid(SpawnedCharacter))
		{
			//Camera Pingpoing start
			APlayerCameraManager* PCM = UGameplayStatics::GetPlayerCameraManager(SpawnedCharacter->GetWorld(), 0);
			ABaseCameraActor* BaseCam = Cast<ABaseCameraActor>(PCM->GetViewTarget());
			if (IsValid(BaseCam))
			{
				BaseCam->MoveStageCamera(SpawnedCharacter->GetActorLocation(), 0.f);
				BaseCam->PingPongCameraForward(SpawnedCharacter->GetActorLocation(), 1.f);

				BaseCam->AutoZoomInOutCameraByPercent(
					true,//bStartZoomIn, 
					0.5f,//ZoomPercent, 
					0.15f,//StartTime, 
					0.0f,//DurationTime, 
					0.5f,//EndTime, 
					nullptr, nullptr);
			}

			//
			auto SkillEntComp = UCharacterHelpFunctionLibrary::GetSkillEntityComponent(SpawnedCharacter);
			auto BattleEntComp = UCharacterHelpFunctionLibrary::GetBattleEntityComponent(SpawnedCharacter);

			if (IsValid(SkillEntComp) && IsValid(BattleEntComp))
			{
				UISkill_V2* TempSkill = SkillEntComp->GetSkillByCategory_V2(ESkillCategoryTypeEnum::VE_ActiveSkill02);
				if (IsValid(TempSkill))
				{
					AActor* CurTarget = nullptr;
					if (TempSkill != nullptr && TempSkill->GetSkillInfo()->TargetType == ESkillTargetTypeEnum::VE_Me)
						CurTarget = SpawnedCharacter;
					else
						CurTarget = BattleEntComp->GetCurrentAttackTarget();

					if (!IsValid(CurTarget))
					{
						UCharacterBattleInfo* BattleInfo = BattleEntComp->GetBattleInfo();
						float AttackRange = BattleInfo->GetCharacterFinalProperty< float >(ECharacterPropertyTypeEnum::VE_AttackRange);

						BattleEntComp->SetCurrentAttackTarget(URenewal_BattleFunctionLibrary::FindEnemy(SpawnedCharacter, SpawnedCharacter->GetActorLocation(), ESearchShapeTypeEnum::VE_Circle, AttackRange, EEnemySearchTargetTypeEnum::VE_Close));
						CurTarget = BattleEntComp->GetCurrentAttackTarget();
					}

					if (CurTarget)
					{
						FVector Location = UMapFunctionLibrary::GetNavAgentLocation(CurTarget);
						SkillEntComp->CastInputSkill(TempSkill->GetSkillID(), SpawnedCharacter, CurTarget, Location);
					}
					
					FHeroSlotInfo& CurrentRestSlot = blackboard->PlayStateInfo->GetSelectedCharacterSlot(blackboard->SlotIdx);
					FHeroSlotInfo& CurrentBattleSlot = blackboard->PlayStateInfo->GetSelectedCharacterSlot(RestIdx);
					CurrentBattleSlot.CharacterCrewState = ECharacterCrewStateEnum::VE_Battle;
					CurrentRestSlot.DespawnAll();
					CurrentRestSlot.CharacterCrewState = ECharacterCrewStateEnum::VE_Rest;

					
					ECharacterRoleEnum CharacterRole = blackboard->SlotIdx <= ROLE_CREW1_REST_INDEX  ? ECharacterRoleEnum::VE_Crew1 : ECharacterRoleEnum::VE_Crew2;

					if (CharacterRole == ECharacterRoleEnum::VE_Crew1)
					{
						variables->SkillIconArray[(int32)ESkillIconSeq::CREW1_ACTIVE2]->Blackboard.ClearBlackboardData();
						variables->SkillIconArray[(int32)ESkillIconSeq::CREW1_ACTIVE2]->Blackboard.SetBlackboardData(EIconState::DEAD, true);

						variables->SkillIconArray[(int32)ESkillIconSeq::CREW1_ACTIVE3]->Blackboard.SetBlackboardData(EIconState::LOCK, false);
						variables->SkillIconArray[(int32)ESkillIconSeq::CREW1_ACTIVE3]->Renderer.Prepare(CurrentBattleSlot, ESkillCategoryTypeEnum::VE_ActiveSkill03);
						variables->SkillIconArray[(int32)ESkillIconSeq::CREW1_ACTIVE3]->Blackboard.SetBlackboardData(EIconState::COOL, true);
					}
					else
					{
						variables->SkillIconArray[(int32)ESkillIconSeq::CREW2_ACTIVE2]->Blackboard.ClearBlackboardData();
						variables->SkillIconArray[(int32)ESkillIconSeq::CREW2_ACTIVE2]->Blackboard.SetBlackboardData(EIconState::DEAD, true);

						variables->SkillIconArray[(int32)ESkillIconSeq::CREW2_ACTIVE3]->Blackboard.SetBlackboardData(EIconState::LOCK, false);
						variables->SkillIconArray[(int32)ESkillIconSeq::CREW2_ACTIVE3]->Renderer.Prepare(CurrentBattleSlot, ESkillCategoryTypeEnum::VE_ActiveSkill03);
						variables->SkillIconArray[(int32)ESkillIconSeq::CREW2_ACTIVE3]->Blackboard.SetBlackboardData(EIconState::COOL, true);
					}

					/*Set SkillIcon blackboard Data End*/
					auto PlayAnimation = [](ECharacterRoleEnum InRole) {
						URCampaignHUD_ThreeControl* ThreeGameHUD = Cast<URCampaignHUD_ThreeControl>(UUIFunctionLibrary::GetInGameHUD());
						if (IsValid(ThreeGameHUD))
						{
							FString aniNameString = (InRole == ECharacterRoleEnum::VE_Crew1) ? TEXT("Crew1Change") : TEXT("Crew2Change");
							ThreeGameHUD->PlayAni(aniNameString);
							for (size_t i = 0; i < ThreeGameHUD->HeroIconArray.Num(); ++i)
							{
								FHeroSlotInfo slotinfo = ThreeGameHUD->HeroIconArray[i]->GetHeroSlotInfo();
								if (slotinfo.CharacterRole == InRole)
									ThreeGameHUD->HeroIconArray[i]->PlayAni(TEXT("CrewChange"));
							}
						}
					};
					PlayAnimation(CharacterRole);
				}
			}
		}
	}
	else
	{
		FHeroSlotInfo& SlotInfo = blackboard->PlayStateInfo->GetSelectedCharacterSlot(blackboard->SlotIdx);
		AActor* BattleActor = SlotInfo.SpawnedTargetActor;

		if (!BattleActor->IsValidLowLevel())
			return;

		FVector TargetLocation;
		TargetLocation = BattleActor->GetActorLocation();
		if (BattleActor->IsValidLowLevel())
		{
			UBattleEntityComponent* BattleComp = UCharacterHelpFunctionLibrary::GetBattleEntityComponent(BattleActor);
			if (BattleComp)
				SlotInfo.PrevHeroHP = BattleComp->GetCurrentHPPercent();
		}

		if (SlotInfo.CharacterRole == ECharacterRoleEnum::VE_Crew1)
			GameInstance->RealTimeModManager->ChangeCrew(ESkillOrigin::VE_Crew1, TargetLocation, FVector(1.f, 0.f, 0.f));
		else if (SlotInfo.CharacterRole == ECharacterRoleEnum::VE_Crew2)
			GameInstance->RealTimeModManager->ChangeCrew(ESkillOrigin::VE_Crew2, TargetLocation, FVector(1.f, 0.f, 0.f));
	}
}

EStateEnum UCrewDead_UserInputState::Update()
{
	/*check if this state comes from dead crew*/
	FHeroSlotInfo slotinfo = blackboard->PlayStateInfo->GetSelectedCharacterSlot(blackboard->SlotIdx);
	SkillLocation = slotinfo.DeadLocation;
	blackboard->ClearBlackboardData();
	return EStateEnum::IDLE;
}

void UCrewDrop_UserInputState::Enter()
{
	URGameInstance* GameInstance = RGAMEINSTANCE(GEngine);
	checkf(GameInstance != nullptr, TEXT("ULeaderControlHelper[RGameInstance Is Empty]"));
	AStagePlayerController* BasePC = Cast<AStagePlayerController>(UUtilFunctionLibrary::GetBasePlayerController(GameInstance->GetWorld()));
	if (!IsValid(BasePC))
		return;

	SelectedSkillIcon = variables->SkillIconArray[(int32)SkillIconSeq];
	SelectedSkillIcon->PlayAni(TEXT("Click_Pressed"));
	variables->CancelIcon->SetVisibility(ESlateVisibility::SelfHitTestInvisible);

	/*Skill DPad Settings*/
	if (variables->SkillJoystick->IsValidLowLevel())
	{
		variables->SkillJoystick->InitTouchPos(blackboard->InitialTouchPos.X, blackboard->InitialTouchPos.Y);

		if (SelectedSkillIcon->IsValidLowLevel())
		{
			UCanvasPanelSlot* StartCanvasSlot = nullptr;
			StartCanvasSlot = UWidgetLayoutLibrary::SlotAsCanvasSlot(variables->Crew_SpiritBoxSwitcher);

			FVector2D StartPosition = StartCanvasSlot->GetPosition() /*+ (StartCanvasSlot->GetSize() * 0.5f)*/;
			FVector2D Size = StartCanvasSlot->GetSize();

			StartPosition.X += Size.X * 0.5f;
			StartPosition.Y += Size.Y * 0.25f;
			if (SkillIconSeq == ESkillIconSeq::CREW2_ACTIVE2)
				StartPosition.Y += Size.Y * 0.5f;

			variables->SkillJoystick->ComputePosition(StartPosition);
		}
	}
	/*Skill DPad Settings End*/

	/*Slot Info Settings*/
	if (SkillIconSeq == ESkillIconSeq::CREW1_ACTIVE2)
		SlotInfo = blackboard->PlayStateInfo->GetBattleActiveCrewSlot(ECharacterRoleEnum::VE_Crew1);
	else
		SlotInfo = blackboard->PlayStateInfo->GetBattleActiveCrewSlot(ECharacterRoleEnum::VE_Crew2);
	/*Slot Info Settings End*/


	/*Dummy Actor Spawn*/
	FCharacterTableInfo* CharacterInfo = URenewal_BattleFunctionLibrary::GetCharacterTableInfo(ECharacterCategoryTypeEnum::VE_UICharacter, SlotInfo->HeroIDName);
	if (CharacterInfo == nullptr)
		return;

	FSkillTableInfo_V2* SkillTable = UUtilFunctionLibrary::GetSkillTableFromHeroID(SlotInfo->HeroIDName);
	if (!SkillTable)
		return;

	FActorSpawnParameters SpawnParams;
	SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;

	UClass* SpawnedHeroClass = nullptr;
	if (GameInstance->GameEnvironmentVar.LobbyHeroToManagement)
		SpawnedHeroClass = CharacterInfo->GetLobbyHeroBlueprintClass();
	if (!IsValid(SpawnedHeroClass))
		SpawnedHeroClass = CharacterInfo->GetHeroBlueprintClass();


	FTransform SpawnTransform = BasePC->GetLeaderCharacter()->GetActorTransform();

	DummyActor = GameInstance->GetWorld()->SpawnActor<ABattleBaseCharacter>(SpawnedHeroClass, SpawnTransform, SpawnParams);
	DummyActor->SetActorHiddenInGame(true);
	/*Dummy Actor Spawn End*/


	auto SkillEntityComponent = UCharacterHelpFunctionLibrary::GetSkillEntityComponent(BasePC->GetLeaderCharacter());
	if (false == IsValid(SkillEntityComponent))
		return;

	/*SkillIndicator Mesh Settings*/
	SkillEntityComponent->SetVisibleCrewSpawnArea(true, SkillTable->ShapeScope[0]);
	/*SkillIndicator Mesh Settings End*/

	/*Camera Settings*/
	//BasePC->ActorSettingOnTheCamera(DummyActor, 0.1f, 0.1f);
	/*Camera Settings End*/

	/*Direction Track*/
	UUtilFunctionLibrary::GetDirectionTrackManager()->Push_DirectionEffect(EDirectionEffectTypeEnum::DE_Battle_CrewChangeEffect);
}

void UCrewDrop_UserInputState::Leave()
{
	URGameInstance* GameInstance = RGAMEINSTANCE(GEngine);
	checkf(GameInstance != nullptr, TEXT("ULeaderControlHelper[RGameInstance Is Empty]"));
	AStagePlayerController* BasePC = Cast<AStagePlayerController>(UUtilFunctionLibrary::GetBasePlayerController(GameInstance->GetWorld()));
	if (!IsValid(BasePC))
		return;

	auto SkillEntityComponent = UCharacterHelpFunctionLibrary::GetSkillEntityComponent(BasePC->GetLeaderCharacter());
	if (false == IsValid(SkillEntityComponent))
		return;

	SelectedSkillIcon->PlayAni(TEXT("Click_Released"));
	SelectedSkillIcon->Blackboard.bColosseumMode = false;

	/*Destroy DummyActor*/
	if (DummyActor->IsValidLowLevel())
		DummyActor->Destroy();
	/*Destroy DummyActor End*/

	/*Skill Indicator Mesh Settings*/
	SkillEntityComponent->SetVisibleSkillArea_V2(false);
	/*Skill Indicator Mesh Settings End*/

	/*Camera Settings*/
	//BasePC->ActorSettingOnTheCamera(BasePC->GetLeaderCharacter(), 0.1f, 0.1f);
	/*Camera Settings End*/


	/*Direction Track*/
	UUtilFunctionLibrary::GetDirectionTrackManager()->Pop_DirectionEffect(EDirectionEffectTypeEnum::DE_Battle_CrewChangeEffect);
	UUtilFunctionLibrary::GetDirectionTrackManager()->Push_DirectionEffect(EDirectionEffectTypeEnum::DE_Battle_Vignette);
	/*Direction Track*/

	variables->CancelIcon->SetVisibility(ESlateVisibility::Collapsed);
	if (blackboard->bFingerOnCancelButton)
		return;

	/*Crew CHange Possibility check*/
	UMapFunctionLibrary::AdjustActorLocation(BasePC->GetLeaderCharacter(), SkillLocation);
	bool bSpawnable = UMapFunctionLibrary::isTargetPointonNavMesh(SkillLocation); //UMapFunctionLibrary::IsValidLocation(SkillLocation) && UMapFunctionLibrary::CanMoveToTargetPosition(BasePC->GetLeaderCharacter(), SkillLocation) && UAIFunctionLibrary::CanMoveToTargetPosition(BasePC->GetLeaderCharacter(), SkillLocation);
	if (bSpawnable == false)
	{
		FText text = UUtilFunctionLibrary::GetLocalizedString(EStringTableType::VE_UI, FName(TEXT("UI_HeroChoice_Warning01")));
		text = FText::FromString(TEXT("Impossible to spawn!!"));

		UUIFunctionLibrary::ShowCommonPopupSplash(UUtilFunctionLibrary::GetLocalizedString(EStringTableType::VE_UI, COMMONSTRING_NOTIFY), text);
		return;
	}
	else /*Do Change Crew*/
	{
		//BasePC->PlayCameraShake(nullptr, BasePC->CameraShakeIntensity);

		FVector TargetLocation = FVector::ZeroVector;
		if (!isInDeadZone)
			TargetLocation = SkillLocation;
		else
		{
			AActor* Actor = blackboard->PlayStateInfo->GetBattleActiveCrewActor(ECharacterRoleEnum::VE_Leader);
			if (Actor->IsValidLowLevel())
				TargetLocation = Actor->GetActorLocation();
		}


		if (SlotInfo->CharacterRole == ECharacterRoleEnum::VE_Crew1)
			GameInstance->RealTimeModManager->DropCrew(ESkillOrigin::VE_Crew1, TargetLocation, FVector(1.f, 0.f, 0.f));
		else if (SlotInfo->CharacterRole == ECharacterRoleEnum::VE_Crew2)
			GameInstance->RealTimeModManager->DropCrew(ESkillOrigin::VE_Crew2, TargetLocation, FVector(1.f, 0.f, 0.f));
	}
}

EStateEnum UCrewDrop_UserInputState::Update()
{
	URGameInstance* GameInstance = RGAMEINSTANCE(GEngine);
	checkf(GameInstance != nullptr, TEXT("ULeaderControlHelper[RGameInstance Is Empty]"));
	AStagePlayerController* BasePC = Cast<AStagePlayerController>(UUtilFunctionLibrary::GetBasePlayerController(GameInstance->GetWorld()));
	if (!IsValid(BasePC))
		return EStateEnum::END;

	FCharacterTableInfo* CharacterInfo = URenewal_BattleFunctionLibrary::GetCharacterTableInfo(ECharacterCategoryTypeEnum::VE_UICharacter, SlotInfo->HeroIDName);
	if (CharacterInfo == nullptr)
		return EStateEnum::END;

	FSkillTableInfo_V2* SkillTable = UUtilFunctionLibrary::GetSkillTableFromHeroID(SlotInfo->HeroIDName);
	if (!SkillTable)
		return EStateEnum::END;

	if (variables->SkillJoystick->IsValidLowLevel())
	{
		variables->SkillJoystick->SetComaparedPos(blackboard->CurrentTouchPos.X, blackboard->CurrentTouchPos.Y);
		isInDeadZone = variables->SkillJoystick->EventTouchMove();

		FVector2D DPadSkillValue = RGAMEINSTANCE(this)->InputManager->GetDPadSkillMoveValue();
		FVector MoveVec = FVector(DPadSkillValue.X, DPadSkillValue.Y, 0);
		MoveVec.Normalize();

		FRotator MapRot = RGAMEINSTANCE(this)->CameraActor->GetActorRotation();
		MapRot.Pitch = 0.0f;
		MapRot.Roll = 0.0f;
		MoveVec = MapRot.RotateVector(MoveVec);

		float SkillRange = 500.f;
		MoveVec.X = MoveVec.X * SkillRange * variables->SkillJoystick->JoyStickScale;
		MoveVec.Y = MoveVec.Y * SkillRange * variables->SkillJoystick->JoyStickScale;
		MoveVec.Z = 0;

		SkillLocation = UMapFunctionLibrary::GetNavAgentLocation(BasePC->GetLeaderCharacter());
		if (!isInDeadZone)
			SkillLocation += MoveVec;

		TArray< USkillAreaIndicatorMeshComponent* >	TargetComponentArray = UCharacterHelpFunctionLibrary::GetSkillAreaArray(BasePC->GetLeaderCharacter());
		for (auto& Element : TargetComponentArray)
		{
			bool IsPickArea = (Element && Element->GetSkillAreaMeshType() == ESkillAreaMeshTypeEnum::VE_SkillPickArea);
			if (IsPickArea == false)
				continue;

			ESearchShapeTypeEnum SkillShapeType = Element->GetSkillShapeType();
			if (SkillShapeType == ESearchShapeTypeEnum::VE_Circle || SkillShapeType == ESearchShapeTypeEnum::VE_Cylinder)
				Element->SkillAreaMeshUpdate(SkillLocation);
		}
	}

	//dummyActor Settings
	if (IsValid(DummyActor))
	{
		if (!isInDeadZone)
			DummyActor->SetActorHiddenInGame(false);

		UCapsuleComponent* CapsuleComp = UCharacterHelpFunctionLibrary::GetCapsuleComponent(DummyActor);
		if (IsValid(CapsuleComp))
		{
			float ScaledHeight = CapsuleComp->GetUnscaledCapsuleHalfHeight();
			FVector NewLocation = SkillLocation + FVector(0.f, 0.f, ScaledHeight);
			DummyActor->SetActorLocation(NewLocation);
		}

		UMaterialInstance* MaterialInstance = nullptr;
		bool bSpawnable = UMapFunctionLibrary::isTargetPointonNavMesh(SkillLocation); //UMapFunctionLibrary::IsValidLocation(SkillLocation) && UMapFunctionLibrary::CanMoveToTargetPosition(BasePC->GetLeaderCharacter(), SkillLocation) && UAIFunctionLibrary::CanMoveToTargetPosition(BasePC->GetLeaderCharacter(), SkillLocation);
		if (bSpawnable)
			MaterialInstance = UUtilFunctionLibrary::GetMaterialInstanceFromTable(FName(TEXT("Crew_Spawn_possible")));
		else
			MaterialInstance = UUtilFunctionLibrary::GetMaterialInstanceFromTable(FName(TEXT("Crew_Spawn_Impossible")));



		USkeletalMeshComponent* SkeletalMeshComponent = DummyActor->FindComponentByClass<USkeletalMeshComponent>();
		int32 MaterialSlotNum = SkeletalMeshComponent->GetNumMaterials();
		for (int32 i = 0; i < MaterialSlotNum; ++i)
		{
			//UMaterialInstanceDynamic* MID = UMaterialInstanceDynamic::Create(MaterialInstance, SkeletalMeshComponent);
			SkeletalMeshComponent->SetMaterial(i, MaterialInstance);
		}
	}

	return CheckBlackboardData();
}
