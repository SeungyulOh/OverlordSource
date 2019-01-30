
#include "ProjectR.h"
#include "UP_Guild_Visit.h"

#include "GameInfo/UserInfo.h"
#include "GameInfo/GuildInfo.h"
#include "GameInfo/RInventory.h"
#include "UI/Guild/GuildMember_ScrollItemData.h"
#include "UI/Guild/GuildRuneDonationScrollItemData.h"
#include "UI/Guild/GuildManagerListScrollItemData.h"
#include "UI/Guild/UC_GuildRuneRequestPopup.h"
#include "UI/Control/RScrollView.h"
#include "UI/Common/HeroBaseScrollItem.h"
#include "UtilFunctionIntegrated.h"
#include "UtilFunctionIntegrated.h"
#include "GlobalIntegrated.h"

#include "Network/PacketFunctionLibrary.h"
#include "Network/HttpRequestClient.h"

//temp
#define MAX_GUILDMEM_NUM 100

void UUP_GuildVisit::NativeConstruct()
{
	Super::NativeConstruct();
	//only reset when array is empty
	GuestGuildMemberScrollView->Init();
	if (cachedData.Num() == 0)
	{
		for(int32 i = 0; i < MAX_GUILDMEM_NUM; i++)
		{
			UGuildManagerListScrollItemData* itemdata = NewObject<UGuildManagerListScrollItemData>((UObject*)GetTransientPackage(), UGuildManagerListScrollItemData::StaticClass());
			itemdata->WidgetSize = GuestGuildMemberScrollView->GetItemInitSize();
			cachedData.Emplace(itemdata);
		}
	}
	Init();
}

void UUP_GuildVisit::NativeDestruct()
{
	Super::NativeDestruct();
}

void UUP_GuildVisit::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);
}

void UUP_GuildVisit::Init()
{
	URGameInstance* RGameInstance = RGAMEINSTANCE(this);
	
	FGUILD_INFO guildInfo = RGameInstance->GuildInfo->Guest_GuildInfo;
	GuestGuildNameText->SetText(FText::FromString(guildInfo.name));
	GuestGuildRankingText->SetText(FText::AsNumber(guildInfo.point_rank));
	GuestGuildPointText->SetText(FText::AsNumber(guildInfo.point));
	GuestGuildGoldText->SetText(FText::AsNumber(guildInfo.gold));
	GuestGuildMemberCntText->SetText(FText::AsNumber(guildInfo.member_count));
	GuestGuildMemberMaxText->SetText(FText::AsNumber(guildInfo.member_limit));
	GuestGuildMemberMaxNotice->SetText(FText::FromString(guildInfo.desc));
	
	
	FString LevelIconKey = "Icon_GuildLevel_";
	if (10 > guildInfo.level)
		LevelIconKey += "0";
	LevelIconKey.AppendInt(guildInfo.level);
	LevelIconKey += "_Sprite";
	UUIFunctionLibrary::SetImageBrush(UUIFunctionLibrary::GetIconImageFromTable(FName(*LevelIconKey)), GuestGuildLevelIconImage);

	
	if (GuestGuildMemberScrollView) {
		GuestGuildMemberScrollView->ClearScrollItemData();
		GuestGuildMemberScrollView->SetVisibility(ESlateVisibility::Visible);

		TArray<FGUILD_MEMBER> GuildMemberList = RGameInstance->GuildInfo->Guest_GuildMemberList;
		EGUILD_GRADE MyGrade = EGUILD_GRADE::MEMBER;
		int32 MyKid = RGameInstance->GuildInfo->MyMemberData.kId;

		for(int32 i = 0; i < GuildMemberList.Num(); ++i)
		{
			FGUILD_MEMBER& GuildMember = GuildMemberList[i];
			UGuildManagerListScrollItemData* ScrollItemData = cachedData[i];
			if (ScrollItemData)
			{
				ScrollItemData->GuildMemberData = GuildMember;
				ScrollItemData->MyGuildGrade = MyGrade;
				ScrollItemData->MyAccountKid = MyKid;
				ScrollItemData->IsBtnOn = false;
				if (GuildMember.grade == EGUILD_GRADE::MASTER)
					GuestGuildMasterNameText->SetText(FText::FromString(GuildMember.nick));

				GuestGuildMemberScrollView->ItemDataList.Emplace(ScrollItemData);
			}
		}

		GuestGuildMemberScrollView->InvalidateData();
	}
}