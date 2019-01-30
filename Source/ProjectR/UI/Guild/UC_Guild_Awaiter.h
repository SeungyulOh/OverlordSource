// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "UI/RBaseWidget.h"
#include "Network/PacketDataStructures.h"
#include "UC_Guild_Awaiter.generated.h"

class URScrollView;
/**
 * 
 */
UCLASS()
class PROJECTR_API UUC_Guild_Awaiter : public URBaseWidget
{
	GENERATED_BODY()
	
public:
	virtual void			NativeConstruct()	override;
	virtual	void			NativeDestruct()	override;

	void OpenAwaiter();
	UFUNCTION(BlueprintCallable, Category = UUC_Guild_Awaiter)
	void CloseAwaiter();

	UFUNCTION(BlueprintCallable, Category = UUC_Guild_Awaiter)
	void OnClick_AwaiterPrev();
	UFUNCTION(BlueprintCallable, Category = UUC_Guild_Awaiter)
	void OnClick_AwaiterNext();
	void RequestAwaiterList();

	UFUNCTION()
	void OnGuildAwaiterListRpDelegate(bool ApplyData);
	void SetScrollDataGuildAwaiterList();

public:
	UPROPERTY()
	URScrollView* ScrollView_AwaiterList;
	UPROPERTY()
	UButton*	Button_PrevAwaiterPage;
	UPROPERTY()
	UButton*	Button_NextAwaiterPage;
	UPROPERTY()
		UButton*	Button_Close;
	UPROPERTY()
		UTextBlock* Text_AwaiterPage;

	int32 CurrentPage;
	int32 RequestPage;

	UPROPERTY()
	TArray<FGUILD_AWAITER>	BackUpData;
};
