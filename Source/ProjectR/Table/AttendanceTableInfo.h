#pragma once

#include "SharedConstants/GlobalConstants.h"
#include "AttendanceTableInfo.generated.h"

/**
*
*/
USTRUCT(BlueprintType)
struct PROJECTR_API FAttendanceTableInfo : public FTableRowBase
{
	GENERATED_USTRUCT_BODY()


public:
	
	//UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "AttendanceTableInfo|Server")
	//EAttendanceType				Type;

	//UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "AttendanceTableInfo|Server")
	//EAttendanceUserType			Type2;
	
	//UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "AttendanceTableInfo|Server")
	//FName						StartDate;

	//UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "AttendanceTableInfo|Server")
	//FName						FinishDate;

	//UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "AttendanceTableInfo|Server")
	//FName						StringIndex;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = URaidStageTableInfo)
	TArray<FName>				RewardList;

};
