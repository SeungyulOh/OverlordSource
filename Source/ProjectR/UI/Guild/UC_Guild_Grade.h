// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "UI/RBaseWidget.h"
#include "UC_Guild_Grade.generated.h"
/**
 * 
 */
UCLASS()
class PROJECTR_API UUC_Guild_Grade : public URBaseWidget
{
	GENERATED_BODY()
	
public:
	virtual void			NativeConstruct()	override;
	virtual	void			NativeDestruct()	override;
	
	void OpenGrade(uint8 MyGuildGrade, int32 kid, const FString& nick);
	
	UFUNCTION(BlueprintCallable, Category = UUC_Guild_Grade)
	void CloseGrade();

	UFUNCTION(BlueprintCallable, Category = UUC_Guild_Grade)
	void OnClickGrade(uint8 grade);

	UFUNCTION(BlueprintCallable, Category = UUC_Guild_Grade)
	void OnClickGradeChange();
	UFUNCTION()
		void GradeChangeRp(FString _nick, int32 grade);
	UFUNCTION()
		void OnClick_MasterBtn();
	UFUNCTION()
		void OnClick_ElderBtn();
	UFUNCTION()
		void OnClick_MemberBtn();

protected:
	UFUNCTION() 
	void GradeTransfer();

	UFUNCTION()
	void RefreshSelectImage(uint8 grade);


public:
	UPROPERTY()
	UButton*					ButtonMaster;
	UPROPERTY()
	UButton*					ButtonElder;
	UPROPERTY()
	UButton*					ButtonMember;
	UPROPERTY()
		UButton* Button_OK;
	UPROPERTY()
		UButton* Button_Cancel;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UUC_Guild_Grade)
	ULocalizingTextBlock*		Text_Desc;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UUC_Guild_Grade)
	TArray<UImage*>				SelectImageList;
	uint8 CurrGrade;
	uint8 ChangeGrade;
	FString nick;
	int32 kid;
};
