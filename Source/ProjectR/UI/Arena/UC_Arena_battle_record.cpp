// Fill out your copyright notice in the Description page of Project Settings.

#include "ProjectR.h"
#include "UC_Arena_battle_record.h"



#include "UtilFunctionIntegrated.h"
#include "UI/Common/Renewal_BaseHeroIcon.h"

void UUC_Arena_Battle_Record_Item::InvalidateData()
{
	//Super::InvalidateData();
	UUC_Arena_Battle_Record_Data* battleRecord = Cast<UUC_Arena_Battle_Record_Data>(ItemData);
	if (battleRecord)
	{
		SetHistory(battleRecord->Defence, battleRecord->Result, battleRecord->Point, battleRecord->Level, battleRecord->Name, battleRecord->Guild, battleRecord->Power, battleRecord->HeroIDs);
	}
}

void UUC_Arena_Battle_Record_Item::SetHistory(bool Defence, bool Result, int32 Point, int32 Level, FText Name, FText Guild, int32 Power, TArray<FName> HeroIDs)
{
	int32		iDefence		=	0;
	int32		iResult			=	1;
	int32		iResultImage	=	0;

	if (Defence == true)
		iDefence = 1;
	if (Result == true)
	{
		iResult			=	0;
		iResultImage	=	1;
	}
	if (Point == 0)
	{
		if(IsValid(WidgetSwitcher_APointImage))
			WidgetSwitcher_APointImage->SetVisibility(ESlateVisibility::Hidden);
		if (IsValid(WidgetSwitcher_DPointImage))
			WidgetSwitcher_DPointImage->SetVisibility(ESlateVisibility::Hidden);
	}
	else
	{
		if (IsValid(WidgetSwitcher_APointImage))
			WidgetSwitcher_APointImage->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
		if (IsValid(WidgetSwitcher_DPointImage))
			WidgetSwitcher_DPointImage->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
	}
	
	if(IsValid(WidgetSwitcher_9))
		WidgetSwitcher_9->SetActiveWidgetIndex(iDefence);
	if (Defence == false)
	{
		if (IsValid(WidgetSwitcher_AResult))
			WidgetSwitcher_AResult->SetActiveWidgetIndex(iResult);
		if (IsValid(WidgetSwitcher_APointImage))
			WidgetSwitcher_APointImage->SetActiveWidgetIndex(iResultImage);
		if (IsValid(TextBlock_APoint))
			TextBlock_APoint->SetText(FText::AsNumber(Point));
		if (IsValid(TextBlock_ALevel))
			TextBlock_ALevel->SetText(FText::Format(FText::FromString("Lv.{0}"), Level));
		if (IsValid(TextBlock_AName))
			TextBlock_AName->SetText(Name);
		if (IsValid(TextBlock_AGuild))
			TextBlock_AGuild->SetText(Guild);
		if (IsValid(TextBlock_APower))
			TextBlock_APower->SetText(FText::AsNumber(Power));
		AttackHeros.Empty();

		FindChildWidgetsWithCertainName<URenewal_BaseHeroIcon>(this, AttackHeros, TEXT("attack_Hero_"));
		for (URenewal_BaseHeroIcon* icon : AttackHeros)
			icon->SetVisibility(ESlateVisibility::Collapsed);

		int32 index = 0;
		for (int32 i = 0; i < HeroIDs.Num(); i++) {
			if (AttackHeros.IsValidIndex(index))
			{
// 				FHERO_ICON_INFO icon_info;
// 				UUIFunctionLibrary::GetHeroIconInfoWithID(icon_info, HeroIDs[i], EHeroOwnershipType::VE_None);
				AttackHeros[index]->SetHeroIconInfoWithID(HeroIDs[i], EHeroOwnershipType::VE_None);
				AttackHeros[index]->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
				AttackHeros[index]->HideUI();
				index++;
			}
		}
	}
	else
	{
		if (IsValid(WidgetSwitcher_DResult))
			WidgetSwitcher_DResult->SetActiveWidgetIndex(iResult);
		if (IsValid(WidgetSwitcher_DPointImage))
			WidgetSwitcher_DPointImage->SetActiveWidgetIndex(iResultImage);
		if (IsValid(TextBlock_DPoint))
			TextBlock_DPoint->SetText(FText::AsNumber(Point));
		if (IsValid(TextBlock_DLevel))
			TextBlock_DLevel->SetText(FText::Format(FText::FromString("Lv.{0}"), Level));
		if (IsValid(TextBlock_DName))
			TextBlock_DName->SetText(Name);
		if (IsValid(TextBlock_DGuild))
			TextBlock_DGuild->SetText(Guild);
		if (IsValid(TextBlock_DPower))
			TextBlock_DPower->SetText(FText::AsNumber(Power));

		DefenceHeros.Empty();
		FindChildWidgetsWithCertainName<URenewal_BaseHeroIcon>(this, DefenceHeros, TEXT("defence_Hero_"));
		for (URenewal_BaseHeroIcon* icon : DefenceHeros)
			icon->SetVisibility(ESlateVisibility::Collapsed);

		int32 index = 0;
		for (int32 i = 0; i < HeroIDs.Num(); i++) {
			if (DefenceHeros.IsValidIndex(index))
			{
// 				FHERO_ICON_INFO icon_info;
// 				UUIFunctionLibrary::GetHeroIconInfoWithID(icon_info, HeroIDs[i], EHeroOwnershipType::VE_None);
				DefenceHeros[index]->SetHeroIconInfoWithID(HeroIDs[i], EHeroOwnershipType::VE_None);
				DefenceHeros[index]->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
				DefenceHeros[index]->HideUI();
				index++;
			}
		}
	}
}

void UUC_Arena_Battle_Record_Data::SetHistory(bool InDefence, bool InResult, int32 InPoint, int32 InLevel, FText InName, FText InGuild, int32 InPower, TArray<FName> InHeroIDs)
{
	Defence = InDefence;
	Result = InResult;
	if(InPoint < 0)
		Point = InPoint*-1;
	else
		Point = InPoint;
	Level = InLevel;
	Name = InName;
	Guild = InGuild;
	Power = InPower;
	HeroIDs = InHeroIDs;
}
