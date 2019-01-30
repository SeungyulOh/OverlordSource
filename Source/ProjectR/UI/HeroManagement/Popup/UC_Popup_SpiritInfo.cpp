// Fill out your copyright notice in the Description page of Project Settings.

#include "ProjectR.h"
#include "UC_Popup_SpiritInfo.h"
#include "UI/Common/Renewal_BaseHeroIcon.h"
#include "UI/ItemManagement/UC_GenericPopup.h"
#include "UtilFunctionIntegrated.h"

#include "GlobalIntegrated.h"
#include "GameInfo/RInventory.h"
#include "Table/SpiritSkill.h"

void UUC_Popup_SpiritInfo::NativeConstruct()
{
 	Super::NativeConstruct();
	if (IsValid(Button_Close))
	{
		Button_Close->OnClicked.RemoveDynamic(this, &UUC_Popup_SpiritInfo::OnClick_Close);
		Button_Close->OnClicked.AddDynamic(this, &UUC_Popup_SpiritInfo::OnClick_Close);
	}

	Descriptions.Empty();
	FindChildWidgetsWithCertainName<ULocalizingTextBlock>(this, Descriptions, TEXT("Description"));
	for (ULocalizingTextBlock* block : Descriptions)
		block->SetVisibility(ESlateVisibility::Collapsed);
}

void UUC_Popup_SpiritInfo::NativeDestruct()
{	
	if (IsValid(Button_Close))
	{
		Button_Close->OnClicked.RemoveDynamic(this, &UUC_Popup_SpiritInfo::OnClick_Close);
	}
	Super::NativeDestruct();
}

void UUC_Popup_SpiritInfo::OnClick_Close()
{
	ParentPopup->Close();
}

void UUC_Popup_SpiritInfo::InvalidateData()
{
	if (IsValid(UC_Icon_Hero))
	{
// 		FHERO_ICON_INFO HeroIconInfo;
// 		UUIFunctionLibrary::GetHeroIconInfoWithUD(HeroIconInfo, CurSpiritUD, EHeroOwnershipType::VE_Me);
		UC_Icon_Hero->SetHeroIconInfoWithUD(CurSpiritUD, EHeroOwnershipType::VE_Me);
		URGameInstance* RGameInstance = RGAMEINSTANCE(this);
		checkf(RGameInstance != nullptr, TEXT("UUC_HeroMaterial_Popup[RGameInstance Is Empty]"));
		checkf(RGameInstance->RInventory != nullptr, TEXT("UUC_HeroMaterial_Popup[RInventory Is Empty]"));
		FHERO* HeroData = RGameInstance->RInventory->GetHeroWithUD(CurSpiritUD);
		if (HeroData != nullptr) {
			FCharacterTableInfo* CharacterTableInfo = UTableManager::GetInstancePtr()->GetCharacterRow(HeroData->heroId);
			if (CharacterTableInfo->SpiritPassive.IsNone())
				return;
			if(IsValid(LocalTextBlock_SpiritName))
				LocalTextBlock_SpiritName->SetText(UUtilFunctionLibrary::GetLocalizedString(EStringTableType::VE_Hero, CharacterTableInfo->DisplayName));
			
			FSkillTableInfo_V2* SpiritSkill = UTableManager::GetInstancePtr()->GetSkillRow_V2(CharacterTableInfo->SpiritPassive);
			if (SpiritSkill == nullptr)
				return;
			FText SkillName = UUtilFunctionLibrary::GetLocalizedString(EStringTableType::VE_Skill, SpiritSkill->SkillName);
			LocalTextBlock_SkillName->SetText(SkillName);
			TArray<FText> Desc;
			FText SkillDesc;
			if (!SpiritSkill->Summary.IsNone())
			{
				SkillDesc = UUtilFunctionLibrary::GetLocalizedString(EStringTableType::VE_Skill, SpiritSkill->Summary);
				Desc.Emplace(SkillDesc);
			}
			/*if (!SpiritSkill->Description2.IsNone())
			{
				SkillDesc = UUtilFunctionLibrary::GetLocalizedString(EStringTableType::VE_Skill, SpiritSkill->Description2);
				Desc.Emplace(SkillDesc);
			}
			if (!SpiritSkill->Description3.IsNone())
			{
				SkillDesc = UUtilFunctionLibrary::GetLocalizedString(EStringTableType::VE_Skill, SpiritSkill->Description3);
				Desc.Emplace(SkillDesc);
			}
			if (!SpiritSkill->Description4.IsNone())
			{
				SkillDesc = UUtilFunctionLibrary::GetLocalizedString(EStringTableType::VE_Skill, SpiritSkill->Description4);
				Desc.Emplace(SkillDesc);
			}*/

			for (int32 i = 0; i < Desc.Num(); i++)
			{
				Descriptions[i]->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
				Descriptions[i]->SetText(Desc[i]);
			}
			LocalTextBlock_Time->SetText(FText::AsNumber(SpiritSkill->CrewCoolTime));

			FSkillResourceTableInfo* SkillResourceRow = UTableManager::GetInstancePtr()->GetSkillResourceRow(SpiritSkill->SkillResourceID);
			if (SkillResourceRow)
			{
				if (IsValid(SkillIcon))
				{
					SkillIcon->SetBrushFromTexture(UUtilFunctionLibrary::GetTexture2DFromAssetID(SkillResourceRow->SkillIconRef));
				}
			}
		}
	}
}