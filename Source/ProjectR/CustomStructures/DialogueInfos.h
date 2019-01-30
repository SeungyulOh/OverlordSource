// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

//#include "Object.h"
#include "SharedConstants/GlobalConstants.h"
#include "Engine/DataTable.h"
#include "DialogueInfos.generated.h"

/**
 * 
 */

USTRUCT(BlueprintType)
struct PROJECTR_API FDialogueInfo : public FTableRowBase
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = FDialogueInfo)
	FName Dialogue_ID;

	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = FDialogueInfo)
	FName SpeakerName;

	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = FDialogueInfo)
	FName SpeakerPos;

	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = FDialogueInfo)
	FName SpeakerRes;

	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = FDialogueInfo)
	FName Movie;

	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = FDialogueInfo)
	TAssetPtr< UTexture2D >				BackgroundImg = nullptr;

	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = FDialogueInfo)
	FName Sequence;
};

USTRUCT(BlueprintType)
struct PROJECTR_API FDialogueResource : public FTableRowBase
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = FDialogueResource)
	TSoftObjectPtr< USkeletalMesh > SkeletalMesh;

	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = FDialogueResource)
	TSoftObjectPtr< UAnimationAsset > Animation;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = FDialogueResource)
	TSoftObjectPtr< UStaticMesh >		RWeaponType = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = FDialogueResource)
	TSoftObjectPtr< UStaticMesh >		LWeaponType = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = FDialogueResource)
	TSoftObjectPtr< UMaterialInstance >		Material0 = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = FDialogueResource)
	TSoftObjectPtr< UMaterialInstance >		Material1 = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = FDialogueResource)
	TSoftObjectPtr< UMaterialInstance >		Material2 = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = FDialogueResource)
	TSoftObjectPtr< UMaterialInstance >		Material3 = nullptr;
};
