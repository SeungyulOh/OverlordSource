// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Object.h"
#include "SharedConstants/GlobalConstants.h"
#include "BaseCondition.generated.h"

/**
 * 
 */
UENUM(BlueprintType)		//"BlueprintType" is essential to include
enum class EConditionStateEnum : uint8
{
	VE_None							UMETA(DisplayName = "None"),
	VE_RootCondition				UMETA(DisplayName = "RootCondition"),
	VE_Condition_And				UMETA(DisplayName = "ConditionAnd"),
	VE_Condition_Or					UMETA(DisplayName = "ConditionOr"),
	VE_Condition_Not				UMETA(DisplayName = "ConditionNot"),
	VE_Condition_EqualValue			UMETA(DisplayName = "ConditionEqualValue"),
	VE_Condition_GreaterValue		UMETA(DisplayName = "ConditionGreaterValue"),
	VE_Condition_GreaterEqualValue	UMETA(DisplayName = "ConditionGreaterEqualValue"),
	VE_Condition_LessValue			UMETA(DisplayName = "ConditionLessValue"),
	VE_Condition_LessEqualValue		UMETA(DisplayName = "ConditionLessEqualValue"),
	VE_Condition_SkillUse			UMETA(DisplayName = "ConditionSkillUse"),
	VE_Condition_BuffEnter			UMETA(DisplayName = "ConditionBuffEnter"),
};


UENUM(BlueprintType)		//"BlueprintType" is essential to include
enum class EConditionPropertyStateEnum : uint8
{
	VE_None									UMETA(DisplayName = "None"),
	VE_Property_Value						UMETA(DisplayName = "PropertyValue"),
	VE_Property_CharacterValue				UMETA(DisplayName = "PropertyCharacterValue"),
	VE_Property_CharacterHpPercentValue		UMETA(DisplayName = "PropertyCharacterHpPercentValue"),
};

UENUM(BlueprintType)		//"BlueprintType" is essential to include
enum class EConditionOperatorEnum : uint8
{
	VE_None						UMETA(DisplayName = "None"),
	VE_Equal					UMETA(DisplayName = "Equal"),
	VE_Greater					UMETA(DisplayName = "Greater"),
	VE_GreaterEqual				UMETA(DisplayName = "GreaterEqual"),
	VE_Less						UMETA(DisplayName = "Less"),
	VE_LessEqual				UMETA(DisplayName = "LessEqual"),
};




UCLASS()
class PROJECTR_API UBaseCondition : public UObject
{
	GENERATED_BODY()

public:
	
	UFUNCTION(BlueprintCallable, Category = Skill)
	virtual void	InitCondition(AActor* Owner, int32 InCheckCount = 0);

	UFUNCTION(BlueprintCallable, Category = Skill)
	virtual bool	IsCheck();

	virtual void	Update( float deltaTime );
	virtual bool	Check();

public:
	////temp
	//UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = Skill)
	//ESkillCastingTypeEnum			CastingType = ESkillCastingTypeEnum::VE_None;

protected:
	UPROPERTY()
	TWeakObjectPtr< AActor >			OwnerCharacter;

	UPROPERTY()
	TWeakObjectPtr< AActor >			CheckTarget;

	int32								CheckCount = 0;
	int32								CurentCheckCount = 0;
	bool								CheckStart = false;
};
