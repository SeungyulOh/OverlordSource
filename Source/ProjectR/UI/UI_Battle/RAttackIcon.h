// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "UI/RBaseWidget.h"
//#include "UI/CharacterUI/CharacterUserWidget.h"
#include "RAttackIcon.generated.h"


class UEventManager;
class AStagePlayerController;

/**
 * 
 */
UCLASS()
class PROJECTR_API URAttackIcon : public URBaseWidget
{
	GENERATED_BODY()
	
public:
	void NativeConstruct() override;
	void NativeDestruct() override;
	void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;

	// Attack Button Event
	UFUNCTION(BlueprintCallable, Category = AttackIcon)
	void BtnEventOnPressed();
	UFUNCTION(BlueprintCallable, Category = AttackIcon)
	void BtnEventOnReleased();
	
	UFUNCTION(BlueprintCallable, Category = AttackIcon)
	void BtnEventOn();

	UFUNCTION(BlueprintCallable, Category = AttackIcon)
	void BtnEventOff();

	FORCEINLINE bool GetAttackButtonActive() { return AttackButtonActive; }

public:

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = RAttackIcon)
	UButton*				Button_Click = nullptr;
	
	UPROPERTY()
	bool AttackButtonActive = false;


private:
	UPROPERTY()
	UEventManager*	EventManagerPtr = nullptr;

	UPROPERTY()
	TWeakObjectPtr<AActor> CurrentAttackTarget;


	UPROPERTY()
	AStagePlayerController*	BasePlayerController = nullptr;
		
	

};
