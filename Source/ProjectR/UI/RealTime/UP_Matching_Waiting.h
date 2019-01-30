// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "SharedConstants/GlobalConstants.h"
#include "UI/RBaseWidget.h"
#include "UP_Matching_Waiting.generated.h"


class ULocalizingTextBlock;

/**
 * 
 */
UCLASS()
class PROJECTR_API UUP_Matching_Waiting : public URBaseWidget
{
	GENERATED_BODY()
	
public:

	void NativeConstruct() override;
	void NativeDestruct() override;

	void RefreshUI(EPVP InConnecMode);

	UFUNCTION(BlueprintCallable, Category = UUP_Matching_Waiting)
	void OnClick_MatchingWaitingCancel();

public:
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = UUP_Matching_Waiting)
	ULocalizingTextBlock*		ContentsTextBlock			=	nullptr;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = UUP_Matching_Waiting)
	UButton*					Button_MatchingCancel		=	nullptr;
	
};
