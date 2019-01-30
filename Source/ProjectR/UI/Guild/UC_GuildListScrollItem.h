// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/Control/RScrollItem.h"
#include "UC_GuildListScrollItem.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTR_API UUC_GuildListScrollItem : public URScrollItem
{
	GENERATED_BODY()
	
public:
	void		NativeConstruct() override;
	void		NativeDestruct() override;
	void		InvalidateData() override;
	
	UFUNCTION()
	void OnClick_GuildJoinFree();
	UFUNCTION()
		void OnClick_GuildJoinApply();
	UFUNCTION()
	void OnClick_GuildJoinCancel();
	UFUNCTION()
	void OnClick_GuildVisit();
	UFUNCTION()
	void WaitCancelGuildJoin();

	void SetGuildRanking(int32 ranking, bool isNew);
	void SetGuildName(FString name, bool inWaiting);
	void SetGuildLevel(int32 level);
	void SetGuildMemCnt(int32 curr, int32 limit);
	void SetGuildJoinType(bool isFree);
	void SetGuildDesc(FString desc);
	void SetGuildBtnType(bool isFree, bool isMine, bool isinfopage, bool isWaiting);

public:
	UPROPERTY()
		UWidgetSwitcher* WidgetSwitcher_BG = nullptr;
	UPROPERTY()
		UWidgetSwitcher* WidgetSwitcher_Ranking = nullptr;
	UPROPERTY()
		UTextBlock* Text_GuildRank = nullptr;
	UPROPERTY()
		UTextBlock* Text_GuildName = nullptr;
	UPROPERTY()
		UHorizontalBox* HorizontalBox_JoinWait = nullptr;
	UPROPERTY()
		UTextBlock* Text_GuildLevel = nullptr;
	UPROPERTY()
		UTextBlock* Text_GuildDesc = nullptr;
	UPROPERTY()
		UTextBlock* Text_MemberCount = nullptr;
	UPROPERTY()
		UTextBlock* Text_MemberLimit = nullptr;
	UPROPERTY()
		UWidgetSwitcher* WidgetSwitcher_JoinType = nullptr;
	UPROPERTY()
		UWidgetSwitcher* WidgetSwitcher_ActionType = nullptr;
	UPROPERTY()
		UButton* Button_Request = nullptr;
	UPROPERTY()
		UButton* Button_WaitCancel = nullptr;
	UPROPERTY()
		UButton* Button_FreeJoin = nullptr;
	UPROPERTY()
		UButton* Button_Visit = nullptr;

};
