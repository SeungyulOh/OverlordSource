// Fill out your copyright notice in the Description page of Project Settings.

#include "ProjectR.h"
#include "UC_Popup_HeroInfo_BreakUp.h"

#include "GlobalIntegrated.h"

#include "Utils/UIFunctionLibrary.h"
#include "Utils/UtilFunctionLibrary.h"
#include "Utils/SharedUtils/Renewal_BattleFunctionLibrary.h"

#include "Network/PacketFunctionLibrary.h"


void UUC_Popup_HeroInfo_BreakUp::NativeConstruct()
{
	Super::NativeConstruct();
	Init();
}

void UUC_Popup_HeroInfo_BreakUp::NativeDestruct()
{
	Release();

	Super::NativeDestruct();
}

void UUC_Popup_HeroInfo_BreakUp::Init()
{
	if (IsValid(Button_OK))
	{
		Button_OK->OnClicked.RemoveDynamic(this, &UUC_Popup_HeroInfo_BreakUp::OnClick_OK);
		Button_OK->OnClicked.AddDynamic(this, &UUC_Popup_HeroInfo_BreakUp::OnClick_OK);
	}
		
	if (IsValid(Button_Cancel))
	{
		Button_Cancel->OnClicked.RemoveDynamic(this, &UUC_Popup_HeroInfo_BreakUp::OnClick_Close);
		Button_Cancel->OnClicked.AddDynamic(this, &UUC_Popup_HeroInfo_BreakUp::OnClick_Close);
	}
		
	if (IsValid(Button_Close))
	{
		Button_Close->OnClicked.RemoveDynamic(this, &UUC_Popup_HeroInfo_BreakUp::OnClick_Close);
		Button_Close->OnClicked.AddDynamic(this, &UUC_Popup_HeroInfo_BreakUp::OnClick_Close);
	}
		
}

void UUC_Popup_HeroInfo_BreakUp::Release()
{
	URGameInstance*		RGameInstance = RGAMEINSTANCE(this);
	URWidgetManager*	RWidgetManager = RGameInstance->RWidgetManager;

	SetVisibility(ESlateVisibility::Collapsed);
	RWidgetManager->DetachUIPage(EUIPageEnum::UIPage_GrindPopup);
}

void UUC_Popup_HeroInfo_BreakUp::SetInfo(FString InCurHeroUD)
{

	URGameInstance*			Game = RGAMEINSTANCE(this);
	URInventory*			Inven = Game->RInventory;

	checkf(Game != nullptr, TEXT("UUP_HeroManager[RGameInstance Is Empty]"));
	checkf(Inven != nullptr, TEXT("UUP_HeroManager[RInventory Is Empty]"));


	CurHeroUD = InCurHeroUD;

	FHERO*		Hero = Inven->GetHeroWithUD(CurHeroUD);
	
	if (Hero == nullptr)
		return;

	if(IsValid(Icon_Hero))
		Icon_Hero->SetHeroIconInfoWithUD(CurHeroUD, EHeroOwnershipType::VE_Me);

	FCharacterTableInfo*	m_UITableInfo	=	URenewal_BattleFunctionLibrary::GetCharacterTableInfo(ECharacterCategoryTypeEnum::VE_Hero, Hero->heroId);

	if (IsValid(Text_Name))
	{
		FText HeroName = UUtilFunctionLibrary::GetLocalizedString(EStringTableType::VE_Hero, m_UITableInfo->DisplayName);
		Text_Name->SetText(HeroName);
	}
}

void UUC_Popup_HeroInfo_BreakUp::OnClick_OK()
{
	TArray<FString>		SelectedHeroUD;

	SelectedHeroUD.Add(CurHeroUD);
	UPacketFunctionLibrary::INVENTORY_HERO_GRIND_RQ(SelectedHeroUD);
	Release();
}

void UUC_Popup_HeroInfo_BreakUp::OnClick_Close()
{
	Release();
}
