// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "UI/RBaseWidget.h"

#include "UI/Control/RScrollView.h"

#include "UC_HeroInfo_Upgrade_Material.generated.h"

UCLASS()
class PROJECTR_API UUC_HeroInfo_Upgrade_Material : public URBaseWidget
{
	GENERATED_BODY()
public:
	void NativeConstruct()	override;
	void NativeDestruct()	override;
protected:
private:

public:
	UPROPERTY()
		URScrollView*			InventoryScrollView		=	nullptr;
protected:
private:

};
