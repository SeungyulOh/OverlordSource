// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "UI/RBaseWidget.h"
#include "UI/UIDefs.h"

#include "UP_HeroInfoSkill.generated.h"


UCLASS()
class PROJECTR_API UUP_HeroInfoSkill : public URBaseWidget
{
	GENERATED_BODY()
//	¸É¹ö ÇÔ¼ö
public:
	void NativeConstruct() override;
	void NativeDestruct() override;
	void Init();
protected:
private:
//	¸É¹ö º¯¼ö
public:	
protected:
private:
};