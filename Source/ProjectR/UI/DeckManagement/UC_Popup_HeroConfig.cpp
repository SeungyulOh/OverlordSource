// Fill out your copyright notice in the Description page of Project Settings.

#include "ProjectR.h"
#include "UC_Popup_HeroConfig.h"

#include "GlobalIntegrated.h"


#include "GameInfo/UserInfo.h"

#include "UtilFunctionIntegrated.h"

#include "UI/Common/Renewal_BaseHeroIcon.h"
#include "UI/RWidgetManager.h"
#include "Network/PacketFunctionLibrary.h"


void UUC_Popup_HeroConfig::NativeConstruct()
{
	Super::NativeConstruct();

// 	URGameInstance* RGameInstance = RGAMEINSTANCE(this);
// 	if (!IsValid(RGameInstance))
// 		return;
	//if(IsValid(RGameInstance->UserInfo->GetHeroConfig())

	/*for (int32 i = 0; i < 5; i++)
	{
		BeforeSaveConfig.Emplace(FHERO_CONFIG());
	}*/
	BeforeSaveConfig.SetNumZeroed(5);

	for (FHERO_CONFIG& config : BeforeSaveConfig)
	{
		config.heroUD = "";
		config.preferTarget = EPREFER_TARGET_TYPE::NONE;
		config.skills.SetNum(3);
		config.skills[0] = true;
		config.skills[1] = true;
		config.skills[2] = true;
	}
}

void UUC_Popup_HeroConfig::NativeDestruct()
{
	BeforeSaveConfig.Empty();

	Super::NativeDestruct();
}

void UUC_Popup_HeroConfig::OnClickOK()
{
// 	UPacketFunctionLibrary::HERO_CONFIG_LIST_SET_RQ(this, (int32)eContentsType,
// 		BeforeSaveConfig[0].heroUD, (int32)BeforeSaveConfig[0].preferTarget, BeforeSaveConfig[0].skills,
// 		BeforeSaveConfig[1].heroUD, (int32)BeforeSaveConfig[1].preferTarget, BeforeSaveConfig[1].skills,
// 		BeforeSaveConfig[2].heroUD, (int32)BeforeSaveConfig[2].preferTarget, BeforeSaveConfig[2].skills,
// 		BeforeSaveConfig[3].heroUD, (int32)BeforeSaveConfig[3].preferTarget, BeforeSaveConfig[3].skills,
// 		BeforeSaveConfig[4].heroUD, (int32)BeforeSaveConfig[4].preferTarget, BeforeSaveConfig[4].skills);
// 	OnClickCancel();
}

void UUC_Popup_HeroConfig::OnClickCancel()
{
	URGameInstance* RGameInstance = RGAMEINSTANCE(this);
	for (FHERO_CONFIG& config : BeforeSaveConfig)
	{
		FHERO_CONFIG original;
		if (RGameInstance->UserInfo->GetHeroConfig(eContentsType, config.heroUD, original))
		{
			config.contents = eContentsType;
			config.heroUD = original.heroUD;
			config.preferTarget = original.preferTarget;
			config.skills = original.skills;
		}
		else
		{
			DefaultResetConfig(config);
// 			config.heroUD = "";
// 			config.preferTarget = EPREFER_TARGET_TYPE::NONE;
// 			config.skills[0] = true;
// 			config.skills[1] = true;
// 			config.skills[2] = true;
		}
	}
}

void UUC_Popup_HeroConfig::SelectHero(int32 InIndex)
{
	for (int32 i = 0; i < HeroIcons.Num(); ++i)//URenewal_BaseHeroIcon* icon : HeroIcons)
	{
		URenewal_BaseHeroIcon* icon = nullptr;
		if (HeroIcons.IsValidIndex(i))
			icon = HeroIcons[i];

		if (IsValid(icon))
		{
			icon->SetIsSelectEnable(true);
			icon->SetIsSelected((i == InIndex));
			icon->SelectIconHero();
		}
	}
	
	FHERO_ICON_INFO heroInfo;
	if(HeroIcons.IsValidIndex(InIndex) && IsValid(HeroIcons[InIndex]))
	{
		HeroIcons[InIndex]->GetHeroIconInfo(heroInfo);
	}

	CurrentHeroIndex = InIndex;
	BeforeSaveConfig[CurrentHeroIndex].heroUD = heroInfo.heroUD;

	FCharacterTableInfo* CharacterTableInfo = UTableManager::GetInstancePtr()->GetCharacterRow(heroInfo.heroId);
	if (CharacterTableInfo)
	{
		TArray<FName> SKills;
		SKills.Emplace(CharacterTableInfo->ActiveSkill01);
		SKills.Emplace(CharacterTableInfo->ActiveSkill02);
		SKills.Emplace(CharacterTableInfo->ActiveSkill03);

		for (int32 i = 0; i < 3; ++i)
		{
			if (SKills[i].IsNone())
				continue;

			if(Skill_3_Box)
			{
				if (i == 2 && heroInfo.level < 15)
					Skill_3_Box->SetVisibility(ESlateVisibility::Collapsed);
				else
					Skill_3_Box->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
			}

			FName SkillTableId = UUtilFunctionLibrary::GetResultSkillID(ECharacterCategoryTypeEnum::VE_Hero, SKills[i], 1);
			FSkillTableInfo_V2* SkillInfo = UTableManager::GetInstancePtr()->GetSkillRow_V2(SkillTableId);
			
			if (SkillInfo)
			{
				if (SkillNameText.IsValidIndex(i) && IsValid(SkillNameText[i]))
				{
					FText SkillName = UUtilFunctionLibrary::GetLocalizedString(EStringTableType::VE_Skill, SkillInfo->SkillName);
					SkillNameText[i]->SetText(SkillName);
				}


				FSkillResourceTableInfo* SkillResourceRow = UTableManager::GetInstancePtr()->GetSkillResourceRow(SkillInfo->SkillResourceID);
				if (SkillResourceRow)
				{
					if (SkillImages.IsValidIndex(i) && IsValid(SkillImages[i]))
					{
						SkillImages[i]->SetBrushFromTexture(UUtilFunctionLibrary::GetTexture2DFromAssetID(SkillResourceRow->SkillIconRef));
					}

				}
			}
		}
	}

	SelectPreferTarget((int32)BeforeSaveConfig[CurrentHeroIndex].preferTarget);
	SetAutoSkill();
}

void UUC_Popup_HeroConfig::SelectPreferTarget(int32 InIndex)
{
	int32 i = 0;
	for (UImage* target : TargetSelectImages)
	{
		if(IsValid(target))
			target->SetVisibility(i == InIndex ? ESlateVisibility::SelfHitTestInvisible : ESlateVisibility::Collapsed);
		++i;
	}

	BeforeSaveConfig[CurrentHeroIndex].preferTarget = (EPREFER_TARGET_TYPE)InIndex;
}

void UUC_Popup_HeroConfig::ResetSkillSelect()
{
	for (auto image : AutoUseSkillImages)
	{
		if (image)
		{
			image->SetVisibility(ESlateVisibility::Collapsed);
		}
	}

	if (BeforeSaveConfig.IsValidIndex(CurrentHeroIndex) )
	{
		BeforeSaveConfig[CurrentHeroIndex].skills[0] = true;
		BeforeSaveConfig[CurrentHeroIndex].skills[1] = true;
		BeforeSaveConfig[CurrentHeroIndex].skills[2] = true;
	}
}

void UUC_Popup_HeroConfig::SelectAutoSkill(int32 InIndex)
{
	if (BeforeSaveConfig.IsValidIndex(CurrentHeroIndex) && BeforeSaveConfig[CurrentHeroIndex].skills.IsValidIndex(InIndex))
	{
		BeforeSaveConfig[CurrentHeroIndex].skills[InIndex] = !BeforeSaveConfig[CurrentHeroIndex].skills[InIndex];
	}

	SetAutoSkill();
}

void UUC_Popup_HeroConfig::SetAutoSkill()
{
	for(int32 i = 0; i < 3; ++i)
	{
		if (AutoUseSkillImages.IsValidIndex(i))
		{
			if (BeforeSaveConfig.IsValidIndex(CurrentHeroIndex) && BeforeSaveConfig[CurrentHeroIndex].skills.IsValidIndex(i))
			{
				if (!BeforeSaveConfig[CurrentHeroIndex].skills[i])
				{
					AutoUseSkillImages[i]->SetVisibility(ESlateVisibility::Collapsed);
				}
				else
				{
					AutoUseSkillImages[i]->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
				}
			}
		}
	}
}

void UUC_Popup_HeroConfig::DefaultSetting()
{
	//SelectHero(0);
	SelectPreferTarget(0);
	ResetSkillSelect();
	SetAutoSkill();
}

void UUC_Popup_HeroConfig::DefaultResetConfig(FHERO_CONFIG& InConfig)
{
	InConfig.heroUD = "";
	InConfig.preferTarget = EPREFER_TARGET_TYPE::NONE;
	InConfig.skills[0] = true;
	InConfig.skills[1] = true;
	InConfig.skills[2] = true;
}

void UUC_Popup_HeroConfig::ShowPopup(ECONTENTS_TYPE InType, FString InHeroUD1, FString InHeroUD2, FString InHeroUD3, FString InHeroUD4, FString InHeroUD5)
{
	URGameInstance* RGameInstance = RGAMEINSTANCE(this);
// 	if (!IsValid(RGameInstance))
// 		return;

	eContentsType = InType;

	TArray<FString> heroUDs;
	heroUDs.Reserve(5);
	heroUDs.Emplace(InHeroUD1);
	heroUDs.Emplace(InHeroUD2);
	heroUDs.Emplace(InHeroUD3);
	heroUDs.Emplace(InHeroUD4);
	heroUDs.Emplace(InHeroUD5);
	
	//DefaultSetting();
	int32 nInit = 0;
	for (FHERO_CONFIG& config : BeforeSaveConfig)
	{
		if (!RGameInstance->UserInfo->GetHeroConfig(InType, heroUDs[nInit++], config))
		{
			DefaultResetConfig(config);
		}
	}

	FString SelectedHeroUD = "";
	int32 nFirstIndex = 0;
	for (FString& ud : heroUDs)
	{
		if (ud.IsEmpty() == false)
		{
			SelectedHeroUD = ud;
			break;
		}
		++nFirstIndex;
	}

	int32 index = 0;
	//int32 TargetIndex = -1;
	for(auto& heroUD : heroUDs)
	{
		if (HeroIcons.IsValidIndex(index) && HeroIcons_Box.IsValidIndex(index))
		{
			if(!heroUD.IsEmpty())
			{
// 				FHERO_ICON_INFO IconInfo;
// 				UUIFunctionLibrary::GetHeroIconInfoWithUD(IconInfo, heroUD, EHeroOwnershipType::VE_Me);
				HeroIcons[index]->SetHeroIconInfoWithUD(heroUD, EHeroOwnershipType::VE_Me);
				HeroIcons_Box[index]->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
				//if (TargetIndex == -1)
				//	TargetIndex = index;
			}
			else
			{
				HeroIcons_Box[index]->SetVisibility(ESlateVisibility::Collapsed);
			}
		}	
		++index;
	}

	/*if (TargetIndex == -1)
	{
		//empty hero
		//return;
	}*/

	SelectHero(nFirstIndex);
	//ResetSkillSelect();

	FHERO_CONFIG outConfig;
	if (RGameInstance->UserInfo->GetHeroConfig(InType, SelectedHeroUD, outConfig))
	{
		SelectPreferTarget((int32)outConfig.preferTarget);
		SetAutoSkill();
	}
	else
	{
		DefaultSetting();
	}

}