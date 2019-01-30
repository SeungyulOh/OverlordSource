#pragma once

#include "SharedConstants/GlobalConstants.h"
#include "WorkedSkillTableInfo.generated.h"

struct FSkillTableInfo_V2;
/**
*
*/
USTRUCT(BlueprintType)
struct PROJECTR_API FWorkedSkillTableInfo : public FTableRowBase
{
	GENERATED_USTRUCT_BODY()

public:
	FSkillTableInfo_V2*		SkillTableInfo_V2 = nullptr;
};
