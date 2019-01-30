
#include "ProjectR.h"
#include "UC_Item_Icon_with_Name.h"
#include "SelectRewardScrollItemData.h"
#include "UI/Common/UC_Item_Icons.h"

void UUC_Item_Icon_with_Name::SetScrollItemData(int32 Index, URScrollItemData* item, URScrollView* ScrollView)
{
//	Super::SetScrollItemData(Index, item, ScrollView);
	if(IsValid(UC_Item_Icons_01))
		UC_Item_Icons_01->SetScrollItemData(Index, item, ScrollView);

	USelectRewardScrollItemData* InvenItemData = Cast< USelectRewardScrollItemData >(item);
	if (InvenItemData)
	{
		SetItemName(InvenItemData->ItemName);
	}
}

void UUC_Item_Icon_with_Name::SetItemName(const FText& InItemName)
{
	if (IsValid(Item_Text_01))
		Item_Text_01->SetText(InItemName);
}

void UUC_Item_Icon_with_Name::SetSelectedItem(bool bSelected)
{
	if(IsValid(UC_Item_Icons_01))
		UC_Item_Icons_01->SetSelectedItem(bSelected);
}
