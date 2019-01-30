#pragma once

#include "CoreMinimal.h"

#include "UI/Control/RScrollItem.h"
#include "UI/Control/RScrollView.h"

#include "UI/Common/Renewal_BaseHeroIcon.h"

#include "UI/Colosseum/UC_Colosseum_Class.h"

#include "UC_Colosseum_BattleRecord.generated.h"

UCLASS()
class PROJECTR_API URColosseum_BattleRecord_Data : public URScrollItemData
{
	GENERATED_BODY()
public:
	bool			Result;
	int32			GradeID;
	int32			Point;
	int32			Level;
	FText			Name;
	FText			Guild;
	int32			Power;
	TArray<FName>	HeroIDs;
	void SetHistory(bool InResult, int32 InGradeID, int32 InPoint, int32 InLevel, FText InName, FText InGuild, int32 InPower, TArray<FName> InHeroIDs);
};

UCLASS()
class PROJECTR_API URColosseum_BattleRecord_Item : public URScrollItem
{
	GENERATED_BODY()

public:
	UPROPERTY()
	UUC_Colosseum_Class*		Colosseum_Tier				=	nullptr;

	UPROPERTY()
	UWidgetSwitcher*			WidgetSwitcher_AResult		=	nullptr;
	UPROPERTY()
	UWidgetSwitcher*			WidgetSwitcher_APointImage	=	nullptr;
	UPROPERTY()
	UWidgetSwitcher*			WidgetSwitcher_DResult		=	nullptr;
	UPROPERTY()
	UWidgetSwitcher*			WidgetSwitcher_DPointImage	=	nullptr;

	UPROPERTY()
	UTextBlock*					TextBlock_APoint			=	nullptr;
	UPROPERTY()
	UTextBlock*					TextBlock_ALevel			=	nullptr;
	UPROPERTY()
	UTextBlock*					TextBlock_AName				=	nullptr;
	UPROPERTY()
	UTextBlock*					TextBlock_AGuild			=	nullptr;
	UPROPERTY()
	UTextBlock*					TextBlock_APower			=	nullptr;
	UPROPERTY()
	TArray<URenewal_BaseHeroIcon*> AttackHeros;

	virtual void	InvalidateData()	override;
	void			SetHistory(bool Result, int32 InGradeID, int32 Point, int32 Level, FText Name, FText Guild, int32 Power, TArray<FName> HeroIds);
};