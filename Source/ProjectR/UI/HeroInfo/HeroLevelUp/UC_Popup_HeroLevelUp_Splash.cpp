// Fill out your copyright notice in the Description page of Project Settings.

#include "ProjectR.h"

#include "Utils/SharedUtils/Renewal_BattleFunctionLibrary.h"
#include "Utils/UtilFunctionLibrary.h"

#include "Global/TableManager.h"
#include "GameInfo/RInventory.h"
#include "UI/RWidgetManager.h"

#include "UC_Popup_HeroLevelUp_Splash.h"

void UUC_Popup_HeroLevelUp_Splash::NativeConstruct()
{
	Super::NativeConstruct();
	Init();
}

void UUC_Popup_HeroLevelUp_Splash::NativeDestruct()
{
	Super::NativeDestruct();
}

void UUC_Popup_HeroLevelUp_Splash::Init()
{

}

void UUC_Popup_HeroLevelUp_Splash::SetSplashText(FText caption, FText desc)
{
	Text_Caption->SetText(caption);
	Text_Desc->SetText(desc);
}

void UUC_Popup_HeroLevelUp_Splash::SetHeroSetting(FHERO_ICON_INFO InPrevHero, FHERO_ICON_INFO InCurHero)
{
	if (IsValid(PrevIcon))
	{
		PrevIcon->SetDummyHero(false);
		InPrevHero.bSpirit = false;
		PrevIcon->SetHeroIconInfoCopy(InPrevHero);
	}
	if (IsValid(CurIcon))
	{
		PrevIcon->SetDummyHero(false);
		InCurHero.bSpirit = false;
		CurIcon->SetHeroIconInfoCopy(InCurHero);
	}

	FCharacterTableInfo*	UITableInfo = URenewal_BattleFunctionLibrary::GetCharacterTableInfo(ECharacterCategoryTypeEnum::VE_Hero, InPrevHero.heroId);

	if (IsValid(Text_PrevName))
	{
		FText HeroName = UUtilFunctionLibrary::GetLocalizedString(EStringTableType::VE_Hero, UITableInfo->DisplayName);
		Text_PrevName->SetText(HeroName);
	}
	if (IsValid(Text_CurName))
	{
		FText HeroName = UUtilFunctionLibrary::GetLocalizedString(EStringTableType::VE_Hero, UITableInfo->DisplayName);
		Text_CurName->SetText(HeroName);
	}
}

void UUC_Popup_HeroLevelUp_Splash::SetResult(FHERO Hero)
{
	URGameInstance* RGameInstance = RGAMEINSTANCE(this);

	FCharacterTableInfo* PrevTable = UTableManager::GetInstancePtr()->GetCharacterRow(Hero.heroId);
	FHERO* CurrData = RGameInstance->RInventory->GetHeroWithUD(Hero.heroUD);
	int32 CurrLv = CurrData->level;
	checkf(PrevTable != nullptr, TEXT("UUC_HeroInfo_Upgrade CharacterTable Is Empty"));

	if (IsValid(PrevIcon))
	{
		PrevIcon->SetHeroIconInfoWithID(Hero.heroId, EHeroOwnershipType::VE_None);
		PrevIcon->Text_HeroLevel->SetText(FText::AsNumber(CurrLv));
	}
	if (IsValid(CurIcon))
	{
		CurIcon->SetHeroIconInfoWithFHero(&Hero, PrevTable->UpgradeID);
		CurIcon->Text_HeroLevel->SetText(FText::AsNumber(CurrLv));
	}

	if (IsValid(Text_PrevName))
		Text_PrevName->SetText(UUtilFunctionLibrary::GetLocalizedString(EStringTableType::VE_Hero, PrevTable->DisplayName));
	if (IsValid(Text_CurName))
		Text_CurName->SetText(UUtilFunctionLibrary::GetLocalizedString(EStringTableType::VE_Hero, PrevTable->DisplayName));
}

FReply UUC_Popup_HeroLevelUp_Splash::NativeOnTouchEnded(const FGeometry& InGeometry, const FPointerEvent& InGestureEvent)
{
	URGameInstance*		RGameInstance = RGAMEINSTANCE(this);
	URWidgetManager*	RWidgetManager = RGameInstance->RWidgetManager;

	SetVisibility(ESlateVisibility::Collapsed);
	RWidgetManager->DetachUIPage(EUIPageEnum::UIPage_LevelUpCommonSplash);
	return Super::NativeOnTouchEnded(InGeometry, InGestureEvent);
}
