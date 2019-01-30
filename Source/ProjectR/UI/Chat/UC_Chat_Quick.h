// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "UI/RBaseWidget.h"
#include "UC_Chat_Quick.generated.h"

UCLASS(Config=Game)
class PROJECTR_API UUC_Chat_Quick : public URBaseWidget
{
	GENERATED_BODY()
	
public:

	void NativeConstruct() override;
	void NativeDestruct() override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Chat)
	TArray<class UEditableTextBox*> TextBox_Inputs;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Chat)
	TArray<class UButton*> Buttons;

	UFUNCTION()
	void OnTextCommitted(const FText& Text, ETextCommit::Type CommitMethod);

	void RefreshButtons();

	UFUNCTION(BlueprintImplementableEvent, Category = Chat)
	void SendMessage(const FString& Message);

	UFUNCTION(BlueprintCallable, Category = Chat)
	void Save();

	UPROPERTY(Config)
	TArray<FString> Messages;

private:
};
