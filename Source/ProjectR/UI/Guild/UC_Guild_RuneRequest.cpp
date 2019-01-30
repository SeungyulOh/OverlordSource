// Fill out your copyright notice in the Description page of Project Settings.

#include "ProjectR.h"
#include "UC_Guild_RuneRequest.h"

#include "GlobalIntegrated.h"
#include "GameInfo/GuildInfo.h"
#include "GameInfo/RInventory.h"

#include "UtilFunctionIntegrated.h"
#include "Network/HttpRequestClient.h"
#include "Network/PacketFunctionLibrary.h"
#include "Network/PacketDataStructures.h"
#include "UI/UIDefs.h"
#include "UI/Common/HeroBaseScrollItem.h"
#include "UI/Guild/GuildRuneRequest_ScrollItemData.h"

void UUC_Guild_RuneRequest::NativeConstruct()
{
	Super::NativeConstruct();
}

void UUC_Guild_RuneRequest::NativeDestruct()
{
	Super::NativeDestruct();
}

void UUC_Guild_RuneRequest::SetScrollItemData(int32 Index, URScrollItemData* item, URScrollView* ScrollView)
{
	if (!IsValid(ItemData))
		return;
	auto RuneRequestItemData = Cast<UGuildRuneRequest_ScrollItemData>(item);
	if (!IsValid(RuneRequestItemData))
		return;
	URGameInstance* RGameInstance = RGAMEINSTANCE(this);
	if (RGameInstance == nullptr)
		return;

	RequestCount = 0;

	FGUILD_RUNE_REQUESTER& GuildRuneRequester = RuneRequestItemData->RuneRequester;

	if (IsValid(RGameInstance->RInventory))
	{
		FHERO* outHero = RGameInstance->RInventory->GetHeroWithID(GuildRuneRequester.heroId);
		if (outHero != nullptr)
		{
// 			FHERO_ICON_INFO HeroIconInfo;
// 			UUIFunctionLibrary::GetHeroIconInfoWithUD(HeroIconInfo, outHero->heroUD, EHeroOwnershipType::VE_Me);
// 			HeroScrollItem->SetHeroIconInfo(HeroIconInfo);
			HeroScrollItem->SetHeroIconInfoWithUD(outHero->heroUD);
		}
	}

	switch (GuildRuneRequester.grade)
	{
	case EGUILD_GRADE::MEMBER:	MemberGrade->SetActiveWidgetIndex(0);	break;
	case EGUILD_GRADE::ELDER:	MemberGrade->SetActiveWidgetIndex(1);	break;
	case EGUILD_GRADE::MASTER:	MemberGrade->SetActiveWidgetIndex(2);	break;
	}

	if (RGameInstance->HttpClient->IsValidKID(GuildRuneRequester.kId))
	{
		if (GuildRuneRequester.gettable)
		{
			SupportType->SetActiveWidgetIndex(1);
			SupportType->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
		}
		else
		{
			SupportType->SetVisibility(ESlateVisibility::Collapsed);
		}
		
		Btn_ReceiveRune->SetIsEnabled((GuildRuneRequester.currentAmount == GuildRuneRequester.maxAmount));
		Btn_RuneCountMinus->SetIsEnabled(false);
		Btn_RuneCountPlus->SetIsEnabled(false);
	}
	else
	{
		SupportType->SetActiveWidgetIndex(0);
		SupportType->SetVisibility(ESlateVisibility::SelfHitTestInvisible);

		Btn_ReceiveRune->SetIsEnabled(false);
		Btn_RuneCountMinus->SetIsEnabled(true);
		Btn_RuneCountPlus->SetIsEnabled(true);
	}
		
	Text_Nick->SetText(FText::FromString(GuildRuneRequester.nick));

	FHERO* HeroData = RGameInstance->RInventory->GetHeroWithID(GuildRuneRequester.heroId);
// 	if (HeroData != nullptr)
// 	{
// 		Text_CurrentHave->SetText(FText::AsNumber(HeroData->rune));
// 	}

	int32 hour = (int32)(GuildRuneRequester.timeLeft / 3600);
	int32 minute = (int32)(GuildRuneRequester.timeLeft / 60) % 60;
	Text_RemainHour->SetText(FText::AsNumber(hour));
	Text_RemainMinute->SetText(FText::AsNumber(minute));

	Text_CurrentAmount->SetText(FText::AsNumber(GuildRuneRequester.currentAmount));
	Text_MaxAmount->SetText(FText::AsNumber(GuildRuneRequester.maxAmount));
	Text_SupportAmount->SetText(FText::AsNumber(RequestCount));

	float fPercent = 0.0f;
	if (0 < GuildRuneRequester.currentAmount)
	{
		fPercent = (float)GuildRuneRequester.currentAmount / (float)GuildRuneRequester.maxAmount;
	}
	ReceiveRune->SetPercent(fPercent);
}

void UUC_Guild_RuneRequest::OnClick_RuneCount(bool Add)
{
	if (!IsValid(ItemData))
		return;
	auto RuneRequestItemData = Cast<UGuildRuneRequest_ScrollItemData>(ItemData);
	if (!IsValid(RuneRequestItemData))
		return;

	FGUILD_RUNE_REQUESTER& GuildRuneRequester = RuneRequestItemData->RuneRequester;

	if (Add)
	{
		++RequestCount;
	}
	else
	{
		--RequestCount;
	}

	int32 NeedCount = GuildRuneRequester.maxAmount - GuildRuneRequester.currentAmount;

	if (0 >= RequestCount)
		RequestCount = 1;
	else if (NeedCount <= RequestCount)
		RequestCount = NeedCount;

	Text_SupportAmount->SetText(FText::AsNumber(RequestCount));
}

void UUC_Guild_RuneRequest::OnClick_RequestRuneSupport()
{
	URGameInstance* RGameInstance = RGAMEINSTANCE(this);
	if (RGameInstance == nullptr)
		return;

	if (!IsValid(ItemData))
		return;
	auto RuneRequestItemData = Cast<UGuildRuneRequest_ScrollItemData>(ItemData);
	if (!IsValid(RuneRequestItemData))
		return;

	FGUILD_RUNE_REQUESTER& GuildRuneRequester = RuneRequestItemData->RuneRequester;

	UPacketFunctionLibrary::GUILD_RUNE_SUPPORT_RQ(GuildRuneRequester.kId, GuildRuneRequester.heroId, RequestCount);
}

void UUC_Guild_RuneRequest::OnClick_RequestRuneGet()
{
	URGameInstance* RGameInstance = RGAMEINSTANCE(this);
	if (RGameInstance == nullptr)
		return;

	UPacketFunctionLibrary::GUILD_RUNE_GET_RQ();
}