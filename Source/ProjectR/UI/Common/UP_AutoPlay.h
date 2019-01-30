// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "UI/RBaseWidget.h"
#include "UP_AutoPlay.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTR_API UUP_AutoPlay : public URBaseWidget
{
	GENERATED_BODY()
	
public:
	void NativeConstruct() override;
	void NativeDestruct() override;

};
