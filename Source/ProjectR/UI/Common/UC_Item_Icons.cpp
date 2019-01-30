// Fill out your copyright notice in the Description page of Project Settings.

#include "ProjectR.h"
#include "SharedConstants/GlobalConstants.h"
#include "UC_Item_Icons.h"

#include "UI/Common/ItemBaseScrollItemData.h"
#include "UI/Control/RScrollItemData.h"
#include "UI/Common/UC_Notify.h"
#include "UtilFunctionIntegrated.h"

#include "GlobalIntegrated.h"




#include "GameInfo/RInventory.h"

#include "Table/ItemTableInfo.h"
#include "Table/CharacterTableInfo.h"

void UUC_Item_Icons::NativeConstruct()
{
	Super::NativeConstruct();

	if (time_remain_text_info)
		time_remain_text_info->SetVisibility(ESlateVisibility::Collapsed);
	/*
	GemOverlayArray.Reset(MaxGemSlot);
	GemOverlayArray.Emplace(Overlay_EquipGem1);
	GemOverlayArray.Emplace(Overlay_EquipGem2);
	GemOverlayArray.Emplace(Overlay_EquipGem3);

	Item_Gems.Reset(MaxGemSlot);
	Item_Gems.Emplace(Image_EquipGem1);
	Item_Gems.Emplace(Image_EquipGem2);
	Item_Gems.Emplace(Image_EquipGem3);*/
}

bool UUC_Item_Icons::IsValidItemData(const FITEM_ICON_INFO& Item_Icon_Info)
{
	return !Item_Icon_Info.itemId.IsNone() || !Item_Icon_Info.itemUD.IsEmpty();
}

void UUC_Item_Icons::OnSetWidgetSizeType(bool bLargeSize)
{
	if (!IsValid(ScaleBox_IconSizeType))
		return;

	UCanvasPanelSlot* MySlot = Cast<UCanvasPanelSlot>(ScaleBox_IconSizeType->Slot);
	if (!IsValid(MySlot))
		return;

	if (bLargeSize)
		MySlot->SetSize(FVector2D(170.f, 170.f));
	else
		MySlot->SetSize(FVector2D(122.f, 122.f));
}

void UUC_Item_Icons::InvalidateData()
{
	//Super::InvalidateData();
	SetToolTipInfo();

	// Set visibility
	SetVisibleBasicIcon();
	SetVisibleCompare();
	
}
void UUC_Item_Icons::SetToolTipInfo()
{
	ToolTipEvent	= ItemIconInfo.ToolTipEventType;
	ToolTipID		= ItemIconInfo.ToolTipID;
}

FItemTableInfo* UUC_Item_Icons::GetItemTableInfo(FName ItemID)
{
	return UTableManager::GetInstancePtr()->GetCharacterItemRow(ItemID);
}

EItemTypeEnum UUC_Item_Icons::GetItemTypeEnum(FName ItemID)
{
	EItemTypeEnum Itemtype = EItemTypeEnum::VE_None;
	
	if (!ItemID.IsNone())
	{
		FItemTableInfo* ItemTable = UTableManager::GetInstancePtr()->GetCharacterItemRow(ItemID);
		if( ItemTable)
			Itemtype = ItemTable->ItemType;
	}
	
	return Itemtype;
}



void UUC_Item_Icons::SetScrollItemData(int32 Index, URScrollItemData* item, URScrollView* ScrollView)
{
	UItemBaseScrollItemData* InvenItemData = Cast< UItemBaseScrollItemData >(item);
	if (InvenItemData)
	{
		IsSelectEnable = item->bSelectEnable && !InvenItemData->ItemIconInfo.bDisable;
		SetItemIconStructure(InvenItemData->ItemIconInfo);
		//SetVisibleEquipInfo(true);
	}
}

void UUC_Item_Icons::SetSelectedItem(bool bSelected)
{
	if (Overlay_Select)
	{
		if( bSelected )
			Overlay_Select->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
		else
			Overlay_Select->SetVisibility(ESlateVisibility::Collapsed);
	}
}

void UUC_Item_Icons::SetNotifyState(ENotifyStateFlagEnum InNotifyState)
{
	if (IsValid(Item_Notify))
	{
		Item_Notify->NotifyState = InNotifyState;
	}
}

void UUC_Item_Icons::SetItemIconStructure(const FITEM_ICON_INFO& InItemIconInfo, bool InIsLargeIcon)
{
	/*if (ItemIconInfo.itemUD.Compare(InItemIconInfo.itemUD) == 0
		&& IsLargeIcon == InIsLargeIcon)
	{
		if (ItemIconInfo.ItemEquipPosition == EItemEquipPositionEnum::VE_NotEquipment)
		{
			ItemIconInfo = InItemIconInfo;
		}
		return;
	}*/

	ItemIconInfo = InItemIconInfo;
	IsLargeIcon = InIsLargeIcon;

	//SetItemIconTypeByItemIconInfo(ItemIconInfo);
	SetVisibleEquipInfo(false);
	InvalidateData();

	OnSetWidgetSizeType(InIsLargeIcon);

	if (ItemIconInfo.itemId.IsNone() == false)
	{
		FItemTableInfo* ItemTableInfo = UTableManager::GetInstancePtr()->GetCharacterItemRow(ItemIconInfo.itemId);
		if (ItemTableInfo != nullptr)
		{
			bool bConsumableItem = ItemTableInfo->ItemType == EItemTypeEnum::VE_ConsumableItem;
			if (bConsumableItem)
			{
				if (IsItemDetailIcon)
				{
					IsSelectEnable = true;
				}
				else
				{
					URGameInstance* RGameInstance = RGAMEINSTANCE(this);
					if (RGameInstance->RInventory->IsTempManageState)
					{
						IsSelectEnable = (RGameInstance->RInventory->bConsumableWidget);
					}
				}
			}
		}
	}

	if (IsValid(Image_Disable))
	{
		if (IsSelectEnable)
			Image_Disable->SetVisibility(ESlateVisibility::Collapsed);
		else
			Image_Disable->SetVisibility(ESlateVisibility::Visible);
		
	}
}

void UUC_Item_Icons::SetEmptyImage()
{
	auto ItemEquipPositionEnumIntValue = static_cast<int32>(ItemIconInfo.ItemEquipPosition);
	auto ItemMaxEuqipPositionEnumIntvalue = static_cast<int32>(EItemEquipPositionEnum::VE_Max);
	if (ItemEquipPositionEnumIntValue >= ItemMaxEuqipPositionEnumIntvalue)
	{
		return;
	}

	auto ItemEmptyTextureByEquipPosition = UUIFunctionLibrary::GetItemEmptyTextureByEquipPosition(ItemIconInfo.ItemEquipPosition);
	if (Image_Icon)
	{
		if (IsValid(ItemEmptyTextureByEquipPosition))
		{
			Image_Icon->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
			//Image_Icon->SetBrushFromTexture(ItemEmptyTextureByEquipPosition);
			UUIFunctionLibrary::SetImageBrush(ItemEmptyTextureByEquipPosition, Image_Icon);

			if (Switcher_Number)
			{
				Switcher_Number->SetVisibility(ESlateVisibility::Collapsed);
			}

			/*if (Text_StackNum)
			{
				Text_StackNum->SetVisibility(ESlateVisibility::Collapsed);
			}*/
		}
	}
}

void UUC_Item_Icons::SetVisibleEquipInfo(bool bVisible)
{
	bIsVisibleEquipInfo = bVisible;

	if (!IsValid(Overlay_EquipHero) || !IsValid(Text_EquipHero_Name) || !IsValid(Image_EquipHero_BG) )
		return;
	
	Overlay_EquipHero->SetVisibility(ESlateVisibility::Collapsed);
	Text_EquipHero_Name->SetVisibility(ESlateVisibility::Collapsed);
	Image_EquipHero_BG->SetVisibility(ESlateVisibility::Collapsed);

	
	URGameInstance* RGameInstance = RGAMEINSTANCE(this);
	if ( !IsValid(RGameInstance->RInventory))
		return;
	
	if (ItemIconInfo.itemUD.IsEmpty())
	{
		if (Switcher_Number)
		{
			Switcher_Number->SetVisibility(ESlateVisibility::Collapsed);
		}

		return;
	}

	FITEM* Item = RGameInstance->RInventory->GetItemDataWithUD(ItemIconInfo.itemUD);
	if (Item == nullptr)
		return;

	ItemLevel = Item->level;

	if (!bIsVisibleEquipInfo)
		return;

	if (!ItemIconInfo.bEquiped)
		return;


	FHERO* Hero = RGameInstance->RInventory->GetHeroWithUD(Item->equipedHeroUD);
	if (Hero == nullptr)
		return;

	FCharacterTableInfo* CharTable = UTableManager::GetInstancePtr()->GetCharacterRow(Hero->heroId);
	if (CharTable == nullptr)
		return;
	Overlay_EquipHero->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
	Text_EquipHero_Name->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
	Image_EquipHero_BG->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
		
	Text_EquipHero_Name->SetText(UUtilFunctionLibrary::GetLocalizedString(EStringTableType::VE_Hero, CharTable->DisplayName));

}

void UUC_Item_Icons::SetItemData(int32 Index, URScrollItemData* item, URScrollView* ScrollView)
{
	SetScrollItemData(Index, item, ScrollView);
}

void UUC_Item_Icons::Init_TimeTreasureReward(FName IconKey, FText InTopText, FText InBottomText)
{
	if (Image_Icon)
		UUIFunctionLibrary::SetImageBrush(UUIFunctionLibrary::GetIconImageFromTable(IconKey), Image_Icon);

	if (TopText)
		TopText->SetText(InTopText);

	if (BottomText)
		BottomText->SetText(InBottomText);

	if (time_remain_text_info)
		time_remain_text_info->SetVisibility(ESlateVisibility::SelfHitTestInvisible);

	if (Switcher_Number)
		Switcher_Number->SetVisibility(ESlateVisibility::Collapsed);
	if (Text_ItemGrade)
		Text_ItemGrade->SetVisibility(ESlateVisibility::Collapsed);
	if (Text_Enchant_Level)
		Text_Enchant_Level->SetVisibility(ESlateVisibility::Collapsed);
}

/*
void UUC_Item_Icons::PlayAnimTrack(FString AniName)
{
	PlayAni(AniName);
}

void UUC_Item_Icons::StopAnimTrack(FString AniName)
{
	StopAni(AniName);
}
*/

void UUC_Item_Icons::SetVisibleBasicIcon()
{
	/*
	Item icon
	*/
	bool bValidItemId = !(ItemIconInfo.itemId.IsNone());

	/*URGameInstance* RGameInstance = UUIFunctionLibrary::GetRGameInstance(GetWorld());
	FItemTableInfo* ItemTableInfo = UTableManager::GetInstancePtr()->GetCharacterItemRow(ItemIconInfo.itemId);
	if (ItemTableInfo)
	{
		FGameDataItemGradeInfo* ItemGradeInfo = RGameInstance->GameDataManager->GetItemGradeInfo(ItemTableInfo->ItemGrade);
		if (ItemGradeInfo)
		{
			ItemGradeInfo->ItemGradeColor;
		}
	}*/

	if (Image_Grade)
	{
		if (bValidItemId)
		{
			UUIFunctionLibrary::SetImageBrush(UUtilFunctionLibrary::GetItemGradeIconImg(ItemIconInfo.itemId), Image_Grade);
		}
	}

	if (Text_ItemGrade)
	{
		FName textKey;
		if (bValidItemId)
		{
			textKey = UUtilFunctionLibrary::GetItemGradeText(ItemIconInfo.itemId);
		}

		if (textKey.IsNone())
		{
			if (Text_ItemGrade->GetVisibility() != ESlateVisibility::Collapsed)
				Text_ItemGrade->SetVisibility(ESlateVisibility::Collapsed);
		}
		else
		{
			if (Text_ItemGrade->GetVisibility() != ESlateVisibility::Visible)
				Text_ItemGrade->SetVisibility(ESlateVisibility::Visible);

			Text_ItemGrade->SetText(UUtilFunctionLibrary::GetLocalizedString(EStringTableType::VE_UI, textKey));
			FSlateColor color = UUtilFunctionLibrary::GetItemGradeColor(ItemIconInfo.itemId);
			Text_ItemGrade->SetColorAndOpacity(color);
		}
	}

	if (Image_Icon)
	{
		if (IsValid(ItemIconInfo.ItemIconRef))
		{
			Image_Icon->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
			UUIFunctionLibrary::SetImageBrush(ItemIconInfo.ItemIconRef, Image_Icon);
		}
		else
		{
			Image_Icon->SetVisibility(ESlateVisibility::Hidden);
		}
	}
		

	if (Text_Enchant_Level)
	{
		if (ItemIconInfo.enchant > 0)
		{
			Text_Enchant_Level->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
			Text_Enchant_Level->SetText(FText::Format(FText::FromString(TEXT("+{0}")), FText::AsNumber(ItemIconInfo.enchant)));
		}
		else
		{
			Text_Enchant_Level->SetVisibility(ESlateVisibility::Collapsed);
		}
	}

// 	if (bIsItemId == false)
// 		return;

	FItemTableInfo* ItemTableInfo = nullptr;
	if (bValidItemId)
	{
		ItemTableInfo = UTableManager::GetInstancePtr()->GetCharacterItemRow(ItemIconInfo.itemId);
	}
// 	if (ItemTableInfo == nullptr)
// 		return;

	bool bIsEquipItem = (ItemTableInfo != nullptr && ItemTableInfo->ItemType == EItemTypeEnum::VE_EquipItem);
	if (bIsEquipItem)
	{
		if (Switcher_Number)
		{
			Switcher_Number->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
			Switcher_Number->SetActiveWidgetIndex(1);
		}

		if (Text_ItemLevel)
		{
			Text_ItemLevel->SetText(FText::AsNumber(ItemLevel));
		}
	}
	else
	{
		if (Switcher_Number)
		{
			Switcher_Number->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
			Switcher_Number->SetActiveWidgetIndex(0);
		}

		if (Text_StackNum)
		{
			if (ItemIconInfo.count > 1)
			{
				Text_StackNum->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
				Text_StackNum->SetText(FText::AsNumber(ItemIconInfo.count));
			}
			else
			{
				Text_StackNum->SetVisibility(ESlateVisibility::Collapsed);
			}
		}
	}

	
	/*
	switch (ItemTableInfo->EquipPosition)
	{
	case EItemEquipPositionEnum::VE_Weapon:
	{
		//if (!ItemTableInfo->EquipHero.IsNone())
		//{
		//	FCharacterTableInfo* CharacterTableInfo = UTableManager::GetInstancePtr()->GetCharacterRow(ItemTableInfo->EquipHero);
		//	if (CharacterTableInfo)
		//	{
		//		Item_Private_Text->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
		//		Item_Private_Text->SetText(UUtilFunctionLibrary::GetLocalizedString(EStringTableType::VE_Hero, CharacterTableInfo->DisplayName));
		//	}
		//}
	}
	break;
	case EItemEquipPositionEnum::VE_Armor:
	case EItemEquipPositionEnum::VE_Pants:
	case EItemEquipPositionEnum::VE_Ring:
	{
	}
	break;
	}
	*/
	/*
	if (Item_Private_Text)
	{
		Item_Private_Text->SetVisibility(ESlateVisibility::Collapsed);

		if (!ItemTableInfo->EquipHero.IsNone())
		{
			FCharacterTableInfo* CharacterTableInfo = UTableManager::GetInstancePtr()->GetCharacterRow(ItemTableInfo->EquipHero);
			if (CharacterTableInfo)
			{
				Item_Private_Text->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
				Item_Private_Text->SetText(UUtilFunctionLibrary::GetLocalizedString(EStringTableType::VE_Hero, CharacterTableInfo->DisplayName));
			}
		}
	}//*/
}


void UUC_Item_Icons::SetVisibleGemIcons()
{
	if (!IsValid(Panel_GemEquip))
		return;

	if (IsValid(Panel_GemEquip))
		Panel_GemEquip->SetVisibility(ESlateVisibility::Collapsed);

	if (ItemIconInfo.itemId.IsNone())
		return;

	FItemTableInfo* ItemTable = GetItemTableInfo(ItemIconInfo.itemId);
	if (ItemTable == nullptr)
		return;

	if (ItemTable->ItemType == EItemTypeEnum::VE_EquipItem)
	{
		switch (ItemTable->EquipPosition)
		{
		case EItemEquipPositionEnum::VE_Weapon:
		case EItemEquipPositionEnum::VE_Armor:
		case EItemEquipPositionEnum::VE_Pants:
		case EItemEquipPositionEnum::VE_Ring:
		// TODO: [서버] 영웅성장 - 영웅 레벨업 / 아이템 랜덤옵션 관련 공유
		/*
		{
			if (IsValid(Panel_GemEquip))
				Panel_GemEquip->SetVisibility(ESlateVisibility::SelfHitTestInvisible);

			int32 ItemGemSlotCount = ItemTable->itemJewelSlotCount;
			int32 index = 0;
			for (UOverlay* gemContainer : GemOverlayArray)
			{
				if (IsValid(gemContainer))
				{
					if (index >= ItemGemSlotCount)
						gemContainer->SetVisibility(ESlateVisibility::Collapsed);
					else
						gemContainer->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
				}

				++index;
			}

			for (UImage* imgGem : Item_Gems)
			{
				imgGem->SetVisibility(ESlateVisibility::Collapsed);
			}


			FString GemID;
			FItemTableInfo* GemItemTable = nullptr;
			for (int32 Index = 0; Index < ItemIconInfo.jewels.Num(); ++Index)
			{
				GemID = ItemIconInfo.jewels[Index];
				if (GemID.IsEmpty())
				{
					Item_Gems[Index]->SetVisibility(ESlateVisibility::Collapsed);
					continue;
				}

				GemItemTable = GetItemTableInfo(FName(*GemID));
				if (GemItemTable && Item_Gems.IsValidIndex(Index))
				{
					UTexture2D* Icon = nullptr;
					if (GemItemTable)
						Icon = UUtilFunctionLibrary::GetTexture2DFromAssetID(GemItemTable->ItemIconRef);

					Item_Gems[Index]->SetBrushFromTexture(Icon);
					Item_Gems[Index]->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
				}
				else
					Item_Gems[Index]->SetVisibility(ESlateVisibility::Collapsed);

			}
		}
	*/
		break;
		}
	}
}

void UUC_Item_Icons::SetVisibleCompare()
{
	if (IsValid(Switcher_ItemCompare))
	{
		Switcher_ItemCompare->SetVisibility(ESlateVisibility::Collapsed);
	}

	if (ItemIconInfo.itemId.IsNone())
		return;

	if (ItemIconInfo.bCompare && ItemIconInfo.CompareValue != 0)
	{
		int32 WidgetIndex = int32(!(ItemIconInfo.bCompareUp));
		if (IsValid(Switcher_ItemCompare))
		{
			Switcher_ItemCompare->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
			Switcher_ItemCompare->SetActiveWidgetIndex(WidgetIndex);
		}

		auto TargetTextBlock = ((WidgetIndex == 0) ? Text_Compare_Better_Value : Text_Compare_Worse_Value);
		if (TargetTextBlock)
		{
			TargetTextBlock->SetText(FText::AsNumber(ItemIconInfo.CompareValue));
			//TargetTextBlock->SetText(FText::Format(FText::FromString("{0:0.0}"), FText::AsNumber(ItemIconInfo.CompareValue)));
		}
	}
}

void UUC_Item_Icons::SetEnableIcon(bool bEnable)
{
	if (IsValid(Item_Selection_Lock_Container))
	{
		if (Item_Selection_Lock_Container->GetVisibility() != ESlateVisibility::Visible)
			Item_Selection_Lock_Container->SetVisibility(ESlateVisibility::Visible);

		UImage* disableImg = FindChildWidget<UImage>(Item_Selection_Lock_Container, TEXT("disable"));
		UImage* equipImg = FindChildWidget<UImage>(Item_Selection_Lock_Container, TEXT("equip_select"));
		UImage* checkImg = FindChildWidget<UImage>(Item_Selection_Lock_Container, TEXT("check"));
		if (IsValid(disableImg) && IsValid(equipImg) && IsValid(checkImg))
		{
			equipImg->SetVisibility(ESlateVisibility::Collapsed);
			checkImg->SetVisibility(ESlateVisibility::Collapsed);

			if (bEnable)
			{
				disableImg->SetVisibility(ESlateVisibility::Collapsed);
			}
			else
			{
				disableImg->SetVisibility(ESlateVisibility::Visible);
			}
		}
	}
}

void UUC_Item_Icons::SetToogleIcon(bool bToogle)
{
	if (IsValid(Item_Selection_Lock_Container))
	{
		UImage* disableImg = FindChildWidget<UImage>(Item_Selection_Lock_Container, TEXT("disable"));
		if (IsValid(disableImg))
		{
			if (disableImg->GetVisibility() != ESlateVisibility::Visible)
			{
				UImage* equipImg = FindChildWidget<UImage>(Item_Selection_Lock_Container, TEXT("equip_select"));
				UImage* checkImg = FindChildWidget<UImage>(Item_Selection_Lock_Container, TEXT("check"));
				if (IsValid(equipImg) && IsValid(checkImg))
				{
					if (bToogle)
					{
						equipImg->SetVisibility(ESlateVisibility::Visible);
						checkImg->SetVisibility(ESlateVisibility::Visible);
					}
					else
					{
						equipImg->SetVisibility(ESlateVisibility::Collapsed);
						checkImg->SetVisibility(ESlateVisibility::Collapsed);
					}
				}
			}
		}
	}
}

/*void OnSetWidgetSizeType(bool InLargeSize)
{
	auto PanelSlot = Cast<>(SacleBox_IconSizeType->Slot);
	if (InLargeSize)
	{

	}
	else
	{

	}
}*/