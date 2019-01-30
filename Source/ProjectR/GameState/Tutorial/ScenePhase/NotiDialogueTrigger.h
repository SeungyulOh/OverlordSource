// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Object.h"
#include "SharedConstants/GlobalConstants.h"
#include "NotiDialogueTrigger.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTR_API UNotiDialogueTrigger : public UObject
{
	GENERATED_BODY()
	
public:
	void CheckTrigger();

private:
	FVector				Location;
	FName				DialogueID;

	bool				IsTrigger = false;
	//bool				IsOnTrigger = false;
};

