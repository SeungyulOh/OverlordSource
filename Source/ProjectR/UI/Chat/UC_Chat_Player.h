// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "UI/RBaseWidget.h"
#include "Network/Chat/ChatManager.h"
#include "UC_Chat_Player.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTR_API UUC_Chat_Player : public URBaseWidget
{
	GENERATED_BODY()
	
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Chat)
	class UTextBlock* Text_Name = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Chat)
	class UButton* Button_Block = nullptr;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Chat)
	FChatUser ChatUser;

	void NativeConstruct() override;
	void NativeDestruct() override;

	UFUNCTION(BlueprintCallable, Category = Chat)
	void Invalidate();

	UFUNCTION(BlueprintCallable, Category = Chat)
	void BlockConfirm();

	UFUNCTION()
	void Block();

private:
	UPROPERTY()
	class UChatManager* ChatManager = nullptr;
};
