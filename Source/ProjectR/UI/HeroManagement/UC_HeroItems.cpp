// Fill out your copyright notice in the Description page of Project Settings.

#include "ProjectR.h"
#include "UC_HeroItems.h"
#include "UI/Common/UC_HeroItems_Icon.h"
#include "UI/RWidgetManager.h"
#include "UI/HeroInfo/HeroManager/UP_HeroManager.h"
#include "UI/ItemManagement/UP_ItemInfo.h"
#include "UP_GenericScrollview.h"
#include "UtilFunctionIntegrated.h"

#include "GlobalIntegrated.h"


#include "GameInfo/RInventory.h"

#include "Table/ItemTableInfo.h"

void UUC_HeroItems::NativeConstruct()
{
	Super::NativeConstruct();

	// Gather BP widgets
	ItemIcons.Empty();
	ItemIcons.Push(Icon_Weapon);
	ItemIcons.Push(Icon_Armor);
	ItemIcons.Push(Icon_Pants);
	ItemIcons.Push(Icon_Ring);
	ItemIcons.Push(Icon_Necklace);
	

 	ItemButtons.Empty();
 	ItemButtons.Push(Button_Weapon);
 	ItemButtons.Push(Button_Armor);
 	ItemButtons.Push(Button_Pants);
	ItemButtons.Push(Button_Ring);
 	ItemButtons.Push(Button_Necklace);
 	

	Button_Weapon->OnClicked.AddDynamic(this, &UUC_HeroItems::OnButtonWeaponClicked);
	Button_Armor->OnClicked.AddDynamic(this, &UUC_HeroItems::OnButtonArmorClicked);
	Button_Pants->OnClicked.AddDynamic(this, &UUC_HeroItems::OnButtonPantsClicked);
	Button_Necklace->OnClicked.AddDynamic(this, &UUC_HeroItems::OnButtonNecklaceClicked);
	Button_Ring->OnClicked.AddDynamic(this, &UUC_HeroItems::OnButtonRingClicked);

	// setup
// 	for (int32 i = 0; i < UUC_ItemInventory_NewDesign::NumItemEquipPositions(); i++)
// 	{
// 		ItemIcons[i]->SetEquipPosition(UUC_ItemInventory_NewDesign::TabIndexToItemEquipPosition(i));
// 	}

	UUserWidget* widget = RGAMEINSTANCE(this)->RWidgetManager->GetUserWidget(EWidgetBluePrintEnum::WBP_Heromanagement_Renewal);
	if (widget)
	{
		UUP_HeroManager* UP_HeroManager = Cast<UUP_HeroManager>(widget);
		if (UP_HeroManager)
		{
			UC_HeroInfo = UP_HeroManager->Hero_Info;
		}
	}

	// hook up delegates
}

void UUC_HeroItems::NativeDestruct()
{
	if (IsValid(Button_Weapon))
		Button_Weapon->OnClicked.RemoveAll(this);
	if (IsValid(Button_Armor))
		Button_Armor->OnClicked.RemoveAll(this);
	if (IsValid(Button_Pants))
		Button_Pants->OnClicked.RemoveAll(this);
	if (IsValid(Button_Necklace))
		Button_Necklace->OnClicked.RemoveAll(this);
	if (IsValid(Button_Ring))
		Button_Ring->OnClicked.RemoveAll(this);
	ItemButtons.Reset();
	ItemIcons.Reset();

	Super::NativeDestruct();
}

void UUC_HeroItems::InvalidateData()
{
	Super::InvalidateData();

	if (CurrentHeroUD.IsEmpty())
		return;

	FHERO* HeroData = RGAMEINSTANCE(this)->RInventory->GetHeroWithUD(CurrentHeroUD);
	if (HeroData == nullptr)
	{
		UE_LOG(LogUI, Log, TEXT("UUC_HeroItems::InvalidateData: failed to get HeroData"));
		return;
	}

	// Set up ItemIcons
	int32 NumIcons = FItemTableInfo::NumItemEquipPositions();
	for (int32 i = 0; i < NumIcons; i++)
	{
		EItemEquipPositionEnum EquipPosition = (EItemEquipPositionEnum)i;

		FString ItemUD; // empty string if not equipped at the position
		if (HeroData->equipedItemUDs.IsValidIndex(i) && !HeroData->equipedItemUDs[i].IsEmpty())
		{
			ItemUD = HeroData->equipedItemUDs[i];
			FITEM* ItemData = RGAMEINSTANCE(this)->RInventory->GetItemDataWithUD(ItemUD);
			if (ItemData == nullptr)
			{
				UE_LOG(LogUI, Log, TEXT("UUC_HeroItems::InvalidateData: failed to get ItemData"));
				return;
			}
			FItemTableInfo* ItemInfo = UTableManager::GetInstancePtr()->GetCharacterItemRow(ItemData->itemId);
			if (ItemData == nullptr)
			{
				UE_LOG(LogUI, Log, TEXT("UUC_HeroItems::InvalidateData: failed to get ItemData"));
				return;
			}
		}

		if (ItemIcons.GetAllocatedSize() != 0)
		{
			if (ItemIcons.IsValidIndex(i))
				ItemIcons[i]->SetEquipPosition((EItemEquipPositionEnum)i);
		}

		// set item icon
		InvalidateItemIcon(i, ItemUD);
	}
}

void UUC_HeroItems::Reset()
{
	Icon_Weapon->SetRenderOpacity(1.f);
	Icon_Weapon->Image_Selecting->SetVisibility(ESlateVisibility::Collapsed);

	Icon_Armor->SetRenderOpacity(1.f);
	Icon_Armor->Image_Selecting->SetVisibility(ESlateVisibility::Collapsed);

	Icon_Pants->SetRenderOpacity(1.f);
	Icon_Pants->Image_Selecting->SetVisibility(ESlateVisibility::Collapsed);

	Icon_Necklace->SetRenderOpacity(1.f);
	Icon_Necklace->Image_Selecting->SetVisibility(ESlateVisibility::Collapsed);

	Icon_Ring->SetRenderOpacity(1.f);
	Icon_Ring->Image_Selecting->SetVisibility(ESlateVisibility::Collapsed);
}

void UUC_HeroItems::OnButtonWeaponClicked()
{

	if (UUIFunctionLibrary::GetGenericScrollview()->Variables.State != EScrollviewType::VE_ITEM_WEAPON)
	{
		UUIFunctionLibrary::GetGenericScrollview()->SetState(EScrollviewType::VE_ITEM_WEAPON);
		UUIFunctionLibrary::GetGenericScrollview()->Appear(true);
	}
	else
	{
		UUIFunctionLibrary::GetGenericScrollview()->Appear(false);
		return;
	}
	

	Icon_Weapon->PlayAni(TEXT("Selecting"), false, 0);
	Icon_Weapon->SetRenderOpacity(1.f);
	Icon_Weapon->Image_Selecting->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
	Icon_Armor->StopAni(TEXT("Selecting"));
	Icon_Armor->SetRenderOpacity(0.5f);
	Icon_Armor->Image_Selecting->SetVisibility(ESlateVisibility::Collapsed);
	Icon_Pants->StopAni(TEXT("Selecting"));
	Icon_Pants->SetRenderOpacity(0.5f);
	Icon_Pants->Image_Selecting->SetVisibility(ESlateVisibility::Collapsed);
	Icon_Necklace->StopAni(TEXT("Selecting"));
	Icon_Necklace->SetRenderOpacity(0.5f);
	Icon_Necklace->Image_Selecting->SetVisibility(ESlateVisibility::Collapsed);
	Icon_Ring->StopAni(TEXT("Selecting"));
	Icon_Ring->SetRenderOpacity(0.5f);
	Icon_Ring->Image_Selecting->SetVisibility(ESlateVisibility::Collapsed);

	if (Icon_Weapon->GetItemUD().IsEmpty())
		return;

	RGAMEINSTANCE(this)->RWidgetManager->AttachUIPage(EUIPageEnum::UIPage_ItemInfo);
	UUP_ItemInfo* iteminfo = Cast<UUP_ItemInfo>(RGAMEINSTANCE(this)->RWidgetManager->GetUserWidget(EWidgetBluePrintEnum::WBP_ItemInfo));
	if (iteminfo)
	{
		UUP_GenericScrollview* genericScrollview = UUIFunctionLibrary::GetGenericScrollview();
		if (genericScrollview)
		{
			genericScrollview->Variables.DerivedPopup.AddUnique(iteminfo);
			iteminfo->SetItemInfo_Left(Icon_Weapon->GetItemUD());
		}
	}

}

void UUC_HeroItems::OnButtonArmorClicked()
{
	if (UUIFunctionLibrary::GetGenericScrollview()->Variables.State != EScrollviewType::VE_ITEM_ARMOR_TOP)
	{
		UUIFunctionLibrary::GetGenericScrollview()->SetState(EScrollviewType::VE_ITEM_ARMOR_TOP);
		UUIFunctionLibrary::GetGenericScrollview()->Appear(true);
	}
	else
	{
		UUIFunctionLibrary::GetGenericScrollview()->Appear(false);
		return;
	}


	Icon_Weapon->StopAni(TEXT("Selecting")); 
	Icon_Weapon->SetRenderOpacity(0.5f);
	Icon_Weapon->Image_Selecting->SetVisibility(ESlateVisibility::Collapsed);
	Icon_Armor->PlayAni(TEXT("Selecting"), false, 0);
	Icon_Armor->SetRenderOpacity(1.f);
	Icon_Armor->Image_Selecting->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
	Icon_Pants->StopAni(TEXT("Selecting"));
	Icon_Pants->SetRenderOpacity(0.5f);
	Icon_Pants->Image_Selecting->SetVisibility(ESlateVisibility::Collapsed);
	Icon_Necklace->StopAni(TEXT("Selecting"));
	Icon_Necklace->SetRenderOpacity(0.5f);
	Icon_Necklace->Image_Selecting->SetVisibility(ESlateVisibility::Collapsed);
	Icon_Ring->StopAni(TEXT("Selecting"));
	Icon_Ring->SetRenderOpacity(0.5f);
	Icon_Ring->Image_Selecting->SetVisibility(ESlateVisibility::Collapsed);

	if (Icon_Armor->GetItemUD().IsEmpty())
		return;

	RGAMEINSTANCE(this)->RWidgetManager->AttachUIPage(EUIPageEnum::UIPage_ItemInfo);
	UUP_ItemInfo* iteminfo = Cast<UUP_ItemInfo>(RGAMEINSTANCE(this)->RWidgetManager->GetUserWidget(EWidgetBluePrintEnum::WBP_ItemInfo));
	if (iteminfo)
	{
		UUP_GenericScrollview* genericScrollview = UUIFunctionLibrary::GetGenericScrollview();
		if (genericScrollview)
		{
			genericScrollview->Variables.DerivedPopup.AddUnique(iteminfo);
			iteminfo->SetItemInfo_Left(Icon_Armor->GetItemUD());
		}
	}
}

void UUC_HeroItems::OnButtonPantsClicked()
{
	if (UUIFunctionLibrary::GetGenericScrollview()->Variables.State != EScrollviewType::VE_ITEM_ARMOR_BOT)
	{
		UUIFunctionLibrary::GetGenericScrollview()->SetState(EScrollviewType::VE_ITEM_ARMOR_BOT);
		UUIFunctionLibrary::GetGenericScrollview()->Appear(true);
	}
	else
	{
		UUIFunctionLibrary::GetGenericScrollview()->Appear(false);
		return;
	}
	
	Icon_Weapon->StopAni(TEXT("Selecting"));
	Icon_Weapon->SetRenderOpacity(0.5f);
	Icon_Weapon->Image_Selecting->SetVisibility(ESlateVisibility::Collapsed);
	Icon_Armor->StopAni(TEXT("Selecting")); 
	Icon_Armor->SetRenderOpacity(0.5f);
	Icon_Armor->Image_Selecting->SetVisibility(ESlateVisibility::Collapsed);
	Icon_Pants->PlayAni(TEXT("Selecting"), false, 0);
	Icon_Pants->SetRenderOpacity(1.f);
	Icon_Pants->Image_Selecting->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
	Icon_Necklace->StopAni(TEXT("Selecting"));
	Icon_Necklace->SetRenderOpacity(0.5f);
	Icon_Necklace->Image_Selecting->SetVisibility(ESlateVisibility::Collapsed);
	Icon_Ring->StopAni(TEXT("Selecting"));
	Icon_Ring->SetRenderOpacity(0.5f);
	Icon_Ring->Image_Selecting->SetVisibility(ESlateVisibility::Collapsed);

	if (Icon_Pants->GetItemUD().IsEmpty())
		return;

	RGAMEINSTANCE(this)->RWidgetManager->AttachUIPage(EUIPageEnum::UIPage_ItemInfo);
	UUP_ItemInfo* iteminfo = Cast<UUP_ItemInfo>(RGAMEINSTANCE(this)->RWidgetManager->GetUserWidget(EWidgetBluePrintEnum::WBP_ItemInfo));
	if (iteminfo)
	{
		UUP_GenericScrollview* genericScrollview = UUIFunctionLibrary::GetGenericScrollview();
		if (genericScrollview)
		{
			genericScrollview->Variables.DerivedPopup.AddUnique(iteminfo);
			iteminfo->SetItemInfo_Left(Icon_Pants->GetItemUD());
		}
	}
}

void UUC_HeroItems::OnButtonNecklaceClicked()
{
	if (UUIFunctionLibrary::GetGenericScrollview()->Variables.State != EScrollviewType::VE_ITEM_AMULET)
	{
		UUIFunctionLibrary::GetGenericScrollview()->SetState(EScrollviewType::VE_ITEM_AMULET);
		UUIFunctionLibrary::GetGenericScrollview()->Appear(true);
	}
	else
	{
		UUIFunctionLibrary::GetGenericScrollview()->Appear(false);
		return;
	}

	Icon_Weapon->StopAni(TEXT("Selecting"));
	Icon_Weapon->SetRenderOpacity(0.5f);
	Icon_Weapon->Image_Selecting->SetVisibility(ESlateVisibility::Collapsed);
	Icon_Armor->StopAni(TEXT("Selecting"));
	Icon_Armor->SetRenderOpacity(0.5f);
	Icon_Armor->Image_Selecting->SetVisibility(ESlateVisibility::Collapsed);
	Icon_Pants->StopAni(TEXT("Selecting"));
	Icon_Pants->SetRenderOpacity(0.5f);
	Icon_Pants->Image_Selecting->SetVisibility(ESlateVisibility::Collapsed);
	Icon_Necklace->PlayAni(TEXT("Selecting"), false, 0); 
	Icon_Necklace->SetRenderOpacity(1.f);
	Icon_Necklace->Image_Selecting->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
	Icon_Ring->StopAni(TEXT("Selecting"));
	Icon_Ring->SetRenderOpacity(0.5f);
	Icon_Ring->Image_Selecting->SetVisibility(ESlateVisibility::Collapsed);

	if (Icon_Necklace->GetItemUD().IsEmpty())
		return;

	RGAMEINSTANCE(this)->RWidgetManager->AttachUIPage(EUIPageEnum::UIPage_ItemInfo);
	UUP_ItemInfo* iteminfo = Cast<UUP_ItemInfo>(RGAMEINSTANCE(this)->RWidgetManager->GetUserWidget(EWidgetBluePrintEnum::WBP_ItemInfo));
	if (iteminfo)
	{
		UUP_GenericScrollview* genericScrollview = UUIFunctionLibrary::GetGenericScrollview();
		if (genericScrollview)
		{
			genericScrollview->Variables.DerivedPopup.AddUnique(iteminfo);
			iteminfo->SetItemInfo_Left(Icon_Necklace->GetItemUD());
		}
	}
}

void UUC_HeroItems::OnButtonRingClicked()
{
	if (UUIFunctionLibrary::GetGenericScrollview()->Variables.State != EScrollviewType::VE_ITEM_RING)
	{
		UUIFunctionLibrary::GetGenericScrollview()->SetState(EScrollviewType::VE_ITEM_RING);
		UUIFunctionLibrary::GetGenericScrollview()->Appear(true);
	}
	else
	{
		UUIFunctionLibrary::GetGenericScrollview()->Appear(false);
		return;
	}

	Icon_Weapon->StopAni(TEXT("Selecting"));
	Icon_Weapon->SetRenderOpacity(0.5f);
	Icon_Weapon->Image_Selecting->SetVisibility(ESlateVisibility::Collapsed);
	Icon_Armor->StopAni(TEXT("Selecting"));
	Icon_Armor->SetRenderOpacity(0.5f);
	Icon_Armor->Image_Selecting->SetVisibility(ESlateVisibility::Collapsed);
	Icon_Pants->StopAni(TEXT("Selecting"));
	Icon_Pants->SetRenderOpacity(0.5f);
	Icon_Pants->Image_Selecting->SetVisibility(ESlateVisibility::Collapsed);
	Icon_Necklace->StopAni(TEXT("Selecting")); 
	Icon_Necklace->SetRenderOpacity(0.5f);
	Icon_Necklace->Image_Selecting->SetVisibility(ESlateVisibility::Collapsed);
	Icon_Ring->PlayAni(TEXT("Selecting"), false, 0); 
	Icon_Ring->SetRenderOpacity(1.0f);
	Icon_Ring->Image_Selecting->SetVisibility(ESlateVisibility::SelfHitTestInvisible);

	if (Icon_Ring->GetItemUD().IsEmpty())
		return;

	RGAMEINSTANCE(this)->RWidgetManager->AttachUIPage(EUIPageEnum::UIPage_ItemInfo);
	UUP_ItemInfo* iteminfo = Cast<UUP_ItemInfo>(RGAMEINSTANCE(this)->RWidgetManager->GetUserWidget(EWidgetBluePrintEnum::WBP_ItemInfo));
	if (iteminfo)
	{
		UUP_GenericScrollview* genericScrollview = UUIFunctionLibrary::GetGenericScrollview();
		if (genericScrollview)
		{
			genericScrollview->Variables.DerivedPopup.AddUnique(iteminfo);
			iteminfo->SetItemInfo_Left(Icon_Ring->GetItemUD());
		}
		
	}
}

void UUC_HeroItems::InvalidateItemIcon(int32 Index, FString ItemUD)
{
	if (ItemIcons.IsValidIndex(Index) == false)
		return;

	UUC_HeroItems_Icon* ItemIcon = ItemIcons[Index];

	ItemIcon->SetItemUD(ItemUD);
	ItemIcon->InvalidateData();
}
