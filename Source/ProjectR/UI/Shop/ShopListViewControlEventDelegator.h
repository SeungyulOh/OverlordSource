// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Object.h"
#include "UI/Control/ControlEventDelegator.h"
#include "ShopListViewControlEventDelegator.generated.h"

class UShopScrollItem;
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FShowPurchasePopupDelegate, UShopScrollData*, SelectedScrollData );

/**
 * 
 */
UCLASS()
class PROJECTR_API UShopListViewControlEventDelegator : public UControlEventDelegator
{
	GENERATED_BODY()
	
	
public:
	
	UPROPERTY(BlueprintAssignable, BlueprintCallable, Category = ShopListViewControlEventDelegator)
	FShowPurchasePopupDelegate			PurchasePopupRequest;

};
