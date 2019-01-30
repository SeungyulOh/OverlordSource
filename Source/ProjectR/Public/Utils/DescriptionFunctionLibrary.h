// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include "SharedConstants/GlobalConstants.h"
#include "UI/UIDefs.h"

#include "Kismet/BlueprintFunctionLibrary.h"

#include "CustomStructures/SharedStructures/CharacterBattleInfo.h"

#include "Table/ItemTableInfo.h"
#include "Table/SkillTableInfo_V2.h"
#include "Table/LevelAppliedTables/WorkedSkillTableInfo.h"
#include "Table/LevelAppliedTables/WorkedItemTableInfo.h"

#include "DescriptionFunctionLibrary.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTR_API UDescriptionFunctionLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()
	
public:
	/*
	======================================================================================================================================================================================================================================
	Hero_Property
	======================================================================================================================================================================================================================================
	*/
	// legacy
	static FString GetHeroPropertyString(const FString& PropertyName, UCharacterBattleInfo* InCharacterBattleInfo);
	static FString GetHeroPropertyPercentString(const FString& PropertyName, UCharacterBattleInfo* InCharacterBattleInfo);

	// NewDesign
	static FText HeroPropertyValueToText(ECharacterPropertyTypeEnum CharacterProperty, UCharacterBattleInfo* InCharacterBattleInfo);
	static FText HeroItemPropertyToText(ECharacterPropertyTypeEnum CharacterProperty, UCharacterBattleInfo* InCharacterBattleInfo);
	static FText HeroAdditionalPropertyToText(ECharacterPropertyTypeEnum CharacterProperty, UCharacterBattleInfo* InCharacterBattleInfo);

	static FName HeroPropertyToUIStringKey(ECharacterPropertyTypeEnum CharacterProperty);

	static bool IsHeroPropertyFloat(ECharacterPropertyTypeEnum CharacterProperty);
	// (design)
	static bool IsHeroPropertyPerMil(ECharacterPropertyTypeEnum CharacterProperty);
	static bool IsHeroPropertyContributedByItem(ECharacterPropertyTypeEnum CharacterProperty, UCharacterBattleInfo* InCharacterBattleInfo);
	static bool IsHeroPorpetryAdditional(ECharacterPropertyTypeEnum CharacterProperty, UCharacterBattleInfo* InCharacterBattleInfo);
	/*
	======================================================================================================================================================================================================================================
	Skill
	======================================================================================================================================================================================================================================
	*/
	static void MakeFormatNamedArguments_WorkedSkillTableInfo(const FString& SkillDesc, const FWorkedSkillTableInfo* InWorkedSkillTableInfo, FFormatNamedArguments& InTargetInfoFormatNamedArguments, const TSet<FString>& CalculatePropertys, bool bActiveSkill = false);
	static float GetSkillModifyValue(EScaleValueTypeEnum InScaleValueTypeEnum, float InBuffModifyValue, EBuffPropensityEnum InSkillBuffPropnensity, float InCharacterPropertyValue, bool bIsCalculate);

	/*
	======================================================================================================================================================================================================================================
	Item
	======================================================================================================================================================================================================================================
	*/

	static FString GetItemPropertyString(const FString& PropertyName, UCharacterBattleInfo* InCharacterBattleInfo);

	//float value with special precision
	UFUNCTION(BlueprintCallable, Category = "UI_DescriptionFuncionLibrary")
	static FString GetFloatToStringWithPrecision(float FloatValue, int32 Precision, bool IncludeLeadingZero = true);

	UFUNCTION(BlueprintCallable, Category = "UI_DescriptionFuncionLibrary")
	static FString GetIntToAbsIntToString(int32 IntValue);

	static FText GetItemEquipPositionToText(EItemEquipPositionEnum EquipPosition);

	/*
	======================================================================================================================================================================================================================================
	Description Unit 
	======================================================================================================================================================================================================================================
	*/
	static void MakeUnitNameArgument(FFormatNamedArguments& InFormatNamedArguments);

	static void MakePrecision(FString& InString, int32 FrontZeroCount = 2);
	//static FFormatNamedArguments UnitNameArgument;

	/*
	======================================================================================================================================================================================================================================
	Time
	======================================================================================================================================================================================================================================
	*/
	UFUNCTION(BlueprintCallable, Category = UDescriptionFunctionLibrary)
	static void GetTimeStr(float Time, FString& Min, FString& Sec);

	UFUNCTION(BlueprintCallable, Category = UDescriptionFunctionLibrary)
	static void GetTimeStrOneParam(float Time, FString& TimeStr);

	/*
	String UtilFunction
	*/
	UFUNCTION(BlueprintCallable, Category = UDescriptionFunctionLibrary)
	static FText GetLocalizedString(EStringTableType TableType, FName StringID);

	UFUNCTION(BlueprintCallable, Category = UDescriptionFunctionLibrary)
	static FName GetItemName(FName ItemID);

// 	UFUNCTION(BlueprintCallable, Category = UDescriptionFunctionLibrary)
// 	static FName GetWalletPropertyName(FName CurrencyType);

	static void GetTimeStringByInteger(FString& InTimeValue, int32 InTimeIntegerValue);
};
