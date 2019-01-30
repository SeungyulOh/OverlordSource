// Fill out your copyright notice in the Description page of Project Settings.

#include "ProjectR.h"
#include "UC_Arena_DeckBtn.h"

#include "UI/Common/Renewal_BaseHeroIcon.h"
#include "UtilFunctionIntegrated.h"
#include "Network/PacketDataStructures.h"

#include "GameInfo/RInventory.h"

void UUC_Arena_DeckBtn::NativeConstruct()
{
	//Blueprint Constructor
	Super::NativeConstruct();

	if (IsValid(Select_DeckBtn))
	{
		Select_DeckBtn->OnClicked.RemoveDynamic(this, &UUC_Arena_DeckBtn::OnClick_DeckButton);
		Select_DeckBtn->OnClicked.AddDynamic(this, &UUC_Arena_DeckBtn::OnClick_DeckButton);
	}
}

void UUC_Arena_DeckBtn::NativeDestruct()
{
	Super::NativeDestruct();
}

void UUC_Arena_DeckBtn::SetAvailable(bool inAvailable)
{
	if (inAvailable)
	{
		NotiImage->SetVisibility(ESlateVisibility::HitTestInvisible);
		UWidgetAnimation* Anim = GetWidgetAnimFromName(TEXT("Pick_Available"));
		PlayAnimation(Anim, 0.f, 0);
	}
	else
	{
		NotiImage->SetVisibility(ESlateVisibility::Hidden);
		Selecting->SetVisibility(ESlateVisibility::Hidden);
	}
}

void UUC_Arena_DeckBtn::SetSelecting(bool bSelecting)
{
	if (bSelecting)
	{
		Selecting->SetVisibility(ESlateVisibility::HitTestInvisible);
		UWidgetAnimation* Anim = GetWidgetAnimFromName(TEXT("Deck_Selecting"));
		PlayAnimation(Anim, 0.f, 0);
	}
	else
	{

		Selecting->SetVisibility(ESlateVisibility::Collapsed);
	}
}

void UUC_Arena_DeckBtn::SetHero(FString heroUD)
{
	URGameInstance* RGameInstance = RGAMEINSTANCE(this);
 	FHERO_ICON_INFO iconInfo;
 	UUIFunctionLibrary::GetHeroIconInfoWithUD(iconInfo, heroUD, EHeroOwnershipType::VE_Me);
	CurHeroUD = heroUD;
	if (RGameInstance->ArenaManager->bMirrorDeckSetting)
	{
		iconInfo.bIsHp = true;
		int32 nRemainHP = RGameInstance->MirrorDungeonManager->FindRemainHP(heroUD);

		if (nRemainHP == 0)
		{
			iconInfo.HpRate = 0.0f;
			iconInfo.bIsDead = true;
		}
		else if (nRemainHP == -1)
		{
			iconInfo.HpRate = 1.0f;
			iconInfo.bIsDead = false;
		}
		else
		{
			int32 nMaxHP = iconInfo.MaxHP;
			iconInfo.HpRate = (float)nRemainHP / (float)nMaxHP;

			iconInfo.bIsDead = false;
		}
	}
 	SetHeroIconInfo(iconInfo);
}

void UUC_Arena_DeckBtn::OnClick_DeckButton()
{
	int32 cnt = 0;
	for (FString& ud : BlackBoard->SelectedHeroUDs)
	{
		if (!ud.IsEmpty())
			cnt++;
	}

	if (m_bEmptyDeck)
	{
		if (cnt >= 5)
		{
			UUIFunctionLibrary::ShowCommonPopupSplash(UUtilFunctionLibrary::GetLocalizedString(EStringTableType::VE_UI, COMMONSTRING_NOTIFY),
				UUtilFunctionLibrary::GetLocalizedString(EStringTableType::VE_UI, FName(TEXT("UI_Full_Herocount"))));

			return;
		}
	}

	if (m_bMyHero)
		OnClick_Deck();
}

void UUC_Arena_DeckBtn::Init(FDeckManager_Blackboard* bb, int deck_pos, bool bIsMyHero)
{
	//m_wpArenaDeckSetting = arena_deck_set;
	BlackBoard = bb;
	m_nDeckPosition = deck_pos;
	m_bMyHero = bIsMyHero;

	DelHeroIconInfo();

	BP_InitArenaDeckBtn();
}

void UUC_Arena_DeckBtn::InitDeckBtnState()
{
	ChangeBtnState(false);
}

void UUC_Arena_DeckBtn::SetIconHeroIcon()
{
	if (Icon_Hero)
	{
		Icon_Hero->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
		Icon_Hero->SetHeroIconInfoCopy(m_HeroIconInfo);
		Icon_Hero->HidePowerPanel(true);
	}
}

void UUC_Arena_DeckBtn::ChangeBtnState(bool select)
{
	// MatchTarget deselect(0), select(1) : MyHero deselect(2), select(3)
	m_bShowSelectedMark = select;
	if(ButtonSwitcher)
		UpdateBtnState(ButtonSwitcher);
}

void UUC_Arena_DeckBtn::UpdateBtnState(UWidgetSwitcher* state_switcher, UImage* ClearImg)
{
	if (m_bShowSelectedMark)
	{
		if (m_bMyHero)
		{
			state_switcher->SetActiveWidgetIndex(3);
			if (ClearImg)
			{
				ClearImg->SetVisibility(ESlateVisibility::HitTestInvisible);
			}
		}
		else
		{
			state_switcher->SetActiveWidgetIndex(1);
			if (ClearImg)
			{
				ClearImg->SetVisibility(ESlateVisibility::Collapsed);
			}
		}
	}
	else
	{
		if (m_bMyHero)
		{
			state_switcher->SetActiveWidgetIndex(2);
			if (ClearImg)
			{
				ClearImg->SetVisibility(ESlateVisibility::Collapsed);
			}
		}
		else
		{
			state_switcher->SetActiveWidgetIndex(0);
			if (ClearImg)
			{
				ClearImg->SetVisibility(ESlateVisibility::Collapsed);
			}
		}
	}
}

void UUC_Arena_DeckBtn::SetSelected()
{
	if (m_bMyHero)
	{
// 		if (m_wpArenaDeckSetting.Get())
// 			m_wpArenaDeckSetting.Get()->SelectDeckBtn(m_nDeckPosition);
	}
}

void UUC_Arena_DeckBtn::BP_InitArenaDeckBtn()
{
	InitDeckBtnState();
	Icon_Clear->SetVisibility(ESlateVisibility::Collapsed);
}

void UUC_Arena_DeckBtn::BP_SetHeroIcon()
{
	SetIconHeroIcon();
	UpdateBtnState(ButtonSwitcher, Icon_Clear);
}

void UUC_Arena_DeckBtn::BP_ShowDeckBtn()
{
	ButtonSwitcher->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
	Button_ScaleBox->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
	Select_DeckBtn->SetVisibility(ESlateVisibility::Visible);
}

void UUC_Arena_DeckBtn::BP_HideDeckBtn()
{
	ButtonSwitcher->SetVisibility(ESlateVisibility::Hidden);
	Button_ScaleBox->SetVisibility(ESlateVisibility::Hidden);
	Select_DeckBtn->SetVisibility(ESlateVisibility::Hidden);
}

void UUC_Arena_DeckBtn::OnClickDeckSelectButton()
{
	SetSelected();
}

bool UUC_Arena_DeckBtn::IsSameHero(const FString& heroUD)
{
	if(false == m_bEmptyDeck)
	{
		if (m_HeroIconInfo.heroUD == heroUD)
			return true;
		URInventory* inventory = RGAMEINSTANCE(this)->RInventory;
		if (!IsValid(inventory))
			return false;

		FHERO* lhs = inventory->GetHeroWithUD(m_HeroIconInfo.heroUD);
		FHERO* rhs = inventory->GetHeroWithUD(heroUD);
		
		if (!lhs || !rhs)
			return false;

		FCharacterTableInfo* lhsTable = UTableManager::GetInstancePtr()->GetCharacterRow(lhs->heroId);
		FCharacterTableInfo* rhsTable = UTableManager::GetInstancePtr()->GetCharacterRow(rhs->heroId);

		if (!lhsTable || !rhsTable)
			return false;

		return (lhsTable->DisplayName == rhsTable->DisplayName);
	}

	return false;
}

bool UUC_Arena_DeckBtn::IsSameHeroUD(const FString& heroUD)
{
	if (false == m_bEmptyDeck)
	{
		if (m_HeroIconInfo.heroUD == heroUD)
			return true;
	}
	return false;
}

void UUC_Arena_DeckBtn::SetHeroIconInfo(FHERO_ICON_INFO& icon_info)
{
	m_bEmptyDeck = true;
	m_bShowSelectedMark = false;
	m_HeroPower = 0;

	m_HeroIconInfo = icon_info;

	if (false == m_HeroIconInfo.heroUD.IsEmpty())
	{
		m_bEmptyDeck = false;
		m_bShowSelectedMark = true;
		m_HeroPower = UClient_BattleFunctionLibrary::GetHeroCombatStat(m_HeroIconInfo.heroUD);

		BP_SetHeroIcon();
	}
}

void UUC_Arena_DeckBtn::SetTargetHeroIconInfo(FBATTLE_USER& MatchList, FHERO_ICON_INFO& icon_info, FString& HeroUD)
{
	m_bEmptyDeck = true;
	m_bShowSelectedMark = false;
	m_HeroPower = 0;

	m_HeroIconInfo = icon_info;

	if (false == m_HeroIconInfo.heroUD.IsEmpty())
	{
		m_bEmptyDeck = false;
		m_bShowSelectedMark = true;

		if(m_HeroIconInfo.bIsDead == false)
			m_HeroPower = UClient_BattleFunctionLibrary::GetTargetHeroCombatStat(MatchList, HeroUD);

		BP_SetHeroIcon();
	}
}

bool UUC_Arena_DeckBtn::GetMyDeckHeroUD(FString& heroUD)
 {
 	if (false == m_bEmptyDeck && false == m_HeroIconInfo.heroUD.IsEmpty())
 	{
		heroUD = m_HeroIconInfo.heroUD;
 		return true;
 	}
 	return false;
 }

void UUC_Arena_DeckBtn::DelHeroIconInfo()
{
	m_bEmptyDeck = true;
	m_bShowSelectedMark = false;
	CurHeroUD.Reset();
	m_HeroIconInfo.heroUD.Empty();
	m_HeroIconInfo.heroId = FName();
	m_HeroIconInfo.grade = 1;
	m_HeroIconInfo.level = 0;
	m_HeroIconInfo.exp = 0;
	m_HeroIconInfo.acquiredExp = 0;
	m_HeroIconInfo.bIsUsed = false;
	m_HeroIconInfo.eIconSelectType = EHeroIconSelectTypeEnum::eHeroSelect_None;
	m_HeroIconInfo.bIsOwned = false;
	m_HeroIconInfo.bLock = false;
	//m_HeroIconInfo.bIsAbleAwaken = false;
	m_HeroIconInfo.MaxExp = 1;
	m_HeroIconInfo.MaxHP = 1;
	m_HeroIconInfo.PortraitTextureRef = nullptr;

	m_HeroPower = 0;

	BP_SetHeroIcon();
}

void UUC_Arena_DeckBtn::PlayAnimPickAvailable()
{
	NotiImage->SetVisibility(ESlateVisibility::HitTestInvisible);
	PlayAni(TEXT("Pick_Available"),false,0);
}

void UUC_Arena_DeckBtn::StopAnimPickAvailable()
{
	UWidgetAnimation* PlayingAni = GetWidgetAnimFromName(TEXT("Pick_Available"));
	if (!IsValid(PlayingAni))
		return; 

	StopAnimation(PlayingAni);
	NotiImage->SetVisibility(ESlateVisibility::Hidden);
}

bool UUC_Arena_DeckBtn::IsPlayingAnimPickAvailable()
{
	UWidgetAnimation* PlayingAni = GetWidgetAnimFromName(TEXT("Pick_Available"));
	if (!IsValid(PlayingAni))
		return false;

	return IsAnimationPlaying(PlayingAni);
}
