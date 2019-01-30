// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "SharedConstants/GlobalConstants.h"
#include "UI/RBaseWidget.h"
#include "UP_ReadyToStart.generated.h"


class URGameInstance;
/**
 * 
 */
UCLASS()
class PROJECTR_API UUP_ReadyToStart : public URBaseWidget
{
	GENERATED_BODY()
	
public:
	void NativeConstruct() override;
	void NativeDestruct() override;

	// Delegate Funtion
	UFUNCTION()
	void OnReadyToStartEvent();
	UFUNCTION()
	void OnPhaseEvent(FString StringKey);

	
	UFUNCTION()
	virtual void BP_ReadyToStartEvent();
	

public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = UUP_ReadyToStart)
	TWeakObjectPtr<URGameInstance> RGameInstance;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = UUP_ReadyToStart)
	EGamePlayModeTypeEnum	CheckGameMode = EGamePlayModeTypeEnum::VE_None;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = UUP_ReadyToStart)
	UWidgetAnimation*		StartCountAnim;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = UUP_ReadyToStart)
	UWidgetAnimation*		StartPhaseAnim;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = UUP_ReadyToStart)
	UTextBlock*				PhaseText;
};
