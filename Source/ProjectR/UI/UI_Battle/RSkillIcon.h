// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "UI/RBaseWidget.h"
#include "SharedConstants/GlobalConstants.h"
#include "RSkillIcon.generated.h"


class URGameInstance;
class UUC_SkillIndicator;
class URBuffIcon;
struct FSkillResourceTableInfo;

/**
 * 
 */
UCLASS()
class PROJECTR_API URSkillIcon : public URBaseWidget
{
	GENERATED_BODY()
	
public:
	void NativeConstruct() override;
	void NativeDestruct() override;
	void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;

	UFUNCTION(BlueprintCallable, Category = SkillIconUI)
	void Init(AActor* InOwnerCharacter, ESkillCategoryTypeEnum InSkillCategoryType );

	UFUNCTION(BlueprintCallable, Category = SkillIconUI)
	void SetSkillIconState( ESkillIconState InSkillIconState );
	
	UFUNCTION(BlueprintCallable, Category = SkillIconUI)
	void PressIcon();

	UFUNCTION(BlueprintCallable, Category = SkillIconUI)
	void ReleaseIcon();
	
	UFUNCTION()
	void OnDeadActor(AActor* DeadActor, AActor* FromActor);
	
	UFUNCTION()
	void CheckSynergy(bool InSet);

	void UpdateSkillUseCost();
	void UpdateCooling();
	void EndCooling();
	void InvalidateOwnerCharacter();

	void EndCrewSkillCooling();
	void SetAbnormalImage(bool InIsVisible);

	void SetSkillIconImage(FSkillResourceTableInfo* SkillResInfo, ESkillCategoryTypeEnum InSkillCategoryType = ESkillCategoryTypeEnum::VE_None);
	void SetOwnerCharacter(AActor* InActor);

	FORCEINLINE void SetComputeSynergyMesh(bool InValue) { IsComputeSynergyMesh = InValue; }
	FORCEINLINE void SetOnlyDisplay(bool only_display)	{ OnlyDisplay = only_display; }
	FORCEINLINE FName GetSkillID()						{ return CurrentSkillID; }
	FORCEINLINE bool GetIsTouch()						{ return IsTouch; }
	FORCEINLINE void SetSkillIndicator(UUC_SkillIndicator* InSkillIndicator) { HeroSkillIndicator = InSkillIndicator; }
	FORCEINLINE AActor* GetOwnerCharacter() { return OwnerCharacter.Get(); }


	void StartCrewCoolTime(AActor* InPrevCharacter, FName InPrevSkillID);
	void StartSpiritCoolTime(FName InPrevSkillID);
	bool IsCoolTime();
public:
	//Param
	//UPROPERTY(EditAnywhere, BLueprintReadWrite, Category = SkillIconUI)
	//float					TouchDelayTime = 0.5f;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = SkillIconUI)
	FLinearColor			InduceColor;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = SkillIconUI)
	FLinearColor			DamageColor;

	//ref
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SkillIcon|Ref")
	UWidgetSwitcher*		Switcher_BGImage = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SkillIcon|Ref")
	UWidgetSwitcher*		PortraitSwitcher = nullptr;

	UPROPERTY(EditAnywhere, BLueprintReadWrite, Category = "SkillIcon|Ref")
	UImage*					Image_ActiveBG = nullptr;


	UPROPERTY(EditAnywhere, BLueprintReadWrite, Category = "SkillIcon|Ref")
	UImage*					Image_SkillIcon = nullptr;

	UPROPERTY(EditAnywhere, BLueprintReadWrite, Category = "SkillIcon|Ref")
	UTextBlock*				Text_CoolTime = nullptr;

	UPROPERTY(EditAnywhere, BLueprintReadWrite, Category = "SkillIcon|Ref")
	UTextBlock*				Text_LockLevel = nullptr;

	UPROPERTY(EditAnywhere, BLueprintReadWrite, Category = "SkillIcon|Ref")
	UImage*					Image_CoolGauge = nullptr;

	UPROPERTY(EditAnywhere, BLueprintReadWrite, Category = "SkillIcon|Ref")
	UImage*					Image_TargetingIndicator = nullptr;

	UPROPERTY(EditAnywhere, BLueprintReadWrite, Category = "SkillIcon|Ref")
	UImage*					Image_SkillPoint = nullptr;

	UPROPERTY(EditAnywhere, BLueprintReadWrite, Category = "SkillIcon|Ref")
	UImage*					Image_SkillPad = nullptr;

	UPROPERTY(EditAnywhere, BLueprintReadWrite, Category = "SkillIcon|Ref")
	UImage*					Image_SkillLock = nullptr;

	UPROPERTY(EditAnywhere, BLueprintReadWrite, Category = "SkillIcon|Ref")
	UImage*					Image_Abnormal = nullptr;

	UPROPERTY(EditAnywhere, BLueprintReadWrite, Category = "SkillIcon|Ref")
	UImage*					Image_Synergy = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SkillIcon|Ref")
	UButton*				Button_Click = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SkillIcon|Ref")
	URBuffIcon*				SkillBuffIcon = nullptr;

	//Bryan Works
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SkillIcon|Ref")
	UOverlay*				PortraitOverlay = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SkillIcon|Ref")
	UOverlay*				Portrait = nullptr;

	UPROPERTY(EditAnywhere, BLueprintReadWrite, Category = "SkillIcon|Ref")
	UImage*					Image_Portrait = nullptr;

	UPROPERTY(EditAnywhere, BLueprintReadWrite, Category = "SkillIcon|Ref")
	UCanvasPanel*			Combine_ON = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SkillIcon|Ref")
	bool					IsCrewChangeIcon = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SkillIcon|Ref")
	ESkillOrigin			SkillOriginType = ESkillOrigin::VE_Leader;
	//

	UPROPERTY()
	float					CoolTime = -1.0f;

private:
	UPROPERTY()
	TWeakObjectPtr< AActor >	OwnerCharacter;

	UPROPERTY()
	FName						CharacterID;

	UPROPERTY()
	FName						CurrentSkillID;

	TWeakObjectPtr< AActor >	PrevCharacter;
	FName						PrevSkillID;

	UPROPERTY()
	FName						SkillTableID;
	UPROPERTY()
	ESkillCategoryTypeEnum		SkillCategoryType;
	bool						IsActive = false;
	UPROPERTY()
	ESkillIconState				SkillIconState = ESkillIconState::VE_None;
	float						MaxCoolTime = -1.0f;
	bool						OnlyDisplay = false;
	bool						IsTouch = false;
	float						CheckTouchTime = 0;

	float						CheckCost = 1;
	bool						IsTouchUpdate = false;
	bool						IsPressFire = false;

	UPROPERTY()
	URGameInstance*				RGameInstance = nullptr;

	//only button icon
	UPROPERTY()
	UUC_SkillIndicator*			HeroSkillIndicator = nullptr;

	bool						IsComputeSynergyMesh = false;
};
