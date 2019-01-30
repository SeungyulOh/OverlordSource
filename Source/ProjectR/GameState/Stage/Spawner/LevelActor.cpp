
#include "ProjectR.h"
#include "ProjectRGameMode.h"
#include "LevelActor.h"
#include "GlobalIntegrated.h"
#include "UtilFunctionIntegrated.h"
#include "Character/Interfaces/EntityBaseProperty.h"
#include "Character/Components/FunctionalComponents/BattleEntityComponent.h"
#include "Table/GrowthTables/GrowthHeroMultiplier.h"
#include "CustomActors/BaseSequenceActor.h"

#include "UI/UI_Battle/RCampaignHUD_OneControl.h"
#include "UI/IngameHUD/UC_HUD_TowerHP.h"
#include "DirectionTrackManager.h"


ALevelActor::ALevelActor(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
#if WITH_UNREAL_DEVELOPER_TOOLS
	UTextRenderComponent*	TextRenderComponent = CreateEditorOnlyDefaultSubobject<UTextRenderComponent>(TEXT("TextRender"));
	if (IsValid(TextRenderComponent))
	{
		TextRenderComponent->SetupAttachment(RootComponent);
		TextRenderComponent->SetText(FText::FromString(GetName()));
		TextRenderComponent->SetTextRenderColor(FColor::Blue);
	}

	UShapeComponent*		ShapeComponent = GetCollisionComponent();
	ShapeComponent->ShapeColor = FColor::Red;
#endif
	PhaseList.Reset();
	CurPhase = 0;
	MaxPhase = 0;
}

void ALevelActor::ExecutePhase_Implementation(uint8 Step)
{

}

// void ALevelActor::GameStart_Implementation()
// {
// 
// }

void ALevelActor::StartPhaseCheck()
{
	if (EGamePlayModeTypeEnum::VE_Campaign != UUtilFunctionLibrary::GetGamePlayModeEnum())
		return;
	bPhaseCheck = true;
	ALevelActorPhase* LevelActorData = GetLevelActorPhase();

	URGameInstance*		GameInstance = RGAMEINSTANCE(this);
	UWorld*				OwnerWorld = GEngine->GetWorldFromContextObject(GameInstance, EGetWorldErrorMode::Assert);

	if (LevelActorData)
	{
		if (LevelActorData->ClearCondition == EPhaseClearEnumType::VE_PlayerSpawn)
			ConditionCheck(EPhaseClearEnumType::VE_PlayerSpawn);
		if(LevelActorData->ClearCondition == EPhaseClearEnumType::VE_Protect)
			ConditionCheck(EPhaseClearEnumType::VE_Protect);
		if (LevelActorData->ClearCondition == EPhaseClearEnumType::VE_ProtectDisable)
			ConditionCheck(EPhaseClearEnumType::VE_ProtectDisable);
		if (LevelActorData->ClearCondition == EPhaseClearEnumType::VE_ResistingEnable)
			ConditionCheck(EPhaseClearEnumType::VE_ResistingEnable);
		if (LevelActorData->ClearCondition == EPhaseClearEnumType::VE_ResistingDisable)
			ConditionCheck(EPhaseClearEnumType::VE_ResistingDisable);
	}
}

void ALevelActor::SetPhaseEndCodition(EPhaseClearEnumType codition /*= EPhaseClearEnumType::VE_MAX*/, int32 InParameter /*= 0*/)
{
	if (EGamePlayModeTypeEnum::VE_Campaign != UUtilFunctionLibrary::GetGamePlayModeEnum())
		return;
	UBaseStageInfo* BaseStageInfo = UGamePlayBluePrintFunction::GetBaseStageInfo();

	if (CurPhase > MaxPhase)
		return;

	ALevelActorPhase* LevelActorData = GetLevelActorPhase();

	if (LevelActorData == nullptr)
		return;

	LevelActorData->ClearCondition = codition;
	LevelActorData->Parameter = InParameter;
	LevelActorData->ResistingTime = InParameter;


	if (LevelActorData->ClearCondition == EPhaseClearEnumType::VE_Resisting || LevelActorData->ClearCondition == EPhaseClearEnumType::VE_ResistingEnable)
	{
		if (LevelActorData->ClearCondition == EPhaseClearEnumType::VE_ResistingEnable)
			IsResisting = true;

		if (TimeHandler_PhaseTime1.IsValid())
			this->GetWorldTimerManager().ClearTimer(TimeHandler_PhaseTime1);
		this->GetWorldTimerManager().SetTimer(TimeHandler_PhaseTime1, this, &ALevelActor::PhaseTimeOut, (float)InParameter, false);
	}

	if (LevelActorData->ClearCondition == EPhaseClearEnumType::VE_Resisting || LevelActorData->ClearCondition == EPhaseClearEnumType::VE_Protect || LevelActorData->ClearCondition == EPhaseClearEnumType::VE_ResistingEnable)
	{
		URBaseIngameHUD* IngameHUD = Cast<URBaseIngameHUD>(UUIFunctionLibrary::GetInGameHUD());

		if (IngameHUD == nullptr)
			return;

		IngameHUD->WidgetSwitcher_0->SetVisibility(ESlateVisibility::Visible);

		if (LevelActorData->ClearCondition == EPhaseClearEnumType::VE_Resisting)
			IngameHUD->WidgetSwitcher_0->SetActiveWidgetIndex(1);
		if (LevelActorData->ClearCondition == EPhaseClearEnumType::VE_Protect)
			IngameHUD->WidgetSwitcher_0->SetActiveWidgetIndex(0);
		if (IsResisting)
			IngameHUD->WidgetSwitcher_0->SetActiveWidgetIndex(1);

		if (IngameHUD->UI_ProtectMode->GetPhaseType() == EPhaseClearEnumType::VE_MAX)
			IngameHUD->UI_ProtectMode->ChangeMode(LevelActorData->ClearCondition);
		else if (IngameHUD->UI_ProtectMode2->GetPhaseType() == EPhaseClearEnumType::VE_MAX)
			IngameHUD->UI_ProtectMode2->ChangeMode(LevelActorData->ClearCondition);

	}
	IsResistingTimeOut = false;
	IsSpecificSpawn = false;
	SpecificMonster.Reset();
}

void ALevelActor::SetPhaseEndCodition2(TArray<FName> InSpecificMonster, EPhaseClearEnumType codition /*= EPhaseClearEnumType::VE_MAX*/, int32 InParameter /*= 0*/)
{
	if (EGamePlayModeTypeEnum::VE_Campaign != UUtilFunctionLibrary::GetGamePlayModeEnum())
		return;
	UBaseStageInfo* BaseStageInfo = UGamePlayBluePrintFunction::GetBaseStageInfo();

	if (CurPhase > MaxPhase)
		return;

	ALevelActorPhase* LevelActorData = GetLevelActorPhase();

	if (LevelActorData == nullptr)
		return;

	LevelActorData->ClearCondition = codition;
	LevelActorData->Parameter = InParameter;
	LevelActorData->ResistingTime = InParameter;
	LevelActorData->SpecificMonsterSpawnID = InSpecificMonster;

	if (LevelActorData->ClearCondition == EPhaseClearEnumType::VE_Resisting || LevelActorData->ClearCondition == EPhaseClearEnumType::VE_ResistingEnable)
	{
		if (LevelActorData->ClearCondition == EPhaseClearEnumType::VE_ResistingEnable)
			IsResisting = true;

		if (TimeHandler_PhaseTime2.IsValid())
			this->GetWorldTimerManager().ClearTimer(TimeHandler_PhaseTime2);
		this->GetWorldTimerManager().SetTimer(TimeHandler_PhaseTime2, this, &ALevelActor::PhaseTimeOut, (float)InParameter, false);
	}

	if (LevelActorData->ClearCondition == EPhaseClearEnumType::VE_Resisting ||	LevelActorData->ClearCondition == EPhaseClearEnumType::VE_Protect ||LevelActorData->ClearCondition == EPhaseClearEnumType::VE_ResistingEnable)
	{
		URBaseIngameHUD* IngameHUD = Cast<URBaseIngameHUD>(UUIFunctionLibrary::GetInGameHUD());

		if (IngameHUD == nullptr)
			return;

		IngameHUD->WidgetSwitcher_0->SetVisibility(ESlateVisibility::Visible);

		if (LevelActorData->ClearCondition == EPhaseClearEnumType::VE_Resisting)
			IngameHUD->WidgetSwitcher_0->SetActiveWidgetIndex(1);
		if (LevelActorData->ClearCondition == EPhaseClearEnumType::VE_Protect)
			IngameHUD->WidgetSwitcher_0->SetActiveWidgetIndex(0);
		if (IsResisting)
			IngameHUD->WidgetSwitcher_0->SetActiveWidgetIndex(1);

		if (IngameHUD->UI_ProtectMode->GetPhaseType() == EPhaseClearEnumType::VE_MAX)
			IngameHUD->UI_ProtectMode->ChangeMode(LevelActorData->ClearCondition);
		else if (IngameHUD->UI_ProtectMode2->GetPhaseType() == EPhaseClearEnumType::VE_MAX)
			IngameHUD->UI_ProtectMode2->ChangeMode(LevelActorData->ClearCondition);
	}
	IsResistingTimeOut = false;
	IsSpecificSpawn = false;
	SpecificMonster.Reset();
}
//	assistant
void ALevelActor::SetPhaseEndCodition3(EPhaseClearEnumType codition /*= EPhaseClearEnumType::VE_MAX*/, EPhaseClearEnumType assistantcodition /*= EPhaseClearEnumType::VE_MAX*/, int32 InParameter /*= 0*/)
{
	if (EGamePlayModeTypeEnum::VE_Campaign != UUtilFunctionLibrary::GetGamePlayModeEnum())
		return;

	UBaseStageInfo* BaseStageInfo = UGamePlayBluePrintFunction::GetBaseStageInfo();

	if (CurPhase > MaxPhase)
		return;

	ALevelActorPhase* LevelActorData = GetLevelActorPhase();

	if (LevelActorData == nullptr)
		return;

	LevelActorData->ClearCondition			=	codition;
	LevelActorData->assistantCondition		=	assistantcodition;
	LevelActorData->Parameter				=	InParameter;
	LevelActorData->ResistingTime			=	InParameter;

	if (LevelActorData->ClearCondition == EPhaseClearEnumType::VE_Resisting || LevelActorData->ClearCondition == EPhaseClearEnumType::VE_ResistingEnable)
	{
		if (LevelActorData->ClearCondition == EPhaseClearEnumType::VE_ResistingEnable)
			IsResisting = true;

		if (TimeHandler_PhaseTime2.IsValid())
			this->GetWorldTimerManager().ClearTimer(TimeHandler_PhaseTime2);
		this->GetWorldTimerManager().SetTimer(TimeHandler_PhaseTime2, this, &ALevelActor::PhaseTimeOut, (float)InParameter, false);
	}

	if (LevelActorData->ClearCondition == EPhaseClearEnumType::VE_Resisting ||LevelActorData->ClearCondition == EPhaseClearEnumType::VE_Protect ||LevelActorData->ClearCondition == EPhaseClearEnumType::VE_ResistingEnable)
	{
		URBaseIngameHUD* IngameHUD = Cast<URBaseIngameHUD>(UUIFunctionLibrary::GetInGameHUD());

		if (IngameHUD == nullptr)
			return;

		IngameHUD->WidgetSwitcher_0->SetVisibility(ESlateVisibility::Visible);

		if (LevelActorData->ClearCondition == EPhaseClearEnumType::VE_Resisting)
			IngameHUD->WidgetSwitcher_0->SetActiveWidgetIndex(1);
		if (LevelActorData->ClearCondition == EPhaseClearEnumType::VE_Protect)
			IngameHUD->WidgetSwitcher_0->SetActiveWidgetIndex(0);
		if (IsResisting)
			IngameHUD->WidgetSwitcher_0->SetActiveWidgetIndex(1);

		if (IngameHUD->UI_ProtectMode->GetPhaseType() == EPhaseClearEnumType::VE_MAX)
			IngameHUD->UI_ProtectMode->ChangeMode(LevelActorData->ClearCondition);
		else if (IngameHUD->UI_ProtectMode2->GetPhaseType() == EPhaseClearEnumType::VE_MAX)
			IngameHUD->UI_ProtectMode2->ChangeMode(LevelActorData->ClearCondition);
	}
	IsResistingTimeOut = false;
	IsSpecificSpawn = false;
	SpecificMonster.Reset();
}


void ALevelActor::SetPhase(int32 InMaxPhase)
{
	SetMaxPhase(InMaxPhase);

	for (int32 Phase = 0; Phase < InMaxPhase+1; Phase++)
	{
		ALevelActorPhase* LevelActorData = NewObject<ALevelActorPhase>();
		if (Phase == 0)
			LevelActorData->ClearCondition = EPhaseClearEnumType::VE_Dialogue;

		PhaseList.Emplace(Phase, LevelActorData);
	}
}

void ALevelActor::ConditionCheck(EPhaseClearEnumType InMode)
{
	if (EGamePlayModeTypeEnum::VE_Campaign != UUtilFunctionLibrary::GetGamePlayModeEnum())
		return;

	if (bPhaseCheck == false)
		return;

	ALevelActorPhase* LevelActorData = GetLevelActorPhase();

	if (LevelActorData)
	{
		EPhaseClearEnumType Condition = LevelActorData->ClearCondition;

 		if (Condition != InMode)
 			return;

		switch (Condition)
		{
			case	EPhaseClearEnumType::VE_PlayerSpawn:
			{
				NextPhase();
				break;
			}
			case	EPhaseClearEnumType::VE_Dialogue:
			{
				if (UGamePlayBluePrintFunction::IsShowDialog())
					break;

				NextPhase();
				break;
			}
			case	EPhaseClearEnumType::VE_Move:
			{
				NextPhase();
				break;
			}
			case	EPhaseClearEnumType::VE_AllKill:
			{
				AllKillEnd();
				break;
			}
			case	EPhaseClearEnumType::VE_BossKill:
			{
				KillBossEnd();
				break;
			}
			case	EPhaseClearEnumType::VE_Resisting:
			{
				ResistingEnd();
				break;
			}
			case	EPhaseClearEnumType::VE_Protect:
			{
				SetProtectNpc();
				break;
			}
			case	EPhaseClearEnumType::VE_ProtectDisable:
			{
				ProtectNpcDisable();
				break;
			}
			case	EPhaseClearEnumType::VE_MonsterCountCheck:
			{
				MonsterCountCheck();
				break;
			}
			case	EPhaseClearEnumType::VE_SpecificMonsterCountCheck:
			{
				SetSpecificMonsterCountCheck();
				break;
			}
			case	EPhaseClearEnumType::VE_CharacterAllSurvival:
			{
				SetCharacterAllSurvivalCheck();
				return;
			}
			case	EPhaseClearEnumType::VE_ResistingEnable:
			{
				NextPhase();
				break;
			}
			case	EPhaseClearEnumType::VE_ResistingDisable:
			{
				ResistingEnd();
				break;
			}
		}
	}

	if (MaxPhase < CurPhase)
		EndPhase();
}

void ALevelActor::ConditionCheck2(EPhaseClearEnumType InMode)
{
	if (EGamePlayModeTypeEnum::VE_Campaign != UUtilFunctionLibrary::GetGamePlayModeEnum())
		return;

	if (bPhaseCheck == false)
		return;

	ALevelActorPhase* LevelActorData = GetLevelActorPhase();

	if (LevelActorData)
	{
		EPhaseClearEnumType Condition = LevelActorData->assistantCondition;

		if (Condition != InMode)
			return;

		switch (Condition)
		{
			case	EPhaseClearEnumType::VE_PlayerSpawn:
			{
				NextPhase();
				break;
			}
			case	EPhaseClearEnumType::VE_Dialogue:
			{
				if (UGamePlayBluePrintFunction::IsShowDialog())
					break;

				NextPhase();
				break;
			}
			case	EPhaseClearEnumType::VE_Move:
			{
				NextPhase();
				break;
			}
			case	EPhaseClearEnumType::VE_AllKill:
			{
				AllKillEnd();
				break;
			}
			case	EPhaseClearEnumType::VE_BossKill:
			{
				KillBossEnd();
				break;
			}
			case	EPhaseClearEnumType::VE_Resisting:
			{
				ResistingEnd();
				break;
			}
			case	EPhaseClearEnumType::VE_Protect:
			{
				SetProtectNpc();
				break;
			}
			case	EPhaseClearEnumType::VE_ProtectDisable:
			{
				ProtectNpcDisable();
				break;
			}
			case	EPhaseClearEnumType::VE_MonsterCountCheck:
			{
				MonsterCountCheck();
				break;
			}
			case	EPhaseClearEnumType::VE_SpecificMonsterCountCheck:
			{
				SetSpecificMonsterCountCheck();
				break;
			}
			case	EPhaseClearEnumType::VE_CharacterAllSurvival:
			{
				SetCharacterAllSurvivalCheck();
				break;
			}
			case	EPhaseClearEnumType::VE_ResistingEnable:
			{
				NextPhase();
				break;
			}
			case	EPhaseClearEnumType::VE_ResistingDisable:
			{
				ResistingEnd();
				break;
			}
		}
	}

	if (MaxPhase < CurPhase)
		EndPhase();
}


void ALevelActor::Check_Have_InGameTuto()
{
	RGAMEINSTANCE(this)->ForceTutorialManager->SetCheckInGame();
}

void ALevelActor::FirstSpawn()
{
	RGAMEINSTANCE(this)->ForceTutorialManager->SetCheckFirstMosterSpawn();
}

ALevelActorPhase* ALevelActor::GetLevelActorPhase()
{
	if (EGamePlayModeTypeEnum::VE_Campaign != UUtilFunctionLibrary::GetGamePlayModeEnum())
		return nullptr;
	if (MaxPhase < CurPhase)
		return nullptr;
	if (PhaseList.GetAllocatedSize() <= 0)
		return nullptr;

	if (PhaseList.Contains(CurPhase))
	{
		ALevelActorPhase* LevelActorData = PhaseList[CurPhase];
		if (LevelActorData == nullptr)
			return nullptr;
		return LevelActorData;
	}
	return nullptr;
}

void ALevelActor::SetNpcHero(AActor* InHero)
{
	ALevelActorPhase* LevelActorData = GetLevelActorPhase();

	if (LevelActorData == nullptr)
		return;

	FProtectNpcList* Npc = ProtectNpc.Find(LevelActorData->Parameter);

	if (Npc == nullptr)
	{
		FProtectNpcList List;
		List.ActorList.Reset();
		List.ActorList.Emplace(InHero);
		ProtectNpc.Emplace(LevelActorData->Parameter, List);
	}
	else
	{
		Npc->ActorList.Emplace(InHero);
	}
}

bool ALevelActor::isNpcHeroDie(AActor* InHero)
{
	ALevelActorPhase* LevelActorData = GetLevelActorPhase();

	if (LevelActorData == nullptr)
		return false;

	for (auto Npc : ProtectNpc)
	{
		for (AActor* actor : Npc.Value.ActorList)
		{
			if (actor == InHero)
			{
				Npc.Value.ActorList.Reset();
				return true;
			}
		}
	}

	return false;
}

float ALevelActor::GetElapsedGameTime()
{
	URGameInstance*		GameInstance = RGAMEINSTANCE(this);

	ALevelActorPhase* LevelActorData = GetLevelActorPhase();

	if (LevelActorData == nullptr)
		return 0.0f;

	float time = this->GetWorldTimerManager().GetTimerRemaining(TimeHandler_PhaseTime1);

	if (time < 0.0f)
		time = 0.0f;

	return time;
}

void ALevelActor::PhaseTimeOut()
{
	if(TimeHandler_PhaseTime1.IsValid())
		GetWorldTimerManager().ClearTimer(TimeHandler_PhaseTime1);
	if(TimeHandler_PhaseTime2.IsValid())
		GetWorldTimerManager().ClearTimer(TimeHandler_PhaseTime2);
	ConditionCheck(EPhaseClearEnumType::VE_Resisting);
	ConditionCheck(EPhaseClearEnumType::VE_BossKill);

	URBaseIngameHUD* IngameHUD = Cast<URBaseIngameHUD>(UUIFunctionLibrary::GetInGameHUD());

	if (IngameHUD)
	{
		IngameHUD->WidgetSwitcher_0->SetVisibility(ESlateVisibility::Collapsed);

		if(IngameHUD->UI_ProtectMode->GetPhaseType() == EPhaseClearEnumType::VE_Resisting)
			IngameHUD->UI_ProtectMode->ResetPhaseType();
		else if (IngameHUD->UI_ProtectMode2->GetPhaseType() == EPhaseClearEnumType::VE_Resisting)
			IngameHUD->UI_ProtectMode2->ResetPhaseType();

		if (IsResisting)
		{
			IngameHUD->UI_ProtectMode->ResetPhaseType();
			IngameHUD->UI_ProtectMode2->ResetPhaseType();
		}
	}
	IsResisting = false;
	IsResistingTimeOut = true;
}

void ALevelActor::AllKillEnd()
{
	ALevelActorPhase* LevelActorData = GetLevelActorPhase();

	if (LevelActorData == nullptr)
		return;

	if (LevelActorData->MonsterSpawnCount <= LevelActorData->MonsterDieCount)
	{
		LevelActorData->MonsterSpawnCount = 0;
		LevelActorData->MonsterDieCount = 0;
		NextPhase();
	}
}

void ALevelActor::KillBossEnd()
{
	UBaseStageInfo* BaseStageInfo = UGamePlayBluePrintFunction::GetBaseStageInfo();
	TArray<AActor*>	Boss = BaseStageInfo->GetBoss();

	if (Boss.GetAllocatedSize() <= 0)
		NextPhase();
}

void ALevelActor::ResistingEnd()
{
	URGameInstance*		GameInstance	=	RGAMEINSTANCE(this);
	UBaseStageInfo*		BaseStageInfo	=	UGamePlayBluePrintFunction::GetBaseStageInfo();

	ALevelActorPhase* LevelActorData = GetLevelActorPhase();

	if (LevelActorData == nullptr)
		return;

	TArray< TWeakObjectPtr< AActor > > outList;

	this->GetWorldTimerManager().ClearTimer(TimeHandler_PhaseTime1);

	if (IsResisting == false)
	{
		if (LevelActorData->MonsterSpawnCount <= LevelActorData->MonsterDieCount)
		{
			LevelActorData->MonsterSpawnCount = 0;
			LevelActorData->MonsterDieCount = 0;
			ClearTimeHud();
		}
		else if (IsResistingTimeOut)
		{
			ClearTimeHud();
		}
	}
	else
	{
		ClearTimeHud();
	}

	IsResisting = false;
}

void ALevelActor::SetProtectNpc()
{
	ALevelActorPhase* LevelActorData = GetLevelActorPhase();

	if (LevelActorData == nullptr)
		return;

	auto Npc = ProtectNpc.Find(LevelActorData->Parameter);

	if (Npc == nullptr)
		return;
	NextPhase();
}

void ALevelActor::ProtectNpcDisable()
{
	ALevelActorPhase* LevelActorData = GetLevelActorPhase();

	if (LevelActorData == nullptr)
		return;

	URBaseIngameHUD* IngameHUD = Cast<URBaseIngameHUD>(UUIFunctionLibrary::GetInGameHUD());

	if (IngameHUD)
	{
		if (IngameHUD->UI_ProtectMode->GetPhaseType() == EPhaseClearEnumType::VE_Protect)
			IngameHUD->UI_ProtectMode->ResetPhaseType();
		else if (IngameHUD->UI_ProtectMode2->GetPhaseType() == EPhaseClearEnumType::VE_Protect)
			IngameHUD->UI_ProtectMode2->ResetPhaseType();
	}

	auto Npc = ProtectNpc.Find(LevelActorData->Parameter);

	if (Npc == nullptr)
		return;

	int32 SuccessNumber = ProtectNpc.Remove(LevelActorData->Parameter);

	if (SuccessNumber != 0)
		NextPhase();
}

void ALevelActor::NextPhase()
{
	if (CurPhase > GetMaxPhase())
		return;

	UBaseStageInfo* BaseStageInfo = UGamePlayBluePrintFunction::GetBaseStageInfo();

	if (BaseStageInfo == nullptr)
		return;

	ALevelActorPhase* LevelActorData = GetLevelActorPhase();

	if (LevelActorData == nullptr)
		return;

	CurPhase++;

	bPhaseCheck = false;
	IsResistingTimeOut = false;

	BaseStageInfo->ArrMonsterInfo.Reset();
	
	ExecutePhase(CurPhase);
	
}

void ALevelActor::EndPhase()
{
	UGamePlayBluePrintFunction::StageClearSuccess();
}

void ALevelActor::SetSpecificMonster(FName InSpecificMonsterSpawnID, AActor* InSpecificMonsterActor)
{
	ALevelActorPhase* LevelActorData = GetLevelActorPhase();

	if (LevelActorData == nullptr)
		return;

	if (LevelActorData->ClearCondition != EPhaseClearEnumType::VE_SpecificMonsterCountCheck)
		return;

	if (LevelActorData->SpecificMonsterSpawnID.Find(InSpecificMonsterSpawnID) != INDEX_NONE)
	{
		IsSpecificSpawn = true;
		SpecificMonster.Emplace(InSpecificMonsterActor);
	}
}

void ALevelActor::BeginDestroy()
{
	PhaseList.Reset();
	CurPhase = 0;
	MaxPhase = 0;
	ProtectNpc.Reset();
	Super::BeginDestroy();
}

void ALevelActor::MonsterCountCheck()
{
	UBaseStageInfo* BaseStageInfo = UGamePlayBluePrintFunction::GetBaseStageInfo();

	if (BaseStageInfo == nullptr)
		return;

	ALevelActorPhase* LevelActorData = GetLevelActorPhase();

	if (LevelActorData == nullptr)
		return;

	if (LevelActorData->Parameter != 0)
	{
		if (LevelActorData->Parameter <= LevelActorData->MonsterDieCount)
		{
			LevelActorData->MonsterSpawnCount = 0;
			LevelActorData->MonsterDieCount = 0;
			NextPhase();
		}
	}
}

void ALevelActor::SetSpecificMonsterCountCheck()
{
	ALevelActorPhase* LevelActorData = GetLevelActorPhase();

	if (LevelActorData == nullptr)
		return;

	if (IsSpecificSpawn && SpecificMonster.GetAllocatedSize() <= 0)
		NextPhase();
}

void ALevelActor::SetCharacterAllSurvivalCheck()
{
	URGameInstance*		GameInstance	=	RGAMEINSTANCE(this);
	UBaseStageInfo*		BaseStageInfo	=	UGamePlayBluePrintFunction::GetBaseStageInfo();

	if (BaseStageInfo == nullptr)
		return;

	ALevelActorPhase* LevelActorData = GetLevelActorPhase();

	if (LevelActorData == nullptr)
		return;

	int32 DieCount = GameInstance->PlayStateInfo->GetHeroDieCount();

	if (DieCount == 0)
		ConditionCheck2(LevelActorData->assistantCondition);
}

void ALevelActor::ClearTimeHud()
{
	URBaseIngameHUD* IngameHUD = Cast<URBaseIngameHUD>(UUIFunctionLibrary::GetInGameHUD());

	NextPhase();

	if (IngameHUD)
		IngameHUD->WidgetSwitcher_0->SetVisibility(ESlateVisibility::Collapsed);
}
