// Fill out your copyright notice in the Description page of Project Settings.

#include "ProjectR.h"
#include "GlobalIntegrated.h"


#include "GameInfo/RInventory.h"
#include "UtilFunctionIntegrated.h"
#include "UC_Hero_Skill_Icon.h"

#include "Character/Components/FunctionalComponents/SkillEntityComponent.h"

#include "CustomActors/CrewSpiritParticleActor.h"

#include "Skill_V2/SharedSkill/SkillSlotContainer.h"

void UUC_Hero_Skill_Icon::NativeConstruct()
{
	Super::NativeConstruct();
	if (IsValid(Button_Click))
	{
		Button_Click->OnClicked.Clear();
		Button_Click->OnClicked.AddDynamic(this, &UUC_Hero_Skill_Icon::OnClick_Hero_Skill_Icon);
	}
	
}

void UUC_Hero_Skill_Icon::NativeDestruct()
{
	if (IsValid(Button_Click))
		Button_Click->OnClicked.Clear();
	Super::NativeDestruct();
}

void UUC_Hero_Skill_Icon::SetSkillName(const FString& InHeroUD, FName InSkillID, uint32 InLevel /* = 1*/, ESkillCategoryTypeEnum IneSkillCategoryTypeEnum /* = ESkillCategoryTypeEnum::VE_None */)
{
	HeroUD = InHeroUD;
	SkillID = InSkillID;
	eSkillCategoryTypeEnum = IneSkillCategoryTypeEnum;
	FSkillTableInfo_V2* SkillTableInfo = UTableManager::GetInstancePtr()->GetSkillRow_V2(SkillID);
	checkf(SkillTableInfo != nullptr, TEXT("UUC_Hero_Skill_Icon::SetSkillName[ SkillTableInfo is Empty]"));
	FSkillResourceTableInfo* SkillResourceTableInfo = UTableManager::GetInstancePtr()->GetSkillResourceRow(SkillTableInfo->SkillResourceID);
	checkf(SkillResourceTableInfo != nullptr, TEXT("UUC_Hero_Skill_Icon::SetSkillName[ SkillResourceTableInfo is Empty]"));
	if (SkillResourceTableInfo)
		Image_SkillIcon->SetBrushFromTexture(UUtilFunctionLibrary::GetTexture2DFromAssetID(SkillResourceTableInfo->SkillIconRef));
	if (SkillTableInfo->SkillControlType == ESkillControlTypeEnum::VE_Active)
			SetControlType(true);
	else//passive, aura
			SetControlType(false);
	LocalizingTextBlock_Skill_Level->SetText(FText::AsNumber(InLevel));
}

void UUC_Hero_Skill_Icon::SetSkillInfo(const FString& InHeroUD, FHERO_SKILL InSkillInfo, uint32 InLevel /* = 1*/, ESkillCategoryTypeEnum IneSkillCategoryTypeEnum /* = ESkillCategoryTypeEnum::VE_None */)
{
	HeroUD = InHeroUD;
	SkillInfo = InSkillInfo;
	SkillID = FName(*InSkillInfo.skillId);
	eSkillCategoryTypeEnum = IneSkillCategoryTypeEnum;
	uint32 _level = (InLevel == 0) ? 1 : InLevel;
	FText MakedSkillID = FText::Format(FText::FromString(TEXT("{0}_{1}")), FText::FromString(InSkillInfo.skillId), FText::AsNumber(_level));

	FSkillTableInfo_V2* SkillTableInfo = UTableManager::GetInstancePtr()->GetSkillRow_V2(FName(*MakedSkillID.ToString()));
	checkf(SkillTableInfo != nullptr, TEXT("UUC_Hero_Skill_Icon::SetSkillInfo[ SkillTableInfo is Empty]"));
	FSkillResourceTableInfo* SkillResourceTableInfo = UTableManager::GetInstancePtr()->GetSkillResourceRow(SkillTableInfo->SkillResourceID);
	checkf(SkillResourceTableInfo != nullptr, TEXT("UUC_Hero_Skill_Icon::SetSkillInfo[ SkillTableInfo is Empty]"));
	if (SkillResourceTableInfo)
		Image_SkillIcon->SetBrushFromTexture(UUtilFunctionLibrary::GetTexture2DFromAssetID(SkillResourceTableInfo->SkillIconRef));
	if (SkillTableInfo->SkillControlType == ESkillControlTypeEnum::VE_Active)
			SetControlType(true);
	else//passive, aura
			SetControlType(false);
	LocalizingTextBlock_Skill_Level->SetText(FText::AsNumber(InLevel));
}

void UUC_Hero_Skill_Icon::SetSpiritSkill(FName InSpiritSkillName)
{
	FSkillTableInfo_V2* SpiritSkillTableInfo = UTableManager::GetInstancePtr()->GetSkillRow_V2(InSpiritSkillName);
	if (SpiritSkillTableInfo)
	{
		FSkillResourceTableInfo* SkillResourceTableInfo = UTableManager::GetInstancePtr()->GetSkillResourceRow(SpiritSkillTableInfo->SkillResourceID);
		if (SkillResourceTableInfo && Image_SkillIcon)
			Image_SkillIcon->SetBrushFromTexture(UUtilFunctionLibrary::GetTexture2DFromAssetID(SkillResourceTableInfo->SkillIconRef));
		SetControlType(false);
	}
}

void UUC_Hero_Skill_Icon::SetLock(bool bLock)
{
	Image_SkillLock->SetVisibility(bLock ? ESlateVisibility::HitTestInvisible : ESlateVisibility::Collapsed);
}

void UUC_Hero_Skill_Icon::SetControlType(bool isActive)
{
	if (ControlTypeSwitcher && LocalizingTextBlock_Skill_Passive)
	{
		if (isActive)
		{
			ControlTypeSwitcher->SetActiveWidgetIndex(0);
			LocalizingTextBlock_Skill_Passive->SetVisibility(ESlateVisibility::Collapsed);
		}
		else//passive, aura
		{
			ControlTypeSwitcher->SetActiveWidgetIndex(1);
			LocalizingTextBlock_Skill_Passive->SetVisibility(ESlateVisibility::Visible);
		}
	}
}

void UUC_Hero_Skill_Icon::SetDisable(bool bDisable)
{
	Super::SetDisable(bDisable);
	Image_SkillIcon->SetIsEnabled(bDisable);
}

void UUC_Hero_Skill_Icon::OnClick_Hero_Skill_Icon()
{
	RGAMEINSTANCE(this)->EventManager->OnClick_SkillDetail_HeroManagement.Broadcast((uint32)eSkillCategoryTypeEnum);
}

