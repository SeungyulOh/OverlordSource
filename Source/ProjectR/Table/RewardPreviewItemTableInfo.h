#pragma once

#include "SharedConstants/GlobalConstants.h"
#include "RewardPreviewItemTableInfo.generated.h"

/**
*
*/
USTRUCT(BlueprintType)
struct PROJECTR_API FRewardPreviewItemTableInfo : public FTableRowBase
{
	GENERATED_USTRUCT_BODY()


public:

	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = ResourceStaticMeshTableInfo)
	FName					NameStringKey;

	//UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = ResourceStaticMeshTableInfo)
	//TAssetPtr< UTexture2D >		ItemIconRef;

	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = ResourceStaticMeshTableInfo)
	FName					ItemIconKey;

	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = ResourceStaticMeshTableInfo)
	EItemGradeEnum			Grade;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = ResourceStaticMeshTableInfo)
	FName					TooltipType;

	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = ResourceStaticMeshTableInfo)
	FName					Tooltip;
};
