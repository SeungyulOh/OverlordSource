#pragma once

#include "SharedConstants/GlobalConstants.h"
#include "SkillTableInfo_V2.generated.h"

/**
 * 
 */
USTRUCT(BlueprintType)
struct PROJECTR_API FSkillTableInfo_V2 : public FTableRowBase
{
	GENERATED_USTRUCT_BODY()

  public:
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = Skill)
	FName SkillName;

	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = Skill)
	FName Description;

	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = Skill)
	FName Summary;

	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = Skill)
	FName SkillResourceID;

	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = Skill)
	FName SkillDefinitionId;

	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = Skill)
	FName SkillCategoryType;

	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = Skill)
	int32 BattlePower;

	/**
	 * 스킬 시전 조건에 해당
	 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Skill)
	ESkillControlTypeEnum SkillControlType = ESkillControlTypeEnum::VE_None;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Skill)
	ESkillAttributeEnum SkillAttribute = ESkillAttributeEnum::VE_None;

	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = Skill)
	ESkillCastingTypeEnum CastingType = ESkillCastingTypeEnum::VE_None;

	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = Skill)
	float CastingTime = 0.3f;

	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = Skill)
	float CoolTime = 1.0f;

	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = Skill)
	float CrewCoolTime = 1.0f;

	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = Skill)
	float Interval = 0.0f;

	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = Skill)
	int32 CastingRange = 100;

	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = Skill)
	ESkillTargetTypeEnum TargetType = ESkillTargetTypeEnum::VE_None;

	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = Skill)
	EFriendshipTypeEnum TargetApply = EFriendshipTypeEnum::VE_None;

	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = Skill)
	EPriorityTypeEnum AISearchTargetType;

	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = Skill)
	ESearchShapeTypeEnum EffectShape = ESearchShapeTypeEnum::VE_None;

	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = Skill)
	TArray<float> ShapeScope;

	// Areaskill life time
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = Skill)
	float GroundDuration;

	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = Skill)
	float GroundInterval = -1.0f;

	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = Skill)
	float AnimHitTime;

	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = Skill)
	int32 ApplyProcedure = 0;

	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = Skill)
	ESkillApplierTypeEnum CastingEffect = ESkillApplierTypeEnum::VE_None;
	// In cm
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = Skill)
	EAbnormalApplyTypeEnum CasterAbnormalType = EAbnormalApplyTypeEnum::VE_None;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = Skill)
	EAbnormalGroupTypeEnum CasterGroupType = EAbnormalGroupTypeEnum::VE_None;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = Skill)
	EAbnormalSlotTypeEnum CasterSlotType = EAbnormalSlotTypeEnum::VE_None;

	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = Skill)
	int32 CasterSlotLevel = 0;

	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = Skill)
	float CasterDuration = 0.0f;

	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = Skill)
	ESkillApplierTypeEnum CasterEffect1;

	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = Skill)
	float CasterPercent1 = 0.0f;

	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = Skill)
	int32 CasterValue1 = 0;

	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = Skill)
	ESkillApplierTypeEnum CasterEffect2;

	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = Skill)
	float CasterPercent2 = 0.0f;

	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = Skill)
	int32 CasterValue2 = 0;

	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = Skill)
	ESkillApplierTypeEnum CasterEffect3;

	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = Skill)
	float CasterPercent3 = 0.0f;

	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = Skill)
	int32 CasterValue3 = 0;

	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = Skill)
	ESkillApplierTypeEnum CasterEffect4;

	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = Skill)
	float CasterPercent4 = 0.0f;

	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = Skill)
	int32 CasterValue4 = 0;

	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = Skill)
	ESkillApplierTypeEnum CasterEffect5;

	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = Skill)
	float CasterPercent5 = 0.0f;

	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = Skill)
	int32 CasterValue5 = 0;

	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = Skill)
	ESkillApplierTypeEnum CasterEffect6;

	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = Skill)
	float CasterPercent6 = 0.0f;

	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = Skill)
	int32 CasterValue6 = 0;

	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = Skill)
	EAbnormalApplyTypeEnum TargetAbnormalType = EAbnormalApplyTypeEnum::VE_None;

	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = Skill)
	EAbnormalGroupTypeEnum TargetGroupType = EAbnormalGroupTypeEnum::VE_None;

	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = Skill)
	EAbnormalSlotTypeEnum TargetSlotType = EAbnormalSlotTypeEnum::VE_None;

	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = Skill)
	int32 TargetSlotLevel = 0;

	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = Skill)
	float TargetDuration = 0.0f;

	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = Skill)
	ESkillApplierTypeEnum TargetEffect1;

	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = Skill)
	float TargetPercent1 = 0.0f;

	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = Skill)
	int32 TargetValue1 = 0;

	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = Skill)
	ESkillApplierTypeEnum TargetEffect2;

	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = Skill)
	float TargetPercent2 = 0.0f;

	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = Skill)
	int32 TargetValue2 = 0;

	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = Skill)
	ESkillApplierTypeEnum TargetEffect3;

	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = Skill)
	float TargetPercent3 = 0.0f;

	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = Skill)
	int32 TargetValue3 = 0;

	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = Skill)
	ESkillApplierTypeEnum TargetEffect4;

	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = Skill)
	float TargetPercent4 = 0.0f;

	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = Skill)
	int32 TargetValue4 = 0;

	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = Skill)
	ESkillApplierTypeEnum TargetEffect5;

	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = Skill)
	float TargetPercent5 = 0.0f;

	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = Skill)
	int32 TargetValue5 = 0;

	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = Skill)
	ESkillApplierTypeEnum TargetEffect6;

	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = Skill)
	float TargetPercent6 = 0.0f;

	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = Skill)
	int32 TargetValue6 = 0;

	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = Skill)
	FString RTSSkillUseClass;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = Skill)
	FString RTSSkillCondition;

	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = Skill)
	FName AICondition1;

	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = Skill)
	FName AICondition2;

	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = Skill)
	FName AICondition3;

	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = Skill)
	float AIForceUseOverTime;

	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = Skill)
	float StartCoolTime;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Skill)
	ESkillShapedIconType UISkillShape = ESkillShapedIconType::VE_Me;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Skill)
	int32 UISkillRange = 0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Skill)
	bool VisibleDecal = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Skill)
	bool VisibleDPAD = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Skill)
	bool VisibleNotifyIcon = false;
};