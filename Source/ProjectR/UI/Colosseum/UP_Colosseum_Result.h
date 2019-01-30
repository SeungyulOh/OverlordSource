// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Object.h"
#include "UI/RBaseWidget.h"
#include "UI/UIDefs.h"

#include "Network/PacketDataStructures.h"

#include "UP_Colosseum_Result.generated.h"


class UUC_ResultStatistics;


/**
 * 
 */
UCLASS()
class PROJECTR_API UUP_Colosseum_Result : public URBaseWidget
{
	GENERATED_BODY()
	
public:
	void NativeConstruct() override;
	
	UFUNCTION(BlueprintCallable, Category = UUP_Colosseum_Result)
	void OnClickEndButton(bool GotoMainLobby);

	UFUNCTION(BlueprintCallable, Category = UUP_Colosseum_Result)
	void SetTextClearTime(UTextBlock* minute, UTextBlock* second10, UTextBlock* second1);

	UFUNCTION(BlueprintCallable, Category = UUP_Colosseum_Result)
	void SetTextRank(UTextBlock* CurrentRank, UWidgetSwitcher* ChangeRankSwitcher, UTextBlock* UpRank, UTextBlock* DownRank);

	UFUNCTION(BlueprintCallable, Category = UUP_Colosseum_Result)
	void SetTextMatchRewards(UTextBlock* point, UCanvasPanel* BonusPanel, UTextBlock* BonusCount, UTextBlock* BonusPoint, UTextBlock* sp2_point);

	UFUNCTION(BlueprintCallable, Category = UUP_Colosseum_Result)
	void SetWinMarkSwitcher(UWidgetSwitcher* widget_switcher);

	UFUNCTION(BlueprintCallable, Category = UUP_Colosseum_Result)
	void GetMatchTargetHeroInfo(TArray<FEQUIPED_HERO>& EquipedHeroList);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UUP_Colosseum_Result)
	UUC_ResultStatistics*		BattleStatistics = nullptr;
};
