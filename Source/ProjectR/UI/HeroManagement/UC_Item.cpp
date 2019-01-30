// Fill out your copyright notice in the Description page of Project Settings.

#include "ProjectR.h"
#include "UC_Item.h"
#include "UI/Common/UC_Item_Icons.h"
#include "UI/RWidgetManager.h"
#include "UtilFunctionIntegrated.h"
#include "GlobalIntegrated.h"


#include "GameInfo/RInventory.h"


void UUC_Item::NativeConstruct()
{
#if LOG_TIME
	double StartTime = FPlatformTime::Seconds();
#endif
	Super::NativeConstruct();

 	RGameInstance = RGAMEINSTANCE(this);
// 	
// 	{
// 		RGameInstance->EventManager->OnClick_ItemInventory_Close.RemoveDynamic(this, &UUC_Item::ClearSelected);
// 		RGameInstance->EventManager->OnClick_ItemInventory_Close.AddDynamic(this, &UUC_Item::ClearSelected);
// 	}
//
	Init();

	
#if LOG_TIME
	double EndTime = FPlatformTime::Seconds();
	double PrintTime = EndTime - StartTime;

	UE_LOG(LogGamePlay, Log, TEXT("%f ____NativeConstruct(%s)"),
		PrintTime, *(this->GetName()));
#endif
}

void UUC_Item::NativeDestruct()
{
// 	if (RGameInstance)
// 	{
// 		RGameInstance->EventManager->OnClick_ItemInventory_Close.RemoveDynamic(this, &UUC_Item::ClearSelected);
// 	}

	ClearSelected();

	Super::NativeDestruct();
}

void UUC_Item::OnWidgetRebuilt()
{
	Super::OnWidgetRebuilt();

	if (Button_AutoEquip)
	{
		Button_AutoEquip->OnClicked.RemoveDynamic(this, &UUC_Item::OnClick_AutoItemEquip);
		Button_AutoEquip->OnClicked.AddDynamic(this, &UUC_Item::OnClick_AutoItemEquip);
	}

	if (WeaponItem_Btn)
	{
		WeaponItem_Btn->OnClicked.RemoveDynamic(this, &UUC_Item::OnClick_Weapon);
		WeaponItem_Btn->OnClicked.AddDynamic(this, &UUC_Item::OnClick_Weapon);
	}
	if (Armor_Btn)
	{
		Armor_Btn->OnClicked.RemoveDynamic(this, &UUC_Item::OnClick_Armor);
		Armor_Btn->OnClicked.AddDynamic(this, &UUC_Item::OnClick_Armor);
	}
	if (NecklaceItem_Btn)
	{
		NecklaceItem_Btn->OnClicked.RemoveDynamic(this, &UUC_Item::OnClick_Necklace);
		NecklaceItem_Btn->OnClicked.AddDynamic(this, &UUC_Item::OnClick_Necklace);
	}
	if (RingItem_Btn)
	{
		RingItem_Btn->OnClicked.RemoveDynamic(this, &UUC_Item::OnClick_Ring);
		RingItem_Btn->OnClicked.AddDynamic(this, &UUC_Item::OnClick_Ring);
	}
	/*if (Button_Awaken)
	{
		Button_Awaken->OnClicked.RemoveDynamic(this, &UUC_Item::OnClick_Awaken);
		Button_Awaken->OnClicked.AddDynamic(this, &UUC_Item::OnClick_Awaken);
	}*/
}

void UUC_Item::Init()
{
	int32 Max = ItemIcons.Num();//(int32)(EItemEquipPositionEnum::VE_EquipPositionMax);
	ItemIconInfoArray.Empty();
	ItemIconInfoArray.SetNum(Max);

	CurrentHeroUD				= "";
	CurrentItemEquipPosition	= EItemEquipPositionEnum::VE_Weapon;
	int32 NotiEnumValue = (int32)ENotifyStateFlagEnum::BM_EquipItemWeapon;
	for (int32 i = 0; i < Max; ++i)
	{
		UUC_Item_Icons* ItemIconElem = ItemIcons[i];
		if (IsValid(ItemIconElem))
		{
			ENotifyStateFlagEnum ItemNoti = (ENotifyStateFlagEnum)(NotiEnumValue + i);
			ItemIconElem->SetNotifyState(ItemNoti);
			ItemIconElem->ItemIconInfo.ItemEquipPosition = (EItemEquipPositionEnum)i;
			ItemIconElem->SetEmptyImage();
			ItemIconElem->SetSelectedItem(false);
		}
	}

	//ClearSelected();
}

void UUC_Item::ClearSelected()
{
	for (const auto& ItemIconElem : ItemIcons)
	{
		ItemIconElem->SetSelectedItem(false);
	}
	CurrentItemEquipPosition = EItemEquipPositionEnum::VE_Weapon;
}

void UUC_Item::InvalidateByHeroUD(const FString& InHeroUD)
{
	//Invalidate Other Hero Info
	if (CurrentHeroUD != InHeroUD)
	{
		ClearSelected();
	}

	CurrentHeroUD = InHeroUD;

	if ( IsValid(RGameInstance->NotifyManager))
	{
		RGameInstance->NotifyManager->CheckHeroUpgradeItemEach(CurrentHeroUD);
		RGameInstance->NotifyManager->CheckHeroLevelEach(CurrentHeroUD);
	}

	InvalidateItemIconInfo();
	Refresh();
}

void UUC_Item::InvalidateItemIconInfo()
{
	FHERO* HeroInvenData = RGameInstance->RInventory->GetHeroWithUD(CurrentHeroUD);
	if (HeroInvenData)
	{
// 		if (IsValid(Button_AutoEquip))
// 		{
// 			if (HeroInvenData->activated)
// 			{
// 				Button_AutoEquip->SetVisibility(ESlateVisibility::Visible);
// 			}
// 			else
// 			{
// 				Button_AutoEquip->SetVisibility(ESlateVisibility::Collapsed);
// 			}
// 		}

		int32 ItemIndex = 0;
		for (auto& HeroEquipedItemUdsElem : HeroInvenData->equipedItemUDs)
		{
			if (ItemIconInfoArray.IsValidIndex(ItemIndex) == false)
			{
				break;
			}

			/*if (HeroEquipedItemUdsElem.IsEmpty())
			{
				++ItemIndex;
				continue;
			}*/

			FITEM_ICON_INFO ItemIconInfo;
			UUIFunctionLibrary::GetItemIconInfoWithUD(ItemIconInfo, HeroEquipedItemUdsElem);

			if (ItemIconInfo.IsValidItemData() == false)
			{
				ItemIconInfo.Clear();
				ItemIconInfo.ItemEquipPosition = static_cast<EItemEquipPositionEnum>(ItemIndex);
				ItemIconInfo.ItemIconRef = UUIFunctionLibrary::GetItemEmptyTextureByEquipPosition(ItemIconInfo.ItemEquipPosition);
			}

			ItemIconInfoArray[ItemIndex] = ItemIconInfo;
			++ItemIndex;
		}
	}
}

void UUC_Item::Refresh()
{
	auto EuipPositionIndex = static_cast<int32>(CurrentItemEquipPosition);
	int32 Max = ItemIconInfoArray.Num();
	for (int32 i = 0; i < Max; ++i)
	{
		ItemIconInfoArray[i].ItemEquipPosition = static_cast<EItemEquipPositionEnum>(i);
		if (ItemIcons.IsValidIndex(i))
		{
			ItemIcons[i]->SetItemIconStructure(ItemIconInfoArray[i]);

			bool bSelected = (i == EuipPositionIndex);
			ItemIcons[i]->SetSelectedItem(bSelected);
		}
	}

	/*
	int32 ItemIconInfoIndex = 0;
	
	for (auto& ItemIconElem : ItemIconInfoArray)
	{
		ItemIconElem.ItemEquipPosition = static_cast<EItemEquipPositionEnum>(ItemIconInfoIndex);
		++ItemIconInfoIndex;
	}

	ItemIconInfoIndex = 0;
	for (const auto& ItemIconElem : ItemIcons)  
	{
		const auto& ItemIconInfo = ItemIconInfoArray[ItemIconInfoIndex];
		ItemIconElem->SetItemIconStructure(ItemIconInfo);		
		++ItemIconInfoIndex;
	}

	auto TempEquipPosition = CurrentItemEquipPosition;
 	ClearSelected();
	CurrentItemEquipPosition = TempEquipPosition;
	auto EuipPositionIndex = static_cast<int32>(CurrentItemEquipPosition);
 	if (ItemIcons.IsValidIndex(EuipPositionIndex))
 	{
 		ItemIcons[EuipPositionIndex]->SetSelectedItem(true);
 	}*/
}

void UUC_Item::OnClick_Weapon()
{
	OnClick_Item(EItemEquipPositionEnum::VE_Weapon);
}
void UUC_Item::OnClick_Armor()
{
	OnClick_Item(EItemEquipPositionEnum::VE_Armor);
}

void UUC_Item::OnClick_Necklace()
{
	OnClick_Item(EItemEquipPositionEnum::VE_Pants);
}

void UUC_Item::OnClick_Ring()
{
	OnClick_Item(EItemEquipPositionEnum::VE_Ring);
}

void UUC_Item::OnClick_Awaken()
{

}

/************************************************************************/
/* UMG Button Event                                                     */
/************************************************************************/
void UUC_Item::OnClick_Item(EItemEquipPositionEnum InItemEquipPosition, bool bEnableClose /*= true*/)
{
	int32 EuipPositionIndex = 0;

	/*
	if (bEnableClose)
	{
		auto TempEquipPosition = CurrentItemEquipPosition;
		ClearSelected();
		CurrentItemEquipPosition = TempEquipPosition;

		if (CurrentItemEquipPosition == EItemEquipPositionEnum::VE_EquipPositionMax 
			|| CurrentItemEquipPosition != InItemEquipPosition)
		{
			CurrentItemEquipPosition = InItemEquipPosition;
			EuipPositionIndex = static_cast<int32>(InItemEquipPosition);
		}
		else
		{
			CurrentItemEquipPosition = EItemEquipPositionEnum::VE_EquipPositionMax;
			EuipPositionIndex = static_cast<int32>(EItemEquipPositionEnum::VE_EquipPositionMax);
		}

		if (ItemIcons.IsValidIndex(EuipPositionIndex))
		{
			ItemIcons[EuipPositionIndex]->SetSelectedItem(true);
		}

		if (CurrentItemEquipPosition == EItemEquipPositionEnum::VE_EquipPositionMax)
		{
			RGameInstance->EventManager->OnClick_ItemInventory_Close.Broadcast();
		}
		else
		{
			RGameInstance->EventManager->OnClickItem_ItemEquipPosition_HeroManagement.Broadcast(EuipPositionIndex, true);
		}
	}
	else*/
	{
		//if (CurrentItemEquipPosition != InItemEquipPosition)
		{
			ClearSelected();

			CurrentItemEquipPosition = InItemEquipPosition;
			EuipPositionIndex = static_cast<int32>(InItemEquipPosition);

			if (ItemIcons.IsValidIndex(EuipPositionIndex) && EuipPositionIndex != 4) //temp awaken
			{
				ItemIcons[EuipPositionIndex]->SetSelectedItem(true);
			}
		}
		
		if (bEnableClose)
		{
			RGameInstance->EventManager->OnClickItem_ItemEquipPosition_HeroManagement.Broadcast(EuipPositionIndex, true);
		}
	}
	RGameInstance->RWidgetManager->CacheEquipPosition = CurrentItemEquipPosition;
}

/*void UUC_Item::ItemSelectImage(EItemEquipPositionEnum InItemEquipPosition)
{
	int32 EuipPositionIndex = static_cast<int32>(CurrentItemEquipPosition);
	if (ItemIcons.IsValidIndex(EuipPositionIndex))
	{
		ItemIcons[EuipPositionIndex]->SetSelectedItem(false);
	}

	CurrentItemEquipPosition = InItemEquipPosition;
	EuipPositionIndex = static_cast<int32>(InItemEquipPosition);

	if (ItemIcons.IsValidIndex(EuipPositionIndex)
		&& CurrentItemEquipPosition != EItemEquipPositionEnum::VE_Awaken)
	{
		ItemIcons[EuipPositionIndex]->SetSelectedItem(true);
	}
}*/

void UUC_Item::OnClick_AutoItemEquip()
{
	RGameInstance->EventManager->OnClick_AutoEquipItem_HeroManagement.Broadcast();
}

/************************************************************************/
/* Getter                                                               */
/************************************************************************/
bool UUC_Item::GetItemIconInfoByEquipPosition(EItemEquipPositionEnum InEquipPosition, FITEM_ICON_INFO& InTargetItemIconInfo)
{
	auto EquipPositionIntValue = static_cast<int32>(InEquipPosition);
	if (ItemIconInfoArray.IsValidIndex(EquipPositionIntValue) == false)
	{
		return false;
	}

	InTargetItemIconInfo = ItemIconInfoArray[EquipPositionIntValue];

	return true;
}

FString UUC_Item::GetItemUDByEquipPosition(EItemEquipPositionEnum InEquipPosition)
{
	auto EquipPositionIntValue = (int32)(InEquipPosition);
	if (ItemIconInfoArray.IsValidIndex(EquipPositionIntValue) == false)
	{
		return "";
	}
	
	return ItemIconInfoArray[EquipPositionIntValue].itemUD;
}