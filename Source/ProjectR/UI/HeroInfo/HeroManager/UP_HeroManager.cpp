// Fill out your copyright notice in the Description page of Project Settings.

#include "ProjectR.h"
#include "UP_HeroManager.h"

#include "Global/RGameInstance.h"
#include "Global/EventManager.h"
#include "Global/NotifyManager.h"
#include "Global/ForceTutorialManager.h"
#include "Global/ContentsUnLockManager.h"

#include "UI/RWidgetManager.h"
#include "UI/ItemManagement/UC_InventoryScrollView.h"
#include "UI/HeroManagement/UC_SkillManagement.h"
#include "UI/HeroInfo/HeroUpgrade/UP_HeroInfo_Upgrade.h"
#include "UI/HeroInfo/HeroStrong/UP_HeroInfo_Strong.h"

#include "Utils/UIFunctionLibrary.h"
#include "Utils/Client_BattleFunctionLibrary.h"
#include "Utils/SharedUtils/Renewal_BattleFunctionLibrary.h"

#include "GameInfo/RInventory.h"

#include "ContentGameMode/Modes/TowerLobbyGameMode.h"

#include "Utils/GamePlayBluePrintFunction.h"

#include "UP_GenericScrollview.h"

#include "Lobby/IndoorLobbyStateActor.h"
#include "Global/TableManager.h"


void UUP_HeroManager::NativeConstruct()
{
	Super::NativeConstruct();

	Init();

	UUIFunctionLibrary::GetGenericScrollview()->Reset();
}

void UUP_HeroManager::NativeDestruct()
{
	auto Widget = UUIFunctionLibrary::GetGenericScrollview();
	if (Widget)
	{
		Widget->Appear(false);
	}

	Super::NativeDestruct();
}

void UUP_HeroManager::Init()
{
	URGameInstance* RGameInstance = RGAMEINSTANCE(this);

	checkf(RGameInstance != nullptr, TEXT("UUP_HeroManager[RGameInstance Is Empty]"));
	checkf(RGameInstance->EventManager != nullptr, TEXT("UUP_HeroManager[RInventory Is Empty]"));

	RGameInstance->EventManager->OnHeroLevelUp.RemoveDynamic(this, &UUP_HeroManager::OnRewardPopup);
	RGameInstance->EventManager->OnHeroGrind.RemoveDynamic(this, &UUP_HeroManager::OnGrindPopup);
	RGameInstance->EventManager->OnHeroLock.RemoveDynamic(this, &UUP_HeroManager::OnHeroLock);
	RGameInstance->EventManager->OnHeroLevelUp.AddDynamic(this, &UUP_HeroManager::OnRewardPopup);
	RGameInstance->EventManager->OnHeroGrind.AddDynamic(this, &UUP_HeroManager::OnGrindPopup);
	RGameInstance->EventManager->OnHeroLock.AddDynamic(this, &UUP_HeroManager::OnHeroLock);
	RGameInstance->EventManager->OnHeroUpgrade.RemoveDynamic(this, &UUP_HeroManager::OnHeroUpgrade);
	RGameInstance->EventManager->OnHeroUpgrade.AddDynamic(this, &UUP_HeroManager::OnHeroUpgrade);
	
	if (IsValid(Hero_Info))
	{
		Hero_Info->OnItemInventoryUpdatedDelegate.RemoveAll(this);
		Hero_Info->OnItemInventoryUpdatedDelegate.AddUObject(this, &UUP_HeroManager::OnItemInventoryUpdated);

		if (IsValid(Hero_Info->DetailStat_Button))
		{
			Hero_Info->DetailStat_Button->OnClicked.RemoveDynamic(this, &UUP_HeroManager::OnClick_DetailStat);
			Hero_Info->DetailStat_Button->OnClicked.AddDynamic(this, &UUP_HeroManager::OnClick_DetailStat);
		}

		if (IsValid(Hero_Info->Button_Grind))
		{
			Hero_Info->Button_Grind->OnClicked.RemoveDynamic(this, &UUP_HeroManager::OnClick_Grind);
			Hero_Info->Button_Grind->OnClicked.AddDynamic(this, &UUP_HeroManager::OnClick_Grind);
		}

		if (IsValid(Hero_Info->Button_LevelUp))
		{
			Hero_Info->Button_LevelUp->OnClicked.RemoveDynamic(this, &UUP_HeroManager::OnClick_LevelUp);
			Hero_Info->Button_LevelUp->OnClicked.AddDynamic(this, &UUP_HeroManager::OnClick_LevelUp);
		}
		if (IsValid(Hero_Info->Button_Upgrade))
		{
			Hero_Info->Button_Upgrade->OnClicked.RemoveDynamic(this, &UUP_HeroManager::OnClick_Upgrade);
			Hero_Info->Button_Upgrade->OnClicked.AddDynamic(this, &UUP_HeroManager::OnClick_Upgrade);
		}
		if (IsValid(Hero_Info->Button_Enchant))
		{
			Hero_Info->Button_Enchant->OnClicked.RemoveDynamic(this, &UUP_HeroManager::OnClick_Enchant);
			Hero_Info->Button_Enchant->OnClicked.AddDynamic(this, &UUP_HeroManager::OnClick_Enchant);
		}
		if (IsValid(Hero_Info->Button_Skills))
		{
			Hero_Info->Button_Skills->OnClicked.RemoveDynamic(this, &UUP_HeroManager::OnClick_Skills);
			Hero_Info->Button_Skills->OnClicked.AddDynamic(this, &UUP_HeroManager::OnClick_Skills);
		}
	}
	
		
	if (IsValid(LevelUpPopup))
	{
		if (IsValid(LevelUpPopup->Button_LevelUp))
		{
			LevelUpPopup->Button_LevelUp->OnClicked.RemoveDynamic(this, &UUP_HeroManager::OnClickLevelUpPopup);
			LevelUpPopup->Button_LevelUp->OnClicked.AddDynamic(this, &UUP_HeroManager::OnClickLevelUpPopup);
		}
			
	}
	
	if (IsValid(ChangeButton))
	{
		ChangeButton->OnClicked.Clear();
		ChangeButton->OnClicked.AddDynamic(this, &UUP_HeroManager::OnClickChange);
	}
	if (IsValid(BagButton))
	{
		BagButton->OnClicked.Clear();
		BagButton->OnClicked.AddDynamic(this, &UUP_HeroManager::OnClickBag);
	}

	

	/*Which Hero?*/
	AIndoorLobbyStateActor* LobbyStateActor = Cast<AIndoorLobbyStateActor>(RGameInstance->GetTowerLobbyGameMode()->GetLobbyStateActor(ETowerLobbyStateEnum::Indoor));
	if (LobbyStateActor)
	{
		OnClick_HeroIcon(LobbyStateActor->CurrentHeroUD);
	}
	/**/

	if (RGameInstance->ForceTutorialManager && RGameInstance->ContentsUnLockManager)
	{
		if(!RGameInstance->ContentsUnLockManager->IsFirstEnter(EFirstCheckContentsType::FirstStageFail))
			RGameInstance->ForceTutorialManager->CheckUnlockTutorial(EFirstCheckContentsType::FirstHeroManagement);
	}
}

void UUP_HeroManager::SetHeroInfo()
{
	URGameInstance*			Game	=	RGAMEINSTANCE(this);
	URInventory*			Inven	=	Game->RInventory;

	checkf(Game != nullptr, TEXT("UUP_HeroManager[RGameInstance Is Empty]"));
	checkf(Inven != nullptr, TEXT("UUP_HeroManager[RInventory Is Empty]"));

	if (m_CurHeroUD.IsEmpty())
		return;

	m_Hero			=	Inven->GetHeroWithUD(m_CurHeroUD);
	UUIFunctionLibrary::GetHeroIconInfoWithUD(m_PrevHeroIcon, m_CurHeroUD, EHeroOwnershipType::VE_Me);

	if (m_Hero == nullptr)
		return;

	m_UIBattleInfo	=	URenewal_BattleFunctionLibrary::MakeCharacteBattleInfoFromInven(m_CurHeroUD);
	m_UITableInfo	=	URenewal_BattleFunctionLibrary::GetCharacterTableInfo(ECharacterCategoryTypeEnum::VE_Hero, m_Hero->heroId);

	if (IsValid(Text_Name))
	{
		FText HeroName = UUtilFunctionLibrary::GetLocalizedString(EStringTableType::VE_Hero, m_UITableInfo->DisplayName);
		Text_Name->SetText(HeroName);
	}

	if (IsValid(Text_Nick))
	{
		FText HeroNickName = UUtilFunctionLibrary::GetLocalizedString(EStringTableType::VE_Hero, m_UITableInfo->Nick);
		Text_Nick->SetText(HeroNickName);
	}

	int32 HeroAttackPower = UClient_BattleFunctionLibrary::GetHeroCombatStat(m_CurHeroUD);

	if (IsValid(Text_AttackPower))
		Text_AttackPower->SetText(FText::Format(FText::FromString(TEXT("+ {0}")), HeroAttackPower));

	if (IsValid(Text_MinLevel))
		Text_MinLevel->SetText(FText::AsNumber(m_Hero->level));

	if (IsValid(Text_MaxLevel))
		Text_MaxLevel->SetText(FText::AsNumber(m_UITableInfo->MaxLevel));

	int32		sum = 0;

	for (int32 p : m_Hero->enchantStat)
		sum += p;

	if (IsValid(Text_ReinForce))
		Text_ReinForce->SetText(FText::Format(FText::FromString(TEXT("+ {0}")), sum));



	/*Star Settings*/
	if (IsValid(UC_heroStar))
		UC_heroStar->SetStar(m_CurHeroUD);
}

bool UUP_HeroManager::IsLock()
{
	if (m_Hero == nullptr)
		return false;

	return m_Hero->locked;
}

bool UUP_HeroManager::IsMaxLevel()
{
	if (m_UITableInfo && m_Hero)
	{
		if (m_Hero->level >= m_UITableInfo->MaxLevel)
			return true;
	}
	return false;
}

void UUP_HeroManager::OnClick_HeroIcon(FString InHeroUD)
{
	URGameInstance* RGameInstance = RGAMEINSTANCE(this);

	checkf(RGameInstance != nullptr, TEXT("UUP_HeroManager[RGameInstance Is Empty]"));
	checkf(RGameInstance->RInventory != nullptr, TEXT("UUP_HeroManager[RInventory Is Empty]"));
	checkf(RGameInstance->NotifyManager != nullptr, TEXT("UUP_HeroManager[NotifyManager Is Empty]"));

	//OnButtonFilterShowClicked();

	m_CurHeroUD = InHeroUD;
	BeforeHeroUpgradeData = *(RGAMEINSTANCE(this)->RInventory->GetHeroWithUD(m_CurHeroUD));
	ATowerLobbyGameMode* TowerLobbyGameMode = RGameInstance->GetTowerLobbyGameMode();
	if (TowerLobbyGameMode)
	{
		AIndoorLobbyStateActor* IndoorLobbyStateActor = TowerLobbyGameMode->IndoorLobbyStateAct;
		if (IsValid(IndoorLobbyStateActor))
		{
			IndoorLobbyStateActor->ChangeHero_Owned(m_CurHeroUD);
			IndoorLobbyStateActor->PlayHeroLobbySocialAnimation();
		}
	}

	//RGameInstance->NotifyManager->CheckHeroSkillUpgradeEach(m_CurHeroUD);

	if (IsValid(UC_HeroItems))
	{
		UC_HeroItems->SetHeroUD(m_CurHeroUD);
		UC_HeroItems->InvalidateData();
	}
	if (IsValid(Hero_Info))
		Hero_Info->SetHero(m_CurHeroUD);

	SetHeroInfo();

	/*if (IsValid(LevelUpPopup))
	{
		if(LevelUpPopup->IsVisible())
			LevelUpPopup->SetHero(m_CurHeroUD);
	}
	if (IsValid(HeroStatInfo))
	{ 
		if (HeroStatInfo->IsVisible())
			HeroStatInfo->SetStatInfo(m_UIBattleInfo);
	}*/
}


void UUP_HeroManager::OnClick_DetailStat()
{
	if (IsValid(HeroStatInfo))
	{
		HeroStatInfo->SetVisibility(ESlateVisibility::Visible);
		HeroStatInfo->SetStatInfo(m_UIBattleInfo);
	}
}

void UUP_HeroManager::OnClick_Grind()
{
	URGameInstance*		RGameInstance = RGAMEINSTANCE(this);
	URWidgetManager*	RWidgetManager = RGameInstance->RWidgetManager;

	if (IsLock())
	{
		UUIFunctionLibrary::ShowCommonPopupSplash(UUtilFunctionLibrary::GetLocalizedString(EStringTableType::VE_UI, COMMONSTRING_NOTIFY),
			UUtilFunctionLibrary::GetLocalizedString(EStringTableType::VE_UI, FName(TEXT("UI_HeroManagement_CannotGrind"))));
		return;
	}
	if (IsValid(LevelUpPopup))
		LevelUpPopup->SetVisibility(ESlateVisibility::Collapsed);

	RWidgetManager->AttachUIPage(EUIPageEnum::UIPage_GrindPopup);

	UUC_Popup_HeroInfo_BreakUp* GrindPopup = Cast<UUC_Popup_HeroInfo_BreakUp>(RWidgetManager->GetUserWidget(EWidgetBluePrintEnum::WBP_GrindPopup));

	if (IsValid(GrindPopup))
	{ 
		GrindPopup->SetVisibility(ESlateVisibility::Visible);
		GrindPopup->SetInfo(m_CurHeroUD);
	}
}

void UUP_HeroManager::OnClick_LevelUp()
{
	if (IsMaxLevel())
	{
		URGameInstance*		RGameInstance = RGAMEINSTANCE(this);
		URWidgetManager*	RWidgetManager = RGameInstance->RWidgetManager;
		checkf(RGameInstance != nullptr, TEXT("UUP_HeroManager[RGameInstance Is Empty]"));
		checkf(RWidgetManager != nullptr, TEXT("UUP_HeroManager[RWidgetManager Is Empty]"));

		UUIFunctionLibrary::ShowCommonPopupSplash(UUtilFunctionLibrary::GetLocalizedString(EStringTableType::VE_UI, COMMONSTRING_NOTIFY),
			UUtilFunctionLibrary::GetLocalizedString(EStringTableType::VE_UI, FName(TEXT("UI_HeroLevelUp_SplashPopUp_Message01"))));
		return;
	}
	if (IsValid(LevelUpPopup))
	{
		LevelUpPopup->SetVisibility(ESlateVisibility::Visible);
		LevelUpPopup->SetHero(m_CurHeroUD);
	}
}

void UUP_HeroManager::OnClick_Upgrade()
{
	URGameInstance*		RGameInstance = RGAMEINSTANCE(this);
	URWidgetManager*	RWidgetManager = RGameInstance->RWidgetManager;
	checkf(RGameInstance != nullptr, TEXT("UUP_HeroManager[RGameInstance Is Empty]"));
	checkf(RWidgetManager != nullptr, TEXT("UUP_HeroManager[RWidgetManager Is Empty]"));

	if (m_UITableInfo == nullptr)
		return;

	
	if (m_UITableInfo->Grade == MaxGrade)
	{
		UUIFunctionLibrary::ShowCommonPopupSplash(UUtilFunctionLibrary::GetLocalizedString(EStringTableType::VE_UI, COMMONSTRING_NOTIFY),
			UUtilFunctionLibrary::GetLocalizedString(EStringTableType::VE_UI, FName(TEXT("UI_Max_Hero_Not_Promote_Msg"))));
		return;
	}
	

	RGAMEINSTANCE(this)->RWidgetManager->AttachUIPage(EUIPageEnum::UIPage_HeroRatingUp);
	UUP_HeroInfo_Upgrade* heroUpgrade = Cast<UUP_HeroInfo_Upgrade>(RGAMEINSTANCE(this)->RWidgetManager->GetUserWidget(EWidgetBluePrintEnum::WBP_HeroRatingUp));
	if (heroUpgrade)
	{
		UUP_GenericScrollview* genericScrollview = UUIFunctionLibrary::GetGenericScrollview();
		if (genericScrollview)
		{
			genericScrollview->SetState(EScrollviewType::VE_HERO_UPGRADE);
			//genericScrollview->Variables.DerivedPopup.AddUnique(heroUpgrade);
			//genericScrollview->Appear(true);
		}
	}
}

void UUP_HeroManager::OnClick_Enchant()
{
	URGameInstance*		RGameInstance = RGAMEINSTANCE(this);
	URWidgetManager*	RWidgetManager = RGameInstance->RWidgetManager;
	checkf(RGameInstance != nullptr, TEXT("UUP_HeroManager[RGameInstance Is Empty]"));
	checkf(RWidgetManager != nullptr, TEXT("UUP_HeroManager[RWidgetManager Is Empty]"));

	FCharacterTableInfo* CTI = UTableManager::GetInstancePtr()->GetCharacterRow(m_Hero->heroId);

	int32 EnchantCount = 0;
	if (m_Hero == nullptr)
		return;
	for (int32 Num : m_Hero->enchantStat)
		EnchantCount += Num;

	int32 MaxEnchantCount = 0;
	FCharacterCostEnchantGrade* EnchantCostInfo = UTableManager::GetInstancePtr()->GetCharacterCostEnchantGradeInfo(FName(*FString::FromInt(CTI->Grade)));
	MaxEnchantCount = FCString::Atoi(*(EnchantCostInfo->MaxEnchantPoint.ToString()));

	if (EnchantCount >= MaxEnchantCount)
	{
		UUIFunctionLibrary::ShowCommonPopupSplash(UUtilFunctionLibrary::GetLocalizedString(EStringTableType::VE_UI, COMMONSTRING_NOTIFY),
			UUtilFunctionLibrary::GetLocalizedString(EStringTableType::VE_UI, FName(TEXT("UI_HeroEnchant_SplashPopUp_Meassage01"))));
		return;
	}


	RGAMEINSTANCE(this)->RWidgetManager->AttachUIPage(EUIPageEnum::UIPage_HeroReinforce);
	UUP_HeroInfo_Strong* Upgrade = Cast<UUP_HeroInfo_Strong>(RWidgetManager->GetUserWidgetAutoCreate(EWidgetBluePrintEnum::WBP_HeroReinforce));
	if (Upgrade)
	{
		Upgrade->SetHero(m_Hero->heroUD);
	}

	UUP_GenericScrollview* GenericScrollview = UUIFunctionLibrary::GetGenericScrollview();
	if (GenericScrollview)
	{
		GenericScrollview->SetState(EScrollviewType::VE_HERO_ENCHANT);
		GenericScrollview->Appear(true);
	}
}

void UUP_HeroManager::OnClick_Skills()
{
	URGameInstance*		RGameInstance = RGAMEINSTANCE(this);
	URWidgetManager*	RWidgetManager = RGameInstance->RWidgetManager;


	RWidgetManager->ChangeUIPage(EUIPageEnum::UIPage_SkillManagement);
	UUC_SkillManagement*	HeroSkill = Cast<UUC_SkillManagement>(RWidgetManager->GetUserWidget(EWidgetBluePrintEnum::WBP_SkillManagement));
	if (HeroSkill)
	{
		//HeroSkill->HeroList->SetSelectHeroUD(m_Hero->heroUD);
		HeroSkill->InvalidateByHeroUD(m_Hero->heroUD);
		HeroSkill->RefreshScrollItemData();
	}
}
//	LevelUp
void UUP_HeroManager::OnClickLevelUpPopup()
{
	URGameInstance*		RGameInstance = RGAMEINSTANCE(this);
	URWidgetManager*	RWidgetManager = RGameInstance->RWidgetManager;

	RWidgetManager->AttachUIPage(EUIPageEnum::UIPage_LevelUpCommonPopup);
	UUC_Popup_HeroInfo_LevelUp*	Popup_HeroInfo_LevelUp = Cast<UUC_Popup_HeroInfo_LevelUp>(RWidgetManager->GetUserWidget(EWidgetBluePrintEnum::WBP_LevelUpCommonPopup));

	if (IsValid(Popup_HeroInfo_LevelUp))
	{
		Popup_HeroInfo_LevelUp->SetVisibility(ESlateVisibility::Visible);
		if (IsValid(Popup_HeroInfo_LevelUp->Button_OK))
		{
			Popup_HeroInfo_LevelUp->Button_OK->OnClicked.RemoveDynamic(this, &UUP_HeroManager::OnClickLevelUpOk);
			Popup_HeroInfo_LevelUp->Button_OK->OnClicked.AddDynamic(this, &UUP_HeroManager::OnClickLevelUpOk);
		}
		if (IsValid(Popup_HeroInfo_LevelUp->Button_Cancel))
		{
			Popup_HeroInfo_LevelUp->Button_Cancel->OnClicked.RemoveDynamic(this, &UUP_HeroManager::OnClickLevelUpCancel);
			Popup_HeroInfo_LevelUp->Button_Cancel->OnClicked.AddDynamic(this, &UUP_HeroManager::OnClickLevelUpCancel);
		}
	}

}

void UUP_HeroManager::OnClickLevelUpOk()
{
	if (IsValid(LevelUpPopup))
	{
		URGameInstance*		RGameInstance = RGAMEINSTANCE(this);
		URWidgetManager*	RWidgetManager = RGameInstance->RWidgetManager;

		UUC_Popup_HeroInfo_LevelUp*	Popup_HeroInfo_LevelUp = Cast<UUC_Popup_HeroInfo_LevelUp>(RWidgetManager->GetUserWidget(EWidgetBluePrintEnum::WBP_LevelUpCommonPopup));

		if (IsValid(Popup_HeroInfo_LevelUp))
		{
			Popup_HeroInfo_LevelUp->SetVisibility(ESlateVisibility::Collapsed);
			if (IsValid(Popup_HeroInfo_LevelUp->Button_OK))
				Popup_HeroInfo_LevelUp->Button_OK->OnClicked.RemoveDynamic(this, &UUP_HeroManager::OnClickLevelUpOk);
			if (IsValid(Popup_HeroInfo_LevelUp->Button_Cancel))
				Popup_HeroInfo_LevelUp->Button_Cancel->OnClicked.RemoveDynamic(this, &UUP_HeroManager::OnClickLevelUpCancel);
			RWidgetManager->DetachUIPage(EUIPageEnum::UIPage_LevelUpCommonPopup);
		}

		if (LevelUpPopup->isLackOfMaterial())
		{
			UUIFunctionLibrary::ShowCommonPopupSplash(UUtilFunctionLibrary::GetLocalizedString(EStringTableType::VE_UI, COMMONSTRING_NOTIFY),
				UUtilFunctionLibrary::GetLocalizedString(EStringTableType::VE_UI, FName(TEXT("UI_HeroLevelUp_SplashPopUp_Message02"))));
			return;
		}

		int32 EnableLevel = m_UITableInfo->MaxLevel - m_Hero->level;
		int32 CurLevel = m_Hero->level + EnableLevel * FMath::Clamp<float>(LevelUpPopup->Slider_Level->GetValue(), 0, 1.0f);

		if (CurLevel == m_Hero->level)
		{
			UUIFunctionLibrary::ShowCommonPopupSplash(UUtilFunctionLibrary::GetLocalizedString(EStringTableType::VE_UI, COMMONSTRING_NOTIFY),
				UUtilFunctionLibrary::GetLocalizedString(EStringTableType::VE_UI, FName(TEXT("UI_HeroLevelUp_SplashPopUp_Message03"))));
			return;
		}

		UPacketFunctionLibrary::INVENTORY_HERO_LEVELUP_RQ(m_CurHeroUD, CurLevel);

		if (m_UITableInfo->MaxLevel == m_Hero->level)
			LevelUpPopup->SetVisibility(ESlateVisibility::Collapsed);
	}
}

void UUP_HeroManager::OnClickLevelUpCancel()
{
	URGameInstance*		RGameInstance = RGAMEINSTANCE(this);
	URWidgetManager*	RWidgetManager = RGameInstance->RWidgetManager;

	UUC_Popup_HeroInfo_LevelUp*	Popup_HeroInfo_LevelUp = Cast<UUC_Popup_HeroInfo_LevelUp>(RWidgetManager->GetUserWidget(EWidgetBluePrintEnum::WBP_LevelUpCommonPopup));

	if (IsValid(Popup_HeroInfo_LevelUp))
	{
		Popup_HeroInfo_LevelUp->SetVisibility(ESlateVisibility::Collapsed);
		if (IsValid(Popup_HeroInfo_LevelUp->Button_OK))
			Popup_HeroInfo_LevelUp->Button_OK->OnClicked.RemoveDynamic(this, &UUP_HeroManager::OnClickLevelUpOk);
		if (IsValid(Popup_HeroInfo_LevelUp->Button_Cancel))
			Popup_HeroInfo_LevelUp->Button_Cancel->OnClicked.RemoveDynamic(this, &UUP_HeroManager::OnClickLevelUpCancel);
		RWidgetManager->DetachUIPage(EUIPageEnum::UIPage_LevelUpCommonPopup);
	}
}

void UUP_HeroManager::OnClickChange()
{
	UUP_GenericScrollview* GenericScrollview = UUIFunctionLibrary::GetGenericScrollview();
	if (GenericScrollview)
	{
		bool bHeroInven = (GenericScrollview->Variables.State == EScrollviewType::VE_HERO_PROTECTED ||
			GenericScrollview->Variables.State == EScrollviewType::VE_HERO_NORMAL ||
			GenericScrollview->Variables.State == EScrollviewType::VE_SPIRIT);

		if (bHeroInven)
			GenericScrollview->Appear(false);
		else
		{
			bool isPrevHero = (GenericScrollview->Variables.PrevState == EScrollviewType::VE_HERO_PROTECTED ||
				GenericScrollview->Variables.PrevState == EScrollviewType::VE_HERO_NORMAL ||
				GenericScrollview->Variables.PrevState == EScrollviewType::VE_SPIRIT);

			if(GenericScrollview->Variables.PrevState != EScrollviewType::VE_END && isPrevHero)
				GenericScrollview->SetState(GenericScrollview->Variables.PrevState);
			else
			{
				TArray<FString> OutArray;
				RGAMEINSTANCE(this)->RInventory->GetProtectedHeros(OutArray);
				if(OutArray.Num())
					GenericScrollview->SetState(EScrollviewType::VE_HERO_PROTECTED);
				else
					GenericScrollview->SetState(EScrollviewType::VE_HERO_NORMAL);
			}
				
			
			GenericScrollview->Appear(true);
		}
	}

}

void UUP_HeroManager::OnClickBag()
{
	UUP_GenericScrollview* GenericScrollview = UUIFunctionLibrary::GetGenericScrollview();
	if (GenericScrollview)
	{
		bool bItemInven = (GenericScrollview->Variables.State == EScrollviewType::VE_BAG_WEAPON ||
			GenericScrollview->Variables.State == EScrollviewType::VE_BAG_ARMOR_TOP ||
			GenericScrollview->Variables.State == EScrollviewType::VE_BAG_ARMOR_BOT ||
			GenericScrollview->Variables.State == EScrollviewType::VE_BAG_RING ||
			GenericScrollview->Variables.State == EScrollviewType::VE_BAG_AMULET ||
			GenericScrollview->Variables.State == EScrollviewType::VE_BAG_MISC);

		if (bItemInven)
			GenericScrollview->Appear(false);
		else
		{
			bool isPrevBag = (GenericScrollview->Variables.PrevState == EScrollviewType::VE_BAG_WEAPON ||
				GenericScrollview->Variables.PrevState == EScrollviewType::VE_BAG_ARMOR_TOP ||
				GenericScrollview->Variables.PrevState == EScrollviewType::VE_BAG_ARMOR_BOT ||
				GenericScrollview->Variables.PrevState == EScrollviewType::VE_BAG_RING ||
				GenericScrollview->Variables.PrevState == EScrollviewType::VE_BAG_AMULET ||
				GenericScrollview->Variables.PrevState == EScrollviewType::VE_BAG_MISC);

			if (GenericScrollview->Variables.PrevState != EScrollviewType::VE_END && isPrevBag)
				GenericScrollview->SetState(GenericScrollview->Variables.PrevState);
			else
				GenericScrollview->SetState(EScrollviewType::VE_BAG_WEAPON);
			
			GenericScrollview->Appear(true);
		}
	}
}

void UUP_HeroManager::OnHeroUpgrade()
{
	URWidgetManager*	RWidgetManager = RGAMEINSTANCE(this)->RWidgetManager;
	UUP_HeroInfo_Upgrade* upgradePage = Cast<UUP_HeroInfo_Upgrade>(RWidgetManager->GetUserWidget(EWidgetBluePrintEnum::WBP_HeroRatingUp));
	if (upgradePage && upgradePage->IsInViewport())
	{
		upgradePage->OnClickBack();
		//
		RWidgetManager->AttachUIPage(EUIPageEnum::UIPage_LevelUpCommonSplash);
		UUC_Popup_HeroLevelUp_Splash*	resultSplash = Cast<UUC_Popup_HeroLevelUp_Splash>(RWidgetManager->GetUserWidget(EWidgetBluePrintEnum::WBP_LevelUpCommonSplash));

		if (resultSplash)
		{
			resultSplash->SetSplashText(UUtilFunctionLibrary::GetLocalizedString(EStringTableType::VE_UI, TEXT("UI_HeroGradeUp_Result_Title")),
				UUtilFunctionLibrary::GetLocalizedString(EStringTableType::VE_UI, TEXT("UI_HeroGradeUp_ResultMessage")));
			resultSplash->SetResult(BeforeHeroUpgradeData);
			resultSplash->SetVisibility(ESlateVisibility::Visible);
		}
		//need to fix
		if (Hero_Info)
			Hero_Info->SetHero(m_CurHeroUD);
		SetHeroInfo();

		BeforeHeroUpgradeData = *(RGAMEINSTANCE(this)->RInventory->GetHeroWithUD(m_CurHeroUD));
	}
}

void UUP_HeroManager::OnRewardPopup()
{
	URGameInstance*		RGameInstance = RGAMEINSTANCE(this);
	URWidgetManager*	RWidgetManager = RGameInstance->RWidgetManager;

	RWidgetManager->AttachUIPage(EUIPageEnum::UIPage_LevelUpCommonSplash);
	UUC_Popup_HeroLevelUp_Splash*	resultSplash = Cast<UUC_Popup_HeroLevelUp_Splash>(RWidgetManager->GetUserWidget(EWidgetBluePrintEnum::WBP_LevelUpCommonSplash));

	if (IsValid(resultSplash))
	{
		FHERO_ICON_INFO					CurIcon;
		UUIFunctionLibrary::GetHeroIconInfoWithUD(CurIcon, m_CurHeroUD, EHeroOwnershipType::VE_Me);
		resultSplash->SetVisibility(ESlateVisibility::Visible);
		resultSplash->SetSplashText(UUtilFunctionLibrary::GetLocalizedString(EStringTableType::VE_UI, TEXT("UI_HeroLevelUp_Result_Title")), 
			UUtilFunctionLibrary::GetLocalizedString(EStringTableType::VE_UI, TEXT("UI_HeroLevelUp_ResultMessage")));
		resultSplash->SetHeroSetting(m_PrevHeroIcon, CurIcon);
		SetHeroInfo();
	}
	if (IsValid(Hero_Info))
		Hero_Info->SetHero(m_CurHeroUD);
	if(IsValid(LevelUpPopup))
		LevelUpPopup->Refresh(m_CurHeroUD);

	URGameInstance*		Game = RGAMEINSTANCE(this);
	URInventory*		Inven = Game->RInventory;
	FHERO* heroData = Inven->GetHeroWithUD(m_CurHeroUD);
	m_UITableInfo = URenewal_BattleFunctionLibrary::GetCharacterTableInfo(ECharacterCategoryTypeEnum::VE_Hero, heroData->heroId);
	int32 m_MinLevel = heroData->level;
	int32 m_MaxLevel = m_UITableInfo->MaxLevel;
	if (m_MinLevel == m_MaxLevel)
		LevelUpPopup->SetVisibility(ESlateVisibility::Collapsed);
}

void UUP_HeroManager::OnGrindPopup(const TArray<FREWARD>& Inrewards)
{
	UUIFunctionLibrary::GetGenericScrollview()->SetState(EScrollviewType::VE_HERO_NORMAL);
	/*TArray<FString> HeroUDs;
	RGAMEINSTANCE(this)->RInventory->GetHeroUDArray(HeroUDs);
	for (size_t j = 0; j < HeroUDs.Num();)
	{
		FHERO* hero = RGAMEINSTANCE(this)->RInventory->GetHeroWithUD(HeroUDs[j]);
		if (hero)
		{
			FCharacterTableInfo* tableinfo = UTableManager::GetInstancePtr()->GetCharacterRow(hero->heroId);
			if (tableinfo && !tableinfo->IsHero)
			{
				HeroUDs.RemoveAt(j);
				continue;
			}
		}
		++j;
	}

	if(HeroUDs.IsValidIndex(0))
		OnClick_HeroIcon(HeroUDs[0]);*/
}

void UUP_HeroManager::OnHeroLock()
{
	SetHeroInfo();

	if (IsValid(Hero_Info))
		Hero_Info->SetHero(m_CurHeroUD);
}

void UUP_HeroManager::OnButtonFilterShowClicked()
{
	if (IsValid(ItemInventoryFilter))
	{
		if (IsValid(Hero_Info) && IsValid(Hero_Info->Switcher_ItemInventory))
		{
			if(Hero_Info->Switcher_ItemInventory->GetActiveWidgetIndex() == 0)
				ItemInventoryFilter->SetVisibility(ESlateVisibility::Hidden);
			else
			{
				if (ItemInventoryFilter->GetVisibility() != ESlateVisibility::Hidden)
					ItemInventoryFilter->SetVisibility(ESlateVisibility::Hidden);
				else
					ItemInventoryFilter->SetVisibility(ESlateVisibility::Visible);
			}
		}
	}
}

//LevelUp

void UUP_HeroManager::OnItemInventoryUpdated()
{
	if (IsValid(UC_HeroItems))
		UC_HeroItems->InvalidateData();
	if (IsValid(Hero_Info))
		Hero_Info->SetHero(m_CurHeroUD);
}

void UUP_HeroManager::OnItemInfoItemInventoryUpdated(EItemInventoryUpdate ItemInventoryUpdateEnum, bool bWidgetComplete, bool IsItemInfoRight)
{
	// let the HeroBaseInfo know
	switch (ItemInventoryUpdateEnum)
	{
		case EItemInventoryUpdate::NoChange:
		{
			break;
		}
		case EItemInventoryUpdate::OneItemUpdated:
		case EItemInventoryUpdate::Updated:
		case EItemInventoryUpdate::FilterChanged:
		{
			UUIFunctionLibrary::GetGenericScrollview()->Appear(false);

			Hero_Info->OnItemInventoryUpdated(EItemInventoryUpdate::Updated, false, (URScrollItem*)nullptr, EWidgetBluePrintEnum::WBP_Popup_ItemInfo /* meaningless argument */);
			break;
		}
	}
	SetHeroInfo();
}



void UUC_HeroStar::SetStar(FString heroUD)
{
	FHERO* hero = RGAMEINSTANCE(this)->RInventory->GetHeroWithUD(heroUD);
	if (hero)
	{
		FCharacterTableInfo* heroTableinfo = UTableManager::GetInstancePtr()->GetCharacterRow(hero->heroId);
		if (heroTableinfo)
		{
			for (UImage* ImgStar : HeroStar)
			{
				if (IsValid(ImgStar))
					ImgStar->SetVisibility(ESlateVisibility::Collapsed);
			}

			for (int32 i = 0; i < heroTableinfo->Grade; i++)
			{
				if (HeroStar.IsValidIndex(i))
					HeroStar[i]->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
			}

			if (IsValid(WidgetSwitcher_Star))
			{
				if (heroTableinfo->Grade <= 5)
					WidgetSwitcher_Star->SetActiveWidgetIndex(0);
				else if (heroTableinfo->Grade < 10)
					WidgetSwitcher_Star->SetActiveWidgetIndex(1);
				else
					WidgetSwitcher_Star->SetActiveWidgetIndex(2);
			}
		}
	}

	
}

void UUC_HeroStar::SetStar(int32 StarCount)
{
	for (UImage* ImgStar : HeroStar)
	{
		if (IsValid(ImgStar))
			ImgStar->SetVisibility(ESlateVisibility::Collapsed);
	}

	for (int32 i = 0; i < StarCount; i++)
	{
		if (HeroStar.IsValidIndex(i))
			HeroStar[i]->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
	}

	if (IsValid(WidgetSwitcher_Star))
	{
		if (StarCount <= 5)
			WidgetSwitcher_Star->SetActiveWidgetIndex(0);
		else if (StarCount < 10)
			WidgetSwitcher_Star->SetActiveWidgetIndex(1);
		else
			WidgetSwitcher_Star->SetActiveWidgetIndex(2);
	}
}
