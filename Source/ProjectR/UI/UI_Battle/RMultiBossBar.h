

#pragma once

#include "UI/RBaseWidget.h"
#include "RMultiBossBar.generated.h"


// class ULocalizingTextBlock;
class UProgressBar_Phase;
// class URBattleTimer;
// class URBuffIcon;
// class UISkill_V2;
class UBaseMonsterIcon;
/**
*
*/
UCLASS()
class PROJECTR_API URMultiBossBar : public URBaseWidget
{
	GENERATED_BODY()

public:
	void	NativeConstruct() override;
	void	NativeDestruct() override;
	void	NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;

	UFUNCTION(BlueprintCallable, Category = RMultiBossBar)
	void	MultiBossSpawnEvent(TArray<AActor*> InCharacterList);

	UFUNCTION(BlueprintCallable, Category = RMultiBossBar)
	void	MultiBossHideEvent();

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = RMultiBossBar)
	ULocalizingTextBlock*	LocalText_BossName = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = RMultiBossBar)
	UTextBlock*				TextBlock_BossLevel = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = RMultiBossBar)
	UProgressBar_Phase*		ProgressBar_Base = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = RMultiBossBar)
	UProgressBar_Phase*		ProgressBar_Deco = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = RMultiBossBar)
	UTextBlock*				Text_BossHPCount = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = RMultiBossBar)
	UWidgetAnimation*		BossGaugeUp_Ref = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = RMultiBossBar)
	UWidgetAnimation*		BossGaugeDown_Ref = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = RMultiBossBar)
	TArray<UBaseMonsterIcon*>		BossPortrait;
protected:
	void SynchronizeBossData();


private:
	UPROPERTY()
	TArray<AActor*>			TargetCharacterList;

	UPROPERTY()
	int32					CheckPhaseValue = -1;

	bool					bBossTypeTarget = false;
	ECharacterCategoryTypeEnum	eTargetCategory = ECharacterCategoryTypeEnum::VE_None;

};