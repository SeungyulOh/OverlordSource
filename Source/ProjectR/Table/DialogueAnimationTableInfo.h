// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include "SharedConstants/GlobalConstants.h"
#include "DialogueAnimationTableInfo.generated.h"

/**
 * 
 */
USTRUCT(BlueprintType)
struct PROJECTR_API FDialogueAnimationTableInfo : public FTableRowBase
{
	GENERATED_USTRUCT_BODY()
	
public:
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = DialogueAnimationTableInfo)
	TAssetPtr< USkeletalMesh >		SkeletalMesh;

	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = DialogueAnimationTableInfo)
	TAssetPtr< USkeletalMesh >		DecoMesh;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = DialogueAnimationTableInfo)
	TAssetPtr< UStaticMesh >		LWeapon;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = DialogueAnimationTableInfo)
	float							LWeaponScale = 1.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = DialogueAnimationTableInfo)
	TAssetPtr< UStaticMesh >		RWeapon;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = DialogueAnimationTableInfo)
	float							RWeaponScale = 1.0f;

	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = DialogueAnimationTableInfo)
	TAssetPtr< UMaterialInstance >	MeshMaterial;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = DialogueAnimationTableInfo)
	FTransform						LeftPos;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = DialogueAnimationTableInfo)
	FTransform						RightPos;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = DialogueAnimationTableInfo)
	TAssetPtr< UAnimationAsset >	Ani_Idle;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = DialogueAnimationTableInfo)
	TAssetPtr< UAnimationAsset >	Ani_LobbyIdle;

	
};