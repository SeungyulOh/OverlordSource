// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "UI/RBaseWidget.h"
#include "UI/UIDefs.h"
#include "UI/RuneItem/UC_RuneItemSlot.h"

#include "UI/Control/RScrollView.h"
#include "UI/Control/RScrollItem.h"
#include "UI/Control/RScrollItemData.h"

#include "Global/RGameInstance.h"

#include "UC_RuneList_ScrollView.generated.h"


/**
 * 
 */
UCLASS()
class PROJECTR_API UUC_RuneList_ScrollView : public URScrollView
{
	GENERATED_BODY()
public:
	void				NativeConstruct()	override;
	void				NativeDestruct()	override;

	//void Init() override;

	void InventoryDataReComposition(const TArray<int32>& InRuneUDList);

	virtual void SetSelected(URScrollItem* SelecteItem) override;

	/*
	==================================================================================================================
	Hero Exp
	==================================================================================================================
	*/
private:
	void ReComposition(const TArray<int32>& InRuneIDList, const TArray<int32>& InRuneCountList);
};
