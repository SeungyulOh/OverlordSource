// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "UI/UIDefs.h"

#include "UI/Control/RScrollItem.h"
#include "UI/Control/RScrollView.h"

#include "UC_Arena_reward_popup.generated.h"
class UUC_MaterialIcon;
/**
 * 
 */

USTRUCT(BlueprintType)
struct PROJECTR_API FArenaRankInfo
{
	GENERATED_USTRUCT_BODY()
public:
	bool	IsRanker;
	int32	Rank;
	FName	RewardKey;
};

UCLASS()
class PROJECTR_API UUC_Arena_reward_Data : public URScrollItemData
{
	GENERATED_BODY()
public:
	bool	IsRanker;
	int32		Rank;
	FName	RewardKey;
};

UCLASS()
class PROJECTR_API UUC_Arena_reward_list : public URScrollItem
{
	GENERATED_BODY()
public:
	void		NativeConstruct() override;
	void		NativeDestruct() override;

	UPROPERTY()
	TArray<UUC_MaterialIcon*>		CurrencyData;
	UPROPERTY()
	UImage*							rank_icon = nullptr;

	virtual void SetScrollItemData(int32 Index, URScrollItemData* item, URScrollView* ScrollView) override;
	void SetDataWithArenaRankInfo(FArenaRankInfo info);
	void SetData(UUC_Arena_reward_Data* RewardData);
	void SetDetailData(bool _IsRanker, int32 _Rank, FName _RewardKey);
	void SetRankImage(int32 InRank);
	void SetPercentImage(int32 InRank);
};

UCLASS()
class PROJECTR_API UArenaRankScrollItemData : public URScrollItemData
{
	GENERATED_BODY()
public:
	FArenaRankInfo leftInfo;
	FArenaRankInfo rightInfo;
};

UCLASS()
class PROJECTR_API UArenaRankScrollItem : public URScrollItem
{
	GENERATED_BODY()
public:
	void NativeConstruct() override;
	void NativeDestruct() override;

public:
	virtual void SetScrollItemData(int32 Index, URScrollItemData* item, URScrollView* ScrollView) override;

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = ArenaRewardItem)
	UUC_Arena_reward_list* LeftRank;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = ArenaRewardItem)
	UUC_Arena_reward_list* RightRank;
};

UCLASS()
class PROJECTR_API UUC_Arena_reward_popup : public URBaseWidget
{
	GENERATED_BODY()
public:
	void		NativeConstruct() override;
	void		NativeDestruct() override;

	UFUNCTION()
	void		OnClickPopupClose();
	void SetArenaRankInfo(const TArray<FName>& InRankerRewardLIst, const TArray<FName>& InAllUserRewardLIst);
public:
	UPROPERTY()
	UButton*							ButtonClose = nullptr;

	UPROPERTY()
	TArray<FName> RankerRewardLIst;
	UPROPERTY()
	TArray<FName> AllUserRewardLIst;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = ArenaReward)
	URScrollView*						RankListScrollView = nullptr;

};