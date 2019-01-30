#pragma once

#include "SharedConstants/GlobalConstants.h"
#include "Character/Bullets/BulletBase.h"
#include "BulletTableInfo.generated.h"

/**
*
*/
USTRUCT(BlueprintType)
struct PROJECTR_API FBulletTableInfo : public FTableRowBase
{
	GENERATED_USTRUCT_BODY()


public:
	
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = Bullet)
	FName					BulletName;

	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = Bullet)
	ESkillBulletTypeEnum	BulletType = ESkillBulletTypeEnum::VE_Bullet;
		

	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = Bullet)
	float					BulletSpeed;


	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = Skill)
	FName					BulletSocket;

	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = Skill)
	FName					BulletHitEffect;
		

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Bullet)
	TAssetSubclassOf< ABulletBase >	BulletClass = nullptr;


	float		GetFloatPropertyValue(FString PropertyName);
	UClass*		GetBulletBlueprintClass(bool forObject = false);
	void		ResetLoadedAssetPtr();
};
