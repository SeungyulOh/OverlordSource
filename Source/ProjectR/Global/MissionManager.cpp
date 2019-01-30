
#include "ProjectR.h"
#include "MissionManager.h"

void UMissionManager::Initialize()
{
	ResetData();
}

void UMissionManager::ResetData()
{
// 	MissionStatus.activated = false;
// 	MissionStatus.floor = 0;
// 	MissionStatus.missions.Empty();
// 	MissionStatus.timeLeft = 0;
}

void UMissionManager::AddID(FString InId)
{
	if(!SelectedIDList.Contains(InId))
		SelectedIDList.Emplace(InId);
}

void UMissionManager::RemoveID(FString InId)
{
	if (SelectedIDList.Contains(InId))
		SelectedIDList.Remove(InId);
}

bool UMissionManager::CheckID(FString InId)
{
	if (SelectedIDList.Contains(InId))
	{
		RemoveID(InId);
		return false;
	}
	else
	{
		AddID(InId);
		return true;
	}
}

bool UMissionManager::IsContainsID(FString InId)
{
	return SelectedIDList.Contains(InId);
}
