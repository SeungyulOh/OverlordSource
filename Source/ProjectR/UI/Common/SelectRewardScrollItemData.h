#pragma once

#include "ItemBaseScrollItemData.h"
#include "SelectRewardScrollItemData.generated.h"
UCLASS()
class PROJECTR_API USelectRewardScrollItemData : public UItemBaseScrollItemData
{
	GENERATED_BODY()

public:
	void		SetData(const FITEM_ICON_INFO& InIconInfo, int32 InItemOrderIndex = 0);
	void		SetItemName(const FText& InItemName);
	
public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = SelectRewardScrollItemData)
	int32					ItemOrderIndex;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = SelectRewardScrollItemData)
	FText					ItemName;
};
