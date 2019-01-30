#pragma once

#include "UI/RBaseWidget.h"
#include "UP_MirrorDungeonResult.generated.h"


// class UUC_ResultStatistics;

class UUC_ResultEffect;
class URScrollView;
class URenewal_BaseHeroIcon;
class UUC_Item_Icons;
/**
*
*/
UCLASS()
class PROJECTR_API UUP_MirrorDungeonResult : public URBaseWidget
{
	GENERATED_BODY()

public:
	void NativeConstruct() override;

	UFUNCTION()
	void		OnMainLobby();
	
	UFUNCTION()
	void		OnRetry();


	void SetResultDisplay();
	void SetHeroHPState();
	void SetRewards();
	void SetTimeText();
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UP_MirrorDungeonResult)
	UUC_ResultEffect*		ResultDisplay = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UP_MirrorDungeonResult)
	UButton*				Button_HeroManager;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UP_MirrorDungeonResult)
	UButton*				Button_GotoMenu;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UP_MirrorDungeonResult)
	UVerticalBox*			RewardBox = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UP_MirrorDungeonResult)
	ULocalizingTextBlock*	Text_ResultTime = nullptr;

	TArray<UUC_Item_Icons*>		ItemIcon;
public:
	UPROPERTY()
	TArray<URenewal_BaseHeroIcon*>			HeroList;


};