// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "UI/Control/RScrollItemData.h"
#include "UI/UIDefs.h"
#include "NotifyList/NotifyEvent.h"
#include "LobbyNotifyScrollItemData.generated.h"

class UTexture2D;
class UNotifyEvent;
/**
 * 
 */
UCLASS()
class PROJECTR_API ULobbyNotifyScrollItemData : public URScrollItemData
{
	GENERATED_BODY()
	
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = ULobbyNotifyScrollItemData)
	UNotifyEvent*			NotifyEvent = nullptr;
};
