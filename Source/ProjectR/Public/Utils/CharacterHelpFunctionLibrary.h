// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Kismet/BlueprintFunctionLibrary.h"
#include "SharedConstants/GlobalConstants.h"

#include "Character/Components/FunctionalComponents/BattleEntityComponent.h"
#include "Character/Components/FunctionalComponents/SkillEntityComponent.h"
#include "Character/Components/FunctionalComponents/EntityAIComponent.h"
#include "Character/Components/FunctionalComponents/EntityRenderComponent.h"
#include "Character/Components/FunctionalComponents/PathFindingComponent.h"

#include "Character/Components/AnimStateComponent.h"
#include "Character/Components/CustomCharacterMovementComponent.h"
#include "Character/Components/SkillAreaIndicatorMeshComponent.h"
#include "Character/Components/WidgetDecoratorComponent.h"
#include "Character/Components/BossComponents/BossBaseComponent.h"
#include "Character/Components/BossComponents/GigantComponent.h"
#include "Character/Components/RemoteMoveComponent.h"
#include "Character/BattleBaseAnimInstance.h"

#include "GameFramework/CharacterMovementComponent.h"

#include "CharacterHelpFunctionLibrary.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTR_API UCharacterHelpFunctionLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()
	
public:

	static FName GetHeroSkillIDByIndex(FName InHeroID, int32 InIndex);
	static void MakeHeroSkillStructure(FName InHeroID, int32 InLevel, int32 InIndex, FHERO_SKILL& OutHeroSkill);

	UFUNCTION(BlueprintCallable, Category = CharacterHelpFunction)
	static class URemoteMoveComponent* GetRemoteMoveComponent(AActor* TargetActor);
	UFUNCTION(BlueprintCallable, Category = CharacterHelpFunction)
	static class UBattleEntityComponent* GetBattleEntityComponent(AActor* TargetActor );
	UFUNCTION(BlueprintCallable, Category = CharacterHelpFunction)
	static class USkillEntityComponent* GetSkillEntityComponent(AActor* TargetActor);
	UFUNCTION(BlueprintCallable, Category = CharacterHelpFunction)
	static class UEntityAIComponent* GetEntityAIComponent(AActor* TargetActor);
	UFUNCTION(BlueprintCallable, Category = CharacterHelpFunction)
	static class UEntityRenderComponent* GetEntityRenderComponent(AActor* TargetActor);
	UFUNCTION(BlueprintCallable, Category = CharacterHelpFunction)
	static class UAnimStateComponent* GetAnimStateComponent(AActor* TargetActor);


	UFUNCTION(BlueprintCallable, Category = CharacterHelpFunction)
	static class UCharacterMovementComponent* GetCharacterMovementComponent(AActor* TargetActor);
	UFUNCTION(BlueprintCallable, Category = CharacterHelpFunction)
	static class UCustomCharacterMovementComponent* GetCustomCharacterMovementComponent(AActor* TargetActor);
	UFUNCTION(BlueprintCallable, Category = CharacterHelpFunction)
	static class USkeletalMeshComponent* GetSkeletalMeshComponent(AActor* TargetActor);
	UFUNCTION(BlueprintCallable, Category = CharacterHelpFunction)
	static class UCapsuleComponent* GetCapsuleComponent(AActor* InTargetActor);

	static TArray< USkillAreaIndicatorMeshComponent* > GetSkillAreaArray(AActor* InTargetActor);
	static class UWidgetDecoratorComponent* GetHpBarWidgetComponent(AActor* InTargetActor);

	UFUNCTION(BlueprintCallable, Category = CharacterHelpFunction)
	static class UBossBaseComponent* GetCharacterBossBaseComponent(AActor* TargetActor);
	static class UAkhanComponent* GetAkhanComponent(AActor* TargetActor);
	static class UGigantComponent* GetGigantComponent(AActor* TargetActor);

	UFUNCTION(BlueprintCallable, Category = CharacterHelpFunction)
	static class UVibrationComponent*				GetVibrationComponent(AActor* TargetActor);
	static class USpecialMovingComponent*			GetSpecialMovingComponent(AActor* TargetActor);
	static class UGridMapInspectorComponent*		GetGridMapInspectorComponent(AActor* TargetActor);
	static class UCustomChangeMaterialComponent*	GetCustomMaterialComponent(AActor* TargetActor);
	static class UEffectComponent*					GetEffectComponent(AActor* TargetActor);

	UFUNCTION(BlueprintCallable, Category = CharacterHelpFunction)
	static class UBattleBaseAnimInstance* GetCharacterBattleBaseAnimInstance(AActor* TargetActor);

	UFUNCTION(BlueprintCallable, Category = CharacterHelpFunction)
	static class UAnimInstance* GetCharacterAnimInstance(AActor* TargetActor);
	
	UFUNCTION(BlueprintCallable, Category = CharacterHelpFunction)
	static ECharacterCategoryTypeEnum GetCharacterCategory(AActor* TargetActor);
	UFUNCTION(BlueprintCallable, Category = CharacterHelpFunction)
	static EClassTypeEnum GetCharacterClassType(AActor* TargetActor);

	UFUNCTION(BlueprintCallable, Category = CharacterHelpFunction)
	static FName GetCharacterID(AActor* TargetActor);

	UFUNCTION(BlueprintCallable, Category = CharacterHelpFunction)
	static class UCharacterBattleInfo* GetBattleInfo(AActor* InTargetActor);

	UFUNCTION(BlueprintCallable, Category = CharacterHelpFunction)
	static bool IsDead(AActor* InTargetActor);
	
	UFUNCTION(BlueprintCallable, Category = CharacterHelpFunction)
	static bool IsRemoteEntity(AActor* InTargetActor);

	UFUNCTION(BlueprintCallable, Category = CharacterHelpFunction)
	static bool LaunchCharacter(AActor* DeadActor, AActor* FromActor, bool bXYOverride = true, bool bZOverride = true);

	UFUNCTION(BlueprintCallable, Category = CharacterHelpFunction)
	static void SetOriginalMaterial(AActor* TargetActor);

	static bool IsPossibleToSpawn(AActor* InTargetActor);

	

	static bool CastSlotSkill(AActor* InTargetActor, ESkillCategoryTypeEnum InSkillCategoryType = ESkillCategoryTypeEnum::VE_ActiveSkill03);


	template<class T>
	static T* GetComponent(AActor* TargetActor)
	{
		if (IsValid(TargetActor) == false)
			return nullptr;

		return Cast<T>(TargetActor->GetComponentByClass(T::StaticClass()));
	}

	template<class T>
	static TArray<T*> GetComponents(AActor* TargetActor)
	{
		if (IsValid(TargetActor) == false)
			return nullptr;

		return  Cast<T>(TargetActor->GetComponentsByClass(T::StaticClass()));
	}

	/*
	Transform
	*/
};