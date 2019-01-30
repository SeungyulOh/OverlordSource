// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/RBaseWidget.h"
#include "SkillIcon_New.generated.h"

UENUM(BlueprintType)
enum class EIconState : uint8
{
	IDLE,
	COOL,
	LOCK,
	ABNORMAL,
	DEAD,
	MAX,
};


USTRUCT(BlueprintType)
struct PROJECTR_API FVariables_SkillIcon
{
	GENERATED_USTRUCT_BODY()

		
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Variables")
	UWidgetSwitcher* WidgetSwitcher_State;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Variables")
	UWidgetSwitcher* PortraitSwitcher;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Variables")
	UImage*		Image_SkillIcon;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Variables")
	UImage*		Image_CoolGauge;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Variables")
	UTextBlock*	Text_CoolTime;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Variables")
	UImage*		Image_SkillPad;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Variables")
	UImage*		Image_Portrait;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Variables")
	UOverlay* PortraitOverlay;
	

};

USTRUCT(BlueprintType)
struct PROJECTR_API FBlackboard_SkillIcon
{
	GENERATED_USTRUCT_BODY()


	UPROPERTY()
	TWeakObjectPtr<AActor>  OwnerCharacter;
	UPROPERTY()
	ESkillCategoryTypeEnum  SkillCategory;
	UPROPERTY()
	EIconState	CurrentState = EIconState::MAX;

	UPROPERTY()
	bool bColosseumMode = false;

	TBitArray<FDefaultBitArrayAllocator>	BlackBoardArray;

	/*Cooltime*/
	float CurrentCoolTime = INDEX_NONE;
	float MaxCoolTime = INDEX_NONE;

	void SetBlackboardData(EIconState InState, bool bOn);
	void ClearBlackboardData();
};


USTRUCT(BlueprintType)
struct PROJECTR_API FRenderer_SkillIcon
{
	GENERATED_USTRUCT_BODY()
	
	FVariables_SkillIcon* variables;
	FBlackboard_SkillIcon* blackboard;

	TWeakObjectPtr<class USkillIcon_New> Parent;

	void Prepare(struct FHeroSlotInfo slotinfo, ESkillCategoryTypeEnum SkillCategory);
	void Render(float DeltaTime);
};


USTRUCT(BlueprintType)
struct PROJECTR_API FStateMachine_SkillIcon
{
	GENERATED_USTRUCT_BODY()

	FVariables_SkillIcon* variables;
	FBlackboard_SkillIcon* blackboard;

	EIconState ChangeState();

	
};


UCLASS()
class PROJECTR_API USkillIcon_New : public URBaseWidget
{
	GENERATED_BODY()
	
public:
	void NativeConstruct() override;
	void NativeDestruct() override;
	void NativeTick(const FGeometry& MyGeometry, float InDeltaTime);

public:
	void IncreaseCoolTime(float InCool);
	bool isDPadEnable();


public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "USkillIcon_New")
	FVariables_SkillIcon	Variables;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "USkillIcon_New")
	FRenderer_SkillIcon		Renderer;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "USkillIcon_New")
	FStateMachine_SkillIcon	FSM;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "USkillIcon_New")
	FBlackboard_SkillIcon	Blackboard;

	
};
