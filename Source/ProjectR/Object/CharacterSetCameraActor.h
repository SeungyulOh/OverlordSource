// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Actor.h"
#include "CharacterSetCameraActor.generated.h"



UCLASS()
class PROJECTR_API ACharacterSetCameraActor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ACharacterSetCameraActor();

	// Called when the game starts or when spawned
	void BeginPlay() override;
	
	// Called every frame
	void Tick( float DeltaSeconds ) override;

	UFUNCTION(BlueprintCallable, Category = CharacterSetCameraActor)
	void RemoveUISpawnCharacter();

	UFUNCTION(BlueprintCallable, Category = CharacterSetCameraActor)
	AActor* CreateUISpawnCharacter(FString& HeroUD);
	
	void SetCameraAndUICharacterToOriginalTrasnformation();

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = CharacterSetCameraActor)
	FTransform	CameraOriginalTransform;

private:
	UPROPERTY()
	TWeakObjectPtr<AActor> UISpawnCharacterPtr;

};
