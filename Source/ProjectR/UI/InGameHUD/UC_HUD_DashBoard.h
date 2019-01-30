// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "UI/RBaseWidget.h"
#include "Network/RTSProtocol/rts.struct_generated.h"
#include "UC_HUD_DashBoard.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTR_API UUC_HUD_DashBoard : public URBaseWidget
{
	GENERATED_BODY()

public:
// 	void NativeConstruct() override;
// 	void NativeDestruct() override;
// 	bool Initialize();
// 	void PrepareWidget();
// 
// 	UFUNCTION()
// 	void OnShowDashBoard();
// 	
// 	UFUNCTION()
// 	void OnModRoomUpdateMedal();
// 
// 	UFUNCTION()
// 	void OnClick_Close();
// 
// 	void SetDashBoardMember(OVERLORD::RTS::STRUCT::PLAYER_SCORET& PlayerInfo, UPanelWidget* PanelRankList, UPanelWidget* PanelMember, UWidgetSwitcher* SwicherMyData,
// 		UTextBlock* MemberName, UTextBlock* MemberGuild, UTextBlock* MemberPoint, UTextBlock* MemberKillCount);
// 
// public:
// 	//Team Data
// 	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UUC_HUD_DashBoard)
// 	UTextBlock*				TextBlock_BlueTeamPouch = nullptr;
// 	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UUC_HUD_DashBoard)
// 	UTextBlock*				TextBlock_RedTeamPouch = nullptr;
// 
// 	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UUC_HUD_DashBoard)
// 	UTextBlock*				TextBlock_BlueTeamKillCount = nullptr;
// 	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UUC_HUD_DashBoard)
// 	UTextBlock*				TextBlock_RedTeamKillCount = nullptr;
// 
// 	//Member Data
// 	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UUC_HUD_DashBoard)
// 	TArray<UPanelWidget*>	Panel_BlueTeamRankList;
// 	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UUC_HUD_DashBoard)
// 	TArray<UPanelWidget*>	Panel_RedTeamRankList;
// 
// 	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UUC_HUD_DashBoard)
// 	TArray<UPanelWidget*>	Panel_BlueTeamList;
// 	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UUC_HUD_DashBoard)
// 	TArray<UPanelWidget*>	Panel_RedTeamList;
// 
// 	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UUC_HUD_DashBoard)
// 	TArray<UWidgetSwitcher*>	Swicher_BlueTeamMyDataList;
// 	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UUC_HUD_DashBoard)
// 	TArray<UWidgetSwitcher*>	Swicher_RedTeamMyDataList;
// 
// 	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UUC_HUD_DashBoard)
// 	TArray<UTextBlock*>		Text_BlueTeamNameList;
// 	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UUC_HUD_DashBoard)
// 	TArray<UTextBlock*>		Text_RedTeamNameList;
// 
// 	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UUC_HUD_DashBoard)
// 	TArray<UTextBlock*>		Text_BlueTeamGuildNameList;
// 	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UUC_HUD_DashBoard)
// 	TArray<UTextBlock*>		Text_RedTeamGuildNameList;
// 
// 	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UUC_HUD_DashBoard)
// 	TArray<UTextBlock*>		Text_BlueTeamPointList;
// 	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UUC_HUD_DashBoard)
// 	TArray<UTextBlock*>		Text_RedTeamPointList;
// 
// 	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UUC_HUD_DashBoard)
// 	TArray<UTextBlock*>		Text_BlueTeamKillCountList;
// 	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UUC_HUD_DashBoard)
// 	TArray<UTextBlock*>		Text_RedTeamKillCountList;
// 
// 	// My Data
// 	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UUC_HUD_DashBoard)
// 	UCanvasPanel*			CanvasPanel_BlueTeam_My = nullptr;
// 	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UUC_HUD_DashBoard)
// 	UCanvasPanel*			CanvasPanel_RedTeam_My = nullptr;
// 
// 	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UUC_HUD_DashBoard)
// 	UTextBlock*				TextBlock_BlueTemMyRanking = nullptr;
// 	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UUC_HUD_DashBoard)
// 	UTextBlock*				TextBlock_RedTemMyRanking = nullptr;
// 
// 	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UUC_HUD_DashBoard)
// 	UButton*				Button_Close = nullptr;
// 
// private:
// 	int32 MyPlayerUK;
};
