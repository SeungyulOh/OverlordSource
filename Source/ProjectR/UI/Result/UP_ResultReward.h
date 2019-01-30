// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "UI/RBaseWidget.h"
#include "CustomActors/BaseSequenceActor.h"
#include "UP_ResultReward.generated.h"

class UUC_Item_Icons;
class UCanvasPanel;
class ABaseSequenceActor;

/**
 * 
 */
UCLASS()
class PROJECTR_API UUP_ResultReward : public URBaseWidget
{
	GENERATED_BODY()
	
public:
	void NativeConstruct() override;
	void NativeDestruct() override;
	
	UFUNCTION(BlueprintCallable, Category = UP_ResultReward)
	void OnClick_Close();
	
	UFUNCTION(BlueprintImplementableEvent, Category = UP_ResultReward)
	void OnPlayAimation();

	UFUNCTION()
	void CallbackSequenceEnd(FName SequenceKey);

	UFUNCTION()
	void CallbackCreateWidget();

	
	

public:
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = UP_ResultReward)
	UCanvasPanel* RewardItemGroup = nullptr;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = UP_ResultReward)
	TArray< UCanvasPanel* >  CanvasRewardItems;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = UP_ResultReward)
	TArray< UUC_Item_Icons* >  RewardItemIcons;


private:
	void SetRewardItemIcons();
	void InitRewardItemIcons();
	void ReturnToMainCamera();

private:
	UPROPERTY()
	TWeakObjectPtr<ABaseSequenceActor> SequenceActor;
	
	UPROPERTY()
	TWeakObjectPtr<	AActor >		MainCameraActor;

	FName							SequenceName = FName(TEXT("Rewardbox_Sequence"));
	FName							SequenceTargetActorName = FName(TEXT("SequenceTargetActor"));


};
