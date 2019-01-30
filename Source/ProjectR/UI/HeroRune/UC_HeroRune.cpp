// Fill out your copyright notice in the Description page of Project Settings.

#include "ProjectR.h"
#include "UC_HeroRune.h"
#include "GlobalIntegrated.h"

#include "UtilFunctionIntegrated.h"

#include "Table/CampaignTableInfo.h"
#include "Table/CharacterTableInfo.h"


void UUC_HeroRune::NativeConstruct()
{
	Super::NativeConstruct();
	
	/*URGameInstance* RGameInstance = RGAMEINSTANCE(this);
	
	{
		
	}*/
}

void UUC_HeroRune::NativeDestruct()
{
	/*URGameInstance* RGameInstance = RGAMEINSTANCE(this);
	
	{
		
	}*/

	Super::NativeDestruct();	
}

void UUC_HeroRune::InvalidateData()
{	
	FCharacterTableInfo* CharacterTableInfo = UTableManager::GetInstancePtr()->GetCharacterRow(HeroID);
	if (CharacterTableInfo)
	{
		if (HeroIconImage)
		{
			FName RunePTKey = *(CharacterTableInfo->PortraitTextureKey.ToString());
			UUIFunctionLibrary::SetImageBrush(UUIFunctionLibrary::GetIconImageFromTable(RunePTKey), HeroIconImage);
			//HeroIconImage->SetBrushFromTexture(UUtilFunctionLibrary::GetTexture2DFromTable(CharacterTableInfo->PortraitTextureKey));
		}
	}
}

void UUC_HeroRune::SetHeroID(FName InHeroID)
{
	HeroID = InHeroID;
	
	InvalidateData();
}