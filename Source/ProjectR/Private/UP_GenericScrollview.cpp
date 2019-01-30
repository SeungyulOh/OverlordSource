// Fill out your copyright notice in the Description page of Project Settings.

#include "ProjectR.h"
#include "UP_GenericScrollview.h"

#include "UI/Control/RScrollView.h"
#include "UI/Control/RScrollItem.h"
#include "UI/Control/RScrollItemData.h"
#include "UI/Common/Renewal_BaseHeroIcon.h"

#include "UI/HeroInfo/HeroManager/UP_HeroManager.h"
#include "UI/HeroInfo/HeroUpgrade/UP_HeroInfo_Upgrade.h"
#include "UI/HeroInfo/HeroStrong/UP_HeroInfo_Strong.h"

#include "UI/ItemManagement/UP_ItemInfo.h"

#include "UI/HeroManagement/Popup/UC_Popup_SoulInfo.h"

#include "UI/RWidgetManager.h"
#include "UI/Common/ScrollView/UC_Hero_SortingFilter.h"
#include "UI/Common/Popup/UC_Popup_Item_Sell.h"

#include "Utils/UIFunctionLibrary.h"

#include "Lobby/IndoorLobbyStateActor.h"
#include "ContentGameMode/Modes/TowerLobbyGameMode.h"

#include "Global/RGameInstance.h"
#include "GameInfo/RInventory.h"

#include "Global/TableManager.h"
#include "UI/ItemManagement/UC_WalletItemInfo.h"
#include "Global/MirrorDungeonManager.h"
#include "GamePlayBluePrintFunction.h"
#include "GameState/Stage/Spawner/BaseStageInfo.h"
#include "Table/CampaignTableInfo.h"
#include "Network/PacketFunctionLibrary.h"
#include "UI/ItemManagement/UP_ItemUpgrade.h"



const int MAXSCROLLITEM = 30;
const int MAXTABCOUNT = 6;
const int MAXSCROLLITEMDATA = 150;
const FLinearColor EnableColor = FLinearColor(0.002f, 0.004f, 0.005f, 0.9f);
const FLinearColor DisableColor = FLinearColor(1.f, 1.f, 1.f, 0.f);

const float INIT_X = 870.f;

const FName PROTECTED_ICONKEY = TEXT("Icon_Tab_Favorite");
const FName HERO_ICONKEY = TEXT("Icon_Tab_Hero_Sprite");
const FName SPIRIT_ICONKEY = TEXT("Icon_Tab_Soul_Sprite");
const FName GRIND_STRINGKEY = TEXT("UIPage_HeroInventory_GrindButton");
const FName SELECTALL_STRINGKEY = TEXT("UIPage_HeroInventory_SelectAllButton");
const FName DONE_STRINGKEY = TEXT("UI_Common_099998");

const FName HEROUPGRADE_STRINGKEY = TEXT("UI_HeroManagement_Tab_Hero");
const FName HEROENCHANT_STRINGKEY = TEXT("UI_HeroManagement_Tab_Spirit");
const FName HEROENCHANTEXT_STRINGKEY = TEXT("UI_HeroManagement_Spirit_Extraction");

const FName HERODECK_STRINGKEY = TEXT("UI_campain_Deck_Setting");

const FName WEAPON_ICONKEY = TEXT("Icon_Tab_Weapon_Sprite");
const FName TOP_ICONKEY = TEXT("Icon_Tab_Top_Sprite");
const FName BOTTOM_ICONKEY = TEXT("Icon_Tab_Bottom");
const FName RING_ICONKEY = TEXT("Icon_Tab_Ring_Sprite");
const FName AMULET_ICONKEY = TEXT("Icon_Tab_Necklace_Sprite");
const FName MISC_ICONKEY = TEXT("Icon_Tab_Etc");

const FName SELL_STRINGKEY = TEXT("UIPage_HeroInventory_GrindButton");

void UUP_GenericScrollview::NativeConstruct()
{
	Super::NativeConstruct();

	if (!Variables.CachedItemDataList.Num())
	{
		for (size_t i = 0; i < MAXSCROLLITEMDATA; ++i)
		{
			UUC_GenericScrollItemData* ItemData = Cast<UUC_GenericScrollItemData>(Variables.ScrollView->CreateScrollItemData(UUC_GenericScrollItemData::StaticClass()));
			ItemData->WidgetSize = Variables.ScrollView->GetItemInitSize();
			Variables.CachedItemDataList.Emplace(ItemData);
		}
	}

	/*Renderer Begin*/
	Renderer.variables = &Variables;
	/*Renderer End*/


	/*Delegates Begin*/
	if (IsValid(Variables.BackButton))
	{
		Variables.BackButton->OnClicked.Clear();
		Variables.BackButton->OnClicked.AddDynamic(this, &UUP_GenericScrollview::OnClick_Backbutton);
	}

	Variables.ButtonArray[0]->OnClicked.Clear();
	Variables.ButtonArray[0]->OnClicked.AddDynamic(this, &UUP_GenericScrollview::OnClickedTab1);
	Variables.ButtonArray[1]->OnClicked.Clear();
	Variables.ButtonArray[1]->OnClicked.AddDynamic(this, &UUP_GenericScrollview::OnClickedTab2);
	Variables.ButtonArray[2]->OnClicked.Clear();
	Variables.ButtonArray[2]->OnClicked.AddDynamic(this, &UUP_GenericScrollview::OnClickedTab3);
	Variables.ButtonArray[3]->OnClicked.Clear();
	Variables.ButtonArray[3]->OnClicked.AddDynamic(this, &UUP_GenericScrollview::OnClickedTab4);
	Variables.ButtonArray[4]->OnClicked.Clear();
	Variables.ButtonArray[4]->OnClicked.AddDynamic(this, &UUP_GenericScrollview::OnClickedTab5);
	Variables.ButtonArray[5]->OnClicked.Clear();
	Variables.ButtonArray[5]->OnClicked.AddDynamic(this, &UUP_GenericScrollview::OnClickedTab6);

	if (IsValid(Variables.Button_Sorting_Show))
	{
		Variables.Button_Sorting_Show->OnClicked.Clear();
		Variables.Button_Sorting_Show->OnClicked.AddDynamic(this, &UUP_GenericScrollview::OnClick_ButtonSortingShow);
	}

	if (IsValid(Variables.Button_Sorting_Hide))
	{
		Variables.Button_Sorting_Hide->OnClicked.Clear();
		Variables.Button_Sorting_Hide->OnClicked.AddDynamic(this, &UUP_GenericScrollview::OnClick_ButtonSortingHide);
	}

	if (IsValid(Variables.Button_Filter_Show))
	{
		Variables.Button_Filter_Show->OnClicked.Clear();
		Variables.Button_Filter_Show->OnClicked.AddDynamic(this, &UUP_GenericScrollview::OnClick_ButtonFilterShow);
	}

	if (IsValid(Variables.Button_Exe))
	{
		Variables.Button_Exe->OnClicked.Clear();
		Variables.Button_Exe->OnClicked.AddDynamic(this, &UUP_GenericScrollview::OnClick_ButtonExe);
	}

	if (IsValid(Variables.Button_Done))
	{
		Variables.Button_Done->OnClicked.Clear();
		Variables.Button_Done->OnClicked.AddDynamic(this, &UUP_GenericScrollview::OnClick_ButtonDone);
	}

	if (IsValid(Variables.Button_SelectAll))
	{
		Variables.Button_SelectAll->OnClicked.Clear();
		Variables.Button_SelectAll->OnClicked.AddDynamic(this, &UUP_GenericScrollview::OnClick_ButtonSelectAll);
	}

	if (IsValid(Variables.Button_InventorySizeUp))
	{
		Variables.Button_InventorySizeUp->OnClicked.Clear();
		Variables.Button_InventorySizeUp->OnClicked.AddDynamic(this, &UUP_GenericScrollview::OnClick_InventorySizeup);
	}
	/*Delegates End*/

	StateController.blackboard = &Blackboard;

	Blackboard.OnBBStateChanged.RemoveDynamic(this, &UUP_GenericScrollview::Update_StateController);
	Blackboard.OnBBStateChanged.AddDynamic(this, &UUP_GenericScrollview::Update_StateController);

	

}

void UUP_GenericScrollview::NativeDestruct()
{
	UCanvasPanelSlot* CanvasPanelSlot = Cast<UCanvasPanelSlot>(Variables.VerticalBox->Slot);
	if (CanvasPanelSlot)
	{
		CanvasPanelSlot->SetAlignment(FVector2D(0.f, 1.f));
		FMargin margin = CanvasPanelSlot->GetOffsets();
		CanvasPanelSlot->SetOffsets(FMargin(INIT_X, margin.Top, margin.Right, margin.Bottom));
	}
	Super::NativeDestruct();
}



void UUP_GenericScrollview::OnClick_Backbutton()
{
	switch (Variables.State)
	{
	case EScrollviewType::VE_HERO_DECK:
	case EScrollviewType::VE_HERO_DECK_FAVORITE:
	case EScrollviewType::VE_HERO_DECK_SHOWHP:
	{
		Blackboard.WaitingList.clear();
		StateController.SetState(EUserBehaviorState::IDLE);
		Blackboard.OnBBStateChanged.Broadcast();
	}break;

	default:
		break;
	}

	Appear(false);
}



void UUP_GenericScrollview::OnClickedTab1()
{
	SetState_Internal(0);
}

void UUP_GenericScrollview::OnClickedTab2()
{
	SetState_Internal(1);
}

void UUP_GenericScrollview::OnClickedTab3()
{
	SetState_Internal(2);
}

void UUP_GenericScrollview::OnClickedTab4()
{
	SetState_Internal(3);
}

void UUP_GenericScrollview::OnClickedTab5()
{
	SetState_Internal(4);
}

void UUP_GenericScrollview::OnClickedTab6()
{
	SetState_Internal(5);
}

void UUP_GenericScrollview::OnClick_ButtonSortingShow()
{
	if (IsValid(Variables.UC_Sorting))
	{
		Variables.UC_Sorting->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
		Variables.Switcher_Sorting->SetActiveWidgetIndex(1);
	}

	switch (Variables.State)
	{
	case EScrollviewType::VE_SPIRIT:
	{
		Variables.UC_Sorting->Button_AttackPower->SetVisibility(ESlateVisibility::Collapsed);
		Variables.UC_Sorting->Button_Level->SetVisibility(ESlateVisibility::Collapsed);

		Variables.UC_Sorting->Render_Enable(Variables.UC_Sorting->Button_Grade);
		Variables.UC_Sorting->Button_Grade->SortTypeSwitcher->SetActiveWidgetIndex((int32)Variables.UC_Sorting->bDescending);
	}
		break;

	default:
	{
		Variables.UC_Sorting->Button_AttackPower->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
		Variables.UC_Sorting->Button_Level->SetVisibility(ESlateVisibility::SelfHitTestInvisible);

		if (Variables.UC_Sorting->CurrentType == EHeroListFilterEnum::VE_Grade)
		{
			Variables.UC_Sorting->Render_Enable(Variables.UC_Sorting->Button_Grade);
			Variables.UC_Sorting->Button_Grade->SortTypeSwitcher->SetActiveWidgetIndex((int32)Variables.UC_Sorting->bDescending);
		}
		else if (Variables.UC_Sorting->CurrentType == EHeroListFilterEnum::VE_AttackPower)
		{
			Variables.UC_Sorting->Render_Enable(Variables.UC_Sorting->Button_AttackPower);
			Variables.UC_Sorting->Button_AttackPower->SortTypeSwitcher->SetActiveWidgetIndex((int32)Variables.UC_Sorting->bDescending);
		}
		else
		{
			Variables.UC_Sorting->Render_Enable(Variables.UC_Sorting->Button_Level);
			Variables.UC_Sorting->Button_Level->SortTypeSwitcher->SetActiveWidgetIndex((int32)Variables.UC_Sorting->bDescending);
		}
	}
		break;
	}

	
	
}

void UUP_GenericScrollview::OnClick_ButtonSortingHide()
{
	if (IsValid(Variables.UC_Sorting))
	{
		Variables.UC_Sorting->SetVisibility(ESlateVisibility::Collapsed);
		Variables.Switcher_Sorting->SetActiveWidgetIndex(0);
	}
}

void UUP_GenericScrollview::OnClick_ButtonFilterShow()
{
	if (Variables.State >= EScrollviewType::VE_ITEM_WEAPON)
		return;

	if (IsValid(Variables.UC_Filter))
	{
		if (!Variables.UC_Filter->IsVisible())
			Variables.UC_Filter->Render(Variables.State);
		else
			Variables.UC_Filter->SetVisibility(ESlateVisibility::Collapsed);
	}
}



void UUP_GenericScrollview::OnClick_ButtonExe()
{
	if (Variables.State == EScrollviewType::VE_BAG_WEAPON)
		SetState(EScrollviewType::VE_BAG_WEAPON_SELL);
	else if (Variables.State == EScrollviewType::VE_BAG_ARMOR_TOP)
		SetState(EScrollviewType::VE_BAG_ARMOR_TOP_SELL);
	else if (Variables.State == EScrollviewType::VE_BAG_ARMOR_BOT)
		SetState(EScrollviewType::VE_BAG_ARMOR_BOT_SELL);
	else if (Variables.State == EScrollviewType::VE_BAG_RING)
		SetState(EScrollviewType::VE_BAG_RING_SELL);
	else if (Variables.State == EScrollviewType::VE_BAG_AMULET)
		SetState(EScrollviewType::VE_BAG_AMULET_SELL);
	else if (Variables.State == EScrollviewType::VE_HERO_NORMAL)
		SetState(EScrollviewType::VE_HERO_NORMAL_GRIND);
	else if (Variables.State == EScrollviewType::VE_HERO_ENCHANT)
	{
		URGameInstance*		GameInstance = RGAMEINSTANCE(this);
		URWidgetManager*	RWidgetManager = GameInstance->RWidgetManager;
		UUP_HeroInfo_Strong* UP_HeroStrong = Cast<UUP_HeroInfo_Strong>(RWidgetManager->GetUserWidget(EWidgetBluePrintEnum::WBP_HeroReinforce));
		if (UP_HeroStrong)
			UP_HeroStrong->Do_Consume();
	}
		
	return;
}


void UUP_GenericScrollview::OnClick_ButtonDone()
{
	URGameInstance*		GameInstance = RGAMEINSTANCE(this);
	URWidgetManager*	RWidgetManager = GameInstance->RWidgetManager;

	TArray<URScrollItemData*> SelectedScrollItemData = Variables.ScrollView->GetSelectedScrollItemData();
	if (SelectedScrollItemData.Num() == 0)
	{
		UUIFunctionLibrary::ShowCommonPopupSplash(UUtilFunctionLibrary::GetLocalizedString(EStringTableType::VE_UI, COMMONSTRING_NOTIFY),
			UUtilFunctionLibrary::GetLocalizedString(EStringTableType::VE_UI, FName(TEXT("GrindSelect_SplashPopUp_Message01"))));
		return;
	}

	TArray<FString> SelectedItemUDs;
	for (size_t i = 0; i < SelectedScrollItemData.Num(); ++i)
	{
		UUC_GenericScrollItemData* itemdata = Cast<UUC_GenericScrollItemData>(SelectedScrollItemData[i]);
		if (itemdata && !itemdata->ItemUD.IsEmpty())
		{
			SelectedItemUDs.AddUnique(itemdata->ItemUD);
		}
	}


	if (Variables.State == EScrollviewType::VE_BAG_WEAPON_SELL ||
		Variables.State == EScrollviewType::VE_BAG_ARMOR_TOP_SELL ||
		Variables.State == EScrollviewType::VE_BAG_ARMOR_BOT_SELL ||
		Variables.State == EScrollviewType::VE_BAG_RING_SELL ||
		Variables.State == EScrollviewType::VE_BAG_AMULET_SELL)
	{
		Variables.Popup_ItemSell->SetData(SelectedItemUDs, TArray<FName>(), TArray<int32>());
		Variables.Popup_ItemSell->SetVisibility(ESlateVisibility::Visible);
		Variables.Popup_ItemSell->OnItemInventoryUpdated.BindLambda([this](EItemInventoryUpdate update, bool bSucess) {
			if (update == EItemInventoryUpdate::Updated)
			{
				Variables.Popup_ItemSell->SetVisibility(ESlateVisibility::Collapsed);

				if (Variables.State == EScrollviewType::VE_BAG_WEAPON_SELL)
					SetState(EScrollviewType::VE_BAG_WEAPON);
				else if (Variables.State == EScrollviewType::VE_BAG_ARMOR_TOP_SELL)
					SetState(EScrollviewType::VE_BAG_ARMOR_TOP);
				else if (Variables.State == EScrollviewType::VE_BAG_ARMOR_BOT_SELL)
					SetState(EScrollviewType::VE_BAG_ARMOR_BOT);
				else if (Variables.State == EScrollviewType::VE_BAG_RING_SELL)
					SetState(EScrollviewType::VE_BAG_RING);
				else if (Variables.State == EScrollviewType::VE_BAG_AMULET_SELL)
					SetState(EScrollviewType::VE_BAG_AMULET);

			}
		});
	}
	else
	{
		if (Variables.State == EScrollviewType::VE_HERO_NORMAL_GRIND)
		{
			UPacketFunctionLibrary::INVENTORY_HERO_GRIND_RQ(SelectedItemUDs);
		}
	}
	

}

void UUP_GenericScrollview::OnClick_ButtonSelectAll()
{
	TArray<URScrollItemData*> ScrollitemdataList = Variables.ScrollView->ItemDataList;

	TArray<URScrollItemData*>& SelectedItemDataList = Variables.ScrollView->GetSelectedScrollItemData();
	TArray<int32>& SelectedItemIdxList = Variables.ScrollView->GetSelectedScrollItem();
	for (size_t i = 0; i < ScrollitemdataList.Num(); ++i)
	{
		URScrollItemData* itemData = ScrollitemdataList[i];
		if (Variables.bAllSelected)
		{
			itemData->bSelected = !Variables.bAllSelected;
			SelectedItemDataList.Remove(itemData);

			if (SelectedItemIdxList.Contains(i))
				SelectedItemIdxList.Remove(i);
		}
		else
		{
			itemData->bSelected = !Variables.bAllSelected;
			SelectedItemDataList.Emplace(itemData);

			if (!SelectedItemIdxList.Contains(i))
				SelectedItemIdxList.Emplace(i);
		}
	}


	for (int32 idx : SelectedItemIdxList)
	{
		for (URScrollItem* ItemElem : Variables.ScrollView->ScrollItemList)
		{
			if (IsValid(ItemElem))
			{
				if (ItemElem->Count == idx)
				{
					ItemElem->IsSelected = true;
					ItemElem->SetSelected(true);
				}
			}
		}
	}

	Variables.bAllSelected = !Variables.bAllSelected;

	Variables.ScrollView->InvalidateDataWithoutResetSelect();
}

void UUP_GenericScrollview::OnClick_InventorySizeup()
{
	if (!RGAMEINSTANCE(this)->RInventory->CanBagBeExtendedFurther())
		return;

	UUIFunctionLibrary::ShowGenericPopupContent(EWidgetBluePrintEnum::WBP_Popup_BagExtend);
}

void UUP_GenericScrollview::Update_StateController()
{
	StateController.ChangeState();
	StateController.Process();
}

void UUP_GenericScrollview::Appear(bool bShow)
{

	if (bShow)
	{
		PlayAni(TEXT("Appear"), false);

		switch (Variables.State)
		{
		case EScrollviewType::VE_HERO_PROTECTED:
		case EScrollviewType::VE_HERO_NORMAL:
		case EScrollviewType::VE_SPIRIT:
		case EScrollviewType::VE_HERO_UPGRADE:
		case EScrollviewType::VE_HERO_ENCHANT:
		case EScrollviewType::VE_ITEM_WEAPON:
		case EScrollviewType::VE_ITEM_ARMOR_TOP:
		case EScrollviewType::VE_ITEM_ARMOR_BOT:
		case EScrollviewType::VE_ITEM_RING:
		case EScrollviewType::VE_ITEM_AMULET:
		case EScrollviewType::VE_ITEM_UPGRADE:
		case EScrollviewType::VE_BAG_WEAPON:
		case EScrollviewType::VE_BAG_ARMOR_TOP:
		case EScrollviewType::VE_BAG_ARMOR_BOT:
		case EScrollviewType::VE_BAG_RING:
		case EScrollviewType::VE_BAG_AMULET:
		case EScrollviewType::VE_BAG_MISC:
		{
			UUP_HeroManager * UP_HeroManager = Cast<UUP_HeroManager>(RGAMEINSTANCE(this)->RWidgetManager->GetUserWidget(EWidgetBluePrintEnum::WBP_Heromanagement_Renewal));
			if (UP_HeroManager)
			{
				UP_HeroManager->Hero_Info->SetVisibility(ESlateVisibility::Collapsed);
				UP_HeroManager->UC_HeroItems->Reset();
			}
			
			//Variables.BackButton->SetVisibility(ESlateVisibility::Visible);
			Variables.ModalImage->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
		}
			break;

		case EScrollviewType::VE_HERO_DECK:
		case EScrollviewType::VE_HERO_DECK_FAVORITE:
		case EScrollviewType::VE_HERO_DECK_SHOWHP:
			//Variables.BackButton->SetVisibility(ESlateVisibility::Collapsed);
			Variables.ModalImage->SetVisibility(ESlateVisibility::Collapsed);
			break;

		default:
			break;
		}

		

		
	}
	else
	{
		PlayAni(TEXT("Appear"), true);

		OnDisappear.ExecuteIfBound();

		switch (Variables.State)
		{
		case EScrollviewType::VE_HERO_PROTECTED:
		case EScrollviewType::VE_HERO_NORMAL:
		case EScrollviewType::VE_HERO_NORMAL_GRIND:
		case EScrollviewType::VE_SPIRIT:
		case EScrollviewType::VE_HERO_UPGRADE:
		case EScrollviewType::VE_HERO_ENCHANT:
		case EScrollviewType::VE_ITEM_WEAPON:
		case EScrollviewType::VE_ITEM_ARMOR_TOP:
		case EScrollviewType::VE_ITEM_ARMOR_BOT:
		case EScrollviewType::VE_ITEM_RING:
		case EScrollviewType::VE_ITEM_AMULET:
		case EScrollviewType::VE_ITEM_UPGRADE:
		case EScrollviewType::VE_BAG_WEAPON:
		case EScrollviewType::VE_BAG_ARMOR_TOP:
		case EScrollviewType::VE_BAG_ARMOR_BOT:
		case EScrollviewType::VE_BAG_RING:
		case EScrollviewType::VE_BAG_AMULET:
		case EScrollviewType::VE_BAG_MISC:
		case EScrollviewType::VE_BAG_WEAPON_SELL:
		case EScrollviewType::VE_BAG_ARMOR_TOP_SELL:
		case EScrollviewType::VE_BAG_ARMOR_BOT_SELL:
		case EScrollviewType::VE_BAG_RING_SELL:
		case EScrollviewType::VE_BAG_AMULET_SELL:
		{
			UUP_HeroManager * UP_HeroManager = Cast<UUP_HeroManager>(RGAMEINSTANCE(this)->RWidgetManager->GetUserWidget(EWidgetBluePrintEnum::WBP_Heromanagement_Renewal));
			if (UP_HeroManager)
			{
				UP_HeroManager->Hero_Info->SetVisibility(ESlateVisibility::SelfHitTestInvisible);

				UP_HeroManager->UC_HeroItems->Reset();
			}
		}
			break;

		case EScrollviewType::VE_HERO_DECK:
		case EScrollviewType::VE_HERO_DECK_FAVORITE:
		case EScrollviewType::VE_HERO_DECK_SHOWHP:
			while (Blackboard.WaitingList.size())
				Blackboard.WaitingList.pop_back();

			StateController.SetState(EUserBehaviorState::IDLE);
			break;

		default:
			break;
		}



		/*check filter is visible*/
		if (Variables.UC_Filter->GetVisibility() != ESlateVisibility::Collapsed)
		{
			Variables.UC_Filter->SetVisibility(ESlateVisibility::Collapsed);
			return;
		}
		else if (Variables.UC_Sorting->GetVisibility() != ESlateVisibility::Collapsed)
		{
			Variables.UC_Sorting->SetVisibility(ESlateVisibility::Collapsed);
			Variables.Switcher_Sorting->SetActiveWidgetIndex(0);
			return;
		}
		/**/

		/*Derived Popup RemovefromViewport*/
		for (size_t i = 0; i < Variables.DerivedPopup.Num(); )
		{
			if (Variables.DerivedPopup[i]->IsInViewport())
			{
				Variables.DerivedPopup[i]->RemoveFromViewport();
				Variables.DerivedPopup.RemoveAt(i);
			}
			else
				++i;
		}
		/**/

		/*State Reset*/
		Variables.PrevState = Variables.State;
		Variables.State = EScrollviewType::VE_END;
	}
}

void UUP_GenericScrollview::Reset()
{
	UCanvasPanelSlot* PanelSlot = Cast<UCanvasPanelSlot>(Variables.BackButton->Slot);
	if (PanelSlot)
	{
		PanelSlot->SetAlignment(FVector2D(1.0f, 0.5f));
	}

	UCanvasPanelSlot* CanvasPanelSlot = Cast<UCanvasPanelSlot>(Variables.VerticalBox->Slot);
	if (CanvasPanelSlot)
	{
		CanvasPanelSlot->SetAlignment(FVector2D(0.f, 1.f));
	}

	Variables.ModalImage->SetRenderOpacity(0.f);


}

void UUP_GenericScrollview::SetState(EScrollviewType eType)
{
	if (Variables.State == eType)
		return;

	Variables.State = eType;
	Variables.bAllSelected = false;
	Renderer.Render();
}


void UUP_GenericScrollview::SetState_Internal(int32 iTabIdx)
{
	if(Variables.CandidateTypeArray.IsValidIndex(iTabIdx))
		SetState(Variables.CandidateTypeArray[iTabIdx]);
}

void UUP_GenericScrollview::Recompose_Itemdatalist(TArray<FString> HeroUDs)
{
	if (Variables.ScrollView)
	{
		Variables.ScrollView->ClearScrollItemData(HeroUDs.Num());

		for (size_t i = 0; i < HeroUDs.Num(); ++i)
		{
			UUC_GenericScrollItemData** FoundItem = Variables.CachedItemDataList.FindByPredicate([HeroUDs, i](UUC_GenericScrollItemData* Source) {
				if (Source->ItemUD == HeroUDs[i])
					return true;
				else
					return false;
			});

			if (FoundItem && *FoundItem)
			{
				Variables.ScrollView->ItemDataList.Emplace(*FoundItem);
			}
		}

		Variables.ScrollView->InvalidateData();
	}
}

bool UUP_GenericScrollview::isInsideViewport()
{
	UCanvasPanelSlot* CanvasPanelSlot = Cast<UCanvasPanelSlot>(Variables.VerticalBox->Slot);
	if (CanvasPanelSlot)
	{
		FMargin margin = CanvasPanelSlot->GetOffsets();
		if (margin.Left == INIT_X)
			return false;
	}
	return true;
}

void UUC_GenericScrollItem::InvalidateData()
{
	Super::InvalidateData();

	UUP_GenericScrollview* GenericScrollview = UUIFunctionLibrary::GetGenericScrollview();
	if (GenericScrollview)
	{
		EScrollviewType scrolltype = GenericScrollview->Variables.State;
		switch (scrolltype)
		{
		case EScrollviewType::VE_HERO_PROTECTED:
		case EScrollviewType::VE_HERO_NORMAL:
		case EScrollviewType::VE_HERO_UPGRADE:
		case EScrollviewType::VE_SPIRIT:
		case EScrollviewType::VE_HERO_DECK:
		case EScrollviewType::VE_HERO_DECK_FAVORITE:
		{
			Switcher_Icon->SetActiveWidgetIndex(0);
			UUC_GenericScrollItemData* itemdata = Cast<UUC_GenericScrollItemData>(ItemData);
			if (itemdata)
			{
				FHERO_ICON_INFO* iconinfo = HeroIcon->GetHeroIconInfoRef();
				if (iconinfo)
					iconinfo->bIsHp = false;

				HeroIcon->SetHeroIconInfoWithUD(itemdata->ItemUD, EHeroOwnershipType::VE_Me);
			}
		}break;
		case EScrollviewType::VE_HERO_NORMAL_GRIND:
		{
			Switcher_Icon->SetActiveWidgetIndex(0);
			UUC_GenericScrollItemData* itemdata = Cast<UUC_GenericScrollItemData>(ItemData);
			if (itemdata)
			{
				FHERO_ICON_INFO* iconinfo = HeroIcon->GetHeroIconInfoRef();
				if (iconinfo)
					iconinfo->bIsHp = false;

				HeroIcon->SetHeroIconInfoWithUD(itemdata->ItemUD, EHeroOwnershipType::VE_Me);
				HeroIcon->SelectIconHero(itemdata->bSelected);
			}
		}break;
		case EScrollviewType::VE_HERO_ENCHANT:
		{
			Switcher_Icon->SetActiveWidgetIndex(0);
			UUC_GenericScrollItemData* itemdata = Cast<UUC_GenericScrollItemData>(ItemData);
			if (itemdata)
			{
				FHERO_ICON_INFO* iconinfo = HeroIcon->GetHeroIconInfoRef();
				if (iconinfo)
					iconinfo->bIsHp = false;

				HeroIcon->SetHeroIconInfoWithUD(itemdata->ItemUD, EHeroOwnershipType::VE_Me);
				HeroIcon->SelectIconHero(itemdata->bSelected);
			}
		}break;
		case EScrollviewType::VE_HERO_DECK_SHOWHP:
		{
			Switcher_Icon->SetActiveWidgetIndex(0);
			UUC_GenericScrollItemData* itemdata = Cast<UUC_GenericScrollItemData>(ItemData);
			if (itemdata)
			{
				bool bFoundSuccess = false;
				UMirrorDungeonManager* MirrorManager = RGAMEINSTANCE(GEngine)->MirrorDungeonManager;
				if (MirrorManager)
				{
					
					for (size_t i = 0 ; i < MirrorManager->MirrorStatus.heroHPs.Num() ; ++i)
					{
						FMIRROR_HERO_HP heroHP = MirrorManager->MirrorStatus.heroHPs[i];
						if (heroHP.heroUD == itemdata->ItemUD)
						{
							FHERO_ICON_INFO heroinfo;
							UUIFunctionLibrary::GetHeroIconInfoWithUD(heroinfo, itemdata->ItemUD, EHeroOwnershipType::VE_Me);

							heroinfo.bIsHp = true;
							if(heroHP.hp <= 0)
								heroinfo.bIsDead = true;
							else
							{
								heroinfo.bIsDead = false;
								heroinfo.HpRate = float(heroHP.hp) / float(heroinfo.MaxHP);
							}
							HeroIcon->SetHeroIconInfo(heroinfo);
							bFoundSuccess = true;
							break;
						}
					}
				}

				if (bFoundSuccess == false)
				{
					FHERO_ICON_INFO heroinfo;
					heroinfo.bIsHp = true;
					heroinfo.HpRate = 1.f;
					UUIFunctionLibrary::GetHeroIconInfoWithUD(heroinfo, itemdata->ItemUD, EHeroOwnershipType::VE_Me);
					HeroIcon->SetHeroIconInfo(heroinfo);
				}
			}
		}break;

		case EScrollviewType::VE_ITEM_WEAPON:
		case EScrollviewType::VE_ITEM_ARMOR_TOP:
		case EScrollviewType::VE_ITEM_ARMOR_BOT:
		case EScrollviewType::VE_ITEM_RING:
		case EScrollviewType::VE_ITEM_AMULET:
		case EScrollviewType::VE_BAG_WEAPON:
		case EScrollviewType::VE_BAG_ARMOR_TOP:
		case EScrollviewType::VE_BAG_ARMOR_BOT:
		case EScrollviewType::VE_BAG_RING:
		case EScrollviewType::VE_BAG_AMULET:
		{
			Switcher_Icon->SetActiveWidgetIndex(1);
			UUC_GenericScrollItemData* itemdata = Cast<UUC_GenericScrollItemData>(ItemData);
			if (itemdata && EquipmentIcon)
			{
				FITEM* Item = RGAMEINSTANCE(this)->RInventory->GetItemDataWithUD(itemdata->ItemUD);
				FItemTableInfo* ItemInfo = UTableManager::GetInstancePtr()->GetCharacterItemRow(Item->itemId);
				if (!ensure(ItemInfo != nullptr))
					return;

				EquipmentIcon->Refresh(ItemInfo, Item, false, false);
			}
		}
			break;

		case EScrollviewType::VE_BAG_WEAPON_SELL:
		case EScrollviewType::VE_BAG_ARMOR_TOP_SELL:
		case EScrollviewType::VE_BAG_ARMOR_BOT_SELL:
		case EScrollviewType::VE_BAG_RING_SELL:
		case EScrollviewType::VE_BAG_AMULET_SELL:
		case EScrollviewType::VE_ITEM_UPGRADE:
		{
			Switcher_Icon->SetActiveWidgetIndex(1);
			UUC_GenericScrollItemData* itemdata = Cast<UUC_GenericScrollItemData>(ItemData);
			if (itemdata && EquipmentIcon)
			{
				FITEM* Item = RGAMEINSTANCE(this)->RInventory->GetItemDataWithUD(itemdata->ItemUD);
				FItemTableInfo* ItemInfo = UTableManager::GetInstancePtr()->GetCharacterItemRow(Item->itemId);
				if (!ensure(ItemInfo != nullptr))
					return;

				if(itemdata->bSelected)
					EquipmentIcon->Refresh(ItemInfo, Item, true, false);
				else
					EquipmentIcon->Refresh(ItemInfo, Item, false, false);
			}
		}break;

		case EScrollviewType::VE_BAG_MISC:
		{
			Switcher_Icon->SetActiveWidgetIndex(2);
			UUC_GenericScrollItemData* itemdata = Cast<UUC_GenericScrollItemData>(ItemData);
			if (itemdata && MaterialIcon)
			{
				FWalletTableInfo* WalletInfo = UTableManager::GetInstancePtr()->GetWalletRow(FName(*itemdata->ItemUD));
				check(WalletInfo != nullptr);

				int32 OwnedAmount = RGAMEINSTANCE(this)->RInventory->GetCurrencyWithKey(FName(*itemdata->ItemUD));
				MaterialIcon->Refresh(WalletInfo->ItemIconKey, OwnedAmount, false, 0);
			}
		}
			break;

		default:
			break;
		}
	}

	

}

void UUC_GenericScrollItem::Selected()
{
	Super::Selected();

	UUP_GenericScrollview* UP_GenericScrollview = UUIFunctionLibrary::GetGenericScrollview();
	if (UP_GenericScrollview)
	{
		EScrollviewType CurrentState = UP_GenericScrollview->Variables.State;

		switch (CurrentState)
		{
		case EScrollviewType::VE_HERO_PROTECTED:
		case EScrollviewType::VE_HERO_NORMAL:
		{
			UUC_GenericScrollItemData* itemdata = Cast<UUC_GenericScrollItemData>(ItemData);
			if (itemdata)
			{
				UUP_HeroManager* UP_HeroManager = Cast<UUP_HeroManager>(RGAMEINSTANCE(this)->RWidgetManager->GetUserWidget(EWidgetBluePrintEnum::WBP_Heromanagement_Renewal));
				if (UP_HeroManager)
				{
					UP_HeroManager->OnClick_HeroIcon(itemdata->ItemUD);
				}
			}
		}break;

		case EScrollviewType::VE_HERO_NORMAL_GRIND:
		{
			UUC_GenericScrollItemData* itemdata = Cast<UUC_GenericScrollItemData>(ItemData);
			if (itemdata)
			{
				HeroIcon->SelectIconHero(itemdata->bSelected);
			}
		}break;

		case EScrollviewType::VE_SPIRIT:
		{
			UUC_GenericScrollItemData * itemdata = Cast<UUC_GenericScrollItemData>(ItemData);
			if (itemdata)
			{
				UUP_HeroManager* UP_HeroManager = Cast<UUP_HeroManager>(RGAMEINSTANCE(this)->RWidgetManager->GetUserWidget(EWidgetBluePrintEnum::WBP_Heromanagement_Renewal));
				if (UP_HeroManager)
				{
					RGAMEINSTANCE(this)->RWidgetManager->AttachUIPage(EUIPageEnum::UIPage_SpiritDetail);
					UUC_Popup_SoulInfo* SpiritInfo = Cast<UUC_Popup_SoulInfo>(RGAMEINSTANCE(this)->RWidgetManager->GetUserWidget(EWidgetBluePrintEnum::WBP_SpiritDetail));
					if (SpiritInfo)
					{
						SpiritInfo->BindData(itemdata->ItemUD);
						UP_GenericScrollview->Variables.DerivedPopup.AddUnique(SpiritInfo);
					}
				}
			}
		}break;

		case EScrollviewType::VE_HERO_UPGRADE:
		{
			UUP_HeroInfo_Upgrade * UP_heroUpgrade = Cast<UUP_HeroInfo_Upgrade>(RGAMEINSTANCE(this)->RWidgetManager->GetUserWidget(EWidgetBluePrintEnum::WBP_HeroRatingUp));
			if (UP_heroUpgrade)
			{
				UP_heroUpgrade->UpgradeHeroInfo->SelectHeroScrollItem(this);
			}
		}break;

		case EScrollviewType::VE_HERO_ENCHANT:
		{
			int32 CurrentSelectedCount = ParentScrollView->GetSelectedScrollItem().Num();
			if (CurrentSelectedCount > UUIFunctionLibrary::GetGenericScrollview()->Variables.MAXENCHANTSPIRIT)
			{
				ParentScrollView->Force_DeSelect(this);
				//popup
			}
			else
			{
				UUC_GenericScrollItemData* itemdata = Cast<UUC_GenericScrollItemData>(ItemData);
				HeroIcon->SelectIconHero(itemdata->bSelected);

				if (IsValid(UP_GenericScrollview->Variables.CurrentSelectedSoulText))
					UP_GenericScrollview->Variables.CurrentSelectedSoulText->SetText(FText::AsNumber(CurrentSelectedCount));
			}
			
		}break;

		case EScrollviewType::VE_HERO_DECK:
		case EScrollviewType::VE_HERO_DECK_FAVORITE:
		{
			URScrollView* ScrollView = ParentScrollView;
			if (!ScrollView)
				break;

			if (!ScrollView->IsSelectEnable)
				break;

			UP_GenericScrollview->Blackboard.SetData_OnBlackBoardData(this);
		}break;
		case EScrollviewType::VE_HERO_DECK_SHOWHP:
		{
			URScrollView* ScrollView = ParentScrollView;
			if (!ScrollView)
				break;

			if (!ScrollView->IsSelectEnable)
				break;

			FHERO_ICON_INFO* heroinfo = HeroIcon->GetHeroIconInfoRef();
			if (heroinfo)
			{
				if (heroinfo->bIsDead)
					break;
			}

			UP_GenericScrollview->Blackboard.SetData_OnBlackBoardData(this);
		}break;


		case EScrollviewType::VE_ITEM_WEAPON:
		case EScrollviewType::VE_ITEM_ARMOR_TOP:
		case EScrollviewType::VE_ITEM_ARMOR_BOT:
		case EScrollviewType::VE_ITEM_RING:
		case EScrollviewType::VE_ITEM_AMULET:
		{
			UUC_GenericScrollItemData * itemdata = Cast<UUC_GenericScrollItemData>(ItemData);
			if (itemdata)
			{
				RGAMEINSTANCE(this)->RWidgetManager->AttachUIPage(EUIPageEnum::UIPage_ItemInfo);
				UUP_ItemInfo* ItemInfo = Cast<UUP_ItemInfo>(RGAMEINSTANCE(this)->RWidgetManager->GetUserWidget(EWidgetBluePrintEnum::WBP_ItemInfo));
				if (ItemInfo)
				{
					ItemInfo->SetItemInfo_Right(itemdata->ItemUD);
					UP_GenericScrollview->Variables.DerivedPopup.AddUnique(ItemInfo);
				}
			}
		}break;

		case EScrollviewType::VE_BAG_WEAPON:
		case EScrollviewType::VE_BAG_ARMOR_TOP:
		case EScrollviewType::VE_BAG_ARMOR_BOT:
		case EScrollviewType::VE_BAG_RING:
		case EScrollviewType::VE_BAG_AMULET:
		{
			UUC_GenericScrollItemData * itemdata = Cast<UUC_GenericScrollItemData>(ItemData);
			if (itemdata)
			{
				RGAMEINSTANCE(this)->RWidgetManager->AttachUIPage(EUIPageEnum::UIPage_ItemInfo);
				UUP_ItemInfo* ItemInfo = Cast<UUP_ItemInfo>(RGAMEINSTANCE(this)->RWidgetManager->GetUserWidget(EWidgetBluePrintEnum::WBP_ItemInfo));
				if (ItemInfo)
				{
					ItemInfo->ItemInfo_L->IsBagMode = true;
					ItemInfo->SetItemInfo_Left(itemdata->ItemUD);
					UP_GenericScrollview->Variables.DerivedPopup.AddUnique(ItemInfo);
				}
			}
		}break;

		case EScrollviewType::VE_BAG_WEAPON_SELL:
		case EScrollviewType::VE_BAG_ARMOR_TOP_SELL:
		case EScrollviewType::VE_BAG_ARMOR_BOT_SELL:
		case EScrollviewType::VE_BAG_RING_SELL:
		case EScrollviewType::VE_BAG_AMULET_SELL:
		{
			UUC_GenericScrollItemData* itemdata = Cast<UUC_GenericScrollItemData>(ItemData);
			if (itemdata && EquipmentIcon)
			{
				FITEM* Item = RGAMEINSTANCE(this)->RInventory->GetItemDataWithUD(itemdata->ItemUD);
				FItemTableInfo* ItemInfo = UTableManager::GetInstancePtr()->GetCharacterItemRow(Item->itemId);
				if (!ensure(ItemInfo != nullptr))
					return;

				if (itemdata->bSelected)
					EquipmentIcon->Refresh(ItemInfo, Item, true, false);
				else
					EquipmentIcon->Refresh(ItemInfo, Item, false, false);
			}
		}break;

		case EScrollviewType::VE_ITEM_UPGRADE:
		{
			int32 MaxMaterialCount = INDEX_NONE;
			UUP_ItemUpgrade* ItemUpgradepage = Cast<UUP_ItemUpgrade>(RGAMEINSTANCE(this)->RWidgetManager->GetUserWidget(EWidgetBluePrintEnum::WBP_Page_ItemUpgrade));
			if (!ItemUpgradepage)
				break;

			FITEM* FItemdata = RGAMEINSTANCE(this)->RInventory->GetItemDataWithUD(ItemUpgradepage->GetItemUD());
			if (!ensure(FItemdata != nullptr))
				return;
			FItemTableInfo* ItemInfo = UTableManager::GetInstancePtr()->GetCharacterItemRow(FItemdata->itemId);
			if (!ensure(ItemInfo != nullptr))
				return;

			FItemUpgradeCostInfo* UpgradeInfo = FItemUpgradeCostInfo::LookUp(ItemInfo->ItemGrade, ItemInfo->EquipType, ItemInfo->ItemLevel);
			if (!ensure(UpgradeInfo))
				return;

			MaxMaterialCount = UpgradeInfo->Material_EquipAmount;


			UUP_GenericScrollview* GenericScrollview = UUIFunctionLibrary::GetGenericScrollview();
			if (GenericScrollview)
			{
				int32 CurrentSelectedCnt = GenericScrollview->Variables.ScrollView->GetSelectedScrollItem().Num();
				if (CurrentSelectedCnt > MaxMaterialCount)
				{
					GenericScrollview->Variables.ScrollView->Force_DeSelect(this);
					return;
				}
			}

			UUC_GenericScrollItemData* itemdata = Cast<UUC_GenericScrollItemData>(ItemData);
			if (itemdata && EquipmentIcon)
			{
				FITEM* Item = RGAMEINSTANCE(this)->RInventory->GetItemDataWithUD(itemdata->ItemUD);
				FItemTableInfo* ItemInfo2 = UTableManager::GetInstancePtr()->GetCharacterItemRow(Item->itemId);
				if (!ensure(ItemInfo2 != nullptr))
					return;

				if (itemdata->bSelected)
					EquipmentIcon->Refresh(ItemInfo2, Item, true, false);
				else
					EquipmentIcon->Refresh(ItemInfo2, Item, false, false);
			}

			ItemUpgradepage->InvalidateData();
		}break;

		case EScrollviewType::VE_BAG_MISC:
		{
			UUC_GenericScrollItemData * itemdata = Cast<UUC_GenericScrollItemData>(ItemData);
			if (itemdata)
			{
				RGAMEINSTANCE(this)->RWidgetManager->AttachUIPage(EUIPageEnum::UIPage_MaterialDetail);
				UUC_WalletItemInfo* ItemInfo = Cast<UUC_WalletItemInfo>(RGAMEINSTANCE(this)->RWidgetManager->GetUserWidget(EWidgetBluePrintEnum::WBP_Popup_WalletItemInfo));
				if (ItemInfo)
				{
					ItemInfo->WalletKey = FName(*itemdata->ItemUD);
					ItemInfo->InvalidateData();
					ItemInfo->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
					UP_GenericScrollview->Variables.DerivedPopup.AddUnique(ItemInfo);
				}
			}
		}break;
			
		}

	}

	

}

void FRenderer_GenericScrollview::Render()
{
	ScrollSettings();
	LayoutSettings();
	
}

void FRenderer_GenericScrollview::LayoutSettings()
{
	URGameInstance* RGameInst = RGAMEINSTANCE(GEngine);
	if (!IsValid(RGameInst))
		return;

	/*Derived Popup RemovefromViewport*/
	for (size_t i = 0; i < variables->DerivedPopup.Num(); )
	{
		if (variables->DerivedPopup[i]->IsInViewport())
		{
			variables->DerivedPopup[i]->RemoveFromViewport();
			variables->DerivedPopup.RemoveAt(i);
		}
		else
			++i;
	}
	/**/

	int32 iMaxTabCount = INDEX_NONE;
	switch (variables->State)
	{
	case EScrollviewType::VE_HERO_PROTECTED:
	case EScrollviewType::VE_HERO_NORMAL:
	case EScrollviewType::VE_SPIRIT:
	{
		iMaxTabCount = 3;
		if (variables->FilterPanel)
			variables->FilterPanel->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
		if (variables->Switcher_Sorting)
			variables->Switcher_Sorting->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
		if (variables->SelectedSoulBox)
			variables->SelectedSoulBox->SetVisibility(ESlateVisibility::Collapsed);
		if (variables->Button_Exe)
		{
			if (variables->State == EScrollviewType::VE_HERO_NORMAL)
			{
				variables->Button_Exe->SetVisibility(ESlateVisibility::Visible);
				if (variables->ButtonName)
					variables->ButtonName->SetText(GetFTextFromStringKey(GRIND_STRINGKEY));
			}
			else
				variables->Button_Exe->SetVisibility(ESlateVisibility::Collapsed);
		}
		if (variables->SwitcherExe)
			variables->SwitcherExe->SetActiveWidgetIndex(0);

		/*TabNameSettings*/
		variables->TabNameArray.Empty(iMaxTabCount);
		variables->TabNameArray.Emplace((PROTECTED_ICONKEY));
		variables->TabNameArray.Emplace((HERO_ICONKEY));
		variables->TabNameArray.Emplace((SPIRIT_ICONKEY));

		/*CandidateArray*/
		variables->CandidateTypeArray.Empty(iMaxTabCount);
		variables->CandidateTypeArray.Emplace(EScrollviewType::VE_HERO_PROTECTED);
		variables->CandidateTypeArray.Emplace(EScrollviewType::VE_HERO_NORMAL);
		variables->CandidateTypeArray.Emplace(EScrollviewType::VE_SPIRIT);
	}
	break;
	case EScrollviewType::VE_HERO_NORMAL_GRIND:
	{
		iMaxTabCount = 3;
		if (variables->FilterPanel)
			variables->FilterPanel->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
		if (variables->Switcher_Sorting)
			variables->Switcher_Sorting->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
		if (variables->SelectedSoulBox)
			variables->SelectedSoulBox->SetVisibility(ESlateVisibility::Collapsed);
		if (variables->SwitcherExe)
			variables->SwitcherExe->SetActiveWidgetIndex(1);

		/*TabNameSettings*/
		variables->TabNameArray.Empty(iMaxTabCount);
		variables->TabNameArray.Emplace((PROTECTED_ICONKEY));
		variables->TabNameArray.Emplace((HERO_ICONKEY));
		variables->TabNameArray.Emplace((SPIRIT_ICONKEY));

		/*CandidateArray*/
		variables->CandidateTypeArray.Empty(iMaxTabCount);
		variables->CandidateTypeArray.Emplace(EScrollviewType::VE_HERO_PROTECTED);
		variables->CandidateTypeArray.Emplace(EScrollviewType::VE_HERO_NORMAL_GRIND);
		variables->CandidateTypeArray.Emplace(EScrollviewType::VE_SPIRIT);
	}break;
	case EScrollviewType::VE_HERO_UPGRADE:
	{
		iMaxTabCount = 1;
		if (variables->FilterPanel)
			variables->FilterPanel->SetVisibility(ESlateVisibility::Collapsed);
		if (variables->Switcher_Sorting)
			variables->Switcher_Sorting->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
		if (variables->SelectedSoulBox)
			variables->SelectedSoulBox->SetVisibility(ESlateVisibility::Collapsed);
		if (variables->Button_Exe)
			variables->Button_Exe->SetVisibility(ESlateVisibility::Collapsed);
		if (variables->SwitcherExe)
			variables->SwitcherExe->SetActiveWidgetIndex(0);

		/*TabNameSettings*/
		variables->TabNameArray.Empty(iMaxTabCount);
		variables->TabNameArray.Emplace((HERO_ICONKEY));

		/*CandidateArray*/
		variables->CandidateTypeArray.Empty(iMaxTabCount);
		variables->CandidateTypeArray.Emplace(EScrollviewType::VE_HERO_UPGRADE);
		
	}
		break;

	case EScrollviewType::VE_HERO_ENCHANT:
	{
		iMaxTabCount = 1;
		if (variables->FilterPanel)
			variables->FilterPanel->SetVisibility(ESlateVisibility::Collapsed);
		if (variables->SelectedSoulBox)
			variables->SelectedSoulBox->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
		if (variables->CurrentSelectedSoulText)
			variables->CurrentSelectedSoulText->SetText(FText::AsNumber(0));

		if (variables->Switcher_Sorting)
			variables->Switcher_Sorting->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
		if (variables->Button_Exe)
			variables->Button_Exe->SetVisibility(ESlateVisibility::Visible);
		if (variables->ButtonName)
			variables->ButtonName->SetText(GetFTextFromStringKey(HEROENCHANTEXT_STRINGKEY));

		if (variables->SwitcherExe)
			variables->SwitcherExe->SetActiveWidgetIndex(0);

		/*TabNameSettings*/
		variables->TabNameArray.Empty(iMaxTabCount);
		variables->TabNameArray.Emplace((SPIRIT_ICONKEY));

		/*CandidateArray*/
		variables->CandidateTypeArray.Empty(iMaxTabCount);
		variables->CandidateTypeArray.Emplace(EScrollviewType::VE_HERO_ENCHANT);
	}break;

	case EScrollviewType::VE_HERO_DECK:
	case EScrollviewType::VE_HERO_DECK_SHOWHP:
	case EScrollviewType::VE_HERO_DECK_FAVORITE:
	{
		iMaxTabCount = 2;
		if (variables->FilterPanel)
			variables->FilterPanel->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
		if (variables->Switcher_Sorting)
			variables->Switcher_Sorting->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
		if (variables->SelectedSoulBox)
			variables->SelectedSoulBox->SetVisibility(ESlateVisibility::Collapsed);
		if (variables->Button_Exe)
			variables->Button_Exe->SetVisibility(ESlateVisibility::Collapsed);
		if (variables->SwitcherExe)
			variables->SwitcherExe->SetActiveWidgetIndex(0);

		/*TabNameSettings*/
		variables->TabNameArray.Empty(iMaxTabCount);
		variables->TabNameArray.Emplace((PROTECTED_ICONKEY));
		variables->TabNameArray.Emplace((HERO_ICONKEY));

		/*CandidateArray*/
		variables->CandidateTypeArray.Empty(iMaxTabCount);
		variables->CandidateTypeArray.Emplace(EScrollviewType::VE_HERO_DECK_FAVORITE);
		if (variables->State == EScrollviewType::VE_HERO_DECK_FAVORITE)
		{
			//only if mirror dungeon deck setting, will show hp decks
			UUserWidget* MirrorDeckWidget = RGameInst->RWidgetManager->GetUserWidget(EWidgetBluePrintEnum::WBP_MirrorDeckSetting);
			if (IsValid(MirrorDeckWidget) && MirrorDeckWidget->IsInViewport())
				variables->CandidateTypeArray.Emplace(EScrollviewType::VE_HERO_DECK_SHOWHP);
			else
				variables->CandidateTypeArray.Emplace(EScrollviewType::VE_HERO_DECK);
		}
		else
			variables->CandidateTypeArray.Emplace(variables->State);
		
	}break;

	case EScrollviewType::VE_ITEM_WEAPON:
	{
		iMaxTabCount = 1;
		if (variables->FilterPanel)
			variables->FilterPanel->SetVisibility(ESlateVisibility::Collapsed);
		if (variables->Switcher_Sorting)
			variables->Switcher_Sorting->SetVisibility(ESlateVisibility::Collapsed);
		if (variables->SelectedSoulBox)
			variables->SelectedSoulBox->SetVisibility(ESlateVisibility::Collapsed);
		if (variables->Button_Exe)
			variables->Button_Exe->SetVisibility(ESlateVisibility::Collapsed);
		if (variables->SwitcherExe)
			variables->SwitcherExe->SetActiveWidgetIndex(0);

		/*TabNameSettings*/
		variables->TabNameArray.Empty(iMaxTabCount);
		variables->TabNameArray.Emplace((WEAPON_ICONKEY));

		/*CandidateArray*/
		variables->CandidateTypeArray.Empty(iMaxTabCount);
		variables->CandidateTypeArray.Emplace(EScrollviewType::VE_ITEM_WEAPON);
	}
		break;

	case EScrollviewType::VE_ITEM_ARMOR_TOP:
	{
		iMaxTabCount = 1;
		if (variables->FilterPanel)
			variables->FilterPanel->SetVisibility(ESlateVisibility::Collapsed);
		if (variables->Switcher_Sorting)
			variables->Switcher_Sorting->SetVisibility(ESlateVisibility::Collapsed);
		if (variables->SelectedSoulBox)
			variables->SelectedSoulBox->SetVisibility(ESlateVisibility::Collapsed);
		if (variables->Button_Exe)
			variables->Button_Exe->SetVisibility(ESlateVisibility::Collapsed);
		if (variables->SwitcherExe)
			variables->SwitcherExe->SetActiveWidgetIndex(0);

		/*TabNameSettings*/
		variables->TabNameArray.Empty(iMaxTabCount);
		variables->TabNameArray.Emplace((TOP_ICONKEY));

		/*CandidateArray*/
		variables->CandidateTypeArray.Empty(iMaxTabCount);
		variables->CandidateTypeArray.Emplace(EScrollviewType::VE_ITEM_ARMOR_TOP);
	}
	break;

	case EScrollviewType::VE_ITEM_ARMOR_BOT:
	{
		iMaxTabCount = 1;
		if (variables->FilterPanel)
			variables->FilterPanel->SetVisibility(ESlateVisibility::Collapsed);
		if (variables->Switcher_Sorting)
			variables->Switcher_Sorting->SetVisibility(ESlateVisibility::Collapsed);
		if (variables->SelectedSoulBox)
			variables->SelectedSoulBox->SetVisibility(ESlateVisibility::Collapsed);
		if (variables->Button_Exe)
			variables->Button_Exe->SetVisibility(ESlateVisibility::Collapsed);
		if (variables->SwitcherExe)
			variables->SwitcherExe->SetActiveWidgetIndex(0);

		/*TabNameSettings*/
		variables->TabNameArray.Empty(iMaxTabCount);
		variables->TabNameArray.Emplace((BOTTOM_ICONKEY));

		/*CandidateArray*/
		variables->CandidateTypeArray.Empty(iMaxTabCount);
		variables->CandidateTypeArray.Emplace(EScrollviewType::VE_ITEM_ARMOR_BOT);
		if (variables->Button_Exe)
			variables->Button_Exe->SetVisibility(ESlateVisibility::Collapsed);
	}
	break;

	case EScrollviewType::VE_ITEM_RING:
	{
		iMaxTabCount = 1;
		if (variables->FilterPanel)
			variables->FilterPanel->SetVisibility(ESlateVisibility::Collapsed);
		if (variables->Switcher_Sorting)
			variables->Switcher_Sorting->SetVisibility(ESlateVisibility::Collapsed);
		if (variables->SelectedSoulBox)
			variables->SelectedSoulBox->SetVisibility(ESlateVisibility::Collapsed);
		if (variables->Button_Exe)
			variables->Button_Exe->SetVisibility(ESlateVisibility::Collapsed);
		if (variables->SwitcherExe)
			variables->SwitcherExe->SetActiveWidgetIndex(0);

		/*TabNameSettings*/
		variables->TabNameArray.Empty(iMaxTabCount);
		variables->TabNameArray.Emplace((RING_ICONKEY));

		/*CandidateArray*/
		variables->CandidateTypeArray.Empty(iMaxTabCount);
		variables->CandidateTypeArray.Emplace(EScrollviewType::VE_ITEM_RING);
	}
	break;

	case EScrollviewType::VE_ITEM_AMULET:
	{
		iMaxTabCount = 1;
		if (variables->FilterPanel)
			variables->FilterPanel->SetVisibility(ESlateVisibility::Collapsed);
		if (variables->Switcher_Sorting)
			variables->Switcher_Sorting->SetVisibility(ESlateVisibility::Collapsed);
		if (variables->SelectedSoulBox)
			variables->SelectedSoulBox->SetVisibility(ESlateVisibility::Collapsed);
		if (variables->Button_Exe)
			variables->Button_Exe->SetVisibility(ESlateVisibility::Collapsed);
		if (variables->SwitcherExe)
			variables->SwitcherExe->SetActiveWidgetIndex(0);

		/*TabNameSettings*/
		variables->TabNameArray.Empty(iMaxTabCount);
		variables->TabNameArray.Emplace((AMULET_ICONKEY));

		/*CandidateArray*/
		variables->CandidateTypeArray.Empty(iMaxTabCount);
		variables->CandidateTypeArray.Emplace(EScrollviewType::VE_ITEM_AMULET);
	}break;

	case EScrollviewType::VE_ITEM_UPGRADE:
	{
		iMaxTabCount = 1;
		if (variables->FilterPanel)
			variables->FilterPanel->SetVisibility(ESlateVisibility::Collapsed);
		if (variables->Switcher_Sorting)
			variables->Switcher_Sorting->SetVisibility(ESlateVisibility::Collapsed);
		if (variables->SelectedSoulBox)
			variables->SelectedSoulBox->SetVisibility(ESlateVisibility::Collapsed);
		if (variables->Button_Exe)
			variables->Button_Exe->SetVisibility(ESlateVisibility::Collapsed);
		if (variables->SwitcherExe)
			variables->SwitcherExe->SetActiveWidgetIndex(0);

		/*TabNameSettings*/
		variables->TabNameArray.Empty(iMaxTabCount);
		variables->TabNameArray.Emplace(FName(TEXT("Upgrade")));

		/*CandidateArray*/
		variables->CandidateTypeArray.Empty(iMaxTabCount);
		variables->CandidateTypeArray.Emplace(EScrollviewType::VE_ITEM_UPGRADE);
	}break;

	case EScrollviewType::VE_BAG_WEAPON:
	case EScrollviewType::VE_BAG_ARMOR_TOP:
	case EScrollviewType::VE_BAG_ARMOR_BOT:
	case EScrollviewType::VE_BAG_RING:
	case EScrollviewType::VE_BAG_AMULET:
	case EScrollviewType::VE_BAG_MISC:
	{
		iMaxTabCount = 6;
		if (variables->FilterPanel)
			variables->FilterPanel->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
		if (variables->Switcher_Sorting)
			variables->Switcher_Sorting->SetVisibility(ESlateVisibility::Collapsed);
		if (variables->SelectedSoulBox)
			variables->SelectedSoulBox->SetVisibility(ESlateVisibility::Collapsed);
		if (variables->Button_Exe)
		{
			if (variables->State != EScrollviewType::VE_BAG_MISC)
			{
				variables->Button_Exe->SetVisibility(ESlateVisibility::Visible);
				if (variables->ButtonName)
					variables->ButtonName->SetText(GetFTextFromStringKey(SELL_STRINGKEY));
			}
			else
				variables->Button_Exe->SetVisibility(ESlateVisibility::Collapsed);
		}
		if (variables->SwitcherExe)
			variables->SwitcherExe->SetActiveWidgetIndex(0);
			

		/*TabNameSettings*/
		variables->TabNameArray.Empty(iMaxTabCount);
		variables->TabNameArray.Emplace((WEAPON_ICONKEY));
		variables->TabNameArray.Emplace((TOP_ICONKEY));
		variables->TabNameArray.Emplace((BOTTOM_ICONKEY));
		variables->TabNameArray.Emplace((RING_ICONKEY));
		variables->TabNameArray.Emplace((AMULET_ICONKEY));
		variables->TabNameArray.Emplace((MISC_ICONKEY));

		/*CandidateArray*/
		variables->CandidateTypeArray.Empty(iMaxTabCount);
		variables->CandidateTypeArray.Emplace(EScrollviewType::VE_BAG_WEAPON);
		variables->CandidateTypeArray.Emplace(EScrollviewType::VE_BAG_ARMOR_TOP);
		variables->CandidateTypeArray.Emplace(EScrollviewType::VE_BAG_ARMOR_BOT);
		variables->CandidateTypeArray.Emplace(EScrollviewType::VE_BAG_RING);
		variables->CandidateTypeArray.Emplace(EScrollviewType::VE_BAG_AMULET);
		variables->CandidateTypeArray.Emplace(EScrollviewType::VE_BAG_MISC);
	}break;

	case EScrollviewType::VE_BAG_WEAPON_SELL:
	{
		iMaxTabCount = 6;
		if (variables->FilterPanel)
			variables->FilterPanel->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
		if (variables->Switcher_Sorting)
			variables->Switcher_Sorting->SetVisibility(ESlateVisibility::Collapsed);
		if (variables->SelectedSoulBox)
			variables->SelectedSoulBox->SetVisibility(ESlateVisibility::Collapsed);
		if (variables->SwitcherExe)
			variables->SwitcherExe->SetActiveWidgetIndex(1);
		

		/*TabNameSettings*/
		variables->TabNameArray.Empty(iMaxTabCount);
		variables->TabNameArray.Emplace((WEAPON_ICONKEY));
		variables->TabNameArray.Emplace((TOP_ICONKEY));
		variables->TabNameArray.Emplace((BOTTOM_ICONKEY));
		variables->TabNameArray.Emplace((RING_ICONKEY));
		variables->TabNameArray.Emplace((AMULET_ICONKEY));
		variables->TabNameArray.Emplace((MISC_ICONKEY));

		/*CandidateArray*/
		variables->CandidateTypeArray.Empty(iMaxTabCount);
		variables->CandidateTypeArray.Emplace(EScrollviewType::VE_BAG_WEAPON_SELL);
		variables->CandidateTypeArray.Emplace(EScrollviewType::VE_BAG_ARMOR_TOP);
		variables->CandidateTypeArray.Emplace(EScrollviewType::VE_BAG_ARMOR_BOT);
		variables->CandidateTypeArray.Emplace(EScrollviewType::VE_BAG_RING);
		variables->CandidateTypeArray.Emplace(EScrollviewType::VE_BAG_AMULET);
		variables->CandidateTypeArray.Emplace(EScrollviewType::VE_BAG_MISC);
	}break;

	case EScrollviewType::VE_BAG_ARMOR_TOP_SELL:
	{
		iMaxTabCount = 6;
		if (variables->FilterPanel)
			variables->FilterPanel->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
		if (variables->Switcher_Sorting)
			variables->Switcher_Sorting->SetVisibility(ESlateVisibility::Collapsed);
		if (variables->SelectedSoulBox)
			variables->SelectedSoulBox->SetVisibility(ESlateVisibility::Collapsed);
		if (variables->SwitcherExe)
			variables->SwitcherExe->SetActiveWidgetIndex(1);

		/*TabNameSettings*/
		variables->TabNameArray.Empty(iMaxTabCount);
		variables->TabNameArray.Emplace((WEAPON_ICONKEY));
		variables->TabNameArray.Emplace((TOP_ICONKEY));
		variables->TabNameArray.Emplace((BOTTOM_ICONKEY));
		variables->TabNameArray.Emplace((RING_ICONKEY));
		variables->TabNameArray.Emplace((AMULET_ICONKEY));
		variables->TabNameArray.Emplace((MISC_ICONKEY));

		/*CandidateArray*/
		variables->CandidateTypeArray.Empty(iMaxTabCount);
		variables->CandidateTypeArray.Emplace(EScrollviewType::VE_BAG_WEAPON);
		variables->CandidateTypeArray.Emplace(EScrollviewType::VE_BAG_ARMOR_TOP_SELL);
		variables->CandidateTypeArray.Emplace(EScrollviewType::VE_BAG_ARMOR_BOT);
		variables->CandidateTypeArray.Emplace(EScrollviewType::VE_BAG_RING);
		variables->CandidateTypeArray.Emplace(EScrollviewType::VE_BAG_AMULET);
		variables->CandidateTypeArray.Emplace(EScrollviewType::VE_BAG_MISC);
	}break;
	case EScrollviewType::VE_BAG_ARMOR_BOT_SELL:
	{
		iMaxTabCount = 6;
		if (variables->FilterPanel)
			variables->FilterPanel->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
		if (variables->Switcher_Sorting)
			variables->Switcher_Sorting->SetVisibility(ESlateVisibility::Collapsed);
		if (variables->SelectedSoulBox)
			variables->SelectedSoulBox->SetVisibility(ESlateVisibility::Collapsed);
		if (variables->SwitcherExe)
			variables->SwitcherExe->SetActiveWidgetIndex(1);

		/*TabNameSettings*/
		variables->TabNameArray.Empty(iMaxTabCount);
		variables->TabNameArray.Emplace((WEAPON_ICONKEY));
		variables->TabNameArray.Emplace((TOP_ICONKEY));
		variables->TabNameArray.Emplace((BOTTOM_ICONKEY));
		variables->TabNameArray.Emplace((RING_ICONKEY));
		variables->TabNameArray.Emplace((AMULET_ICONKEY));
		variables->TabNameArray.Emplace((MISC_ICONKEY));

		/*CandidateArray*/
		variables->CandidateTypeArray.Empty(iMaxTabCount);
		variables->CandidateTypeArray.Emplace(EScrollviewType::VE_BAG_WEAPON);
		variables->CandidateTypeArray.Emplace(EScrollviewType::VE_BAG_ARMOR_TOP);
		variables->CandidateTypeArray.Emplace(EScrollviewType::VE_BAG_ARMOR_BOT_SELL);
		variables->CandidateTypeArray.Emplace(EScrollviewType::VE_BAG_RING);
		variables->CandidateTypeArray.Emplace(EScrollviewType::VE_BAG_AMULET);
		variables->CandidateTypeArray.Emplace(EScrollviewType::VE_BAG_MISC);
	}break;
	case EScrollviewType::VE_BAG_RING_SELL:
	{
		iMaxTabCount = 6;
		if (variables->FilterPanel)
			variables->FilterPanel->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
		if (variables->Switcher_Sorting)
			variables->Switcher_Sorting->SetVisibility(ESlateVisibility::Collapsed);
		if (variables->SelectedSoulBox)
			variables->SelectedSoulBox->SetVisibility(ESlateVisibility::Collapsed);
		if (variables->SwitcherExe)
			variables->SwitcherExe->SetActiveWidgetIndex(1);

		/*TabNameSettings*/
		variables->TabNameArray.Empty(iMaxTabCount);
		variables->TabNameArray.Emplace((WEAPON_ICONKEY));
		variables->TabNameArray.Emplace((TOP_ICONKEY));
		variables->TabNameArray.Emplace((BOTTOM_ICONKEY));
		variables->TabNameArray.Emplace((RING_ICONKEY));
		variables->TabNameArray.Emplace((AMULET_ICONKEY));
		variables->TabNameArray.Emplace((MISC_ICONKEY));

		/*CandidateArray*/
		variables->CandidateTypeArray.Empty(iMaxTabCount);
		variables->CandidateTypeArray.Emplace(EScrollviewType::VE_BAG_WEAPON);
		variables->CandidateTypeArray.Emplace(EScrollviewType::VE_BAG_ARMOR_TOP);
		variables->CandidateTypeArray.Emplace(EScrollviewType::VE_BAG_ARMOR_BOT);
		variables->CandidateTypeArray.Emplace(EScrollviewType::VE_BAG_RING_SELL);
		variables->CandidateTypeArray.Emplace(EScrollviewType::VE_BAG_AMULET);
		variables->CandidateTypeArray.Emplace(EScrollviewType::VE_BAG_MISC);
	}break;
	case EScrollviewType::VE_BAG_AMULET_SELL:
	{
		iMaxTabCount = 6;
		if (variables->FilterPanel)
			variables->FilterPanel->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
		if (variables->Switcher_Sorting)
			variables->Switcher_Sorting->SetVisibility(ESlateVisibility::Collapsed);
		if (variables->SelectedSoulBox)
			variables->SelectedSoulBox->SetVisibility(ESlateVisibility::Collapsed);
		if (variables->SwitcherExe)
			variables->SwitcherExe->SetActiveWidgetIndex(1);

		/*TabNameSettings*/
		variables->TabNameArray.Empty(iMaxTabCount);
		variables->TabNameArray.Emplace((WEAPON_ICONKEY));
		variables->TabNameArray.Emplace((TOP_ICONKEY));
		variables->TabNameArray.Emplace((BOTTOM_ICONKEY));
		variables->TabNameArray.Emplace((RING_ICONKEY));
		variables->TabNameArray.Emplace((AMULET_ICONKEY));
		variables->TabNameArray.Emplace((MISC_ICONKEY));

		/*CandidateArray*/
		variables->CandidateTypeArray.Empty(iMaxTabCount);
		variables->CandidateTypeArray.Emplace(EScrollviewType::VE_BAG_WEAPON);
		variables->CandidateTypeArray.Emplace(EScrollviewType::VE_BAG_ARMOR_TOP);
		variables->CandidateTypeArray.Emplace(EScrollviewType::VE_BAG_ARMOR_BOT);
		variables->CandidateTypeArray.Emplace(EScrollviewType::VE_BAG_RING);
		variables->CandidateTypeArray.Emplace(EScrollviewType::VE_BAG_AMULET_SELL);
		variables->CandidateTypeArray.Emplace(EScrollviewType::VE_BAG_MISC);
	}break;


	default:
		UE_LOG(LogClass, Log, TEXT("State is not reasonable"));
		break;
	}

	if (iMaxTabCount == INDEX_NONE)
		return;

	for (size_t i = 0; i < MAXTABCOUNT; ++i)
	{
		if (variables->CandidateTypeArray.IsValidIndex(i))
		{
			if (variables->CandidateTypeArray[i] == variables->State)
			{
				FButtonStyle style = variables->ButtonArray[i]->WidgetStyle;
				FSlateBrush brush = style.Normal;
				brush.TintColor = FSlateColor(DisableColor);
				style.SetNormal(brush);
				style.SetHovered(brush);
				style.SetPressed(brush);
				variables->ButtonArray[i]->SetStyle(style);

				variables->TabImageArray[i]->SetColorAndOpacity(FLinearColor(1.f, 1.f, 1.f, 1.f));
				variables->UnSelectedImageArray[i]->SetVisibility(ESlateVisibility::Collapsed);
				variables->UnSelectedImageArray2[i]->SetVisibility(ESlateVisibility::Collapsed);
			}
			else
			{
				FButtonStyle style = variables->ButtonArray[i]->WidgetStyle;
				FSlateBrush brush = style.Normal;
				brush.TintColor = FSlateColor(DisableColor);
				style.SetNormal(brush);
				style.SetHovered(brush);
				style.SetPressed(brush);
				variables->ButtonArray[i]->SetStyle(style);

				variables->TabImageArray[i]->SetColorAndOpacity(FLinearColor(0.5f, 0.5f, 0.5f, 1.f));
				variables->UnSelectedImageArray[i]->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
				variables->UnSelectedImageArray2[i]->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
			}
		}

		if (i < iMaxTabCount)
		{
			variables->OverlayArray[i]->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
			if (variables->TabNameArray.IsValidIndex(i))
			{
				variables->TabImageArray[i]->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
				//variables->TabImageArray[i]->SetText(variables->TabNameArray[i]);
				UUIFunctionLibrary::SetImageBrush(UUIFunctionLibrary::GetIconImageFromTable(FName(*variables->TabNameArray[i].ToString())), variables->TabImageArray[i]);
				
			}
				
		}
		else
		{
			variables->OverlayArray[i]->SetVisibility(ESlateVisibility::HitTestInvisible);
			variables->TabImageArray[i]->SetVisibility(ESlateVisibility::Hidden);
			variables->UnSelectedImageArray[i]->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
			variables->UnSelectedImageArray2[i]->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
		}
			
	}

	if (variables->State >= EScrollviewType::VE_ITEM_WEAPON || variables->State == EScrollviewType::VE_HERO_DECK_SHOWHP)
		return;

	/*Sorting*/
	if (IsValid(variables->UC_Sorting) && IsValid(variables->UC_Filter))
	{
		variables->UC_Sorting->SetVisibility(ESlateVisibility::Collapsed);
		variables->UC_Sorting->Filtering();

		variables->UC_Filter->OnClicked_Button_Close();
	}
}

void FRenderer_GenericScrollview::ScrollSettings()
{
	URGameInstance* RGameInst = RGAMEINSTANCE(GEngine);
	if (!IsValid(RGameInst))
		return;
	URInventory* RInventory = RGameInst->RInventory;
	if (!IsValid(RInventory))
		return;

	/*1. ScrollItem Add Begin*/
	if (!variables->ScrollView->ScrollItemList.Num())
		variables->ScrollView->Init_NoClear(MAXSCROLLITEM);
	/*1. ScrollItem Add End*/

	/*2. ScrollItemData Add Begin*/
	variables->CachedItemUDList.Empty();
	switch (variables->State)
	{
	case EScrollviewType::VE_HERO_PROTECTED:
		variables->ScrollView->MultiSelect = false;
		RInventory->GetProtectedHeros(variables->CachedItemUDList);
		break;
	case EScrollviewType::VE_HERO_NORMAL:
	{
		variables->ScrollView->MultiSelect = false;
		RInventory->GetHeroUDArray(variables->CachedItemUDList);
		for (size_t j = 0; j < variables->CachedItemUDList.Num();)
		{
			FHERO* hero = RInventory->GetHeroWithUD(variables->CachedItemUDList[j]);
			if (hero)
			{
				FCharacterTableInfo* tableinfo = UTableManager::GetInstancePtr()->GetCharacterRow(hero->heroId);
				if (tableinfo && !tableinfo->IsHero)
				{
					variables->CachedItemUDList.RemoveAt(j);
					continue;
				}
			}
			++j;
		}
	}
	break;

	case EScrollviewType::VE_HERO_NORMAL_GRIND:
	{
		variables->ScrollView->MultiSelect = true;
		RInventory->GetHeroUDArray(variables->CachedItemUDList);
		for (size_t j = 0; j < variables->CachedItemUDList.Num();)
		{
			FHERO* hero = RInventory->GetHeroWithUD(variables->CachedItemUDList[j]);
			if (hero)
			{
				FCharacterTableInfo* tableinfo = UTableManager::GetInstancePtr()->GetCharacterRow(hero->heroId);
				if (tableinfo && !tableinfo->IsHero)
				{
					variables->CachedItemUDList.RemoveAt(j);
					continue;
				}

				if (hero->locked)
				{
					variables->CachedItemUDList.RemoveAt(j);
					continue;
				}

				ATowerLobbyGameMode* TowerGameMode = Cast<ATowerLobbyGameMode>(UGameplayStatics::GetGameMode(RGameInst->GetWorld()));
				if (TowerGameMode)
				{
					AIndoorLobbyStateActor* IndoorLobbyStateActor = Cast<AIndoorLobbyStateActor>(TowerGameMode->GetLobbyStateActor(ETowerLobbyStateEnum::Indoor));
					if (IndoorLobbyStateActor)
					{
						if (variables->CachedItemUDList[j] == IndoorLobbyStateActor->CurrentHeroUD)
						{
							variables->CachedItemUDList.RemoveAt(j);
							continue;
						}
					}
				}

				int32 FoundIdx = RInventory->GetHeroDeck().arena.Find(hero->heroUD);
				if (FoundIdx != INDEX_NONE)
				{
					variables->CachedItemUDList.RemoveAt(j);
					continue;
				}

				FoundIdx = RInventory->GetHeroDeck().mirror.Find(hero->heroUD);
				if (FoundIdx != INDEX_NONE)
				{
					variables->CachedItemUDList.RemoveAt(j);
					continue;
				}

				FoundIdx = RInventory->GetHeroDeck().defense.Find(hero->heroUD);
				if (FoundIdx != INDEX_NONE)
				{
					variables->CachedItemUDList.RemoveAt(j);
					continue;
				}
			}
			++j;
		}
	}
	break;

	case EScrollviewType::VE_SPIRIT:
	{
		variables->ScrollView->MultiSelect = false;
		RInventory->GetHeroUDArray(variables->CachedItemUDList);
		for (size_t j = 0; j < variables->CachedItemUDList.Num();)
		{
			FHERO* hero = RInventory->GetHeroWithUD(variables->CachedItemUDList[j]);
			if (hero)
			{
				FCharacterTableInfo* tableinfo = UTableManager::GetInstancePtr()->GetCharacterRow(hero->heroId);
				if (tableinfo && tableinfo->IsHero)
				{
					variables->CachedItemUDList.RemoveAt(j);
					continue;
				}
			}
			++j;
		}

		if (IsValid(variables->UC_Sorting))
		{
			variables->UC_Sorting->CurrentType = EHeroListFilterEnum::VE_Grade;
			variables->UC_Sorting->bDescending = true;
		}
	}
	break;

	case EScrollviewType::VE_HERO_UPGRADE:
		variables->ScrollView->MultiSelect = false;
		RInventory->GetHeroUDArray(variables->CachedItemUDList);
		break;
	case EScrollviewType::VE_HERO_ENCHANT:
		variables->ScrollView->MultiSelect = true;
		RInventory->GetHeroUDArray(variables->CachedItemUDList);
		for (size_t j = 0; j < variables->CachedItemUDList.Num();)
		{
			FHERO* hero = RInventory->GetHeroWithUD(variables->CachedItemUDList[j]);
			if (hero)
			{
				FCharacterTableInfo* tableinfo = UTableManager::GetInstancePtr()->GetCharacterRow(hero->heroId);
				if (tableinfo && tableinfo->IsHero)
				{
					variables->CachedItemUDList.RemoveAt(j);
					continue;
				}
			}
			++j;
		}

		if (IsValid(variables->UC_Sorting))
		{
			variables->UC_Sorting->CurrentType = EHeroListFilterEnum::VE_Grade;
			variables->UC_Sorting->bDescending = true;
		}
		break;

	case EScrollviewType::VE_HERO_DECK:
	case EScrollviewType::VE_HERO_DECK_SHOWHP:
	{
		variables->ScrollView->MultiSelect = false;
		RInventory->GetHeroUDArray(variables->CachedItemUDList);
		for (size_t j = 0; j < variables->CachedItemUDList.Num();)
		{
			FHERO* hero = RInventory->GetHeroWithUD(variables->CachedItemUDList[j]);
			if (hero)
			{
				//check if elite mode
				
				UBaseStageInfo *BaseStageInfo = UGamePlayBluePrintFunction::GetBaseStageInfo();
				bool bOnlyHero = !BaseStageInfo->bPlayAlone;
				if (BaseStageInfo)
				{
					FFloorStageDetail *FloorStageTable = UTableManager::GetInstancePtr()->GetFloorStageDetailRow(BaseStageInfo->StageID);
					if (FloorStageTable)
					{
						if (FloorStageTable->PlayType.ToString().Contains(TEXT("N")))
							bOnlyHero = true;
						else if (FloorStageTable->PlayType.ToString().Contains(TEXT("H")))
							bOnlyHero = false;
					}
				}
				UUP_GenericScrollview* UP_GenericScrollview = UUIFunctionLibrary::GetGenericScrollview();
				if (UP_GenericScrollview)
				{
					size_t SelectedIdx = INDEX_NONE;

					UUserWidget* widget = UP_GenericScrollview->Blackboard.WaitingList.front();
					URBaseDeckWidget *Waiting_Deck = Cast<URBaseDeckWidget>(widget);
					if (Waiting_Deck)
						SelectedIdx = UP_GenericScrollview->Blackboard.TargetWidgetArray.Find(Waiting_Deck);

					if (SelectedIdx == ROLE_LEADER_INDEX)
						bOnlyHero = true;

					FCharacterTableInfo* tableinfo = UTableManager::GetInstancePtr()->GetCharacterRow(hero->heroId);
					if (tableinfo)
					{
						if (!tableinfo->IsHero && bOnlyHero == true)
						{
							variables->CachedItemUDList.RemoveAt(j);
							continue;
						}
						else if (tableinfo->IsHero && bOnlyHero == false)
						{
							variables->CachedItemUDList.RemoveAt(j);
							continue;
						}
					}


					bool bExe = false;
					for (size_t i = 0; i < UP_GenericScrollview->Blackboard.SelectedHeroUDs.Num(); ++i)
					{
						if (UP_GenericScrollview->Blackboard.SelectedHeroUDs.IsValidIndex(i))
						{
							if (!UP_GenericScrollview->Blackboard.SelectedHeroUDs[i].IsEmpty())
							{
								FHERO* hero2 = RInventory->GetHeroWithUD(UP_GenericScrollview->Blackboard.SelectedHeroUDs[i]);
								if (hero2)
								{
									if (i == SelectedIdx)
									{
										if (hero->heroUD == hero2->heroUD)
										{
											variables->CachedItemUDList.RemoveAt(j);
											bExe = true;
											break;
										}
									}
									else
									{
										FCharacterTableInfo* tableinfo2 = UTableManager::GetInstancePtr()->GetCharacterRow(hero2->heroId);
										if (tableinfo2)
										{
											if (tableinfo2->DisplayName == tableinfo->DisplayName)
											{
												variables->CachedItemUDList.RemoveAt(j);
												bExe = true;
												break;
											}
										}
									}

								}
							}
						}
					}

					if (bExe)
						continue;
				}
			}
			++j;
		}
		if (variables->State == EScrollviewType::VE_HERO_DECK_SHOWHP)
		{
			variables->CachedItemUDList.Sort([](const FString& Source, const FString& Dest) {
				FHERO_ICON_INFO SourceHeroInfo;
				UUIFunctionLibrary::GetHeroIconInfoWithUD(SourceHeroInfo, Source, EHeroOwnershipType::VE_Me);
				FHERO_ICON_INFO DestHeroInfo;
				UUIFunctionLibrary::GetHeroIconInfoWithUD(DestHeroInfo, Dest, EHeroOwnershipType::VE_Me);

				int32 SourceMaxHP = SourceHeroInfo.MaxHP;
				int32 DestMaxHP = DestHeroInfo.MaxHP;
				int32 SourceCurHP = RGAMEINSTANCE(GEngine)->MirrorDungeonManager->FindRemainHP(Source);
				if (SourceCurHP == INDEX_NONE)
					SourceCurHP = SourceMaxHP;
				int32 DestCurHP = RGAMEINSTANCE(GEngine)->MirrorDungeonManager->FindRemainHP(Dest);
				if (DestCurHP == INDEX_NONE)
					DestCurHP = DestMaxHP;

				float SourceHPRate = float(SourceCurHP / SourceMaxHP);
				float DestHPRate = float(DestCurHP / DestMaxHP);

				if (SourceHPRate == DestHPRate)
					return SourceMaxHP > DestMaxHP;
				else
					return SourceHPRate > DestHPRate;
			});
		}
	}break;

	case EScrollviewType::VE_HERO_DECK_FAVORITE:
	{
		variables->ScrollView->MultiSelect = false;
		RInventory->GetProtectedHeros(variables->CachedItemUDList);
		for (size_t j = 0; j < variables->CachedItemUDList.Num();)
		{
			FHERO* hero = RInventory->GetHeroWithUD(variables->CachedItemUDList[j]);
			if (hero)
			{
				//check if elite mode
				UBaseStageInfo *BaseStageInfo = UGamePlayBluePrintFunction::GetBaseStageInfo();
				bool bOnlyHero = !BaseStageInfo->bPlayAlone;
				if (BaseStageInfo)
				{
					FFloorStageDetail *FloorStageTable = UTableManager::GetInstancePtr()->GetFloorStageDetailRow(BaseStageInfo->StageID);
					if (FloorStageTable)
					{
						if (FloorStageTable->PlayType.ToString().Contains(TEXT("N")))
							bOnlyHero = true;
						else if (FloorStageTable->PlayType.ToString().Contains(TEXT("H")))
							bOnlyHero = false;
					}
				}
				UUP_GenericScrollview* UP_GenericScrollview = UUIFunctionLibrary::GetGenericScrollview();
				if (UP_GenericScrollview)
				{
					size_t SelectedIdx = INDEX_NONE;

					UUserWidget* widget = UP_GenericScrollview->Blackboard.WaitingList.front();
					URBaseDeckWidget *Waiting_Deck = Cast<URBaseDeckWidget>(widget);
					if (Waiting_Deck)
						SelectedIdx = UP_GenericScrollview->Blackboard.TargetWidgetArray.Find(Waiting_Deck);

					if (SelectedIdx == ROLE_LEADER_INDEX)
						bOnlyHero = true;

					FCharacterTableInfo* tableinfo = UTableManager::GetInstancePtr()->GetCharacterRow(hero->heroId);
					if (tableinfo)
					{
						if (!tableinfo->IsHero && bOnlyHero == true)
						{
							variables->CachedItemUDList.RemoveAt(j);
							continue;
						}
						else if (tableinfo->IsHero && bOnlyHero == false)
						{
							variables->CachedItemUDList.RemoveAt(j);
							continue;
						}
					}


					bool bExe = false;
					for (size_t i = 0; i < UP_GenericScrollview->Blackboard.SelectedHeroUDs.Num(); ++i)
					{
						if (UP_GenericScrollview->Blackboard.SelectedHeroUDs.IsValidIndex(i))
						{
							if (!UP_GenericScrollview->Blackboard.SelectedHeroUDs[i].IsEmpty())
							{
								FHERO* hero2 = RInventory->GetHeroWithUD(UP_GenericScrollview->Blackboard.SelectedHeroUDs[i]);
								if (hero2)
								{
									if (i == SelectedIdx)
									{
										if (hero->heroUD == hero2->heroUD)
										{
											variables->CachedItemUDList.RemoveAt(j);
											bExe = true;
											break;
										}
									}
									else
									{
										FCharacterTableInfo* tableinfo2 = UTableManager::GetInstancePtr()->GetCharacterRow(hero2->heroId);
										if (tableinfo2)
										{
											if (tableinfo2->DisplayName == tableinfo->DisplayName)
											{
												variables->CachedItemUDList.RemoveAt(j);
												bExe = true;
												break;
											}
										}
									}

								}
							}
						}
					}

					if (bExe)
						continue;
				}
			}
			++j;
		}

	}break;
	
	case EScrollviewType::VE_ITEM_UPGRADE:
	{
		variables->ScrollView->MultiSelect = true;
		UUP_ItemUpgrade* UpgradePage = Cast<UUP_ItemUpgrade>(RGAMEINSTANCE(GEngine)->RWidgetManager->GetUserWidget(EWidgetBluePrintEnum::WBP_Page_ItemUpgrade));
		if (UpgradePage)
		{
			FITEM* ItemData = RInventory->GetItemDataWithUD(UpgradePage->GetItemUD());
			FItemTableInfo* ItemInfo = UTableManager::GetInstancePtr()->GetCharacterItemRow(ItemData->itemId);

			TArray<FString> ItemUDs;
			RInventory->GetUpgradableItems(ItemUDs, *ItemInfo);

			int32 FoundIdx = ItemUDs.Find(ItemData->itemUD);
			if (FoundIdx != INDEX_NONE)
				ItemUDs.RemoveAt(FoundIdx);

			RInventory->SetItemSort(ItemUDs, EFilterSortTypeEnum::VE_AttackPower, true);
			variables->CachedItemUDList = ItemUDs;
		}
		
	}break;

	case EScrollviewType::VE_ITEM_WEAPON:
	case EScrollviewType::VE_BAG_WEAPON:
		{
		variables->ScrollView->MultiSelect = false;
			ATowerLobbyGameMode* TowerGameMode = Cast<ATowerLobbyGameMode>(UGameplayStatics::GetGameMode(RGameInst->GetWorld()));
			if (TowerGameMode)
			{
				AIndoorLobbyStateActor* IndoorLobbyStateActor = Cast<AIndoorLobbyStateActor>(TowerGameMode->GetLobbyStateActor(ETowerLobbyStateEnum::Indoor));
				if (IndoorLobbyStateActor)
				{
					FHERO* hero = RInventory->GetHeroWithUD(IndoorLobbyStateActor->CurrentHeroUD);
					FCharacterTableInfo* tableinfo = UTableManager::GetInstancePtr()->GetCharacterRow(hero->heroId);
					if (tableinfo)
					{
						TArray<FString> ItemUDs;
						if (variables->State == EScrollviewType::VE_ITEM_WEAPON)
							RInventory->GetWeaponKeys(ItemUDs, tableinfo->AttackType);
						else
							RInventory->GetItemEquipmentKeys(ItemUDs, EItemTypeEnum::VE_EquipItem, EItemEquipPositionEnum::VE_Weapon, EItemEquipTypeEnum::VE_None);
						
						RInventory->SetItemSort(ItemUDs, EFilterSortTypeEnum::VE_AttackPower, true);
						variables->CachedItemUDList = ItemUDs;
					}
				}
			}
		}break;

	case EScrollviewType::VE_BAG_WEAPON_SELL:
	{
		variables->ScrollView->MultiSelect = true;
		ATowerLobbyGameMode* TowerGameMode = Cast<ATowerLobbyGameMode>(UGameplayStatics::GetGameMode(RGameInst->GetWorld()));
		if (TowerGameMode)
		{
			AIndoorLobbyStateActor* IndoorLobbyStateActor = Cast<AIndoorLobbyStateActor>(TowerGameMode->GetLobbyStateActor(ETowerLobbyStateEnum::Indoor));
			if (IndoorLobbyStateActor)
			{
				FHERO* hero = RInventory->GetHeroWithUD(IndoorLobbyStateActor->CurrentHeroUD);
				FCharacterTableInfo* tableinfo = UTableManager::GetInstancePtr()->GetCharacterRow(hero->heroId);
				if (tableinfo)
				{
					TArray<FString> ItemUDs;
					RInventory->GetSellableItems(ItemUDs, EItemTypeEnum::VE_EquipItem, EItemEquipPositionEnum::VE_Weapon,EItemEquipTypeEnum::VE_None);
					RInventory->SetItemSort(ItemUDs, EFilterSortTypeEnum::VE_AttackPower, true);
					variables->CachedItemUDList = ItemUDs;
				}
			}
		}
	}break;

	case EScrollviewType::VE_ITEM_ARMOR_TOP:
	case EScrollviewType::VE_BAG_ARMOR_TOP:
	{
		variables->ScrollView->MultiSelect = false;
		ATowerLobbyGameMode* TowerGameMode = Cast<ATowerLobbyGameMode>(UGameplayStatics::GetGameMode(RGameInst->GetWorld()));
		if (TowerGameMode)
		{
			AIndoorLobbyStateActor* IndoorLobbyStateActor = Cast<AIndoorLobbyStateActor>(TowerGameMode->GetLobbyStateActor(ETowerLobbyStateEnum::Indoor));
			if (IndoorLobbyStateActor)
			{
				FHERO* hero = RInventory->GetHeroWithUD(IndoorLobbyStateActor->CurrentHeroUD);
				FCharacterTableInfo* tableinfo = UTableManager::GetInstancePtr()->GetCharacterRow(hero->heroId);
				if (tableinfo)
				{
					TArray<FString> ItemUDs;
					RInventory->GetItemEquipmentKeys(ItemUDs, EItemTypeEnum::VE_EquipItem, EItemEquipPositionEnum::VE_Armor);
					RInventory->SetItemSort(ItemUDs, EFilterSortTypeEnum::VE_AttackPower, true);
					variables->CachedItemUDList = ItemUDs;
				}
			}
		}
	}break;

	case EScrollviewType::VE_BAG_ARMOR_TOP_SELL:
	{
		variables->ScrollView->MultiSelect = true;
		ATowerLobbyGameMode* TowerGameMode = Cast<ATowerLobbyGameMode>(UGameplayStatics::GetGameMode(RGameInst->GetWorld()));
		if (TowerGameMode)
		{
			AIndoorLobbyStateActor* IndoorLobbyStateActor = Cast<AIndoorLobbyStateActor>(TowerGameMode->GetLobbyStateActor(ETowerLobbyStateEnum::Indoor));
			if (IndoorLobbyStateActor)
			{
				FHERO* hero = RInventory->GetHeroWithUD(IndoorLobbyStateActor->CurrentHeroUD);
				FCharacterTableInfo* tableinfo = UTableManager::GetInstancePtr()->GetCharacterRow(hero->heroId);
				if (tableinfo)
				{
					TArray<FString> ItemUDs;
					RInventory->GetSellableItems(ItemUDs, EItemTypeEnum::VE_EquipItem, EItemEquipPositionEnum::VE_Armor, EItemEquipTypeEnum::VE_None);
					RInventory->SetItemSort(ItemUDs, EFilterSortTypeEnum::VE_AttackPower, true);
					variables->CachedItemUDList = ItemUDs;
				}
			}
		}
	}break;

	case EScrollviewType::VE_ITEM_ARMOR_BOT:
	case EScrollviewType::VE_BAG_ARMOR_BOT:
	{
		variables->ScrollView->MultiSelect = false;
		ATowerLobbyGameMode* TowerGameMode = Cast<ATowerLobbyGameMode>(UGameplayStatics::GetGameMode(RGameInst->GetWorld()));
		if (TowerGameMode)
		{
			AIndoorLobbyStateActor* IndoorLobbyStateActor = Cast<AIndoorLobbyStateActor>(TowerGameMode->GetLobbyStateActor(ETowerLobbyStateEnum::Indoor));
			if (IndoorLobbyStateActor)
			{
				FHERO* hero = RInventory->GetHeroWithUD(IndoorLobbyStateActor->CurrentHeroUD);
				FCharacterTableInfo* tableinfo = UTableManager::GetInstancePtr()->GetCharacterRow(hero->heroId);
				if (tableinfo)
				{
					TArray<FString> ItemUDs;
					RInventory->GetItemEquipmentKeys(ItemUDs, EItemTypeEnum::VE_EquipItem, EItemEquipPositionEnum::VE_Pants);
					RInventory->SetItemSort(ItemUDs, EFilterSortTypeEnum::VE_AttackPower, true);
					variables->CachedItemUDList = ItemUDs;
				}
			}
		}
	}
	break;

	case EScrollviewType::VE_BAG_ARMOR_BOT_SELL:
	{
		variables->ScrollView->MultiSelect = true;
		ATowerLobbyGameMode* TowerGameMode = Cast<ATowerLobbyGameMode>(UGameplayStatics::GetGameMode(RGameInst->GetWorld()));
		if (TowerGameMode)
		{
			AIndoorLobbyStateActor* IndoorLobbyStateActor = Cast<AIndoorLobbyStateActor>(TowerGameMode->GetLobbyStateActor(ETowerLobbyStateEnum::Indoor));
			if (IndoorLobbyStateActor)
			{
				FHERO* hero = RInventory->GetHeroWithUD(IndoorLobbyStateActor->CurrentHeroUD);
				FCharacterTableInfo* tableinfo = UTableManager::GetInstancePtr()->GetCharacterRow(hero->heroId);
				if (tableinfo)
				{
					TArray<FString> ItemUDs;
					RInventory->GetSellableItems(ItemUDs, EItemTypeEnum::VE_EquipItem, EItemEquipPositionEnum::VE_Pants, EItemEquipTypeEnum::VE_None);
					RInventory->SetItemSort(ItemUDs, EFilterSortTypeEnum::VE_AttackPower, true);
					variables->CachedItemUDList = ItemUDs;
				}
			}
		}
	}break;

	case EScrollviewType::VE_ITEM_RING:
	case EScrollviewType::VE_BAG_RING:
	{
		variables->ScrollView->MultiSelect = false;
		ATowerLobbyGameMode* TowerGameMode = Cast<ATowerLobbyGameMode>(UGameplayStatics::GetGameMode(RGameInst->GetWorld()));
		if (TowerGameMode)
		{
			AIndoorLobbyStateActor* IndoorLobbyStateActor = Cast<AIndoorLobbyStateActor>(TowerGameMode->GetLobbyStateActor(ETowerLobbyStateEnum::Indoor));
			if (IndoorLobbyStateActor)
			{
				FHERO* hero = RInventory->GetHeroWithUD(IndoorLobbyStateActor->CurrentHeroUD);
				FCharacterTableInfo* tableinfo = UTableManager::GetInstancePtr()->GetCharacterRow(hero->heroId);
				if (tableinfo)
				{
					TArray<FString> ItemUDs;
					RInventory->GetItemEquipmentKeys(ItemUDs, EItemTypeEnum::VE_EquipItem, EItemEquipPositionEnum::VE_Ring);
					RInventory->SetItemSort(ItemUDs, EFilterSortTypeEnum::VE_AttackPower, true);
					variables->CachedItemUDList = ItemUDs;
				}
			}
		}
	}
	break;

	case EScrollviewType::VE_BAG_RING_SELL:
	{
		variables->ScrollView->MultiSelect = true;
		ATowerLobbyGameMode* TowerGameMode = Cast<ATowerLobbyGameMode>(UGameplayStatics::GetGameMode(RGameInst->GetWorld()));
		if (TowerGameMode)
		{
			AIndoorLobbyStateActor* IndoorLobbyStateActor = Cast<AIndoorLobbyStateActor>(TowerGameMode->GetLobbyStateActor(ETowerLobbyStateEnum::Indoor));
			if (IndoorLobbyStateActor)
			{
				FHERO* hero = RInventory->GetHeroWithUD(IndoorLobbyStateActor->CurrentHeroUD);
				FCharacterTableInfo* tableinfo = UTableManager::GetInstancePtr()->GetCharacterRow(hero->heroId);
				if (tableinfo)
				{
					TArray<FString> ItemUDs;
					RInventory->GetSellableItems(ItemUDs, EItemTypeEnum::VE_EquipItem, EItemEquipPositionEnum::VE_Ring, EItemEquipTypeEnum::VE_None);
					RInventory->SetItemSort(ItemUDs, EFilterSortTypeEnum::VE_AttackPower, true);
					variables->CachedItemUDList = ItemUDs;
				}
			}
		}
	}break;

	case EScrollviewType::VE_ITEM_AMULET:
	case EScrollviewType::VE_BAG_AMULET:
	{
		variables->ScrollView->MultiSelect = false;
		ATowerLobbyGameMode* TowerGameMode = Cast<ATowerLobbyGameMode>(UGameplayStatics::GetGameMode(RGameInst->GetWorld()));
		if (TowerGameMode)
		{
			AIndoorLobbyStateActor* IndoorLobbyStateActor = Cast<AIndoorLobbyStateActor>(TowerGameMode->GetLobbyStateActor(ETowerLobbyStateEnum::Indoor));
			if (IndoorLobbyStateActor)
			{
				FHERO* hero = RInventory->GetHeroWithUD(IndoorLobbyStateActor->CurrentHeroUD);
				FCharacterTableInfo* tableinfo = UTableManager::GetInstancePtr()->GetCharacterRow(hero->heroId);
				if (tableinfo)
				{
					TArray<FString> ItemUDs;
					RInventory->GetItemEquipmentKeys(ItemUDs, EItemTypeEnum::VE_EquipItem, EItemEquipPositionEnum::VE_Necklace);
					RInventory->SetItemSort(ItemUDs, EFilterSortTypeEnum::VE_AttackPower, true);
					variables->CachedItemUDList = ItemUDs;
				}
			}
		}
	}
	break;

	case EScrollviewType::VE_BAG_AMULET_SELL:
	{
		variables->ScrollView->MultiSelect = true;
		ATowerLobbyGameMode* TowerGameMode = Cast<ATowerLobbyGameMode>(UGameplayStatics::GetGameMode(RGameInst->GetWorld()));
		if (TowerGameMode)
		{
			AIndoorLobbyStateActor* IndoorLobbyStateActor = Cast<AIndoorLobbyStateActor>(TowerGameMode->GetLobbyStateActor(ETowerLobbyStateEnum::Indoor));
			if (IndoorLobbyStateActor)
			{
				FHERO* hero = RInventory->GetHeroWithUD(IndoorLobbyStateActor->CurrentHeroUD);
				FCharacterTableInfo* tableinfo = UTableManager::GetInstancePtr()->GetCharacterRow(hero->heroId);
				if (tableinfo)
				{
					TArray<FString> ItemUDs;
					RInventory->GetSellableItems(ItemUDs, EItemTypeEnum::VE_EquipItem, EItemEquipPositionEnum::VE_Necklace, EItemEquipTypeEnum::VE_None);
					RInventory->SetItemSort(ItemUDs, EFilterSortTypeEnum::VE_AttackPower, true);
					variables->CachedItemUDList = ItemUDs;
				}
			}
		}
	}break;

	case EScrollviewType::VE_BAG_MISC:
	{
		variables->ScrollView->MultiSelect = false;
		TArray<FString> WalletKeyArray;

		TArray<FName> WalletKeys = UTableManager::GetInstancePtr()->DTWallet->GetRowNames();
		for (FName WalletKey : WalletKeys)
		{
			FWalletTableInfo* WalletInfo = UTableManager::GetInstancePtr()->GetWalletRow(WalletKey);
			if (!ensure(WalletInfo != nullptr))
				continue;

			if (WalletInfo->PropertyType == EWalletPropertyType::Material)
			{
				int32 Amount = RGAMEINSTANCE(GEngine)->RInventory->GetCurrencyWithKey(WalletKey);
				if (Amount > 0)
				{
					WalletKeyArray.Add(WalletKey.ToString());
				}
			}
		}

		// Sort
		WalletKeyArray.Sort([&](FString WalletKey_A, FString WalletKey_B) {
			FWalletTableInfo* WalletInfo_A = UTableManager::GetInstancePtr()->GetWalletRow(FName(*WalletKey_A));
			check(WalletInfo_A != nullptr);
			FWalletTableInfo* WalletInfo_B = UTableManager::GetInstancePtr()->GetWalletRow(FName(*WalletKey_B));
			check(WalletInfo_B != nullptr);

			if (WalletInfo_A->Sorting_Order < WalletInfo_B->Sorting_Order)
			{
				return true;
			}
			else if (WalletInfo_A->Sorting_Order > WalletInfo_B->Sorting_Order)
			{
				return false;
			}

			return false;
		});

		variables->CachedItemUDList = WalletKeyArray;
	}
		break;

	default:
		UE_LOG(LogClass, Log, TEXT("State is not reasonable"));
		break;
	}

	bool bHeroInven = FMath::IsWithin(variables->State, EScrollviewType::VE_HERO_PROTECTED, EScrollviewType::VE_SPIRIT);
	bool bItemInven = FMath::IsWithin(variables->State, EScrollviewType::VE_ITEM_WEAPON, EScrollviewType::VE_BAG_MISC);
	if (IsValid(variables->TotalHeroCountText))
	{
		if(bHeroInven)
			variables->TotalHeroCountText->SetText(FText::AsNumber(RInventory->GetMaxHeroInventoryCount()));
		if(bItemInven)
			variables->TotalHeroCountText->SetText(FText::AsNumber(RInventory->GetMaxItemInventoryCount()));
	}
	if (IsValid(variables->CurrentHeroCountText))
	{
		if(bHeroInven)
			variables->CurrentHeroCountText->SetText(FText::AsNumber(RInventory->GetCurrentHeroInventoryCount()));
		if (bItemInven)
			variables->CurrentHeroCountText->SetText(FText::AsNumber(RInventory->GetCurrentItemInventoryCount()));
	}

	variables->ScrollView->ClearScrollItemData(MAXSCROLLITEMDATA);
	if (variables->CachedItemUDList.Num() == 0)
	{
		variables->SwitcherScrollview->SetActiveWidgetIndex(1);
	}
	else
	{
		variables->SwitcherScrollview->SetActiveWidgetIndex(0);

		for (size_t i = 0; i < variables->CachedItemUDList.Num(); ++i)
		{
			if (i >= MAXSCROLLITEMDATA)
				break;

			if (variables->CachedItemUDList[i].IsEmpty())
				continue;

			if (variables->CachedItemDataList.IsValidIndex(i))
			{
				variables->CachedItemDataList[i]->ItemUD = TEXT("");

				UUC_GenericScrollItemData* ScrollItemData = variables->CachedItemDataList[i];
				if (ScrollItemData)
				{
					ScrollItemData->bSelected = false;
					ScrollItemData->ItemUD = variables->CachedItemUDList[i];
					variables->ScrollView->ItemDataList.Emplace(ScrollItemData);
				}
			}
		}
	}

	
		

	/*3. Invalidate Begin*/
	variables->ScrollView->InvalidateData();
	/*3. Invalidate Begin End*/
}

FText FRenderer_GenericScrollview::GetFTextFromStringKey(FName name)
{
	return UUtilFunctionLibrary::GetLocalizedString(EStringTableType::VE_UI, name);
}

