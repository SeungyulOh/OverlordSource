// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "SharedConstants/GlobalConstants.h"
#include "Kismet/BlueprintFunctionLibrary.h"

#include "ResourceFunctionLibrary.generated.h"

//SkillAttribute Type Texture Key ( DT_Texture2DResource)
#define Skill_Attribute_Physical	FName("Skill_Attribute_Physical")
#define Skill_Attribute_Dark		FName("Skill_Attribute_Dark")
#define Skill_Attribute_Fire		FName("Skill_Attribute_Fire")
#define Skill_Attribute_Holy		FName("Skill_Attribute_Holy")
#define Skill_Attribute_Ice			FName("Skill_Attribute_Ice")
#define Skill_Attribute_Lightning	FName("Skill_Attribute_Lightning")
#define Skill_Attribute_Poison		FName("Skill_Attribute_Poison")

//HeroClassType Texture Key(DT_Texture2DResource)
#define Hero_Class_Job01 FName("Hero_Class_Job01")
#define Hero_Class_Job02 FName("Hero_Class_Job02")
#define Hero_Class_Job03 FName("Hero_Class_Job03")
#define Hero_Class_Job04 FName("Hero_Class_Job04")
#define Hero_Class_Job05 FName("Hero_Class_Job05")
#define Hero_Class_Job06 FName("Hero_Class_Job04")

#define Hero_Class_Job01_Name FName("USK_up_Decksetting_Tanker")
#define Hero_Class_Job02_Name FName("USK_up_Decksetting_NearDealer")
#define Hero_Class_Job03_Name FName("USK_up_Decksetting_RangeDealer")
#define Hero_Class_Job04_Name FName("USK_up_Decksetting_Healer")
#define Hero_Class_Job05_Name FName("USK_up_Decksetting_Suppoter")
#define Hero_Class_Job06_Name FName("USK_up_Decksetting_MagicDealer")

// ContinentType Sprite key(DT_SpriteResource)
#define Continent_MidLandName		FName("BG_Continent_01_Sprite")
#define Continent_DarkLandName		FName("BG_Continent_04_Sprite")
#define Continent_OutLandName		FName("BG_Continent_02_Sprite")
#define Continent_UnderLandName		FName("BG_Continent_03_Sprite")

//HeroAttribute Texture Key(DT_Texture2DResource)
#define Hero_Attribute_Physical		FName("Hero_Attribute_Physical")
#define Hero_Attribute_Fire			FName("Hero_Attribute_Fire")
#define Hero_Attribute_Ice			FName("Hero_Attribute_Ice")
#define Hero_Attribute_Lightning	FName("Hero_Attribute_Lightning")
#define Hero_Attribute_Poison		FName("Hero_Attribute_Poison")
#define Hero_Attribute_Holy			FName("Hero_Attribute_Holy")
#define Hero_Attribute_Dark			FName("Hero_Attribute_Dark")


#define Hero_EquipType_Sword_Name		FName("USK_up_Sward")
#define Hero_EquipType_Wand_Name		FName("USK_up_Wand")
#define Hero_EquipType_Staff_Name		FName("USK_up_Staff")
#define Hero_EquipType_Dagger_Name		FName("USK_up_Dagger")
#define Hero_EquipType_Magic_Name		FName("USK_up_Cube")
#define Hero_EquipType_Bow_Name			FName("USK_up_Bow")
#define Hero_EquipType_Longsword_Name	FName("USK_up_TwoHandSward")
#define Hero_EquipType_Hammer_Name		FName("USK_up_Ax")
#define Hero_EquipType_Rod_Name			FName("USK_up_Speare")

#define Hero_EquipType_Heavy_Name		FName("Heavy")
#define Hero_EquipType_Light_Name		FName("Light")
#define Hero_EquipType_Leather_Name		FName("Leather")
#define Hero_EquipType_Fabric_Name		FName("Fabric")

/**
 * 
 */
UCLASS()
class PROJECTR_API UResourceFunctionLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	//UFUNCTION(BlueprintCallable, Category = ResourceFunction)
	//static UTexture2D* GetSkillAttributeTypeTexture(EAttackAttributeTypeEnum AttributeType);
	
	UFUNCTION(BlueprintCallable, Category = ResourceFunction)
	static UTexture2D* GetHeroClassTypeTexture(EClassTypeEnum ClassType);

	UFUNCTION(BlueprintCallable, Category = ResourceFunction)
	static UPaperSprite* GetHeroClassTypeSprite(EClassTypeEnum ClassType);	

	UFUNCTION(BlueprintCallable, Category = ResourceFunction)
	static FText GetHeroClassTypeName(EClassTypeEnum ClassType);

	UFUNCTION(BlueprintCallable, Category = ResourceFunction)
	static UPaperSprite* GetHeroContinentTypeSprite(EContinentTypeEnum ContinentType);

	UFUNCTION(BlueprintCallable, Category = ResourceFunction)
	static FText GetHeroEquipTypeName(EItemEquipTypeEnum ItemEquipType);

	UFUNCTION(BlueprintCallable, Category = ResourceFunction)
	static UPaperSprite* GetHeroEquipTypeSprite(EItemEquipTypeEnum ItemEquipType);

	//UFUNCTION(BlueprintCallable, Category = ResourceFunction)
	//static UTexture2D* GetHeroAttributeTypeTexture(EAttackAttributeTypeEnum AttributeType);

	UFUNCTION(BlueprintCallable, Category = ResourceFunction)
	static UTexture2D* GetSkillAttackPropertyTypeTexture(ECharacterPropertyTypeEnum InAttackPropertyType);


	UFUNCTION(BlueprintCallable, Category = ResourceFunction)
	static UTexture2D* GetHeroAttackPropertyTypeTexture(ECharacterPropertyTypeEnum InAttackPropertyType);

	static FText GetGuildGradeTypeName(EGUILD_GRADE GuildGradeTyp);
};
