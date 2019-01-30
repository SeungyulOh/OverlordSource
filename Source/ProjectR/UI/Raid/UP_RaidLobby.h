

#pragma once

#include "UI/RBaseWidget.h"
#include "UP_RaidLobby.generated.h"

class UUC_Item_Icons;

USTRUCT(BlueprintType)
struct PROJECTR_API FVariables_RaidLobby
{
	GENERATED_USTRUCT_BODY()

	// left ui
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Variables")
	TArray<UImage*>				BossImages;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Variables")
	TArray<UButton*>			StageButtons;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Variables")
	TArray<UImage*>				DisableImages;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Variables")
	TArray<UOverlay*>			LockOverlays;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Variables")
	TArray<UImage*>				FocusingImages;

	// 0 : unselect, 1 : select, 2 : lock
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Variables")
	TArray<UWidgetSwitcher*>	DifficultyButtonSwitchers;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Variables")
	TArray<UButton*>			DifficultyButtons;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Variables")
	TArray<UButton*>			DifficultyLockButtons;
	//right ui
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Variables")
	UTextBlock*					ParticipantCountText;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Variables")
	UTextBlock*					RecommendPowerText;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Variables")
	UTextBlock*					AttributeText;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Variables")
	UTextBlock*					LimitMinuteText;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Variables")
	TArray<UUC_Item_Icons*>		RewardItems;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Variables")
	UWidgetSwitcher*			ButtonSwitcher;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Variables")
	UTextBlock*					RemainTryCount;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Variables")
	UButton*					EnterButton;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Variables")
	UButton*					JoinRoomButton;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Variables")
	UButton*					CreateRoomButton;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Variables")
	UButton*					QuickEnterButton;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Variables")
	UTextBlock*					RewardText;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Variables")
		class UUC_Party_Keypad*					UC_party_keypad;

};

UCLASS()
class PROJECTR_API UUP_RaidLobby: public URBaseWidget
{
	GENERATED_BODY()

public:
	void NativeConstruct() override;
	void NativeDestruct() override;

	void BindDelegate();

public:

	void SetRaidUI();
	void RefreshRaidInfoUI();


	// select boss
	void SelectBoss(int32 InBossIndex);

	UFUNCTION()
	void OnClickSelectBoss0();
	UFUNCTION()
	void OnClickSelectBoss1();
	UFUNCTION()
	void OnClickSelectBoss2();

	// select difficulty
	void SelectDiffiCulty(ERaidDifficultyType InDifficultyType);

	UFUNCTION()
	void OnClickEasy();
	UFUNCTION()
	void OnClickNormal();
	UFUNCTION()
	void OnClickHard();
	UFUNCTION()
	void OnClickVeryHard();

	UFUNCTION()
	void OnClickLockEasy();
	UFUNCTION()
	void OnClickLockNormal();
	UFUNCTION()
	void OnClickLockHard();
	UFUNCTION()
	void OnClickLockVeryHard();

	
	UFUNCTION()
	void OnClickEnter();
	UFUNCTION()
	void OnClickJoinRoom();
	UFUNCTION()
	void OnClickCreateRoom();
	UFUNCTION()
	void OnClickQuickEnter();
	UFUNCTION()
		void OnEnterPartyNumber(FString PartyID);

private:
	int32 ConversionDifficultyTypetoInteger(ERaidDifficultyType InType);
	FText GetDayofWeekText(ERaidDayType InDayType);

public:
// 	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UP_RaidLobby")

	int32					SelectedBossIndex = 0;
	ERaidDifficultyType		SelectedDifficulty = ERaidDifficultyType::EASY;


public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UP_RaidLobby")
	FVariables_RaidLobby	variables;

private:
	FName					BossKey = NAME_None;
};