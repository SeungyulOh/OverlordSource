// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "UI/RBaseWidget.h"
#include "UI/UIDefs.h"
#include "Network/PacketDataStructures.h"
#include "ContentGameMode/Controllers/LobbyPlayerController.h"
#include "UC_LobbyInvite_Popup.h"
#include "UP_Lobby_Indoor.generated.h"

class UButton;
class AIndoorLobbyStateActor;
class ALobbyPlayerController;
class UNPCSystemMessage;


/**
 * 
 */
UCLASS()
class PROJECTR_API UUP_Lobby_Indoor : public URBaseWidget
{
	GENERATED_BODY()	
	
public:

	void NativeConstruct() override;
	void NativeDestruct() override;
	
	//// BP Widgets

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UUP_Lobby_Indoor)
	UOverlay* Chat_Minimalize_Overlay;

// 	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UUP_Lobby_Indoor)
// 	UUC_LobbyInvite_Popup* Invite_Popup_Indoor;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UUP_Lobby_Indoor)
	UUserWidget* UC_Lobby_MenuBasic;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UUP_Lobby_Indoor)
	UUserWidget* UC_Lobby_MenuOpen;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UUP_Lobby_Indoor)
	UButton* Button_OutTransition;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UUP_Lobby_Indoor)
	UWidgetSwitcher* WidgetSwitcher_DropBox = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UUP_Lobby_Indoor)
	UButton* Button_DropDown = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UUP_Lobby_Indoor)
	UButton* Button_Dropup = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UUP_Lobby_Indoor)
		UButton* Button_CSGrade = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UUP_Lobby_Indoor)
		ULocalizingTextBlock* CurrentCSGrade;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UUP_Lobby_Indoor)
	UTextBlock* ActivateFloor_Close_TB = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UUP_Lobby_Indoor)
	UTextBlock* ActivateFloor_Open_TB = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UUP_Lobby_Indoor)
	UTextBlock* ElevatedFloor_Open_TB = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UUP_Lobby_Indoor)
	UTextBlock* CurrentFloor_Open_TB = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UUP_Lobby_Indoor)
	UWidgetAnimation * OutmenuAnim = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UUP_Lobby_Indoor)
	class UUC_HeroStar* HeroStar = nullptr;

// 	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UUP_Lobby_Indoor)
// 		UButton * Button_Quest_Fold = nullptr;

	//
// 	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UUP_Lobby_Indoor)
// 	UWidgetSwitcher* WidgetSwitcher_LineLength = nullptr;
// 	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UUP_Lobby_Indoor)
// 	ULocalizingTextBlock* Text_NickName = nullptr;
// 	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UUP_Lobby_Indoor)
// 	ULocalizingTextBlock* Text_Message = nullptr;
// 	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UUP_Lobby_Indoor)
// 	ULocalizingTextBlock* Text_NickNameLong = nullptr;
// 	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UUP_Lobby_Indoor)
// 	ULocalizingTextBlock* Text_MessageLong_1 = nullptr;
// 	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UUP_Lobby_Indoor)
// 	ULocalizingTextBlock* Text_MessageLong_2 = nullptr;


	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UUP_Lobby_Indoor)
	UWidgetSwitcher*			WidgetSwitcherHeroStarImage = nullptr;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UUP_Lobby_Indoor)
	TArray<UImage*>				StarImageList;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UUP_Lobby_Indoor)
	ULocalizingTextBlock*		Text_HeroName = nullptr;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UUP_Lobby_Indoor)
	UImage*						Image_Job = nullptr;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UUP_Lobby_Indoor)
		UUC_Notify*					UC_Notify = nullptr;
// 	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UUP_Lobby_Indoor)
// 	UButton*		Button_FloorShop = nullptr;
// 	
// 	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UUP_Lobby_Indoor)
// 	UButton*		Button_FloorRaid = nullptr;
// 	
// 	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UUP_Lobby_Indoor)
// 	UButton*		Button_Adventure = nullptr;
	

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UUP_Lobby_Indoor)
	UOverlay*					ImportantMissionOverlay = nullptr;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UUP_Lobby_Indoor)
	URRichTextBlock*			ImportantMissionTargetText = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UUP_Lobby_Indoor)
	ULocalizingTextBlock*		ImportantCurrentText = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UUP_Lobby_Indoor)
	ULocalizingTextBlock*		ImportantMaxText = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UUP_Lobby_Indoor)
	UButton*					GotoMissionContentsButton = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UUP_Lobby_Indoor)
	UWidgetSwitcher*			MenuOpenButtonSwitcher = nullptr;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UUP_Lobby_Indoor)
	UButton*					Button_MenuOpen = nullptr;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UUP_Lobby_Indoor)
	UButton*					Button_MenuClose = nullptr;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UUP_Lobby_Indoor)
	UButton*					Button_NPC_Speech = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UUP_Lobby_Indoor)
	UButton*					Button_HeroShop = nullptr;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UUP_Lobby_Indoor)
	UButton*					Button_Bag= nullptr;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UUP_Lobby_Indoor)
	UButton*					Button_HeroManagement= nullptr;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UUP_Lobby_Indoor)
	UNPCSystemMessage*			NPCMessage;

	//// Delegates
	UFUNCTION(BlueprintCallable)
	void OnClickTutorial();

	UFUNCTION()
	void OnClickMenuOpen();
	UFUNCTION()
	void OnClickMenuClose();
	UFUNCTION()
	void OnClickNPCSpeechBtn();
	UFUNCTION()
		void OnClickedCSGrade();
	UFUNCTION()
	void OnClickHeroShop();
	UFUNCTION()
	void OnClickBag();
	UFUNCTION()
	void OnClickHeroManagement();


	UFUNCTION()
	void OnButtonOutTransitionClicked();

	UFUNCTION()
	void OnButtonDropDownClicked();
	
	UFUNCTION()
	void OnButtonDropUpClicked();

// 	UFUNCTION()
// 	void OnClickFloorShopButton();
// 
// 	UFUNCTION()
// 	void OnClickFloorRaidButton();
// 
// 	UFUNCTION()
// 	void OnClickAdventureButton();

	UFUNCTION()
	void SetFloorInfo();

	UFUNCTION()
	void OutMenuAnimPlay(bool reverse);

	UFUNCTION()
	void SetMinimalizeOverlay();

	UFUNCTION()
	void ChatMinimalizeOnOff(bool isOn);

	UFUNCTION()
	void UpdateMessage(FString nick, FString message);

	UFUNCTION()
	void OnClickGotoMissionContents();

	UFUNCTION()
	void OnClickFloorMissionButton();
	
	UFUNCTION()
	void GoCampaign();
	
	void SetHeroText();

	void SetLobbyHeroGrade(int32 InGrade);

	void SetImportantMissionInfo();

	void SetWelcomeText();

	bool m_reverse = false;

};
