
#include "ProjectR.h"
#include "UC_GuildRaid_MemberItem.h"

#include "GlobalIntegrated.h"

#include "UtilFunctionIntegrated.h"
#include "UI/Guild/GuildRaid/UC_GuildRaid_MemberData.h"


void UUC_GuildRaid_MemberItem::SetScrollItemData(int32 Index, URScrollItemData* item, URScrollView* ScrollView)
{
	if (false == IsValid(item))
		return;
	
	Refresh();
}

void UUC_GuildRaid_MemberItem::Refresh()
{
	if (false == IsValid(this->ItemData))
		return;

	UUC_GuildRaid_MemberData* ScrollItemData = Cast<UUC_GuildRaid_MemberData>(this->ItemData);
	if (false == IsValid(ScrollItemData))
		return;
	MemberNameText->SetText(FText::FromName(ScrollItemData->MemberName));

	ChallengeCount->SetText(FText::AsNumber(ScrollItemData->ChallengeCount));
	CreateCount->SetText(FText::AsNumber(ScrollItemData->CreateCount));

	/*
	USK_up_Arenalobby_SeasonExit_Day : day
	UI_GuildRaid_0006 : hour
	UI_GuildRaid_0007 : minute
	UI_GuildRaid_0008 : second
	*/
	int32& rTime = ScrollItemData->SinceTime;
	int32		nTime1 = 0;
	int32		nTime2 = 0;

	if (GetDays(rTime) > 0)
	{
		LocTimeText1->SetText(UUtilFunctionLibrary::GetLocalizedString(EStringTableType::VE_UI, FName(TEXT("USK_up_Arenalobby_SeasonExit_Day"))));
		LocTimeText2->SetText(UUtilFunctionLibrary::GetLocalizedString(EStringTableType::VE_UI, FName(TEXT("UI_GuildRaid_0006"))));
		nTime1 = GetDays(rTime);
		nTime2 = GetHours(rTime);
	}
	else if (GetHours(rTime) > 0)
	{
		LocTimeText1->SetText(UUtilFunctionLibrary::GetLocalizedString(EStringTableType::VE_UI, FName(TEXT("UI_GuildRaid_0006"))));
		LocTimeText2->SetText(UUtilFunctionLibrary::GetLocalizedString(EStringTableType::VE_UI, FName(TEXT("UI_GuildRaid_0007"))));
		nTime1 = GetHours(rTime);
		nTime2 = GetMinutes(rTime);
	}
	else if (GetMinutes(rTime) > 0)
	{
		LocTimeText1->SetText(UUtilFunctionLibrary::GetLocalizedString(EStringTableType::VE_UI, FName(TEXT("UI_GuildRaid_0007"))));
		LocTimeText2->SetText(UUtilFunctionLibrary::GetLocalizedString(EStringTableType::VE_UI, FName(TEXT("UI_GuildRaid_0008"))));
		nTime1 = GetMinutes(rTime);
		nTime2 = GetSeconds(rTime);
	}
	else if (GetSeconds(rTime) > 0)
	{
		LocTimeText1->SetText(FText::GetEmpty());
		LocTimeText2->SetText(UUtilFunctionLibrary::GetLocalizedString(EStringTableType::VE_UI, FName(TEXT("UI_GuildRaid_0008"))));
		nTime1 = 0;
		nTime2 = GetSeconds(rTime);
	}
	
	if(nTime1 > 0)
		TimeNum1->SetText(FText::AsNumber(nTime1));
	else
		TimeNum1->SetText(FText::GetEmpty());
	
	TimeNum2->SetText(FText::AsNumber(nTime2));

	++rTime;
}

int32 UUC_GuildRaid_MemberItem::GetDays(int32 InTick)
{
	// 3600 * 24 // day to sec
	return (InTick / 86400);
}

int32 UUC_GuildRaid_MemberItem::GetHours(int32 InTick)
{
	return ((InTick / 3600) % 24);
}

int32 UUC_GuildRaid_MemberItem::GetMinutes(int32 InTick)
{
	return (InTick % 3600) / 60;
}

int32 UUC_GuildRaid_MemberItem::GetSeconds(int32 InTick)
{
	return InTick % 60;
}