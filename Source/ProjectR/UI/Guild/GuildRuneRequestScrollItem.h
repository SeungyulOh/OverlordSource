#pragma once
#include "UI/RBaseWidget.h"
#include "UI/Control/RScrollItem.h"
#include "GuildRuneRequestScrollItem.generated.h"

class UHeroBaseScrollItem;
class UTextBlock;
class URScrollItemData;
class URScrollView;

/**
*
*/
UCLASS()
class PROJECTR_API UUC_GuildRuneRequestScrollItem : public URScrollItem
{
	GENERATED_BODY()

public:
	void NativeConstruct() override;
	void NativeDestruct() override;
	void SetScrollItemData(int32 Index, URScrollItemData* item, URScrollView* ScrollView) override;
	void SetSelectedHero(bool bSelected);
	

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = GuildRuneRequestScrollItem)
	UHeroBaseScrollItem*			HeroScrollIcon = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = GuildRuneRequestScrollItem)
	UTextBlock*						RuneCount = nullptr;
};