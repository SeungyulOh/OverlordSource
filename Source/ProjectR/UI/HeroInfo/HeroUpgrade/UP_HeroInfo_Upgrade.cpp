// Fill out your copyright notice in the Description page of Project Settings.

#include "ProjectR.h"

#include "Global/RGameInstance.h"
#include "Global/EventManager.h"
#include "Global/TableManager.h"
#include "UI/RWidgetManager.h"

#include "GameInfo/RInventory.h"

#include "UI/HeroInfo/HeroManager/UP_HeroManager.h"
#include "UP_HeroInfo_Upgrade.h"
#include "UIFunctionLibrary.h"

#include "Lobby/IndoorLobbyStateActor.h"
#include "ContentGameMode/Modes/TowerLobbyGameMode.h"

void UUP_HeroInfo_Upgrade::NativeConstruct()
{
	Super::NativeConstruct();
	Init();
	
}

void UUP_HeroInfo_Upgrade::NativeDestruct()
{
	Super::NativeDestruct();
	
}

void UUP_HeroInfo_Upgrade::Init()
{
	URGameInstance* RGameInstance = RGAMEINSTANCE(this);


	checkf(RGameInstance != nullptr, TEXT("UUP_HeroInfo_Upgrade[RGameInstance Is Empty]"));
	checkf(RGameInstance->EventManager != nullptr, TEXT("UUP_HeroInfo_Upgrade[RInventory Is Empty]"));

	if (IsValid(ButtonBack))
	{
		ButtonBack->OnClicked.Clear();
		ButtonBack->OnClicked.AddDynamic(this, &UUP_HeroInfo_Upgrade::OnClickBack);
	}


}

void UUP_HeroInfo_Upgrade::SetHero(FString InHeroUD)
{
	OnClick_HeroIcon(InHeroUD);
}

void UUP_HeroInfo_Upgrade::OnClick_HeroIcon(FString InHeroUD)
{
	URGameInstance*			RGameInstance	=	RGAMEINSTANCE(this);
	checkf(RGameInstance != nullptr, TEXT("UUP_HeroManager[RGameInstance Is Empty]"));

	m_HeroUD = InHeroUD;
	m_Hero = RGameInstance->RInventory->GetHeroWithUD(m_HeroUD);

	if (IsValid(UpgradeHeroInfo))
	{
		UpgradeHeroInfo->SetHero(m_HeroUD);
	}
	if (IsValid(UpgradeResult))
		UpgradeResult->SetHero(*m_Hero);
}

void UUP_HeroInfo_Upgrade::OnHeroUpgrade()
{
	if (IsValid(UpgradeResult))
	{
		UpgradeResult->SetVisibility(ESlateVisibility::Visible);
		//UpgradeResult->SetResult();
	}

	OnClick_HeroIcon(m_HeroUD);

	URGameInstance*			RGameInstance = RGAMEINSTANCE(this);
	checkf(RGameInstance != nullptr, TEXT("UUC_HeroInfo_Upgrade[RGameInstance Is Empty]"));

	FHERO* heroData = RGameInstance->RInventory->GetHeroWithUD(m_HeroUD);
	checkf(heroData != nullptr, TEXT("UUC_HeroInfo_Upgrade HeroObject Is Empty"));

	FCharacterTableInfo* currentTable = UTableManager::GetInstancePtr()->GetCharacterRow(heroData->heroId);
	checkf(currentTable != nullptr, TEXT("UUC_HeroInfo_Upgrade CharacterTable Is Empty"));

	//if (currentTable->Grade == 10)
	UUIFunctionLibrary::GoUIPage(EUIPageEnum::UIPage_HeroManagement_Renewal);
}

void UUP_HeroInfo_Upgrade::OnClickBack()
{
	if (IsValid(UpgradeHeroInfo))
		UpgradeHeroInfo->OnClickClose();
}
