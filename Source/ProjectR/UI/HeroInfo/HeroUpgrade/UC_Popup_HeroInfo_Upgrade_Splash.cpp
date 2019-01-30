// Fill out your copyright notice in the Description page of Project Settings.

#include "ProjectR.h"

#include "Global/RGameInstance.h"
#include "Global/TableManager.h"

#include "Utils/UtilFunctionLibrary.h"

#include "UC_Popup_HeroInfo_Upgrade_Splash.h"

void UUC_Popup_HeroInfo_Upgrade_Splash::NativeConstruct()
{
	Super::NativeConstruct();
}

void UUC_Popup_HeroInfo_Upgrade_Splash::NativeDestruct()
{
	Super::NativeDestruct();
}

void UUC_Popup_HeroInfo_Upgrade_Splash::SetHero(FHERO InHero)
{
	Hero = InHero;
}

void UUC_Popup_HeroInfo_Upgrade_Splash::SetResult(FHERO InHero)
{
	URGameInstance* RGameInstance = RGAMEINSTANCE(this);

	FCharacterTableInfo* PrevTable = UTableManager::GetInstancePtr()->GetCharacterRow(InHero.heroId);
	checkf(PrevTable != nullptr, TEXT("UUC_HeroInfo_Upgrade CharacterTable Is Empty"));

	if (IsValid(PrevIcon))
		PrevIcon->SetHeroIconInfoWithID(InHero.heroId, EHeroOwnershipType::VE_None);
	if (IsValid(NextIcon))
		NextIcon->SetHeroIconInfoWithFHero(&InHero, PrevTable->UpgradeID);

	if (IsValid(Text_PrevName))
		Text_PrevName->SetText(UUtilFunctionLibrary::GetLocalizedString(EStringTableType::VE_Hero, PrevTable->DisplayName));
	if (IsValid(Text_NextName))
		Text_NextName->SetText(UUtilFunctionLibrary::GetLocalizedString(EStringTableType::VE_Hero, PrevTable->DisplayName));
}

FReply UUC_Popup_HeroInfo_Upgrade_Splash::NativeOnTouchEnded(const FGeometry& InGeometry, const FPointerEvent& InGestureEvent)
{
	SetVisibility(ESlateVisibility::Collapsed);
	return FReply::Unhandled();
}