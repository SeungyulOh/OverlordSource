

#include "ProjectR.h"
#include "RMultiBossBar.h"

#include "GlobalIntegrated.h"

#include "UtilFunctionIntegrated.h"
#include "UI/Common/BaseMonsterIcon.h"
#include "UI/Component/ProgressBar_Phase.h"

void URMultiBossBar::NativeConstruct()
{
	Super::NativeConstruct();

	FindChildWidgets<UBaseMonsterIcon>(this, BossPortrait);

	if (RGAMEINSTANCE(this) == nullptr)
		return;

	auto EventManager = RGAMEINSTANCE(this)->EventManager;
	if (EventManager == nullptr)
		return;

	EventManager->OnMultiBossSpawn.RemoveDynamic(this, &URMultiBossBar::MultiBossSpawnEvent);
	EventManager->OnMultiBossSpawn.AddDynamic(this, &URMultiBossBar::MultiBossSpawnEvent);

	EventManager->OnBossGuageHide.RemoveDynamic(this, &URMultiBossBar::MultiBossHideEvent);
	EventManager->OnBossGuageHide.AddDynamic(this, &URMultiBossBar::MultiBossHideEvent);

	BossGaugeDown_Ref = GetAni("BossGaugeDown");
	BossGaugeUp_Ref = GetAni("BossGaugeUp");
	this->SetVisibility(ESlateVisibility::Collapsed);
}

void URMultiBossBar::NativeDestruct()
{
	if (RGAMEINSTANCE(this))
	{
		auto EventManager = RGAMEINSTANCE(this)->EventManager;
		if (EventManager)
		{
			EventManager->OnMultiBossSpawn.RemoveDynamic(this, &URMultiBossBar::MultiBossSpawnEvent);
			EventManager->OnBossGuageHide.RemoveDynamic(this, &URMultiBossBar::MultiBossHideEvent);
		}
	}
	
	Super::NativeDestruct();
}

void URMultiBossBar::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);


	if (0 < TargetCharacterList.Num())
	{
		if (ProgressBar_Base)
		{
			int32 PhaseValue = ProgressBar_Base->GetCurrentPhase();
			if (PhaseValue != CheckPhaseValue)
			{
				CheckPhaseValue = PhaseValue;
				if (Text_BossHPCount)
				{
					Text_BossHPCount->SetText(FText::AsNumber(PhaseValue));
				}
			}

			if (CheckPhaseValue == 0 && ProgressBar_Base->GetCurrentHP() <= 0)
			{
				CheckPhaseValue = -1;
				MultiBossHideEvent();
			}
		}
		for (int i = 0; i < TargetCharacterList.Num(); ++i)
		{
			if(TargetCharacterList.IsValidIndex(i) == false)
				continue;
			if(BossPortrait.IsValidIndex(i) == false)
				continue;
			if (BossPortrait[i]->IsDeath())
				continue;

			AActor* boss = TargetCharacterList[i];
			
			UBattleEntityComponent* BattleEntityComponent = UCharacterHelpFunctionLibrary::GetBattleEntityComponent(boss);
			if (BattleEntityComponent)
			{
				auto CharacterBattleInfo = BattleEntityComponent->GetBattleInfo();
				if (CharacterBattleInfo)
				{
					if (CharacterBattleInfo->GetHP() <= 0)
					{
						BossPortrait[i]->SetDeath(true);
					}
				}
			}

		}
	}
	else
	{
		if (eTargetCategory != ECharacterCategoryTypeEnum::VE_None)
		{
			MultiBossHideEvent();
		}
	}
}

void URMultiBossBar::MultiBossSpawnEvent(TArray<AActor*> InCharacterList)
{
	if (InCharacterList.Num() == 0)
		return;

	this->SetVisibility(ESlateVisibility::Visible);

	TargetCharacterList = InCharacterList;

	bBossTypeTarget = true;
	eTargetCategory = UCharacterHelpFunctionLibrary::GetCharacterCategory(InCharacterList[0]);

	AActor* TargetCharacter = InCharacterList[0];

	for (auto portrait : BossPortrait)
	{
		portrait->SetVisibility(ESlateVisibility::Collapsed);
	}

	int32 index = 0;
	for (AActor* boss : InCharacterList)
	{
		if (IsValid(boss) && BossPortrait.IsValidIndex(index))
		{
			FName bossname = UCharacterHelpFunctionLibrary::GetCharacterID(boss);
			int32 bosslevel = 0;
			UBattleEntityComponent* BattleEntityComponent = UCharacterHelpFunctionLibrary::GetBattleEntityComponent(boss);
			if (BattleEntityComponent)
			{
				bosslevel = BattleEntityComponent->GetBattleInfo()->GetLevel();
			}

			FHERO_ICON_INFO NPCIconInfo;
			UUIFunctionLibrary::GetHeroIconInfoForNpc(NPCIconInfo, bossname);
			NPCIconInfo.level = bosslevel;
			BossPortrait[index]->SetMonsterIconByIconInfo(NPCIconInfo, true);
			BossPortrait[index]->SetDeath(false);
			BossPortrait[index]->SetVisibility(ESlateVisibility::SelfHitTestInvisible);

		}
		++index;
	}
	if (ProgressBar_Base)
	{
		//ProgressBar_Base->SetCustomPhaseColor();
		ProgressBar_Base->SetTargetActorList(InCharacterList, EProgressTypeEnum::VE_Layer1);
		ProgressBar_Base->GetPhasePercent();
		ProgressBar_Base->SynchronizeProperties();

		int32 PhaseValue = ProgressBar_Base->GetCurrentPhase();
		if (Text_BossHPCount)
		{
			Text_BossHPCount->SetText(FText::AsNumber(PhaseValue));
		}
	}
	if (ProgressBar_Deco)
	{
		//ProgressBar_Deco->SetCustomPhaseColor();
		ProgressBar_Deco->SetTargetActorList(InCharacterList, EProgressTypeEnum::VE_Layer2);
		ProgressBar_Deco->GetPhasePercent();
		ProgressBar_Deco->SynchronizeProperties();
	}

	//Set BossTableData
	UBattleEntityComponent* BattleEntityComponent = UCharacterHelpFunctionLibrary::GetBattleEntityComponent(TargetCharacter);
	if (IsValid(BattleEntityComponent))
	{
		if (IsValid(TextBlock_BossLevel))
		{
			if (BattleEntityComponent->GetBattleInfo())
			{
				int32 lv = BattleEntityComponent->GetBattleInfo()->GetLevel();
				TextBlock_BossLevel->SetText(FText::AsNumber(lv));
			}
		}

		if (BattleEntityComponent->IsBoss())
		{
			this->SetVisibility(ESlateVisibility::HitTestInvisible);

			if (BossGaugeDown_Ref)
			{
				if (IsAnimationPlaying(BossGaugeUp_Ref))
				{
					StopAnimation(BossGaugeUp_Ref);
				}
				PlayAnimation(BossGaugeDown_Ref);
			}
		}
	}

	SynchronizeBossData();
}


void URMultiBossBar::SynchronizeBossData()
{
	if (TargetCharacterList.Num() == 0)
		return;

	AActor* TargetCharacter = TargetCharacterList[0];

	FText BossNameText = UUtilFunctionLibrary::GetBossName(TargetCharacter);

	if (LocalText_BossName)
		LocalText_BossName->SetText(BossNameText);
}

void URMultiBossBar::MultiBossHideEvent()
{
	if (eTargetCategory != ECharacterCategoryTypeEnum::VE_None)
	{
		if (IsValid(BossGaugeDown_Ref) && IsValid(BossGaugeUp_Ref))
		{
			if (IsAnimationPlaying(BossGaugeDown_Ref))
			{
				StopAnimation(BossGaugeDown_Ref);
			}
			PlayAnimation(BossGaugeUp_Ref);
		}
	}

	eTargetCategory = ECharacterCategoryTypeEnum::VE_None;
}
