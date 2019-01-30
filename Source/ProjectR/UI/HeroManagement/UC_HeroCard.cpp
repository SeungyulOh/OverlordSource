// Fill out your copyright notice in the Description page of Project Settings.

#include "ProjectR.h"
#include "UC_HeroCard.h"

#include "Network/PacketDataStructures.h"
#include "GlobalIntegrated.h"

#include "Table/CharacterTableInfo.h"
#include "UtilFunctionIntegrated.h"
#include "UC_HeroGrade.h"
#include "GameInfo/RInventory.h"


void UUC_HeroCard::InvalidateData()
{
	Super::InvalidateData();

	FHERO* HeroData = RGAMEINSTANCE(this)->RInventory->GetHeroWithUD(HeroUD);
	if (HeroData == nullptr)
	{
		UE_LOG(LogUI, Log, TEXT("UUC_HeroCard::InvalidateData: failed to look up HeroData"));
		return;
	}
	FCharacterTableInfo* HeroInfo = UTableManager::GetInstancePtr()->GetCharacterRow(HeroData->heroId);
	if (HeroInfo == nullptr)
	{
		UE_LOG(LogUI, Log, TEXT("UUC_HeroCard::InvalidateData: failed to look up HeroInfo"));
		return;
	}

	// card image
	{
		FString text = TEXT("Campaign_BG_Hero_") + HeroInfo->DisplayName.ToString();
		Image_HeroCard->SetBrushFromTexture(UUtilFunctionLibrary::GetTexture2DFromTable(FName(*text)));
	}

	// hero level
	// "LV. 20"
	{
		FString text = FString::Printf(TEXT("LV. %d"), HeroData->level);
		Text_HeroLevel->SetText(FText::FromString(text));
	}

	// grade
	HeroGrade->Refresh(HeroInfo->Grade);

	// name
	Text_HeroName->SetText(UUtilFunctionLibrary::GetLocalizedString(EStringTableType::VE_Hero, HeroInfo->DisplayName));
}
