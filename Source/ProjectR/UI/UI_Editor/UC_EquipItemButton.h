// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "UI/RBaseWidget.h"
#include "SharedConstants/GlobalConstants.h"
#include "Network/PacketDataStructures.h"
#include "CustomStructures/HeroSlotInfo.h"
#include "UC_EquipItemButton.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FEquiptItemButtonDelegate_NoParam);

/**
 * 
 */

UCLASS()
class PROJECTR_API UUC_EquipItemButton : public URBaseWidget
{
	GENERATED_BODY()
	
public:
	void NativeConstruct() override;
	void InvalidateData() override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UUC_EquipItemButton)
	UImage*					ItemImage = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UUC_EquipItemButton)
	UBorder*				ItemGradeBorder = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UUC_EquipItemButton)
	UTextBlock*				ItemText = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UUC_EquipItemButton)
	FName					CurSelectItemKey;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UUC_EquipItemButton)
	TArray<FName>			GemKeyList;

	UPROPERTY(BlueprintAssignable, BlueprintCallable, Category = UUC_EquipItemButton)
	FEquiptItemButtonDelegate_NoParam	OnButtonClicked;

	UFUNCTION(BlueprintCallable, Category = UUC_EquipItemButton)
	void					ButtonClicked();

	void					SetCurItem(FName inItemKey);
	static void				EquipItem(AActor* Obj, FITEM& InItem);
	static bool				GetFItem(FString InItemStr, FITEM& OutItem);
	static bool				GetFItem(FTestItemInfo InItemInfo, FITEM& OutItem);
};
