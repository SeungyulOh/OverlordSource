// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "UI/RBaseWidget.h"
#include "UP_SelectRewardPopup.generated.h"


class URScrollView;
struct FREWARD;

/**
*
*/
UCLASS()
class PROJECTR_API UUP_SelectRewardPopup: public URBaseWidget
{
	GENERATED_BODY()

public:
	void NativeConstruct() override;
	void NativeDestruct() override;

public:
	UFUNCTION(BlueprintCallable, Category = UP_SelectRewardPopup)
	void OnClickReceive();

	UFUNCTION(BlueprintCallable, Category = UP_SelectRewardPopup)
	void OnClickCancel();

	UFUNCTION()
	void OnSelectItem(URScrollItem* selectItem);

	UFUNCTION()
	void OnSelectRewardConfirmOK();
public:
	void Init(const FString& InMailUD, const TArray<FREWARD>& InRewards);

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UP_SelectRewardPopup|Ref")
	URScrollView*				ItemScrollView;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UP_SelectRewardPopup|Ref")
	UButton*					SelectOKButton;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UP_SelectRewardPopup|Ref")
	UButton*					Button_Close;
	

private:
	int32		CurrentSelected;

	UPROPERTY()
	FString		CurrentMailUD;
};