// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "UI/RBaseWidget.h"
#include "UI/UIDefs.h"
#include "UI/RuneItem/UC_RuneIcon.h"
#include "UI/Control/RScrollItem.h"

#include "Global/RGameInstance.h"

#include "UC_RuneScrollItem.generated.h"


/**
 * 
 */

UCLASS()
class PROJECTR_API URuneScrollItemData : public URScrollItemData
{
	GENERATED_BODY()

public:

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = URuneScrollItemData)
	int32				RuneID;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = URuneScrollItemData)
	int32				RuneCount;
};

UCLASS()
class PROJECTR_API UUC_RuneScrollItem : public URScrollItem
{
	GENERATED_BODY()
public:
	void				NativeConstruct()	override;

	UFUNCTION(BlueprintCallable, Category = UUC_RuneScrollItem)
	void SelectIconRune();

	UFUNCTION(BlueprintCallable, Category = UUC_RuneScrollItem)
	void SetItemDataSource(int32 InIndex, URScrollItemData* InItem, URScrollView* InScrollView);

	UFUNCTION()
	virtual void SetSelected(bool bSelected) override;

	UFUNCTION()
	virtual void SetItemData(int32 Index, URScrollItemData* item, URScrollView* ScrollView) override;

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UUC_RuneScrollItem)
	UUC_RuneIcon* RuneIcon = nullptr;
};
