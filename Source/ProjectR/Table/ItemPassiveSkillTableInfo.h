#pragma once

#include "SharedConstants/GlobalConstants.h"
#include "SkillTableInfo_V2.h"
#include "ItemPassiveSkillTableInfo.generated.h"

/**
 * 
 */
USTRUCT(BlueprintType)
struct PROJECTR_API FItemPassiveSkillTableInfo : public FSkillTableInfo_V2
{
	GENERATED_USTRUCT_BODY()


public:
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = ItemPassiveSkillTableInfo)
	TAssetPtr< UTexture2D >			PassiveIconRef = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Skill)
	FName					ItemPrefix;
};

