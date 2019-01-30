// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "UI/RBaseWidget.h"
#include "SharedConstants/GlobalConstants.h"
#include "RCancelIcon.generated.h"



class AStagePlayerController;
/**
 * 
 */
UCLASS(Abstract)
class PROJECTR_API URCancelIcon : public URBaseWidget
{
	GENERATED_BODY()
	
public:
	void NativeConstruct() override;
	void NativeDestruct() override;
	void BeginDestroy() override;
	bool IsReadyForFinishDestroy() override;

	UFUNCTION()
	void BtnEventOnHovered();

	UFUNCTION()
	void BtnEventOnUnhovered();
	
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = RCancelIcon)
	UButton*	CancelButton;

};
