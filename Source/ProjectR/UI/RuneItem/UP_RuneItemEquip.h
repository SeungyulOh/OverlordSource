// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "UI/RBaseWidget.h"
#include "UI/UIDefs.h"
#include "UI/RuneItem/UC_RuneItemInfo.h"
#include "UI/RuneItem/UC_RuneList_ScrollView.h"
#include "UI/RuneItem/UC_RuneOption.h"

#include "Global/RGameInstance.h"

#include "UP_RuneItemEquip.generated.h"


/**
 * 
 */
UCLASS()
class PROJECTR_API UUP_RuneItemEquip : public URBaseWidget
{
	GENERATED_BODY()
public:
	void				NativeConstruct()	override;
	void				NativeDestruct()	override;

	void				SetInValidData();
	void				DelegateAdd();
	void				DelegateRemove();


	UFUNCTION()
	void				OnClickedGoRuneMix();
	UFUNCTION()
	void				OnClick_RuneIcon(URScrollItemData* InSelectedHeroScrollItemData);
	UFUNCTION()
	void				RuneItemUpdate();
	UFUNCTION()
	void				RuneItemAllRelease();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UUP_RuneItemEquip)
	UUC_RuneItemInfo*			RuneItemInfo		=	nullptr;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UUP_RuneItemEquip)
	UUC_RuneList_ScrollView*	BaseRuneScrollView	=	nullptr;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UUP_RuneItemEquip)
	UUC_RuneOption*				RuneOption			=	nullptr;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UUP_RuneItemEquip)
	UButton*					Button_RuneMix		=	nullptr;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UUP_RuneItemEquip)
	UButton*					Button_AllRelease	=	nullptr;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UUP_RuneItemEquip)
	UTextBlock*					TextBlock_RuneWord	=	nullptr;
	

private:
	FString					CurrentItemUD;
};
