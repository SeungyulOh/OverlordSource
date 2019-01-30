// Fill out your copyright notice in the Description page of Project Settings.

#include "ProjectR.h"
#include "UC_GuildRaidSRScrollItem.h"

#include "UI/Guild//GuildRaid/RGuildRaidSRScrollItemData.h"
#include "UI/Common/UC_Item_Icons.h"

#include "UtilFunctionIntegrated.h"




void UUC_GuildRaidSRScrollItem::NativeConstruct()
{
	Super::NativeConstruct();

}

void UUC_GuildRaidSRScrollItem::NativeDestruct()
{
	Super::NativeDestruct();
}

void UUC_GuildRaidSRScrollItem::SetScrollItemData(int32 Index, URScrollItemData* item, URScrollView* ScrollView)
{
	URGuildRaidSRScrollItemData* RewardItemData = Cast<URGuildRaidSRScrollItemData>(item);

	if( IsValid( RewardItemData) )
	{
		if( IsValid( Text_Level) )
			Text_Level->SetText( FText::AsNumber( RewardItemData->RewardIndex + 1 ));

		if( IsValid( ItemIcons) )
			ItemIcons->SetItemIconStructure(RewardItemData->ItemIconInfo);

		if (IsValid(Text_Desc))
		{
			FText rtText;

			auto CurrencyTableInfo = UTableManager::GetInstancePtr()->GetCurrencyPropertyRow(RewardItemData->ItemIconInfo.ToolTipID);
			if (CurrencyTableInfo)
			{
				rtText = UUtilFunctionLibrary::GetLocalizedString(EStringTableType::VE_Common, CurrencyTableInfo->CommonStringKey);				
			}

			if (rtText.IsEmpty())
			{
				auto ItemTableInfo = UTableManager::GetInstancePtr()->GetCharacterItemRow(RewardItemData->ItemIconInfo.ToolTipID);
				if (ItemTableInfo)
				{
					rtText = UUtilFunctionLibrary::GetLocalizedString(EStringTableType::VE_Item, ItemTableInfo->ItemName);
				}
			}

			if (rtText.IsEmpty())
			{
				auto PreviewItemTableInfo = UTableManager::GetInstancePtr()->GetRewardPreviewItemRow(RewardItemData->ItemIconInfo.ToolTipID);
				if (PreviewItemTableInfo)
				{
					rtText = UUtilFunctionLibrary::GetLocalizedString(EStringTableType::VE_Name, PreviewItemTableInfo->NameStringKey);
				}
			}
			
			if (rtText.IsEmpty())
			{
				rtText = UUtilFunctionLibrary::GetLocalizedString(EStringTableType::VE_Tooltip, RewardItemData->ItemIconInfo.ToolTipID);
			}

			if (!rtText.IsEmpty())
			{
				Text_Desc->SetText(rtText);
			}

		}

	}

}
