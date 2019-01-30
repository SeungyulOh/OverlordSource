// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "UI/RBaseWidget.h"
#include "UC_Guild_RuneSupport.generated.h"

class URScrollView;
class UUC_HeroManagement_HeroList;
/**
 * 
 */
UCLASS()
class PROJECTR_API UUC_Guild_RuneSupport : public URBaseWidget
{
	GENERATED_BODY()
	
public:
	virtual void			NativeConstruct()	override;
	virtual	void			NativeDestruct()	override;

	void OpenRuneSupport();
	UFUNCTION(BlueprintCallable, Category = UUC_Guild_RuneSupport)
	void CloseRuneSupport();

	UFUNCTION()
	void OnGuildRuneStatusRpDelegate();
	UFUNCTION()
	void OnClick_HeroIcon(FString InHeroUD);
	UFUNCTION()
	void GuildRuneRequest();

	void RefreshTimer();

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UUC_Guild_RuneSupport)
	URScrollView* ScrollView_RuneRequestList;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UUC_Guild_RuneSupport)
	UTextBlock*		Text_RemainHour;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UUC_Guild_RuneSupport)
	UTextBlock*		Text_RemainMinute;

	UPROPERTY()
	FName			SelectHeroID;
};
