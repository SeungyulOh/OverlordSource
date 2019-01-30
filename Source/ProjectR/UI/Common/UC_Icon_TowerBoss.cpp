// Fill out your copyright notice in the Description page of Project Settings.

#include "ProjectR.h"
#include "UC_Icon_TowerBoss.h"
#include "UI/RWidgetManager.h"
#include "Public/Components/TextBlock.h"
#include "Public/Components/WidgetSwitcher.h"
#include "Public/Components/Image.h"
#include "Public/Components/ProgressBar.h"
#include "GlobalIntegrated.h"
#include "UtilFunctionIntegrated.h"
#include "GameInfo/UserInfo.h"
#include "UI/Common/Renewal_BaseHeroIcon.h"
#include "Runtime/Engine/Classes/Kismet/KismetMathLibrary.h"
#include "UI/Common/UC_Notify.h"



void UUC_Icon_TowerBoss::NativeConstruct()
{
	Super::NativeConstruct();
}

void UUC_Icon_TowerBoss::NativeDestruct()
{
	Super::NativeDestruct();
}

void UUC_Icon_TowerBoss::SetBossIconWithUD(FString InHeroUD, EHeroOwnershipType InOwnerType /*= EHeroOwnershipType::VE_None*/)
{
	if (InHeroUD.IsEmpty())
		return;
	UUIFunctionLibrary::GetHeroIconInfoWithUD(HeroIconInfo, InHeroUD, InOwnerType);
	SetBossIcon(HeroIconInfo);
}

void UUC_Icon_TowerBoss::SetHeroIconInfoCopy(FHERO_ICON_INFO InHeroIconInfo)
{
	SetBossIcon(InHeroIconInfo);
}

void UUC_Icon_TowerBoss::SetBossIcon(FHERO_ICON_INFO InHeroIconInfo)
{
	if (IsValidHeroIconInfo(InHeroIconInfo) == false)
	{
		this->SetVisibility(ESlateVisibility::Hidden);

		return;
	}

	HeroIconInfo = InHeroIconInfo;

	CharacterTableInfo = UTableManager::GetInstancePtr()->GetCharacterRow(HeroIconInfo.heroId);
	if (CharacterTableInfo == nullptr)
		return;

	ToolTipEvent = HeroIconInfo.ToolTipEventType;
	ToolTipID = HeroIconInfo.ToolTipID;
	SelectIconHero();
	SetImages();
	SetTexts();
	SetIsLockedPanel();
}

void UUC_Icon_TowerBoss::SetImages()
{
	SetHeroImage();
	SetHeroClassImage();
	SetHeroContinentImage();
}

void UUC_Icon_TowerBoss::SetHeroImage()
{
	DummyHero->SetVisibility(ESlateVisibility::Collapsed);

	UUIFunctionLibrary::SetImageBrush(HeroIconInfo.PortraitTextureRef, Image_Hero);
}

void UUC_Icon_TowerBoss::SetHeroClassImage()
{
	if (IsValid(Image_HeroClass) == false)
		return;

	if (nullptr == CharacterTableInfo)
		return;

	UPaperSprite* Sprite = UResourceFunctionLibrary::GetHeroClassTypeSprite(CharacterTableInfo->Class);
	UUIFunctionLibrary::SetImageBrush(Sprite, Image_HeroClass);
}

void UUC_Icon_TowerBoss::SetHeroContinentImage()
{
	if (!IsValid(Image_Continent))
		return;
	if (CharacterTableInfo == nullptr)
		return;

	UPaperSprite* Sprite = UResourceFunctionLibrary::GetHeroContinentTypeSprite(CharacterTableInfo->Continent);
	if (Sprite)
	{
		UUIFunctionLibrary::SetImageBrush(Sprite, Image_Continent);
	}
}

void UUC_Icon_TowerBoss::SetTexts()
{
	SetHeroLevel();
	SetBossFloor();
}

void UUC_Icon_TowerBoss::SetHeroLevel()
{
	if (Text_HeroLevel == nullptr)
		return;

	Text_HeroLevel->SetVisibility(HeroIconInfo.bSpirit ? ESlateVisibility::Collapsed : ESlateVisibility::HitTestInvisible);
	FString levelString = FString("LV. ") + FString::FromInt(HeroIconInfo.level);
	Text_HeroLevel->SetText(FText::FromString(levelString));
}

void UUC_Icon_TowerBoss::SetBossFloor()
{
	if (TextBlock_Floor == nullptr)
		return;

	TextBlock_Floor->SetVisibility(ESlateVisibility::HitTestInvisible);
	FString floorString = FString::FromInt(HeroIconInfo.enchantPoint) + FString("F");
	TextBlock_Floor->SetText(FText::FromString(floorString));
}

void UUC_Icon_TowerBoss::SetIsLockedPanel()
{
	if (IsValid(Panel_IsLocked))
		Panel_IsLocked->SetVisibility(HeroIconInfo.bLock ? ESlateVisibility::Visible : ESlateVisibility::Collapsed);
}

void UUC_Icon_TowerBoss::HideLockPanel(bool bHide)
{
	if (nullptr == Panel_IsLocked)
		return;

	if (bHide)
	{
		Panel_IsLocked->SetVisibility(ESlateVisibility::Hidden);
	}
	else
	{
		Panel_IsLocked->SetVisibility(ESlateVisibility::Visible);
	}
}

void UUC_Icon_TowerBoss::SelectIconHero()
{
	if (bIsSelectEnable == false)
		return;

	SelectBossIconBySwitcher();
}


void UUC_Icon_TowerBoss::SelectBossIconBySwitcher()
{
	if (WidgetSwitcher_Select == nullptr)
		return;

	switch (HeroIconInfo.eIconSelectType)
	{
	case EHeroIconSelectTypeEnum::eHeroSelect_None:
		WidgetSwitcher_Select->SetVisibility(ESlateVisibility::Collapsed);
		break;
	case EHeroIconSelectTypeEnum::eHeroSelect_Selected:
		WidgetSwitcher_Select->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
		WidgetSwitcher_Select->SetActiveWidgetIndex(0);
		break;
	case EHeroIconSelectTypeEnum::eHeroSelect_Using:
		WidgetSwitcher_Select->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
		WidgetSwitcher_Select->SetActiveWidgetIndex(1);
		break;
	default:
		WidgetSwitcher_Select->SetVisibility(ESlateVisibility::Collapsed);
		break;
	}
}

bool UUC_Icon_TowerBoss::IsValidHeroIconInfo(const FHERO_ICON_INFO& InHeroIconInfo)
{
	if (InHeroIconInfo.heroUD.IsEmpty() && InHeroIconInfo.heroId.IsNone())
	{
		return false;
	}

	return true;
}
