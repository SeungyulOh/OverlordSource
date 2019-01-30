// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "UI/RBaseWidget.h"
#include "UI/UIDefs.h"

#include "Global/RGameInstance.h"

#include "UC_RuneItemSlot.generated.h"


/**
 * 
 */
UCLASS()
class PROJECTR_API UUC_RuneItemSlot : public URBaseWidget
{
	GENERATED_BODY()
public:
	void				NativeConstruct()	override;
	void				NativeDestruct()	override;

	void				Init(int32 InIndex, FString InCurrentItemUD);
	void				SetEquipSelectRune(int32 InSelectRuneID);
	void				RuneEquip(int32 InSlot);
	void				RuneRelease(int32 InSlot);
	void				RuneStatInfo();

	UFUNCTION()
	void				OnClickRuneEquip();
	UFUNCTION()
	void				OnClickRuneRelease();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UUC_RuneItemSlot)
	UImage*				RuneSlotImage			=	nullptr;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UUC_RuneItemSlot)
	UTextBlock*			RuneOptionText			=	nullptr;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UUC_RuneItemSlot)
	UTextBlock*			RuneOption				=	nullptr;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UUC_RuneItemSlot)
	UWidgetSwitcher*	WidgetSwitcher_Button	=	nullptr;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UUC_RuneItemSlot)
	UButton*			Button_Equip			=	nullptr;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UUC_RuneItemSlot)
	UButton*			Button_Release			=	nullptr;

	int32				SlotIndex		=	0;
	int32				SelectRuneID	=	0;
	FString				CurrentItemUD;
};
