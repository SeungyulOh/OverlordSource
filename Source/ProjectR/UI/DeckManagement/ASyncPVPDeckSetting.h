// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/RBaseWidget.h"
#include "UI/Common/ScrollView/UP_HeroScrollView_Bottom.h"
#include "UP_GenericScrollview.h"
#include "UI/Component/CheckBox_Radio.h"
#include "UI/Arena/UC_Arena_charge_popup.h"
#include "UI/Arena/UC_Arena_DeckBtn.h"
#include "Global/TableManager.h"
#include "Global/RGameInstance.h"
#include "UI/Component/CanvasPanel_RadioGroup.h"
#include "ASyncPVPDeckSetting.generated.h"

/**
 * 
 */

const int32 AUTO_DECK_311_POS_INDEX[] = { 0, 1, 2, 4, 7 };
const int32 AUTO_DECK_212_POS_INDEX[] = { 0, 2, 4, 6, 8 };
const int32 AUTO_DECK_113_POS_INDEX[] = { 1, 4, 6, 7, 8 };

const int32 DAUTO_DECK_311_POS_INDEX[] = { 1, 4, 6, 7, 8 };
const int32 DAUTO_DECK_113_POS_INDEX[] = { 0, 1, 2, 4, 7 };
//

UENUM(BlueprintType)
enum class EPVPDeckType : uint8
{
	PVPARENA, 
	PVPMIRROR,
	END
};

USTRUCT(BlueprintType)
struct PROJECTR_API FVariables_ASyncPVP
{
	GENERATED_USTRUCT_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UASyncPVPDeckSetting)
		UCanvasPanel*					PlayerInfo = nullptr;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UASyncPVPDeckSetting)
		UTextBlock*						Avatar_Level = nullptr;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UASyncPVPDeckSetting)
		UTextBlock*						Avatar_Name = nullptr;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UASyncPVPDeckSetting)
		UTextBlock*						Avatar_Guild = nullptr;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UASyncPVPDeckSetting)
		UTextBlock*						Avatar_Power = nullptr;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UASyncPVPDeckSetting)
		UTextBlock*						Enemy_Level = nullptr;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UASyncPVPDeckSetting)
		UTextBlock*						Enemy_Name = nullptr;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UASyncPVPDeckSetting)
		UTextBlock*						Enemy_Guild = nullptr;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UASyncPVPDeckSetting)
		UTextBlock*						Enemy_Power = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UASyncPVPDeckSetting)
		UCanvasPanel*					defence_play_info = nullptr;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UASyncPVPDeckSetting)
		UTextBlock*						TextBlock_DefencePower = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UASyncPVPDeckSetting)
		UCanvasPanel*					today_number_info = nullptr;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UASyncPVPDeckSetting)
		UTextBlock*						TextBlock_SeasonCount = nullptr;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UASyncPVPDeckSetting)
		UButton*						Button_ArenaCountCharge = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UASyncPVPDeckSetting)
		UOverlay*						renewal_btn = nullptr;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UASyncPVPDeckSetting)
		UTextBlock*						Text_TargetCount = nullptr;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UASyncPVPDeckSetting)
		UTextBlock*						Text_TargetReset = nullptr;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UASyncPVPDeckSetting)
		UTextBlock*						Text_TargetMax = nullptr;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UASyncPVPDeckSetting)
		UButton*						Button_ResetTarget = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UASyncPVPDeckSetting)
		UCanvasPanel*					Progress = nullptr;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UASyncPVPDeckSetting)
		TArray<UImage*>					PickProgressImageArray;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UASyncPVPDeckSetting)
		UTextBlock*						Pick_PlaceCount = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UASyncPVPDeckSetting)
		UWidgetSwitcher*				WidgetSwitcher_Start = nullptr;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UASyncPVPDeckSetting)
		UButton*						SaveButton = nullptr;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UASyncPVPDeckSetting)
		UButton*						EnterButton = nullptr;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UASyncPVPDeckSetting)
		UOverlay*						ArenaEnterCostOverlay = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UASyncPVPDeckSetting)
		UCanvasPanel_RadioGroup*		AutoSelectRadioGroup = nullptr;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UASyncPVPDeckSetting)
		TArray<UCheckBox_Radio*>		CheckBoxArray;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UASyncPVPDeckSetting)
		TArray<UWidgetSwitcher*>		SwitcherArray;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UASyncPVPDeckSetting)
		TArray<UUC_Arena_DeckBtn*>		m_MyHeroDeckBtnList;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UASyncPVPDeckSetting)
		TArray<UUC_Arena_DeckBtn*>		m_EnemyHeroDeckBtnList;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UASyncPVPDeckSetting)
		UUC_Arena_charge_popup*			UC_Arena_charge_popup = nullptr;

	uint8 m_nMyPlacementCount = 0;
	bool bIsArena = true;


	UPROPERTY()
	EPVPDeckType DeckType = EPVPDeckType::END;
};

USTRUCT(BlueprintType)
struct PROJECTR_API FRenderer_ASyncPVP
{
	GENERATED_USTRUCT_BODY()

public:
	void Prepare();
	void Render();

	//
	void DefenceSettingInit();
	void MirrorDungeonInit();
	void ArenaAttackInit();
	void DeckButtonInit(bool isMine);

	//
	void SetAttackDeck();
	void SetDefenceDeck();
	void SetAutoDeckRender(int32 index);
	void SetDeckBtnHP(FMIRROR mirror, FHERO_ICON_INFO& info, FEQUIPED_HERO selectHero, int32 index);

	//
	void SetMyInfo();
	void SetCompetitorInfo();
	void SetCompetitorDeck();
	void SetCompetitorRefreshOverlay();

	//
	void ReleaseAllRadioSwitcher();
	int32 GetListNum(TArray<FString> list);
	void SetPickCountNPowerUI(TArray<FString> herolist);
	
	//
	URGameInstance*			GameInstance;
	UArenaManager*			ArenaManager;
	URWidgetManager*		WidgetManager;
	UUserInfo*				UserInfo;
	UGuildInfo*				GuildInfo;
	UMirrorDungeonManager*	MirrorManager;
	URInventory*			RInventory;
	UTableManager*			TablePtr;

	//
	FVariables_ASyncPVP* variables;
	FDeckManager_Blackboard* Blackboard;
	FUI_StateController* StateController;
};

UCLASS()
class PROJECTR_API UASyncPVPDeckSetting : public URBaseWidget
{
	GENERATED_BODY()
	
public:
	void NativeConstruct() override;
	void NativeDestruct() override;

	UFUNCTION() void CountResetRp(bool bResult);
	UFUNCTION() void CountResetStatusRp(int32 nResetCount);

	//when blackboard state changed, this function will be called.
	UFUNCTION()	void Update_FromBlackboard();

	UFUNCTION() void DefenceSetting();
	UFUNCTION() void StartArenaMatch();
	UFUNCTION() void StartMirrorMatch();
	UFUNCTION() void GoShop();

	UFUNCTION() void OnClick_Auto_311(bool isChecked);
	UFUNCTION() void OnClick_Auto_212(bool isChecked);
	UFUNCTION() void OnClick_Auto_113(bool isChecked);

	UFUNCTION()	void Button_ClickDailyBonusAdd();
	UFUNCTION() void OnClickDefenceReset();
// 	UFUNCTION() void OnClick_Button_ArenaCountCharge();
// 	UFUNCTION() void OnClick_Button_ResetTarget();

	UFUNCTION() void ShowRejectRequestPopupDelay();
	
	void DailyBonusCharge(int32 nResetCount);
	void DefenceChangeCharge(int32 nResetCount);

	void SetRadioSwitcher(int32 idx);

	//

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UASyncPVPDeckSetting)
		FVariables_ASyncPVP variables;
	UPROPERTY()
		FRenderer_ASyncPVP Renderer;

private:
	URGameInstance* GameInstance;
	FTimerHandle DelayTimer;
	int32 CurrencyChargeState;
};
