// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Object.h"
#include "Party.generated.h"

/**
 * For managing parties in PvP contents. Parties are created by game contents and members added by GameObjectManager.
 */
UCLASS()
class PROJECTR_API UParty : public UObject
{
	GENERATED_BODY()
	
public:
	UParty();

	void Initialize();

	void AddMember(int32 PlayerUK);
	void RemoveMember(int32 PlayerUK);

	bool IsMyPartyPlayer(int32 PlayerUK);
	bool IsPartyAllDie();
	void GetPartyMember(TArray< TWeakObjectPtr<AActor> >& outPartyMemberList);
	
public:

	void StopSkillEffect();

	//////////////////////////////////////////////////////////////////////////
	// PartyAI
	void StartPartyAI(bool auto_active_skill);
	void StopPartyAI();
		
	UPROPERTY()
	TArray<int32>		MemberPlayerUKList;
};
