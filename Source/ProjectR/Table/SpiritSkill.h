#pragma once

#include "SharedConstants/GlobalConstants.h"
#include "SpiritSkill.generated.h"

/**
 * 
 */
USTRUCT(BlueprintType)
struct PROJECTR_API FSpiritSkillTableInfo : public FTableRowBase
{
	GENERATED_USTRUCT_BODY()


public:
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = SpiritSkill)
	FName					SkillName;

	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = SpiritSkill)
	FName					Summary;

	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = SpiritSkill)
	FName					SkillResourceID;

	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = SpiritSkill)
	TAssetPtr< UParticleSystem >		ParticleResourceAsset;
	
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = SpiritSkill)
	FName					Description1;
	
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = SpiritSkill)
	FName					Description2;
	
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = SpiritSkill)
	FName					Description3;
	
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = SpiritSkill)
	FName					Description4;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = SpiritSkill)
	ESkillControlTypeEnum	SkillControlType = ESkillControlTypeEnum::VE_None;

	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = SpiritSkill)
	ESkillCastingTypeEnum	CastingType = ESkillCastingTypeEnum::VE_None;

	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = SpiritSkill)
	float					CastingTime = 0.3f;

	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = SpiritSkill)
	float					CoolTime = 1.0f;
	
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = SpiritSkill)
	float					CrewCoolTime = 1.0f;
	
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = SpiritSkill)
	float					Interval = 0.0f;
	// In cm
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = SpiritSkill)
	int32					CastingRange = 100;

	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = SpiritSkill)
	EAbnormalApplyTypeEnum	CasterAbnormalType = EAbnormalApplyTypeEnum::VE_None;

	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = SpiritSkill)
	EAbnormalGroupTypeEnum	CasterGroupType = EAbnormalGroupTypeEnum::VE_None;

	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = SpiritSkill)
	EAbnormalSlotTypeEnum	CasterSlotType = EAbnormalSlotTypeEnum::VE_None;

	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = SpiritSkill)
	int32					CasterSlotLevel = 0;

	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = SpiritSkill)
	float					CasterDuration = 0.0f;

	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = SpiritSkill)
	ESkillApplierTypeEnum	CasterEffect1;

	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = SpiritSkill)
	float					CasterPercent1 = 0.0f;

	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = SpiritSkill)
	int32					CasterValue1 = 0;

	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = SpiritSkill)
	ESkillApplierTypeEnum	CasterEffect2;

	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = SpiritSkill)
	float					CasterPercent2 = 0.0f;

	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = SpiritSkill)
	int32					CasterValue2 = 0;

	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = SpiritSkill)
	ESkillApplierTypeEnum	CasterEffect3;

	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = SpiritSkill)
	float		CasterPercent3 = 0.0f;

	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = SpiritSkill)
	int32		CasterValue3 = 0;

	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = SpiritSkill)
	ESkillApplierTypeEnum	CasterEffect4;

	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = SpiritSkill)
	float		CasterPercent4 = 0.0f;

	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = SpiritSkill)
	int32		CasterValue4 = 0;

	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = SpiritSkill)
	ESkillApplierTypeEnum		CasterEffect5;

	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = SpiritSkill)
	float		CasterPercent5 = 0.0f;

	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = SpiritSkill)
	int32		CasterValue5 = 0;

	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = SpiritSkill)
	ESkillApplierTypeEnum		CasterEffect6;

	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = SpiritSkill)
	float		CasterPercent6 = 0.0f;

	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = SpiritSkill)
	int32		CasterValue6 = 0;

	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = SpiritSkill)
	EAbnormalApplyTypeEnum	TargetAbnormalType = EAbnormalApplyTypeEnum::VE_None;

	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = SpiritSkill)
	EAbnormalGroupTypeEnum	TargetGroupType = EAbnormalGroupTypeEnum::VE_None;

	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = SpiritSkill)
	EAbnormalSlotTypeEnum	TargetSlotType = EAbnormalSlotTypeEnum::VE_None;

	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = SpiritSkill)
	int32					TargetSlotLevel = 0;

	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = SpiritSkill)
	float					TargetDuration = 0.0f;

	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = SpiritSkill)
	ESkillApplierTypeEnum		TargetEffect1;

	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = SpiritSkill)
	float		TargetPercent1 = 0.0f;

	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = SpiritSkill)
	int32		TargetValue1 = 0;

	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = SpiritSkill)
	ESkillApplierTypeEnum		TargetEffect2;

	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = SpiritSkill)
	float		TargetPercent2 = 0.0f;

	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = SpiritSkill)
	int32		TargetValue2 = 0;

	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = SpiritSkill)
	ESkillApplierTypeEnum		TargetEffect3;

	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = SpiritSkill)
	float		TargetPercent3 = 0.0f;

	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = SpiritSkill)
	int32		TargetValue3 = 0;

	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = SpiritSkill)
	ESkillApplierTypeEnum		TargetEffect4;

	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = SpiritSkill)
	float		TargetPercent4 = 0.0f;

	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = SpiritSkill)
	int32		TargetValue4 = 0;

	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = SpiritSkill)
	ESkillApplierTypeEnum		TargetEffect5;

	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = SpiritSkill)
	float		TargetPercent5 = 0.0f;

	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = SpiritSkill)
	int32		TargetValue5 = 0;

	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = SpiritSkill)
	ESkillApplierTypeEnum		TargetEffect6;

	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = SpiritSkill)
	float		TargetPercent6 = 0.0f;

	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = SpiritSkill)
	int32		TargetValue6 = 0;


	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = SpiritSkill)
	ESkillTargetTypeEnum		TargetType = ESkillTargetTypeEnum::VE_None;

	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = SpiritSkill)
	EFriendshipTypeEnum	TargetApply = EFriendshipTypeEnum::VE_None;

	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = SpiritSkill)
	ESearchShapeTypeEnum		EffectShape = ESearchShapeTypeEnum::VE_None;

	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = SpiritSkill)
	TArray<float>				ShapeScope;

	// Areaskill life time
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = SpiritSkill)
	float						SummonTime;

	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = SpiritSkill)
	float StartCoolTime;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = SpiritSkill)
	ESkillShapedIconType	UISkillShape = ESkillShapedIconType::VE_Me;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = SpiritSkill)
	int32					UISkillRange = 0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = SpiritSkill)
	bool					VisibleDecal = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = SpiritSkill)
	bool					VisibleDPAD = false;
};