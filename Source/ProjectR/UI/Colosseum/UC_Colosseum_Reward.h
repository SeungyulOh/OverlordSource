#pragma once

#include "CoreMinimal.h"

#include "UI/Control/RScrollItem.h"
#include "UI/Control/RScrollView.h"
#include "UI/Component/CheckBox_Radio.h"

#include "UI/Colosseum/UC_Colosseum_RewardInfo.h"

#include "UC_Colosseum_Reward.generated.h"

UCLASS()
class PROJECTR_API UUC_Colosseum_Reward : public URBaseWidget
{
	GENERATED_BODY()
public:
	void			NativeConstruct()		override;
	void			NativeDestruct()		override;
	void			Init();
	void			Reset();
	UFUNCTION()
	void			OnRadioCheckRankReward(bool InIsCheck);
	UFUNCTION()
	void			OnRadioCheckGradeReward(bool InIsCheck);
	UFUNCTION()
	void			OnClickClose();
public:
	UPROPERTY()
	UCheckBox_Radio*					Radio_RankReward	=	nullptr;
	UPROPERTY()
	UCheckBox_Radio*					Radio_GradeReward	=	nullptr;
	UPROPERTY()
	URScrollView*						RewardScroll		=	nullptr;
	UPROPERTY()
	UButton*							Button_Close		=	nullptr;
	UPROPERTY()
		TArray<URColosseum_Reward_ItemData*> CachedScrollItemData;
};