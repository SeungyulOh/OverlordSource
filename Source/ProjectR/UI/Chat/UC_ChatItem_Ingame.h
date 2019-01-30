// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "UC_ChatItem.h"
#include "Network/Chat/ChatList.h"
#include "Network/Chat/ChatManager.h"
#include "UC_ChatItem_Ingame.generated.h"

#define CHATITEM_INGAME_MINIMIZED_LIMIT_KO_KR 24
#define CHATITEM_INGAME_SYSTEM_MINIMIZED_LIMIT_KO_KR 16

#define CHATITEM_INGAME_ELLIPSIS TEXT("...")


UCLASS()
class PROJECTR_API UUC_ChatItem_Ingame : public UUC_ChatItem
{
	GENERATED_BODY()
	
public:
	void NativeConstruct() override;

	UFUNCTION(BlueprintCallable, Category = Chat)
	FText GetRichIngameMessage() const;

	void InvalidateNative() override;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = Chat)
	bool bMinimized = false;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = Chat)
	class URRichTextBlock* Text_Message_Normal = nullptr;


private:
	FText ApplyColorRichText(FText InText, FColor InColor) const;

	static FText GetPossiblyTruncatedText(FText Text, int Limit);

	FText RichIngameMessage;
};
