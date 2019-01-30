// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "UI/RBaseWidget.h"
#include "BaseSubPage.h"
#include "UI/UIDefs.h"
#include "UP_CreateNickName.generated.h"

class UCheckBox_Radio;
/**
 * 
 */
UCLASS()
class PROJECTR_API UUP_CreateNickName : public UBaseSubPage
{
	GENERATED_BODY()
	
public:
	void NativeConstruct() override;
	void NativeDestruct() override;

	UFUNCTION(BlueprintCallable, Category = UP_CreateNickName)
	void InValidNickName(bool bInSuccess);

public:
	//UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UP_CreateNickName|Ref")
	//UEditableTextBox*		EditText_NickName = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UP_CreateNickName|Ref")
	UEditableText*			EditText_NickName = nullptr;

protected:
	UFUNCTION()
	virtual void OnButton_Close() override;


	UFUNCTION()
	void Refresh() override;

	UFUNCTION()
	virtual void OnButton_Click() override;

	virtual void CallNextPage() override;

private:
	bool ExceptionLengthLimit();
	bool ExceptionSlang();
	bool ExceptionOverlapNickName();

	void CallProcess();
	UFUNCTION()
	bool CheckProcess();

	


private:
	
	TArray<bool> CheckState;

	int32 UseTargetNum = 0;

	UPROPERTY()
	FString TargetString;
};
