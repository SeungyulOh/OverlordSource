#pragma once

#include "SharedConstants/SharedGlobalConstants.h"
#include "SharedConstants/GlobalConstants.h"

#include "TowerSkillTableInfo.generated.h"

USTRUCT(BlueprintType)
struct PROJECTR_API FTowerSkillTable : public FTableRowBase
{
	GENERATED_USTRUCT_BODY()

public:
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = TowerSkillTable)
	FName		SkillName;

	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = TowerSkillTable)
	FName		Description;

	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = TowerSkillTable)
	FName		Summary;

	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = TowerSkillTable)
	FName		SkillResourceID;

	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = TowerSkillTable)
	ESkillAniTypeEnum		SkillAniType = ESkillAniTypeEnum::VE_None;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = TowerSkillTable)
	ESkillControlTypeEnum	SkillControlType = ESkillControlTypeEnum::VE_None;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = TowerSkillTable)
	ESkillAttributeEnum		SkillAttribute = ESkillAttributeEnum::VE_None;

	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = TowerSkillTable)
	ESkillCastingTypeEnum	CastingType = ESkillCastingTypeEnum::VE_None;

	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = TowerSkillTable)
	float					CastingTime = 0.3f;

	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = TowerSkillTable)
	float					CoolTime = 1.0f;

	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = TowerSkillTable)
	float					CoolTimeAdd1 = 1.0f;

	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = TowerSkillTable)
	float					CoolTimeAdd2 = 1.0f;

	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = TowerSkillTable)
	float					CrewCoolTime = 1.0f;

	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = TowerSkillTable)
	float					Interval = 0.0f;

	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = TowerSkillTable)
	int32					CastingRange = 100;

	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = TowerSkillTable)
	EAbnormalApplyTypeEnum	CasterAbnormalType = EAbnormalApplyTypeEnum::VE_None;

	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = TowerSkillTable)
	EAbnormalGroupTypeEnum	CasterGroupType = EAbnormalGroupTypeEnum::VE_None;

	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = TowerSkillTable)
	EAbnormalSlotTypeEnum	CasterSlotType = EAbnormalSlotTypeEnum::VE_None;

	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = TowerSkillTable)
	int32					CasterSlotLevel = 0;

	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = TowerSkillTable)
	float					CasterDuration = 0.0f;

	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = TowerSkillTable)
	ESkillApplierTypeEnum		CasterEffect1;

	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = TowerSkillTable)
	float		CasterPercent1 = 0.0f;

	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = TowerSkillTable)
	int32		CasterValue1 = 0;

	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = TowerSkillTable)
	float		CasterMovingDuration = 0.0f;

	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = TowerSkillTable)
	ESkillApplierTypeEnum		CasterEffect2;

	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = TowerSkillTable)
	float		CasterPercent2 = 0.0f;

	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = TowerSkillTable)
	int32		CasterValue2 = 0;

	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = TowerSkillTable)
	ESkillApplierTypeEnum		CasterEffect3;

	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = TowerSkillTable)
	float		CasterPercent3 = 0.0f;

	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = TowerSkillTable)
	int32		CasterValue3 = 0;

	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = TowerSkillTable)
	ESkillApplierTypeEnum		CasterEffect4;

	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = TowerSkillTable)
	float		CasterPercent4 = 0.0f;

	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = TowerSkillTable)
	int32		CasterValue4 = 0;

	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = TowerSkillTable)
	ESkillApplierTypeEnum		CasterEffect5;

	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = TowerSkillTable)
	float		CasterPercent5 = 0.0f;

	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = TowerSkillTable)
	int32		CasterValue5 = 0;

	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = TowerSkillTable)
	ESkillApplierTypeEnum		CasterEffect6;

	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = TowerSkillTable)
	float		CasterPercent6 = 0.0f;

	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = TowerSkillTable)
	int32		CasterValue6 = 0;

	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = TowerSkillTable)
	EAbnormalApplyTypeEnum	TargetAbnormalType = EAbnormalApplyTypeEnum::VE_None;

	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = TowerSkillTable)
	EAbnormalGroupTypeEnum	TargetGroupType = EAbnormalGroupTypeEnum::VE_None;

	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = TowerSkillTable)
	EAbnormalSlotTypeEnum	TargetSlotType = EAbnormalSlotTypeEnum::VE_None;

	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = TowerSkillTable)
	int32					TargetSlotLevel = 0;

	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = TowerSkillTable)
	float					TargetDuration = 0.0f;

	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = TowerSkillTable)
	ESkillApplierTypeEnum		TargetEffect1;

	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = TowerSkillTable)
	float		TargetPercent1 = 0.0f;

	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = TowerSkillTable)
	int32		TargetValue1 = 0;

	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = TowerSkillTable)
	float		TargetMovingDuration = 0.0f;

	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = TowerSkillTable)
	ESkillApplierTypeEnum		TargetEffect2;

	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = TowerSkillTable)
	float		TargetPercent2 = 0.0f;

	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = TowerSkillTable)
	int32		TargetValue2 = 0;

	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = TowerSkillTable)
	ESkillApplierTypeEnum		TargetEffect3;

	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = TowerSkillTable)
	float		TargetPercent3 = 0.0f;

	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = TowerSkillTable)
	int32		TargetValue3 = 0;

	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = TowerSkillTable)
	ESkillApplierTypeEnum		TargetEffect4;

	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = TowerSkillTable)
	float		TargetPercent4 = 0.0f;

	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = TowerSkillTable)
	int32		TargetValue4 = 0;

	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = TowerSkillTable)
	ESkillApplierTypeEnum		TargetEffect5;

	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = TowerSkillTable)
	float		TargetPercent5 = 0.0f;

	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = TowerSkillTable)
	int32		TargetValue5 = 0;

	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = TowerSkillTable)
	ESkillApplierTypeEnum		TargetEffect6;

	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = TowerSkillTable)
	float		TargetPercent6 = 0.0f;

	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = TowerSkillTable)
	int32		TargetValue6 = 0;

	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = TowerSkillTable)
	int32		ApplyProcedure = 0;

	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = TowerSkillTable)
	ESkillTargetTypeEnum		TargetType = ESkillTargetTypeEnum::VE_None;

	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = TowerSkillTable)
	EFriendshipTypeEnum	TargetApply = EFriendshipTypeEnum::VE_None;

	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = TowerSkillTable)
	EPriorityTypeEnum		AISearchTargetType;

	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = TowerSkillTable)
	ESearchShapeTypeEnum		EffectShape = ESearchShapeTypeEnum::VE_None;

	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = TowerSkillTable)
	TArray<float>				ShapeScope;

	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = TowerSkillTable)
	float						SummonTime;

	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = TowerSkillTable)
	FName AICondition1;

	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = TowerSkillTable)
	FName AICondition2;

	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = TowerSkillTable)
	FName AICondition3;

	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = TowerSkillTable)
	float AIForceUseOverTime;

	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = TowerSkillTable)
	float StartCoolTime;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = TowerSkillTable)
	ESkillShapedIconType	UISkillShape = ESkillShapedIconType::VE_Me;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = TowerSkillTable)
	int32					UISkillRange = 0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = TowerSkillTable)
	bool					VisibleDecal = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = TowerSkillTable)
	bool					VisibleDPAD = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = TowerSkillTable)
	bool					VisibleNotifyIcon = false;

	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = TowerSkillTable)
	float CombineInputRange;

	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = TowerSkillTable)
	float JustInputStart;

	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = TowerSkillTable)
	float JustInputEnd;
};
