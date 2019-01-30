// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "RAI_Blackboard.generated.h"


UENUM(BlueprintType)		//"BlueprintType" is essential to include
enum class ERAIBalckboardValueEnum : uint8
{
	VE_None					UMETA(DisplayName = "None"),
	VE_CurrentTargetActor 	UMETA(DisplayName = "CurrentTargetActor"),
	VE_TargetLocation		UMETA(DisplayName = "TargetLocation"),
	VE_ForceTarget	 		UMETA(DisplayName = "ForceTarget"),	
	VE_AttackLocation		UMETA(DisplayName = "AttackLocation"),	
	VE_LeaderToFollow		UMETA(DisplayName = "LeaderToFollow"),
	VE_KeepSafeTarget		UMETA(DisplayName = "KeepSafeTarget"),
	VE_AvoidanceVector		UMETA(DisplayName = "AvoidanceVector"),
	VE_CrewTacticsMove		UMETA(DisplayName = "CrewTacticsMove"),
	VE_Max,
};

/** Class needed to support InterfaceCast<IToStringInterface>(Object) */
UCLASS()
class  PROJECTR_API URAI_Blackboard : public UObject
{
	GENERATED_BODY()
			
public:
	URAI_Blackboard();
	void BeginDestroy() override;

	void					SetBlackboardFloatValue(ERAIBalckboardValueEnum InValueType, float InValue);
	void					SetBlackboardVectorValue(ERAIBalckboardValueEnum InValueType, FVector InValue);
	void					SetBlackboardActorValue(ERAIBalckboardValueEnum InValueType, AActor* InValue);
	void					SetBlackboardBoolValue(ERAIBalckboardValueEnum InValueType, bool InValue);

	float					GetBlackboardFloatValue(ERAIBalckboardValueEnum InValueType);
	FVector					GetBlackboardVectorValue(ERAIBalckboardValueEnum InValueType);
	AActor*					GetBlackboardActorValue(ERAIBalckboardValueEnum InValueType);
	bool					GetBlackboardBoolValue(ERAIBalckboardValueEnum InValueType);

	bool					IsSetBlackboardValue(ERAIBalckboardValueEnum InValueType);
	void					ClearBlackboardValue(ERAIBalckboardValueEnum InValueType);
	void					ClearAllBlackboardValue();

protected:
	TBitArray<FDefaultBitArrayAllocator>	BlackValueState;
	UPROPERTY()
	TWeakObjectPtr< AActor >				CurrentTargetActor;
	UPROPERTY()
	TWeakObjectPtr< AActor >				ForceTargetActor;
	UPROPERTY()
	TWeakObjectPtr< AActor >				LeaderToFollow;
	UPROPERTY()
	TWeakObjectPtr< AActor >				KeepSafeTargetActor;

	UPROPERTY()
	FVector									TargetLocation;
	UPROPERTY()
	FVector									AttackLocation;
	UPROPERTY()
	FVector									AvoidanceVector;

	//TArray<ERAIBalckboardValueEnum> AIExecuteArray;
};
