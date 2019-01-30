// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "UI/RBaseWidget.h"
#include "UI/Control/RScrollItem.h"
#include "UC_Guild_RuneRequest.generated.h"

class UHeroBaseScrollItem;
/**
 * 
 */
UCLASS()
class PROJECTR_API UUC_Guild_RuneRequest : public URScrollItem
{
	GENERATED_BODY()
	
public:

	void NativeConstruct() override;
	void NativeDestruct() override;

	void SetScrollItemData(int32 Index, URScrollItemData* item, URScrollView* ScrollView) override;
	
	UFUNCTION(BlueprintCallable, Category = UUC_Guild_RuneRequest)
	void OnClick_RuneCount(bool Add);
	UFUNCTION(BlueprintCallable, Category = UUC_Guild_RuneRequest)
	void OnClick_RequestRuneSupport();
	UFUNCTION(BlueprintCallable, Category = UUC_Guild_RuneRequest)
	void OnClick_RequestRuneGet();

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UUC_Guild_RuneRequest)
	UHeroBaseScrollItem*	HeroScrollItem;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UUC_Guild_RuneRequest)
	UWidgetSwitcher*	MemberGrade;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UUC_Guild_RuneRequest)
	UWidgetSwitcher*	SupportType;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UUC_Guild_RuneRequest)
	UTextBlock*		Text_Nick;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UUC_Guild_RuneRequest)
	UTextBlock*		Text_CurrentHave;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UUC_Guild_RuneRequest)
	UTextBlock*		Text_RemainHour;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UUC_Guild_RuneRequest)
	UTextBlock*		Text_RemainMinute;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UUC_Guild_RuneRequest)
	UTextBlock*		Text_CurrentAmount;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UUC_Guild_RuneRequest)
	UTextBlock*		Text_MaxAmount;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UUC_Guild_RuneRequest)
	UTextBlock*		Text_SupportAmount;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UUC_Guild_RuneRequest)
	UButton*		Btn_ReceiveRune;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UUC_Guild_RuneRequest)
	UButton*		Btn_RuneCountMinus;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UUC_Guild_RuneRequest)
	UButton*		Btn_RuneCountPlus;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UUC_Guild_RuneRequest)
	UProgressBar*	ReceiveRune;

	UPROPERTY()
	int32	RequestCount;
};
