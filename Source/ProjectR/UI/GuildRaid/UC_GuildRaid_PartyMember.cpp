 // Fill out your copyright notice in the Description page of Project Settings.

#include "ProjectR.h"
#include "UC_GuildRaid_PartyMember.h"

#include "GlobalIntegrated.h"

//#include "Global/BattleFieldManager.h"
#include "UtilFunctionIntegrated.h"
#include "UtilFunctionIntegrated.h"
#include "UI/Common/Renewal_BaseHeroIcon.h"

void UUC_GuildRaid_PartyMember::NativeConstruct()
{
	Super::NativeConstruct();

//	SelectMember = false;
}

void UUC_GuildRaid_PartyMember::NativeDestruct()
{
	Super::NativeDestruct();
}
// 
// void UUC_GuildRaid_PartyMember::OnClick_Member()
// {
// 	auto RGameInstance = RGAMEINSTANCE(this);
// 	if (IsValid(RGameInstance) == false)
// 		return;
// 
// 	RGameInstance->EventManager->OnPartyMemberClick.Broadcast(PartyIndex, SlotIndex, !SelectMember);
// }
// 
// void UUC_GuildRaid_PartyMember::SetGroupPartyData(int32 InGroupID, int32 InPartyIndex)
// {
// 	PartyIndex = InPartyIndex;
// 
// 	RefreshPartyMember();
// }
// 
// void UUC_GuildRaid_PartyMember::RefreshGroupPartyData(int32 InPartyIndex, int32 InSlotIndex)
// {
// 	if (PartyIndex != InPartyIndex)
// 		return;
// 	if (SlotIndex != InSlotIndex)
// 		return;
// 
// 	RefreshPartyMember();
// }
// 
// void UUC_GuildRaid_PartyMember::ClickGroupPartyData(int32 InPartyIndex, int32 InSlotIndex, bool InSelectMember)
// {
// 	if (PartyIndex != InPartyIndex)
// 		return;
// 	if (SlotIndex != InSlotIndex)
// 		return;
// 
// 	SelectMember = InSelectMember;
// 
// 	ESlateVisibility MemberSelectVisible = (SelectMember ? ESlateVisibility::SelfHitTestInvisible : ESlateVisibility::Collapsed);
// 	if (IsValid(Switcher_MemberSelect))
// 	{
// 		Switcher_MemberSelect->SetVisibility(MemberSelectVisible);
// 	}
// }
// 
// void UUC_GuildRaid_PartyMember::RefreshPartyMember()
// {
// 	auto RGameInstance = RGAMEINSTANCE(this);
// 	if (IsValid(RGameInstance) == false)
// 		return;
// 	if (IsValid(RGameInstance->BattleFieldManager) == false)
// 		return;
// 
// 	ESlateVisibility MemberVisible = ESlateVisibility::Collapsed;
// 	ESlateVisibility HeroIconVisible = ESlateVisibility::Collapsed;
// 	ESlateVisibility GuildLeaderVisible = ESlateVisibility::Collapsed;
// 	ESlateVisibility IsMineVisible = ESlateVisibility::Collapsed;
// 	ESlateVisibility MemberSelectVisible = ESlateVisibility::Collapsed;
// 	
// 	UUIFunctionLibrary::ClearHeroIconIfo(HeroIconInfo);
// 
// 	FBattleFieldPartyMemberInfo* PartyMemberInfo = RGameInstance->BattleFieldManager->GetPartyMemberInfo(PartyIndex, SlotIndex);
// 	if (PartyMemberInfo)
// 	{
// 		if (!PartyMemberInfo->NickName.IsEmpty())
// 		{
// 			MemberVisible = ESlateVisibility::SelfHitTestInvisible;
// 			GuildLeaderVisible = (PartyMemberInfo->IsGroupCommander ? ESlateVisibility::SelfHitTestInvisible : ESlateVisibility::Collapsed);
// 			IsMineVisible = (PartyMemberInfo->IsMine ? ESlateVisibility::SelfHitTestInvisible : ESlateVisibility::Collapsed);
// 			MemberSelectVisible = (SelectMember ? ESlateVisibility::SelfHitTestInvisible : ESlateVisibility::Collapsed);
// 
// 			for (auto TextName : Text_MemberName)
// 			{
// 				if (IsValid(TextName))
// 				{
// 					TextName->SetText(FText::FromString(PartyMemberInfo->NickName));
// 				}
// 			}
// 
// 			if (!PartyMemberInfo->HeroID.IsNone())
// 			{
// 				HeroIconVisible = ESlateVisibility::SelfHitTestInvisible;
// 				UUIFunctionLibrary::GetHeroIconInfoWithID(this, HeroIconInfo, PartyMemberInfo->HeroID, EHeroOwnershipType::VE_None);
// 				HeroIconInfo.level = PartyMemberInfo->HeroLevel;
// 				if (IsValid(HeroIcon))
// 				{
// 					HeroIcon->SetHeroIconInfo(HeroIconInfo);
// 				}
// 			}
// 		}
// 	}
// 
// 	if (IsValid(Switcher_MemberName))
// 	{
// 		Switcher_MemberName->SetVisibility(MemberVisible);
// 
// 		if (ESlateVisibility::SelfHitTestInvisible == HeroIconVisible)
// 		{
// 			Switcher_MemberName->SetActiveWidgetIndex(0);
// 		}
// 		else
// 		{
// 			Switcher_MemberName->SetActiveWidgetIndex(1);
// 		}
// 	}
// 	if (IsValid(HeroIcon))
// 	{
// 		HeroIcon->SetVisibility(HeroIconVisible);
// 	}
// 	if (IsValid(GuildLeader))
// 	{
// 		GuildLeader->SetVisibility(GuildLeaderVisible);
// 	}
// 	if (IsValid(Image_IsMine))
// 	{
// 		Image_IsMine->SetVisibility(IsMineVisible);
// 	}
// 	if (IsValid(Switcher_MemberSelect))
// 	{
// 		Switcher_MemberSelect->SetVisibility(MemberSelectVisible);
// 	}
// }