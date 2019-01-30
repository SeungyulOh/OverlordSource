// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "UI/RBaseWidget.h"
#include "UI/UIDefs.h"
#include "UI/Control/RScrollItem.h"
#include "UI/Control/RScrollItemData.h"
#include "SharedConstants/GlobalConstants.h"
#include "Network/PacketDataStructures.h"
#include "UI/Component/CanvasPanel_RadioGroup.h"
#include "UP_Quest.generated.h"

class UCanvasPanel;
class USizeBox;
class URScrollView;
class UUC_RewardIcon;
class UHorizontalBox;
class UUC_Item_Icons;
class UCheckBox_Radio;
class UUC_Notify;
class UUC_DailyContentsList;

USTRUCT(BlueprintType)
struct PROJECTR_API FTwoDepth_SizeBoxData
{
	GENERATED_USTRUCT_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UUP_Quest)
	USizeBox* Box = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UUP_Quest)
	float Height;
};

UENUM(BlueprintType)
enum class EQuestTabType : uint8
{
	DailyQuest = 0,
	ImportantMissionTab = 1,
	AchieveTab = 2,
};

UCLASS()
class PROJECTR_API UFloorScrollItem : public URScrollItem
{
	GENERATED_BODY()

public:
	void NativeConstruct() override;
	void NativeDestruct() override;

	UFUNCTION(BlueprintCallable, Category = ScrollItem)
	void				SetScrollItemData(int32 Index, URScrollItemData* item, URScrollView* ScrollView) override;

	UFUNCTION()
	virtual void		SetSelected(bool bSelected);

	UPROPERTY()
	UWidgetSwitcher*		SelectStateSwitcher = nullptr;

	UPROPERTY()
	ULocalizingTextBlock*	ActiveText = nullptr;
	
	UPROPERTY()
	ULocalizingTextBlock*	InActiveText = nullptr;
};

UCLASS()
class PROJECTR_API UFloorScrollItemData : public URScrollItemData
{
	GENERATED_BODY()
public:
	UPROPERTY()
	int32					Floor = 0;
};

UCLASS()
class PROJECTR_API UMissionScrollItem : public URScrollItem
{
	GENERATED_BODY()

public:
	void NativeConstruct() override;
	void NativeDestruct() override;
	void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;
	UFUNCTION(BlueprintCallable, Category = ScrollItem)
	void				SetScrollItemData(int32 Index, URScrollItemData* item, URScrollView* ScrollView) override;

	UFUNCTION()
	virtual void		SetSelected(bool bSelected);

	void SetStateSwitcher(int32 CurrentProgress, int32 itemkey);

public:
	UFUNCTION()
	void			OnClickGotoTarget();
public:
	UPROPERTY()
	ULocalizingTextBlock*		TitleText = nullptr;

	UPROPERTY()
	URRichTextBlock*			DocumentText = nullptr;
	
	UPROPERTY()
	URRichTextBlock*			TargetRichText = nullptr;

	UPROPERTY()
	UOverlay*					CountOverlay = nullptr;

	UPROPERTY()
	ULocalizingTextBlock*		CurrentCountText = nullptr;

	UPROPERTY()
	ULocalizingTextBlock*		MaxCountText = nullptr;

	UPROPERTY()
	UWidgetSwitcher*			MissionStateSwitcher = nullptr;

	UPROPERTY()
	UButton*					GotoTargetButton = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = MissionScrollItem)	
	TArray<UHorizontalBox*>		RewardBoxList;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = MissionScrollItem)
	TArray<UUC_Item_Icons*>		RewardItemList;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = MissionScrollItem)
	float						ItemBigSize;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = MissionScrollItem)
	float						ItemSmallSize;
private:
	FLinearColor				ActiveColor_Blue = FLinearColor(0.049707f, 0.679543f, 1.0f);
	FLinearColor				ProgressColor_Blue = FLinearColor(0.049707f, 0.679543f, 1.0f);
	FLinearColor				OtherColor_Gray = FLinearColor(0.502887f, 0.502887f, 0.502887f);

	bool						bAniPlaying = false;
	bool						bReverse = false;
	float						fCurrentDeltaTime = 0.0f;
};

UCLASS()
class PROJECTR_API UMissionScrollItemData : public URScrollItemData
{
	GENERATED_BODY()
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = MissionScrollItemData)
	int32		nCurrentFloor;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = MissionScrollItemData)
	int32		FloorQuestKey_int;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = MissionScrollItemData)
	int32		ConditionGoal;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = MissionScrollItemData)
	FName		TitleKey;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = MissionScrollItemData)
	FName		DocumentKey;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = MissionScrollItemData)
	FName		TargetStringKey;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = MissionScrollItemData)
	TArray<FITEM_ICON_INFO> RewardItemIconInfo;

};

/**
 * 
 */
UCLASS()
class PROJECTR_API UUP_Quest : public URBaseWidget
{
	GENERATED_BODY()
	
public:

	void NativeConstruct() override;
	void NativeDestruct() override;
	void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;


public:
	void PrepareWidget();

	UFUNCTION()
	void InvalidateData_Achievement();

	void InvalidateMissionScrollView();
	void InvalidateFloorScrollView();
	void SelectActiveFloor();

	UFUNCTION()
	void InvalidateData_ImportantMission();

	UFUNCTION()
	void GetRewardRp(const TArray<FREWARD>& InQuestRewards);

	UFUNCTION(BlueprintCallable, Category = Quest)
	void OnClick_CloseQuest();

	UFUNCTION(BlueprintCallable, Category = Quest)
	void OnClick_Reward();

	UFUNCTION(BlueprintCallable, Category = Quest)
	void OnClick_RewardIcon(int32 InIndex);

	UFUNCTION(BlueprintCallable, Category = Quest)
	void OnClick_CloseNotify();

	UFUNCTION(BlueprintCallable, Category = Quest)
	void OnClick_DetailMissionCategory(EMissionCategoryType InMissionCategory);
	
	UFUNCTION(BlueprintCallable, Category = Quest)
	void OnClick_Tab(EQuestTabType eTabType);

	UFUNCTION()
	void OnFloorSelect(int32 InFloor);
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Quest)
	URScrollView* BP_ScrollView_Quest = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Quest)
	UCanvasPanel*	CanvasPanel_AllPassQuest = nullptr;

	EMissionCategoryType	DetailMissionCategoryType = EMissionCategoryType::VE_None;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Quest)
	UCheckBox_Radio*			CheckboxAchievement = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Quest)
	TArray<UCheckBox_Radio*>	CheckboxRadio_Category;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Quest)
	TArray<FTwoDepth_SizeBoxData>		TwoDepth_SizeBoxDatas;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Quest)
	TArray<UWidgetSwitcher*>	DetailMissionCategory;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Quest)
	TArray<UUC_Notify*>	MainCategory_Notify;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Quest)
	TArray<UUC_Notify*>	DetailCategory_Notify;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Quest)
	UWidgetSwitcher*					Page_Switcher = nullptr;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Quest)
	UTextBlock*							Current_Score = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Quest)
	UTextBlock*							Remain_Hour = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Quest)
	UTextBlock*							Remain_Minute = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Quest)
	bool								IsInitAchievement = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Quest)
	UCanvasPanel*						DailyQuestRoot = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Quest)
	UCanvasPanel*						AchivementRoot = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Quest)
	UWidgetSwitcher*					QuestTypeSwitcher = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Quest)
		UCanvasPanel_RadioGroup*		Tab_Quest = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Quest)
	URScrollView*						FloorScrollView = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Quest)
	URScrollView*						MissionScrollView = nullptr;

	UPROPERTY()
	TArray<UFloorScrollItemData*>			CachedFloorScrollDataList;
	UPROPERTY()
	TArray<UMissionScrollItemData*>			CachedMissionScrollDataList;

	UPROPERTY()
	TArray<class UUC_QuestList_Data*>				CachedAchivementScrollDataList;

private:
	bool IsValidDatas();
	void RefreshCheckBoxRadio_DetailMissionCategory();

	void SetAchievementScrollItem(const FACHIEVE_STATUS& InAcheive_Status, int32 InDataIndex);
	void SetDetailCategoryNotify();
	
	UPROPERTY()
	TArray<bool> DetailCategory_Notify_ShowingValue;

private:
	int32			nCurrentFloor = 0;


	const int32		ACHIVEMENT_SCROLLITEM_MAX = 7;

	const int32		MISSIONSCROLLITEMMAX = 10;
	const int32		MISSIONSCROLLDATAMAX = 6;

};