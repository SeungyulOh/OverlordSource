#pragma once

#include "SharedConstants/GlobalConstants.h"
#include "Character/Bullets/BulletBase.h"
#include "SkillResourceTableInfo.generated.h"

/**
*
*/
USTRUCT(BlueprintType)
struct PROJECTR_API FSkillResourceTableInfo : public FTableRowBase
{
	GENERATED_USTRUCT_BODY()


public:

	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = Skill)
	ESkillAniTypeEnum		SkillAniType = ESkillAniTypeEnum::VE_None;

	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = Skill)
	TAssetPtr< UParticleSystem >			CastingEffect = nullptr;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = Skill)
	FName					CastingEffectSocket;

	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = Skill)
	TAssetPtr< UParticleSystem >			CastingBeamEffect = nullptr;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = Skill)
	FName					CastingBeamSocket;

	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = Skill)
	TAssetPtr< UParticleSystem >		ActionEffect = nullptr;

	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = Skill)
	FName					ActionEffectSocket;

	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = Skill)
	ESkillStateEnum			ActionEffectTiming = ESkillStateEnum::VE_None;

	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = Skill)
	TAssetPtr< UParticleSystem >	AreaEffect = nullptr;

	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = Skill)
	FName					BulletID;
	
// 	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = Skill)
// 	TAssetSubclassOf< ABulletBase >	BulletClass = nullptr;
// 
// 	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = Skill)
// 	FName					BulletSocket;
// 
// 	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = Skill)
// 	FName					BulletHitEffect;
// 	
// 	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = Skill)
// 	FName					BulletHitEffectSocket;

	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = Skill)
	FName					HitEffect;

	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = Skill)
	FName					HitEffectSocket;

	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = Skill)
	bool					BeamTypeHitEffect = false;

	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = Skill)
	FName					BeamCasterSocket;
	
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = Skill)
	TAssetPtr< UTexture2D >				SkillIconRef = nullptr;

	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = Skill)
	TAssetPtr< UTexture2D >				SkillPotrait = nullptr;

	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = Skill)
	float					UltimateSkillSlowTime = 0.0f;

	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = Skill)
	float					UltimateSkillSlowRate = 0.0f;

	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = Skill)
	float					UltimatePostprocessTime = 0.0f;

	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = Skill)
	TAssetPtr< UParticleSystem >		ScreenEffect = nullptr;

	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = Skill)
	ESkillStateEnum			ScreenEffectTiming = ESkillStateEnum::VE_None;

	//UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = Skill)
	//ESkillIconColorEnum		SkillIconColor;

	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = Skill)
	ESkillIconShapeEnum		SkillIconShape;

	UClass*		GetBulletBlueprintClass();
};
