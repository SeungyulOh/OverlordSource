// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "SharedConstants/GlobalConstants.h"
#include "EntityBaseProperty.generated.h"

struct FEntityBasePropertyInfo;

/** Class needed to support InterfaceCast<IToStringInterface>(Object) */
UINTERFACE(MinimalAPI)
class  UEntityBaseProperty : public UInterface
{
	GENERATED_UINTERFACE_BODY()
};

class IEntityBaseProperty
{
	GENERATED_IINTERFACE_BODY()
	
public:

	virtual EEntityTypeEnum				GetEntityType() PURE_VIRTUAL(IEntityBaseProperty::GetEntityType, return EEntityTypeEnum::VE_None;);
	virtual void						SetEntityType( EEntityTypeEnum InEntityType ) PURE_VIRTUAL(IEntityBaseProperty::SetEntityType, return;);
		
	virtual int32						GetLevel() PURE_VIRTUAL(IEntityBaseProperty::GetLevel, return 1;);
	virtual void						SetLevel(int32 InLevel) PURE_VIRTUAL(IEntityBaseProperty::SetLevel, return;);

	virtual int32						GetGrade() PURE_VIRTUAL(IEntityBaseProperty::GetGrade, return 1;);
	virtual void						SetGrade(int32 InGrade) PURE_VIRTUAL(IEntityBaseProperty::SetGrade, return;);

	virtual EClassTypeEnum				GetJobClass() PURE_VIRTUAL(IEntityBaseProperty::GetJobClass, return EClassTypeEnum::VE_Tanker;);
	virtual void						SetJobClass( EClassTypeEnum InJobClass) PURE_VIRTUAL(IEntityBaseProperty::SetJobClass, return;);

	virtual bool						IsReady() PURE_VIRTUAL(IEntityBaseProperty::IsReady, return false;);
	virtual void						SetReady(bool InReady) PURE_VIRTUAL(IEntityBaseProperty::SetReady, return;);
	
	virtual FName						GetCharacterID() PURE_VIRTUAL(IEntityBaseProperty::GetCharacterID, return FName(););
	virtual void						SetCharacterID(FName InCharacterID ) PURE_VIRTUAL(IEntityBaseProperty::SetCharacterID, return;);

	virtual ECharacterCategoryTypeEnum	GetCharacterCategory() PURE_VIRTUAL(IEntityBaseProperty::GetCharacterCategory, return ECharacterCategoryTypeEnum::VE_None;);
	virtual void						SetCharacterCategory(ECharacterCategoryTypeEnum InCharacterCategory) PURE_VIRTUAL(IEntityBaseProperty::SetCharacterCategory, return;);
	
	virtual int32						GetUserKID() PURE_VIRTUAL(IEntityBaseProperty::GetUserKID, return 0;);
	virtual void						SetUserKID(int32 InUserKID ) PURE_VIRTUAL(IEntityBaseProperty::SetUserKID, return;);

	virtual FString						GetHeroUD() PURE_VIRTUAL(IEntityBaseProperty::GetHeroUD, return FString(););
	virtual void						SetHeroUD(FString InHeroUD ) PURE_VIRTUAL(IEntityBaseProperty::SetHeroUD, return;);

	virtual int32						GetPlayerUK() PURE_VIRTUAL(IEntityBaseProperty::GetPlayerUK, return 0;);
	virtual void						SetPlayerUK(int32 InPlayerUK) PURE_VIRTUAL(IEntityBaseProperty::SetPlayerUK, return;);

	virtual int32						GetPartyID() PURE_VIRTUAL(IEntityBaseProperty::GetPartyID, return 0;);
	virtual void						SetPartyID(int32 InGroupKey) PURE_VIRTUAL(IEntityBaseProperty::SetPartyID, return;);

	virtual int32						GetGroupKey() PURE_VIRTUAL(IEntityBaseProperty::GetGroupKey, return 0;);
	virtual void						SetGroupKey(int32 InGroupKey) PURE_VIRTUAL(IEntityBaseProperty::SetGroupKey, return;);

	virtual ECharacterPropertyTypeEnum	GetAttachPropertyType() PURE_VIRTUAL(IEntityBaseProperty::GetAttachPropertyType, return ECharacterPropertyTypeEnum::VE_None;);
	virtual void						SetAttachPropertyType(ECharacterPropertyTypeEnum InPropertyType) PURE_VIRTUAL(IEntityBaseProperty::SetAttachPropertyType, return;);

	virtual bool						GetAccountCharacter() PURE_VIRTUAL(IEntityBaseProperty::GetAccountCharacter, return false;);
	virtual void						SetAccountCharacter(bool InFlag) PURE_VIRTUAL(IEntityBaseProperty::SetAccountCharacter, return;);

	virtual int32						GetRealTimeParam() PURE_VIRTUAL(IEntityBaseProperty::GetRealTimeParam, return 0;);
	virtual void						SetRealTimeParam(int32 InParam) PURE_VIRTUAL(IEntityBaseProperty::SetRealTimeParam, return;);
	virtual void						ReSetRealTimeParam() PURE_VIRTUAL(IEntityBaseProperty::ReSetRealTimeParam, return;);

	virtual void						InitStartProperty() PURE_VIRTUAL(IEntityBaseProperty::InitStartProperty, return;);

	//-------------------------------------------------------------------------------------------------------
	// Creation interface
	//-------------------------------------------------------------------------------------------------------
	
	virtual void						InitMembers() PURE_VIRTUAL(IEntityBaseProperty::InitMembers, return;);
	virtual void						InitHeroBase() PURE_VIRTUAL(IEntityBaseProperty::InitHeroBase, return;);
	virtual void						InitHeroAfterCreation() PURE_VIRTUAL(IEntityBaseProperty::InitHeroAfterCreation, return;);
	virtual void						SetSpawnEvent() PURE_VIRTUAL(IEntityBaseProperty::SetSpawnEvent, return;);
	virtual void						SetSpawnEventForArena() PURE_VIRTUAL(IEntityBaseProperty::SetSpawnEventForArena, return;);
	virtual void						CharacterTickEnabled(bool TickEnabled) PURE_VIRTUAL(IEntityBaseProperty::CharacterTickEnabled, return;);
	virtual void						AttachUI() PURE_VIRTUAL(IEntityBaseProperty::AttachUI, return;);
	virtual void						AttachComponents() PURE_VIRTUAL(IEntityBaseProperty::AttachComponents, return;);

	// BlueprintImplementableEvent
	virtual void						OnInit() {};
	virtual void						OnInitHero() {};
	virtual void						OnInitHeroAfterCreation() {};

	virtual void						OnChangeSituation() {};
	virtual void						OnSetAttackTarget(AActor* TargetActor) {};

	virtual FEntityBasePropertyInfo*	GetEntityBasePropertyInfo() PURE_VIRTUAL(IEntityBaseProperty::GetEntityBasePropertyInfo, return nullptr;);
};

/**
*
*/
USTRUCT(BlueprintType)
struct PROJECTR_API FEntityBasePropertyInfo
{
	GENERATED_USTRUCT_BODY()

public:
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = EntityBaseProperty)
	int32					Level = 1;		// cache for preparing crate CharacteBattleInfo

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = EntityBaseProperty)
	int32					Grade = 1;		// cache for preparing crate CharacteBattleInfo

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = EntityBaseProperty)
	EClassTypeEnum			JobClassType = EClassTypeEnum::VE_Tanker;
		
	bool					bIsReady = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = EntityBaseProperty)
	FName					CharacterID;
		
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = EntityBaseProperty)
	ECharacterCategoryTypeEnum	CharacterCategory = ECharacterCategoryTypeEnum::VE_None;

	bool					AccountCharacter = false;
	int32					UserKID;

	int32					PlayerUK = PLAYER_UK_NONE;
	int32					PartyID = PartyID_NONE;
	int32					GroupKey = GroupKey_SINGLE_NONE;
	
	FString					HeroUD; //test

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = EntityBaseProperty)
	EEntityTypeEnum			EntityType = EEntityTypeEnum::VE_None;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = EntityBaseProperty)
	ECharacterPropertyTypeEnum AttackPropertyType;

	int32					RealTimeParam = 0;

	/*
	for test
	*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = EntityBaseProperty)
	bool							bCreateUI = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = EntityBaseProperty)
	bool							bTestInit = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = EntityBaseProperty)
	bool							bDrawSkillShape = false;


};
