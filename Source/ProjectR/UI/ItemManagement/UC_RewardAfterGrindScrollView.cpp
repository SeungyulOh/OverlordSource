// Fill out your copyright notice in the Description page of Project Settings.

#include "ProjectR.h"
#include "UI/ItemManagement/UC_Currency_Icon.h"
#include "Network/PacketDataStructures.h"
#include "UtilFunctionIntegrated.h"
#include "UI/ItemManagement/UC_CurrencyScrollItem.h"
#include "UI/ItemManagement/UC_CurrencyScrollItemData.h"

#include "UC_RewardAfterGrindScrollView.h"




void UUC_RewardAfterGrindScrollView::NativeConstruct()
{
	Super::NativeConstruct();

	InitScrollView();
}

void UUC_RewardAfterGrindScrollView::InitScrollView()
{
	if (IsValid(BGPanel))
	{
		BGPanel->SetRenderTransformPivot(FVector2D::ZeroVector);
		BGPanel->SetRenderScale(InitScale);
	}

	CurScrollOffset = 0.0f;
	StartIdx = 0;
	
	ScrollItemList_RemoveAllElement(GrindRewards.Num());
	ScrollItemList_AddElement(GrindRewards.Num());
}

void UUC_RewardAfterGrindScrollView::SetReward(TArray<FREWARD>& Rewards)
{
	GrindRewards = Rewards;
}

void UUC_RewardAfterGrindScrollView::SetItemDataintoScrollItem()
{
	ClearScrollItemData(GrindRewards.Num());

	//fill
	for (auto& Element : GrindRewards)
	{
		UUC_CurrencyScrollItemData* ScrollItemData = Cast<UUC_CurrencyScrollItemData>(AddScrollItemData(UUC_CurrencyScrollItemData::StaticClass()));
		if (!IsValid(ScrollItemData))
			continue;
		
		
		if (Element.type == EREWARD_TYPE::WALLET)
		{
			FWalletTableInfo* WalletTableInfo = UTableManager::GetInstancePtr()->GetWalletRow(Element.rewardId);
			if (WalletTableInfo)
			{
				ScrollItemData->Iconkey = WalletTableInfo->ItemIconKey;
				ScrollItemData->CommonStringKey = WalletTableInfo->CommonStringKey;
			}
			
		}
		else if(Element.type == EREWARD_TYPE::AVATAR_EXP)
		{
			FCurrencyPropertyTableInfo* CurrencyTable = UTableManager::GetInstancePtr()->GetCurrencyPropertyRow(FName("AVATAR_EXP"));
			if (CurrencyTable)
			{
				ScrollItemData->Iconkey = CurrencyTable->ItemIconKey;
				ScrollItemData->CommonStringKey = CurrencyTable->CommonStringKey;
			}
		}
		else if (Element.type == EREWARD_TYPE::ITEM)
		{
			FItemTableInfo* ItemTableInfo = UTableManager::GetInstancePtr()->GetCharacterItemRow(Element.rewardId);
			if (ItemTableInfo)
			{
				ScrollItemData->Iconkey = ItemTableInfo->ItemIcon;
				ScrollItemData->CommonStringKey = ItemTableInfo->ItemName;
			}

		}
					

		ScrollItemData->amount = Element.amt;
		
		//UObject* Image = UUIFunctionLibrary::GetIconImageFromTable(WalletTableInfo->ItemIconKey);
		
	}
	//

	InvalidateData();
}
