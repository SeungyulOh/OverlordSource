// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "UI/Control/RScrollItem.h"
#include "UI/UIDefs.h"
#include "Network/PacketDataStructures.h"
#include "GuildManagerListScrollItem.generated.h"

class UUC_ScrollItem_AvatarIcon;
/**
 * 
 */
UCLASS()
class PROJECTR_API UGuildManagerListScrollItem : public URScrollItem
{
	GENERATED_BODY()

public:
	void NativeConstruct() override;
	void NativeDestruct() override;
	void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;
	void SetScrollItemData(int32 Index, URScrollItemData* item, URScrollView* ScrollView) override;
	UFUNCTION()
	void MemberKick();
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UGuildManagerListScrollItem)
	ULocalizingTextBlock* TextBlockLevel;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UGuildManagerListScrollItem)
	ULocalizingTextBlock* TextBlockName;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UGuildManagerListScrollItem)
	ULocalizingTextBlock* TextBlockContribute;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UGuildManagerListScrollItem)
	UWidgetSwitcher*	  backgroundSwitcher;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UGuildManagerListScrollItem)
	UHorizontalBox*		  GradeBox;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UGuildManagerListScrollItem)
	UHorizontalBox*		  BanishBox;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UGuildManagerListScrollItem)
	UWidgetSwitcher*	  GradeIconSwitcher;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UGuildManagerListScrollItem)
	UUC_ScrollItem_AvatarIcon* AvatarIcon;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UGuildManagerListScrollItem)
	UTextBlock*			  TextBlockDay;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UGuildManagerListScrollItem)
	ULocalizingTextBlock* LocalizingTextBlockDay;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UGuildManagerListScrollItem)
	UTextBlock*			  TextBlockHour;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UGuildManagerListScrollItem)
	ULocalizingTextBlock* LocalizingTextBlockHour;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UGuildManagerListScrollItem)
	UTextBlock*			  TextBlockMinute;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UGuildManagerListScrollItem)
	ULocalizingTextBlock* LocalizingTextBlockMinute;
	UPROPERTY()
		UButton* Btn_Banish;
	UPROPERTY()
		UButton* BtnGrade;
	UFUNCTION(BlueprintCallable, Category = UGuildManagerListScrollItem)
	void OnClick_Grade();
	UFUNCTION(BlueprintCallable, Category = UGuildManagerListScrollItem)
	void OnClick_Kick();
public:
	UPROPERTY()
	FGUILD_MEMBER	GuildMemberData;

// public:
// 	//UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UUP_Guild_Management)
// 	FGUILD_MEMBER	GuildMamberData;
// 	//UPROPERTY()
// 	EGUILD_GRADE	MyGuildGrade;
// 	//UPROPERTY()
// 	int32			MyAccountKid;
// 	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = GuildManagerListScrollItem)
// 	UCanvasPanel*	PopupPanel = nullptr;
// 	
// 	UFUNCTION(BlueprintCallable, meta = (HidePin = "WorldContextObject", DefaultToSelf = "WorldContextObject"), Category = GuildManagerListScrollItem)
// 	void SetGuildMemberData(UObject* WorldContextObject, UTextBlock* MemberName, UTextBlock* MemberLevel, UTextBlock* MemberContribute, UTextBlock* Text_Hour, UTextBlock* Text_HourWord);
// 	UFUNCTION(BlueprintCallable, meta = (HidePin = "WorldContextObject", DefaultToSelf = "WorldContextObject"), Category = GuildManagerListScrollItem)
// 	void SetGuildMemberButton(UObject* WorldContextObject, UButton* btnGrade, UButton* MemberKick);
// 
// 	UFUNCTION(BlueprintCallable, meta = (HidePin = "WorldContextObject", DefaultToSelf = "WorldContextObject"), Category = GuildManagerListScrollItem)
// 	void OnClick_SetVisiblePanel(UObject* WorldContextObject, UCanvasPanel* Panel, ESlateVisibility PanelVisibility);
// 	void SetVisiblePanel(ESlateVisibility PanelVisibility);
// 
// 	UFUNCTION(BlueprintCallable, meta = (HidePin = "WorldContextObject", DefaultToSelf = "WorldContextObject"), Category = GuildManagerListScrollItem)
// 	void OnClick_Grade(UObject* WorldContextObject);
// 
// 	UFUNCTION(BlueprintCallable, meta = (HidePin = "WorldContextObject", DefaultToSelf = "WorldContextObject"), Category = GuildManagerListScrollItem)
// 	void OnClick_Kick(UObject* WorldContextObject);
// 	UFUNCTION(BlueprintCallable, meta = (HidePin = "WorldContextObject", DefaultToSelf = "WorldContextObject"), Category = UGuildManager_ScrollItemData)
// 	void OnClick_GradeUp(UObject* WorldContextObject);
// 	UFUNCTION(BlueprintCallable, meta = (HidePin = "WorldContextObject", DefaultToSelf = "WorldContextObject"), Category = UGuildManager_ScrollItemData)
// 	void OnClick_GradeDown(UObject* WorldContextObject);
// 	UFUNCTION(BlueprintCallable, meta = (HidePin = "WorldContextObject", DefaultToSelf = "WorldContextObject"), Category = UGuildManager_ScrollItemData)
// 	void OnClick_Kick(UObject* WorldContextObject);
// 	UFUNCTION()
// 	void MemberKick();
// 	UFUNCTION(BlueprintCallable, meta = (HidePin = "WorldContextObject", DefaultToSelf = "WorldContextObject"), Category = UGuildManager_ScrollItemData)
// 	void OnClick_ChangeMaster(UObject* WorldContextObject);
// 	UFUNCTION()
// 	void ChangeMaster();
// 	UFUNCTION(BlueprintCallable, meta = (HidePin = "WorldContextObject", DefaultToSelf = "WorldContextObject"), Category = UGuildManager_ScrollItemData)
// 	void OnClick_AddFriend(UObject* WorldContextObject);
// 	UFUNCTION(BlueprintCallable, meta = (HidePin = "WorldContextObject", DefaultToSelf = "WorldContextObject"), Category = UGuildManager_ScrollItemData)
// 	void OnClick_View(UObject* WorldContextObject);
// 	UFUNCTION(BlueprintCallable, meta = (HidePin = "WorldContextObject", DefaultToSelf = "WorldContextObject"), Category = UGuildManager_ScrollItemData)
// 	void OnClick_Mail(UObject* WorldContextObject);
};