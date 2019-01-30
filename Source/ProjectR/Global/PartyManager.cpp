// Fill out your copyright notice in the Description page of Project Settings.

#include "ProjectR.h"
#include "PartyManager.h"
#include "Character/Party/Party.h"
#include "RGameInstance.h"
#include "Network/HttpRequestClient.h"
#include "UtilFunctionIntegrated.h"

UPartyManager::UPartyManager()
{
}

void UPartyManager::Initialize(/*URGameInstance* instance*/)
{
//	RGameInstance = instance;
	
	ClearPartyList();
}

void UPartyManager::Tick(float DeltaTime)
{
}

UParty* UPartyManager::CreateParty(int32 PartyID)
{
	URGameInstance* RGameInstance = RGAMEINSTANCE(this);
	UParty* PartyData = nullptr;

	if (PartyMap.Contains(PartyID))
	{
		PartyData = PartyMap[PartyID];
	}
	else
	{
		PartyData = NewObject< UParty >((UObject*)GetTransientPackage(), UParty::StaticClass());
		PartyData->Initialize();

		PartyMap.Emplace(PartyID, PartyData);
		PartyList.Emplace(PartyData);
	}

	return PartyData;
}

void UPartyManager::AddPartyMember(int32 PartyID, int32 PlayerUK)
{
	TWeakObjectPtr<UParty> PartyData = GetPartyByPartyID(PartyID);
	if (PartyData.IsValid() == false)
	{
		PartyData = CreateParty(PartyID);
	}

	if (PartyData.IsValid())
	{
		PartyData->AddMember(PlayerUK);
	}
}

void UPartyManager::RemovePartyMember(int32 PartyID, int32 PlayerUK)
{
	TWeakObjectPtr<UParty> PartyData = GetPartyByPartyID(PartyID);
	if (PartyData.IsValid())
	{
		PartyData->RemoveMember(PlayerUK);
	}
}

void UPartyManager::MovePartyMember(int32 BeforePartyID, int32 AfterPartyID, int32 PlayerUK)
{
	RemovePartyMember(BeforePartyID, PlayerUK);
	AddPartyMember(AfterPartyID, PlayerUK);
}

void UPartyManager::ClearPartyList()
{
	PartyMap.Empty();
	PartyList.Empty();
}

TWeakObjectPtr<UParty> UPartyManager::GetPartyByPartyID(int32 PartyID)
{
	if (PartyMap.Contains(PartyID))
	{
		return *PartyMap.Find(PartyID);
	}
	return nullptr;
}

TWeakObjectPtr<UParty> UPartyManager::GetMyParty()
{
	URGameInstance* RGameInstance = RGAMEINSTANCE(this);
	if (IsValid(RGameInstance) /*&& IsValid(RGameInstance->GameObjectMgr)*/)
	{
// 		int32 MyPartyID = RGameInstance->GameObjectMgr->GetMyPartyID();
//		int32 MyPartyID = UObjectFunctionLibrary::GetMyPartyID();
		return GetPartyByPartyID(UObjectFunctionLibrary::GetMyPartyID());
	}

	return nullptr;
}

void UPartyManager::GetPartyList(TArray< TWeakObjectPtr< UParty > >& outPartyList)
{
	outPartyList = PartyList;
}

bool UPartyManager::GetPartyMemberyPartyID(int32 PartyID, TArray< TWeakObjectPtr<AActor> >& outPartyMemberList)
{
	auto PartyData = GetPartyByPartyID(PartyID);
	if (PartyData.IsValid())
	{
		PartyData->GetPartyMember(outPartyMemberList);
		return true;
	}

	return false;
}