// Fill out your copyright notice in the Description page of Project Settings.

#include "ProjectR.h"
#include "GameDataManager.h"

#include "SharedConstants/GlobalConstants.h"
#include "GlobalIntegrated.h"

#include "UtilFunctionIntegrated.h"



void UGameDataManager::Initialize(/*URGameInstance* GameInstance*/)
{	
	URGameInstance* RGameInstance = RGAMEINSTANCE(this);
}



FLinearColor UGameDataManager::GetItemEnchantLevelColor(int32 InLevel)
{
	int32 Num = ItemEnchantColorList.Num();

	FSlateColor LastColor;
	if (Num > 0)
	{
		LastColor = ItemEnchantColorList[Num - 1].ItemEnchantColor;
	}

	for (FGameDataItemEnchantInfo& info : ItemEnchantColorList)
	{
		if (info.ItemEnchantLevel == InLevel)
		{
			return info.ItemEnchantColor.GetSpecifiedColor();
		}
	}

	return LastColor.GetSpecifiedColor();
}

FLinearColor UGameDataManager::GetWarningTextColor(FText InType)
{
	int32 Num = WarningTextColorList.Num();

	FSlateColor LastColor;
	if(Num > 0)
	{
		LastColor = WarningTextColorList[Num - 1].TextColor;
	}

	for (FGameDataWarningTextColorInfo& info : WarningTextColorList)
	{
		if (info.WarningTextColorType.EqualTo(InType))
		{
			return info.TextColor.GetSpecifiedColor();
		}
	}

	return LastColor.GetSpecifiedColor();
}

void UGameDataManager::SpawnNextTargetActor()
{
	URGameInstance* RGameInstance = RGAMEINSTANCE(this);
// 	if (IsValid(RGameInstance) == false)
// 		return;

	for (int32 i = 0; i < 5; ++i)
	{
		if (NextTargetActorList.IsValidIndex(i) && NextTargetActorList[i].IsValid())
			continue;
			
		if (!NextTargetActorPath.IsEmpty())
		{
			FTransform trans;
			trans.SetIdentity();

			AActor* actor = UUtilFunctionLibrary::SpawnPath(RGameInstance->GetWorld(), trans, NextTargetActorPath);
			if (IsValid(actor))
			{
				actor->SetActorHiddenInGame(true);
				NextTargetActorList.Emplace(Cast<ANextTargetActor>(actor));
			}
		}
	}
}

ANextTargetActor* UGameDataManager::GetNextTargetActor(bool InCheckPrevHide /*= true*/)
{
	if (NextTargetActorList.Num() == 0)
	{
		SpawnNextTargetActor();
	}

	if (NextTargetActorList.IsValidIndex(CurIdx) && NextTargetActorList[CurIdx].IsValid())
	{
		if (InCheckPrevHide && CurIdx != 0)
		{
			int32 Test = CurIdx - 1;
			NextTargetActorList[Test]->Hide();
		}

		return NextTargetActorList[CurIdx++].Get();
	}
	else
	{
		CurIdx = 0;
		if (NextTargetActorList.IsValidIndex(CurIdx) && NextTargetActorList[CurIdx].IsValid())
		{
			if (InCheckPrevHide && CurIdx == 0)
			{
				NextTargetActorList[4]->Hide();
			}
			return NextTargetActorList[CurIdx].Get();
		}
	}

	return nullptr;
}
