// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include "SharedConstants/GlobalConstants.h"
#include "AbnormalStateGroupTableInfo_V2.generated.h"
/**
 * 
 */
USTRUCT(BlueprintType)
struct PROJECTR_API FAbnormalStateGroupTableInfo_V2 : public FTableRowBase
{
	GENERATED_USTRUCT_BODY()

public:
	
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = AbnormalStateGroupTableInfo_V2)
	EAbnormalSlotTypeEnum	SlotType = EAbnormalSlotTypeEnum::VE_None;

	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = AbnormalStateGroupTableInfo_V2)
	EAbnormalGroupTypeEnum	Group = EAbnormalGroupTypeEnum::VE_None;

	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = AbnormalStateGroupTableInfo_V2)
	EAbnormalApplyTypeEnum	BuffType = EAbnormalApplyTypeEnum::VE_None;
		

	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = AbnormalStateGroupTableInfo_V2)
	TAssetPtr< UParticleSystem >	Effect;

	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = AbnormalStateGroupTableInfo_V2)
	FName					Socket;

	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = AbnormalStateGroupTableInfo_V2)
	TAssetPtr< UMaterial >	EffectMaterial;

	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = AbnormalStateGroupTableInfo_V2)
	FColor					StatusColor;

	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = AbnormalStateGroupTableInfo_V2)
	FName					DescKey;
};
