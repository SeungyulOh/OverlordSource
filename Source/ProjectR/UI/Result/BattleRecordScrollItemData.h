// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "UI/UIDefs.h"
#include "UI/Control/RScrollItemData.h"
#include "Global/BattleRecordManager.h"
#include "BattleRecordScrollItemData.generated.h"



/**
 * 
 */
UCLASS()
class PROJECTR_API UBattleRecordScrollItemData : public URScrollItemData
{
	GENERATED_BODY()
	
public:
	bool				MyRecord = false;

	bool				AllyRecord = false;
	bool				GroupPlayMode = false;

	EBattleRecordType	RecordType = EBattleRecordType::VE_RecordTypeMax;
	int32				SumByRecordType = 0;

	int32				RecordRank = 999;
	FBattleRecord*		BattleRecord = nullptr;
};