// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "UI/RBaseWidget.h"
#include "UP_MailBox.generated.h"


class URScrollItemData;
class URScrollView;
class UUC_Notify;
class UUP_SelectRewardPopup;
class UCheckBox_Radio;
/**
 * BP: UP_MailBox
 */
UCLASS()
class PROJECTR_API UUP_MailBox : public URBaseWidget
{
	GENERATED_BODY()
	
public:
	void NativeConstruct() override;
	void NativeDestruct() override;
	
	UFUNCTION(BlueprintCallable, Category = UUP_MailBox)
	void Click_Btn_Close();
	
	UFUNCTION(BlueprintCallable, Category = UUP_MailBox)
	void AllReceive();

	UFUNCTION(BlueprintCallable, Category = UUP_MailBox)
	void Get_Gift_List_Rp(const TArray<FMAIL>& InMailList);

	UFUNCTION(BlueprintCallable, Category = UUP_MailBox)
	void Get_Gift_Open_Rp(const TArray<FREWARD>& InRewards);

	UFUNCTION(BlueprintCallable, Category = UUP_MailBox)
	void Get_Special_Gift_List_Rp(const TArray<FMAIL>& InMailList);

	UFUNCTION(BlueprintCallable, Category = UUP_MailBox)
	void Get_Special_Gift_Open_Rp(const TArray<FREWARD>& InRewards);

	UFUNCTION(BlueprintCallable, Category = UUP_MailBox)
	void Get_Gift_List_More_Rp();

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = UUP_MailBox)
	void OnEmptyMailText(bool bIsVisible);	

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = UUP_MailBox)
	void OnSetTextCount(int32 Num);

	UFUNCTION(BlueprintCallable, Category = UUP_MailBox)
	void OnClick_EventRewardOpen(const FString& InMailUD, const TArray<FREWARD>& InRewards);
	
	UFUNCTION(BlueprintCallable, Category = UUP_MailBox)
	void OnEventRewardRp(const FString& RewardId);
	
	UFUNCTION(BlueprintCallable, Category = UUP_MailBox)
	void OnClick_MailCategory(EMailBoxType InType);

	UFUNCTION()
	void OnClick_CheckBox(bool _bChecked);

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UP_MailBox|Ref")
	URScrollView*	Scroll_MailList = nullptr;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UP_MailBox|Ref")
	UUC_Notify*		MailNotify = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UP_MailBox|Ref")
	UCanvasPanel*	EmptyMail_CanvasPanel = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UP_MailBox|Ref")
	UButton*		Btn_GetAll = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UP_MailBox|Ref")
	UUP_SelectRewardPopup*		SelectRewardPopup = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UP_MailBox|Ref")
	UWidgetSwitcher*			WidgetSwitcher_Gift = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UP_MailBox|Ref")
	UWidgetSwitcher*			WidgetSwitcher_Event = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UP_MailBox|Ref")
	UCheckBox_Radio*			Checkbox_Gift = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UP_MailBox|Ref")
	UCheckBox_Radio*			Checkbox_Event = nullptr;

private:
	void Init();
	void UpdateList();

private:
	EMailBoxType CurrentMailBoxType = EMailBoxType::VE_Gift;

	const int32 MAXSCROLLITEM = 10;
	const int32 MAXSCROLLITEMDATA = 50;

	UPROPERTY()
	TArray<class UMailListScrollData*> CachedItemDataList;
};
