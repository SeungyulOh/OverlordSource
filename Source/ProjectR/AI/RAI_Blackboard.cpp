// Fill out your copyright notice in the Description page of Project Settings.

#include "ProjectR.h"
#include "RAI_Blackboard.h"


URAI_Blackboard::URAI_Blackboard()
{
	BlackValueState.Init(false, (int32)ERAIBalckboardValueEnum::VE_Max);
}

void URAI_Blackboard::BeginDestroy()
{
	BlackValueState.Empty();

	Super::BeginDestroy();
}

void URAI_Blackboard::SetBlackboardFloatValue(ERAIBalckboardValueEnum InValueType, float InValue)
{
	int32 bitArrayIndex = (int32)InValueType;
	if (BlackValueState.IsValidIndex(bitArrayIndex))
		BlackValueState[bitArrayIndex] = true;
}

void URAI_Blackboard::SetBlackboardVectorValue(ERAIBalckboardValueEnum InValueType, FVector InValue)
{
	int32 bitArrayIndex = (int32)InValueType;
	if (BlackValueState.IsValidIndex(bitArrayIndex))
		BlackValueState[bitArrayIndex] = true;

	switch (InValueType)
	{
		case ERAIBalckboardValueEnum::VE_TargetLocation:
			TargetLocation = InValue;
		case ERAIBalckboardValueEnum::VE_AttackLocation:
			AttackLocation = InValue;
		case ERAIBalckboardValueEnum::VE_AvoidanceVector:
			AvoidanceVector = InValue;
			break;		
	}
}

void URAI_Blackboard::SetBlackboardActorValue(ERAIBalckboardValueEnum InValueType, AActor* InValue)
{
	int32 bitArrayIndex = (int32)InValueType;
	if (BlackValueState.IsValidIndex(bitArrayIndex))
		BlackValueState[bitArrayIndex] = true;

	switch (InValueType)
	{
		case ERAIBalckboardValueEnum::VE_CurrentTargetActor:
			CurrentTargetActor = InValue;
			break;
		case ERAIBalckboardValueEnum::VE_ForceTarget:
			ForceTargetActor = InValue;
		case ERAIBalckboardValueEnum::VE_LeaderToFollow:
			LeaderToFollow = InValue;
			break;
		case ERAIBalckboardValueEnum::VE_KeepSafeTarget:
			KeepSafeTargetActor = InValue;
			break;
	}
}

void URAI_Blackboard::SetBlackboardBoolValue(ERAIBalckboardValueEnum InValueType, bool InValue)
{
	int32 bitArrayIndex = (int32)InValueType;
	if (BlackValueState.IsValidIndex(bitArrayIndex))
		BlackValueState[bitArrayIndex] = InValue;
}


float URAI_Blackboard::GetBlackboardFloatValue(ERAIBalckboardValueEnum InValueType)
{
	int32 bitArrayIndex = (int32)InValueType;
	if (BlackValueState.IsValidIndex(bitArrayIndex))
	{
		if (BlackValueState[bitArrayIndex])
		{		
		}
	}
	return 0.0f;
}

FVector URAI_Blackboard::GetBlackboardVectorValue(ERAIBalckboardValueEnum InValueType)
{
	int32 bitArrayIndex = (int32)InValueType;
	if (BlackValueState.IsValidIndex(bitArrayIndex))
	{
		if (BlackValueState[bitArrayIndex])
		{
			switch (InValueType)
			{
			case ERAIBalckboardValueEnum::VE_TargetLocation:
				return TargetLocation;
			case ERAIBalckboardValueEnum::VE_AttackLocation:
				return AttackLocation;
			case ERAIBalckboardValueEnum::VE_AvoidanceVector:
				return AvoidanceVector;
			}
		}
	}
	return FVector::ZeroVector;
}

AActor* URAI_Blackboard::GetBlackboardActorValue(ERAIBalckboardValueEnum InValueType)
{
	int32 bitArrayIndex = (int32)InValueType;
	if (BlackValueState.IsValidIndex(bitArrayIndex))
	{
		if (BlackValueState[bitArrayIndex])
		{
			switch (InValueType)
			{
				case ERAIBalckboardValueEnum::VE_CurrentTargetActor:
					{
						if (CurrentTargetActor.IsValid() == false)
							return nullptr;
						return CurrentTargetActor.Get();
					}
				case ERAIBalckboardValueEnum::VE_ForceTarget:
					{
						if (ForceTargetActor.IsValid() == false)
							return nullptr;
						return ForceTargetActor.Get();
					}
				case ERAIBalckboardValueEnum::VE_LeaderToFollow:
				{
					if (LeaderToFollow.IsValid() == false)
						return nullptr;
					return LeaderToFollow.Get();
				}
				case ERAIBalckboardValueEnum::VE_KeepSafeTarget:
				{
					if (KeepSafeTargetActor.IsValid() == false)
						return nullptr;
					return KeepSafeTargetActor.Get();
				}
			}
		}
	}
	return nullptr;
}

bool URAI_Blackboard::GetBlackboardBoolValue(ERAIBalckboardValueEnum InValueType)
{
	int32 bitArrayIndex = (int32)InValueType;
	if (BlackValueState.IsValidIndex(bitArrayIndex))
	{
		return BlackValueState[bitArrayIndex];
	}

	return false;
}

bool URAI_Blackboard::IsSetBlackboardValue(ERAIBalckboardValueEnum InValueType)
{
	int32 bitArrayIndex = (int32)InValueType;
	if (BlackValueState.IsValidIndex(bitArrayIndex))
		return BlackValueState[bitArrayIndex];

	return false;
}

void URAI_Blackboard::ClearBlackboardValue(ERAIBalckboardValueEnum InValueType)
{
	int32 bitArrayIndex = (int32)InValueType;
	if( BlackValueState.IsValidIndex(bitArrayIndex) )
		BlackValueState[bitArrayIndex] = false;
}

void URAI_Blackboard::ClearAllBlackboardValue()
{
	BlackValueState.Init(false, (int32)ERAIBalckboardValueEnum::VE_Max);
}