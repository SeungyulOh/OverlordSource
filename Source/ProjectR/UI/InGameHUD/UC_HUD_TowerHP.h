// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "UI/RBaseWidget.h"
#include "GameState/Stage/Spawner/LevelActor.h"
#include "UC_HUD_TowerHP.generated.h"



class UTextBlock;
class UProgressBar;
class UImage;
class UBattleEntityComponent;


/**
 * 
 */
UCLASS()
class PROJECTR_API UUC_HUD_TowerHP : public URBaseWidget
{
	GENERATED_BODY()
	
public:
	virtual void NativeConstruct() override;
	virtual void NativeDestruct() override;

	void ChangeMode(EPhaseClearEnumType InType);

	FORCEINLINE bool isProtectMode() { return ClearConditionWidgetSwitcher->GetActiveWidgetIndex() == 1; }

	EPhaseClearEnumType GetPhaseType() { return PhaseType; }
	void ResetPhaseType();

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UC_HUD_TowerHP|Ref")
	UImage*			Image_Target;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UC_HUD_TowerHP|Ref")
	UImage*			Attacked; //noti

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UC_HUD_TowerHP|Ref")
	UProgressBar*	ProgressBar_HP;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UC_HUD_TowerHP|Ref")
	UTextBlock*		Text_HP;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UC_HUD_TowerHP|Ref")
	UTextBlock*		Unit_Percent;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UC_HUD_TowerHP|RefAni")
	UWidgetAnimation*		HpBarDamageAni = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UC_HUD_TowerHP)
	FLinearColor	NormalColor;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UC_HUD_TowerHP)
	FLinearColor	DangerousColor;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UC_HUD_TowerHP)
	UTextBlock*	Text_TimeMinutes;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UC_HUD_TowerHP)
	UTextBlock*	Text_TimeSecond;

	UPROPERTY()
	UWidgetSwitcher*		ClearConditionWidgetSwitcher;
private:
	UFUNCTION()
	void CheckProtectObjectSpawn(AActor* SpawnActor);

	UFUNCTION()
	void OnDamageNotifyFromBattleCompnonentNotifyDamageDelegate( AActor* OwnerActor, float FinalDamage, EBattleDamageResultEnum InDamageResultType, AActor* FromActor);
	
	void SetUI_ProtectTower(AActor* SpawnActor);

private:
	UPROPERTY()
	TWeakObjectPtr< AActor > ProtectActor;

	UPROPERTY()
	UBattleEntityComponent* BattleEntityComp = nullptr;

	EPhaseClearEnumType 	PhaseType = EPhaseClearEnumType::VE_MAX;

	bool					IsDangerous = false;

	static bool				bFirst;
};