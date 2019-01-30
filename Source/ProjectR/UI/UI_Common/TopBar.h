// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "UI/RBaseWidget.h"
#include "UI/UIDefs.h"
#include "TopBar.generated.h"

class UEnum;
class UTextBlock;
class UWidgetSwitcher;

class UUC_TopBar_Property;
class UUC_TopBar_Info;
struct FTopbarTableInfo;

/**
 * 
 */
UCLASS()
class PROJECTR_API UTopBar : public URBaseWidget
{
	GENERATED_BODY()
	
public:

	void NativeConstruct() override;
	void NativeDestruct() override;

	UFUNCTION(BlueprintCallable, Category = TopBar)
	void PrevButton();

	//UFUNCTION(BlueprintCallable, Category = TopBar)
	//void NotifyListButton();

	UFUNCTION(BlueprintCallable, Category = TopBar)
	void OnClick_Search();

	UFUNCTION(BlueprintCallable, Category = TopBar)
	void OnClick_Mail();

	UFUNCTION(BlueprintCallable, Category = TopBar)
	void OnClick_Setting();

	UFUNCTION(BlueprintCallable, Category = TopBar)
	void OnClick_Chat();

	UFUNCTION(BlueprintCallable, Category = TopBar)
	void OnClick_Avatar();

	UFUNCTION(BlueprintCallable, Category = TopBar)
	void OnClick_OpenTotalMenu();


	UFUNCTION()
	void RefreshDataUI();

	UFUNCTION()
	void RefreshData();

	void		RefreshProperty();
	void		RefreshProperty_InShop(int32 InShopTabIndex);
	FString		GetShopTabString(int32 InShopTabIndex);

public:
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = TopBar)
	UWidgetSwitcher* WidgetSiwtcher_Account = nullptr;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = TopBar)
	ULocalizingTextBlock*		Text_Content = nullptr;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = TopBar)
	TArray<UUC_TopBar_Property*> TopBar_Propertys;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = TopBar)
	UUC_TopBar_Info*	UC_TopBar_Info;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = TopBar)
	UTextBlock*		Text_AccountName = nullptr;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = TopBar)
	ULocalizingTextBlock* Text_AccountEXP = nullptr;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = TopBar)
	UTextBlock*		Text_Guild = nullptr;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = TopBar)
	UTextBlock*		Text_AvatarLevel = nullptr;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = TopBar)
	UProgressBar*	ProgressBar_AvatarEXP;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = TopBar)
	UImage*			Image_BackBoard = nullptr;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = TopBar)
	UImage*			top_bg = nullptr;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = TopBar)
	UWidgetSwitcher*	WidgetSwitcher_SearchBtn = nullptr;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = TopBar)
	UButton*		Button_Search = nullptr;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = TopBar)
	UButton*		Button_Mail = nullptr;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = TopBar)
	UButton*		Button_Alarm = nullptr;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = TopBar)
	UButton*		Button_Chat = nullptr;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = TopBar)
	UWidgetSwitcher*	WidgetSwitcher_MenuListBtn = nullptr;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = TopBar)
	UButton*		Button_OpenTotalMenu = nullptr;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = TopBar)
	UButton*		Button_CloseTotalMenu = nullptr;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = TopBar)
	UButton*		Button_Avatar = nullptr;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = TopBar)
	UButton*		Btn_Top_Prev = nullptr;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = TopBar)
	class UUC_Colosseum_Class* UC_Colosseum_Tier = nullptr;

	DECLARE_DELEGATE(FOnClickPrevButton)
	static FOnClickPrevButton OnClickPrevButton;

	
	bool			TempSkipPrevString = false;
	bool			bOpenMenuFlag;
	bool			bOpenTopBarInfo;

private:
	

	UPROPERTY()
	UEnum* UIPageEnumPtr;

	EUIPageEnum		CurUIPageEnum = EUIPageEnum::UIPage_None;

private:
	void RefreshState();
	void RefreshPropertyInfo();
	void RefreshAvatarInfo();
	void SetStateSwitcher();
	void SetCurrentUIString(EUIPageEnum InCurrentUIPage);
	void SetSetting(FTopbarTableInfo* InTableInfo);
	
	void CheckNotify();
	void CheckNotify_AlarmNotify();

public:
	void RefreshCurrentUIString();
	
};