#pragma once


#include "UI/Control/RScrollItem.h"
#include "UI/Control/RScrollItemData.h"



// #include "Network/PacketDataStructures.h"
#include "UC_MissionScrollItem.generated.h"

struct FDailyMissionTableInfo;
// class UUP_QuestGuide;
// class UUC_Item_Icons;
/**
*
*/


UCLASS()
class PROJECTR_API UUC_MissionScrollItem : public URBaseWidget
{
	GENERATED_BODY()

public:
	void NativeConstruct() override;
	void InvalidateData() override;

	void SetMissionText(FName InTitleKey, FName InDescKey);
	void SetProgressState(FFLOOR_MISSION missionStatus);
	void SetRewardIcon(FName InCurrencyType, int32 InCurrencyValue);

	UFUNCTION()
	void OnDirectLinkButtonClick();
	UFUNCTION()
	void OnRequestRewardButtonClick();
	
public:
	//property_widget
	UPROPERTY()
		UImage* Image_Category;

	UPROPERTY()
	ULocalizingTextBlock*				MissionNameText = nullptr;

	UPROPERTY()
	ULocalizingTextBlock*				MissionDescText = nullptr;

	UPROPERTY()
	UCanvasPanel*						ProgressBarPanel = nullptr;

	UPROPERTY()
	class UProgressBar*					MissionProgressBar = nullptr;

	UPROPERTY()
	UTextBlock*							CurrentProgressText = nullptr;

	UPROPERTY()
	UTextBlock*							MaxProgressText = nullptr;

	UPROPERTY()
	class UUC_MaterialIcon*				RewardIcon = nullptr;

	UPROPERTY()
	UButton*							DirectLinkButton = nullptr;

	UPROPERTY()
	UButton*							RequestRewardButton = nullptr;

	UPROPERTY()
		ULocalizingTextBlock* ButtonText;

	//property_data
	UPROPERTY()
		FFLOOR_MISSION missionData;
};