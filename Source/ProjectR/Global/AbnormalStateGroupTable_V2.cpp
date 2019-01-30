// Fill out your copyright notice in the Description page of Project Settings.

#include "ProjectR.h"
#include "AbnormalStateGroupTable_V2.h"
#include "Table/AbnormalStateGroupTableInfo_V2.h"

#include "SharedConstants/GlobalConstants.h"


void UAbnormalStateGroupTable_V2::Init()
{
	BuffSlotTypes.Empty();
	SlotGroupTypes.Init(TSet<EAbnormalSlotTypeEnum>(), (int32)EAbnormalGroupTypeEnum::VE_Max);

	TArray< FName > RowNames;
	UTableManager::GetInstancePtr()->GetDataTableRowNamesByType(EDataTableType::VE_AbnormalStateGroupTable, RowNames);
	for (FName& rowName : RowNames)
	{
		FAbnormalStateGroupTableInfo_V2* AbnormalStateGroupInfo = UTableManager::GetInstancePtr()->GetAbnormalStateGroupRow_V2( rowName );
		checkf(AbnormalStateGroupInfo != nullptr, TEXT("AbnormalStateGroupInfo is Empty"));

		// Is buff?
		if (AbnormalStateGroupInfo->BuffType == EAbnormalApplyTypeEnum::VE_Buff)
			BuffSlotTypes.Emplace(AbnormalStateGroupInfo->SlotType);

		// Identify group
		if(SlotGroupTypes.IsValidIndex((int32)AbnormalStateGroupInfo->Group))
			SlotGroupTypes[(int32)AbnormalStateGroupInfo->Group].Emplace(AbnormalStateGroupInfo->SlotType);
	}
}

void UAbnormalStateGroupTable_V2::BeginDestroy()
{
	BuffSlotTypes.Empty();
	SlotGroupTypes.Empty();

	Super::BeginDestroy();
}

bool UAbnormalStateGroupTable_V2::IsInBuff(EAbnormalSlotTypeEnum InSlotType)
{
	return BuffSlotTypes.Contains(InSlotType);
}

bool UAbnormalStateGroupTable_V2::IsInGroup(EAbnormalGroupTypeEnum InGroupType, EAbnormalSlotTypeEnum InSlotType)
{
	int32 groupIndex = (int32)InGroupType;
	if (SlotGroupTypes.IsValidIndex(groupIndex) == false)
		return false;

	return SlotGroupTypes[groupIndex].Contains(InSlotType);
}

