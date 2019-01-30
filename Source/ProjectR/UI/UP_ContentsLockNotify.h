// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "UI/RBaseWidget.h"
#include "Network/PacketDataEnum.h"
#include "UP_ContentsLockNotify.generated.h"

class ULocalizingTextBlock;

/**
 * 
 */
UCLASS()
class PROJECTR_API UUP_ContentsLockNotify : public URBaseWidget
{
	GENERATED_BODY()
	
public:
	void RefreshUI(EOVERLORD InContentsType);

public:
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = UP_ContentsLockNotify)
	ULocalizingTextBlock*					TextBlock_Notify;
	
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = UP_ContentsLockNotify)
	UTextBlock*								Text_UnLockLevel;
};
