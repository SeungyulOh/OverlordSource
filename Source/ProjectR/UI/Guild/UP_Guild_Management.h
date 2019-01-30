// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include "EngineMinimal.h"
#include "UI/RBaseWidget.h"
#include "UI/UIDefs.h"
#include "UI/Control/RScrollItem.h"
#include "UP_Guild_Management.generated.h"


class URScrollView;
class UUC_Guild_Attendance;
class UUC_Guild_Donation;
class UUC_Guild_RuneSupport;
class UUC_Guild_Awaiter;
class UUC_Guild_History;
class UUC_Guild_LevelUpDesc;
class UUC_Guild_JoinType_Change;
class UCanvasPanel_RadioGroup;
class ULocalizingTextBlock;
class UUC_CurrencyDisplay;
class UUC_Guild_Grade;
class UCheckBox_Radio;
class UUC_ScrollItem_AvatarIcon;
class UUC_GuildModifyPopup;
/**
 * 
 */


UENUM(BlueprintType)
enum class EGuildManagementTab: uint8
{
	VE_GuildInfo,
	VE_GuildMember,
	VE_GuildRank,
	VE_GuildManager,
};

UCLASS()
class PROJECTR_API UUC_GuildMember_ScrollItem : public URScrollItem
{
	GENERATED_BODY()

public:
	void NativeConstruct() override;
	void NativeDestruct() override;
	void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;
	void SetScrollItemData(int32 Index, URScrollItemData* item, URScrollView* ScrollView) override;
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UGuildManagerListScrollItem)
		ULocalizingTextBlock* TextBlockLevel;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UGuildManagerListScrollItem)
		ULocalizingTextBlock* TextBlockName;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UGuildManagerListScrollItem)
		UTextBlock* TextBlockContribute;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UGuildManagerListScrollItem)
		UWidgetSwitcher*	  backgroundSwitcher;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UGuildManagerListScrollItem)
		UHorizontalBox*		  GradeBox;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UGuildManagerListScrollItem)
		UHorizontalBox*		  BanishBox;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UGuildManagerListScrollItem)
		UWidgetSwitcher*	  GradeIconSwitcher;
// 	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UGuildManagerListScrollItem)
// 		UWidgetSwitcher*	  WidgetSwitcher_GradeIcon;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UGuildManagerListScrollItem)
		UUC_ScrollItem_AvatarIcon* AvatarIcon;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UGuildManagerListScrollItem)
		UTextBlock*			  TextBlockDay;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UGuildManagerListScrollItem)
		ULocalizingTextBlock* LocalizingTextBlockDay;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UGuildManagerListScrollItem)
		UTextBlock*			  TextBlockHour;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UGuildManagerListScrollItem)
		ULocalizingTextBlock* LocalizingTextBlockHour;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UGuildManagerListScrollItem)
		UTextBlock*			  TextBlockMinute;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UGuildManagerListScrollItem)
		ULocalizingTextBlock* LocalizingTextBlockMinute;
public:
	UPROPERTY()
		FGUILD_MEMBER	GuildMemberData;
};

UCLASS()
class PROJECTR_API UUP_Guild_Management : public URBaseWidget
{
	GENERATED_BODY()
	
public:

	void NativeConstruct() override;
	void NativeDestruct() override;

	void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;
	
	void Init();

	//////////////////////////////////////////////////////////////////////////
	// Tab Click
	void RefreshGuildManagementUI();
	void RequestGuildRanking();

	//////////////////////////////////////////////////////////////////////////
	// Info Tab

//	UFUNCTION(BlueprintCallable, Category = UUP_Guild_Management)
// 	void SetGuildInfoUI(UImage* GuildLevelIcon, UTextBlock* GuildName, UTextBlock* GuildLevel, UTextBlock* GuildRank, UTextBlock* GuildPoint, UTextBlock* GuildGold,
// 		UTextBlock* GuildMaster, UTextBlock* MemberCount, UTextBlock* MemberLimit, UTextBlock* GuildInfoNotice,
// 		UButton* BtnGuildDesc, UButton* BtnGuildNotice);

	UFUNCTION(BlueprintCallable, Category = UUP_Guild_Management)
	void OnClick_Info_Attendance();
	UFUNCTION(BlueprintCallable, Category = UUP_Guild_Management)
	void OnClick_Info_Donation();
	UFUNCTION(BlueprintCallable, Category = UUP_Guild_Management)
	void OnClick_Info_Rune();
	UFUNCTION(BlueprintCallable, Category = UUP_Guild_Management)
	void OnClick_Info_Boss();
	UFUNCTION(BlueprintCallable, Category = UUP_Guild_Management)
	void OnClick_Info_Battle();
	UFUNCTION(BlueprintCallable, Category = UUP_Guild_Management)
	void OnClick_Info_Desc();
	UFUNCTION(BlueprintCallable, Category = UUP_Guild_Management)
	void OnClick_Info_Notice();
	UFUNCTION(BlueprintCallable, Category = UUP_Guild_Management)
	void GuildInfoUpdate();

	//////////////////////////////////////////////////////////////////////////
	//  Tab

	UFUNCTION(BlueprintCallable, Category = UUP_Guild_Management)
	void SetGuildMemberUI(bool ApplyData);

	UFUNCTION(BlueprintCallable, Category = UUP_Guild_Management)
	void OnClick_Member_JoinList();
	UFUNCTION(BlueprintCallable, Category = UUP_Guild_Management)
	void OnClick_Member_GuildClose();
	UFUNCTION()
	void GuildClose();
	UFUNCTION()
	void GuildDropOut();

	UFUNCTION(BlueprintCallable, Category = UUP_Guild_Management)
	void OnClick_GuildMemberPrev();
	UFUNCTION(BlueprintCallable, Category = UUP_Guild_Management)
	void OnClick_GuildMemberNext();

	void SetScrollDataGuildMemberList();

	UFUNCTION(BlueprintCallable, Category = UUP_Guild_Management)
	void SetScrollDataGuildManagerList();

	void SetOpenRaidTimeText(int32 InMin, int32 InSec);
	void OpenRaidTimeTick(float InDeltaTime);

	

	//////////////////////////////////////////////////////////////////////////
	// Manager Tab
	UFUNCTION()
	void SetGuildManagerUI(bool ApplyData);

// 	UFUNCTION(BlueprintCallable, Category = UUP_Guild_Management)
// 	void OnClick_Manager_ChangeGuildName(UEditableText* Text_ChangeGuildName);
	UFUNCTION(BlueprintCallable, Category = UUP_Guild_Management)
	void OnClick_Manager_ChangeGuildJoinType();
	UFUNCTION(BlueprintCallable, Category = UUP_Guild_Management)
	void OnClick_Manager_ChangeGuildName();
	UFUNCTION(BlueprintCallable, Category = UUP_Guild_Management)
	void OnClick_Manager_LevelUp();
// 	UFUNCTION(BlueprintCallable, Category = UUP_Guild_Management)
// 	void OnClick_Manager_AutoGradeUp(bool bAuto);
	UFUNCTION()
		void OnClick_GuildManagerPrev();
	UFUNCTION()
		void OnClick_GuildManagerNext();

	UFUNCTION(BlueprintCallable, Category = UUP_Guild_Management)
	void OnClick_Manager_GuildBossDesc();
	
public:
	// Delegate
	UFUNCTION()
	void OnRequestGuildInfo();
	UFUNCTION()
	void OnRefreshGuildInfoUI();
	UFUNCTION()
	void OnRequestGuildMember();
	UFUNCTION()
	void OnRefreshGuildMemberUI(bool ApplyData);
	UFUNCTION()
	void OnGuildContributeRankingRpDelegate(bool ApplyData);
	UFUNCTION()
	void OnReceiveGuestGuildInfo();

	// Event
	UFUNCTION()
		void OnClick_GuildRankingPrev();
	UFUNCTION()
		void OnClick_GuildRankingNext();

	UFUNCTION(BlueprintCallable, Category = UUP_Guild_Management)
	void OnClick_GuildInfoUI(bool IsCheck);
	UFUNCTION(BlueprintCallable, Category = UUP_Guild_Management)
	void OnClick_GuildMemberUI(bool IsCheck);
	UFUNCTION(BlueprintCallable, Category = UUP_Guild_Management)
	void OnClick_GuildRankUI(bool IsCheck);
	UFUNCTION(BlueprintCallable, Category = UUP_Guild_Management)
	void OnClick_GuildManagerUI(bool IsCheck);
	UFUNCTION()
		void OnClick_OpenNotiPopup();
	UFUNCTION()
		void OnClick_OpenDescPopup();
	UFUNCTION()
		void SetScrollViewUI(URScrollView* ScrollView, TArray<FGUILD_INFO> dataList);

	UFUNCTION()
		void OpenChangePositionPop(uint8 Grade, int32 kid, FString Nick);

public:
	// UI Type
	UPROPERTY()
	EGuildManagementTab GuildUIState = EGuildManagementTab::VE_GuildInfo;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UUP_Guild_Management)
	bool						bGuildManager = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UUP_Guild_Management)
	UCanvasPanel_RadioGroup*	RadioGroup_GuildManagementUI;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UUP_Guild_Management)
	UWidgetSwitcher*			WidgetSwitcher_GuildManagementUI;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UUP_Guild_Management)
	UCheckBox_Radio*			CheckBox_Radio_GuildInfo;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UUP_Guild_Management)
	UCheckBox_Radio*			CheckBox_Radio_GuildMember;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UUP_Guild_Management)
	UCheckBox_Radio*			CheckBox_Radio_GuildRanking;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UUP_Guild_Management)
	UCheckBox_Radio*			CheckBox_Radio_GuildManager;
	
	//////////////////////////////////////////////////////////////////////////
	// Info Tab
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UUP_Guild_Management)
	UUC_Guild_Attendance*	UC_Guild_AttendanceBook = nullptr;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UUP_Guild_Management)
	UUC_Guild_Donation*		UC_Guild_Donation = nullptr;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UUP_Guild_Management)
	UUC_Guild_RuneSupport*	UC_Guild_RuneSupport = nullptr;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UUP_Guild_Management)
	UImage*					Image_GuildLevelIcon = nullptr;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UUP_Guild_Management)
	ULocalizingTextBlock*				LocalizingTextBlock_GuildName = nullptr;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UUP_Guild_Management)
	UTextBlock*				TextBlock_GuildRank = nullptr;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UUP_Guild_Management)
	UTextBlock*	TextBlock_GuildPoint = nullptr;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UUP_Guild_Management)
		UTextBlock*	TextBlock_GuildGold = nullptr;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UUP_Guild_Management)
	UTextBlock*				LocalizingTextBlock_GuildMaster = nullptr;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UUP_Guild_Management)
	ULocalizingTextBlock*	LocalizingTextBlock_MemberCount = nullptr;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UUP_Guild_Management)
	ULocalizingTextBlock*	LocalizingTextBlock_MemberLimit = nullptr;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UUP_Guild_Management)
	ULocalizingTextBlock*	LocalizingTextBlock_Info_Notice = nullptr;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UUP_Guild_Management)
	UButton*				Button_Info_Attendance = nullptr;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UUP_Guild_Management)
	UButton*				Button_Info_Donation = nullptr;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UUP_Guild_Management)
	UButton*				Button_History = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UUP_Guild_Management)
	UButton* BtnGuildDesc = nullptr;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UUP_Guild_Management)
	UButton* BtnGuildNotice = nullptr;

	//////////////////////////////////////////////////////////////////////////
	// Member Tab
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UUP_Guild_Management)
	URScrollView* ScrollView_GuildMemberList;
	UPROPERTY()
		TArray<class UGuildMember_ScrollItemData*> CachedGuildMemberList;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UUP_Guild_Management)
	UButton*	Button_Page_Prev;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UUP_Guild_Management)
	UButton*	Button_Page_Next;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UUP_Guild_Management)
	UButton*	Button_GuildClose = nullptr;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UUP_Guild_Management)
	UButton*	Button_ShowGuildJoinList = nullptr;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UUP_Guild_Management)
	UTextBlock* TextBlock_MemberPage;
	UPROPERTY()
		UTextBlock* Text_GuildClose;
	UPROPERTY()
		UTextBlock* Text_JoinAccept;

	int32 CurrentMemberPage;
	int32 RequestMemberPage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UUP_Guild_Management)
	UUC_Guild_Awaiter*		UC_Guild_Approve;

	//////////////////////////////////////////////////////////////////////////
	// Ranking Tab
	UPROPERTY()
		UButton*	Button_PrevRanking;
	UPROPERTY()
		UButton*	Button_NextRanking;
	UPROPERTY()
		UTextBlock*				TextBlock_RankingPage;
	UPROPERTY()
	URScrollView*				ScrollView_GuildRankingList;
	UPROPERTY()
		TArray<class UGuildList_ScrollItemData*> CachedGuildRankingList;
	int32						CurrentRankingPage;
	int32						RequestRankingPage;

	//////////////////////////////////////////////////////////////////////////
	// Manager Tab
	UPROPERTY()
		UButton* Button_Manager_Prev;
	UPROPERTY()
		UButton* Button_Manager_Next;
	UPROPERTY()
		UTextBlock* Text_Manager_Page;
	UPROPERTY()
		UButton* Button_Manager_Notice;
	UPROPERTY()
		UButton* ButtonOverlayGuildNotice;
	UPROPERTY()
		UTextBlock*	Text_Manager_Notice;

	UPROPERTY()
		UButton* Button_Manager_Desc;
	UPROPERTY()
		UButton* ButtonOverlayGuildDesc;
	UPROPERTY()
		UTextBlock*	Text_Manager_Desc;

	UPROPERTY()
		UButton* ButtonGuildJoinType;
	UPROPERTY()
		UButton* ButtonGuildName;
	UPROPERTY()
		UButton* ButtonGuildLevelup;

	UPROPERTY()
	UUC_Guild_Grade* UC_Popup_Guild_Manager_Position;
	UPROPERTY()
		UUC_GuildModifyPopup* UC_Popup_Guild_Manager_Notice;
	UPROPERTY()
		UUC_Guild_JoinType_Change* UP_Popup_Guild_Manager_JoinSetting;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UUP_Guild_Management)
	URScrollView*				ScrollView_MemberList_InManager;
	UPROPERTY()
		TArray<class UGuildManagerListScrollItemData*> CachedGuildMemberInManagerList;
	int32						CurrenManagerPage;
	int32						RequestManagerPage;

	//////////////////////////////////////////////////////////////////////////
	// Guild Raid	
public:
	UFUNCTION(BlueprintCallable, Category = UUP_Guild_Management)
	void RequestRaidOpenInfo();

private:
	void SetGuildRaidOpenInfo();
	
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UUP_Guild_Management)
	UTextBlock*		Text_GuildRaidRemainTime_Min;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UUP_Guild_Management)
	UTextBlock*		Text_GuildRaidRemainTime_Sec;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UUP_Guild_Management)
	UWidgetSwitcher*		Switcher_GuildRaidBoss;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UUP_Guild_Management)
	UVerticalBox*			Vertical_RaidInfo;
	//////////////////////////////////////////////////////////////////////////
private:
	bool				bOpenRaidTimeCount = false;
	int64				nCurrentOpenRaidTime = 1;
};