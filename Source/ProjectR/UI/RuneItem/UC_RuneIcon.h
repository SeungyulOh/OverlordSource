// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "UI/RBaseWidget.h"
#include "UI/UIDefs.h"
#include "UI/RuneItem/UC_RuneItemSlot.h"

#include "Global/RGameInstance.h"

#include "Table/ItemTableInfo.h"

#include "UC_RuneIcon.generated.h"


/**
 * 
 */
UCLASS()
class PROJECTR_API UUC_RuneIcon : public URBaseWidget
{
	GENERATED_BODY()
public:
	void				NativeConstruct()	override;
	void				NativeDestruct()	override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UUC_RuneIcon)
	UImage*				Image_Rune				=	nullptr;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UUC_RuneIcon)
	UWidgetSwitcher*	WidgetSwitcher_Select	=	nullptr;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UUC_RuneIcon)
	UTextBlock*			TextBlock_Count			=	nullptr;
	

	FORCEINLINE void	SetIsSelectEnable(bool InbEnableSelect) { bIsSelectEnable = InbEnableSelect; }
	FORCEINLINE void	SetIsSelected(bool InbIsSelected) { bIsSelected = InbIsSelected; }
	FORCEINLINE bool	GetIsSelectEnable() { return bIsSelectEnable; }
	FORCEINLINE bool	GetIsSelected() { return bIsSelected; }

	void				SetRune(int32 InRuneId, int32 InRuneCount);
	void				SelectIconRune();
	void				SelectHeroIconBySwitcher();

	int32			RuneId		=	0;
	FRuneTable*		RuneTalbe	=	nullptr;
	bool			bIsSelectEnable = false;
	bool			bIsSelected = false;
};
