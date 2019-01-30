// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Character/BattleBaseCharacter.h"
#include "PlayerCharacter.generated.h"


/**
 * 
 */
UCLASS()
class PROJECTR_API APlayerCharacter : public ABattleBaseCharacter
{
	GENERATED_BODY()
	
	
public:

	APlayerCharacter(const FObjectInitializer& ObjectInitializer);

	void BeginPlay() override;
	void Destroyed() override;
	void Tick( float DeltaSecond ) override;
	

	//----------------------------------------------------------------------------------------
	// Battle base characet interface
	//----------------------------------------------------------------------------------------
	void AttachUI() override;


public:

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = APlayerCharacter)
	bool		ShowBoundingCircle = false;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = APlayerCharacter)
	bool		ShowDebugingPanel = false;
private:
	
	void		DrawDebugBoundingCircle();

	
};
