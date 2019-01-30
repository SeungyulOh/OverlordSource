// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "UI/Control/RScrollItemData.h"
#include "UC_Chat_BanList_ScrollData.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTR_API UUC_Chat_BanList_ScrollData : public URScrollItemData
{
	GENERATED_BODY()
	
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Chat)
	int32 kId;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Chat)
	FString who;
};
