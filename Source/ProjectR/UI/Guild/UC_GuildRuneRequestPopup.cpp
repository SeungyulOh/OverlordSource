
#include "ProjectR.h"
#include "UC_GuildRuneRequestPopup.h"
#include "UI/Guild/GuildRuneRequestScrollItemData.h"
#include "UI/Guild/GuildRuneRequestScrollItem.h"
#include "UI/Control/RScrollView.h"
#include "GlobalIntegrated.h"
#include "UtilFunctionIntegrated.h"
#include "GameInfo/RInventory.h"
#include "Network/PacketFunctionLibrary.h"
#include "UtilFunctionIntegrated.h"

void UUC_GuildRuneRequestPopup::NativeConstruct()
{
	Super::NativeConstruct();

	if (IsValid(Button_OK))
	{
		Button_OK->OnClicked.RemoveDynamic(this, &UUC_GuildRuneRequestPopup::OnClickOK);
		Button_OK->OnClicked.AddDynamic(this, &UUC_GuildRuneRequestPopup::OnClickOK);
	}
	if (IsValid(Button_Close))
	{
		Button_Close->OnClicked.RemoveDynamic(this, &UUC_GuildRuneRequestPopup::OnClickClose);
		Button_Close->OnClicked.AddDynamic(this, &UUC_GuildRuneRequestPopup::OnClickClose);
	}
	if(IsValid(RequestRuneScrollView))
	{
		RequestRuneScrollView->Init();
		RequestRuneScrollView->OnScrollViewSelectedItem.RemoveDynamic(this, &UUC_GuildRuneRequestPopup::OnSelectHeroScrollItem);
		RequestRuneScrollView->OnScrollViewSelectedItem.AddDynamic(this, &UUC_GuildRuneRequestPopup::OnSelectHeroScrollItem);
	}
}

void UUC_GuildRuneRequestPopup::NativeDestruct()
{
	if (IsValid(Button_OK))
	{
		Button_OK->OnClicked.RemoveDynamic(this, &UUC_GuildRuneRequestPopup::OnClickOK);
	}
	if (IsValid(Button_Close))
	{
		Button_Close->OnClicked.RemoveDynamic(this, &UUC_GuildRuneRequestPopup::OnClickClose);
	}
	if (IsValid(RequestRuneScrollView))
	{
		RequestRuneScrollView->OnScrollViewSelectedItem.RemoveDynamic(this, &UUC_GuildRuneRequestPopup::OnSelectHeroScrollItem);
	}
	Super::NativeDestruct();
}

void UUC_GuildRuneRequestPopup::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);

	if (bPlayCloseAni)
	{
		if (!IsPlayAni(TEXT("Appear")))
		{
			SetVisibility(ESlateVisibility::Collapsed);
			bPlayCloseAni = false;
		}
	}
}

void UUC_GuildRuneRequestPopup::ShowRequestRunePopup()
{
	Init();
	SetVisibility(ESlateVisibility::SelfHitTestInvisible);
	PlayAni(TEXT("Appear"));
}

void UUC_GuildRuneRequestPopup::Init()
{
	if (IsValid(RequestRuneScrollView))
	{
		RequestRuneScrollView->Init();
		RequestRuneScrollView->ClearScrollItemData();
		URGameInstance* RGameInstance = RGAMEINSTANCE(this);
// 		if (IsValid(RGameInstance) == false)
// 			return;
		TArray<FString> ScrollAllHeroUDList;
		RGameInstance->RInventory->GetHeroUDs(ScrollAllHeroUDList);

		for (FString& HeroUD : ScrollAllHeroUDList)
		{
			UGuildRuneRequestScrollItemData* HeroData = Cast<UGuildRuneRequestScrollItemData>(RequestRuneScrollView->AddScrollItemData(UGuildRuneRequestScrollItemData::StaticClass()));
			HeroData->HaveCount = 0;
			UUIFunctionLibrary::GetHeroIconInfoWithUD(HeroData->HeroIconInfo, HeroUD, EHeroOwnershipType::VE_Me);
// 			FHERO* HeroInfo = RGameInstance->HeroInventory->GetInventoryDataWithUD(HeroUD);
// 			if (HeroInfo != nullptr)
// 			{
// 				HeroData->HaveCount = HeroInfo->rune;
// 			}
		}

		RequestRuneScrollView->InvalidateData();
	}

	if (IsValid(Button_OK))
	{
		Button_OK->SetIsEnabled(false);
	}
}

void UUC_GuildRuneRequestPopup::OnClickOK()
{
	URGameInstance* RGameInstance = RGAMEINSTANCE(this);
// 	if (IsValid(RGameInstance) == false)
// 		return;
	UPacketFunctionLibrary::GUILD_RUNE_REQUEST_RQ(SelectedHeroID);

	OnClickClose();
}

void UUC_GuildRuneRequestPopup::OnClickClose()
{
	bPlayCloseAni = true;
	PlayAni(TEXT("Appear"), true);
	SelectedHeroID = FName();
}

void UUC_GuildRuneRequestPopup::OnSelectHeroScrollItem(URScrollItem* selectItem)
{
	if (IsValid(RequestRuneScrollView))
	{
		for (int i = 0; i < RequestRuneScrollView->GetScrollItemAll().Num(); ++i)
		{
			UUC_GuildRuneRequestScrollItem* pHero = static_cast<UUC_GuildRuneRequestScrollItem*>(RequestRuneScrollView->GetScrollItem(i));
			if (pHero)
				pHero->SetSelectedHero(false);
		}
	}
	
	if (IsValid(selectItem))
	{
		UUC_GuildRuneRequestScrollItem* pSelectedHero = static_cast<UUC_GuildRuneRequestScrollItem*>(selectItem);
		
		if (IsValid(pSelectedHero))
		{
			pSelectedHero->SetSelectedHero(true);
			auto SelectedItemData = static_cast<UGuildRuneRequestScrollItemData*>(selectItem->ItemData);
			if (SelectedItemData)
			{
				SelectedHeroID = SelectedItemData->HeroIconInfo.heroId;
				if (IsValid(Button_OK))
					Button_OK->SetIsEnabled(true);
			}
		}
	}
}