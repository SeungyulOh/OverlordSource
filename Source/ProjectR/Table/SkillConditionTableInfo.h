#pragma once
#include "SharedConstants/GlobalConstants.h"
#include "SkillConditionTableInfo.generated.h"

/**
 * 
 */
USTRUCT(BlueprintType)
struct PROJECTR_API FSkillConditionTableInfo : public FTableRowBase
{
	GENERATED_USTRUCT_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = SkillConditionTableInfo)
	ESkillTargetTypeEnum		ConditionCheckTarget;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = SkillConditionTableInfo)
	EFriendshipTypeEnum			ConditionCheckTargetApply;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = SkillConditionTableInfo)
	int32						ConditionCheckRange;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = SkillConditionTableInfo)
	int32						ConditionCheckApplyValue;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = SkillConditionTableInfo)
	int32						ConditionCheckDistanceMin;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = SkillConditionTableInfo)
	int32						ConditionCheckDistanceMax;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = SkillConditionTableInfo)
	EAbnormalApplyTypeEnum		OnAbnormalBuff;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = SkillConditionTableInfo)
	EAbnormalApplyTypeEnum		OffAbnormalBuff;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = SkillConditionTableInfo)
	EAbnormalGroupTypeEnum		OnGroupState;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = SkillConditionTableInfo)
	EAbnormalGroupTypeEnum		OffGroupState;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = SkillConditionTableInfo)
	EAbnormalSlotTypeEnum		OnSlotState;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = SkillConditionTableInfo)
	EAbnormalSlotTypeEnum		OffSlotState;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = SkillConditionTableInfo)
	int32						RemainHpPercent;
};