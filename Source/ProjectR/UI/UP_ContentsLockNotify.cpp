// Fill out your copyright notice in the Description page of Project Settings.

#include "ProjectR.h"

#include "UP_ContentsLockNotify.h"
#include "Table/ContentsUnlockTableInfo.h"
#include "Global/TableManager.h"




void UUP_ContentsLockNotify::RefreshUI(EOVERLORD InContentsType)
{
	if (Text_UnLockLevel)
	{
		int32 Index = (int32)InContentsType;
		FName IndexName = FName(*FString::FromInt(Index));
		FContentsUnlockTableInfo* TableInfo = UTableManager::GetInstancePtr()->GetContentsUnlockRow(IndexName);
		if (TableInfo)
		{
			int32 level = TableInfo->Level;
			if (level == 0)
			{
				level = 99;
			}
			FString Text = TEXT("Lv.") + FString::FromInt(level);
			Text_UnLockLevel->SetText(FText::FromString(Text));
		}
	}
}