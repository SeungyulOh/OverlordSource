// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Blueprint/UserWidget.h"
#include "Network/Chat/ChatManager.h"
#include "ChatScrollBox.h"
#include "UC_ChatItem.h"
#include "UI/Component/CanvasPanel_RadioGroup.h"
#include "UC_Chat.generated.h"

/**
 * 
 */
 UCLASS()
class PROJECTR_API UUC_Chat : public URBaseWidget
{
	GENERATED_BODY()
	
public:

	virtual void NativeConstruct() override;
	virtual void NativeDestruct() override;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Chat)
	class UChatScrollBox* ScrollBox_ChatList = nullptr;

	UFUNCTION(BlueprintImplementableEvent, Category = Chat)
	void OnClickPlayerName(FChatUser ChatUser);

	UFUNCTION(BlueprintImplementableEvent, Category = Chat)
	void SetChannelEnabled(EChatListEnum ChatListEnum, bool bEnabled);

	// delegate
	UFUNCTION()
	void OnUserScrolledToEnd();

	// Tabs
//  	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Chat)
//  	UCanvasPanel_RadioGroup*	CanvasPanel_RadioGroup_Tabs;

	UFUNCTION(BlueprintCallable, Category = Chat)
	void OnClickTab(EChatListEnum ChatListEnum);

	void ShowPopupMessage(FString ContentString);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Chat)
	class UWidget* Notify_AdditionalMessage = nullptr;

	UFUNCTION(BlueprintCallable, Category = Chat)
	void InputUserMessage(FString Message, EChatListEnum ChatListEnum);

	UFUNCTION(BlueprintCallable, Category = Chat)
	virtual void InputUserMessageTargetInput(FString Message);

	UFUNCTION(BlueprintCallable, Category = Chat)
	void OnClickClose();

	UFUNCTION(BlueprintImplementableEvent, Category = Chat)
	void SetBattleChannelName(const FText& TabName);
	
	// Kakao
	UFUNCTION(BlueprintCallable, Category = Chat)
	void OnClickKakaoGuildChat();

	UFUNCTION(BlueprintImplementableEvent, Category = Chat)
	void Invalidate();

	// TODO super inefficient
	UFUNCTION(BlueprintCallable, Category = Chat)
	virtual void InvalidateNative();

	void InvalidateChatList();


	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Chat)
	TAssetSubclassOf<class UUC_ChatItem> ChatItemClass = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Chat)
	EChatListEnum ViewChatList = EChatListEnum::VE_GENERAL;


	UFUNCTION(BlueprintImplementableEvent, Category = Chat)
	void SetLastChatItem(const FChatItem& ChatItem);
	
	////// Backend interface - ChatManager

	// delegates
	UFUNCTION()
	void OnNewMessage(TArray<EChatListEnum> AffectedChatLists);

	UFUNCTION()
	void OnSubscriptionChange(TArray<EChatListEnum> AffectedChatLists);

	UFUNCTION()
	virtual void OnInputCooldownChange(bool bOnCooldown, int RemainingSeconds);

	// Error messages (Channel change failure because full)
	UFUNCTION()
	void OnPopupMessage(FString Message);

	//// Utilities for implementation classes
	// ?
	static FString FormatDateTime(const FDateTime& DateTime);

	UFUNCTION(BlueprintCallable, Category = Chat)
	FText MessageEnterAttemptInactiveChatList(EChatListEnum ChatListEnum);

	UFUNCTION(BlueprintCallable, Category = Chat)
	void IssueSystemMessageToCurrentChatList(FText Message);

	virtual FText GetChatListName(EChatListEnum ChatListEnum) const;

protected:
	UPROPERTY()
	class UChatManager* ChatManager = nullptr;

private:

	void UpdateScrollBox_ChatList();

	// false means to adjust scroll to bottom when a new message comes under
	bool bScrollLock = false;

	class UUC_ChatItem* GetChatItemView(const FChatItem& InChatItem);

	CHATID_T NextChatId = 0;

};
