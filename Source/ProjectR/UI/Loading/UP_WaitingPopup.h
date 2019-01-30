// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "UI/RBaseWidget.h"
#include "Table/LoadingTableInfo.h"
#include "UP_WaitingPopup.generated.h"

class UTextBlock;
class UImage;

/**
 * 
 */
UCLASS()
class PROJECTR_API UUP_WaitingPopup : public URBaseWidget
{
	GENERATED_BODY()
	
	
public:
	void NativeConstruct() override;
	void NativeDestruct() override;


public:
	UPROPERTY()
	class UUC_Particle* UC_Particle;
	

};
