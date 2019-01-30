// Fill out your copyright notice in the Description page of Project Settings.

#include "ProjectR.h"
#include "GlobalIntegrated.h"
#include "UtilFunctionIntegrated.h"
#include "UI/RWidgetManager.h"
#include "UtilFunctionIntegrated.h"
#include "UI/Control/RScrollView.h"
#include "UI/Common/Renewal_BaseHeroIcon.h"
#include "UI/Raid/RaidListScrollData.h"
#include "UI/Arena/UC_Arena_DeckBtn.h"
#include "Network/PacketFunctionLibrary.h"
#include "Network/PacketDataStructures.h"
#include "Network/PacketDataEnum.h"
#include "Network/HttpRequestClient.h"
#include "Network/RTSManager.h"
#include "Components/TextBlock.h"
#include "Engine/Texture2D.h"
#include "UP_RaidPartyRoom.h"
#include "UI/Common/HeroScrollItemData.h"

void UUC_RaidPartyDeck::NativeConstruct()
{
	Super::NativeConstruct();
}

void UUC_RaidPartyDeck::NativeDestruct()
{
	Super::NativeDestruct();
}

void UUC_RaidPartyDeck::InitPlayer(FRTS_PLAYER InPlayer, bool bIsMyDeck /*= false*/)
{
	mPlayer = InPlayer;
	mIsMyDeck = bIsMyDeck;
	mReady = false;
	mKID = InPlayer.kId;
	nickname->SetText(FText::FromString(mPlayer.nick));
	UC_Hero_Slot->Init(EPartyRole::Leader);
	UC_Soul_Slot1->Init(EPartyRole::Crew1_First);
	UC_Soul_Slot2->Init(EPartyRole::Crew1_Second);
	UC_Soul_Slot3->Init(EPartyRole::Crew2_First);
	UC_Soul_Slot4->Init(EPartyRole::Crew2_Second);
	LocalTextBlockReady->SetVisibility(ESlateVisibility::Hidden);
}

void UUC_RaidPartyDeck::ReadyHeroSlot()
{
	if (mIsMyDeck)
	{
		UC_Hero_Slot->ReadyHeroSlot();
		UC_Hero_Slot->OnClickedHeroSlot = this->OnClickedHeroSlot;
		UC_Soul_Slot1->ReadyHeroSlot();
		UC_Soul_Slot1->OnClickedHeroSlot = this->OnClickedHeroSlot;
		UC_Soul_Slot2->ReadyHeroSlot();
		UC_Soul_Slot2->OnClickedHeroSlot = this->OnClickedHeroSlot;
		UC_Soul_Slot3->ReadyHeroSlot();
		UC_Soul_Slot3->OnClickedHeroSlot = this->OnClickedHeroSlot;
		UC_Soul_Slot4->ReadyHeroSlot();
		UC_Soul_Slot4->OnClickedHeroSlot = this->OnClickedHeroSlot;
	}
}

void UUC_RaidPartyDeck::FrezeeHeroSlot()
{
	UC_Hero_Slot->FrezeeHeroSlot();
	UC_Soul_Slot1->FrezeeHeroSlot();
	UC_Soul_Slot2->FrezeeHeroSlot();
	UC_Soul_Slot3->FrezeeHeroSlot();
	UC_Soul_Slot4->FrezeeHeroSlot();
}

void UUC_RaidPartyDeck::HideNotiImage()
{
	UC_Hero_Slot->NotiImage->SetVisibility(ESlateVisibility::Collapsed);
	UC_Soul_Slot1->NotiImage->SetVisibility(ESlateVisibility::Collapsed);
	UC_Soul_Slot2->NotiImage->SetVisibility(ESlateVisibility::Collapsed);
	UC_Soul_Slot3->NotiImage->SetVisibility(ESlateVisibility::Collapsed);
	UC_Soul_Slot4->NotiImage->SetVisibility(ESlateVisibility::Collapsed);
}

void UUC_RaidPartyDeck::OnReceiveHero(FDeckSelected InReceiveHero)
{
	switch (InReceiveHero.crewNo)
	{
	case 1:
		UC_Hero_Slot->ReceiveHero(InReceiveHero.heroId, InReceiveHero.heroUD, InReceiveHero.level);
		break;
	case 2:
		UC_Soul_Slot1->ReceiveHero(InReceiveHero.heroId, InReceiveHero.heroUD, InReceiveHero.level);
		break;
	case 3:
		UC_Soul_Slot2->ReceiveHero(InReceiveHero.heroId, InReceiveHero.heroUD, InReceiveHero.level);
		break;
	case 4:
		UC_Soul_Slot3->ReceiveHero(InReceiveHero.heroId, InReceiveHero.heroUD, InReceiveHero.level);
		break;
	case 5:
		UC_Soul_Slot4->ReceiveHero(InReceiveHero.heroId, InReceiveHero.heroUD, InReceiveHero.level);
		break;
	}
}

void UUC_RaidPartyDeck::SetReady(bool InReady)
{
	if (InReady)
		LocalTextBlockReady->SetVisibility(ESlateVisibility::Visible);
	else
		LocalTextBlockReady->SetVisibility(ESlateVisibility::Hidden);
	mReady = InReady;
}


void UUC_RaidPartyDeckBtn::NativeConstruct()
{
	Super::NativeConstruct();

}

void UUC_RaidPartyDeckBtn::NativeDestruct()
{
	Super::NativeDestruct();
}

void UUC_RaidPartyDeckBtn::Init(EPartyRole InPartyRole)
{
	mPartyRole = InPartyRole;
	DeckHeroIcon->SetVisibility(ESlateVisibility::Hidden);
	ButtonSwitcher->SetVisibility(ESlateVisibility::HitTestInvisible);
	ButtonSwitcher->SetActiveWidgetIndex(0);
	mHeroID = TEXT("");
	mHeroUD = TEXT("");
	mHeroLevel = -1;
}

void UUC_RaidPartyDeckBtn::ReadyHeroSlot()
{
	ButtonSwitcher->SetActiveWidgetIndex(0);
	NotiImage->SetVisibility(ESlateVisibility::Visible);
	UWidgetAnimation* Anim = GetWidgetAnimFromName(TEXT("Deck_Available"));
	PlayAnimation(Anim, 0.f, 0);
	ButtonInput->OnClicked.RemoveDynamic(this, &UUC_RaidPartyDeckBtn::OnClickedThis);
	ButtonInput->OnClicked.AddDynamic(this, &UUC_RaidPartyDeckBtn::OnClickedThis);
}

void UUC_RaidPartyDeckBtn::FrezeeHeroSlot()
{
	OnClickedHeroSlot.Clear();
	NotiImage->SetVisibility(ESlateVisibility::Collapsed);
	if (mHeroID.IsEmpty() || mHeroUD.IsEmpty()  || mHeroLevel == -1)
	{
		DeckHeroIcon->SetVisibility(ESlateVisibility::Hidden);
		ButtonSwitcher->SetActiveWidgetIndex(0);
	}
	else
	{
		DeckHeroIcon->SetVisibility(ESlateVisibility::Visible);
		ButtonSwitcher->SetActiveWidgetIndex(1);
	}
}

void UUC_RaidPartyDeckBtn::ReceiveHero(FString HeroID, FString HeroUD, int16 level)
{
	if (HeroID.IsEmpty() || HeroUD.IsEmpty() || level == -1)
	{
		mHeroID = HeroID;
		mHeroUD = HeroUD;
		mHeroLevel = level;
		DeckHeroIcon->SetVisibility(ESlateVisibility::Hidden);
		ButtonSwitcher->SetActiveWidgetIndex(0);
	}
	else
	{
		mHeroID = HeroID;
		mHeroUD = HeroUD;
		mHeroLevel = level;
		DeckHeroIcon->SetHeroIconInfoWithID(FName(*HeroID), EHeroOwnershipType::VE_None);
	}
	
}

void UUC_RaidPartyDeckBtn::OnClickedThis()
{
	if (OnClickedHeroSlot.IsBound())
		OnClickedHeroSlot.Broadcast(mPartyRole);
}
