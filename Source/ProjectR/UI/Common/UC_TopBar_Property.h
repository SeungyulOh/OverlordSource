// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "UI/RBaseWidget.h"
#include "Network/PacketDataEnum.h"
#include "UC_TopBar_Property.generated.h"

class UImage;
class UTextBlock;
class UBasePropertyState;

/**
 * 
 */
UCLASS()
class PROJECTR_API UUC_TopBar_Property : public URBaseWidget
{
	GENERATED_BODY()
	
public:
	void NativeConstruct() override;
	void NativeDestruct() override;
	void SetPropertyType(FName InType);
	FName GetPropertyType() { return PropertyType; }
	void RefreshCurrent();


	

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UC_TopBar_Property|Ref")
	UImage*			CurrencyImage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UC_TopBar_Property|Ref")
	UTextBlock*		CurrencyValue;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UC_TopBar_Property|Ref")
	UButton*		Button_Click = nullptr;

private:
	bool SetState();
	void Refresh();

	UFUNCTION()
	void OnClick_DirectLink();	

private:
	UPROPERTY()
	FName		PropertyType;

	UPROPERTY()
	UBasePropertyState* PropertyState = nullptr;
};
