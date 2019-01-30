// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "UI/RBaseWidget.h"
#include "UI/UIDefs.h"
#include "UI/RuneItem/UC_RuneItemInfo.h"
#include "UI/RuneItem/UC_RuneList_ScrollView.h"
#include "UI/RuneItem/UC_RuneOption.h"

#include "Global/RGameInstance.h"

#include "UP_RuneItemMix.generated.h"


/**
 * 
 */
UCLASS()
class PROJECTR_API UUP_RuneItemMix : public URBaseWidget
{
	GENERATED_BODY()
public:
	void				NativeConstruct()	override;
	void				NativeDestruct()	override;

	void				SetInValidData();
	void				SetTextCount();
	void				DelegateAdd();
	void				DelegateRemove();

	UFUNCTION()
	void OnClick_RuneIcon(URScrollItemData* InSelectedHeroScrollItemData);
	UFUNCTION()
	void OnClick_CountUp();
	UFUNCTION()
	void OnClick_CountDown();
	UFUNCTION()
	void OnClick_Mix();
	UFUNCTION()
	void UpdateRuneMix();


	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UUP_RuneItemMix)
	UUC_RuneList_ScrollView*	BaseRuneScrollView		=	nullptr;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UUP_RuneItemMix)
	UImage*						Image_Rune				=	nullptr;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UUP_RuneItemMix)
	UImage*						Image_MixRune			=	nullptr;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UUP_RuneItemMix)
	UTextBlock*					TextBlock_Count			=	nullptr;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UUP_RuneItemMix)
	USlider*					Slider_Count			=	nullptr;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UUP_RuneItemMix)
	UButton*					Button_Mix				=	nullptr;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UUP_RuneItemMix)
	UButton*					Button_Up				=	nullptr;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UUP_RuneItemMix)
	UButton*					Button_Down				=	nullptr;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UUP_RuneItemMix)
	UTextBlock*					TextBlock_CurrentCount	=	nullptr;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UUP_RuneItemMix)
	UTextBlock*					TextBlock_MaxCount		=	nullptr;

	int32						Count;
	int32						CurrentRuneIndex;
};
