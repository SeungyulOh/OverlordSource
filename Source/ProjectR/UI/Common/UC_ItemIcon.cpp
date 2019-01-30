// Fill out your copyright notice in the Description page of Project Settings.

#include "ProjectR.h"
#include "UC_ItemIcon.h"

#include "GlobalIntegrated.h"
#include "GameInfo/RInventory.h"
#include "UtilFunctionIntegrated.h"
#include "UtilFunctionIntegrated.h"
#include "Table/ResourceTexture2DTableInfo.h"

#include "UI/HeroManagement/UC_HeroGrade.h"
#include "Table/ItemTableInfo.h"

void UUC_EquipmentIcon::NativeConstruct()
{
	Super::NativeConstruct();

	// save
	if (Text_EnchantLevel)
	{
		Color_EnchantLevelNormal = Text_EnchantLevel->ColorAndOpacity;
	}
	
}

void UUC_EquipmentIcon::NativeDestruct()
{
	Super::NativeDestruct();
}

void UUC_ItemIconCommon::RefreshIcon(FName IconImageKey)
{
	// icon

	UUIFunctionLibrary::SetImageBrushFromTable(IconImageKey, Image_Icon);
	//UTexture2D* IconImage = UUtilFunctionLibrary::GetTexture2DFromTable(IconImageKey); // nonnull
	//Image_Icon->SetBrushFromTexture(IconImage);
	if(Image_Icon)
		Image_Icon->SetVisibility(ESlateVisibility::HitTestInvisible);
}

void UUC_ItemIconCommon::RefreshGradeFrame(EItemGradeEnum Grade)
{
	FItemGradeTableInfo* ItemGradeInfo = UTableManager::GetInstancePtr()->GetItemGradeTableRow(UUIFunctionLibrary::GetItemGradeName(Grade));
	if (ItemGradeInfo && Image_GradeFrame)
	{
		UTexture2D* Brush = ItemGradeInfo->IconImg.LoadSynchronous();
		Image_GradeFrame->SetBrushFromTexture(Brush);
	}
}

void UUC_ItemIconCommon::RefreshEmpty()
{
	if (Image_Icon)
		Image_Icon->SetVisibility(ESlateVisibility::Collapsed);
	RefreshGradeFrame(EItemGradeEnum::Normal);
}

void UUC_EquipmentIcon::Refresh(const FItemTableInfo* ItemInfo, const FITEM* ItemData, bool bSelected, bool IsInLockMode)
{
	if (ItemInfo == nullptr || ItemData == nullptr)
		return; // error

	// icon
	if (ItemIconCommon)
	{
		ItemIconCommon->RefreshIcon(ItemInfo->ItemIcon);
		ItemIconCommon->RefreshGradeFrame(ItemInfo->ItemGrade);
	}
	//temp
	SetVisiblitySwap(true);

	FItemGradeTableInfo* ItemGradeInfo = UTableManager::GetInstancePtr()->GetItemGradeTableRow(UUIFunctionLibrary::GetItemGradeName(ItemInfo->ItemGrade));
	// grade text
	if (ItemGradeInfo && Text_Grade && Panel_Level)
	{
		Text_Grade->SetColorAndOpacity(FLinearColor(ItemGradeInfo->Color));
		Text_Grade->SetText(UUtilFunctionLibrary::GetLocalizedString(EStringTableType::VE_UI, ItemGradeInfo->UIStringKey));
		// item level
		// Only show item level for grade Tower
		//if (ItemInfo->ItemGrade == EItemGradeEnum::Tower)
		//{
			Text_ItemLevel->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
			Text_ItemLevel->SetText(FText::AsNumber(ItemData->level));
			Panel_Level->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
		//}
		/*else
		{
			Panel_Level->SetVisibility(ESlateVisibility::Collapsed);
		}*/
	}
	// enchant level
	// do not display for enchant level 0
	if (ItemData->enchant == 0)
	{
		Text_EnchantLevel->SetVisibility(ESlateVisibility::Collapsed);
	}
	else
	{
		if (Text_EnchantLevel)
		{
			Text_EnchantLevel->SetText(FText::Format(FText::FromString(TEXT("+{0}")), FText::AsNumber(ItemData->enchant)));
			Text_EnchantLevel->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
			Text_EnchantLevel->SetColorAndOpacity(ItemData->enchantLocked ? Color_EnchantLevelUnenchantable : Color_EnchantLevelNormal);
		}
	}

	// lock
	// if selected in lock/unlock mode, toggle the lock state
	if (bSelected && IsInLockMode && Image_Lock)
	{
		Image_Lock->SetVisibility(!ItemData->locked ? ESlateVisibility::SelfHitTestInvisible : ESlateVisibility::Collapsed);
	}
	else
	{
		Image_Lock->SetVisibility(ItemData->locked ? ESlateVisibility::SelfHitTestInvisible : ESlateVisibility::Collapsed);
	}

	if (Panel_Selected)
	{
		// Selected
		if (bSelected && !IsInLockMode)
			Panel_Selected->SetVisibility(ESlateVisibility::HitTestInvisible);
		else
			Panel_Selected->SetVisibility(ESlateVisibility::Collapsed);
	}
	// Holder
	if(Panel_Holder)
		Panel_Holder->SetVisibility(ItemData->equipedHeroUD.IsEmpty() ? ESlateVisibility::Collapsed : ESlateVisibility::SelfHitTestInvisible);
	
	//temp tier
	if (HBox_Tier)
	{
		HBox_Tier->SetVisibility(ESlateVisibility::Visible);

		if(ItemInfo->ItemLevel > 0 && ItemInfo->ItemLevel < 7)
			Text_TierNumeric->SetText(FText::AsNumber(ItemInfo->ItemLevel));
		else
			HBox_Tier->SetVisibility(ESlateVisibility::Collapsed);
	}
}

void UUC_EquipmentIcon::SetRuneUI(FName IconKey, int32 OwnedAmount, bool IsSelected, int32 Grade, int32 detailGrade)
{
	//for new Rune UI
	ItemIconCommon->RefreshIcon(IconKey);

	SetVisiblitySwap(false);

	FColor color;
	switch (Grade)
	{
	case 1:
		color = FColor(255, 255, 255);
		ItemIconCommon->RefreshGradeFrame(EItemGradeEnum::Normal);
		break;
	case 2:
		color = FColor(51, 162, 70);
		ItemIconCommon->RefreshGradeFrame(EItemGradeEnum::Rare);
		break;
	case 3:
		color = FColor(59, 154, 196);
		ItemIconCommon->RefreshGradeFrame(EItemGradeEnum::Epic);
		break;
	case 4:
		color = FColor(188, 164, 81);
		ItemIconCommon->RefreshGradeFrame(EItemGradeEnum::Unique);
		break;
	case 5:
		color = FColor(247, 131, 240);
		ItemIconCommon->RefreshGradeFrame(EItemGradeEnum::Tower);
		break;
	default:
		break;
	}
	Text_RuneGText->SetColorAndOpacity(FLinearColor::FromSRGBColor(color));
	Text_RuneGNumeric->SetColorAndOpacity(FLinearColor::FromSRGBColor(color));
	Text_RuneGNumeric->SetText(FText::AsNumber(detailGrade));
	Text_ItemLevel->SetText(FText::AsNumber(OwnedAmount));
	
	if (IsSelected)
		Panel_Selected->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
	else
		Panel_Selected->SetVisibility(ESlateVisibility::Collapsed);

	if (Panel_Holder)
		Panel_Holder->SetVisibility(ESlateVisibility::Collapsed);
}

void UUC_EquipmentIcon::SetVisiblitySwap(bool IsItem)
{
	if (IsItem)
	{
		Panel_Grade->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
		HBox_Tier->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
		Panel_Level->SetVisibility(ESlateVisibility::Collapsed);
		TextBlock_LVText->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
		HBox_RuneGrade->SetVisibility(ESlateVisibility::Collapsed);
	}
	else
	{
		Panel_Grade->SetVisibility(ESlateVisibility::Collapsed);
		HBox_Tier->SetVisibility(ESlateVisibility::Collapsed);
		Panel_Level->SetVisibility(ESlateVisibility::Visible);
		TextBlock_LVText->SetVisibility(ESlateVisibility::Collapsed);
		Text_ItemLevel->SetVisibility(ESlateVisibility::Visible);
		HBox_RuneGrade->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
		Text_EnchantLevel->SetVisibility(ESlateVisibility::Collapsed);
		Image_Lock->SetVisibility(ESlateVisibility::Collapsed);
	}
}

void UUC_EquipmentIcon::SetSelected(bool bSelected)
{
	if (!IsValid(Panel_Selected))
		return;

	if (bSelected)
		Panel_Selected->SetVisibility(ESlateVisibility::Visible);
	else
		Panel_Selected->SetVisibility(ESlateVisibility::Collapsed);
}

void UUC_HeroPieceIcon::Refresh(const FWalletTableInfo* WalletInfo, int32 OwnedAmount)
{
	if (WalletInfo == nullptr)
		return;
	if (ItemIconCommon && HeroGrade && Text_Amount && ProgressBar_Amount)
	{
		ItemIconCommon->RefreshIcon(WalletInfo->ItemIconKey);

		HeroGrade->Refresh(WalletInfo->Grade);

		// TODO
		int32 ClaimAmount = 100;

		FString AmountText = FString::Printf(TEXT("%d / %d"), OwnedAmount, ClaimAmount);
		Text_Amount->SetText(FText::FromString(AmountText));

		float ProgressBarFill = OwnedAmount >= ClaimAmount ? 1.0f : (float)OwnedAmount / ClaimAmount;
		ProgressBar_Amount->SetPercent(ProgressBarFill);
	}
}

void UUC_MaterialIcon::Refresh(FName IconKey, int32 OwnedAmount, bool IsSelected, int32 SelectedAmount)
{
	ItemIconCommon->RefreshIcon(IconKey);
	if (Panel_Selected && Text_Amount && Panel_SelectedAmount && Text_SelectedAmount && Text_OwnedAmount)
	{
		if (IsSelected)
		{
			Panel_Selected->SetVisibility(ESlateVisibility::SelfHitTestInvisible);

			if (SelectedAmount == 0)
			{
				Text_Amount->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
				Text_Amount->SetText(FText::AsNumber(OwnedAmount));

				Panel_SelectedAmount->SetVisibility(ESlateVisibility::Collapsed);
			}
			else
			{
				Text_Amount->SetVisibility(ESlateVisibility::Collapsed);

				Panel_SelectedAmount->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
				Text_SelectedAmount->SetText(FText::AsNumber(SelectedAmount));
				Text_OwnedAmount->SetText(FText::AsNumber(OwnedAmount));
			}
		}
		else
		{
			Panel_Selected->SetVisibility(ESlateVisibility::Collapsed);

			Text_Amount->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
			Text_Amount->SetText(FText::AsNumber(OwnedAmount));
		}
	}
}

void UUC_MaterialIcon::SetSelected(bool bSelected)
{
	if (bSelected)
		Panel_Selected->SetVisibility(ESlateVisibility::Visible);
	else
		Panel_Selected->SetVisibility(ESlateVisibility::Collapsed);
}

void UUC_GenericItemIcon::RefreshEquipmentIcon(const FItemTableInfo* ItemInfo, const FITEM* ItemData, bool bSelected, bool IsInLockMode)
{
	if (Switcher_Icon && EquipmentIcon)
	{
		Switcher_Icon->SetActiveWidgetIndex(0);
		EquipmentIcon->Refresh(ItemInfo, ItemData, bSelected, IsInLockMode);
	}
}

void UUC_GenericItemIcon::RefreshEquipmentIcon(FString ItemUD)
{
//	check(IsValid(RGAMEINSTANCE(this)->RInventory));

	FITEM* ItemData = RGAMEINSTANCE(this)->RInventory->GetItemDataWithUD(ItemUD);
	FItemTableInfo* ItemInfo = UTableManager::GetInstancePtr()->GetCharacterItemRow(ItemData->itemId);
	if (!ensure(ItemInfo != nullptr))
		return;

	RefreshEquipmentIcon(ItemInfo, ItemData);
}

void UUC_GenericItemIcon::SetSelectedPanel(bool bSelected, bool isEquipItem)
{
	if (!IsValid(EquipmentIcon))
		return;
	if (!IsValid(MaterialIcon))
		return;

	if (isEquipItem)
		EquipmentIcon->SetSelected(bSelected);
	else
		MaterialIcon->SetSelected(bSelected);
}

void UUC_GenericItemIcon::RefreshWalletIcon(FName WalletKey, const FWalletTableInfo* WalletInfo, int32 OwnedAmount, bool IsSelected, int32 SelectedAmount)
{
//	check(WalletInfo != nullptr);
	if (WalletInfo == nullptr)
		return;
	if (Switcher_Icon && HeroPieceIcon && MaterialIcon)
	{
		switch (WalletInfo->PropertyType)
		{
			case EWalletPropertyType::HeroPiece:
			{
				Switcher_Icon->SetActiveWidgetIndex(1);
				HeroPieceIcon->Refresh(WalletInfo, OwnedAmount); // TODO: SelectedAmount
			}
			break;
			case EWalletPropertyType::Rune:
			{
				//temp
				Switcher_Icon->SetActiveWidgetIndex(0);
	
				int32			idx = RGAMEINSTANCE(this)->RInventory->RuneWallet.Find(WalletKey);
				if (!ensure(idx != INDEX_NONE))
					return;
	
				auto TM = UTableManager::GetInstancePtr();
				FRuneTable*		RuneInfo = TM->GetRuneOptionRowInt(idx + 1);
				FWalletTableInfo* WTInfo = TM->GetWalletRow(WalletKey);
				int32 grade = 0;
				if (WTInfo)
					grade = WTInfo->Grade;
				if (!ensure(RuneInfo != nullptr))
					return;
				//MaterialIcon->Refresh(RuneInfo->RuneInventoryImage, OwnedAmount, IsSelected, SelectedAmount);
				EquipmentIcon->SetRuneUI(RuneInfo->RuneInventoryImage, OwnedAmount, IsSelected, grade, RuneInfo->ID);
			}
			break;
			default:
			{
				Switcher_Icon->SetActiveWidgetIndex(2);
				MaterialIcon->Refresh(WalletInfo->ItemIconKey, OwnedAmount, IsSelected, SelectedAmount);
			}
			break;
		}
	}
}

void UUC_GenericItemIcon::RefreshHeroIcon(FString ItemUD)
{
	Switcher_Icon->SetActiveWidgetIndex(3);
	if (HeroIcon)
		HeroIcon->SetHeroIconInfoWithUD(ItemUD, EHeroOwnershipType::VE_Me);
}
