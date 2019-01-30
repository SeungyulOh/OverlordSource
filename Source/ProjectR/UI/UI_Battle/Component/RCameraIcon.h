// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "UI/RBaseWidget.h"
#include "SharedConstants/GlobalConstants.h"
#include "RCameraIcon.generated.h"


UENUM(BlueprintType)
enum class ECameraType : uint8
{
	VE_NormalLong	UMETA(DisplayName = "NormalLong"),
	VE_NormalShort	UMETA(DisplayName = "NormalShort"),
	VE_Dynamic		UMETA(DisplayName = "Dynamic"),
};

/**
*
*/
UCLASS()
class PROJECTR_API URCameraIcon : public URBaseWidget
{
	GENERATED_BODY()

public:
	void NativeConstruct() override;
	void NativeDestruct() override;

	void RefreshCameraView();

	UFUNCTION(BlueprintCallable, Category = URCameraIcon)
	void OnClick_CameraView();

	ECameraType GetCurrentViewType() { return CurrentCameraViewType; }

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = RCameraIcon)
	UWidgetSwitcher*	Switcher_IconImage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = RCameraIcon)
	UButton*	CameraView_Short;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = RCameraIcon)
	UButton*	CameraView_Dynamic;

private:
	UPROPERTY()
	ECameraType				CurrentCameraViewType = ECameraType::VE_NormalLong;
	UPROPERTY()
	EGamePlayModeTypeEnum	CheckGameMode = EGamePlayModeTypeEnum::VE_None;
	UPROPERTY()
	FVector					CameraInitLocation = FVector::ZeroVector;

};
