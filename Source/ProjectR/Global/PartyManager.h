// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Object.h"
#include "SharedConstants/GlobalConstants.h"
#include "Character/Party/Party.h"
#include "PartyManager.generated.h"

/**
 * For managing parties in PvP contents. Parties are created by game contents and members added by GameObjectManager.
 */
UCLASS()
class PROJECTR_API UPartyManager : public UObject
{
	GENERATED_BODY()
	
public:
	UPartyManager();

	void Initialize(/*URGameInstance* instance*/);
	void Tick(float DeltaTime);

	UParty* CreateParty(int32 PartyID);
	void AddPartyMember(int32 PartyID, int32 PlayerUK);
	void RemovePartyMember(int32 PartyID, int32 PlayerUK);
	void MovePartyMember(int32 BeforePartyID, int32 AfterPartyID, int32 PlayerUK);
	void ClearPartyList();

	TWeakObjectPtr<UParty> GetPartyByPartyID(int32 PartyID);
	TWeakObjectPtr<UParty> GetMyParty();
	void GetPartyList(TArray< TWeakObjectPtr< UParty > >& outPartyList);
	bool GetPartyMemberyPartyID(int32 PartyID, TArray< TWeakObjectPtr<AActor> >& outPartyMemberList);

private:
// 	UPROPERTY()
// 	URGameInstance*			RGameInstance = nullptr;

	UPROPERTY()
	TMap<int32, UParty*>	PartyMap;
	UPROPERTY()
	TArray< TWeakObjectPtr< UParty > >	PartyList;
	
};
