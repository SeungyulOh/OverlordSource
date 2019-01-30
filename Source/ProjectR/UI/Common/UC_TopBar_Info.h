// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "UI/RBaseWidget.h"
#include "UI/UIDefs.h"
#include "UC_TopBar_Info.generated.h"

class UUC_TopBar_Property;

/**
 * 
 */
UCLASS()
class PROJECTR_API UUC_TopBar_Info : public URBaseWidget
{
	GENERATED_BODY()

public:
	void NativeConstruct() override;
	void NativeDestruct() override;
	void RefreshProperty();
	void SetPropertySet();

public:
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = TopBar_Info)
	TArray<UUC_TopBar_Property*> TopBar_Info_Propertys;
};
