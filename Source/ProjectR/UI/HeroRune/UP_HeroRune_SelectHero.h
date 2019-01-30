#pragma once

#include "UI/RBaseWidget.h"
#include "UP_HeroRune_SelectHero.generated.h"

class ULocalizingTextBlock;
class URScrollView;

UCLASS()
class PROJECTR_API UUP_HeroRune_SelectHero : public URBaseWidget
{
	GENERATED_BODY()

public:
	virtual void NativeConstruct() override;
	virtual void NativeDestruct() override;

	UFUNCTION()
	void Init();
	
	UFUNCTION()
	void OnClickChallengeButton();
	
	UFUNCTION()
	void OnClickChargeButton();

	UFUNCTION()
	void OnSelectItem(URScrollItem* selectItem);

	UFUNCTION()
	void CountResetRp(bool bResult);

	UFUNCTION()
	void CountResetStatusRp(int32 nResetCount);

	UFUNCTION()
	void OnContentsResetButtonOK();

	FName GetHeroIDbyEpisodeID(FName& episodeId);

	void InvalidateScrollData();
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UP_HeroRune_SelectHero)
	UImage*							AvatarImage = nullptr;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UP_HeroRune_SelectHero)
	ULocalizingTextBlock*			HeroName = nullptr;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UP_HeroRune_SelectHero)
	ULocalizingTextBlock*			HeroNick = nullptr;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UP_HeroRune_SelectHero)
	UTextBlock*						RuneCount = nullptr;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UP_HeroRune_SelectHero)
	UTextBlock*						RemainCount = nullptr;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UP_HeroRune_SelectHero)
	UTextBlock*						MaxCount = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UP_HeroRune_SelectHero)
	UWidgetSwitcher*				MyPlaySwitcher = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UP_HeroRune_SelectHero)
	UWidgetSwitcher*				ChallengeButtonSwitcher = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UP_HeroRune_SelectHero)
	UButton*						ChallengeButton = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UP_HeroRune_SelectHero)
	UButton*						ChargeButton = nullptr;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UP_HeroRune_SelectHero)
	URScrollView*					HeroSelectScrollView = nullptr;

private:
	//bool							IsCheckEvent = false;
	//int32							EventItemIndex = -1;
};