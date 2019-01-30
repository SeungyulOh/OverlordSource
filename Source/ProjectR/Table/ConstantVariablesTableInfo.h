#pragma once

#include "SharedConstants/GlobalConstants.h"
#include "ConstantVariablesTableInfo.generated.h"

/**
*
*/
USTRUCT(BlueprintType)
struct PROJECTR_API FConstantVariablesTableInfo : public FTableRowBase
{
	GENERATED_USTRUCT_BODY()


public:
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = ConstantVariablesTableInfo)
	EConstantType				Type;

	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = ConstantVariablesTableInfo)
	FString						Value;


public:
	bool GetValue(int32& outValue);
	bool GetValue(float& outValue);
	bool GetValue(FString& outValue);
};
