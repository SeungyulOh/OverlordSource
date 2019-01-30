// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "UI/RBaseWidget.h"
#include "SharedConstants/GlobalConstants.h"
#include "RBossBar.generated.h"


class ULocalizingTextBlock;
class UProgressBar_Phase;
class URBattleTimer;
class URBuffIcon;
class UISkill_V2;
/**
 * 
 */
UCLASS()
class PROJECTR_API URBossBar : public URBaseWidget
{
	GENERATED_BODY()
	
public:
	void	NativeConstruct() override;
	void	NativeDestruct() override;
	void	NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;

	bool	IsValidData();

	UFUNCTION(BlueprintCallable, Category = RBossBar)
	void	BossSpawnEvent(AActor* InCharacter);

	UFUNCTION(BlueprintCallable, Category = RBossBar)
	void	BossHideEvent();

	UFUNCTION(BlueprintCallable, Category = RBossBar)
	void	ReceiveDamageEvent(AActor* InCharacter, float Damage, EBattleDamageResultEnum DamageResultType, AActor* FromActor);

	UFUNCTION(BlueprintCallable, Category = RBossBar)
	void	CallbackShieldNotify(float InShieldPercent, bool InIsTimeOut);

	UFUNCTION()
	void SetCurrentSkillIcon(AActor* InCaster, UISkill_V2* UseSkill);

	//////////////////////////////////////////////////////////////////////////
	// RealTimeMod
	UFUNCTION()
	void OnTargetChange(AActor* AttackTarget);
	UFUNCTION()
	void OnDisappear_RTS_Player(int32 UserKID);

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = RBossBar)
	ULocalizingTextBlock*	LocalText_BossName = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = RBossBar)
	UTextBlock*				TextBlock_BossLevel = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = RBossBar)
	UProgressBar_Phase*		ProgressBar_Base = nullptr;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = RBossBar)
	UProgressBar_Phase*		ProgressBar_Deco = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = RBossBar)
	UProgressBar*			ProgressBar_Shield_HP = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = RBossBar)
	USlider*				Slider_Needle = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = RBossBar)
	UTextBlock*				Text_BossHPCount = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = RBossBar)
	UTextBlock*				Text_BossHPSign = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = RBossBar)
	UTextBlock*				Text_TargetName = nullptr;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = RBossBar)
	UImage*					TargetImage = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = RBossBar)
	URBattleTimer*			BattleTimer = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = RBossBar)
	UCanvasPanel*			Skill_Box = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = RBossBar)
	UProgressBar*			ProgressBarSkillCasting = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = RBossBar)
	UTextBlock*				SkillName = nullptr;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = RBossBar)
	UOverlay*				Skill_Overlay = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = RBossBar)
	UImage*					ImageSkillIcon = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "RBossBar|Ref")
	URBuffIcon*				BuffIcon1;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "RBossBar|Ref")
	URBuffIcon*				BuffIcon2;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "RBossBar|Ref")
	URBuffIcon*				BuffIcon3;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "RBossBar|Ref")
	URBuffIcon*				BuffIcon4;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "RBossBar|Ref")
	URBuffIcon*				BuffIcon5;

	//Animation
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = RBossBar)
	UWidgetAnimation*		GlowLoop_Ref;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = RBossBar)
	UWidgetAnimation*		GaugeDamageAni_Ref;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = RBossBar)
	UWidgetAnimation*		BossGaugeUp_Ref;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = RBossBar)
	UWidgetAnimation*		BossGaugeDown_Ref;
	
protected:
	void ShowHPBar(AActor* InCharacter);
	void SynchronizeTargetData();

	void SynchronizeBossData();
	void SkillCastingProgressBarTick(float InDeltaTime);
	void ProgressSkillCastingDataReset();
	
	void TargetNameCheckTick(float InDeltaTime);
	void SetTargetNameText(FText TargetName);
	void HideTargetNameText();

	void ClearAbnormalIcon();
	void CheckAbnormalFromChangeTarget();
	void SetAbnormalIcon();
	UFUNCTION()
	void CheckAbnormal(EAbnormalSlotTypeEnum InSlotType, bool InSet);
	void CreateCheckAbnormalArray();

private:
	UPROPERTY()
	AActor*					TargetCharacter = nullptr;

	UPROPERTY()
	TArray<URBuffIcon*>		AbnormalIconArray;

	UPROPERTY()
	TArray<EAbnormalSlotTypeEnum> CheckAbnormalStatArray;

	UPROPERTY()
	int32					CheckPhaseValue = -1;

	bool					IsFireSkill = false;

	float					fCastingTime = 0.0f;
	float					fCurrentPercent = 0.0f;
	float					fCurrentTime = 0.0f;
	bool					bCastingTrigger = false;
	bool					bBossTypeTarget = false;
	bool					IsInitAbnormalStat = true;

	ECharacterCategoryTypeEnum	eTargetCategory = ECharacterCategoryTypeEnum::VE_None;
	UPROPERTY()
	FText					CurrentDisplayName;
};
