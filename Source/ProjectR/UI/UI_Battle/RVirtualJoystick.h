// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "UI/RBaseWidget.h"
//#include "SharedConstants/GlobalConstants.h"
//#include "SVirtualJoystick.h"
#include "RVirtualJoystick.generated.h"

class ABasePlayerController;
/**
 * 
 */
UCLASS()
class PROJECTR_API URVirtualJoystick : public URBaseWidget
{
	GENERATED_BODY()
	
public:
	void NativeConstruct() override;
	void NativeDestruct() override;
	void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;
	void ComputePosition(FVector2D& InPosition);

	bool EventTouchMove();

	void InitTouchPos(float InPosX, float InPosY);
	void SetComaparedPos(float InPosX, float InPosY);
	void Reset();

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = RVirtualJoystick)
	float	TouchTolerance = 15.0f;

	//Blueprint Ref
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = RVirtualJoystick_Ref)
	UImage*	Image1 = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = RVirtualJoystick_Ref)
	UImage*	Image2 = nullptr;
	UPROPERTY()
	float		JoyStickScale;

private:
	bool IsInDeadZone(FVector2D InCurrentPos);

private:
	UPROPERTY()
	class AStagePlayerController*	BasePlayerController = nullptr;

	/** The position of the thumb, in relation to the VisualCenter */
	UPROPERTY()
	FVector2D ThumbPosition;

	/** For recentered joysticks, this is the re-center location */
	UPROPERTY()
	FVector2D VisualCenter;

	/** The corrected scale for control input */
	UPROPERTY()
	float CorrectedThumbSize;

	UPROPERTY()
	TWeakObjectPtr<AActor>	OwnerCharacter;

	UPROPERTY()
	float		DeadZoneDist = 20.0f;
	UPROPERTY()
	bool		CheckDeadZone = false;
	UPROPERTY()
	FVector2D	InputScale;
	UPROPERTY()
	bool		IsStickOn = false;

	UPROPERTY()
	FVector		DirectTargetLocation;
	UPROPERTY()
	FVector		DeadZoneVec;

	
	UPROPERTY()
	FVector2D	StartPos;
	UPROPERTY()
	FVector2D	PressedPos;
	UPROPERTY()
	FVector2D	ComparePos;
	UPROPERTY()
	FVector2D	PreComparePos;

	float		TouchTime = 0;
	float		ZPos_SkillArea = 0;
	float		ZPos_SkillPickArea = 0;
};
