#pragma once

#include "UI/Control/RScrollItem.h"

#include "UC_Item_Icon_with_Name.generated.h"

class UUC_Item_Icons;
UCLASS()
class PROJECTR_API UUC_Item_Icon_with_Name : public URScrollItem
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, Category = ScrollItem)
	virtual void		SetScrollItemData(int32 Index, URScrollItemData* item, URScrollView* ScrollView) override;

public:
	void SetItemName(const FText& InItemName);
	void SetSelectedItem(bool bSelected);
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item_Icon_with_Name|Ref")
	UTextBlock*			Item_Text_01 = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item_Icon_with_Name|Ref")
	UUC_Item_Icons*		UC_Item_Icons_01 = nullptr;
};
