// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "UI/RBaseWidget.h"
#include "UI/UIDefs.h"
#include "UI/Common/UC_ItemIcon.h"

#include "Global/RGameInstance.h"

#include "UC_RuneItemInfo.generated.h"


/**
 * 
 */
UCLASS()
class PROJECTR_API UUC_RuneItemInfo : public URBaseWidget
{
	GENERATED_BODY()
public:
	void				NativeConstruct()	override;
	void				NativeDestruct()	override;

	void				InvalidateBasicStats(const TArray<int32>& BasicOptions);
	void				InvalidateSubstats(const TArray<int32>& BasicOptions);
	void				RefreshStatUI(EItemStatOption ItemStatOption, int32 Value, TArray<UTextBlock*> LabelTextBlocks, TArray<UTextBlock*> ValueTextBlocks, int32 WidgetIndex);

	UFUNCTION()
	void				SetInValidData(FString InCurrentItemUD);
	UFUNCTION()
	void				SetItemData();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UUC_RuneItemInfo)
	UCanvasPanel*		BaseOptionInfoText	=	nullptr;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UUC_RuneItemInfo)
	UCanvasPanel*		BaseOptionInfo		=	nullptr;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UUC_RuneItemInfo)
	UCanvasPanel*		AddOptionInfoText	=	nullptr;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UUC_RuneItemInfo)
	UCanvasPanel*		AddOptionInfo		=	nullptr;

	UPROPERTY()
	TArray<UTextBlock*> BasicStatLabelTextBlocks;
	UPROPERTY()
	TArray<UTextBlock*> BasicStatValueTextBlocks;
	UPROPERTY()
	TArray<UTextBlock*> SubstatLabelTextBlocks;
	UPROPERTY()
	TArray<UTextBlock*> SubstatValueTextBlocks;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UUC_RuneItemInfo)
	UUC_GenericItemIcon*		RuneIcon			=	nullptr;

private:
	FString							CurrentItemUD;
};
