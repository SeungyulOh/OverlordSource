#pragma once

#include "CoreMinimal.h"

#include "UI/Control/RScrollItem.h"
#include "UI/Control/RScrollView.h"

#include "UI/Component/CheckBox_Radio.h"

#include "UI/Common/UC_Item_Icons.h"
#include "UI/Colosseum/UC_Colosseum_Class.h"

#include "UC_Colosseum_RewardInfo.generated.h"

#define GRADEINDEX_MAX	8
#define RANKINDEX_MAX	5

UCLASS()
class PROJECTR_API UUC_Colosseum_RewardInfo : public URBaseWidget
{
	GENERATED_BODY()
public:
	void			NativeConstruct()		override;
	void			NativeDestruct()		override;
	void			Init();

	void			SetGradeReward(int32 InGrade);
	void			SetRankReward(int32 InRank);

public:
	UPROPERTY()
	UWidgetSwitcher*			WidgetSwitcher_Reward	=	nullptr;
	UPROPERTY()
	UUC_Colosseum_Class*		ColosseumGrade			=	nullptr;
	UPROPERTY()
	UUserWidget*				ColosseumRank			=	nullptr;
	UPROPERTY()
	TArray<UUC_Item_Icons*>		ItemIcon;
};

UCLASS()
class PROJECTR_API URColosseum_Reward_ItemData : public URScrollItemData
{
	GENERATED_BODY()
public:
	UFUNCTION()
	void								SetRank(int32 InIndex);
	void								SetGrade(int32 InIndex);
public:
	int32								Index		=	0;
	bool								IsRank		=	false;
};

UCLASS()
class PROJECTR_API URColosseum_Reward_Item : public URScrollItem
{
	GENERATED_BODY()
public:
	virtual void SetItemData(int32 InIndex, URScrollItemData* item, URScrollView* ScrollView) override;
public:
	UPROPERTY()
	UUC_Colosseum_RewardInfo*			RankingReward01 = nullptr;
	UPROPERTY()
	UUC_Colosseum_RewardInfo*			RankingReward02 = nullptr;
};