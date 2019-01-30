// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

//#include "UI/RBaseWidget.h"
#include "UI/CharacterUI/CharacterUserWidget.h"
#include "SharedConstants/GlobalConstants.h"
#include "UCW_HPBar.generated.h"

class URBuffIcon;
class ULocalizingTextBlock;
class UISkill_V2;
/**
 * BP: UCW_HPBar and UCW_HPBar_Small
 * WBP_CW_HpBar, WBP_CW_HpBar_Small
 */
UCLASS()
class PROJECTR_API UUCW_HPBar : public UCharacterUserWidget
{
	GENERATED_BODY()
	
public:
	void NativeConstruct() override;
	void NativeDestruct() override;
	void NativeTick(const FGeometry& MyGeometry, float InDeltaTime)	override;
	
	//TSharedRef<SWidget> RebuildWidget() override;

	void SetOwner(AActor* TargetActor) override;
	void RefreshCharacterUserWidget() override;
	
	//UFUNCTION(BlueprintImplementableEvent)
	//void ReceiveDamageAnimation();

	UFUNCTION(BlueprintCallable, Category = UCW_HPBar)
	void PlayAniBuffText(bool IsBuff);

	UFUNCTION(BlueprintCallable, Category = UCW_HPBar)
	void PlayAniImmuneText();

	UFUNCTION()
	void OnStart_TacticMoves(float Duration);

	//UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UCW_HPBar|Ref")
	UWidgetAnimation*		increase_text;
	//UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UCW_HPBar|Ref")
	UWidgetAnimation*		decrease_text;
	//UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UCW_HPBar|Ref")
	UWidgetAnimation*		immune_text;

	//UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UCW_HPBar|Ref")
	UWidgetAnimation*		HpBar_Damage;

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UCW_HPBar|Ref")
	UOverlay*				AvartarInfo;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UCW_HPBar|Ref")
	UTextBlock*				TextUserName;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UCW_HPBar|Ref")
	UTextBlock*				TextGuildName;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UCW_HPBar|Ref")
	UPanelWidget*			PanelMedal;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UCW_HPBar|Ref")
	UTextBlock*				TextMedalCount;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UCW_HPBar|Ref")
	UImage*					ImageJobInfo;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UCW_HPBar|Ref")
	ULocalizingTextBlock*	LocalText_Stat = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UCW_HPBar|Ref")
	UPanelWidget*			Panel_ProgressBar = nullptr;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UCW_HPBar|Ref")
	UProgressBar*			ProgressBar_HP = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UCW_HPBar|Ref")
	UImage*					Image_Hero = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UCW_HPBar|Ref")
	UImage*					Image_BG = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UCW_HPBar|Ref")
	UProgressBar*			ProgressBar_SkillCasting = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UCW_HPBar|Ref")
	URBuffIcon*				AbnormalIcon1;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UCW_HPBar|Ref")
	URBuffIcon*				AbnormalIcon2;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UCW_HPBar|Ref")
	URBuffIcon*				AbnormalIcon3;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UCW_HPBar|RefAni")
	UWidgetAnimation*		HpBarDamageAni = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UCW_HPBar|RefAni")
	UWidgetAnimation*		HpBarSelectAni = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UCW_HPBar|RefAni")
	UWidgetAnimation*		HpBarUnSelectAni = nullptr;

	//UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UCW_HPBar|RefAni")
	//UWidgetAnimation*		IncreaseTextAni = nullptr;

	//UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UCW_HPBar|RefAni")
	//UWidgetAnimation*		DecreaseTextAni = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UCW_HPBar)
	bool					bHpBar_Small = false;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UCW_HPBar)
	bool					bHideDelay = false;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UCW_HPBar)
	bool					AlwaysShowHPBar;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UCW_HPBar|Value")
	FLinearColor			Color_EnemyBar = FLinearColor::Red;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UCW_HPBar|Value")
	FLinearColor			Color_FriendBar = FLinearColor(0.000911f, 0.391572f, 0.846873f);//FLinearColor::Blue;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UCW_HPBar|Value")
	FSlateColor				Color_EnemySlate = FLinearColor::Red;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UCW_HPBar|Value")
	FSlateColor				Color_FriendSlate = FLinearColor::White;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UCW_HPBar|Value")
	FSlateColor				Color_PartySlate = FLinearColor::Blue;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UCW_HPBar|Value")
	FVector2D				DefaultSize = FVector2D(1.0f, 1.0f);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UCW_HPBar|Value")
	FVector2D				SelectSize = FVector2D(1.3f, 1.3f);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UCW_HPBar|Ref")
	UHorizontalBox*			Box_AbnormalIcons;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UCW_HPBar|Ref")
	UImage*					Image_ProtectionMark;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UCW_HPBar|Ref")
	ULocalizingTextBlock*	LocalizingTextBlock_1 = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UCW_HPBar|Ref")
	class UUC_PileUpUI*		PileUpProgressUI;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UCW_HPBar|Ref")
	UHorizontalBox*			HpBar_Box = nullptr;// combineskill 사용할 때 숨김

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UCW_HPBar|Ref")
	UCanvasPanel*			HpBar_Box_Small = nullptr;// combineskill 사용할 때 숨김
public:
	UFUNCTION()
	void OnUpdateAttackCount(AActor* InActor);

private:
	UFUNCTION()
	void OnReceiveDamage(AActor* Invoker, float Damage, EBattleDamageResultEnum DamageResultType, AActor* FromActor);

	UFUNCTION()
	void OnRefreshRealTimeParam(int32 InRealTimeParam);

	UFUNCTION()
	void SetCurrentSkillIcon(AActor* InCaster, UISkill_V2* UseSkill);

	UFUNCTION()
	void HideEvent();

	UFUNCTION()
	void SettingHPBar();

	UFUNCTION()
	void UpdateHP();
	void UpdateCustomScale();

	void UpdateTaticsMove(float TimeDelta);
	
	void SetAbnormalIcon();
	UFUNCTION()
	void CheckAbnormal(EAbnormalSlotTypeEnum InSlotType, bool InSet);
	UFUNCTION()
	void CheckStatAbnormal(EAbnormalSlotTypeEnum InSlotType, bool InSet);
	
	UFUNCTION()
	void SetBlockAbnormalMsg(ESkillApplierTypeEnum InBlockApplierType);

	UFUNCTION()
	void CheckProtectActorSpawn(AActor* SpawnActor);

	void SetScale();
	void SetCharacterName();
	void SetCharacterColor();
	void SetCharacterJob();
	void SetDamageDelegate();
	void SetRealTimeParamDelegate();
	void SetSkillCastingProgress();
	
	void ProgressSkillCastingDataReset();
	
	void UpdateSkillCastingProgress(float InDeltaTime);

	UFUNCTION()
	void OnHpBarBoxShow(bool bShow);
private:
	UPROPERTY()
	class ABasePlayerController*	BasePlayerController = nullptr;
	UPROPERTY()
	class UBattleEntityComponent*	BattleEntityComponent = nullptr;

	UPROPERTY()
	FTimerHandle CheckTimerHandler;

	UPROPERTY()
	ECharacterCategoryTypeEnum CharacterCategory;

	UPROPERTY()
	TArray<URBuffIcon*> AbnormalIconArray;

	UPROPERTY()
	TArray<EAbnormalSlotTypeEnum> AbnormalStack;

	bool		bEnemyAgainstCurrentPlayer = false;
	bool		IsSelectHpBar = false;
	bool		IsCustomScale = false;
	bool		IsControlCharacter = false;
	bool		IsPartyCharacter = false;
	bool		IsNonPortrait = false;
	bool		IsBossCharacter = false;
	bool		IsEnemyHero = false;
	bool		IsEnemySummon = false;
	bool		IsUpdateCombineProgress = false;
	bool		bCastingTrigger = false;
	float		fCurrentHP = 0.0f;
	float		fCastingTime = 0.0f;
	float		fCurrentPercent = 0.0f;
	float		fCurrentTime = 0.0f;

	/*Tactics Move*/
	UPROPERTY()
	UOverlay*		Meet_CoolTime;
	UPROPERTY()
	UImage*			Image_CoolGauge;
	UPROPERTY()
	UTextBlock*		Text_CoolTime;
	UPROPERTY()
	UMaterialInstanceDynamic* MID;

	float		TM_Duration = -1.f;
	float		TM_RemainingTimeToReuse = -1.f;
	/**/

};
