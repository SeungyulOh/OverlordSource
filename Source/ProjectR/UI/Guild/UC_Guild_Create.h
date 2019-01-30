// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "UI/RBaseWidget.h"
#include "UC_Guild_Create.generated.h"
/**
 * 
 */
class UUC_CurrencyDisplay;
UCLASS()
class PROJECTR_API UUC_Guild_Create : public URBaseWidget
{
	GENERATED_BODY()
	
public:
	virtual void			NativeConstruct()	override;
	virtual	void			NativeDestruct()	override;

// 	void OpenGrade(uint8 MyGuildGrade, int32 kid, const FString& nick, uint8 GuildGrade);
// 	
// 	UFUNCTION(BlueprintCallable, Category = UUC_Guild_Create)
// 	void CloseGrade();
// 
// 	UFUNCTION(BlueprintCallable, Category = UUC_Guild_Create)
// 	void OnClickGrade(uint8 grade);
// 
// 	UFUNCTION(BlueprintCallable, Category = UUC_Guild_Create)
// 	void OnClickGradeChange();

public:
	UFUNCTION()
	void Open();
	UFUNCTION()
	void Close();
	UFUNCTION()
	void Init();
	
	UFUNCTION(BlueprintCallable, Category = UUC_Guild_Create)
	void OnClickCancel();
	UFUNCTION(BlueprintCallable, Category = UUC_Guild_Create)
	void OnClickCreate();
	UFUNCTION(BlueprintCallable, Category = UUC_Guild_Create)
	void OnClickJoinType(int32 iAuto);

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UUC_Guild_Create)
	UMultiLineEditableText*	Create_GuildName;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UUC_Guild_Create)
	UMultiLineEditableText*	Create_GuildDesc;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UUC_Guild_Create)
	UCheckBox*				Create_JoinTypeAuto;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UUC_Guild_Create)
	UCheckBox*				Create_JoinTypeConfirm;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UUC_Guild_Create)
	ULocalizingTextBlock*	Create_GuildCreatePrice;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UUC_Guild_Create)
	UUC_CurrencyDisplay*	UC_CurrencyDisplay;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UUC_Guild_Create)
	UImage*					Create_Backscreen;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UUC_Guild_Create)
	UVerticalBox*			Create_PopupPanel;

	bool	Create_AutoGradeUp = true;
	int32	GuildCreateGold;
};
