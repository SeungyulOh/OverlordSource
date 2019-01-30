#pragma once

#include "UI/RBaseWidget.h"
#include "DailyQuestPage.generated.h"

class UUC_Item_Icons;
class UUC_DailyContentsList;
class UOverlay;

UENUM()
enum class EREWARD_STATE_TYPE : uint8
{
	NotRewardable = 0,
	Rewardable = 1,
	AlreadyRewarded = 2,
};

USTRUCT(BlueprintType)
struct PROJECTR_API FVariables_DailyQuestPage
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Variables")
	UTextBlock*					CurrentScoreText = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Variables")
	UTextBlock*					RemainHourText = nullptr;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Variables")
	UTextBlock*					RemainMinuteText = nullptr;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Variables")
	UTextBlock*					RemainSecondText = nullptr;


	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Variables")
	TArray<UTextBlock*>			ClearCountText;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Variables")
	TArray<UProgressBar*>		ScoreRatioProgressBar;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Variables")
	TArray<UTextBlock*>			DestinationStep_ScoreTextList;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Variables")
	TArray<UUC_Item_Icons*>		DestinationStep_ItemList;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Variables")
	TArray<UOverlay*>			DestinationStep_CompleteImageList;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Variables")
	TArray<UImage*>				DestinationStep_ReceiveableImageList;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Variables")
	TArray<UUC_DailyContentsList*>	ContentsInfoButtonList;
};

/**
*
*/
UCLASS()
class PROJECTR_API UDailyQuestPage : public URBaseWidget
{
	GENERATED_BODY()

public:
	void NativeConstruct() override;
	void NativeTick(const FGeometry& MyGeometry, float InDeltaTime);

	void Init();

	UFUNCTION()
	void Refresh();

	// BP Call function
	UFUNCTION(BlueprintCallable, Category = "DailyQuestPage")
	void OnClickReward(int32 InIndex);
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "DailyQuestPage")
	FVariables_DailyQuestPage		variables;

	int64			nTickCheckTime;
};