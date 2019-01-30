// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "UI/RBaseWidget.h"
#include "SharedConstants/GlobalConstants.h"
#include "RAutoSkillIcon.generated.h"



class URGameInstance;
class AStagePlayerController;
/**
 * 
 */
UCLASS()
class PROJECTR_API URAutoSkillIcon : public URBaseWidget
{
	GENERATED_BODY()

public:
	void	NativeConstruct() override;
	void	NativeDestruct() override;

	UFUNCTION()
	void	BtnEventOnClicked();
	void	NextAutoSkillType(bool InIsSave);
	UFUNCTION()
	void	RefreshAutoSkillIcon();

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "RAutoSkillIcon|Ref")
	UImage*				Image_BG;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "RAutoSkillIcon|Ref")
	UWidgetSwitcher*	Switcher_IconImage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "RAutoSkillIcon|Ref")
	UButton*			Button_Click;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "RAutoSkillIcon|Ref")
	UButton*			Button_tutorial;
private:
	UPROPERTY()
	AStagePlayerController*	BasePlayerController = nullptr;

	int32	ActiveSwitchIndex = 0;

	bool	bUnlockAutoSkillStage = true;
};
