// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include "SharedConstants/GlobalConstants.h"
#include "AvatarAbilityTableInfo.generated.h"

/**
 * 
 */
USTRUCT(BlueprintType)
struct PROJECTR_API FAvatarAbilityTableInfo : public FTableRowBase
{
	GENERATED_USTRUCT_BODY()
	
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = AvatarAbilityTableInfo)
	int32 AbilityIndex;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = AvatarAbilityTableInfo)
	int32 MaxPoint;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = AvatarAbilityTableInfo)
	int32 Point1_gold;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = AvatarAbilityTableInfo)
	int32 Point2_gold;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = AvatarAbilityTableInfo)
	int32 Point3_gold;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = AvatarAbilityTableInfo)
	int32 Point4_gold;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = AvatarAbilityTableInfo)
	int32 Point5_gold;

	//UPROPERTY(EditAnywhere, BlueprintReadWrite,  Category = "AvatarAbilityTableInfo|None")
	//EClassTypeEnum Ability_Class;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = AvatarAbilityTableInfo)
	EAvatarAbilityType Ability_type;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = AvatarAbilityTableInfo)
	float Ability_Value1;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = AvatarAbilityTableInfo)
	float Ability_Value2;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = AvatarAbilityTableInfo)
	float Ability_Value3;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = AvatarAbilityTableInfo)
	float Ability_Value4;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = AvatarAbilityTableInfo)
	float Ability_Value5;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = AvatarAbilityTableInfo)
	FName Icon; //ImageFromTable

	float GetValue(int32 Point)
	{
		float ReturnValue = 0.0f;
		switch (Point)
		{
		case 1:	ReturnValue = Ability_Value1;	break;
		case 2:	ReturnValue = Ability_Value2;	break;
		case 3:	ReturnValue = Ability_Value3;	break;
		case 4:	ReturnValue = Ability_Value4;	break;
		case 5:	ReturnValue = Ability_Value5;	break;
		}
		return ReturnValue;
	}
};