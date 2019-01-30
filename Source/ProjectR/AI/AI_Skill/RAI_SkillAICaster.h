// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include "Character/Components/FunctionalComponents/SkillEntityComponent.h"
#include "RAI_SkillCondition_Base.h"
#include "Table/SkillTableInfo_V2.h"
#include "RAI_SkillAICaster.generated.h"

UCLASS()
class PROJECTR_API USkillInfoWithCondition : public UObject
{	
	GENERATED_BODY()

public:
	FName						SkillID;
	EPriorityTypeEnum			AISearchPreferType;
	UPROPERTY()
	URAI_SkillCondition_Base*	SkillCondtion;

	float						RestElapsedTime = 0.0f;
	float						MaxRestTime = 0.0f;
};

/** Class needed to support InterfaceCast<IToStringInterface>(Object) */
UCLASS()
class  PROJECTR_API URAI_SkillAICaster : public UObject
{
	GENERATED_BODY()
			
public:
	URAI_SkillAICaster();

	virtual void		Init(AActor* InOwnerActor, TArray< FName >& InSkillList, float InCheckInterval);
	virtual void		Clear();

	/*
	 deprecated...
	*/
	virtual bool		CheckReadyToCastSkill( float DeltaTime );	
	virtual void		IncreaseCheckIndex();

	/*
	Use condition check module
	*/
	virtual bool		CheckReadyToCastSkill_V2(float DeltaTime);
	

protected:
	virtual void		AddNewSkillCondition(FName SkillID);
	virtual FSkillTableInfo_V2*	GetSkillTableInfo(FName InSkillID) { return nullptr; };

	virtual	bool		CheckSkillConditions(float DeltaTime);
	virtual bool		CanCastSkill();
	virtual bool		UpdateElapsedTime(float DeltaTime);




protected:
	//TArray< FName >			CheckSkillList;

	int32					CurrentCheckIndex = 0;
	float					CheckInterval = 5.0f;

	float					CheckElapsedTime = 0.0f;
	bool					bActivateTimeInterval = true;

	FName										CurrentReadyToFireSkill;
	TWeakObjectPtr< AActor >					CheckTargetActor;
	FVector										CheckTargetLocation;

	UPROPERTY()
	TArray< USkillInfoWithCondition* >			CheckSkillList;

	/*
	Cache
	*/
	TWeakObjectPtr< AActor >					OwnerActor;
	TWeakObjectPtr< USkillEntityComponent >		OwnerSkillEntityComponent;
};
