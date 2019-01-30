// Fill out your copyright notice in the Description page of Project Settings.

#include "ProjectR.h"

#include "ContentGameMode/Controllers/StagePlayerController.h"
#include "UtilFunctionIntegrated.h"
#include "Character/Components/FunctionalComponents/BattleEntityComponent.h"
#include "Character/Components/FunctionalComponents/SkillEntityComponent.h"


#include "GlobalIntegrated.h"


#include "Blueprint/WidgetLayoutLibrary.h"
#include "LeaderControlHelper.h"
//#include "SVirtualJoystick.h"
#include "RVirtualJoystick.h"

void URVirtualJoystick::NativeConstruct()
{
	BasePlayerController = Cast<AStagePlayerController>(UUtilFunctionLibrary::GetBasePlayerController(this->GetWorld()));

	URGameInstance* RGameInstance = RGAMEINSTANCE(this);
	ZPos_SkillArea = RGameInstance->GameEnvironmentVar.ZPos_SkillArea;
	//ZPos_SkillPickArea = RGameInstance->GameEnvironmentVar.ZPos_SkillPickArea;

	auto CanvasSlot = UWidgetLayoutLibrary::SlotAsCanvasSlot(this);
	InputScale = CanvasSlot->GetSize() * 0.5f;

	auto ImageCanvasSlot = UWidgetLayoutLibrary::SlotAsCanvasSlot(Image1);
	CorrectedThumbSize = ImageCanvasSlot->GetSize().X * 0.5f;
}

void URVirtualJoystick::NativeDestruct()
{

	Super::NativeDestruct();

}

void URVirtualJoystick::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);

	TouchTime += InDeltaTime;

	if (IsStickOn == false)
		return;

	if (IsValid(BasePlayerController) == false)
		return;

	if(IsInDeadZone(ComparePos))
		Image2->SetColorAndOpacity(FLinearColor(1.f, 0.f, 0.f));
	else
		Image2->SetColorAndOpacity(FLinearColor(1.f, 1.f, 1.f));
	

#if LOG_SKILLPAD
	UE_LOG(LogBattle, Log, TEXT("URVirtualJoystick::NativeTick offset %f, %f"), ThumbPosition.X, ThumbPosition.Y);
#endif
	FSlateApplication::Get().OnControllerAnalog(FGamepadKeyNames::RightAnalogX, 0, ThumbPosition.X);
	FSlateApplication::Get().OnControllerAnalog(FGamepadKeyNames::RightAnalogY, 0, ThumbPosition.Y);
}

void URVirtualJoystick::ComputePosition(FVector2D& InPosition)
{
#if LOG_SKILLPAD
	UE_LOG(LogBattle, Log, TEXT("URVirtualJoystick::ComputePosition"));
#endif
	auto CanvasSlot = UWidgetLayoutLibrary::SlotAsCanvasSlot(this);
	VisualCenter = InPosition - InputScale;
	CanvasSlot->SetPosition(VisualCenter);
	
	IsStickOn = true;
	
}



bool URVirtualJoystick::EventTouchMove()
{
#if LOG_SKILLPAD
	UE_LOG(LogBattle, Log, TEXT("URVirtualJoystick::EventTouchMove"));
#endif

	//UE_LOG(LogBattle, Log, TEXT("URVirtualJoystick::EventTouchMove"));

	ThumbPosition = ComparePos - StartPos;
	if (ThumbPosition.Size() >= CorrectedThumbSize)
	{
		ThumbPosition.Normalize();
		ThumbPosition *= CorrectedThumbSize;
	}

	auto CanvasSlot = UWidgetLayoutLibrary::SlotAsCanvasSlot(Image2);
	CanvasSlot->SetPosition(ThumbPosition);

	//bryan getting new scale
	JoyStickScale = (ThumbPosition.Size() - DeadZoneDist) / (CorrectedThumbSize - DeadZoneDist);
	
	return IsInDeadZone(ComparePos);
}

bool URVirtualJoystick::IsInDeadZone(FVector2D InCurrentPos)
{
// 	UE_LOG(LogBattle, Log, TEXT("URVirtualJoystick::EventTouchMove InCurrentPos(%f,%f)"), InCurrentPos.X, InCurrentPos.Y);
// 	UE_LOG(LogBattle, Log, TEXT("URVirtualJoystick::EventTouchMove ThumbPosition(%f,%f)"), PressedPos.X, PressedPos.Y);

	float SquaredTouchDist = FVector2D::DistSquared(PressedPos, InCurrentPos);
	if (SquaredTouchDist < (DeadZoneDist * DeadZoneDist))
	{
//		BasePlayerController->GetLeaderControlHelper()->GetLeaderSkillControl()->SkillSimpleTouch = true;
		return true;
	}
	//BasePlayerController->GetLeaderControlHelper()->GetLeaderSkillControl()->SkillSimpleTouch = false;
	return false;
}

void URVirtualJoystick::InitTouchPos(float InPosX, float InPosY)
{
	StartPos.Set(InPosX, InPosY);
	PressedPos.Set(InPosX, InPosY);
	ComparePos.Set(InPosX, InPosY);
	PreComparePos.Set(InPosX, InPosY);

	TouchTime = 0;
}

void URVirtualJoystick::SetComaparedPos(float InPosX, float InPosY)
{
	if (TouchTime < 1.5f)
	{
		FVector2D Temp(InPosX, InPosY);
	
		float SquaredTouchDist = FVector2D::DistSquared(PreComparePos, Temp);
		if (SquaredTouchDist < (TouchTolerance * TouchTolerance))
			return;

	}

	ComparePos.Set(InPosX, InPosY);
	PreComparePos.Set(InPosX, InPosY);
}

void URVirtualJoystick::Reset()
{
	IsStickOn = false;
}
