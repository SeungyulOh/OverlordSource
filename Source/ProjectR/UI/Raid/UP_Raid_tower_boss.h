// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "UI/RBaseWidget.h"
#include "UI/Control/RScrollView.h"
#include "Table/RaidTableInfo.h"
#include "UP_Raid_tower_boss.generated.h"

class UButton;
class ULocalizingTextBlock;
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FDelegate_OnEnterPartyNumber, FString, InPartyNumber);
UENUM(BlueprintType)
enum ETowerScrollType {
	VE_MONSTERS		UMETA("Monster"),
	VE_ITEMS		UMETA("Item"),
	VE_NONE			UMETA("None"),
};
UCLASS()
class PROJECTR_API UUC_Party_Keypad : public URBaseWidget {
	GENERATED_BODY()
public:
	UPROPERTY(BlueprintAssignable, BlueprintCallable, Category = UUC_Party_Keypad)
	FDelegate_OnEnterPartyNumber			OnEnterPartyNumber;
	void NativeConstruct() override;
	void NativeDestruct() override;
	void Init();
	UFUNCTION()
	void NumberPrinting();
	UFUNCTION()
	void Clear();
	UFUNCTION()
	void OnClickedOK();
	UFUNCTION()
	void OnClickedCancel();

	UFUNCTION()
	void OnClickedNumber0();
	UFUNCTION()
	void OnClickedNumber1();
	UFUNCTION()
	void OnClickedNumber2();
	UFUNCTION()
	void OnClickedNumber3();
	UFUNCTION()
	void OnClickedNumber4();
	UFUNCTION()
	void OnClickedNumber5();
	UFUNCTION()
	void OnClickedNumber6();
	UFUNCTION()
	void OnClickedNumber7();
	UFUNCTION()
	void OnClickedNumber8();
	UFUNCTION()
	void OnClickedNumber9();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UUC_Party_Keypad)
	UButton*						KeyButton0 = nullptr;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UUC_Party_Keypad)
	UButton*						KeyButton1 = nullptr;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UUC_Party_Keypad)
	UButton*						KeyButton2 = nullptr;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UUC_Party_Keypad)
	UButton*						KeyButton3 = nullptr;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UUC_Party_Keypad)
	UButton*						KeyButton4 = nullptr;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UUC_Party_Keypad)
	UButton*						KeyButton5 = nullptr;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UUC_Party_Keypad)
	UButton*						KeyButton6 = nullptr;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UUC_Party_Keypad)
	UButton*						KeyButton7 = nullptr;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UUC_Party_Keypad)
	UButton*						KeyButton8 = nullptr;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UUC_Party_Keypad)
	UButton*						KeyButton9 = nullptr;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UUC_Party_Keypad)
	UButton*						Button_Clear = nullptr;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UUC_Party_Keypad)
	UButton*						Button_OK = nullptr;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UUC_Party_Keypad)
	UButton*						Button_Cancel = nullptr;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UUC_Party_Keypad)
	ULocalizingTextBlock*			Party_Number_Label = nullptr;

	FString		PartyNumberString = TEXT("");
	const int32	NumberLen = 5;
};

/**
 * 
 */
UCLASS()
class PROJECTR_API UUP_Raid_tower_boss : public URBaseWidget
{
	GENERATED_BODY()
	
public:
	void NativeConstruct() override;
	void NativeDestruct() override;

	void Init();
	void SetSingleText();
	void SetMultiText();
	void SetSingleScroll();
	void SetMultiScroll();
	void SetBossList();
	UFUNCTION()
	void OnClickSingle_Monster();
	UFUNCTION()
	void OnClickSingle_Item();
	UFUNCTION()
	void OnClickMulti_Monster();
	UFUNCTION()
	void OnClickMulti_Item();
	UFUNCTION()
	void OnClickSingleEntrance();
	UFUNCTION()
	void OnClickMultiPartyCreate();
	UFUNCTION()
	void OnClickMultiPartyNumber();
	UFUNCTION()
	void OnClickMultiFastEntrance();
	UFUNCTION()
	void OnClickMultiGuildParty();
	UFUNCTION()
	void OnEnterPartyNumber(FString PartyID);
	UFUNCTION()
	void OnTowerBossRefersh(int32 changedFloor);
	UFUNCTION()
	void OnUpdateTowerBossUIPage();
public:
	ETowerScrollType	eTowerSingleScrollType = ETowerScrollType::VE_MONSTERS;
	ETowerScrollType	eTowerMultiScrollType = ETowerScrollType::VE_MONSTERS;
	//	싱글
	UPROPERTY()
	UTextBlock*			Text_SPersonnel			=	nullptr;
	UPROPERTY()
	UTextBlock* 		Text_SLimitTime			=	nullptr;
	UPROPERTY()
	UTextBlock* 		Text_SReCommandedPower	=	nullptr;
	UPROPERTY()
	UTextBlock* 		Text_SRemainPlayCount	=	nullptr;
	UPROPERTY()
	UTextBlock* 		Text_SMaxPlayCount		=	nullptr;
	UPROPERTY()
	URScrollView*		Single_ScrollView_Monster		=	nullptr;
	UPROPERTY()
	URScrollView*		Single_ScrollView_Item			=	nullptr;
	UPROPERTY()
	UButton*			Button_SEntrance		=	nullptr;
	UPROPERTY()
	UButton*			Btn_Single_Monster		=	nullptr;
	UPROPERTY()
	UButton*			Btn_Single_Item			=	nullptr;

	//	멀티
	UPROPERTY()
	UTextBlock*			Text_MPersonnel			=	nullptr;
	UPROPERTY()
	UTextBlock*			Text_MLimitTime			=	nullptr;
	UPROPERTY()
	UTextBlock*			Text_MReCommandedPower	=	nullptr;
	UPROPERTY()
	URScrollView*		ScrollView_MReward		=	nullptr;
	UPROPERTY()
	UButton*			Button_MPartyCreate		=	nullptr;
	UPROPERTY()
	UButton*			Button_MPartyNumber		=	nullptr;
	UPROPERTY()
	UButton*			Button_MFastEntrance	=	nullptr;
	UPROPERTY()
	UButton*			Button_MGuildParty		=	nullptr;
	UPROPERTY()
	URScrollView*		Multi_ScrollView_Item	= nullptr;
	UPROPERTY()
	URScrollView*		Multi_ScrollView_Monster = nullptr;
	UPROPERTY()
	UButton*			Btn_Multi_Monster = nullptr;
	UPROPERTY()
	UButton*			Btn_Multi_Item = nullptr;

	//	공용
	UPROPERTY()
	UUC_Party_Keypad*	UC_party_keypad			=	nullptr;
private:
	int32				SelectFloor				=	1;
	TArray<FName>		FloorStageIDs;
	FName				SingleBossKey;
	FBossStage*			SingleBossTable			=	nullptr;
	FName				MultiBossKey;
	FBossStage*			MultiBossTable			=	nullptr;
	FFloorStageDetail*  SingleFloorStage		=	nullptr;
	FFloorStageDetail*  MultiFloorStage			=	nullptr;
};