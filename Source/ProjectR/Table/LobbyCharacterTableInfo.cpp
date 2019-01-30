// Fill out your copyright notice in the Description page of Project Settings.

#include "ProjectR.h"
#include "LobbyCharacterTableInfo.h"
#include "GlobalIntegrated.h"
#include "UtilFunctionIntegrated.h"




// UClass*	FLobbyCharacterTableInfo::GetLobbyActorBlueprintClass()
// {
// 	return UUtilFunctionLibrary::GetLobbyActorClassFromAssetID(LobbyActor);
// }

FLobbyCharacterTableInfo* FLobbyCharacterTableInfo::LookUp(FName InDisplayName)
{
	TArray<FName> LobbyCharacterTableRowNames = UTableManager::GetInstancePtr()->DTLobbyCharacterTableInfo->GetRowNames();
	for (FName LobbyCharacterRowName : LobbyCharacterTableRowNames)
	{
		FLobbyCharacterTableInfo* LobbyCharacterInfo = UTableManager::GetInstancePtr()->GetLobbyCharacterTableRow(LobbyCharacterRowName);

		if (LobbyCharacterInfo->DisplayName == InDisplayName)
		{
			return LobbyCharacterInfo;
		}
	}

	return nullptr;
}

FLobbyCharacterTableInfo* FLobbyCharacterTableInfo::ChooseRandomly()
{
	// Randomly choose per 'Frequence', assuming it to be weight of possibility
	
	TArray<FName> RowNames = UTableManager::GetInstancePtr()->GetLobbyCharacterTableRowNames();
	TArray<FLobbyCharacterTableInfo*> LobbyCharacterList;

	for (auto& RowName : RowNames)
	{
		FLobbyCharacterTableInfo* LobbyCharacterTableInfo = UTableManager::GetInstancePtr()->GetLobbyCharacterTableRow(RowName);
		if (LobbyCharacterTableInfo)
		{
			for (int32 Count = 0; Count < LobbyCharacterTableInfo->Frequence; ++Count)
			{
				LobbyCharacterList.Emplace(LobbyCharacterTableInfo);
			}
		}
	}
	checkf(LobbyCharacterList.Num() != 0, TEXT("FLobbyCharacterTableInfo::ChooseRandomly: no lobby character to choose from"));

	int32 ChosenIndex = FMath::RandHelper(LobbyCharacterList.Num());
	return LobbyCharacterList[ChosenIndex];
}
