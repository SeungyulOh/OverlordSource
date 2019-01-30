#include "ProjectR.h"
#include "UC_Colosseum_BattleRecord.h"

void URColosseum_BattleRecord_Data::SetHistory(bool InResult, int32 InGradeID, int32 InPoint, int32 InLevel, FText InName, FText InGuild, int32 InPower, TArray<FName> InHeroIDs)
{
	Result = InResult;
	if (InPoint < 0)
		Point = InPoint * -1;
	else
		Point = InPoint;
	GradeID = InGradeID;
	Level = InLevel;
	Name = InName;
	Guild = InGuild;
	Power = InPower;
	HeroIDs = InHeroIDs;
}


void URColosseum_BattleRecord_Item::InvalidateData()
{
	//Super::InvalidateData();
	URColosseum_BattleRecord_Data* battleRecord = Cast<URColosseum_BattleRecord_Data>(ItemData);
	if (battleRecord)
		SetHistory(battleRecord->Result, battleRecord->GradeID, battleRecord->Point, battleRecord->Level, battleRecord->Name, battleRecord->Guild, battleRecord->Power, battleRecord->HeroIDs);
}

void URColosseum_BattleRecord_Item::SetHistory(bool Result, int32 InGradeID, int32 Point, int32 Level, FText Name, FText Guild, int32 Power, TArray<FName> HeroIDs)
{
	int32		iResult = 1;
	int32		iResultImage = 0;

	if (Result == true)
	{
		iResult = 0;
		iResultImage = 1;
	}
	if (Point == 0)
	{
		if (IsValid(WidgetSwitcher_APointImage))
			WidgetSwitcher_APointImage->SetVisibility(ESlateVisibility::Hidden);
	}
	else
	{
		if (IsValid(WidgetSwitcher_APointImage))
			WidgetSwitcher_APointImage->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
	}
	if (IsValid(Colosseum_Tier))
		Colosseum_Tier->SetColosseumClassIndex(InGradeID);
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
	for (int32 i = 0; i < HeroIDs.Num(); i++)
	{
		if (AttackHeros.IsValidIndex(index))
		{
			AttackHeros[index]->SetHeroIconInfoWithID(HeroIDs[i], EHeroOwnershipType::VE_None);
			AttackHeros[index]->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
			AttackHeros[index]->HideUI();
			index++;
		}
	}
}