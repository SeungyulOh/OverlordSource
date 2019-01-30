// Fill out your copyright notice in the Description page of Project Settings.

#include "ProjectR.h"
#include "RBuffIcon.h"


#include "UtilFunctionIntegrated.h"

#include "Blueprint/WidgetLayoutLibrary.h"

#define ICON_BUFF_ACCURACY		FName("BuffIcon_Buff_Accuracy");
#define ICON_BUFF_ARANGE		FName("BuffIcon_Buff_AttackRange");
#define ICON_BUFF_ASPEED		FName("BuffIcon_Buff_AttackSpeed");
#define ICON_BUFF_AVOID			FName("BuffIcon_Buff_Avoid");
#define ICON_BUFF_CRIDAMAGE		FName("BuffIcon_Buff_CriticalDamage");
#define ICON_BUFF_CRIRATE		FName("BuffIcon_Buff_CriticalRate");
#define ICON_BUFF_CRIRESIDMG	FName("BuffIcon_Buff_CriticalResistDamage");
#define ICON_BUFF_CRIRESI		FName("BuffIcon_Buff_CriticalResistValue");
#define ICON_BUFF_MATTACK		FName("BuffIcon_Buff_MagicalAttack");
#define ICON_BUFF_MDEF			FName("BuffIcon_Buff_MagicalDefence");
#define ICON_BUFF_MCRIRATE		FName("BuffIcon_Buff_MagicCriticalRate");
#define ICON_BUFF_MSPEED		FName("BuffIcon_Buff_MoveSpeed");
#define ICON_BUFF_PARRY			FName("BuffIcon_Buff_Parry");
#define ICON_BUFF_PATTACK		FName("BuffIcon_Buff_PhysicalAttack");
#define ICON_BUFF_PDEF			FName("BuffIcon_Buff_PhysicalDefence");
#define ICON_BUFF_PIERCE		FName("BuffIcon_Buff_Pierce");

#define ICON_DEBUFF_ACCURACY	FName("BuffIcon_Debuff_Accuracy");
#define ICON_DEBUFF_ARANGE		FName("BuffIcon_Debuff_AttackRange");
#define ICON_DEBUFF_ASPEED		FName("BuffIcon_Debuff_AttackSpeed");
#define ICON_DEBUFF_AVOID		FName("BuffIcon_Debuff_Avoid");
#define ICON_DEBUFF_CRIDAMAGE	FName("BuffIcon_Debuff_CriticalDamage");
#define ICON_DEBUFF_CRIRATE		FName("BuffIcon_Debuff_CriticalRate");
#define ICON_DEBUFF_CRIRESIDMG	FName("BuffIcon_Debuff_CriticalResistDamage");
#define ICON_DEBUFF_CRIRESI		FName("BuffIcon_Debuff_CriticalResistValue");
#define ICON_DEBUFF_MATTACK		FName("BuffIcon_Debuff_MagicalAttack");
#define ICON_DEBUFF_MDEF		FName("BuffIcon_Debuff_MagicalDefence");
#define ICON_DEBUFF_MCRIRATE	FName("BuffIcon_Debuff_MagicCriticalRate");
#define ICON_DEBUFF_MSPEED		FName("BuffIcon_Debuff_MoveSpeed");
#define ICON_DEBUFF_PARRY		FName("BuffIcon_Debuff_Parry");
#define ICON_DEBUFF_PATTACK		FName("BuffIcon_Debuff_PhysicalAttack");
#define ICON_DEBUFF_PDEF		FName("BuffIcon_Debuff_PhysicalDefence");
#define ICON_DEBUFF_PIERCE		FName("BuffIcon_Debuff_Pierce");

#define ICON_AIRBORNE_IN		FName("BuffIcon_CC_Airborne");
#define ICON_BLEED_IN			FName("BuffIcon_CC_Bleed");
#define ICON_BLINDNESS_IN		FName("BuffIcon_CC_Blindness");
#define ICON_CAPTIVATE_IN		FName("BuffIcon_CC_Captivate");
#define ICON_DOWN_IN			FName("BuffIcon_CC_Down");
#define ICON_DRAIN				FName("BuffIcon_CC_Drain");
#define ICON_FEAR_IN			FName("BuffIcon_CC_Fear");
#define ICON_FIRE_IN			FName("BuffIcon_CC_Fire");
#define ICON_HOLD_IN			FName("BuffIcon_CC_Hold");
#define ICON_ICE_IN				FName("BuffIcon_CC_Ice");
#define ICON_MARK_DAGGER		FName("BuffIcon_CC_MarkDagger");
#define ICON_POISON_IN			FName("BuffIcon_CC_Poison");
#define ICON_TAUNT				FName("BuffIcon_CC_Provoke");
#define ICON_REFLECT_DAMAGE		FName("BuffIcon_CC_ReflectDamage"); 
#define ICON_SILENCE			FName("BuffIcon_CC_Silence");
#define ICON_STUN_IN			FName("BuffIcon_CC_Stun");
// #define ICON_FIRE_DA			FName("Icon_Fire_Damage");
// #define ICON_BLEED_DA		FName("Icon_Bleeding_Damage");
// #define ICON_DOWN_DA			FName("Icon_Down_Damage");
// #define ICON_FEAR_DA			FName("Icon_Fear_Damage");
// #define ICON_ICE_DA			FName("Icon_Ice_Damage");
// #define ICON_HOLD_DA			FName("Icon_Hold_Damage");
// #define ICON_POISON_DA		FName("Icon_Poison_Damage");
// #define ICON_STUN_DA			FName("Icon_Stun_Damage");
// #define ICON_REFLECTDMG		FName("Icon_ReflectDamage");

void URBuffIcon::NativeConstruct()
{
	Super::NativeConstruct();

	/*if (IsSpawnAni)
	{
		StartAni = GetAni("IconSpawn");
	}*/
}

void URBuffIcon::NativeDestruct()
{
	Super::NativeDestruct();
}

void URBuffIcon::OnWidgetRebuilt()
{
	Super::OnWidgetRebuilt();

	if (Image_Icon)
	{
		if (TempCustomSize.IsZero() == false)
		{
			//Image_Icon->Brush.TintColor = BGTint;
			auto CanvasSlot = UWidgetLayoutLibrary::SlotAsCanvasSlot(Image_Icon);
			CanvasSlot->SetSize(TempCustomSize);
		}
	}
	StartAni = GetAni("IconSpawn");
	/*if (IsSpawnAni)
	{
		StartAni = GetAni("IconSpawn");
	}*/
}

void URBuffIcon::SetAbnormalState(EAbnormalSlotTypeEnum InAbnormal)
{
	FName Key;
	switch (InAbnormal)
	{
		case EAbnormalSlotTypeEnum::VE_None:
		{
			CurrentSlot = InAbnormal;
			this->SetVisibility(ESlateVisibility::Collapsed);
		}
		break;
		case EAbnormalSlotTypeEnum::VE_Slot_Vampire:
			Key = ICON_DRAIN;
		break;
		case EAbnormalSlotTypeEnum::VE_Slot_Stun:
			Key = ICON_STUN_IN;
		break;
		case EAbnormalSlotTypeEnum::VE_Slot_Down:
			Key = ICON_DOWN_IN;
		break;
		case EAbnormalSlotTypeEnum::VE_Slot_Fear:
			Key = ICON_FEAR_IN;
		break;
		case EAbnormalSlotTypeEnum::VE_Slot_Airborne:
			Key = ICON_AIRBORNE_IN;
		break;
		case EAbnormalSlotTypeEnum::VE_Slot_Blindness:
			Key = ICON_BLINDNESS_IN;
			break;
		case EAbnormalSlotTypeEnum::VE_Slot_Mark_Dagger:
			Key = ICON_MARK_DAGGER;
			break;
		case EAbnormalSlotTypeEnum::VE_Slot_Hold:
			Key = ICON_HOLD_IN;
		break;
		case EAbnormalSlotTypeEnum::VE_Slot_Silence:
			Key = ICON_SILENCE;
		break;
		case EAbnormalSlotTypeEnum::VE_Slot_Provoke:
			Key = ICON_TAUNT;
		break;
		case EAbnormalSlotTypeEnum::VE_Slot_Ice:
		case EAbnormalSlotTypeEnum::VE_Slot_Freezing:
			Key = ICON_ICE_IN;
		break;
		case EAbnormalSlotTypeEnum::VE_Slot_Poison_Yoseph:
		case EAbnormalSlotTypeEnum::VE_Slot_Poison_Jin:
		case EAbnormalSlotTypeEnum::VE_Slot_Poison_Zagan:
			Key = ICON_POISON_IN;
		break;
		case EAbnormalSlotTypeEnum::VE_Slot_Bleeding_Than:
		case EAbnormalSlotTypeEnum::VE_Slot_Bleeding_Buddy:
		case EAbnormalSlotTypeEnum::VE_Slot_Bleeding_Bartor:
			Key = ICON_BLEED_IN;
		break;
		case EAbnormalSlotTypeEnum::VE_Slot_Fire_Flame:
		case EAbnormalSlotTypeEnum::VE_Slot_Ignite_Flame:
		case EAbnormalSlotTypeEnum::VE_Slot_Fire_Akhan:
		case EAbnormalSlotTypeEnum::VE_Slot_Fire_Scarlet:
		case EAbnormalSlotTypeEnum::VE_Slot_Ignite_Scarlet:
			Key = ICON_FIRE_IN;
		break;
		default:
		break;
	}

	if (Key.IsNone())
		return;

	CurrentSlot = InAbnormal;

	if (Image_Icon)
	{
		auto Image = UUIFunctionLibrary::GetIconImageFromTable(Key);
		UUIFunctionLibrary::SetImageBrush(Image, Image_Icon);
		SetVisibility(ESlateVisibility::SelfHitTestInvisible);
	}

	if (NotiMaterial)
	{
		NotiMaterial->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
	}

	if (IsSpawnAni)
	{
#ifdef RULE_TESTCHANGE
		if (StartAni)
		{
			PlayAnimation(StartAni);
		}
#else
		BP_PlayAniSpawn();
#endif
	}
}

void URBuffIcon::SetStatUpDownState(EAbnormalSlotTypeEnum InAbnormal)
{
	FName Key;
	switch (InAbnormal)
	{
		case EAbnormalSlotTypeEnum::VE_None:
		{
			CurrentSlot = InAbnormal;
			this->SetVisibility(ESlateVisibility::Collapsed);
		}
		break;

		// buff
		case EAbnormalSlotTypeEnum::VE_Slot_StatUpPhysicalAttack:
			Key = ICON_BUFF_PATTACK;
		break;
		case EAbnormalSlotTypeEnum::VE_Slot_StatUpMagicalAttack:
			Key = ICON_BUFF_MATTACK;
		break;
		case EAbnormalSlotTypeEnum::VE_Slot_StatUpPhysicalDefence:
			Key = ICON_BUFF_PDEF;
		break;
		case EAbnormalSlotTypeEnum::VE_Slot_StatUpMagicalDefence:
			Key = ICON_BUFF_MDEF;
		break;
		case EAbnormalSlotTypeEnum::VE_Slot_StatUpAccuracy:
			Key = ICON_BUFF_ACCURACY;
		break;
		case EAbnormalSlotTypeEnum::VE_Slot_StatUpAvoid:
			Key = ICON_BUFF_AVOID;
		break;
		case EAbnormalSlotTypeEnum::VE_Slot_StatUpAttackSpeed:
			Key = ICON_BUFF_ASPEED;
		break;
		case EAbnormalSlotTypeEnum::VE_Slot_StatUpAttackRange:
			Key = ICON_BUFF_ARANGE;
		break;
		case EAbnormalSlotTypeEnum::VE_Slot_StatUpMoveSpeed:
			Key = ICON_BUFF_MSPEED;
		break;
		case EAbnormalSlotTypeEnum::VE_Slot_StatUpCriticalRate:
			Key = ICON_BUFF_CRIRATE;
		break;
		case EAbnormalSlotTypeEnum::VE_Slot_StatUpCriticalDamage:
			Key = ICON_BUFF_CRIDAMAGE;
		break;
		case EAbnormalSlotTypeEnum::VE_Slot_StatUpPierceValue:
			Key = ICON_BUFF_PIERCE;
		break;
// 		case EAbnormalSlotTypeEnum::VE_Slot_StatUpParryValue:
// 			Key = ICON_BUFF_PARRY;
// 		break;
// 		case EAbnormalSlotTypeEnum::VE_Slot_StatUpCriticalResistValue:
// 			Key = ICON_BUFF_CRIRESI;
// 		break;
// 		case EAbnormalSlotTypeEnum::VE_Slot_StatUpCriticalResistDamage:
// 			Key = ICON_BUFF_CRIRESIDMG;
// 		break;
// 		case EAbnormalSlotTypeEnum::VE_Slot_StatUpMagicCriticalRate:
// 			Key = ICON_BUFF_MCRIRATE;
// 		break;
		case EAbnormalSlotTypeEnum::VE_Slot_ReflectDamage:
			Key = ICON_REFLECT_DAMAGE;
		break;

		// debuff
		case EAbnormalSlotTypeEnum::VE_Slot_StatDownPhysicalAttack:
			Key = ICON_BUFF_PATTACK;
		break;
		case EAbnormalSlotTypeEnum::VE_Slot_StatDownMagicalAttack:
			Key = ICON_DEBUFF_MATTACK;
		break;
		case EAbnormalSlotTypeEnum::VE_Slot_StatDownPhysicalDefence:
			Key = ICON_DEBUFF_PDEF;
		break;
		case EAbnormalSlotTypeEnum::VE_Slot_StatDownMagicalDefence:
			Key = ICON_DEBUFF_MDEF;
		break;
		case EAbnormalSlotTypeEnum::VE_Slot_StatDownAccuracy:
			Key = ICON_DEBUFF_ACCURACY;
		break;
		case EAbnormalSlotTypeEnum::VE_Slot_StatDownAvoid:
			Key = ICON_DEBUFF_AVOID;
		break;
		case EAbnormalSlotTypeEnum::VE_Slot_StatDownAttackSpeed:
			Key = ICON_DEBUFF_ASPEED;
		break;
		case EAbnormalSlotTypeEnum::VE_Slot_StatDownAttackRange:
			Key = ICON_DEBUFF_ARANGE;
		break;
		case EAbnormalSlotTypeEnum::VE_Slot_StatDownMoveSpeed:
			Key = ICON_DEBUFF_MSPEED;
		break;
		case EAbnormalSlotTypeEnum::VE_Slot_StatDownCriticalRate:
			Key = ICON_DEBUFF_CRIRATE;
		break;
		case EAbnormalSlotTypeEnum::VE_Slot_StatDownCriticalDamage:
			Key = ICON_DEBUFF_CRIDAMAGE;
		break;
		case EAbnormalSlotTypeEnum::VE_Slot_StatDownPierceValue:
			Key = ICON_DEBUFF_PIERCE;
		break;
// 		case EAbnormalSlotTypeEnum::VE_Slot_StatDownParryValue:
// 			Key = ICON_DEBUFF_PARRY;
// 		break;
// 		case EAbnormalSlotTypeEnum::VE_Slot_StatDownCriticalResistValue:
// 			Key = ICON_DEBUFF_CRIRESI;
// 		break;
// 		case EAbnormalSlotTypeEnum::VE_Slot_StatDownCriticalResistDamage:
// 			Key = ICON_DEBUFF_CRIRESIDMG;
// 		break;
// 		case EAbnormalSlotTypeEnum::VE_Slot_StatDownMagicCriticalRate:
// 			Key = ICON_DEBUFF_MCRIRATE;
// 		break;

		default:
		break;
	}

	if (Key.IsNone())
		return;

	CurrentSlot = InAbnormal;

	if (Image_Icon)
	{
		auto Image = UUIFunctionLibrary::GetIconImageFromTable(Key);
		UUIFunctionLibrary::SetImageBrush(Image, Image_Icon);
		Image_Icon->SetColorAndOpacity(FLinearColor(1, 1, 1, 1));
		SetVisibility(ESlateVisibility::SelfHitTestInvisible);
	}

	if (NotiMaterial)
	{
		NotiMaterial->SetVisibility(ESlateVisibility::Collapsed);
	}

	/*if (IsSpawnAni && StartAni)
	{
		PlayAnimation(StartAni);
	}*/
}

void URBuffIcon::SetSkillIconState(ESkillIconShapeEnum InShape)
{
	FName Key;
	switch (InShape)
	{
		case ESkillIconShapeEnum::VE_Stun_Induce:		Key = ICON_STUN_IN;		break;
		case ESkillIconShapeEnum::VE_Down_Induce:		Key = ICON_DOWN_IN;		break;
		case ESkillIconShapeEnum::VE_Fear_Induce:		Key = ICON_FEAR_IN;		break;
		case ESkillIconShapeEnum::VE_Hold_Induce:		Key = ICON_HOLD_IN;		break;
		case ESkillIconShapeEnum::VE_Poison_Induce:		Key = ICON_POISON_IN;	break;
		case ESkillIconShapeEnum::VE_Bleeding_Induce:	Key = ICON_BLEED_IN;	break;
		case ESkillIconShapeEnum::VE_Fire_Induce:		Key = ICON_FIRE_IN;		break;
		case ESkillIconShapeEnum::VE_Ice_Induce:		Key = ICON_ICE_IN;		break;
		/*
		case ESkillIconShapeEnum::VE_Stun_Damage:		Key = ICON_STUN_DA;		break;
		case ESkillIconShapeEnum::VE_Down_Damage:		Key = ICON_DOWN_DA;		break;
		case ESkillIconShapeEnum::VE_Fear_Damage:		Key = ICON_FEAR_DA;		break;
		case ESkillIconShapeEnum::VE_Hold_Damage:		Key = ICON_HOLD_DA;		break;
		case ESkillIconShapeEnum::VE_Poison_Damage:		Key = ICON_POISON_DA;	break;
		case ESkillIconShapeEnum::VE_Bleeding_Damage:	Key = ICON_BLEED_DA;	break;
		case ESkillIconShapeEnum::VE_Fire_Damage:		Key = ICON_FIRE_DA;		break;
		case ESkillIconShapeEnum::VE_Ice_Damage:		Key = ICON_ICE_DA;		break;
		*/

		case ESkillIconShapeEnum::VE_None:
		default:
		{
			this->SetVisibility(ESlateVisibility::Collapsed);
		}
		break;
	}

	if (Key.IsNone())
		return;

	if (Image_Icon)
	{
		auto Image = UUIFunctionLibrary::GetIconImageFromTable(Key);
		UUIFunctionLibrary::SetImageBrush(Image, Image_Icon);
		Image_Icon->SetColorAndOpacity(FLinearColor(1, 1, 1, 1));
		SetVisibility(ESlateVisibility::SelfHitTestInvisible);
	}

	if (NotiMaterial)
	{
		NotiMaterial->SetVisibility(ESlateVisibility::Collapsed);
	}
}

void URBuffIcon::SetIconImage(FName InKey)
{
	if (Image_Icon)
	{
		auto Image = UUIFunctionLibrary::GetIconImageFromTable(InKey);
		UUIFunctionLibrary::SetImageBrush(Image, Image_Icon);
		Image_Icon->SetColorAndOpacity(FLinearColor(1, 1, 1, 1));
		SetVisibility(ESlateVisibility::SelfHitTestInvisible);
	}

}

void URBuffIcon::BP_PlayAniSpawn()
{
	PlayAni(TEXT("IconSpawn"));
}
