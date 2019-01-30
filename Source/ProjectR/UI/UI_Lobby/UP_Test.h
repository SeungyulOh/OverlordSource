// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "UI/RBaseWidget.h"


#include "UP_Test.generated.h"
/**
 * 
 */


UENUM(BlueprintType)		//"BlueprintType" is essential to include
enum class ECharacterSelectTypeEnum : uint8
{
	VE_Main				UMETA(DisplayName = "Main"),
	VE_Left				UMETA(DisplayName = "Left"),
	VE_Right			UMETA(DisplayName = "Right"),
};

UCLASS()
class PROJECTR_API UUP_Test : public URBaseWidget
{
	GENERATED_BODY()
	
public:
	void NativeConstruct() override;
	void NativeDestruct() override;

	UFUNCTION()
	void ChangeHero(FString InHeroUD);

	UFUNCTION()
	void MainChangeHero(FString InHeroUD);

	UFUNCTION()
	void LeftChangeHero(FString InHeroUD);
	UFUNCTION()
	void RightChangeHero(FString InHeroUD);
	UFUNCTION()
	void DestroyLeftCharacter();
	UFUNCTION()
	void DestroyRightCharacter();

	UFUNCTION()
	void MainChangeButton();
	UFUNCTION()
	void LeftChangeButton();
	UFUNCTION()
	void RightChangeButton();

	UPROPERTY()
	AActor*							LeftLobbyCharacter		=	nullptr;
	UPROPERTY()
	AActor*							LeftRWeapon				=	nullptr;
	UPROPERTY()
	AActor*							LeftLWeapon				=	nullptr;
	UPROPERTY()
	AActor*							RightLobbyCharacter		=	nullptr;
	UPROPERTY()
	AActor*							RightRWeapon			=	nullptr;
	UPROPERTY()
	AActor*							RightLWeapon			=	nullptr;

	UPROPERTY()
	ECharacterSelectTypeEnum		CharacterSelectType		=	ECharacterSelectTypeEnum::VE_Main;

	UPROPERTY()
	UButton*						Button_MainChange		=	nullptr;
	UPROPERTY()
	UButton*						Button_RightChange		=	nullptr;
	UPROPERTY()
	UButton*						Button_LeftChange		=	nullptr;
};