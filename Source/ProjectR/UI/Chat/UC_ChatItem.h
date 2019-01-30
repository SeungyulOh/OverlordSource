// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "UI/RBaseWidget.h"
#include "Network/Chat/ChatList.h"
#include "UI/Control/RRichTextBlock.h"
#include "UI/Control/RScrollItem.h"
#include "Network/Chat/ChatManager.h"
#include "UC_ChatItem.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FDelegate_ClickPlayer);

/**
Dispense
 * ItemType
 * Who
 * Mine
 * Message (escaped and formatted)
 * formatted time
 */
UCLASS()
class PROJECTR_API UUC_ChatItemData : public URScrollItemData
{
	GENERATED_BODY()

public:
	EChatListEnum currTab;
	FChatItem itemData;
	FInviteData InviteData;

	UPROPERTY(BlueprintAssignable, BlueprintCallable, Category = Chat)
		FDelegate_ClickPlayer OnClickPlayer;
};

UCLASS()
class PROJECTR_API UUC_ChatItem : public URScrollItem
{
	GENERATED_BODY()
	
public:

	void NativeConstruct() override;
	void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;
	virtual void InvalidateData() override;

	UFUNCTION(BlueprintCallable, Category = Chat)
		virtual void InvalidateNative();

	UPROPERTY()
		USizeBox* Button_SizeBox;

	UPROPERTY()
		UOverlay* Overlay_ScrollItem;

	UPROPERTY()
		URRichTextBlock* Text_Title;

	UPROPERTY()
		UTextBlock* Text_Message;

	UPROPERTY()
		URRichTextBlock* Text_Timestamp;

	UPROPERTY()
		UButton* Button_Invite;

	UPROPERTY()
		UButton* Button_Goto;

	UPROPERTY()
		USizeBox* EntranceSizeBox;

	UFUNCTION()
		void OnClick_Invite();
	UFUNCTION()
		void OnClick_GoTo();

	UFUNCTION(BlueprintCallable, Category = Chat)
	EChatItemUIType GetItemUIType() const;

	UFUNCTION(BlueprintCallable, Category = Chat)
	FString GetWho() const;

	UFUNCTION(BlueprintCallable, Category = Chat)
	FText GetMessage() const;

	UFUNCTION(BlueprintCallable, Category = Chat)
	FString GetTimestamp() const;

// 	UFUNCTION(BlueprintCallable, Category = Chat)
// 	FChatUser GetChatUser() const;

	UFUNCTION(BlueprintCallable, Category = Chat)
	void OnClick_Name();

	UFUNCTION(BlueprintCallable, Category = Chat)
		void SetNickName(FText setText);

	UFUNCTION(BlueprintCallable, Category = Chat)
		void SetColor(FColor setColor);

	UFUNCTION(BlueprintCallable, Category = Chat)
		void SetMessage(FText setText);

	UFUNCTION()
		void ItemSizeChangedByTextSize();

	UFUNCTION()
		void SetSizeChange(FVector2D ChangeSize, bool bParentUpdate);
	// TODO: Action Menu on player

	//// Backend
	UFUNCTION(BlueprintCallable, Category = Chat)
	void SetChatItem(const FChatItem& InChatItem);

	UFUNCTION(BlueprintCallable, Category = ScrollItem)
		virtual void		SetScrollItemData(int32 Index, URScrollItemData* item, URScrollView* ScrollView) override;

	UFUNCTION()
		virtual void		SetItemData(int32 Index, URScrollItemData* item, URScrollView* ScrollView) override;

	UFUNCTION()
		virtual void		SetSelected(bool bSelected) override;

protected:
	FChatItem ChatItem;

private:
	class URScrollView* ParentScrollView = nullptr;
	FGeometry cachedGeo;
	FVector2D messageSize;
};

UCLASS()
class PROJECTR_API UUC_InviteItemData : public URScrollItemData
{
	GENERATED_BODY()

public:
	FInviteData InviteData;
};

UCLASS()
class PROJECTR_API UUC_InviteItem : public URScrollItem
{
	GENERATED_BODY()

public:
	void NativeConstruct() override;
	virtual void InvalidateData() override;

	UFUNCTION()
		void OnClick_AcceptInvite();

	UPROPERTY()
		URRichTextBlock* Text_Title;

	UPROPERTY()
		UTextBlock* Text_Message;

	UPROPERTY()
		URRichTextBlock* Text_Timestamp;

	UPROPERTY()
		UButton* Button_Entrance;
};