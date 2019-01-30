// Fill out your copyright notice in the Description page of Project Settings.

#include "ProjectR.h"
#include "UP_HeroRuneResult.h"
#include "GlobalIntegrated.h"


#include "UI/RWidgetManager.h"
#include "UtilFunctionIntegrated.h"
#include "UI/HeroRune/UC_HeroRune.h"

void UUP_HeroRuneResult::NativeConstruct()
{
	Super::NativeConstruct();
	
	URGameInstance* RGameInstance = RGAMEINSTANCE(this);
	
	{	
		AcquiredHeroID = RGameInstance->HeroRuneDungeonManager->GetLatestAcquiredHeroID();
		InvalidateData();
	}
}

void UUP_HeroRuneResult::NativeDestruct()
{
	URGameInstance* RGameInstance = RGAMEINSTANCE(this);
	
	{		
	}

	Super::NativeDestruct();	
}

void UUP_HeroRuneResult::InvalidateData()
{
	URGameInstance* RGameInstance = RGAMEINSTANCE(this);
	
	{
		FCharacterTableInfo* CharacterTableInfo = UTableManager::GetInstancePtr()->GetCharacterRow(AcquiredHeroID);
		if (CharacterTableInfo)
		{
			if (RuneHeroName)
				RuneHeroName->SetText(UUtilFunctionLibrary::GetLocalizedString(EStringTableType::VE_Hero, CharacterTableInfo->DisplayName));
			
			if (RuneHeroAcquireCount)
				RuneHeroAcquireCount->SetText(FText::AsNumber(RGameInstance->HeroRuneDungeonManager->GetLatestAcquiredRuneCount()));

			if (HeroRune)
				HeroRune->SetHeroID(AcquiredHeroID);
		}
	}
}

void UUP_HeroRuneResult::OnClick_Close()
{
	URGameInstance* RGameInstance = RGAMEINSTANCE(this);
	
	{
		//RGameInstance->RWidgetManager->ShowNewHeroUI();		
	}
}