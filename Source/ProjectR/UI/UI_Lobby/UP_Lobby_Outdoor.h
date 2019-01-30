// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "UI/RBaseWidget.h"
#include "UI/UIDefs.h"
#include "Network/PacketDataStructures.h"
#include "UC_LobbyInvite_Popup.h"
#include "UP_Lobby_Outdoor.generated.h"

#define CANTREWARD 0
#define CANREWARD 1
#define ALREADYREWARD 2

/**
 * 
 */
USTRUCT(BlueprintType)
struct PROJECTR_API FVariables_Outdoor
{
	GENERATED_USTRUCT_BODY()

public:
	//daily reward widget
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = FVariables_Outdoor) ULocalizingTextBlock* CurrentScore;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = FVariables_Outdoor) TArray<class UUC_Item_Icons*> Item_Icons;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = FVariables_Outdoor) TArray<UOverlay*> AlreadyGet_Overlays;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = FVariables_Outdoor) TArray<UImage*> CanGet_Images;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = FVariables_Outdoor) TArray<UButton*> RewardButtons;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = FVariables_Outdoor) UButton* Button_GotoDailyMission;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = FVariables_Outdoor) UOverlay* Overlay_ArenaLock;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = FVariables_Outdoor) ULocalizingTextBlock* Text_ArenaLock;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = FVariables_Outdoor) UOverlay* Overlay_ColosseumLock;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = FVariables_Outdoor) ULocalizingTextBlock* Text_ColosseumLock;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = FVariables_Outdoor) UOverlay* Overlay_ScenarioNormalLock;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = FVariables_Outdoor) ULocalizingTextBlock* Text_ScenarioNormalLock;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = FVariables_Outdoor) UOverlay* Overlay_ScenarioEliteLock;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = FVariables_Outdoor) ULocalizingTextBlock* Text_ScenarioEliteLock;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = FVariables_Outdoor) UOverlay* Overlay_SingleRaidLock;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = FVariables_Outdoor) ULocalizingTextBlock* Text_SingleRaidLock;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = FVariables_Outdoor) UOverlay* Overlay_WorldBossLock;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = FVariables_Outdoor) ULocalizingTextBlock* Text_WorldBossLock;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = FVariables_Outdoor) UOverlay* Overlay_MirrorDungeonLock;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = FVariables_Outdoor) ULocalizingTextBlock* Text_MirrorDungeonLock;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = FVariables_Outdoor) UOverlay* Overlay_WeekDungeonLock;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = FVariables_Outdoor) ULocalizingTextBlock* Text_WeekDungeonLock;
};

UCLASS()
class PROJECTR_API UUP_Lobby_Outdoor : public URBaseWidget
{
	GENERATED_BODY()	
	
public:
	
	void NativeConstruct() override;
	void NativeDestruct() override;
	void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;
	
	//// Delegates
	//overlay buttons
	UFUNCTION()	void OnButtonPVPClicked();
	UFUNCTION()	void OnButtonCampaignClicked();
	UFUNCTION()	void OnButtonBattleFieldClicked();
	UFUNCTION()	void OnButtonPVEClicked();
	UFUNCTION()	void OnButtonDungeonClicked();
	//

	//image buttons
	UFUNCTION()	void OnButtonArenaClicked();
	UFUNCTION()	void OnButtonColosseumClicked();
	UFUNCTION()	void OnButtonNormalStageClicked();
	UFUNCTION()	void OnButtoEliteStageClicked();
	UFUNCTION()	void OnButtonMirrorDungeonClicked();
	UFUNCTION()	void OnButtonWeekDungeonClicked();
	UFUNCTION()	void OnButtonRaidClicked();
	UFUNCTION()	void OnButtonWorldBossClicked();
	//
	UFUNCTION() void OnButtonDailyMissionClicked();

	//
	UFUNCTION()	void ChatMinimalizeOnOff(bool isOn);
	UFUNCTION()	void UpdateMessage(FString nick, FString message);
	UFUNCTION()	void OnClickFloorMissionButton();
	//

	UFUNCTION(BlueprintCallable, Category = UUP_Lobby_Outdoor) void OnClicked_RewardIcon(int32 index);

	//	¾öÀç¿µ Test
	UFUNCTION()	void OnButtonTestClicked();
	//
	UFUNCTION()
	void SetDailyQuest();

	bool m_reverse = false;

	//// BP Widgets
	UPROPERTY() UOverlay* Chat_Minimalize_Overlay;
	UPROPERTY() UUC_LobbyInvite_Popup* Invite_Popup_Outdoor;
	//

	//Overlay which place in OBJ
	UPROPERTY() UOverlay* Btn_PVP_Overlay = nullptr;
	UPROPERTY() UButton* Btn_PVP = nullptr;
	UPROPERTY() UOverlay* Btn_Campaign_Overlay = nullptr;
	UPROPERTY() UButton* Btn_Campaign = nullptr;
	UPROPERTY() UOverlay* Btn_BattleField_Overlay = nullptr;
	UPROPERTY() UButton* Btn_BattleField = nullptr;
	UPROPERTY() UOverlay* Btn_Raid_Overlay = nullptr;
	UPROPERTY()	UButton* Btn_Raid = nullptr;
	UPROPERTY()	UOverlay* Btn_Dungeon_Overlay = nullptr;
	UPROPERTY()	UButton* Btn_Dungeon = nullptr;
	UPROPERTY() UWidgetSwitcher* WidgetSwitcher_UI;
	//

	//Text Block and image in Overlay
	UPROPERTY()	ULocalizingTextBlock* Text_Campaign;
	UPROPERTY()	UImage* ForeImage_Campaign;
	UPROPERTY()	UImage* BackImage_Campaign;
	UPROPERTY()	ULocalizingTextBlock* Text_PVP;
	UPROPERTY()	UImage* ForeImage_PVP;
	UPROPERTY()	UImage* BackImage_PVP;
	UPROPERTY()	ULocalizingTextBlock* Text_BattleField;
	UPROPERTY()	UImage* ForeImage_BattleField;
	UPROPERTY()	UImage* BackImage_BattleField;
	UPROPERTY()	ULocalizingTextBlock* Text_Raid;
	UPROPERTY()	UImage* ForeImage_Raid;
	UPROPERTY()	UImage* BackImage_Raid;
	UPROPERTY()	ULocalizingTextBlock* Text_Dungeon;
	UPROPERTY()	UImage* ForeImage_Dungeon;
	UPROPERTY()	UImage* BackImage_Dungeon;

	//Subwidget of Image Button
	UPROPERTY()	UImage* Image_ContentsSymbol;
	UPROPERTY()	ULocalizingTextBlock* Text_ContentsTitle;
	UPROPERTY()	UButton* Button_Arena;
	UPROPERTY()	UButton* Button_Colosseum;
	UPROPERTY()	UButton* Button_NormalStage;
	UPROPERTY()	UButton* Button_EliteStage;
	UPROPERTY()	UButton* Button_Raid;
	UPROPERTY()	UButton* Button_WorldBoss;
	UPROPERTY()	UButton* Button_MirrorDungeon;
	UPROPERTY()	UButton* Button_WeekDungeon;
	UPROPERTY() ULocalizingTextBlock* Text_NormalFloor;
	UPROPERTY() ULocalizingTextBlock* Text_NormalStage;
	UPROPERTY() ULocalizingTextBlock* Text_EliteFloor;
	UPROPERTY() ULocalizingTextBlock* Text_EliteStage;
	//

	//Subwidget of Chatwidget
	UPROPERTY()	UWidgetSwitcher* WidgetSwitcher_LineLength = nullptr;
	UPROPERTY()	ULocalizingTextBlock* Text_NickName = nullptr;
	UPROPERTY()	ULocalizingTextBlock* Text_Message = nullptr;
	UPROPERTY()	ULocalizingTextBlock* Text_NickNameLong = nullptr;
	UPROPERTY()	ULocalizingTextBlock* Text_MessageLong_1 = nullptr;
	UPROPERTY()	ULocalizingTextBlock* Text_MessageLong_2 = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UUP_Lobby_Outdoor)
		FVariables_Outdoor variables;
	
private:
	//function for set ui
	void DoorObjInit();
	void SetFloorInfo();
	void SetButtonLocationWithMapObj();
	void SetOffset(AActor* actor, UOverlay* overlay);
	void SetMinimalizeOverlay();
	void SetButtonColorPush(int32 index);
	void SetButtonColorRelease(int32 index);
	void SetCampaignProgress();
	void ContentsLayoutsettings();

	class ALobbyPlayerController * PlayerController;
	AActor* LobbyOutDoorPVPObj;
	AEmitter* LobbyOutDoorBFObj;
	AActor* LobbyOutDoorDungeonObj;
	AActor* LobbyOutDoorCampaignObj;
	AActor* LobbyOutDoorRaidObj;

	bool bGetAllDoorFinished = false;
	bool bSetFirstBtnLocationFinished = true;
	uint8 iCurrentPush = 0;
};