// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Blueprint/UserWidget.h"
#include "Network/Chat/ChatManager.h"
#include "UC_Chat.h"
#include "UC_ChatItem.h"
#include "UC_Chat_Ingame.generated.h"

class FVirtualKeyboardEntry: public IVirtualKeyboardEntry
{

public:
	static TSharedRef<FVirtualKeyboardEntry> Create(class UUC_Chat* InOwner, const FText& InHintT);

	virtual void SetTextFromVirtualKeyboard(const FText& InNewText, ETextEntryType TextEntryType) override;
	virtual FText GetText() const override;
	virtual FText GetHintText() const override;
	virtual EKeyboardType GetVirtualKeyboardType() const override;
	virtual bool IsMultilineEntry() const override;

	class UUC_Chat* Owner = nullptr; // Assuming this is accessed on the main thread only

	FText HintText;
private:
	FVirtualKeyboardEntry(class UUC_Chat* InOwner, const FText& InHintT);
};

/**
 * 
 */
 UCLASS(Config=Game)
class PROJECTR_API UUC_Chat_Ingame : public UUC_Chat
{
	GENERATED_BODY()
	
public:

	void NativeConstruct() override;
	void NativeDestruct() override;

	/*
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Chat)
	class UUC_ChatItem_Ingame* ChatItemView_Last;
	*/
	UFUNCTION(BlueprintCallable, Category = Chat)
	void InputUserMessageVirtualKeyboard();

	UFUNCTION(BlueprintImplementableEvent, Category = Chat)
	void SetCurrentChannelMenu(const FText& ChannelName);


	// TODO super inefficient
	void InvalidateNative() override;

	void InvalidateInputCooldown();

	void OnInputCooldownChange(bool bOnCooldown, int RemainingSeconds) override;

	UFUNCTION(BlueprintImplementableEvent, Category = Chat)
	void SetInputCooldown(bool bOnCooldown, int RemainingSeconds);

	UPROPERTY(Config, BlueprintReadWrite, Category = Chat)
	bool bMinimized = false;

	virtual FText GetChatListName(EChatListEnum ChatListEnum) const override;

	UFUNCTION()
	void OnClick_Show();
	UFUNCTION()
	void OnClick_Hide();

public:
	UPROPERTY()
	UButton* Button_Show;
	UPROPERTY()
	UButton* Button_Hide;
	UPROPERTY()
	UWidgetSwitcher* WidgetSwitcher_Chat;

private:

	TSharedPtr<FVirtualKeyboardEntry> VirtualKeyboardEntry;
};
