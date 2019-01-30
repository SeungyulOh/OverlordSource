// Fill out your copyright notice in the Description page of Project Settings.

#include "ProjectR.h"

#include "Global/RGameInstance.h"
#include "Global/EventManager.h"
#include "Global/TableManager.h"

#include "GameInfo/RInventory.h"

#include "UP_HeroInfo_Strong.h"

#include "UI/RWidgetManager.h"
#include "UP_GenericScrollview.h"
#include "UIFunctionLibrary.h"
#include "../HeroManager/UP_HeroManager.h"
#include "Lobby/IndoorLobbyStateActor.h"
#include "ContentGameMode/Modes/TowerLobbyGameMode.h"
#include "Network/PacketFunctionLibrary.h"

void UUP_HeroInfo_Strong::NativeConstruct()
{
	Super::NativeConstruct();

	if (Button_Back)
	{
		Button_Back->OnClicked.Clear();
		Button_Back->OnClicked.AddDynamic(this, &UUP_HeroInfo_Strong::OnClick_BackButton);
	}
	Init();
}

void UUP_HeroInfo_Strong::NativeDestruct()
{
	Super::NativeDestruct();
}

void UUP_HeroInfo_Strong::Init()
{
	URGameInstance* RGameInstance = RGAMEINSTANCE(this);
	checkf(RGameInstance != nullptr, TEXT("UUP_HeroInfo_Upgrade[RGameInstance Is Empty]"));
	checkf(RGameInstance->EventManager != nullptr, TEXT("UUP_HeroInfo_Upgrade[RInventory Is Empty]"));

	RGameInstance->EventManager->OnHeroEnchant.Clear();
	RGameInstance->EventManager->OnHeroEnchant.AddDynamic(this, &UUP_HeroInfo_Strong::OnHeroEnchantResult);

	RGameInstance->EventManager->OnHeroStatApply.Clear();
	RGameInstance->EventManager->OnHeroStatApply.AddDynamic(this, &UUP_HeroInfo_Strong::OnReceiveStatApplyResult);
}

void UUP_HeroInfo_Strong::SetHero(FString HeroUD)
{
	URGameInstance* RGameInstance = RGAMEINSTANCE(this);
	checkf(RGameInstance != nullptr, TEXT("UUC_HeroMaterial_Popup[RGameInstance Is Empty]"));
	checkf(RGameInstance->RInventory != nullptr, TEXT("UUC_HeroMaterial_Popup[RInventory Is Empty]"));
	m_HeroUD = HeroUD;

	m_Hero = RGameInstance->RInventory->GetHeroWithUD(m_HeroUD);

	if (IsValid(EnchantInfo))
		EnchantInfo->SetHero(m_HeroUD);

	m_nPrevPoint = m_Hero->enchantPoint;
// 	if (IsValid(EnchantMaterialList))
// 		EnchantMaterialList->SetHero(m_HeroUD);
}

void UUP_HeroInfo_Strong::Do_Consume()
{
	/*Get Enchant Points*/
	URGameInstance* RGameInstance = RGAMEINSTANCE(this);
	checkf(RGameInstance != nullptr, TEXT("UC_Hero_Strong.cpp : [RGameInstance Is Empty]"));
	checkf(RGameInstance->RInventory != nullptr, TEXT("UC_Hero_Strong.cpp : [RInventory Is nullptr]"));

	ATowerLobbyGameMode* TowerGameMode = Cast<ATowerLobbyGameMode>(UGameplayStatics::GetGameMode(RGameInstance->GetWorld()));
	checkf(TowerGameMode != nullptr, TEXT("UC_Hero_Strong.cpp : [TowerGameMode Is nullptr]"));

	AIndoorLobbyStateActor* IndoorLobbyStateActor = Cast<AIndoorLobbyStateActor>(TowerGameMode->GetLobbyStateActor(ETowerLobbyStateEnum::Indoor));
	checkf(IndoorLobbyStateActor != nullptr, TEXT("UC_Hero_Strong.cpp : [IndoorLobbyStateActor Is nullptr]"));

	UTableManager* TableManager = UTableManager::GetInstancePtr();
	checkf(TableManager != nullptr, TEXT("UC_Hero_Strong.cpp : [TableManager Is nullptr]"));

	FHERO* hero = RGameInstance->RInventory->GetHeroWithUD(IndoorLobbyStateActor->CurrentHeroUD);
	FCharacterTableInfo* CharacterTable = TableManager->GetCharacterRow(hero->heroId);
	FCharacterCostEnchantGrade* EnchantCostInfo = TableManager->GetCharacterCostEnchantGradeInfo(FName(*FString::FromInt(CharacterTable->Grade)));
	int32 EnchantCount = 0;
	for (int32 Num : hero->enchantStat)
		EnchantCount += Num;

	if (hero->enchantPoint + EnchantCount >= FCString::Atoi(*EnchantCostInfo->MaxEnchantPoint.ToString()))
	{
		UUIFunctionLibrary::ShowCommonPopupSplash(UUtilFunctionLibrary::GetLocalizedString(EStringTableType::VE_UI, COMMONSTRING_NOTIFY),
			UUtilFunctionLibrary::GetLocalizedString(EStringTableType::VE_UI, FName(TEXT("UI_HeroEnchant_SplashPopUp_Meassage01"))));
		return;
	}

	TArray<FString>	SelectSpiritUDList;
	SelectSpiritUDList.Empty();

	UUP_GenericScrollview* GenericScrollView = UUIFunctionLibrary::GetGenericScrollview();

	TArray<URScrollItemData*> itemdataList = GenericScrollView->Variables.ScrollView->GetSelectedScrollItemData();
	for (size_t i = 0; i < itemdataList.Num(); ++i)
	{
		UUC_GenericScrollItemData* itemdata = Cast<UUC_GenericScrollItemData>(itemdataList[i]);
		if (itemdata)
		{
			SelectSpiritUDList.AddUnique(itemdata->ItemUD);
		}
	}
	/*Selected Spirit Ready*/
	//UUP_HeroInfo_Strong* heroenchantpage = Cast<UUP_HeroInfo_Strong>(RGameInstance->RWidgetManager->GetUserWidget(EWidgetBluePrintEnum::WBP_HeroReinforce));
	FCharacterCostEnchant* CharacterCostEnchant = UTableManager::GetInstancePtr()->GetCharacterCostEnchant(TEXT("1"));

	m_SpiritPercentList.Reset();
	m_HeroID.Reset();

	for (const FString& SelectedSpiritUD : SelectSpiritUDList)
	{
		FHERO* Spirit = RGameInstance->RInventory->GetHeroWithUD(SelectedSpiritUD);
		FCharacterTableInfo* CharacterTableInfo = UTableManager::GetInstancePtr()->GetCharacterRow(Spirit->heroId);
		if (CharacterTableInfo->Grade == 1)
			m_SpiritPercentList.Emplace(CharacterCostEnchant->SpiritGrade1 / 10);
		else if (CharacterTableInfo->Grade == 2)
			m_SpiritPercentList.Emplace(CharacterCostEnchant->SpiritGrade2 / 10);
		else if (CharacterTableInfo->Grade == 3)
			m_SpiritPercentList.Emplace(CharacterCostEnchant->SpiritGrade3 / 10);
		else if (CharacterTableInfo->Grade == 4)
			m_SpiritPercentList.Emplace(CharacterCostEnchant->SpiritGrade4 / 10);
		else if (CharacterTableInfo->Grade == 5)
			m_SpiritPercentList.Emplace(CharacterCostEnchant->SpiritGrade5 / 10);
		else if (CharacterTableInfo->Grade == 6)
			m_SpiritPercentList.Emplace(CharacterCostEnchant->SpiritGrade6 / 10);

		m_HeroID.Emplace(Spirit->heroId);
	}

	if (SelectSpiritUDList.GetAllocatedSize() <= 0)
	{
		UUIFunctionLibrary::ShowCommonPopupSplash(UUtilFunctionLibrary::GetLocalizedString(EStringTableType::VE_UI, COMMONSTRING_NOTIFY),
			UUtilFunctionLibrary::GetLocalizedString(EStringTableType::VE_UI, FName(TEXT("UI_HeroEnchant_SplashPopUp_Meassage04"))));
		return;
	}
	/*if (CharacterCostEnchant->Cost_Value > RGameInstance->RInventory->GetGold())
	{
		UUIFunctionLibrary::ShowCommonPopupSplash(UUtilFunctionLibrary::GetLocalizedString(EStringTableType::VE_UI, COMMONSTRING_NOTIFY),
			UUtilFunctionLibrary::GetLocalizedString(EStringTableType::VE_UI, FName(TEXT("USK_up_ItemManage_tip07"))));
		return;
	}*/

	UPacketFunctionLibrary::INVENTORY_HERO_ENCHANT_CONSUME_RQ(IndoorLobbyStateActor->CurrentHeroUD, SelectSpiritUDList);
}

void UUP_HeroInfo_Strong::OnClick_HeroIcon(FString InHeroUD)
{
	URGameInstance* RGameInstance = RGAMEINSTANCE(this);
	checkf(RGameInstance != nullptr, TEXT("UUC_HeroMaterial_Popup[RGameInstance Is Empty]"));
	checkf(RGameInstance->RInventory != nullptr, TEXT("UUC_HeroMaterial_Popup[RInventory Is Empty]"));
	m_HeroUD = InHeroUD;

	m_Hero = RGameInstance->RInventory->GetHeroWithUD(m_HeroUD);

	if (IsValid(EnchantInfo))
		EnchantInfo->SetHero(m_HeroUD);
	if (IsValid(EnchantMaterialList))
		EnchantMaterialList->SetHero(m_HeroUD);

	EnchantMaterialList->SetCost(0);
}

void UUP_HeroInfo_Strong::OnSelectedItem_HeroEnchant_ScrollView(URScrollItem* SelectedItem)
{
	URGameInstance* RGameInstance = RGAMEINSTANCE(this);
	checkf(RGameInstance != nullptr, TEXT("UUC_HeroMaterial_Popup[RGameInstance Is Empty]"));
	checkf(RGameInstance->RInventory != nullptr, TEXT("UUC_HeroMaterial_Popup[RInventory Is Empty]"));

	m_SpiritPercentList.Reset();

	FHERO*		CurrenHeroData	=	RGameInstance->RInventory->GetHeroWithUD(m_HeroUD);
	int32		sum				=	0;
	int32		Cost			=	0;

	if (CurrenHeroData == nullptr)
		return;

	for (int32 p : CurrenHeroData->enchantStat)
		sum += p;

	TArray<FString>			SelectSpiritUDList;
	SelectSpiritUDList.Empty();
	m_HeroID.Empty();
	//if(IsValid(EnchantMaterialList))
	//	if(EnchantMaterialList->UC_HeroScrollView)
	//		EnchantMaterialList->UC_HeroScrollView->GetSelectedHeroUDs(SelectSpiritUDList);

	m_nPrevPoint = CurrenHeroData->enchantPoint;
	
	int32 EnchantPosition = ((m_nPrevPoint + sum)/5)+1;

	if (EnchantPosition <= 0 || EnchantPosition > 6)
		return;

	FCharacterCostEnchant* CharacterCostEnchant = UTableManager::GetInstancePtr()->GetCharacterCostEnchant(FName(*FString::FromInt(EnchantPosition)));
	if (CharacterCostEnchant->EnchantPoint_Min <= (m_nPrevPoint + sum) && CharacterCostEnchant->EnchantPoint_Max >= (m_nPrevPoint + sum))
	{
		for (FString& SelectedSpiritUD : SelectSpiritUDList)
		{
			FHERO* Spirit = RGameInstance->RInventory->GetHeroWithUD(SelectedSpiritUD);
			FCharacterTableInfo* CharacterTableInfo = UTableManager::GetInstancePtr()->GetCharacterRow(Spirit->heroId);
			if (CharacterTableInfo->Grade == 1)
			{
				m_SpiritPercentList.Emplace(CharacterCostEnchant->SpiritGrade1/10);
			}
			else if (CharacterTableInfo->Grade == 2)
			{
				m_SpiritPercentList.Emplace(CharacterCostEnchant->SpiritGrade2/10);
			}
			else if (CharacterTableInfo->Grade == 3)
			{
				m_SpiritPercentList.Emplace(CharacterCostEnchant->SpiritGrade3/10);
			}
			else if (CharacterTableInfo->Grade == 4)
			{
				m_SpiritPercentList.Emplace(CharacterCostEnchant->SpiritGrade4/10);
			}
			else if (CharacterTableInfo->Grade == 5)
			{
				m_SpiritPercentList.Emplace(CharacterCostEnchant->SpiritGrade5/10);
			}
			m_HeroID.Emplace(Spirit->heroId);
		}
	}
	if (IsValid(EnchantMaterialList))
		EnchantMaterialList->SetCost(Cost);

	EnchantInfo->SetMaterial(SelectSpiritUDList, m_SpiritPercentList);
}

void UUP_HeroInfo_Strong::OnHeroEnchantResult(TArray<bool>& Result, int32 InCurrentPoint)
{
	if (IsValid(Result_Popup))
	{
		Result_Popup->SetVisibility(ESlateVisibility::Visible);
		int32 nGetPoint = (InCurrentPoint - m_nPrevPoint);
		if (nGetPoint < 0)
			nGetPoint = 0;
		Result_Popup->SetEnchantResult(Result, m_HeroID, m_SpiritPercentList, nGetPoint);
	}

	UUIFunctionLibrary::GetGenericScrollview()->Appear(false);

	URGameInstance* RGameInstance = RGAMEINSTANCE(this);
	FHERO* heroData = RGameInstance->RInventory->GetHeroWithUD(m_HeroUD);
	int32 alreadyUsePoint = 0;
	for(int32 i = 0; i < heroData->enchantStat.Num(); i++)
		alreadyUsePoint += heroData->enchantStat[i];

	SetHero(m_HeroUD);
}

void UUP_HeroInfo_Strong::OnReceiveStatApplyResult()
{
	SetHero(m_HeroUD);

	EnchantPoint_Apply->SetEnchant(EnchantInfo->m_PrevStat ,m_Hero->enchantStat);
	EnchantPoint_Apply->SetVisibility(ESlateVisibility::Visible);

	UUIFunctionLibrary::GetGenericScrollview()->Appear(false);

	UUP_HeroManager* heromanager = Cast<UUP_HeroManager>(RGAMEINSTANCE(this)->RWidgetManager->GetUserWidget(EWidgetBluePrintEnum::WBP_Heromanagement_Renewal));
	if (heromanager)
		heromanager->OnClick_HeroIcon(m_HeroUD);
}

void UUP_HeroInfo_Strong::OnClick_BackButton()
{
	UUP_GenericScrollview* GenericScrollView =  UUIFunctionLibrary::GetGenericScrollview();

	if (!GenericScrollView)
		return;

	if (GenericScrollView->isInsideViewport())
		GenericScrollView->Appear(false);

	RGAMEINSTANCE(this)->RWidgetManager->DetachUIPage(EUIPageEnum::UIPage_HeroReinforce);
}
