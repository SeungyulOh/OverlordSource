// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "UI/RBaseWidget.h"
#include "UP_ContentsReset.generated.h"


DECLARE_DYNAMIC_MULTICAST_DELEGATE(FDelegate_OnClick);

class ULocalizingTextBlock;
class UTextBlock;
class UUC_CurrencyDisplay;
//struct FContentResetTableInfo;

/**
*
*/
UCLASS()
class PROJECTR_API UUP_ContentsReset : public URBaseWidget
{
	GENERATED_BODY()
public:
	void NativeConstruct() override;
public:
	UFUNCTION(BlueprintCallable, Category = UP_ContentsReset)
	void SetContentsResetPopup(FText InCaption, FText InContents, FName InTableName, int32 nResetCount);

	UFUNCTION(BlueprintCallable, Category = UP_ContentsReset)
	void SetContentsRefreshPopup(FText InCaption, FText InContents, EREWARD_TYPE eType, int32 nQuantity);

	UFUNCTION(BlueprintCallable, Category = UP_ContentsReset)
	void RefreshUI();

	UFUNCTION()
	void OnClicked_ButtonOK();

	UFUNCTION()
	void OnClicked_ButtonCancel();

public:
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = UP_ContentsReset)
	ULocalizingTextBlock*					Text_Caption = nullptr;

	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = UP_ContentsReset)
	ULocalizingTextBlock*					Text_Contents = nullptr;

	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = UP_ContentsReset)
	UTextBlock*								Text_AddCount = nullptr;

	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = UP_ContentsReset)
	UTextBlock*								Text_RemainCount = nullptr;
	
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = UP_ContentsReset)
	UTextBlock*								Text_MaxCount = nullptr;

	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = UP_ContentsReset)
	UUC_CurrencyDisplay*					UC_CurrencyDisplay = nullptr;
	
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = UP_ContentsReset)
	ULocalizingTextBlock*					Text_Purpose = nullptr;

	UPROPERTY(BlueprintAssignable, BlueprintCallable, Category = UP_ContentsReset)
	FDelegate_OnClick						OnClickOK;

	UPROPERTY(BlueprintAssignable, BlueprintCallable, Category = UP_ContentsReset)
	FDelegate_OnClick						OnClickCancel;

	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = UP_ContentsReset)
	UButton*								Button_OK;

	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = UP_ContentsReset)
	UButton*								Button_Cancel;

	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = UP_ContentsReset)
	UOverlay*								Desc_Area;

private:
	UPROPERTY()
	FText Caption;
	UPROPERTY()
	FText Contents;
	UPROPERTY()
	int32 addCount;
	UPROPERTY()
	int32 remainCount;
	UPROPERTY()
	int32 resetCount;
	UPROPERTY()
	int32 maxCount;
	UPROPERTY()
	EREWARD_TYPE currencyType;
	UPROPERTY()
	int32 currencyQuantity;
	UPROPERTY()
	bool bExtraCharge;

	bool bShowDescriptionArea;
};
