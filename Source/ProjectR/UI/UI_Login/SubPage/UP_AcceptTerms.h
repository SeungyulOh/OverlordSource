// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "UI/RBaseWidget.h"
#include "BaseSubPage.h"
#include "UI/UIDefs.h"
#include "UP_AcceptTerms.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTR_API UUP_AcceptTerms : public UBaseSubPage
{
	GENERATED_BODY()
	
public:
	void NativeConstruct() override;
	void NativeDestruct() override;

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "UP_AcceptTerms")
	void OnCloseEvent();

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "UP_AcceptTerms")
	void OnRefreshEvent();

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UP_AcceptTerms|Ref")
	UButton*				Button_Accept1 = nullptr;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UP_AcceptTerms|Ref")
	UHorizontalBox*			HBox_Accept1 = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UP_AcceptTerms|Ref")
	UButton*				Button_Accept2 = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UP_AcceptTerms|Ref")
	UHorizontalBox*			HBox_Accept2 = nullptr;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UP_AcceptTerms|Ref")
	UOverlay*				Overlay_Web;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UP_AcceptTerms|Ref")
	TAssetSubclassOf<UUserWidget> WebViewer;

	UPROPERTY()
	UUserWidget*	CachedWidget = nullptr;

private:
	void Refresh() override;

	virtual void CallNextPage() override;


	UFUNCTION()
	virtual void OnButton_Close() override;

	UFUNCTION()
	void OnButton_Accept1();

	UFUNCTION()
	void OnButton_Accept2();

private:
	void Internal_Init();
	void Internal_Accept(int32 InClickIndex);
	void AddWebViewer();
	void RemoveWebViewer();

private:
	bool bIsAccept1 = false;
	bool bIsAccept2 = false;

	FSlateColor AcceptColor;
	FSlateColor NoAcceptColor = FSlateColor(FLinearColor(0.1f, 0.1f, 0.1f));
};
