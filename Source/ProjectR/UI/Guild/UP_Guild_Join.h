// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "UI/RBaseWidget.h"
#include "UI/UIDefs.h"
#include "Network/PacketDataStructures.h"
#include "Text.h"
#include "UI/Component/CheckBox_Radio.h"
#include "UP_Guild_Join.generated.h"

class URScrollView;
class UUC_CurrencyDisplay;
class UUC_Guild_Create;

/**
 * 
 */

UENUM(BlueprintType)
enum class EGuildJoinTab : uint8
{
	VE_Join,
	VE_Ranking,
	VE_Create,
};

UCLASS()
class PROJECTR_API UUP_Guild_Join : public URBaseWidget
{
	GENERATED_BODY()
	
public:

	void NativeConstruct() override;
	void NativeDestruct() override;

	void Init();

	//////////////////////////////////////////////////////////////////////////
	// Tab Click
	UFUNCTION(BlueprintCallable, Category = UUP_Guild_Join)
	void OnClick_GuildJoinTab(EGuildJoinTab InType);
	
	void RefreshGuildJoinUI();


	//////////////////////////////////////////////////////////////////////////
	// Join Tab
	UFUNCTION(BlueprintCallable, Category = UUP_Guild_Join)
	void OnClick_GuildFind();
	UFUNCTION(BlueprintCallable, Category = UUP_Guild_Join)
	void OnClick_GuildJoinAuto();
	
	UFUNCTION(BlueprintCallable, Category = UUP_Guild_Join)
	void OnClick_Guild_MainTab_Join(bool isCheck);
	UFUNCTION(BlueprintCallable, Category = UUP_Guild_Join)
	void OnClick_Guild_MainTab_Ranking(bool isCheck);
	UFUNCTION(BlueprintCallable, Category = UUP_Guild_Join)
	void OnClick_Guild_Create_Join_Type_Auto(bool isCheck);
	UFUNCTION(BlueprintCallable, Category = UUP_Guild_Join)
	void OnClick_Guild_Create_Join_Type_Confirm(bool isCheck);
	UFUNCTION()
	void GuildFindTimeOut();
// 	UFUNCTION(BlueprintCallable, Category = UUP_Guild_Join)
// 	void SetScrollDataFindGuildList(URScrollView* ScrollView);
	UFUNCTION()
		void SetScrollViewUI(URScrollView* ScrollView, TArray<FGUILD_INFO> dataList);


	//////////////////////////////////////////////////////////////////////////
	// Ranking Tab
	UFUNCTION(BlueprintCallable, Category = UUP_Guild_Join)
	void OnClick_GuildRankingPrev();
	UFUNCTION(BlueprintCallable, Category = UUP_Guild_Join)
	void OnClick_GuildRankingNext();

	void RequestGuildRanking();
	
	//////////////////////////////////////////////////////////////////////////
	// Create Tab
 	UFUNCTION(BlueprintCallable, Category = UUP_Guild_Join)
 	void OnClick_GuildCreate();
	UFUNCTION(BlueprintCallable, Category = UUP_Guild_Join)
	void OnClick_GuildCreateTabOpen();
	UFUNCTION()
	void CreateGuildSplash();
public:
	// Delegate
	UFUNCTION()
	void OnRefreshGuildInfoUI();
	UFUNCTION()
	void OnGuildCreateRpDelegate();
	UFUNCTION()
	void OnGuildFindNameRpDelegate();
	UFUNCTION()
	void OnGuildJoinRpDelegate(int32 GuildID, bool AutoGradeUp);
	UFUNCTION()
	void OnGuildAwaiterCancelRpDelegate(int32 guild_id);
	UFUNCTION()
	void OnGuildContributeRankingRpDelegate(bool ApplyData);

	// Event
	UFUNCTION()
	void RefreshJoinUI();
	UFUNCTION()
	void RefreshRankingUI();
// 	UFUNCTION()
// 	void RefreshCreateUI();

public:
	// UI Type
	UPROPERTY()
	EGuildJoinTab GuildUIState = EGuildJoinTab::VE_Join;
	
	// GuildJoin UI Type : CreateGuild
	UPROPERTY()
	UEditableTextBox*	Text_GuildName;
	UPROPERTY()
	UEditableTextBox*	Text_GuildDesc;
	UPROPERTY()
	bool	Create_AutoGradeUp = true;

	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = UUP_Guild_Join)
	FText HintText_FindGuildName;
	FText HintText_GuildName;
	FText HintText_GuildDesc;
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UUP_Guild_Join)
	UButton*				Button_FindGuildName;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UUP_Guild_Join)
	UButton*				Button_FindGuildRefresh;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UUP_Guild_Join)
	UButton*				Button_GuildJoinAuto;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UUP_Guild_Join)
	UButton*				Button_CreateGuild;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UUP_Guild_Join)
	UEditableTextBox*		EditableTextBox_FindGuildName;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UUP_Guild_Join)
	UCheckBox_Radio*		CheckBox_Radio_Join;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UUP_Guild_Join)
	UCheckBox_Radio*		CheckBox_Radio_Ranking;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UUP_Guild_Join)
	UCheckBox_Radio*		CheckBox_Radio_AutoGradeUp;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UUP_Guild_Join)
	UCheckBox_Radio*		CheckBox_Radio_WaitGradeUp;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UUP_Guild_Join)
	UWidgetSwitcher*		WidgetSwitcher_Radio_Join;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UUP_Guild_Join)
	UWidgetSwitcher*		WidgetSwitcher_Radio_Ranking;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UUP_Guild_Join)
	UButton*				Button_PrevRanking;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UUP_Guild_Join)
	UButton*				Button_NextRanking;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UUP_Guild_Join)
	UTextBlock*				TextBlock_RankingPage;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UUP_Guild_Join)
	UWidgetSwitcher*		WidgetSwitcher_JoinType;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UUP_Guild_Join)
	UUC_CurrencyDisplay*	UC_CurrencyDisplay;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UUP_Guild_Join)
	UButton*				Button_RQGuildCreate;
	UPROPERTY()
	URScrollView* ScrollView_Recommand_GuildList;
	UPROPERTY()
		TArray<class UGuildList_ScrollItemData*> CachedGuildList;
	UPROPERTY()
	URScrollView* ScrollView_Ranking_List;
// 	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UUP_Guild_Join)
// 	UUC_Guild_Create*		CreateGuildPopup;

private:
	int32				CurrentRankingPage;
	int32				RequestRankingPage;

	FTimerHandle		TimeHandler_GuildFind;
	FTimerHandle		TimeHandler_CreateGuild;
	int32				GuildCreateGold;

};
