// Fill out your copyright notice in the Description page of Project Settings.

#include "ProjectR.h"
#include "UC_SkillIndicator.h"
//#include "HUDBase.h"

#include "UtilFunctionIntegrated.h"
#include "GlobalIntegrated.h"



#include "Table/CharacterTableInfo.h"
#include "Table/SkillTableInfo_V2.h"
#include "Table/SkillResourceTableInfo.h"


void UUC_SkillIndicator::NativeConstruct()
{
	Super::NativeConstruct();
	
	/*
	if(IsNotEvent)
		return;

	auto RGameInstance = RGAMEINSTANCE(this);
	if (IsValid(RGameInstance->EventManager))
	{
		RGameInstance->EventManager->OnIndicator.RemoveDynamic(this, &UUC_SkillIndicator::OnSkillIndicator);
		RGameInstance->EventManager->OnIndicator.AddDynamic(this, &UUC_SkillIndicator::OnSkillIndicator);
	}*/
}

void UUC_SkillIndicator::NativeDestruct()
{
	/*SkillImage = nullptr;
	HeroNameText = nullptr;
	HeroSkillNameText = nullptr;*/

	/*auto RGameInstance = RGAMEINSTANCE(this);
	if (IsValid(RGameInstance->EventManager))
	{
		RGameInstance->EventManager->OnIndicator.RemoveDynamic(this, &UUC_SkillIndicator::OnSkillIndicator);
	}*/

	Super::NativeDestruct();
}

void UUC_SkillIndicator::OnSkillIndicator(FName id)
{
	if (id.IsNone())
	{
		this->SetVisibility(ESlateVisibility::Collapsed);
	}
	else
	{
		//ShowSkillIndicator(id, 3.0f);
		this->SetVisibility(ESlateVisibility::HitTestInvisible);
		SetIndicator(id);
	}
}

void UUC_SkillIndicator::SetIndicator(FName& id, float InShowTime /*= 0.0f*/)
{
	/*FCharacterTableInfo* TableInfo = UTableManager::GetInstancePtr()->GetCharacterRow(id);
	if (TableInfo)
	{
		if (IsValid(HeroNameText))
		{
			HeroNameText->SetText(UUtilFunctionLibrary::GetLocalizedString(EStringTableType::VE_Hero, TableInfo->DisplayName));
		}
	}*/
	FName SkillTableID = UUtilFunctionLibrary::GetResultSkillID(ECharacterCategoryTypeEnum::VE_Hero, id, 1);

	FSkillTableInfo_V2* SkillTableInfo = UTableManager::GetInstancePtr()->GetSkillRow_V2(SkillTableID);
	if (SkillTableInfo)
	{
		/*if (IsValid(HeroImage))
		{
			HeroImage->SetBrushFromTexture(UUtilFunctionLibrary::GetTexture2DFromTable(TableInfo->PortraitTextureKey));
		}*/

			

		if (IsValid(HeroSkillNameText))
		{
			HeroSkillNameText->SetText(UUtilFunctionLibrary::GetLocalizedString(EStringTableType::VE_Skill, SkillTableInfo->SkillName));
		}

		if (HeroSkillDescText)
		{
			/*FWorkedSkillTableInfo SkillInfo;
			FHERO ViewHeroData;
			auto HeroData = RGameInstance->HeroInventory->GetInventoryData(HeroUDs[FindIndex]	, ViewHeroData);
			if (URenewal_BattleFunctionLibrary::GetSkillTableWorkedInfo(SkillTableInfo, ECharacterCategoryTypeEnum::VE_Hero, skillList[i], level))
			{
					UCharacterBattleInfo* BattleInfo = URenewal_BattleFunctionLibrary::MakeBObjectBattleInfoFromTable(RGameInstance, pditemData->IconInfo.heroId, level);
					if (IsValid(BattleInfo))
					{
						FText text = UUIFunctionLibrary::GetSkillDescText(SkillInfo, BattleInfo, false, false);
					}
			}*/
				
			/*
			//hud GetSelected
			//icon index
			auto SkillEntityComponent = UCharacterHelpFunctionLibrary::GetSkillEntityComponent(TargetCharacter.Get());
			if (IsValid(SkillEntityComponent))
			{
				int32		SkillCategoryNum = int32(ESkillCategoryTypeEnum::VE_ActiveSkill03);
				auto Skill = SkillEntityComponent->GetSkillByCategory_V2(ESkillCategoryTypeEnum::VE_ActiveSkill03);
				auto SkillInfo = Skill->SkillTableInfos[SkillCategoryNum];
				//auto UICharacterBattleInfo = URenewal_BattleFunctionLibrary::MakeCharacteBattleInfoFromInven(RGameInstance, CurrentHeroUD);

				//todo level - SkillTableInfo
				auto BattleEntityComponent = UCharacterHelpFunctionLibrary::GetBattleEntityComponent(OwnerActor.Get());
				if (IsValid(BattleEntityComponent))
				{
					auto BattleInfo = BattleEntityComponent->GetBattleInfo();
					//FWorkedSkillTableInfo...
					FText text = UUIFunctionLibrary::GetSkillDescText(SkillTableInfo, BattleInfo, false, false);
					HeroSkillDescText->SetText(text);
				}
			}
			*/
			if (InShowTime > 0) //temp
			{
				HeroSkillDescText->SetVisibility(ESlateVisibility::Collapsed);
			}
			else
			{
				FText text = UUtilFunctionLibrary::GetLocalizedString(EStringTableType::VE_Skill, SkillTableInfo->Summary);
				HeroSkillDescText->SetText(text);
			}
			//HeroSkillDescText->SetText(UUtilFunctionLibrary::GetLocalizedString(EStringTableType::VE_Skill, SkillTableInfo->Description));
		}

		if (IsValid(SkillImage))
		{
			FSkillResourceTableInfo* SkillResourceTableInfo = UTableManager::GetInstancePtr()->GetSkillResourceRow(SkillTableInfo->SkillResourceID);
			if (SkillResourceTableInfo)
			{
				SkillImage->SetBrushFromTexture(UUtilFunctionLibrary::GetTexture2DFromAssetID(SkillResourceTableInfo->SkillIconRef));
			}
		}

		if (InShowTime > 0) //temp
		{
			if (WidgetAnimationList.Num() > 0)
			{
				PlayAnimation(WidgetAnimationList[0]);
			}
		}
	}

	if (InShowTime > 0)
	{
		GetWorld()->GetTimerManager().SetTimer(DelayTimeHandle, this, &UUC_SkillIndicator::DelayEvent, InShowTime, false);
	}
}

void UUC_SkillIndicator::DelayEvent()
{
	this->SetVisibility(ESlateVisibility::Collapsed);
}