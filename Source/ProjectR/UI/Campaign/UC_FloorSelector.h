// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/RBaseWidget.h"
#include "UI/Control/RScrollView.h"
#include "UI/Control/RScrollItem.h"
#include "UI/Control/RScrollItemData.h"
#include "UC_FloorSelector.generated.h"


UCLASS()
class PROJECTR_API URFloorScrollItemData : public URScrollItemData
{
	GENERATED_BODY()

public:
	int32 Floor = -1;
};

UCLASS()
class PROJECTR_API URFloorScrollItem : public URScrollItem
{
	GENERATED_BODY()

public:
	virtual void Selected() override;

public:
	UPROPERTY(EditAnywhere , BlueprintReadWrite , Category = "ScrollItem")
	UWidgetSwitcher* WidgetSwitcher_Select;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ScrollItem")
	UTextBlock* FloorText1;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ScrollItem")
	UTextBlock* FloorText2;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ScrollItem")
	UImage*	SelectedImage;
};
/**
*
*/
UCLASS()
class PROJECTR_API URFloorScrollView : public URScrollView
{
	GENERATED_BODY()

public:
	void Custom_Init();

	virtual void			InvalidateData() override;
	virtual void			ScrollItemList_ChangeItemDataList(int32 dataIndex) override;
	void					MoveScrollPos(URScrollItemData* SelectItemData);
//	void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;
//	void SetScrollUpdate(URScrollItem* CurItem, FVector2D diff);
//
//	FORCEINLINE void ResetCurScrollOffSet() { CurScrollOffset = 0; }
//
//private:
//	float InitialVelocity = 0.f;
//	int32 TargetOffSet = 0.f;


};



USTRUCT(BlueprintType)
struct PROJECTR_API FVariables_FloorSelector
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Variables")
	UWidgetSwitcher* WidgetSwitcher_State;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Variables")
	UTextBlock* FloorTitleText1;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Variables")
	UTextBlock* FloorTitleText2;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Variables")
	UButton* Button_Menu;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Variables")
	URFloorScrollView* ScrollView;
};


/**
 * 
 */
UCLASS()
class PROJECTR_API UUC_FloorSelector : public URBaseWidget
{
	GENERATED_BODY()
	
public:
	virtual void NativeConstruct() override;

public:
	UFUNCTION()
	void OnClick_Menu();
	
	

	void ShowFloorSelector(bool bShow);
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "FloorSelector")
	FVariables_FloorSelector variables;

};
