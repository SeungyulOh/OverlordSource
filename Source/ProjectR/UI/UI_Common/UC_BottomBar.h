// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "UI/RBaseWidget.h"
#include "UI/UIDefs.h"
#include "UC_BottomBar.generated.h"

class UUC_Notify;
class URGameInstance;
/**
 * 
 */
UCLASS()
class PROJECTR_API UUC_BottomBar : public URBaseWidget
{
	GENERATED_BODY()
	
public:
	void NativeConstruct() override;
	
	void SetLobbyCharacter(FName InLobbyCharacterKey);

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UC_BottomBar|Ref")
	UButton*				Button_HeroInfo = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UC_BottomBar|Ref")
	UImage*					Image_HeroJob = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UC_BottomBar|Ref")
	UTextBlock*				Text_HeroName = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UC_BottomBar|Ref")
	UTextBlock*				Text_HeroNick = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UC_BottomBar|Ref")
	UHorizontalBox*			Panel_GotoHero = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UC_BottomBar|Ref")
	UCanvasPanel*			Panel_Noti = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UC_BottomBar|Ref")
	UUC_Notify*				UC_Notify_New_Hero = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UC_BottomBar|Ref")
	UButton*				Button_Daily = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UC_BottomBar|Ref")
	UButton*				Button_Achievement = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UC_BottomBar|Ref")
	UButton*				Button_Friend = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UC_BottomBar|Ref")
	UButton*				Button_Item = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UC_BottomBar|Ref")
	UButton*				Button_Grind = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UC_BottomBar|Ref")
	UButton*				Btn_HeroManager = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UC_BottomBar|Ref")
	UButton*				Btn_ItemCraft = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UC_BottomBar|Ref")
	UButton*				Btn_Guild = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UC_BottomBar|Ref")
	UButton*				Button_Quest = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UC_BottomBar|Ref")
	UButton*				Button_QuestGuide = nullptr;

private:
	UFUNCTION()
	void OnClickHeroInfo();

	UFUNCTION()
	void OnClickDaily();

	UFUNCTION()
	void OnClickAchievement();

	UFUNCTION()
	void OnClickFriend();

	UFUNCTION()
	void OnClickItem();

	UFUNCTION()
	void OnClickGrind();

	UFUNCTION()
	void OnClickHeroManager();

	UFUNCTION()
	void OnClickItemManufacture();

	UFUNCTION()
	void OnClickGuild();

	UFUNCTION()
	void OnClickQuestGuide();

	UFUNCTION()
	void OnClickQuest();

private:
	UPROPERTY()
	URGameInstance* RGameInstance = nullptr;

	UPROPERTY()
	FName HeroId;
	int32 HeroIdx = 0;
	bool isCheckedQuest = false;
};