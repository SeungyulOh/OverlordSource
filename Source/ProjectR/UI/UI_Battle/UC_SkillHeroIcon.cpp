// Fill out your copyright notice in the Description page of Project Settings.

#include "ProjectR.h"
#include "UC_SkillHeroIcon.h"
//#include "HUDBase.h"

#include "UtilFunctionIntegrated.h"
#include "GlobalIntegrated.h"

#include "GameInfo/RInventory.h"
#include "Network/PacketDataStructures.h"
#include "Table/CharacterTableInfo.h"


void UUC_SkillHeroIcon::NativeConstruct()
{
	Super::NativeConstruct();
}

void UUC_SkillHeroIcon::SetHero(FName id)
{
	URGameInstance* RGameInstance = RGAMEINSTANCE(this);
	if ( IsValid(RGameInstance->RInventory))
	{
		FCharacterTableInfo* TableInfo = UTableManager::GetInstancePtr()->GetCharacterRow(id);
		if (TableInfo)
		{
			if (IsValid(Image_Hero))
			{
				Image_Hero->SetBrushFromTexture(UUtilFunctionLibrary::GetTexture2DFromTable(TableInfo->PortraitTextureKey));
			}

			if (IsValid(Text_HeroName))
			{
				Text_HeroName->SetText(UUtilFunctionLibrary::GetLocalizedString(EStringTableType::VE_Hero, TableInfo->DisplayName));
			}
		}

		FHERO* outData = RGameInstance->RInventory->GetHeroWithID(id);
		if (outData != nullptr)
		{
			if (IsValid(Text_Level))
			{
				Text_Level->SetText(FText::AsNumber(outData->level));
			}
		}
	}
}

void UUC_SkillHeroIcon::SetHero(FHeroSlotInfo info)
{
	URGameInstance* RGameInstance = RGAMEINSTANCE(this);
	
	{
		FCharacterTableInfo* TableInfo = UTableManager::GetInstancePtr()->GetCharacterRow(info.HeroIDName);
		if (TableInfo)
		{
			if (IsValid(Image_Hero))
			{
				Image_Hero->SetBrushFromTexture(UUtilFunctionLibrary::GetTexture2DFromTable(TableInfo->PortraitTextureKey));
			}

			if (IsValid(Text_HeroName))
			{
				Text_HeroName->SetText(UUtilFunctionLibrary::GetLocalizedString(EStringTableType::VE_Hero, TableInfo->DisplayName));
			}
		}

		if (IsValid(Text_Level))
		{
			Text_Level->SetText(FText::AsNumber(info.HeroLevel));
		}
	}
}

