// Fill out your copyright notice in the Description page of Project Settings.

#include "ProjectR.h"
#include "UC_Popup_SoulInfo.h"
#include "Global/TableManager.h"
#include "UtilFunctionLibrary.h"
#include "Table/SpiritSkill.h"
#include "GameInfo/RInventory.h"
#include "UI/RWidgetManager.h"


void UUC_Popup_SoulInfo::NativeConstruct()
{
	if (Button_OK)
		Button_OK->OnClicked.AddDynamic(this, &UUC_Popup_SoulInfo::CloseWidget);

	if(Button_Close)
		Button_Close->OnClicked.AddDynamic(this, &UUC_Popup_SoulInfo::CloseWidget);

	Text_SkillDescs.Empty();
	FindChildWidgetsWithCertainName<ULocalizingTextBlock>(this, Text_SkillDescs, TEXT("Text_SkillDesc"));
	for (ULocalizingTextBlock* block : Text_SkillDescs)
		block->SetVisibility(ESlateVisibility::Collapsed);

	this->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
}

void UUC_Popup_SoulInfo::NativeDestruct()
{
	if (Button_OK)
		Button_OK->OnClicked.RemoveDynamic(this, &UUC_Popup_SoulInfo::CloseWidget);

	if (Button_Close)
		Button_Close->OnClicked.RemoveDynamic(this, &UUC_Popup_SoulInfo::CloseWidget);
}

void UUC_Popup_SoulInfo::BindData(FString spiritUD)
{
	if(IsValid(UC_Icon_Soul))
		UC_Icon_Soul->SetHeroIconInfoWithUD(spiritUD, EHeroOwnershipType::VE_Me);
	FHERO* HeroData = RGAMEINSTANCE(this)->RInventory->GetHeroWithUD(spiritUD);
	if (HeroData) {
		FCharacterTableInfo* CharacterTableInfo = UTableManager::GetInstancePtr()->GetCharacterRow(HeroData->heroId);

		Text_SoulName->SetText(UUtilFunctionLibrary::GetLocalizedString(EStringTableType::VE_Hero, CharacterTableInfo->DisplayName));
		Text_Explain->SetText(UUtilFunctionLibrary::GetLocalizedString(EStringTableType::VE_Hero, CharacterTableInfo->Summary));


		FSkillTableInfo_V2* SpiritSkill = UTableManager::GetInstancePtr()->GetSkillRow_V2(CharacterTableInfo->SpiritPassive);
		Text_SkillLevel->SetText(FText::Format(FText::FromString(TEXT("Lv.{0}")), FText::AsNumber(SpiritSkill->CasterSlotLevel)));
		Text_SkillName->SetText(UUtilFunctionLibrary::GetLocalizedString(EStringTableType::VE_Skill, SpiritSkill->SkillName));

		TArray<FText> Desc;
		FText aa;
		FText SkillDesc;

		if (!SpiritSkill->Summary.IsNone())
		{
			SkillDesc = UUtilFunctionLibrary::GetLocalizedString(EStringTableType::VE_Skill, SpiritSkill->Summary);
			Desc.Emplace(SkillDesc);
		}

		for (int32 i = 0; i < Desc.Num(); i++)
		{
			Text_SkillDescs[i]->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
			Text_SkillDescs[i]->SetText(FText::Format(Desc[i] , FText::AsNumber(SpiritSkill->CasterPercent1) , FText::FromString(TEXT("%"))));
		}

		FSkillResourceTableInfo* SkillResourceRow = UTableManager::GetInstancePtr()->GetSkillResourceRow(SpiritSkill->SkillResourceID);
		if (SkillResourceRow)
		{
			if (IsValid(UC_Hero_SKill->Image_SkillIcon))
			{
				UC_Hero_SKill->Image_SkillIcon->SetBrushFromTexture(UUtilFunctionLibrary::GetTexture2DFromAssetID(SkillResourceRow->SkillIconRef));
			}
		}
	}
}

void UUC_Popup_SoulInfo::CloseWidget()
{
	RGAMEINSTANCE(this)->RWidgetManager->DetachUIPage(EUIPageEnum::UIPage_SpiritDetail);
}
