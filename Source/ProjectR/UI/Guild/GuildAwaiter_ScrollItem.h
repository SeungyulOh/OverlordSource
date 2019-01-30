// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/Control/RScrollItem.h"
#include "GuildAwaiter_ScrollItem.generated.h"

class UGuildAwaiter_ScrollItemData;

/**
 * 
 */
UCLASS()
class PROJECTR_API UGuildAwaiter_ScrollItem : public URScrollItem
{
	GENERATED_BODY()
	
public:
	void NativeConstruct() override;
	void NativeDestruct() override;

	void InvalidateData();

	//delegate
	UFUNCTION()
		void OnClick_Button_UserInfo();
	UFUNCTION()
		void OnClick_Button_Approve();
	UFUNCTION()
		void OnClick_Button_Reject();

	//Set UI
	void SetNickName(FString nick);
	void SetLevel(int32 level);
	void SetLastAccessTime(int32 elapseTime);

	//property
	UPROPERTY()
		UTextBlock* Text_NickName;
	UPROPERTY()
		UTextBlock* Text_Level;
	UPROPERTY()
		UTextBlock* Text_Day;
	UPROPERTY()
		UTextBlock* Text_Hour;
	UPROPERTY()
		UTextBlock* Text_Minute;
	UPROPERTY()
		UTextBlock* Text_Day_Unit;
	UPROPERTY()
		UTextBlock* Text_Hour_Unit;
	UPROPERTY()
		UTextBlock* Text_Minute_Unit;
	UPROPERTY()
		UButton* Button_View;
	UPROPERTY()
		UButton* Button_Approve;
	UPROPERTY()
		UButton* Button_Reject;
};
