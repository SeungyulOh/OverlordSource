// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "UI/RBaseWidget.h"
#include "UI/UIDefs.h"
#include "UI/RuneItem/UC_RuneItemSlot.h"

#include "Global/RGameInstance.h"

#include "UC_RuneOption.generated.h"


/**
 * 
 */
UCLASS()
class PROJECTR_API UUC_RuneOption : public URBaseWidget
{
	GENERATED_BODY()
public:
	void				NativeConstruct()	override;
	void				NativeDestruct()	override;

	void				SetInValidData(FString InCurrentItemUD);
	void				SetSelectRune(int32 InSelectRune);

	void				DelegateAdd();
	void				DelegateRemove();

	UFUNCTION()
	void				RuneSlotUpdate();

	int32							SelectRuneID;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UUC_RuneOption)
	UUC_RuneItemSlot*				RuneSlot_01		=	nullptr;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UUC_RuneOption)
	UUC_RuneItemSlot*				RuneSlot_02		=	nullptr;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UUC_RuneOption)
	UUC_RuneItemSlot*				RuneSlot_03		=	nullptr;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UUC_RuneOption)
	UUC_RuneItemSlot*				RuneSlot_04		=	nullptr;

	TArray<UUC_RuneItemSlot*>		RuneSlotList;

private:
	FString							CurrentItemUD;
};
