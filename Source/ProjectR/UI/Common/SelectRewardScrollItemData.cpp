
#include "ProjectR.h"
#include "SelectRewardScrollItemData.h"


void USelectRewardScrollItemData::SetData(const FITEM_ICON_INFO& InItemIconInfo, int32 InItemOrderIndex)
{
	ItemIconInfo = InItemIconInfo;
	ItemOrderIndex = InItemOrderIndex;
}

void USelectRewardScrollItemData::SetItemName(const FText& InItemName)
{
	ItemName = InItemName;
}