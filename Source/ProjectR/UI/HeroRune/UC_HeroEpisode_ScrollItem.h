#pragma once


#include "UI/Control/RScrollItem.h"
#include "UC_HeroEpisode_ScrollItem.generated.h"

class UUC_HeroDeck;
/**
*
*/
UCLASS()
class PROJECTR_API UUC_HeroEpisode_ScrollItem : public URScrollItem
{
	GENERATED_BODY()

public:
	void SetScrollItemData(int32 Index, URScrollItemData* item, URScrollView* ScrollView) override;

public:
	UFUNCTION()
	void OnDeckClick(FString ud);

	UFUNCTION()
	void OnContentsResetButtonOK();

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UUC_HeroEpisode_ScrollItem)
	UUC_HeroDeck*			HeroDeckInfo = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UUC_HeroEpisode_ScrollItem)
	UTextBlock*				CurrentCount = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UUC_HeroEpisode_ScrollItem)
	UTextBlock*				MaxCount = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UUC_HeroEpisode_ScrollItem)
	UWidgetSwitcher*		ReadyStateSwitcher = nullptr;

	bool					bNeedCharge = false;
};
