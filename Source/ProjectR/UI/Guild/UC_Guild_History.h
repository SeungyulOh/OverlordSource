// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "UI/RBaseWidget.h"
#include "UC_Guild_History.generated.h"

class URScrollView;
/**
 * 
 */
UCLASS()
class PROJECTR_API UUC_Guild_History : public URBaseWidget
{
	GENERATED_BODY()
	
public:
	virtual void			NativeConstruct()	override;
	virtual	void			NativeDestruct()	override;

	void OpenHistory();
	UFUNCTION(BlueprintCallable, Category = UUC_Guild_Awaiter)
	void CloseHistory();

	UFUNCTION()
	void OnGuildHistoryRpDelegate();

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UUC_Guild_Awaiter)
	URScrollView* ScrollView_HistoryList;
};
