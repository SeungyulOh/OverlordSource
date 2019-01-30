// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "UI/RBaseWidget.h"
#include "SharedConstants/GlobalConstants.h"
#include "RBuffIcon.generated.h"


/**
 * 
 */
UCLASS()
class PROJECTR_API URBuffIcon : public URBaseWidget
{
	GENERATED_BODY()
	
public:
	void NativeConstruct() override;
	void NativeDestruct() override;
	void OnWidgetRebuilt() override;

	void SetAbnormalState(EAbnormalSlotTypeEnum InAbnormal);
	void SetStatUpDownState(EAbnormalSlotTypeEnum InAbnormal);
	void SetSkillIconState(ESkillIconShapeEnum InShape);
	void SetIconImage(FName InKey);
	UFUNCTION()
	void BP_PlayAniSpawn();

	FORCEINLINE bool IsActive()										{ return (CurrentSlot != EAbnormalSlotTypeEnum::VE_None); }
	FORCEINLINE bool IsTypeCheck(EAbnormalSlotTypeEnum InAbnormal)	{ return (CurrentSlot == InAbnormal); }

public:
	UPROPERTY(EditAnywhere, BLueprintReadWrite, Category = "RBuffIcon|Ref")
	UImage*					Image_Icon = nullptr;
	
	UPROPERTY(EditAnywhere, BLueprintReadWrite, Category = "RBuffIcon|Ref")
	UImage*					NotiMaterial = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "RBuffIcon|Ani")
	UWidgetAnimation*		StartAni = nullptr;

	UPROPERTY(EditAnywhere, BLueprintReadWrite, Category = RBuffIcon)
	bool					IsSpawnAni = true;

	UPROPERTY(EditAnywhere, BLueprintReadWrite, Category = RBuffIcon)
	FVector2D				TempCustomSize;

private:
	UPROPERTY()
	EAbnormalSlotTypeEnum	CurrentSlot = EAbnormalSlotTypeEnum::VE_None;

};
