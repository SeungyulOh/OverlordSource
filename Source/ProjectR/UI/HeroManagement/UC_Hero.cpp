// Fill out your copyright notice in the Description page of Project Settings.

#include "ProjectR.h"
#include "UC_Hero.h"
#include "UI/Common/UC_Item_Icons.h"
#include "UI/Common/Renewal_BaseHeroIcon.h"
#include "UI/RWidgetManager.h"
#include "UtilFunctionIntegrated.h"
#include "UtilFunctionIntegrated.h"
#include "GlobalIntegrated.h"


#include "GameInfo/RInventory.h"


void UUC_Hero::NativeConstruct()
{
	Super::NativeConstruct();
}

void UUC_Hero::NativeDestruct()
{
	Super::NativeDestruct();
}

void UUC_Hero::Init()
{
	//SelectedSpiritUDs.Empty();
	UC_HeroIcons.Empty();
	FindChildWidgetsWithCertainName<URenewal_BaseHeroIcon>(this, UC_HeroIcons, TEXT("UC_Icon_Hero_"));
	for (URenewal_BaseHeroIcon* HeroIcon : UC_HeroIcons)
	{
		HeroIcon->SetVisibility(ESlateVisibility::Collapsed);
	}
}

void UUC_Hero::UpdateSelectedHeroUDs(const TArray<FString>& InSpiritUDs)
{
	//SelectedSpiritUDs = InSpiritUDs;
	//SelectedSpiritUDs.Empty();
	//for (FString& SpiritUD : InSpiritUDs)
	//	SelectedSpiritUDs.Emplace(SpiritUD);
	UC_HeroIcons.Reset();
	FindChildWidgetsWithCertainName<URenewal_BaseHeroIcon>(this, UC_HeroIcons, TEXT("UC_Icon_Hero_"));
	for (URenewal_BaseHeroIcon* HeroIcon : UC_HeroIcons)
	{
		HeroIcon->SetVisibility(ESlateVisibility::Collapsed);
	}

	int32 Index = 0;
	URGameInstance* RGameInstance = RGAMEINSTANCE(this);
	checkf(RGameInstance != nullptr, TEXT("UUC_HeroMaterial_Popup[RGameInstance Is Empty]"));
	checkf(RGameInstance->RInventory != nullptr, TEXT("UUC_HeroMaterial_Popup[RInventory Is Empty]"));
	for (const FString& SpiritUD : InSpiritUDs)
	{
		if (UC_HeroIcons.IsValidIndex(Index) && IsValid(UC_HeroIcons[Index]))
		{
			FHERO* outHero = RGameInstance->RInventory->GetHeroWithUD(SpiritUD);
			if (outHero != nullptr)
			{
// 				FHERO_ICON_INFO HeroIconInfo;
// 				UUIFunctionLibrary::GetHeroIconInfoWithID(HeroIconInfo, outHero->heroId, EHeroOwnershipType::VE_None);

				UC_HeroIcons[Index]->SetHeroIconInfoWithID(outHero->heroId, EHeroOwnershipType::VE_None);
				UC_HeroIcons[Index]->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
				Index++;
			}
		}
	}
}

