// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "UI/RBaseWidget.h"
#include "BaseSubPage.h"
#include "UI/UIDefs.h"
#include "UP_SelectPlatform.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTR_API UUP_SelectPlatform : public UBaseSubPage
{
	GENERATED_BODY()
	
public:
	void NativeConstruct() override;
	void NativeDestruct() override;


	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "UP_SelectPlatform")
	void OnShowExternalEvent();

	UFUNCTION(BlueprintCallable, Category = "UP_SelectPlatform")
	void TestSuccess();

	UFUNCTION(BlueprintCallable, Category = "UP_SelectPlatform")
	void TestFail();

	UFUNCTION()
	void OnButton_Google();

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UP_SelectPlatform|Ref")
	UWidgetSwitcher*		Switcher_Platform = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UP_SelectPlatform|Ref")
	UHorizontalBox*			HBox_Login = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UP_SelectPlatform|Ref")
	UButton*				Button_Facebook = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UP_SelectPlatform|Ref")
	UButton*				Button_IOS = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UP_SelectPlatform|Ref")
	UButton*				Button_Google = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UP_SelectPlatform|Ref")
	UButton*				Button_Guest = nullptr;

protected:
	void Refresh() override;

private:

	UFUNCTION()
	void OnRequestReceive();

	UFUNCTION()
	void OnButton_Facebook();
	
	UFUNCTION()
	void OnButton_IOS();
	UFUNCTION()
	void OnButton_Guest();

	void InvalidPlatform();

private:

};
