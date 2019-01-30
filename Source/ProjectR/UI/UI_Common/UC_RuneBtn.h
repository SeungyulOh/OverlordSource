// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "UI/RBaseWidget.h"
#include "UI/UIDefs.h"
#include "UC_RuneBtn.generated.h"



/**
 * 
 */
UCLASS()
class PROJECTR_API UUC_RuneBtn : public URBaseWidget
{
	GENERATED_BODY()
	
public:

	void NativeConstruct() override;

	//UFUNCTION(BlueprintCallable, Category = TopBar)
	//void PrevButton();

public:
	//UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = TopBar)
	//UWidgetSwitcher* WidgetSiwtcher_Account = nullptr;
	
};