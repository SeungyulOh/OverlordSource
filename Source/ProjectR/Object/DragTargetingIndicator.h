// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Actor.h"
#include "DragTargetingIndicator.generated.h"

UENUM(BlueprintType)		//"BlueprintType" is essential to include
enum class EDragTargetTypeEnum : uint8
{
	VE_None				UMETA(DisplayName = "None"),
	VE_Noraml			UMETA(DisplayName = "Normal"),	
	VE_EnemyTarget		UMETA(DisplayName = "Enemy Target"),
	VE_FriendTarget		UMETA(DisplayName = "Friend Target"),	
};

UCLASS()
class PROJECTR_API ADragTargetingIndicator : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ADragTargetingIndicator();

	// Called when the game starts or when spawned
	virtual void							BeginPlay() override;

	// Called every frame
	virtual void							Tick(float DeltaSeconds) override;

	void									ActivateDragTarget(AActor* InDragOwner, bool InActivate, FVector CurPos = FVector::ZeroVector, bool IsBlackOutTime = true);
	void									DragTarget(FVector CurPos);
	void									SetDragTargetType(EDragTargetTypeEnum InDragTargetType);
	EDragTargetTypeEnum						GetDragTargetType();
	AActor*									GetCurTargetActor() { return CurTargetActor.Get(); }

	bool									CheckTarget(FVector curPos);

	void									SpawnBeamParticle(AActor* InDragOwner);
	void									DespawnBeamParticle();

	UFUNCTION(BlueprintImplementableEvent, Category = Battle)
	void									OnSetDragTargetType(EDragTargetTypeEnum InDragTargetType);

	UFUNCTION(BlueprintCallable, Category = Battle)
	void SetActivateSnapMode(bool IsActive, FVector TargetLocation, bool bAutoRelease = true, float Gap = 100.f);

public:
	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category = DragTargetingIndicator)
	UParticleSystem*						PickUIBeam;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = DragTargetingIndicator)
	UParticleSystemComponent*				PickUIBeam_Comp;
private:
	void									SetBeamSourceLocation();
	FVector									GetBeamSourceLocation();
	void									BlackOutTimeStart();
	void									BlackOutTimeEnd();
private:

	TWeakObjectPtr< AActor >				DragOwner;
	EDragTargetTypeEnum						CurTargetType;
	TWeakObjectPtr< AActor >				PrevTargetActor;
	TWeakObjectPtr< AActor >				CurTargetActor;

	FTimerHandle							DragTargetDarkEndHandle;
	
	bool									IsSnapMode = false;
	FVector									SnapTarget;
	float									SnapGap = 100;
	bool									IsAutoRelease = true;
};
