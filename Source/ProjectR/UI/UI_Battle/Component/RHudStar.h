// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "UI/RBaseWidget.h"
#include "SharedConstants/GlobalConstants.h"
#include "ProjectRGameMode.h"

#include "RHudStar.generated.h"

class UImage;

/**
 * 
 */
UCLASS()
class PROJECTR_API URHudStar : public URBaseWidget
{
	GENERATED_BODY()
	
public:
	void	NativeConstruct() override;
	void	NativeDestruct() override;

	UFUNCTION(BlueprintCallable, Category = RHudStar)
	void UpdateImage(EPlayStageStarEnum InStarCnt);

	UFUNCTION()
	void UpdateStar();

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = RHudStar)
	UImage*	Image_1		= nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = RHudStar)
	UImage*	Image_2		= nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = RHudStar)
	UImage*	Image_3 = nullptr;

private:
	UPROPERTY()
	EPlayStageStarEnum  StarCnt = EPlayStageStarEnum::VE_None;

	int32	StarNum = 2;
};
