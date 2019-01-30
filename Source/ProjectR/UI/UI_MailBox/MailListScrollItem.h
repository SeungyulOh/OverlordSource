// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "UI/Control/RScrollItem.h"
#include "MailListScrollItem.generated.h"

class URScrollItemData;
class ULocalizingTextBlock;
class UButton;
class UUC_Item_Icons;

/**
 * BP: UC_Mail
 */
UCLASS()
class PROJECTR_API UMailListScrollItem : public URScrollItem
{
	GENERATED_BODY()
	
public:
	void NativeConstruct() override;

	UFUNCTION(BlueprintCallable, Category = UMailListScrollItem)
	void SetMailScrollItem();

	UFUNCTION(BlueprintCallable, Category = UMailListScrollItem)
	void ClickReciveButton();

	bool IsValidData();
	void Clear();

	UFUNCTION()
	virtual void SetItemData(int32 Index, URScrollItemData* item, URScrollView* ScrollView) override;

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = MailListScrollItem)
	bool					bEventMail = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = MailListScrollItem)
	FString					MailUD;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = MailListScrollItem)
	int32					kId = 0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = MailListScrollItem)
	TArray<UUC_Item_Icons*> GiftIcons;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = MailListScrollItem)
	UImage*					Image_Title = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = MailListScrollItem)
	UTextBlock*				Text_Title = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = MailListScrollItem)
	UTextBlock*				Text_NickName = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = MailListScrollItem)
	UTextBlock*				Text_ReceiveDay = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = MailListScrollItem)
	UTextBlock*				Text_RemainDay = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = MailListScrollItem)
	UButton*				Button_Click = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = MailListScrollItem)
	UUC_Item_Icons*			ItemIcon1;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = MailListScrollItem)
	UUC_Item_Icons*			ItemIcon2;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = MailListScrollItem)
	UUC_Item_Icons*			ItemIcon3;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = MailListScrollItem)
	UUC_Item_Icons*			ItemIcon4;
};