// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "UI/RBaseWidget.h"
#include "UC_QuickToggle.generated.h"

class UBaseToggleState;

UENUM(BlueprintType)
enum class EToggleType : uint8
{
	VE_None			UMETA(DisplayName = "None"),
	VE_Campaign		UMETA(DisplayName = "Campaign"),
	VE_Raid			UMETA(DisplayName = "Raid"),
};

class UWidgetSwitcher;;
/**
 * 
 */
UCLASS()
class PROJECTR_API UUC_QuickToggle : public URBaseWidget
{
	GENERATED_BODY()
	
public:
	void CreateState(EToggleType ToggleType, int32 InToggleIndex = -1);

	UFUNCTION(BlueprintCallable, Category = UUC_QuickToggle)
	void OnClick_QuickToggle();

	void SetChapterID(FName InChapterID)	{ ChapterID = InChapterID; }
	FName GetChapterID()					{ return ChapterID; }
	void SetActive(bool InbIsActive);
	int32 GetToggleIndex() { return ToggleIndex; }
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UUC_QuickToggle)
	FName	ChapterID;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UUC_QuickToggle)
	UWidgetSwitcher*	WidgetSwitcher_Toggle;

	UPROPERTY()
	UBaseToggleState* BaseToggleState = nullptr;

private:
	int32 ToggleIndex = -1;
};
