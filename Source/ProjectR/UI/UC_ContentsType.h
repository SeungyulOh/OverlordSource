// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Blueprint/UserWidget.h"
#include "Network/PacketDataEnum.h"
#include "UC_ContentsType.generated.h"

class ULocalizingTextBlock;

/**
 * 
 */
UCLASS()
class PROJECTR_API UUC_ContentsType : public UUserWidget
{
	GENERATED_BODY()
public:
	void NativeConstruct() override;

	UFUNCTION(BlueprintCallable, Category = UC_ContentsType)
	void OnClickShortCut();
public:
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = UC_ContentsType)
	ULocalizingTextBlock*					TextBlock_Title;
	
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = UC_ContentsType)
	ULocalizingTextBlock*					LocTextBlock_Contents;

	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = UC_ContentsType)
	ULocalizingTextBlock*					LocTextBlock_Desc;
	
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = UC_ContentsType)
	EGamePlayModeTypeEnum								eContentsType = EGamePlayModeTypeEnum::VE_None;

	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = UC_ContentsType)
	UButton*								Button_OK;

};
