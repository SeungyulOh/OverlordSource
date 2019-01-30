// Fill out your copyright notice in the Description page of Project Settings.

#include "ProjectR.h"
#include "CharacterBattleProperty.h"



void FCharacterBattleProperty::Clear()
{
	AttackPropertyType = ECharacterPropertyTypeEnum::VE_PhysicalAttackPower;
	AttackType = EAttackTypeEnum::VE_Melee;

	// attack value
	PhysicalAttackPower = 0;
	MagicAttackPower = 0;
	AccuracyValue = 0;
	PierceValue = 0;
	AttackSpeed = 0.0f;
	CriticalValue = 0;
	CriticalDamageRate_Scale = 0;
	AttackRange = 0.0f;


	// defence value
	Armor = 0;
	MagicResistance = 0;
	MaxHP = 0;
	DodgeValue = 0;

	// etc value
	MoveSpeed = 0.0f;
	
}
