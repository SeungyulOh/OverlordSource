// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "UI/RBaseWidget.h"
#include "Network/Chat/ChatManager.h"
#include "UI/Control/RScrollView.h"
#include "UC_Chat_BanList.generated.h"

/**
 * 
 */
 UCLASS()
class PROJECTR_API UUC_Chat_BanList : public URBaseWidget
{
	GENERATED_BODY()
	
public:

	void NativeConstruct() override;
	void NativeDestruct() override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Chat)
	class URScrollView* Scroll_BanList = nullptr;

	UFUNCTION()
	void InvalidateData();

private:
	UPROPERTY()
	class UChatManager* ChatManager = nullptr;
};
