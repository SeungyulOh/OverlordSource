// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Object.h"
#include "Table/CharacterTableInfo.h"
#include "SharedConstants/GlobalConstants.h"
#include "AutoSpawner.generated.h"

UENUM(BlueprintType)		//"BlueprintType" is essential to include
enum class ESpawnGroup : uint8
{
	VE_Group1		UMETA(DisplayName = "Group1"),
	VE_Group2		UMETA(DisplayName = "Group2"),
	VE_Group3		UMETA(DisplayName = "Group3"),
	VE_Max,
};

USTRUCT(BlueprintType)
struct PROJECTR_API FSpawnCheck
{
	GENERATED_USTRUCT_BODY()
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "DataStructure")
	TArray<bool> SpawnCheck;
};

/**
 * 
 */
UCLASS()
class PROJECTR_API UAutoSpawner : public UObject
{
	GENERATED_BODY()

public:
	UAutoSpawner();
	
	bool Init();
	bool AutoSpawnCharacters(FTransform SpawnTransform = FTransform::Identity);

private:
	bool GetValidSpawnPointByGroup(const int32 SlotIndex, FVector& ValidSpawnPos);
	bool SpawnCharacters(FTransform SpawnTransform);
	bool GetValidSpawnPosition(ESpawnGroup SpawnGroup, FVector& ValidSpawnPos);
	bool SetSelectedCharacterProperty();
	bool SetCurrentAttackTargetProperty();
	bool SetSpawnCharacterProperty( const int32 SlotIndex);
	bool GetOffSetY(ESpawnGroup SpawnGroup, float& OffsetY);
	bool GetOffSetYSelectedAttackTypeNormal(ESpawnGroup SpawnGroup, float& OffsetY);
	bool GetOffSetYSelectedAttackTypeRange(ESpawnGroup SpawnGroup, float& OffSetY);
	bool GetOffSetYSelectedAttackTypeHeal(ESpawnGroup SpawnGroup, float& OffSetY);
	bool GetOffSetY_Group2WithGroup3(float& OffsetY);
	bool GetOffSetY_Group3WithGroup2(float& OffsetY);
	bool ContainSpawnCharacters(FString& HeroKey);

	// Not Used
	//bool IsValidSPawnLocation(const float& OffsetX, const float& OffsetY);
	//bool CheckCharacterAttackType(EAttackTypeEnum AttackType, const FName& HeroName);
	
private:
	UPROPERTY()
	TArray<FSpawnCheck>	SpawnPositionCheck;
	UPROPERTY()
	AActor*					SelectedCharacter = nullptr;
	UPROPERTY()
	AActor*					CurrentAttackTarget = nullptr;
	float					SelectedCharacterUnitRadius;
	float					SelectedCharacterAttackRange;
	float					SpawnCharacterUnitRadius;
	float					EnemyRadius;
	float					SpawnCharacterAttackRange;
	int32					SelectedCharacterSlotIndex;
	int32					SpawnCharacterSlotIndex;
	EAttackTypeEnum			SelectedCharacterAttackType;
	EAttackTypeEnum			SpawnCharacterAttackType;

};
