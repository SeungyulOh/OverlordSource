// Fill out your copyright notice in the Description page of Project Settings.

#include "ProjectR.h"
#include "UtilFunctionIntegrated.h"
#include "ResourceFunctionLibrary.h"

#include "Global/TableManager.h"


/*
UTexture2D* UResourceFunctionLibrary::GetSkillAttributeTypeTexture(EAttackAttributeTypeEnum AttributeType)
{
	switch (AttributeType)
	{
	case EAttackAttributeTypeEnum::VE_Dark:
		return UUtilFunctionLibrary::GetTexture2DFromTable(Skill_Attribute_Dark);
		break;
	case EAttackAttributeTypeEnum::VE_Fire:
		return UUtilFunctionLibrary::GetTexture2DFromTable(Skill_Attribute_Fire);
		break;
	case EAttackAttributeTypeEnum::VE_Holy:
		return UUtilFunctionLibrary::GetTexture2DFromTable(Skill_Attribute_Holy);
		break;
	case EAttackAttributeTypeEnum::VE_Ice:
		return UUtilFunctionLibrary::GetTexture2DFromTable(Skill_Attribute_Ice);
		break;
	case EAttackAttributeTypeEnum::VE_Lightning:
		return UUtilFunctionLibrary::GetTexture2DFromTable(Skill_Attribute_Lightning);
		break;
	case EAttackAttributeTypeEnum::VE_Physical:
		return UUtilFunctionLibrary::GetTexture2DFromTable(Skill_Attribute_Physical);
		break;
	case EAttackAttributeTypeEnum::VE_Poison:
		return UUtilFunctionLibrary::GetTexture2DFromTable(Skill_Attribute_Poison);
		break;
	default:
		break;
	}

	return nullptr;
}

*/

/*
UTexture2D* UResourceFunctionLibrary::GetHeroAttributeTypeTexture(EAttackAttributeTypeEnum AttributeType)
{
	switch (AttributeType)
	{
	case EAttackAttributeTypeEnum::VE_Dark:
		return UUtilFunctionLibrary::GetTexture2DFromTable(Hero_Attribute_Dark);
		break;
	case EAttackAttributeTypeEnum::VE_Fire:
		return UUtilFunctionLibrary::GetTexture2DFromTable(Hero_Attribute_Fire);
		break;
	case EAttackAttributeTypeEnum::VE_Holy:
		return UUtilFunctionLibrary::GetTexture2DFromTable(Hero_Attribute_Holy);
		break;
	case EAttackAttributeTypeEnum::VE_Ice:
		return UUtilFunctionLibrary::GetTexture2DFromTable(Hero_Attribute_Ice);
		break;
	case EAttackAttributeTypeEnum::VE_Lightning:
		return UUtilFunctionLibrary::GetTexture2DFromTable(Hero_Attribute_Lightning);
		break;
	case EAttackAttributeTypeEnum::VE_Physical:
		return UUtilFunctionLibrary::GetTexture2DFromTable(Hero_Attribute_Physical);
		break;
	case EAttackAttributeTypeEnum::VE_Poison:
		return UUtilFunctionLibrary::GetTexture2DFromTable(Hero_Attribute_Poison);
		break;
	default:
		break;
	}

	return nullptr;
}
*/

UTexture2D* UResourceFunctionLibrary::GetSkillAttackPropertyTypeTexture(ECharacterPropertyTypeEnum InAttackPropertyType)
{
	switch (InAttackPropertyType)
	{
	case ECharacterPropertyTypeEnum::VE_MagicAttackPower:
		return UUtilFunctionLibrary::GetTexture2DFromTable(Skill_Attribute_Lightning);
		break;

	case ECharacterPropertyTypeEnum::VE_PhysicalAttackPower:
		return UUtilFunctionLibrary::GetTexture2DFromTable(Skill_Attribute_Physical);
		break;
	default:
		break;
	}

	return nullptr;
}

UTexture2D* UResourceFunctionLibrary::GetHeroAttackPropertyTypeTexture(ECharacterPropertyTypeEnum InAttackPropertyType)
{
	switch (InAttackPropertyType)
	{
	case ECharacterPropertyTypeEnum::VE_MagicAttackPower:
		return UUtilFunctionLibrary::GetTexture2DFromTable(Hero_Attribute_Lightning);
		break;

	case ECharacterPropertyTypeEnum::VE_PhysicalAttackPower:
		return UUtilFunctionLibrary::GetTexture2DFromTable(Hero_Attribute_Physical);
		break;
	default:
		break;
	}

	return nullptr;
}


UTexture2D* UResourceFunctionLibrary::GetHeroClassTypeTexture(EClassTypeEnum ClassType)
{
	switch (ClassType)
	{
	case EClassTypeEnum::VE_Tanker:
		return UUtilFunctionLibrary::GetTexture2DFromTable(Hero_Class_Job03);
		break;
	case EClassTypeEnum::VE_MeleeDealer:
		return UUtilFunctionLibrary::GetTexture2DFromTable(Hero_Class_Job01);
		break;
	case EClassTypeEnum::VE_RangeDealer:
		return UUtilFunctionLibrary::GetTexture2DFromTable(Hero_Class_Job02);
		break;
	case EClassTypeEnum::VE_Healer:
		return UUtilFunctionLibrary::GetTexture2DFromTable(Hero_Class_Job04);
		break;
	case EClassTypeEnum::VE_Supporter:
		return UUtilFunctionLibrary::GetTexture2DFromTable(Hero_Class_Job05);
		break;
	default:
		break;
	}

	return nullptr;
}

UPaperSprite* UResourceFunctionLibrary::GetHeroClassTypeSprite(EClassTypeEnum ClassType)
{
	switch (ClassType)
	{
	case EClassTypeEnum::VE_Tanker:
		return UUtilFunctionLibrary::GetSpriteFromTable(Hero_Class_Job03);
	break;
	case EClassTypeEnum::VE_MeleeDealer:
		return UUtilFunctionLibrary::GetSpriteFromTable(Hero_Class_Job01);
	break;
	case EClassTypeEnum::VE_RangeDealer:
		return UUtilFunctionLibrary::GetSpriteFromTable(Hero_Class_Job02);
	break;
	case EClassTypeEnum::VE_Healer:
		return UUtilFunctionLibrary::GetSpriteFromTable(Hero_Class_Job04);
	break;
	case EClassTypeEnum::VE_Supporter:
		return UUtilFunctionLibrary::GetSpriteFromTable(Hero_Class_Job05);
	break;
	case EClassTypeEnum::VE_MagicDealer:
		return UUtilFunctionLibrary::GetSpriteFromTable(Hero_Class_Job06);
		break;
	default:
	break;
	}

	return nullptr;
}

UPaperSprite* UResourceFunctionLibrary::GetHeroContinentTypeSprite(EContinentTypeEnum ContinentType)
{
	switch (ContinentType)
	{
	case EContinentTypeEnum::Luna:
		return UUtilFunctionLibrary::GetSpriteFromTable(Continent_MidLandName);
		break;
	case EContinentTypeEnum::Lux:
		return UUtilFunctionLibrary::GetSpriteFromTable(Continent_DarkLandName);
		break;
	case EContinentTypeEnum::Nox:
		return UUtilFunctionLibrary::GetSpriteFromTable(Continent_OutLandName);
		break;
	case EContinentTypeEnum::Solona:
		return UUtilFunctionLibrary::GetSpriteFromTable(Continent_UnderLandName);
		break;
	default:
		break;
	}

	return nullptr;
}


FText UResourceFunctionLibrary::GetHeroClassTypeName(EClassTypeEnum ClassType)
{
	switch (ClassType)
	{
	case EClassTypeEnum::VE_Tanker:
		return UUtilFunctionLibrary::GetLocalizedString( EStringTableType::VE_UI, Hero_Class_Job01_Name );
		break;		
	case EClassTypeEnum::VE_MeleeDealer:
		return UUtilFunctionLibrary::GetLocalizedString(EStringTableType::VE_UI, Hero_Class_Job02_Name);
		break;
	case EClassTypeEnum::VE_RangeDealer:
		return UUtilFunctionLibrary::GetLocalizedString(EStringTableType::VE_UI, Hero_Class_Job03_Name);
		break;
	case EClassTypeEnum::VE_Healer:
		return UUtilFunctionLibrary::GetLocalizedString(EStringTableType::VE_UI, Hero_Class_Job04_Name);
		break;
	case EClassTypeEnum::VE_Supporter:
		return UUtilFunctionLibrary::GetLocalizedString(EStringTableType::VE_UI, Hero_Class_Job05_Name);
		break;
	case EClassTypeEnum::VE_MagicDealer:
		return UUtilFunctionLibrary::GetLocalizedString(EStringTableType::VE_UI, Hero_Class_Job06_Name);
		break;
	default:
		break;
	}

	return FText();
}

// TODO
FText UResourceFunctionLibrary::GetHeroEquipTypeName(EItemEquipTypeEnum ItemEquipType)
{
	switch (ItemEquipType)
	{
	case EItemEquipTypeEnum::VE_Sword:
		return UUtilFunctionLibrary::GetLocalizedString(EStringTableType::VE_UI, Hero_EquipType_Sword_Name);
		break;
	case EItemEquipTypeEnum::VE_Dagger:
		return UUtilFunctionLibrary::GetLocalizedString(EStringTableType::VE_UI, Hero_EquipType_Dagger_Name);
		break;
	case EItemEquipTypeEnum::VE_Hammer:
		return UUtilFunctionLibrary::GetLocalizedString(EStringTableType::VE_UI, Hero_EquipType_Hammer_Name);
		break;
	case EItemEquipTypeEnum::VE_Rod:
		return UUtilFunctionLibrary::GetLocalizedString(EStringTableType::VE_UI, Hero_EquipType_Rod_Name);
		break;

	case EItemEquipTypeEnum::VE_Bow:
		return UUtilFunctionLibrary::GetLocalizedString(EStringTableType::VE_UI, Hero_EquipType_Bow_Name);
		break;
	case EItemEquipTypeEnum::VE_Crossbow:
		return FText::FromString(TEXT("Crossbow"));
		break;
	case EItemEquipTypeEnum::VE_Gun:
		return FText::FromString(TEXT("Gun"));
		break;

	case EItemEquipTypeEnum::VE_Staff:
		return UUtilFunctionLibrary::GetLocalizedString(EStringTableType::VE_UI, Hero_EquipType_Staff_Name);
		break;
	case EItemEquipTypeEnum::VE_Magic:
		return UUtilFunctionLibrary::GetLocalizedString(EStringTableType::VE_UI, Hero_EquipType_Magic_Name);
		break;
	case EItemEquipTypeEnum::VE_Wand:
		return UUtilFunctionLibrary::GetLocalizedString(EStringTableType::VE_UI, Hero_EquipType_Wand_Name);
		break;

	case EItemEquipTypeEnum::VE_HeavyArmor:
		return FText::FromString(TEXT("HeavyArmor"));
		break;
	case EItemEquipTypeEnum::VE_LightArmor:
		return FText::FromString(TEXT("LightArmor"));
		break;
	case EItemEquipTypeEnum::VE_FabricArmor:
		return FText::FromString(TEXT("FabricArmor"));
		break;

	case EItemEquipTypeEnum::VE_HeavyPants:
		return FText::FromString(TEXT("HeavyPants"));
		break;
	case EItemEquipTypeEnum::VE_LightPants:
		return FText::FromString(TEXT("LightPants"));
		break;
	case EItemEquipTypeEnum::VE_FabricPants:
		return FText::FromString(TEXT("FabricPants"));
		break;

	case EItemEquipTypeEnum::VE_Necklace:
		return FText::FromString(TEXT("Necklace"));
		break;
	case EItemEquipTypeEnum::VE_Ring:
		return UUtilFunctionLibrary::GetLocalizedString(EStringTableType::VE_Item, "ring");
		break;
	}

	return FText();
}

UPaperSprite* UResourceFunctionLibrary::GetHeroEquipTypeSprite(EItemEquipTypeEnum ItemEquipType)
{
	FName ItemEquipTypeName = UUtilFunctionLibrary::GetEnumToName(TEXT("EItemEquipTypeEnum"), (int32)ItemEquipType);
	FItemEquipTypeTableInfo* ItemEquipTypeInfo = UTableManager::GetInstancePtr()->GetItemEquipTypeTableRow(ItemEquipTypeName);
	
	if (ItemEquipTypeInfo == nullptr)
		return nullptr;

	return ItemEquipTypeInfo->IconImg.LoadSynchronous();
}

FText UResourceFunctionLibrary::GetGuildGradeTypeName(EGUILD_GRADE GuildGradeTyp)
{
	switch (GuildGradeTyp)
	{
	case EGUILD_GRADE::MEMBER:
		return UUtilFunctionLibrary::GetLocalizedString(EStringTableType::VE_UI, FName("UI_GuildMember_Member"));
		break;
	case EGUILD_GRADE::ELDER:
		return UUtilFunctionLibrary::GetLocalizedString(EStringTableType::VE_UI, FName("UI_GuildMember_Elder"));
		break;
	case EGUILD_GRADE::MASTER:
		return UUtilFunctionLibrary::GetLocalizedString(EStringTableType::VE_UI, FName("UI_GuildMember_Founder"));
		break;
	}

	return FText();
}
