// Fill out your copyright notice in the Description page of Project Settings.

#include "ProjectR.h"
#include "UP_ItemEnchantPopup.h"


#include "GlobalIntegrated.h"




#include "GameInfo/RInventory.h"
#include "CustomActors/BaseSequenceActor.h"
#include "UtilFunctionIntegrated.h"
#include "UI/RWidgetManager.h"
#include "UI/Common/UC_Item_Icons.h"

#define TABLEKEY_SEQUENCE_ENCHANT	FName("UI_Enchant")


void UUP_ItemEnchantPopup::NativeConstruct()
{
	//Blueprint Constructor
	Super::NativeConstruct();

	if (Button_OK)
	{
		Button_OK->OnClicked.RemoveDynamic(this, &UUP_ItemEnchantPopup::OnClickOK);
		Button_OK->OnClicked.AddDynamic(this, &UUP_ItemEnchantPopup::OnClickOK);
	}
}

void UUP_ItemEnchantPopup::NativeDestruct()
{
	Super::NativeDestruct();
}


bool UUP_ItemEnchantPopup::Initialize()
{
	Super::Initialize();

	return true;
}

void UUP_ItemEnchantPopup::OnClickOK()
{
	IsUIEvent = true;
	OnClickClose();
}

void UUP_ItemEnchantPopup::OnClickClose()
{
	//this->SetVisibility(ESlateVisibility::Collapsed);

	URGameInstance* RGameInstance = RGAMEINSTANCE(this);
	
	{
		/*if (IsUIEvent == false)
		{
			RGameInstance->RWidgetManager->RemoveUIEvent(EUIEventEnum::VE_ItemEnchant);
		}*/

		
		//RGameInstance->RWidgetManager->HideUserWidget(EWidgetBluePrintEnum::WBP_Popup_ItemEnchant);
		/*if(IsValid(RGameInstance->EventManager))
			RGameInstance->EventManager->OnClickOK.Broadcast();*/

		RGameInstance->RWidgetManager->HideUserWidget(EWidgetBluePrintEnum::WBP_Popup_ItemEnchant);
		IsUIEvent = false;
	}
	SetCaptureSequence(false);
	if(inInvalidUpgrade.IsBound())
		inInvalidUpgrade.Broadcast();
	inInvalidUpgrade.RemoveAll(this);
	this->SetVisibility(ESlateVisibility::Collapsed);
}


void UUP_ItemEnchantPopup::RefreshPopup()
{
	if(!ItemUD.IsEmpty())
		UUIFunctionLibrary::GetItemIconInfoWithUD(CurItemIconInfo, ItemUD);
	
	FItemTableInfo* ItemTableInfo = UTableManager::GetInstancePtr()->GetCharacterItemRow(CurItemIconInfo.itemId);
	if (ItemTableInfo == nullptr)
		return;

	this->SetVisibility(ESlateVisibility::Visible);

	URGameInstance* RGameInstance = RGAMEINSTANCE(this);

	FText ItemNameText;
	ULocalStringTableManager::GetInstancePtr()->GetLocalizedText(EStringTableType::VE_Item, ItemTableInfo->ItemName, ItemNameText);
	if (Text_DisplayName)
		Text_DisplayName->SetText(ItemNameText);

	if (IsValid(Text_EnchantLvBefore))
	{
		FItemGradeTableInfo* ItemGradeInfo = UTableManager::GetInstancePtr()->GetItemGradeTableRow(UUIFunctionLibrary::GetItemGradeName(beforeItemGrade));
		Text_EnchantLvBefore->SetColorAndOpacity(FLinearColor(ItemGradeInfo->Color));
		Text_EnchantLvBefore->SetText(UUtilFunctionLibrary::GetLocalizedString(EStringTableType::VE_UI, ItemGradeInfo->UIStringKey));
	}

	if (IsValid(Text_EnchantLvAfter) && Text_Grade)
	{
		FItemGradeTableInfo* ItemGradeInfo = UTableManager::GetInstancePtr()->GetItemGradeTableRow(UUIFunctionLibrary::GetItemGradeName(ItemTableInfo->ItemGrade));
		FLinearColor color = FLinearColor(ItemGradeInfo->Color);
		FText text = UUtilFunctionLibrary::GetLocalizedString(EStringTableType::VE_UI, ItemGradeInfo->UIStringKey);
		Text_EnchantLvAfter->SetColorAndOpacity(color);
		Text_EnchantLvAfter->SetText(text);
		Text_Grade->SetColorAndOpacity(color);
		Text_Grade->SetText(text);
	}
	
	if (IsValid(UC_Item_Icons))
	{
		UC_Item_Icons->SetItemIconStructure(CurItemIconInfo, false);
		UC_Item_Icons->SetVisibleEquipInfo(true);
	}
	FITEM* afterItemData = RGAMEINSTANCE(this)->RInventory->GetItemDataWithUD(ItemUD);
	int32 afterCombatPower = FItemTableInfo::GetCombatPower(afterItemData);

	if (Text_CombatValue_Before)
		Text_CombatValue_Before->SetText(FText::AsNumber(beforeItemCombatPower));

	if (Text_CombatValue_Extra)
	{
		FString str;
		if (afterCombatPower - beforeItemCombatPower >= 0)
			 str = FString("(+") + FString::FromInt(afterCombatPower - beforeItemCombatPower) + FString(")");
		else
			str = FString("(") + FString::FromInt(afterCombatPower - beforeItemCombatPower) + FString(")");
		Text_CombatValue_Extra->SetVisibility(ESlateVisibility::Visible);
		Text_CombatValue_Extra->SetText(FText::FromString(str));
	}
}

void UUP_ItemEnchantPopup::SetCaptureSequence(bool InVisible)
{
	URGameInstance* RGameInstance = RGAMEINSTANCE(this);
	if (InVisible)
	{
		ABaseSequenceActor* Sequence = RGameInstance->SequenceManager->SpawnSequence(TABLEKEY_SEQUENCE_ENCHANT, true);
		if (IsValid(Sequence))
		{
			//if (false == Sequence->OnSequenceEndEvent.IsAlreadyBound(this, &AStageBase::OnSequenceEndProcess))
			//{
			//	Sequence->OnSequenceEndEvent.AddDynamic(this, &AStageBase::OnSequenceEndProcess);
			//}
		}

		if (Image_CaptureMat)
		{
			Image_CaptureMat->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
		}
	}
	else
	{
		RGameInstance->SequenceManager->ClearSequence();

		if (Image_CaptureMat)
		{
			Image_CaptureMat->SetVisibility(ESlateVisibility::Collapsed);
		}
	}
}
