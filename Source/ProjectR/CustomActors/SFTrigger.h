// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Actor.h"
#include "SFTrigger.generated.h"

class ABasePlayerController;
class ASkeletalMeshActor;
class URSystemNoti;

UCLASS()
class PROJECTR_API ASFTrigger : public AActor
{
	GENERATED_BODY()
	
public:
	// Sets default values for this actor's properties
	ASFTrigger(const FObjectInitializer& ObjectInitializer);

	//void OnConstruction(const FTransform& Transform) override;

	// Called when the game starts or when spawned
	void BeginPlay() override;
	
	// Called every frame
	void Tick( float DeltaSeconds ) override;
	void Destroyed() override;
	
	bool ComputeTriggerLocation(FVector& InOutLocation);
	void SetNotiDialogue();
	bool SetTargetActor();
	void CheckTick();
	int32 GetGroupIndex();
public:
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = SFTrigger)
	FVector			TargetLocation;

	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = SFTrigger)
	FName			DialogueID;

	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = SFTrigger)
	FName			TargetActorID;

	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = SFTrigger)
	int32			GroupIndex = 0;

	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = SFTrigger)
	bool			IsBlockIn = false;

	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = SFTrigger)
	bool			IsBlockX = false;

	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = SFTrigger)
	bool			IsBlockY = false;

	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = SFTrigger)
	bool			IsBeginTick = false;

	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = ExceptionTrigger)
	int32			PlayerTargetIndex = -1;

	//UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = BaseTrigger)
	//UShapeComponent*		CollisionComponent = nullptr;

private:
	void CheckBlock();

private:
	UPROPERTY()
	ABasePlayerController*	BasePlayerController = nullptr;

	UPROPERTY()
	ASkeletalMeshActor*		TargetActor = nullptr;

	UPROPERTY()
	USkeletalMeshComponent*		TempSaveActor = nullptr;

	UPROPERTY()
	URSystemNoti*			SystemNotiWidget = nullptr;

	UPROPERTY()
	UUserWidget* 			NotiDialogue = nullptr;
};
