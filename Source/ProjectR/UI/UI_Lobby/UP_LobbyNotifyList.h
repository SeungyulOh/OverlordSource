// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "UI/RBaseWidget.h"
#include "UI/UIDefs.h"
#include "Network/PacketDataStructures.h"
#include "UP_LobbyNotifyList.generated.h"

class URScrollView;
class UNotifyEvent;
/**
 * 
 */
UCLASS()
class PROJECTR_API UUP_LobbyNotifyList : public URBaseWidget
{
	GENERATED_BODY()	
	
public:
	void NativeConstruct() override;
	void InvalidateData() override;

	UFUNCTION()
	void OnAddNotifyEvent(UNotifyEvent* notify);

	UFUNCTION()
	void OnRemoveNotifyEvent(UNotifyEvent* notify);

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UUP_LobbyNotifyList)
	URScrollView*			NotifyScrollView = nullptr;
};