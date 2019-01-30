// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "UI/RBaseWidget.h"
#include "UC_ChangeChannelPopup.generated.h"

/**
 * Directly interact with ChatManager
 */
UCLASS()
class PROJECTR_API UUC_ChangeChannelPopup : public URBaseWidget
{
	GENERATED_BODY()
public:

	void NativeConstruct() override;
	void NativeDestruct() override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Chat)
	UEditableTextBox*		BP_TextBox_Input;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Chat)
		UButton*		Button_Cancel;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Chat)
		UButton*		Button_OK;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Chat)
		UButton*		Button_Close;
		
	// Return of true means that the popup can be closed/finished
	UFUNCTION(BlueprintCallable, Category = Chat)
	void OnClickOk();
	UFUNCTION(BlueprintCallable, Category = Chat)
	void OnClickCancel();
	UFUNCTION(BlueprintCallable, Category = Chat)
		void OnClickClose();
	
};
