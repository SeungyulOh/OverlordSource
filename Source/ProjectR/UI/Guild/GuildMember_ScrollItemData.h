// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "UI/Control/RScrollItemData.h"
#include "UI/UIDefs.h"
#include "Network/PacketDataStructures.h"
#include "GuildMember_ScrollItemData.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTR_API UGuildMember_ScrollItemData : public URScrollItemData
{
	GENERATED_BODY()
	
public:
	UPROPERTY()
	FGUILD_MEMBER	GuildMemberData;
	UPROPERTY()
	EGUILD_GRADE	MyGuildGrade;
	UPROPERTY()
	int32			MyAccountKid;
	UPROPERTY()
	UCanvasPanel*	PopupPanel = nullptr;
	
	UFUNCTION(BlueprintCallable, meta = (HidePin = "WorldContextObject", DefaultToSelf = "WorldContextObject"), Category = UGuildMember_ScrollItemData)
	void SetGuildMemberData(UObject* WorldContextObject, UTextBlock* MemberName, UTextBlock* MemberLevel, UWidgetSwitcher* MamberGrade, UTextBlock* MemberContribute, 
		UTextBlock* Text_Day, UTextBlock* Text_DayWord, UTextBlock* Text_Hour, UTextBlock* Text_HourWord, UTextBlock* Text_Minute, UTextBlock* Text_MinuteWord);
	UFUNCTION(BlueprintCallable, meta = (HidePin = "WorldContextObject", DefaultToSelf = "WorldContextObject"), Category = UGuildMember_ScrollItemData)
	void SetGuildMemberButton(UObject* WorldContextObject, UButton* GradeUp, UButton* GradeDown, UButton* MemberKick, UButton* ChangeMaster, UButton* AddFriend, UButton* ViewInfo, UButton* SendMail);

	UFUNCTION(BlueprintCallable, meta = (HidePin = "WorldContextObject", DefaultToSelf = "WorldContextObject"), Category = UGuildMember_ScrollItemData)
	void OnClick_SetVisiblePanel(UObject* WorldContextObject, UCanvasPanel* Panel, ESlateVisibility PanelVisibility);
	void SetVisiblePanel(ESlateVisibility PanelVisibility);
	
	UFUNCTION(BlueprintCallable, meta = (HidePin = "WorldContextObject", DefaultToSelf = "WorldContextObject"), Category = UGuildMember_ScrollItemData)
	void OnClick_GradeUp(UObject* WorldContextObject);
	UFUNCTION(BlueprintCallable, meta = (HidePin = "WorldContextObject", DefaultToSelf = "WorldContextObject"), Category = UGuildMember_ScrollItemData)
	void OnClick_GradeDown(UObject* WorldContextObject);
	UFUNCTION(BlueprintCallable, meta = (HidePin = "WorldContextObject", DefaultToSelf = "WorldContextObject"), Category = UGuildMember_ScrollItemData)
	void OnClick_Kick(UObject* WorldContextObject);
	UFUNCTION()
	void MemberKick();
	UFUNCTION(BlueprintCallable, meta = (HidePin = "WorldContextObject", DefaultToSelf = "WorldContextObject"), Category = UGuildMember_ScrollItemData)
	void OnClick_ChangeMaster(UObject* WorldContextObject);
	UFUNCTION()
	void ChangeMaster();
	UFUNCTION(BlueprintCallable, meta = (HidePin = "WorldContextObject", DefaultToSelf = "WorldContextObject"), Category = UGuildMember_ScrollItemData)
	void OnClick_AddFriend(UObject* WorldContextObject);
	UFUNCTION(BlueprintCallable, meta = (HidePin = "WorldContextObject", DefaultToSelf = "WorldContextObject"), Category = UGuildMember_ScrollItemData)
	void OnClick_View(UObject* WorldContextObject);
	UFUNCTION(BlueprintCallable, meta = (HidePin = "WorldContextObject", DefaultToSelf = "WorldContextObject"), Category = UGuildMember_ScrollItemData)
	void OnClick_Mail(UObject* WorldContextObject);

private:
	UWorld*	OwnedWorld = nullptr;
};