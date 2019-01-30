// Fill out your copyright notice in the Description page of Project Settings.

#include "ProjectR.h"
#include "UC_EquipItemButton.h"


#include "GlobalIntegrated.h"

#include "Table/ItemTableInfo.h"
#include "Table/ItemGradeTableInfo.h"

#include "Character/Components/FunctionalComponents/BattleEntityComponent.h"
#include "UtilFunctionIntegrated.h"

#include "Network/PacketDataStructures.h"


#if WITH_EDITOR
#include "Runtime/Engine/Classes/Kismet/KismetSystemLibrary.h"
#endif

void UUC_EquipItemButton::NativeConstruct()
{
	Super::NativeConstruct();

	InvalidateData();
}

void UUC_EquipItemButton::InvalidateData()
{
	if (IsValid(ItemImage) && IsValid(ItemText) && IsValid(ItemGradeBorder))
	{
		ItemImage->SetVisibility(ESlateVisibility::Collapsed);
		ItemGradeBorder->SetVisibility(ESlateVisibility::Collapsed);
		ItemText->SetText(FText::FromString(TEXT("None")));

		FItemTableInfo* ItemTableInfo = UTableManager::GetInstancePtr()->GetCharacterItemRow(CurSelectItemKey);
		if (ItemTableInfo)
		{
			ItemImage->SetVisibility(ESlateVisibility::Visible);
			ItemGradeBorder->SetVisibility(ESlateVisibility::Visible);
			ItemText->SetText(UUtilFunctionLibrary::GetLocalizedString(EStringTableType::VE_Item, ItemTableInfo->ItemName));
			ItemImage->SetBrushFromTexture(UUtilFunctionLibrary::GetTexture2DFromTable(ItemTableInfo->ItemIcon));

			URGameInstance* RGameInstance = RGAMEINSTANCE(this);
			if ( IsValid(RGameInstance->GameDataManager))
			{
				FItemGradeTableInfo* ItemGradeInfo = UTableManager::GetInstancePtr()->GetItemGradeTableRow(UUIFunctionLibrary::GetItemGradeName(ItemTableInfo->ItemGrade));
				
				ItemGradeBorder->SetBrushColor(FLinearColor(ItemGradeInfo->Color));
			}			
		}
	}
}

void UUC_EquipItemButton::ButtonClicked()
{
	OnButtonClicked.Broadcast();
}

void UUC_EquipItemButton::SetCurItem(FName inItemKey)
{
	CurSelectItemKey = inItemKey;

	InvalidateData();
}

void UUC_EquipItemButton::EquipItem(AActor* Obj, FITEM& InItem)
{
	UBattleEntityComponent* BattleEntityComponent = UCharacterHelpFunctionLibrary::GetBattleEntityComponent(Obj);
	if (IsValid(BattleEntityComponent))
	{
		FItemTableInfo* ItemTableInfo = UTableManager::GetInstancePtr()->GetCharacterItemRow(InItem.itemId);
		if (ItemTableInfo)
		{
			BattleEntityComponent->EquipItemFITEMTest(ItemTableInfo->EquipPosition, InItem);
		}
	}
}

bool UUC_EquipItemButton::GetFItem(FString InItemStr, FITEM& OutItem)
{
	const TCHAR* Delims[] = { TEXT(":") };
	FString strLower = InItemStr.ToLower();
	TArray<FString> ParsedSub;
	strLower.ParseIntoArray(ParsedSub, Delims, 1);

// 	if (ParsedSub.Num() > 0)
// 	{
// 		FName itemKey = FName(*ParsedSub[0]);
// 		FItemTableInfo* ItemTableInfo = UTableManager::GetInstancePtr()->GetCharacterItemRow(itemKey);
// 		if (ItemTableInfo)
// 		{
// 			OutItem.enchantValue = 0;
// 			OutItem.tuneCnt = 0;
// 			OutItem.itemUD = TEXT("");
// 			OutItem.equipedHeroUD = TEXT("");
// 			OutItem.enchant = 0;
// 			OutItem.stack = 0;
// 
// 			OutItem.itemId = itemKey;
// 
// 			OutItem.jewels.Empty();
// 
// 			for (int32 i = 1; i < ParsedSub.Num(); ++i)
// 			{
// 				if (ParsedSub.IsValidIndex(i))
// 				{
// 					OutItem.jewels.Emplace(ParsedSub[i]);
// 				}
// 			}
// 
// 			OutItem.options.Empty();
// 
// 			/*FItemGenerateOptionTableInfo* GenerateOptionTableInfo = UTableManager::GetInstancePtr()->GetItemGenerateOptionRow(ItemTableInfo->ItemGenerateOptionKey);
// 			if (GenerateOptionTableInfo)
// 			{
// 				int32 iValue = FMath::RandRange(GenerateOptionTableInfo->ItemGenerateBonusOptionValueMin, GenerateOptionTableInfo->ItemGenerateBonusOptionValueMax);
// 
// 				//qsmname
// 				//OutItem.options.Emplace(iValue);
// 			}*/
// 
// 			return true;
// 		}		
// 	}

	return false;
}

bool UUC_EquipItemButton::GetFItem(FTestItemInfo InItemInfo, FITEM& OutItem)
{
// 	FItemTableInfo* ItemTableInfo = UTableManager::GetInstancePtr()->GetCharacterItemRow(InItemInfo.ItemKey);
// 	if (ItemTableInfo)
// 	{
// 		OutItem.enchantValue = 0;
// 		OutItem.tuneCnt = 0;
// 		OutItem.itemUD = TEXT("");
// 		OutItem.equipedHeroUD = TEXT("");
// 		OutItem.enchant = 0;
// 		OutItem.stack = 0;
// 
// 		OutItem.itemId = InItemInfo.ItemKey;
// 
// 		OutItem.jewels.Empty();
// 
// 		for (int32 i = 0; i < InItemInfo.GemList.Num(); ++i)
// 		{
// 			if (InItemInfo.GemList.IsValidIndex(i))
// 			{
// 				OutItem.jewels.Emplace(InItemInfo.GemList[i]);
// 			}
// 		}
// 
// 		OutItem.options.Empty();
// 		/*
// 		FItemGenerateOptionTableInfo* GenerateOptionTableInfo = UTableManager::GetInstancePtr()->GetItemGenerateOptionRow(ItemTableInfo->ItemGenerateOptionKey);
// 		if (GenerateOptionTableInfo)
// 		{
// 			int32 iValue = FMath::RandRange(GenerateOptionTableInfo->ItemGenerateBonusOptionValueMin, GenerateOptionTableInfo->ItemGenerateBonusOptionValueMax);
// 
// 			//qsmname
// 			//OutItem.options.Emplace(iValue);
// 		}
// 		*/
// 		return true;
// 	}

	return false;
}