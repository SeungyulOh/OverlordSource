#pragma once

#include "UI/RBaseWidget.h"
#include "UI/UIDefs.h"
#include "Network/PacketDataStructures.h"
#include "UC_RewardsSplash.generated.h"


// Tentative
UENUM(BlueprintType)
enum class EItemInventoryRewardOrigin : uint8
{
	Grind = 0,
	Sell,
	RuneCombine,

	Nonspecific,

	Max,
};

/**
* GenericSplash content of UP_RewardsSplash
*/
UCLASS()
class PROJECTR_API UUC_RewardsSplash : public URBaseWidget
{
	GENERATED_BODY()

public:
	//// Interface

	void RefreshRewards(const TArray<FREWARD>& Rewards, EItemInventoryRewardOrigin RewardOrigin);

	//// BP Widgets
	
	/** (design) 5 of them */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UUC_RewardsSplash|Ref")
	TArray<class UUC_GenericItemIcon*> ItemIcons;

	//UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UUC_RewardsSplash|Ref")
	//class ULocalizingTextBlock* Text_Description;

	/** indexed by (int32)EItemInventoryRewardOrigin */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UUC_RewardsSplash|Ref")
	UWidgetSwitcher* Switcher_Description;
};
