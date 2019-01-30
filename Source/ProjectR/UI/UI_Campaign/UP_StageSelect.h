#pragma once

#include "UI/RBaseWidget.h"
#include "UP_StageSelect.generated.h"


class UCampaignStageButton;
class URenewal_BaseHeroIcon;
class UTowerFloorMapPopup;
USTRUCT(BlueprintType)
struct PROJECTR_API FVariables_StageSelect
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Variables")
	UTextBlock*				CurrentFloorText = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Variables")
	UTextBlock*				FloorTitleText = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Variables")
	UButton*				FloorSelectButton = nullptr;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Variables")
	UButton*				DownFloorButton = nullptr;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Variables")
	UButton*				UpFloorButton = nullptr;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Variables")
	UButton*				GotoMissionContentsButton = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Variables")
	URRichTextBlock*		ImportantMissionTargetText = nullptr;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Variables")
	ULocalizingTextBlock*	ImportantCurrentText = nullptr;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Variables")
	ULocalizingTextBlock*	ImportantMaxText = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Variables")
	UWidgetSwitcher*		ModeSwitcher = nullptr; // normal, hard

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Variables")
	UImage*					TitleBGImage = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Variables")
	TArray<UCampaignStageButton*>		StageButtonList;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Variables")
	UTextBlock*				CurrentStarText = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Variables")
	UProgressBar*			RewardProgressBar = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Variables")
	UButton*				RewardReceiveButton = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Variables")
	UImage*					RewardImage1 = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Variables")
	UImage*					RewardImage2 = nullptr;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Variables")
	URenewal_BaseHeroIcon*	RewardBonus = nullptr;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Variables")
	UTowerFloorMapPopup*	FloorSelectPopup = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Variables")
	USizeBox*				ClearImage = nullptr;
};


UCLASS()
class PROJECTR_API UUP_StageSelect : public URBaseWidget
{
	GENERATED_BODY()

public:
	void NativeConstruct() override;
	void NativeDestruct() override;


	void SetStageUI();

	UFUNCTION()
	void OnClickGotoMissionContents();

	UFUNCTION()
	void OnClickDownStairButton();
	UFUNCTION()
	void OnClickUpStairButton();

	UFUNCTION()
	void OnClickRewardReceiveButton();

	UFUNCTION()
	void OnClickFloorSelectButton();

	UFUNCTION()
	void OnChangeFloorbyShortCut(int32 InFloor);
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UUP_StageSelect")
	FVariables_StageSelect		variables;

// 	일반컬러 : R:57  G : 134  B : 238  #3986ee
// 	정예컬러: R:86  G : 13   B : 16   #560d10
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UUP_StageSelect")	// set UMG
	FLinearColor				ColorNormal = FLinearColor::FromSRGBColor(FColor(57, 134, 238));

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UUP_StageSelect")	// set UMG
	FLinearColor				ColorHard = FLinearColor::FromSRGBColor(FColor(86, 13, 16));

private:
	void BindDelegates();

	void SetTitleText();
	void SetDifficulty();
	void SetStageButtons();
	void SetRewardInfo();
	void SetImportantMissionInfo();

private:
	EStageDifficultyEnum		StageDifficulty = EStageDifficultyEnum::Normal;
	int32						nFloorRewardIndex = -1;
};
