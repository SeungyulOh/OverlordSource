#pragma once

#include "SharedConstants/GlobalConstants.h"
#include "UI/UIDefs.h"
#include "RewardPreviewTableInfo.generated.h"

/**
*
*/
USTRUCT(BlueprintType)
struct PROJECTR_API FRewardPreviewTableInfo : public FTableRowBase
{
	GENERATED_USTRUCT_BODY()

public:

	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = RewardPreviewTableInfo)
	FName			CodeName1;

	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = RewardPreviewTableInfo)
	int32			min_amount1;

	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = RewardPreviewTableInfo)
		int32			max_amount1;



	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = RewardPreviewTableInfo)
		FName			CodeName2;

	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = RewardPreviewTableInfo)
		int32			min_amount2;

	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = RewardPreviewTableInfo)
		int32			max_amount2;

	

	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = RewardPreviewTableInfo)
		FName			CodeName3;

	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = RewardPreviewTableInfo)
		int32			min_amount3;

	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = RewardPreviewTableInfo)
		int32			max_amount3;



	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = RewardPreviewTableInfo)
		FName			CodeName4;

	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = RewardPreviewTableInfo)
		int32			min_amount4;

	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = RewardPreviewTableInfo)
		int32			max_amount4;



	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = RewardPreviewTableInfo)
		FName			CodeName5;

	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = RewardPreviewTableInfo)
		int32			min_amount5;

	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = RewardPreviewTableInfo)
		int32			max_amount5;



	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = RewardPreviewTableInfo)
		FName			CodeName6;

	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = RewardPreviewTableInfo)
		int32			min_amount6;

	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = RewardPreviewTableInfo)
		int32			max_amount6;



	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = RewardPreviewTableInfo)
		FName			CodeName7;

	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = RewardPreviewTableInfo)
		int32			min_amount7;

	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = RewardPreviewTableInfo)
		int32			max_amount7;



	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = RewardPreviewTableInfo)
		FName			CodeName8;

	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = RewardPreviewTableInfo)
		int32			min_amount8;

	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = RewardPreviewTableInfo)
		int32			max_amount8;



	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = RewardPreviewTableInfo)
		FName			CodeName9;

	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = RewardPreviewTableInfo)
		int32			min_amount9;

	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = RewardPreviewTableInfo)
		int32			max_amount9;



	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = RewardPreviewTableInfo)
		FName			CodeName10;

	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = RewardPreviewTableInfo)
		int32			min_amount10;

	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = RewardPreviewTableInfo)
		int32			max_amount10;




	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = RewardPreviewTableInfo)
		FName			CodeName11;

	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = RewardPreviewTableInfo)
		int32			min_amount11;

	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = RewardPreviewTableInfo)
		int32			max_amount11;



	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = RewardPreviewTableInfo)
		FName			CodeName12;

	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = RewardPreviewTableInfo)
		int32			min_amount12;

	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = RewardPreviewTableInfo)
		int32			max_amount12;

/* //!RULE_TODOMEMORY
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = RewardPreviewTableInfo)
		FName			CodeName13;

	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = RewardPreviewTableInfo)
		int32			min_amount13;

	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = RewardPreviewTableInfo)
		int32			max_amount13;



	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = RewardPreviewTableInfo)
		FName			CodeName14;

	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = RewardPreviewTableInfo)
		int32			min_amount14;

	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = RewardPreviewTableInfo)
		int32			max_amount14;



	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = RewardPreviewTableInfo)
		FName			CodeName15;

	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = RewardPreviewTableInfo)
		int32			min_amount15;

	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = RewardPreviewTableInfo)
		int32			max_amount15;



	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = RewardPreviewTableInfo)
		FName			CodeName16;

	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = RewardPreviewTableInfo)
		int32			min_amount16;

	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = RewardPreviewTableInfo)
		int32			max_amount16;



	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = RewardPreviewTableInfo)
		FName			CodeName17;

	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = RewardPreviewTableInfo)
		int32			min_amount17;

	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = RewardPreviewTableInfo)
		int32			max_amount17;



	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = RewardPreviewTableInfo)
		FName			CodeName18;

	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = RewardPreviewTableInfo)
		int32			min_amount18;

	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = RewardPreviewTableInfo)
		int32			max_amount18;



	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = RewardPreviewTableInfo)
		FName			CodeName19;

	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = RewardPreviewTableInfo)
		int32			min_amount19;

	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = RewardPreviewTableInfo)
		int32			max_amount19;



	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = RewardPreviewTableInfo)
		FName			CodeName20;

	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = RewardPreviewTableInfo)
		int32			min_amount20;

	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = RewardPreviewTableInfo)
		int32			max_amount20;

*/

};
