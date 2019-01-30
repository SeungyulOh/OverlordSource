// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include "Object.h"
#include "SharedConstants/GlobalConstants.h"
#include "UI/RBaseWidget.h"
#include "Table/RaidTableInfo.h"
#include "Network/PacketDataStructures.h"
#include "UI/Control/RScrollItem.h"
#include "UI/Control/RScrollItemData.h"
#include "UP_Raid_World_Boss.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FDelegate_Selected_Difficulty,FName, InDifficulty);

class URScrollView;
/**
 * 
 */
// UCLASS()
// class PROJECTR_API UUC_WorldBossLevelButton : public URBaseWidget
// {
// 	GENERATED_BODY()
// public:
// 	void NativeConstruct() override;
// 	void NativeDestruct() override;
// 	void SetInfo(FName InBossKey);
// 	void SetSelected(bool bSelected);
// 	UFUNCTION()
// 	void OnClick();
// 
// 	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UUC_WorldBossLevelButton)
// 	ULocalizingTextBlock*							BP_Text_Difficulty;
// 	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UUC_WorldBossLevelButton)
// 	UImage*											BP_Image_Stage_Lock;
// 	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UUC_WorldBossLevelButton)
// 	UImage*											BP_Image_Stage_Selected;
// 	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UUC_WorldBossLevelButton)
// 	UImage*											BP_Image_Stage_Difficulty;
// 	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UUC_WorldBossLevelButton)
// 	UButton*										BP_Btn_Stage_Main;
// 	
// 	FName											BossKey;
// 	FName											Difficulty;
// 	FDelegate_Selected_Difficulty					OnClickDifficulty;
// };
// 
// UCLASS()
// class PROJECTR_API UUC_WorldBossLevel : public URBaseWidget
// {
// 	GENERATED_BODY()
// public:
// 	void NativeConstruct() override;
// 	void NativeDestruct() override;
// 	void Init();
// 	void SetLevel(const TMap<FName,FWorldBossStageInfo*>& InWorldBossStageInfo);
// 	void SelectedLevel(FName InDifficulty);
// 	
// 	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UUC_WorldBossLevel)
// 	TArray<UUC_WorldBossLevelButton*>				BP_DifficultyButtons;
// 
// 	FDelegate_Selected_Difficulty					OnClickDifficulty;
// };

UCLASS()
class PROJECTR_API UWorldRaidLevel_ScrollData : public URScrollItemData
{
	GENERATED_BODY()

public:
	FWorldBossStageInfo*	StageInfo = nullptr;
	FName					BossKey;
};

UCLASS()
class PROJECTR_API UWorldRaidLevel_ScrollItem : public URScrollItem
{
	GENERATED_BODY()

public:

	FBossStage*			BossStageInfo;
	FName				BossKey;

	virtual void SetScrollItemData(int32 Index, URScrollItemData* item, URScrollView* ScrollView) override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UUC_WorldBossLevelButton)
	ULocalizingTextBlock*							BP_Text_Difficulty;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UUC_WorldBossLevelButton)
	UImage*											BP_Image_Stage_Lock;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UUC_WorldBossLevelButton)
	UImage*											BP_Image_Stage_Selected;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UUC_WorldBossLevelButton)
	UImage*											BP_Image_Stage_Difficulty;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UUC_WorldBossLevelButton)
	UButton*										BP_Btn_Stage_Main;


	virtual void SetSelected(bool bSelected) override;

};

UCLASS()
class PROJECTR_API UUP_Raid_World_Boss : public URBaseWidget
{
	GENERATED_BODY()
	
public:
	void NativeConstruct() override;
	void NativeDestruct() override;
	void SetRaidCategory(EUIMenuIDEnum InUIMenuIDEnum);
	UFUNCTION()
	void OnGET_RAID_STAGE_ALL_RP();
	UFUNCTION()
	void OnGET_RAID_STAGE_USER_RP(FString RaidStageID);
	UFUNCTION()
	void OnSelectDifficulty(FName InDifficulty);
	UFUNCTION()
	void OnClick_Single_Raid();

	UFUNCTION()
	void OnSelectLevelScrollItem(URScrollItem* selectItem);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UUP_Raid_World_Boss)
	ULocalizingTextBlock*								BP_Text_RemainPlayCount;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UUP_Raid_World_Boss)
	ULocalizingTextBlock*								BP_Text_MaxPlayCount;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UUP_Raid_World_Boss)
	ULocalizingTextBlock*								BP_Text_GameTime;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UUP_Raid_World_Boss)
	ULocalizingTextBlock*								BP_Text_RecommendPower;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UUP_Raid_World_Boss)
	ULocalizingTextBlock*								BP_Text_PlayTime;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UUP_Raid_World_Boss)
	URScrollView*										BP_Scroll_ClearRewardList;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UUP_Raid_World_Boss)
	URScrollView*										BP_WorldBossLevelScroll;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UUP_Raid_World_Boss)
	UButton*											BP_Btn_Enter_Single_Raid;
	UPROPERTY()
	FName												SelectedDifficulty;
	TMap<FName, FWorldBossStageInfo*>					SingleStageInfos;
	TMap<FName, FWorldBossStageInfo*>					MultiStageInfos;
};