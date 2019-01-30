// Fill out your copyright notice in the Description page of Project Settings.

#include "ProjectR.h"
#include "Renewal_BaseHeroIcon.h"
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
#include "UI/HeroInfo/HeroManager/UP_HeroManager.h"



void URenewal_BaseHeroIcon::NativeConstruct()
{
	Super::NativeConstruct();

	if (IsValid(Notify))
	{
		Notify->NotifyState = ENotifyStateFlagEnum::BM_None;
		Notify->SetVisibleByNotify(ENotifyStateFlagEnum::BM_None, false);
	}

	ImgStars.Empty();
	FindChildWidgetsWithCertainName<UImage>(this, ImgStars, TEXT("ImgStar"));
}

void URenewal_BaseHeroIcon::NativeDestruct()
{
	//ProgressBar_RewardExp = nullptr;
	//ProgressBar_OwnedExp = nullptr;
	CharacterTableInfo = nullptr;

	Super::NativeDestruct();
}

void URenewal_BaseHeroIcon::SetOtherHeroIconInfo(const FEQUIPED_HERO& EquippedHero, const FHERO_ICON_INFO& InHeroIconInfo)
{
	auto RGameInstance = RGAMEINSTANCE(this);
	if (RGameInstance == nullptr)
	{
		return;
	}

	SetHeroIconInfo(InHeroIconInfo);
	
	if (false == EquippedHero.heroId.IsNone())
	{
		if (Panel_Power)
		{
			Panel_Power->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
		}

		//other hero attackpower resetting
		if (Text_AttackPower)
		{
			int32 power = UClient_BattleFunctionLibrary::GetEquipedHeroCombatStat(EquippedHero);
			Text_AttackPower->SetText(FText::AsNumber(power));
		}
	}
	else
	{
		if (Panel_Power)
		{
			Panel_Power->SetVisibility(ESlateVisibility::Collapsed);
		}
	}
}

void URenewal_BaseHeroIcon::SetHeroIconInfoCopy(FHERO_ICON_INFO InHeroIconInfo)
{
	SetHeroIconInfo(InHeroIconInfo);
}

void URenewal_BaseHeroIcon::SetHeroIconInfo(FHERO_ICON_INFO InHeroIconInfo)
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
	HeroIconInfo.bSpirit = !CharacterTableInfo->IsHero;
	HeroIconInfo.grade = CharacterTableInfo->Grade;

	SetImages();
	SetSwitchers();
	SetTexts();
	SetProgressBars();
	SetPanels();
	SetNotify();
	SetIsLockedPanel();
	SetUsedHeroIcon();
	SetHeroEnchantPoint();

	this->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
}

void URenewal_BaseHeroIcon::SetHeroIconInfoWithUD(FString InHeroUD, EHeroOwnershipType InOwnerType)
{
	if (InHeroUD.IsEmpty())
		return;
	UUIFunctionLibrary::GetHeroIconInfoWithUD(HeroIconInfo, InHeroUD, InOwnerType);
	SetHeroIconInfo(HeroIconInfo);
}

void URenewal_BaseHeroIcon::SetHeroIconInfoWithResult(FHeroSlotInfo InHeroSlot)
{
	UUIFunctionLibrary::GetHeroIconInfoWithUD(HeroIconInfo, InHeroSlot.HeroUD, EHeroOwnershipType::VE_Me);

	HeroIconInfo.exp = InHeroSlot.HeroExp;

	if (IsValidHeroIconInfo(HeroIconInfo) == false)
	{
		SetVisibility(ESlateVisibility::Hidden);
		return;
	}

	CharacterTableInfo = UTableManager::GetInstancePtr()->GetCharacterRow(HeroIconInfo.heroId);
	if (CharacterTableInfo == nullptr)
		return;

	ToolTipEvent = HeroIconInfo.ToolTipEventType;
	ToolTipID = HeroIconInfo.ToolTipID;
	HeroIconInfo.bSpirit = !CharacterTableInfo->IsHero;

	SetImages();
	SetSwitchers();
	SetTexts();
	SetNotify();
	SetIsLockedPanel();
	if (!HeroIconInfo.bSpirit)
		SetHeroEnchantPoint();

	SetUMGHeroExp();
	SetOwnedExp();

	SetVisibility(ESlateVisibility::SelfHitTestInvisible);
}

void URenewal_BaseHeroIcon::SetResultProgressBar(float InPercent)
{
	if (ProgressBar_OwnedExp == nullptr)
		return;

	ProgressBar_OwnedExp->SetPercent(InPercent);
}

void URenewal_BaseHeroIcon::SetHeroIconInfoWithID(FName InHeroID, EHeroOwnershipType InOwnerType)
{
	if (InHeroID.IsValid())
	{
		UUIFunctionLibrary::GetHeroIconInfoWithID(HeroIconInfo, InHeroID, InOwnerType);
		SetHeroIconInfo(HeroIconInfo);
	}
}

void URenewal_BaseHeroIcon::SetHeroIconInfoWithFHero(FHERO* PrevHero, FName InNextID)
{
	UUIFunctionLibrary::GetHeroIconInfoWithID(HeroIconInfo, InNextID, EHeroOwnershipType::VE_None);
	HeroIconInfo.level = PrevHero->level;
	SetHeroIconInfo(HeroIconInfo);
}

void URenewal_BaseHeroIcon::SetHeroIconInfoWithEquipedHero(FEQUIPED_HERO InEquipedHero)
{
	UUIFunctionLibrary::GetHeroIconInfoByEquipedHero(HeroIconInfo, InEquipedHero);
	SetHeroIconInfo(HeroIconInfo);
}

void URenewal_BaseHeroIcon::SetHeroIconInfoForInven(FHERO_ICON_INFO InHeroIconInfo)
{
	SetHeroIconInfo(InHeroIconInfo);

	SetHeroEnchantPoint();
	SetIsLockedPanel();
}

void URenewal_BaseHeroIcon::SetSwitcherUseMark()
{
	if (WidgetSwitcher_UseMark)
	{
		if (HeroIconInfo.bIsUsed)
		{
			WidgetSwitcher_UseMark->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
			WidgetSwitcher_UseMark->SetActiveWidgetIndex(1);
		}
		else
		{
			WidgetSwitcher_UseMark->SetVisibility(ESlateVisibility::Hidden);
		}
	}
}

void URenewal_BaseHeroIcon::ShowOnlyHeroImage(bool bOnly)
{
	ESlateVisibility PanelVisibility = ESlateVisibility::SelfHitTestInvisible;
	if (bOnly)
	{
		PanelVisibility = ESlateVisibility::Hidden;
	}

	if (Panel_JobLevel)
	{
		Panel_JobLevel->SetVisibility(PanelVisibility);
	}

	if (Panel_Power)
	{
		Panel_Power->SetVisibility(PanelVisibility);
	}
}

void URenewal_BaseHeroIcon::HidePowerPanel(bool bHide)
{
	if (nullptr == Panel_Power)
		return;

	if (bHide)
	{
		Panel_Power->SetVisibility(ESlateVisibility::Hidden);
	}
	else
	{
		if (HeroIconInfo.bIsOwned)
		{
			if (false == HeroIconInfo.heroUD.IsEmpty())
			{
				URGameInstance* RGameInstance = RGAMEINSTANCE(this);
				
				{
					Panel_Power->SetVisibility(ESlateVisibility::SelfHitTestInvisible);

					int32 power = UClient_BattleFunctionLibrary::GetHeroCombatStat(HeroIconInfo.heroUD);
					Text_AttackPower->SetText(FText::AsNumber(power));
				}
			}
		}
	}
}

void URenewal_BaseHeroIcon::HideLockPanel(bool bHide)
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

void URenewal_BaseHeroIcon::HideUI()
{
	Icon_Lock->SetVisibility(ESlateVisibility::Collapsed);
	WidgetSwitcher_Star->SetVisibility(ESlateVisibility::Collapsed);
	Text_HeroLevel->SetVisibility(ESlateVisibility::Collapsed);
	VerticalBox_Panel->SetVisibility(ESlateVisibility::Collapsed);
}

void URenewal_BaseHeroIcon::HideIcon()
{
	HeroIconInfo.heroUD = TEXT("");
	HeroIconInfo.heroId = NAME_None;
	SetHeroIconInfo(HeroIconInfo);
}

void URenewal_BaseHeroIcon::SetUsedHeroIcon()
{
// 	HeroIconInfo.bIsUsed = use;

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

// 	if (HeroIconInfo.eIconSelectType)
// 	{
// 		WidgetSwitcher_Select->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
// 		WidgetSwitcher_Select->SetActiveWidgetIndex(1);
// 	}
// 	else
// 	{
// 		WidgetSwitcher_Select->SetVisibility(ESlateVisibility::Collapsed);
// 	}
}

/*
Set Image Componnents
*/
void URenewal_BaseHeroIcon::SetImages()
{
	SetHeroImage();
	SetHeroClassImage();
	SetHeroContinentImage();
	SetHeroGradeImage();
}

void URenewal_BaseHeroIcon::SetHeroImage()
{
	if (IsValid(Image_Hero) == false)
		return;

	SetDummyHero(false);

	UUIFunctionLibrary::SetImageBrush(HeroIconInfo.PortraitTextureRef, Image_Hero);	
}

void URenewal_BaseHeroIcon::SetHeroClassImage()
{
	if (IsValid(Image_HeroClass) == false)
		return;

	if (nullptr == CharacterTableInfo)
		return;

	/*UTexture2D* tex2d = UResourceFunctionLibrary::GetHeroClassTypeTexture(CharacterTableInfo->Class);
	if (IsValid(tex2d))
	{
		Image_HeroClass->SetBrushFromTexture(tex2d);
	}
	else*/
	{
		UPaperSprite* Sprite = UResourceFunctionLibrary::GetHeroClassTypeSprite(CharacterTableInfo->Class);
		UUIFunctionLibrary::SetImageBrush(Sprite, Image_HeroClass);
	}
}

void URenewal_BaseHeroIcon::SetHeroContinentImage()
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

void URenewal_BaseHeroIcon::SetHeroGradeImage()
{
	for (UImage* ImgStar : ImgStars)
		ImgStar->SetVisibility(ESlateVisibility::Collapsed);

	checkf(RGAMEINSTANCE(this)->RWidgetManager, TEXT("RWidgetManager null"));
	TArray<UPaperSprite*> BGArray = URAsyncLoader::GetInstancePtr()->PortraitBGArray;

	FLinearColor color;
	if (HeroIconInfo.grade >= 1 && HeroIconInfo.grade < 4)
	{
		color = FLinearColor::Black;
		BG->Brush.SetResourceObject(BGArray[0]);
	}
	else if (HeroIconInfo.grade >= 4 && HeroIconInfo.grade < 6)
	{
		color = FColor(67, 186, 35);
		BG->Brush.SetResourceObject(BGArray[1]);
	}
	else if (HeroIconInfo.grade >= 6 && HeroIconInfo.grade < 8)
	{
		color = FColor(38, 174, 238);
		BG->Brush.SetResourceObject(BGArray[2]);
	}
	else if (HeroIconInfo.grade >= 8 && HeroIconInfo.grade < 10)
	{
		color = FColor(216, 158, 67);
		BG->Brush.SetResourceObject(BGArray[3]);
	}
	else
		BG->Brush.SetResourceObject(BGArray[4]);
	
	if (ImageBack && HeroIconInfo.grade < 10)
		ImageBack->Brush.TintColor = FSlateColor(color);

	for (int32 i = 0; i < HeroIconInfo.grade; i++)
		if (ImgStars.IsValidIndex(i))
			ImgStars[i]->SetVisibility(ESlateVisibility::SelfHitTestInvisible);

	//Grade_10->SetVisibility(ESlateVisibility::Collapsed);
	if (HeroIconInfo.grade <= 5)
		WidgetSwitcher_Star->SetActiveWidgetIndex(0);
	else if (HeroIconInfo.grade < 10)
		WidgetSwitcher_Star->SetActiveWidgetIndex(1);
	else
	{
		WidgetSwitcher_Star->SetActiveWidgetIndex(2);
		//Grade_10->SetVisibility(ESlateVisibility::HitTestInvisible);
		ImageBack->SetVisibility(ESlateVisibility::Collapsed);
	}
}

void URenewal_BaseHeroIcon::SetSwitchers()
{
	SetOwnStatus();
	SetUnOwnedStatus();
	SetOwnedStatus();
}

void URenewal_BaseHeroIcon::SetOwnStatus()
{
	if (WidgetSwitcher_OwnStatus == nullptr)
		return;

	if (HeroIconInfo.bIsOwned == false)
	{
		WidgetSwitcher_OwnStatus->SetActiveWidgetIndex(0);
	}
	else
	{
		WidgetSwitcher_OwnStatus->SetActiveWidgetIndex(1);
	}
}

void URenewal_BaseHeroIcon::SetUnOwnedStatus()
{
	if (WidgetSwitcher_UnOwned == nullptr)
		return;

	auto RGameInstance = RGAMEINSTANCE(this);
	if (IsValid(RGameInstance->UserInfo) == false)
		return;

}

void URenewal_BaseHeroIcon::SetOwnedStatus()
{
	if (WidgetSwitcher_Owned == nullptr)
		return;
}

void URenewal_BaseHeroIcon::SetTexts()
{
	SetHeroName();
	SetHeroLevel();
	SetAttackPower();
}

void URenewal_BaseHeroIcon::SetHeroName()
{
	if (Text_HeroName == nullptr)
		return;

	Text_HeroName->SetText(UUtilFunctionLibrary::GetLocalizedString(EStringTableType::VE_Hero, CharacterTableInfo->DisplayName));
}

void URenewal_BaseHeroIcon::SetHeroLevel()
{
	if (Text_HeroLevel == nullptr)
		return;

	Text_HeroLevel->SetVisibility(HeroIconInfo.bSpirit ? ESlateVisibility::Collapsed : ESlateVisibility::HitTestInvisible);
	Text_HeroLevel->SetText(FText::AsNumber(HeroIconInfo.level));
}

void URenewal_BaseHeroIcon::SetAttackPower()
{
	if (Text_AttackPower == nullptr)
		return;

	if (HeroIconInfo.bIsOwned)
	{
		if (false == HeroIconInfo.heroUD.IsEmpty())
		{
			if (Panel_Power)
			{
				Panel_Power->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
			}

			URGameInstance* RGameInstance = RGAMEINSTANCE(this);
			
			{
				int32 power = UClient_BattleFunctionLibrary::GetHeroCombatStat(HeroIconInfo.heroUD);
				Text_AttackPower->SetText(FText::AsNumber(power));
			}
		}
	}
	else
	{
		if (Panel_Power)
		{
			Panel_Power->SetVisibility(ESlateVisibility::Collapsed);
		}
	}
}

void URenewal_BaseHeroIcon::SetHeroEnchantPoint()
{
	if (Text_HeroLevel == nullptr)
		return;

	TextBlock_Enchant->SetVisibility(HeroIconInfo.bSpirit ? ESlateVisibility::Collapsed : ESlateVisibility::HitTestInvisible);
	TextBlock_Enchant->SetText(FText::AsNumber(HeroIconInfo.enchantPoint));
}

void URenewal_BaseHeroIcon::SetProgressBars()
{
	SetRewardExp();
	SetOwnedExp();
	SetHeroExp();
}

void URenewal_BaseHeroIcon::SetRewardExp()
{
	if (ProgressBar_RewardExp == nullptr)
		return;

	float RewardExpPercent = static_cast<float>(HeroIconInfo.acquiredExp) / static_cast<float>(HeroIconInfo.MaxHP);
	ProgressBar_RewardExp->SetPercent(RewardExpPercent);
}

void URenewal_BaseHeroIcon::SetOwnedExp()
{
	if (ProgressBar_OwnedExp == nullptr)
		return;

	FCharacterCostGrowth* m_CostGrowthInfo = UTableManager::GetInstancePtr()->GetCharacterCostGrowthRow(FName(*FText::AsNumber(HeroIconInfo.level).ToString()));

	float OwnedExpPercent = static_cast<float>(HeroIconInfo.exp) / static_cast<float>(m_CostGrowthInfo->Cost_HeroRune);
	ProgressBar_OwnedExp->SetPercent(OwnedExpPercent);
}

void URenewal_BaseHeroIcon::SetHeroExp()
{
	if (Text_Exp)
	{
		Text_Exp->SetText(FText::AsNumber(HeroIconInfo.exp));
	}
}

void URenewal_BaseHeroIcon::SetPanels()
{
	SetHp();
	SetDeath();
}

void URenewal_BaseHeroIcon::SetNotify()
{
	if (IsValid(Notify))
	{
		Notify->SetVisibleByNotify(ENotifyStateFlagEnum::BM_None, HeroIconInfo.IsNotify);
	}
}

void URenewal_BaseHeroIcon::SetHp()
{
	if (IsValid(HeroHpPanel))
	{
		if (HeroIconInfo.bIsHp)
		{
			HeroHpPanel->SetVisibility(ESlateVisibility::SelfHitTestInvisible);

			if (IsValid(HpProgressBar))
			{
				HpProgressBar->SetPercent(HeroIconInfo.HpRate);
			}

			if (IsValid(HpPercentText))
			{
				HpPercentText->SetText(FText::AsPercent(HeroIconInfo.HpRate));
			}
		}
		else
		{
			HeroHpPanel->SetVisibility(ESlateVisibility::Collapsed);
		}
	}
}

void URenewal_BaseHeroIcon::SetDeath()
{
	Grade_10->SetVisibility(ESlateVisibility::Collapsed);
	Grade_soul->SetVisibility(ESlateVisibility::Collapsed);

	if (IsValid(HeroDeathPanel))
	{
		if (HeroIconInfo.bIsDead)
		{
			HeroDeathPanel->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
			ImageBack->SetVisibility(ESlateVisibility::Collapsed);
		}
		else
		{
			HeroDeathPanel->SetVisibility(ESlateVisibility::Collapsed);
			ImageBack->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
		
			FCharacterTableInfo*	UITableInfo = URenewal_BattleFunctionLibrary::GetCharacterTableInfo(ECharacterCategoryTypeEnum::VE_Hero, HeroIconInfo.heroId);
			if (UITableInfo)
			{
				if (UITableInfo->IsHero)
					Grade_10->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
				else
					Grade_soul->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
			}

		}
	}
}

void URenewal_BaseHeroIcon::SetHeroIcon(FString heroID , int32 Level)
{
	FName Id = FName(*heroID);
	FCharacterTableInfo* Info = UTableManager::GetInstancePtr()->GetCharacterRow(Id);
	if (!Info)
	{
		SetVisibility(ESlateVisibility::Collapsed);
		return;
	}
		

	if (HBox_AttackPower)
		HBox_AttackPower->SetVisibility(ESlateVisibility::Collapsed);
	if (Text_HeroLevel)
	{
		Text_HeroLevel->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
		Text_HeroLevel->SetText(FText::AsNumber(Level));
	}

	if (IsValid(Image_Hero) == false)
		return;	
	UUIFunctionLibrary::SetImageBrush(UUIFunctionLibrary::GetIconImageFromTable(Info->PortraitTextureKey), Image_Hero);

	if (IsValid(Image_HeroClass) == false)
		return;

	UPaperSprite* Sprite = UResourceFunctionLibrary::GetHeroClassTypeSprite(Info->Class);
	UUIFunctionLibrary::SetImageBrush(Sprite, Image_HeroClass);

	Sprite = UResourceFunctionLibrary::GetHeroContinentTypeSprite(Info->Continent);
	if (Sprite)
		UUIFunctionLibrary::SetImageBrush(Sprite, Image_Continent);


	for (UImage* ImgStar : ImgStars)
		ImgStar->SetVisibility(ESlateVisibility::Collapsed);

	checkf(RGAMEINSTANCE(this)->RWidgetManager, TEXT("RWidgetManager null"));
	TArray<UPaperSprite*> BGArray = URAsyncLoader::GetInstancePtr()->PortraitBGArray;

	FLinearColor color;
	if (Info->Grade >= 1 && Info->Grade < 4)
	{
		color = FLinearColor::Black;
		BG->Brush.SetResourceObject(BGArray[0]);
	}
	else if (Info->Grade >= 4 && Info->Grade < 6)
	{
		color = FColor(67, 186, 35);
		BG->Brush.SetResourceObject(BGArray[1]);
	}
	else if (Info->Grade >= 6 && Info->Grade< 8)
	{
		color = FColor(38, 174, 238);
		BG->Brush.SetResourceObject(BGArray[2]);
	}
	else if (Info->Grade >= 8 && Info->Grade< 10)
	{
		color = FColor(216, 158, 67);
		BG->Brush.SetResourceObject(BGArray[3]);
	}
	else
		BG->Brush.SetResourceObject(BGArray[4]);

	if (ImageBack && Info->Grade < 10)
		ImageBack->Brush.TintColor = FSlateColor(color);

	for (int32 i = 0; i < Info->Grade; i++)
		if (ImgStars.IsValidIndex(i))
			ImgStars[i]->SetVisibility(ESlateVisibility::SelfHitTestInvisible);

	if (Info->Grade <= 5)
		WidgetSwitcher_Star->SetActiveWidgetIndex(0);
	else if (Info->Grade < 10)
		WidgetSwitcher_Star->SetActiveWidgetIndex(1);
	else
	{
		WidgetSwitcher_Star->SetActiveWidgetIndex(2);
		ImageBack->SetVisibility(ESlateVisibility::Collapsed);
	}

	SetVisibility(ESlateVisibility::SelfHitTestInvisible);
}

void URenewal_BaseHeroIcon::SetUMGHeroExp()
{
	if(IsValid(WidgetSwitcher_Star))
		WidgetSwitcher_Star->SetVisibility(ESlateVisibility::Collapsed);
	if(IsValid(ProgressBar_OwnedExp))
		ProgressBar_OwnedExp->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
	if (IsValid(Panel_Power))
		Panel_Power->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
	if(IsValid(HBox_AttackPower))
		HBox_AttackPower->SetVisibility(ESlateVisibility::Collapsed);
}

void URenewal_BaseHeroIcon::SetIsLockedPanel()
{
	if(IsValid(Icon_Lock))
		Icon_Lock->SetVisibility(HeroIconInfo.bLock ? ESlateVisibility::Visible : ESlateVisibility::Collapsed);
}

void URenewal_BaseHeroIcon::SetDummyHero(bool bDummy)
{
	DummyHero->SetVisibility(bDummy ? ESlateVisibility::SelfHitTestInvisible : ESlateVisibility::Collapsed);
}

bool URenewal_BaseHeroIcon::IsValidHeroIconInfo(const FHERO_ICON_INFO& InHeroIconInfo)
{
	if (InHeroIconInfo.heroUD.IsEmpty() && InHeroIconInfo.heroId.IsNone())
	{
		return false;
	}

	return true;
}

void URenewal_BaseHeroIcon::SelectIconHero()
{
	if (bIsSelectEnable == false)
		return;

	SelectHeroIconBySwitcher();
}

void URenewal_BaseHeroIcon::SelectIconHero(bool bSelected)
{
	if (bSelected)
	{
		WidgetSwitcher_Select->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
		WidgetSwitcher_Select->SetActiveWidgetIndex(0);
	}
	else
	{
		WidgetSwitcher_Select->SetVisibility(ESlateVisibility::Collapsed);
	}
}

void URenewal_BaseHeroIcon::SelectHeroIconBySwitcher()
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


void URenewal_BaseHeroIcon::SetIsAbled(bool bIsAbled)
{
	if (WidgetSwitcher_Owned == nullptr)
		return;

	if (!bIsAbled)
	{
		WidgetSwitcher_Owned->SetActiveWidgetIndex(3);
	}
	else
	{
		WidgetSwitcher_Owned->SetActiveWidgetIndex(0);
	}
}


///////////////////////////////////////////////////////////////////////////////////////////

void UUpgrade_BaseHeroIcon::NativeConstruct()
{
	Super::NativeConstruct();

}

void UUpgrade_BaseHeroIcon::NativeDestruct()
{
	CharacterTableInfo = nullptr;

	Super::NativeDestruct();
}

void UUpgrade_BaseHeroIcon::SetHeroIconInfo(FHERO_ICON_INFO InHeroIconInfo)
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
	HeroIconInfo.bSpirit = !CharacterTableInfo->IsHero;

	SetImages();

	this->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
}

void UUpgrade_BaseHeroIcon::SetHeroIconInfoWithID(FName InHeroID, int32 InMaxCount, bool IsSprit)
{
	if (InHeroID.IsValid())
	{
		UUIFunctionLibrary::GetHeroIconInfoWithID(HeroIconInfo, InHeroID, EHeroOwnershipType::VE_None);
		SetHeroIconInfo(HeroIconInfo);

		if (IsValid(WidgetSwitcher_Potrait))
			WidgetSwitcher_Potrait->SetActiveWidgetIndex(0);

		SetCount(0, InMaxCount);

		if (IsValid(HeroMaxCount))
		{
			HeroMaxCount->SetText(FText::AsNumber(InMaxCount));
			if (InMaxCount == 0)
				HeroMaxCount->SetVisibility(ESlateVisibility::Collapsed);
			else
				HeroMaxCount->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
		}
			

		if (IsSprit)
		{
			if(IsValid(Grade_10))
				Grade_10->SetVisibility(ESlateVisibility::Collapsed);
		}
		else
		{
			if (IsValid(Grade_10))
				Grade_10->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
		}
	}
}

void UUpgrade_BaseHeroIcon::SetClassIcon(EUpgradeCondition InHeroType, EClassTypeEnum InClassType, int32 InGrade, int32 InMaxCount, bool IsSprit)
{
	if(IsValid(WidgetSwitcher_Potrait))
		WidgetSwitcher_Potrait->SetActiveWidgetIndex(1);

	UPaperSprite* Sprite = UResourceFunctionLibrary::GetHeroClassTypeSprite(InClassType);

	if(IsValid(Image_ClassPortrait))
		UUIFunctionLibrary::SetImageBrush(Sprite, Image_ClassPortrait);

	SetCount(0, InMaxCount);

	if (IsValid(ClassMaxCount))
		ClassMaxCount->SetText(FText::AsNumber(InMaxCount));

	/*for (UImage* ImgStar : ClassStars)
		ImgStar->SetVisibility(ESlateVisibility::Collapsed);

	for (int32 i = 0; i < InGrade; i++)
		if (ClassStars.IsValidIndex(i))
			ClassStars[i]->SetVisibility(ESlateVisibility::SelfHitTestInvisible);*/

	if(IsValid(StarWidget))
		StarWidget->SetStar(InGrade);

	/*if (IsSprit)
	{
		if (IsValid(Grade_10))
			Grade_10->SetVisibility(ESlateVisibility::Collapsed);
	}
	else
	{
		if (IsValid(Grade_10))
			Grade_10->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
	}*/
}

void UUpgrade_BaseHeroIcon::SetAllIcon(EUpgradeCondition InHeroType, int32 InGrade, int32 InMaxCount, bool IsSprit)
{
	if (IsValid(WidgetSwitcher_Potrait))
		WidgetSwitcher_Potrait->SetActiveWidgetIndex(2);

	SetCount(0, InMaxCount);

	if (IsValid(AllMaxCount))
		AllMaxCount->SetText(FText::AsNumber(InMaxCount));

	if (IsSprit)
	{
		if (IsValid(Grade_10))
			Grade_10->SetVisibility(ESlateVisibility::Collapsed);
		if(IsValid(Grade_soul))
			Grade_soul->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
	}
	else
	{
		if (IsValid(Grade_10))
			Grade_10->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
		if (IsValid(Grade_soul))
			Grade_soul->SetVisibility(ESlateVisibility::Collapsed);
	}
}

void UUpgrade_BaseHeroIcon::SetCount(int32 InCount, int32 InMax)
{
	if (IsValid(WidgetSwitcher_Potrait))
	{
		bool		IsRed = false;
		if (InCount < InMax)
			IsRed = true;

		switch (WidgetSwitcher_Potrait->GetActiveWidgetIndex())
		{
			case 0:
			{
				if (IsValid(HeroMinCount))
				{
					HeroMinCount->SetText(FText::AsNumber(InCount));
					if(IsRed)
						HeroMinCount->SetColorAndOpacity(FSlateColor(FLinearColor(1.f, 0.04f, 0.04f, 1.f)));
					else
						HeroMinCount->SetColorAndOpacity(FSlateColor(FLinearColor(1.0f, 1.0f, 1.0f, 1.0f)));
				}
				break;
			}
			case 1:
			{
				if (IsValid(ClassMinCount))
				{
					ClassMinCount->SetText(FText::AsNumber(InCount));
					if (IsRed)
						ClassMinCount->SetColorAndOpacity(FSlateColor(FLinearColor(1.f, 0.04f, 0.04f, 1.f)));
					else
						ClassMinCount->SetColorAndOpacity(FSlateColor(FLinearColor(1.0f, 1.0f, 1.0f, 1.0f)));
				}

				break;
			}
			case 2:
			{
				if (IsValid(AllMinCount))
				{
					AllMinCount->SetText(FText::AsNumber(InCount));
					if (IsRed)
						AllMinCount->SetColorAndOpacity(FSlateColor(FLinearColor(1.f, 0.04f, 0.04f, 1.f)));
					else
						AllMinCount->SetColorAndOpacity(FSlateColor(FLinearColor(1.0f, 1.0f, 1.0f, 1.0f)));
				}
				break;
			}
		}
	}
}

void UUpgrade_BaseHeroIcon::HideUI()
{
	VerticalBox_Panel->SetVisibility(ESlateVisibility::Collapsed);
}

void UUpgrade_BaseHeroIcon::HideIcon()
{
	HeroIconInfo.heroUD = TEXT("");
	HeroIconInfo.heroId = NAME_None;
	SetHeroIconInfo(HeroIconInfo);
}

void UUpgrade_BaseHeroIcon::SetUsedHeroIcon(bool use)
{
	HeroIconInfo.bIsUsed = use;

	if (WidgetSwitcher_Select == nullptr)
		return;

	if (HeroIconInfo.bIsUsed)
	{
		WidgetSwitcher_Select->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
		WidgetSwitcher_Select->SetActiveWidgetIndex(1);
	}
	else
	{
		WidgetSwitcher_Select->SetVisibility(ESlateVisibility::Collapsed);
	}
}

/*
Set Image Componnents
*/
void UUpgrade_BaseHeroIcon::SetImages()
{
	SetHeroImage();
	SetHeroClassImage();
	SetHeroContinentImage();
}

void UUpgrade_BaseHeroIcon::SetHeroImage()
{
	if (IsValid(Image_Hero) == false)
		return;

	UUIFunctionLibrary::SetImageBrush(HeroIconInfo.PortraitTextureRef, Image_Hero);
}

void UUpgrade_BaseHeroIcon::SetHeroClassImage()
{
	if (IsValid(Image_HeroClass) == false)
		return;

	if (nullptr == CharacterTableInfo)
		return;

	UPaperSprite* Sprite = UResourceFunctionLibrary::GetHeroClassTypeSprite(CharacterTableInfo->Class);
	UUIFunctionLibrary::SetImageBrush(Sprite, Image_HeroClass);
}

void UUpgrade_BaseHeroIcon::SetHeroContinentImage()
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

bool UUpgrade_BaseHeroIcon::IsValidHeroIconInfo(const FHERO_ICON_INFO& InHeroIconInfo)
{
	if (InHeroIconInfo.heroUD.IsEmpty() && InHeroIconInfo.heroId.IsNone())
	{
		return false;
	}

	return true;
}

void UUpgrade_BaseHeroIcon::SelectIconHero()
{
	SelectHeroIconBySwitcher();
}

void UUpgrade_BaseHeroIcon::SelectHeroIconBySwitcher()
{
	if (IsValid(Image_Seleting))
	{
		Image_Seleting->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
		Image_Seleting->SetRenderOpacity(1.f);
	}

	if (IsValid(Image_Selected))
	{
		Image_Selected->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
	}
}