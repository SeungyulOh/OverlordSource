#pragma once


#include "Object.h"
#include "Table/RaidStageTableInfo.h"
#include "SharedConstants/SharedGlobalConstants.h"

#include "RaidManager.generated.h"

/**
*
*/
UCLASS()
class PROJECTR_API URaidManager : public UObject
{
	GENERATED_BODY()

public:
	void Initialize(/*URGameInstance* InRGameInstance*/);

	void SetTableData();

public:
	TMap<ERaidDayType, TArray<FRaidStageTableInfo> >		RaidStageList;
	ERaidDayType		TodayType;// = ERaidDayType::WEEK;
};