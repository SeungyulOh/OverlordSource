// Fill out your copyright notice in the Description page of Project Settings.

#include "ProjectR.h"
#include "UC_Colosseum_Deck.h"

#include "GlobalIntegrated.h"


void UUC_Colosseum_Deck::NativeConstruct()
{
	Super::NativeConstruct();
}

void UUC_Colosseum_Deck::NativeDestruct()
{
	Super::NativeDestruct();
}

void UUC_Colosseum_Deck::InitPlayer(FRTS_PLAYER InPlayer, bool bIsMyDeck /*= false*/)
{
	mPlayer = InPlayer;
	mIsMyDeck = bIsMyDeck;
	mReady = false;
	mKID = InPlayer.kId;

	int32	TeamColor = mPlayer.team - 1;

	if (IsValid(Team_Switcher))
		Team_Switcher->SetActiveWidgetIndex(TeamColor);

	for (int32 i = 0; i < 5; i++)
	{
		if (IsValid(BlueDeck[i]) && BlueDeck.IsValidIndex(i))
			BlueDeck[i]->Init(EPartyRole(i));
		if (IsValid(RedDeck[i]) && RedDeck.IsValidIndex(i))
			RedDeck[i]->Init(EPartyRole(i));
	}
}

void UUC_Colosseum_Deck::ReadyHeroSlot()
{
	if (mIsMyDeck)
	{
		for (int32 i = 0; i < 5; i++)
		{
			if (IsValid(BlueDeck[i]) && BlueDeck.IsValidIndex(i))
			{
				BlueDeck[i]->OnClickedHeroSlot = this->OnClickedHeroSlot;
				BlueDeck[i]->ReadyHeroSlot();
			}

			if (IsValid(RedDeck[i]) && RedDeck.IsValidIndex(i))
			{
				RedDeck[i]->OnClickedHeroSlot = this->OnClickedHeroSlot;
				RedDeck[i]->ReadyHeroSlot();
			}
		}
	}
}

void UUC_Colosseum_Deck::OnReceiveHero(FCSDeckSelected InReceiveHero)
{
	int16			CrewNo = InReceiveHero.crewNo - 1;
	if (BlueDeck.IsValidIndex(CrewNo))
		BlueDeck[CrewNo]->ReceiveHero(InReceiveHero.heroId, InReceiveHero.heroUD, InReceiveHero.level);
	if (RedDeck.IsValidIndex(CrewNo))
		RedDeck[CrewNo]->ReceiveHero(InReceiveHero.heroId, InReceiveHero.heroUD, InReceiveHero.level);
}

void UUC_Colosseum_Deck::FrezeeHeroSlot()
{
	for (int32 i = 0; i < 5; i++)
	{
		if (IsValid(BlueDeck[i]) && BlueDeck.IsValidIndex(i))
			BlueDeck[i]->FrezeeHeroSlot();
		if (IsValid(RedDeck[i]) && RedDeck.IsValidIndex(i))
			RedDeck[i]->FrezeeHeroSlot();
	}
}

void UUC_Colosseum_Deck::HideNotiImage()
{
	for (int32 i = 0; i < 5; i++)
	{
		if (IsValid(BlueDeck[i]) && BlueDeck.IsValidIndex(i))
			BlueDeck[i]->NotiImage->SetVisibility(ESlateVisibility::Collapsed);
		if (IsValid(RedDeck[i]) && RedDeck.IsValidIndex(i))
			RedDeck[i]->NotiImage->SetVisibility(ESlateVisibility::Collapsed);
	}
}
