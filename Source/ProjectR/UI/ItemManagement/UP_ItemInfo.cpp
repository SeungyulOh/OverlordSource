// Fill out your copyright notice in the Description page of Project Settings.

#include "ProjectR.h"
#include "UP_ItemInfo.h"

#include "UI/ItemManagement/UC_HeroItemInfo.h"
#include "UI/HeroInfo/HeroManager/UP_HeroManager.h"
#include "UI/RWidgetManager.h"

#include "Lobby/IndoorLobbyStateActor.h"

#include "Global/RGameInstance.h"


#include "ContentGameMode/Modes/TowerLobbyGameMode.h"
#include "UIFunctionLibrary.h"
#include "UP_GenericScrollview.h"



void UUP_ItemInfo::NativeConstruct()
{
	Super::NativeConstruct();

	if (ItemInfo_L && ItemInfo_R) 
	{
		ItemInfo_L->IsBagMode = false;
		ItemInfo_R->IsBagMode = false;

		ItemInfo_L->bLeft = true;
		ItemInfo_L->EquippedByCurrentHero = true;

		UUP_HeroManager* UP_heroManager = Cast<UUP_HeroManager>(RGAMEINSTANCE(this)->RWidgetManager->GetUserWidget(EWidgetBluePrintEnum::WBP_Heromanagement_Renewal));
		if (UP_heroManager)
		{
			ItemInfo_L->OnItemInventoryUpdatedDelegate.Unbind();
			ItemInfo_L->OnItemInventoryUpdatedDelegate.BindUObject(UP_heroManager, &UUP_HeroManager::OnItemInfoItemInventoryUpdated, false);
			ItemInfo_R->OnItemInventoryUpdatedDelegate.BindUObject(UP_heroManager, &UUP_HeroManager::OnItemInfoItemInventoryUpdated, true);
		}
	}
}

void UUP_ItemInfo::NativeDestruct()
{
	if (IsValid(ItemInfo_L))
	{
		ItemInfo_L->SetVisibility(ESlateVisibility::Collapsed);
		ItemInfo_L->OnItemInventoryUpdatedDelegate.Unbind();
	}
	if (IsValid(ItemInfo_R))
	{
		ItemInfo_R->SetVisibility(ESlateVisibility::Collapsed);
		ItemInfo_R->OnItemInventoryUpdatedDelegate.Unbind();
	}

	Super::NativeDestruct();
}

void UUP_ItemInfo::SetItemInfo_Left(FString InItemUD)
{
	ATowerLobbyGameMode* TowerGameMode = Cast<ATowerLobbyGameMode>(UGameplayStatics::GetGameMode(RGAMEINSTANCE(this)->GetWorld()));
	if (TowerGameMode)
	{
		AIndoorLobbyStateActor* IndoorLobbyStateActor = Cast<AIndoorLobbyStateActor>(TowerGameMode->GetLobbyStateActor(ETowerLobbyStateEnum::Indoor));
		if (IndoorLobbyStateActor)
		{
			ItemInfo_L->SetVisibility(ESlateVisibility::Visible);

			ItemInfo_L->ItemUD = InItemUD;
			ItemInfo_L->CurrentHeroUD = IndoorLobbyStateActor->CurrentHeroUD;
			ItemInfo_L->InvalidateData();
		}
	}
	
}

void UUP_ItemInfo::SetItemInfo_Right(FString InItemUD)
{
	ATowerLobbyGameMode* TowerGameMode = Cast<ATowerLobbyGameMode>(UGameplayStatics::GetGameMode(RGAMEINSTANCE(this)->GetWorld()));
	if (TowerGameMode)
	{
		AIndoorLobbyStateActor* IndoorLobbyStateActor = Cast<AIndoorLobbyStateActor>(TowerGameMode->GetLobbyStateActor(ETowerLobbyStateEnum::Indoor));
		if (IndoorLobbyStateActor)
		{
			ItemInfo_R->SetVisibility(ESlateVisibility::Visible);

			ItemInfo_R->ItemUD = InItemUD;
			ItemInfo_R->CurrentHeroUD = IndoorLobbyStateActor->CurrentHeroUD;
			ItemInfo_R->InvalidateData();
		}
	}
}