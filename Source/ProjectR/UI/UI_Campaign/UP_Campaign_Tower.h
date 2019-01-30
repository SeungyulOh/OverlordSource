// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/RBaseWidget.h"
#include "UI/Control/RScrollItem.h"
#include "UI/Control/RScrollItemData.h"
#include "SharedConstants/GlobalConstants.h"
#include "UP_Campaign_Tower.generated.h"


UCLASS()
class PROJECTR_API UUC_TowerFloorSelect : public URBaseWidget
{
	GENERATED_BODY()

public:
	void NativeConstruct() override;

public:

	UFUNCTION()
	void CallBack_TouchBegin(ETouchIndex::Type TouchIndex, FVector Location);
	UFUNCTION()
	void CallBack_TouchOver(ETouchIndex::Type TouchIndex, FVector Location);
	UFUNCTION()
	void CallBack_TouchReleased(ETouchIndex::Type TouchIndex, FVector Location);

	int32 Calculate_CurrentFloorIdx();
	void Update_FloorInfo();

public:
	UPROPERTY()
	UButton* Button_ShowScroll;

	UPROPERTY()
	UButton* Button_Drag;

	UPROPERTY()
	UWidgetSwitcher* WidgetSwitcher_LeftRight;

	UPROPERTY()
	UOverlay* Overlay_Floor;

	UPROPERTY()
	UTextBlock*	TextexpFloor;
		
	UPROPERTY()
	TWeakObjectPtr<class UUP_Campaign_Tower> UP_Campaign_Tower;

private:
	bool isShow = false;
	bool isDragState = false;
	float ButtonSizeY = 0.f;
	float Height_1Floor = 0.f;
	float FirstPosY = 0.f;
	float FirstPosX = 0.f;
	float LeftTopPos = 0.f;
	float TargetTextFloorPos = 0.f;

	FVector2D MousePos;
};

UCLASS()
class PROJECTR_API UUC_TowerStageInfo : public URBaseWidget
{
	GENERATED_BODY()

public:
	void NativeConstruct() override;

public:
	UFUNCTION()
	void OnClick_MonsterTab(bool isChecked);

	UFUNCTION()
	void OnClick_ItemTab(bool isChecked);

	/*Create ScrollItem as many as current stage can spawn monsters and scrollitemdata*/
	void Set_ScrollItem_Pair(); // first init only
	void SetSwitcherControlMode(bool isEliteMode);
	void Invalidate_ScrollView();

public:
	UPROPERTY()
	UCheckBox* CheckBox_Monster;

	UPROPERTY()
	UCheckBox* CheckBox_Item;

	UPROPERTY()
	UWidgetSwitcher* WidgetSwitcher_Monster;

	UPROPERTY()
	UWidgetSwitcher* WidgetSwitcher_Item;

	UPROPERTY()
	UTextBlock* Title;
	UPROPERTY()
	UTextBlock* TopText;
	UPROPERTY()
	UTextBlock* BottomText;

	UPROPERTY()
	UWidgetSwitcher* WidgetSwitcher_ControlMode;

	UPROPERTY()
	UWidgetSwitcher* WidgetSwitcher_ControlMode2;

	UPROPERTY()
	class URScrollView*	ScrollView_Monster;

	UPROPERTY()
	class URScrollView*	ScrollView_Item;

	UPROPERTY()
	TWeakObjectPtr<class UUP_Campaign_Tower> UP_Campaign_Tower;

private:
	class URGameInstance* RGameInst;

};

UENUM(BlueprintType)		//"BlueprintType" is essential to include
enum class ETreasureState : uint8
{
	TREASURE_IMPOSSIBLE,
	TREASURE_POSSIBLE,
	TREASURE_ACQUIRED,
};

/**
 * UP_Campaign_Tower
 */
UCLASS()
class PROJECTR_API UUP_Campaign_Tower : public URBaseWidget
{
	GENERATED_BODY()

	friend class UUC_TowerStageInfo;
	friend class UUC_StageButton;
	
public:
	void NativeConstruct() override;

	UFUNCTION()
	void Invalidate();

	void SetTreasureBox();
	UFUNCTION()
	void SetFloorMissionButton();
	UFUNCTION()
		void OnClickFloorMissionButton();
	UFUNCTION()
		void RequestGetFloorMission();

	UFUNCTION()
	void Render_OnFloorChanged(int32 Floor);

	/**/
	void QuestImageOnStageBtnReset();
	void Init_NormalStageButton();
	void Init_EliteStageButton();

	void Toggle_ControlMode(bool isEliteMode);

	UFUNCTION()
	void OnClick_Treasure();

	UFUNCTION()
	void OnClick_HeroCard();

	void SetImportantMissionInfo();

	UFUNCTION()
	void OnClickGotoMissionContents();
	void SetSelectTargetStageButton(int32 InTargetStageIndex);
public:
	UPROPERTY()
	UUC_TowerStageInfo* TowerStageInfo;

	UPROPERTY()
	class UUC_HeroInventory_ScrollItem* BossCard;

	UPROPERTY()
	class UUC_FloorSelector* FloorSelector;

	//floor mission
	UPROPERTY()
		UWidgetSwitcher* WidgetSwitcher_FloorMission;
	UPROPERTY()
		UButton* Button_FloorMission;
	//

	UPROPERTY()
	TArray<class UUC_StageButton*> NormalStageButtonArray;

	UPROPERTY()
	TArray<class UUC_StageButton*> EliteStageButtonArray;

	UPROPERTY()
	UTextBlock* TargetStarCnt;

	UPROPERTY()
	UTextBlock* CurStarCnt;


	UPROPERTY()
	UButton* TreasureButton;

	UPROPERTY()
	UCanvasPanel* CanvasPanel_Message;

	UPROPERTY()
	UTextBlock* FloorTitleText;
	UPROPERTY()
	UTextBlock* Text_PossibleRewardCount;

	UPROPERTY()
	UProgressBar* ProgressBar_Star;

	FORCEINLINE TArray<FName>& GetFloorStageIds() { return FloorStageIDs; }
	FORCEINLINE int32 GetCurrentStageIdx(int32 InCurrentFloor, int32 StageIdx) { return (InCurrentFloor - 1) * 14 + StageIdx; }
	FORCEINLINE void SetCurrentFloor(int32 inFloor) { CurrentFloor = inFloor; }
	FORCEINLINE int32 GetCurrentFloor() { return CurrentFloor; }

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UUP_Campaign_Tower)
	UOverlay*					ImportantMissionOverlay = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UUP_Campaign_Tower)
	URRichTextBlock*			ImportantMissionTargetText = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UUP_Campaign_Tower)
	ULocalizingTextBlock*		ImportantCurrentText = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UUP_Campaign_Tower)
	ULocalizingTextBlock*		ImportantMaxText = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UUP_Campaign_Tower)
	UButton*					GotoMissionContentsButton = nullptr;

private:
	int32 iCurrentStarNum = 0;
	int32 CurrentSelectedStageIdx = 0;
	int32 CurrentFloor = -1;
	EUIPageEnum PrevUIPage;

	UPROPERTY()
	TArray<FName> FloorStageIDs;

	UPROPERTY()
	TArray<ETreasureState> TreasureStateArray;
	
};

