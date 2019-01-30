#pragma once


#include "UI/Control/RScrollItem.h"
#include "UC_HeroMediaItem.generated.h"

/**
*
*/
UCLASS()
class PROJECTR_API UUC_HeroMediaItem : public URScrollItem
{
	GENERATED_BODY()

public:
	void SetScrollItemData(int32 Index, URScrollItemData* item, URScrollView* ScrollView) override;

	void Refresh();

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UUC_HeroItem)
	UImage*					MediaImage = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UUC_HeroItem)
	UTextBlock*				FrontHeroText = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UUC_HeroItem)
	UTextBlock*				BackHeroText = nullptr;

};