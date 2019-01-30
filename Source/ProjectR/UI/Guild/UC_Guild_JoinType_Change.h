// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "UI/RBaseWidget.h"
#include "UC_Guild_JoinType_Change.generated.h"
/**
 * 
 */
UCLASS()
class PROJECTR_API UUC_Guild_JoinType_Change : public URBaseWidget
{
	GENERATED_BODY()
	
public:
	void NativeConstruct() override;
	void NativeDestruct() override;

	UFUNCTION(BlueprintCallable, Category = UUC_Guild_JoinType_Change)
	void OnClickJoinTypeFree();

	UFUNCTION(BlueprintCallable, Category = UUC_Guild_JoinType_Change)
	void OnClickJoinTypeAccept();

	UFUNCTION(BlueprintCallable, Category = UUC_Guild_JoinType_Change)
	void OnClickOK();

	UFUNCTION()
	void OpenJoinType(uint8 JoinType);
	
	UFUNCTION(BlueprintCallable, Category = UUC_Guild_JoinType_Change)
	void CloseJoinType();

	UFUNCTION() 
	void SendGuildAutoGradeUp();

	UFUNCTION()
	void RefreshSelectImage(uint8 JoinType);
public:
	UPROPERTY()
		UButton* Button_Free;
	UPROPERTY()
		UButton* Button_Accept;
	UPROPERTY()
		UButton* Button_Cancel;
	UPROPERTY()
		UButton* Button_OK;
	UPROPERTY()
	TArray<UImage*>				SelectImageList;

	uint8 SelectJoinType;
};
