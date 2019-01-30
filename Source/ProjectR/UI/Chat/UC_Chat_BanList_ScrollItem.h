// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "UI/Control/RScrollItem.h"
#include "Network/Chat/ChatManager.h"
#include "UC_Chat_BanList_ScrollItem.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTR_API UUC_Chat_BanList_ScrollItem : public URScrollItem
{
	GENERATED_BODY()
	
public:
	void NativeConstruct() override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Chat)
	class UTextBlock* Text_Name = nullptr;

	UFUNCTION(BlueprintCallable, Category = Chat)
	void Unblock();

	void InvalidateData() override;

private:
	UPROPERTY()
	class UChatManager* ChatManager = nullptr;
};
