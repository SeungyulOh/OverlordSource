// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "UI/RBaseWidget.h"
#include "UI/UIDefs.h"
#include "BaseSubPage.generated.h"

DECLARE_MULTICAST_DELEGATE(FOnUpdatePage);
typedef FOnUpdatePage::FDelegate FOnUpdatePageDelegate;

#define DEFINE_PAGE_DELEGATE_BASE(DelegateName) \
public: \
	F##DelegateName DelegateName##Delegates; \
public: \
	virtual FDelegateHandle Add##DelegateName##Delegate_Handle(const F##DelegateName##Delegate& Delegate) \
	{ \
		DelegateName##Delegates.Add(Delegate); \
		return Delegate.GetHandle(); \
	} \
	virtual void Clear##DelegateName##Delegate_Handle(FDelegateHandle& Handle) \
	{ \
		DelegateName##Delegates.Remove(Handle); \
		Handle.Reset(); \
	}

#define DEFINE_MAINPAGE_DELEGATE(DelegateName) \
	DEFINE_PAGE_DELEGATE_BASE(DelegateName) \
	virtual void Trigger##DelegateName##Delegates() \
	{ \
		DelegateName##Delegates.Broadcast(); \
	}


/**
 * 
 */
UCLASS()
class PROJECTR_API UBaseSubPage : public URBaseWidget
{
	GENERATED_BODY()
	
public:
	void NativeConstruct() override;
	void NativeDestruct() override;

	virtual void Refresh();

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "BaseSubPages|Ref")
	UButton*				Button_Click = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "BaseSubPages|Ref")
	UButton*				Button_Close = nullptr;

protected:
	UFUNCTION()
	virtual void OnButton_Click();

	UFUNCTION()
	virtual void OnButton_Close();
	
	virtual void CallNextPage();
	
	//FOnUpdatePage 
	/**
	 * Called when MainPage Request
	 *
	 */
	DEFINE_MAINPAGE_DELEGATE(OnUpdatePage);
	FDelegateHandle DelegateHandle;

private:
	//virtual void Internal_Init();

};
