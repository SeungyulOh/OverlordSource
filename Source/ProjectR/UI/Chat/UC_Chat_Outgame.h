// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Blueprint/UserWidget.h"
#include "UI/Component/CheckBox_Radio.h"
#include "UC_ChatItem.h"
#include "UC_Chat.h"
#include "UC_ChangeChannelPopup.h"
#include "UI/Component/CanvasPanel_RadioGroup.h"
#include "UI/Control/RScrollView.h"
#include "GlobalConstants.h"
#include "UC_Chat_Outgame.generated.h"


/**
 * 
 */
 UCLASS(BlueprintType)
class PROJECTR_API UUC_Chat_Outgame : public UUC_Chat
{
	GENERATED_BODY()
	
public:


	void NativeConstruct() override;
	void NativeDestruct() override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Chat)
		UCheckBox_Radio* CheckBox_Radio_Tab_General = nullptr;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Chat)
		UWidgetSwitcher* WidgetSwitcher_Tab_General = nullptr;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Chat)
		UCheckBox_Radio* CheckBox_Radio_Tab_Guild = nullptr;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Chat)
		UWidgetSwitcher* WidgetSwitcher_Tab_Guild = nullptr;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Chat)
		UCheckBox_Radio* CheckBox_Radio_Tab_Party = nullptr;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Chat)
		UWidgetSwitcher* WidgetSwitcher_Tab_Party = nullptr;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Chat)
		UCheckBox_Radio* CheckBox_Radio_Tab_Invite = nullptr;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Chat)
		UWidgetSwitcher* WidgetSwitcher_Tab_Invite = nullptr;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Chat)
		UEditableTextBox* BP_TextBox_Input = nullptr;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Chat)
		UButton* Button_Send = nullptr;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Chat)
		UButton* Button_ChangeChannel = nullptr;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Chat)
		UButton* Button_Setting = nullptr;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Chat)
		UButton* Button_Close = nullptr;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Chat)
		class URChatScrollView* Chat_ScrollView = nullptr;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Chat)
		UCanvasPanel_RadioGroup*	BP_CanvasPanel_RadioGroup_Tabs = nullptr;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Chat)
		UUC_ChangeChannelPopup*	UC_Chatting_ChangeChannel = nullptr;

	UFUNCTION()
		void OnCheckRadioCheck_General(bool bIsChecked);
	UFUNCTION()
		void OnCheckRadioCheck_Party(bool bIsChecked);
	UFUNCTION()
		void OnCheckRadioCheck_Guild(bool bIsChecked);
	UFUNCTION()
		void OnCheckRadioCheck_Invite(bool bIsChecked);
	UFUNCTION()
		void GoTo_InviteTab();
// 	UFUNCTION()
// 		void OnCheckRadioCheck_Battle(bool bIsChecked);

	UFUNCTION(BlueprintCallable, Category = Chat)
		void OnClickChangeChannel();
	UFUNCTION(BlueprintCallable, Category = Chat)
		void OnClickSendButton();
	UFUNCTION(BlueprintCallable, Category = Chat)
		void OnClickCloseButton();
	UFUNCTION()
		void OnCheckRadioLockEvent_Party();
	UFUNCTION()
		void OnCheckRadioLockEvent_Guild();
	//receive event signal
	UFUNCTION()
		void Receive_ChannelData(FString channel);
	UFUNCTION()
		void Receive_MessageData();
	UFUNCTION()
		void Receive_UserInOut();

	UFUNCTION(BlueprintImplementableEvent, Category = Chat)
	void SetGeneralChannelName(const FText& ChannelName);

	UFUNCTION(BlueprintCallable, Category = Chat)
	void OnClickInputTarget(EChatListEnum ChatListEnum);

	UFUNCTION(BlueprintImplementableEvent, Category = Chat)
	void SetInputTargetName(EChatListEnum ChatListEnum, const FText& InputTargetName);

	UFUNCTION(BlueprintImplementableEvent, Category = Chat)
	void SetInputTargetEnabled(EChatListEnum ChatListEnum, bool bEnabled);

	UFUNCTION(BlueprintImplementableEvent, Category = Chat)
	void SetCurrentInputTargetName(const FText& InputTargetName);

	// Lineage Revolution also uses UEditableTextBox. Will not try to do better than them.
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Chat)
	class UEditableTextBox* TextBox_Input = nullptr;

	UFUNCTION(BlueprintCallable, Category = Chat)
	void InputUserMessageTargetInput(FString Message) override;

	UFUNCTION(BlueprintCallable, Category = Chat)
	void OnTextBoxChanged(const FText& InText);

	UFUNCTION(BlueprintCallable, Category = Chat)
	void OnTextBoxCommitted(const FText& InText, ETextCommit::Type CommitMethod);



	// TODO super inefficient
	void InvalidateNative() override;

	void InvalidateTabs();
	void InvalidateInputTargets();

	void InvalidateInputCooldown();

	void OnInputCooldownChange(bool bOnCooldown, int RemainingSeconds) override;
//	void SetScrollItemCntInit();
	void BindChatData();
	void BindInviteData();

	EChatListEnum eTabType = EChatListEnum::VE_GENERAL;

private:
	void OnActualCommitted(FString Message);

	// GENERAL, BATTLE, PARTY, GUILD, ?
	TArray<EChatListEnum> InputChatLists;
	//cached scrollitemdata list
	UPROPERTY()
		TArray<UUC_ChatItemData*> cachedChatItemDataList;
	UPROPERTY()
		TArray<UUC_ChatItemData*> cachedInviteItemDataList;

	const int32 MAXSCROLLITEM = 11;
};
