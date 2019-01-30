// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "UI/RBaseWidget.h"
#include "UI/UIDefs.h"
#include "UP_AvatarIcon.generated.h"

class URScrollView;
class URGameInstance;
/**
 * 
 */
UCLASS()
class PROJECTR_API UUP_AvatarIcon : public URBaseWidget
{
	GENERATED_BODY()
	
public:
	void NativeConstruct() override;
	void NativeDestruct() override;

	void SetActiveSubPage() override;

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UP_AvatarIcon|Ref")
	URScrollView*			Scroll_Icon = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UP_AvatarIcon|Ref")
	UButton*				Button_Click = nullptr;
	
private:
	void Refresh();
	void SetUITopBarString();

	UFUNCTION()
	void SetIconScroll();

	UFUNCTION()
	void ClickButton();

private:
	UPROPERTY()
	class URGameInstance* RGameInstance = nullptr;
};
