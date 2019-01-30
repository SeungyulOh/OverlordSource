// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "UI/RBaseWidget.h"
#include "UI/UIDefs.h"
#include "Network/PacketDataStructures.h"
#include "UC_LobbyList.generated.h"

class ULocalizingTextBlock;
class UUC_Notify;
class URGameInstance;
/**
 * 
 */
UCLASS()
class PROJECTR_API UUC_LobbyList : public URBaseWidget
{
	GENERATED_BODY()	
	
public:
	void NativeConstruct() override;
	void NativeDestruct() override;

	
	void SetState(EUIMenuIDEnum InUIMenuID);

	
public:
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "UC_LobbyList|Ref")
	UImage*					Image_Icon = nullptr;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "UC_LobbyList|Ref")
	UTextBlock*				Text_RestTime = nullptr;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "UC_LobbyList|Ref")
	ULocalizingTextBlock*	LText_Title = nullptr;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "UC_LobbyList|Ref")
	ULocalizingTextBlock*	LText_Desc = nullptr;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "UC_LobbyList|Ref")
	UButton*				Button_Click = nullptr;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "UC_LobbyList|Ref")
	UUC_Notify*				UC_Notify = nullptr;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "UC_LobbyList|Ref")
	UOverlay*				Overlay_Time = nullptr;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "UC_LobbyList|Ref")
	UTextBlock*				Text_UnLockLevel = nullptr;


private:
	UFUNCTION()
	void OnClickButton();

	bool CheckContentsUnLock();

private:
	UPROPERTY()
	EUIMenuIDEnum	UIMenuID = EUIMenuIDEnum::VE_Max;
	UPROPERTY()
	EOVERLORD		ContentsUnlockKey = EOVERLORD::TUTORIAL; //none...

	UPROPERTY()
	FTimerHandle	Handler_RestTime;

	UPROPERTY()
	URGameInstance* RGameInstance = nullptr;

	bool			bIsLockContents = false;
	bool			IsPacketSend = false;
};