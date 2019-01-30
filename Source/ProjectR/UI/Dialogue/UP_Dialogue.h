// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "UI/RBaseWidget.h"
#include "SharedConstants/GlobalConstants.h"
#include "CustomStructures/DialogueInfos.h"

#include "UP_Dialogue.generated.h"

class ULocalizingTextBlock;
class URGameInstance;

class UButton;

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FDialogueDelegate_NoParm);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FDialogueDelegate_OnOneParam, int32, Index);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FDialogueContentsAnimDelegate_OnOneParam, FString, VisibleString);


UCLASS()
class PROJECTR_API UDialogueStringAnimation : public UObject
{
	GENERATED_BODY()


	friend class UUP_Dialogue;

public:
	void Init();
	
	void					StartStringAnimation(FString contents);
	void					EndAnimation();
	void					Clear();
	void					ClearTimer();

	UFUNCTION()
	void					CallbackContentsTimer();

	bool					IsActiveAnimation();

	bool					ComputeSkipAnimation();

public:
	UPROPERTY()
	FDialogueContentsAnimDelegate_OnOneParam		ContentsAnimEvent;

private:

	UPROPERTY()
	FString						Contents;	
	UPROPERTY()
	FString						VisibleString;
	
	int32						CurrentIndex;
	int32						MaxIndex;

	UPROPERTY()
	FTimerHandle				TimerHandle_StringAnimation;

	UPROPERTY()
	float						TIMER_RATE = 0.05f;

};


USTRUCT(BlueprintType)
struct PROJECTR_API FCustomizeDialogue
{
	GENERATED_USTRUCT_BODY()

public:
	
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = FCustomizeDialogue)
	FVector		AdditionalLocation;

	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = FCustomizeDialogue)
	FRotator	AdditionalRotation;

	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = FCustomizeDialogue)
	UAnimationAsset*	OverridedAnim;
};

/**
 * 
 */
UCLASS()
class PROJECTR_API UUP_Dialogue : public URBaseWidget
{
	GENERATED_BODY()
	
public:
	enum FADESTATE { IDLE, FADEIN, FADEOUT };

public:
	void NativeConstruct() override;
	void NativeDestruct() override;
	void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;

	void	Init_AfterCreation(TArray<FDialogueInfo>& InArray, TMap<int32, FCustomizeDialogue> InMap);
	void	Create3DDialogueActor();
	

	UFUNCTION()
	void	Invalidate();

	UFUNCTION()
	void HideUI();

	UFUNCTION()
	void SkipUI();

	UFUNCTION()
	void ClickEvent();

	UFUNCTION()
	void CheckAutoPlay();
	
	void ShowSkipButton(bool bShow);
		
	
	FORCEINLINE void ResetTalkIndex()					{ CurrentTalkIndex = 0; }
	FORCEINLINE int32 GetCurrentDialogueID() { return DialogueIndexes[CurrentTalkIndex]; }

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UP_Dialogue|Ref")
	ULocalizingTextBlock*	ContentLocTextBlock;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UP_Dialogue|Ref")
	TArray<UTextBlock*>		NameTextArray;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UP_Dialogue|Ref")
	UWidgetSwitcher*		CharacterName_Switcher;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UP_Dialogue|Ref")
	UButton*				SkipButton;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UP_Dialogue|Ref")
	UButton*				InputShieldButton;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UP_Dialogue|Ref")
	UWidgetAnimation*		Animation_Btn_Dialogue_Looping;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UP_Dialogue|Ref")
	UCanvasPanel*			Dialogue;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UP_Dialogue|Ref")
	UImage*					Modal;

	UPROPERTY()
	UVerticalBox* chapter_text;
	
public:
	
	UPROPERTY()
	TArray<FDialogueInfo> Dialogues;

	UPROPERTY()
	TMap<int32, FCustomizeDialogue> OverrideMap;
	UPROPERTY()
	TArray<int32> DialogueIndexes;

private:
	void CreateContentsStringAnimation();

	UFUNCTION()
	void CallbackStringAnimation(FString InString);
	UFUNCTION()
	void EndSequence(FName InSequenceKey);

	bool IsPlayingStringAnimation();
	void PlayingStringAnimation();
	void EndStringAnimation(bool InForce = false);

	void StartFadeInOut(FADESTATE InFadeState);

	

private:
	//UPROPERTY()
	//URGameInstance*						RGameInstance = nullptr;
	UPROPERTY()
	class A3DDialogueActor*			Dialogue3DActor = nullptr;

	UPROPERTY()
	FName								CurrentCharNameID;

	UPROPERTY()
	UDialogueStringAnimation*			ContentsStringAnimation;

	int32								CurrentTalkIndex = 0;
	
	UPROPERTY()
	FString						ContentString;
	UPROPERTY()
	FString						VisibleString;
	UPROPERTY()
	int32						StringAnimationIndex = 0;
	UPROPERTY()
	FTimerHandle				TimerHandle_StringAnimation;

	float						FadeInOutTime = 3.f;
	float						FadeIntervalTime = 0.1f;
	
	FADESTATE					eFadeState = FADESTATE::IDLE;
	bool						IsSpecialDialogeState = false;
};
