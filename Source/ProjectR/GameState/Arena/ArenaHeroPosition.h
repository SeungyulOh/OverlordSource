// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Actor.h"
#include "ArenaHeroPosition.generated.h"



UCLASS()
class PROJECTR_API AArenaHeroPosition : public AActor
{
	GENERATED_BODY()
	
public:	
	AArenaHeroPosition(); // Sets default values for this actor's properties

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = ArenaHeroPosition)
	bool					MyHeroPosition = true;

	// PositionIndex counting start from 1.
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = ArenaHeroPosition)
	int32					PositionIndex = 1;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = ArenaHeroPosition)
	bool					bTestCharacter = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = ArenaHeroPosition)
	TArray<FName>			TestCharacterList;

	UFUNCTION(BlueprintCallable, Category = ArenaHeroPosition)
	void SaveHeroPosition();
};
