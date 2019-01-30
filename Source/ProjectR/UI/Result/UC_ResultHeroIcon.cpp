// Fill out your copyright notice in the Description page of Project Settings.

#include "ProjectR.h"
#include "UC_ResultHeroIcon.h"

#include "SharedConstants/GlobalConstants.h"

#include "GlobalIntegrated.h"
#include "UtilFunctionIntegrated.h"


void UUC_ResultHeroIcon::NativeConstruct()
{
	SlotIndex = dEMPTYSLOT;
	Super::NativeConstruct();
}

void UUC_ResultHeroIcon::NativeDestruct()
{
	Super::NativeDestruct();
}

void UUC_ResultHeroIcon::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);

	URGameInstance* RGameInstance = RGAMEINSTANCE(this);

	TArray<FHeroSlotInfo>& HeroSlot = RGameInstance->PlayStateInfo->GetSelectedCharacterArray();

	if (HeroSlot.IsValidIndex(SlotIndex) == false || HeroSlot[SlotIndex].UserKID == KID_NONE)
		return;

	if (Hero == nullptr)
		return;

	if (HeroSlot[SlotIndex].HeroLevel <= Hero->level)
	{
		if ((HeroSlot[SlotIndex].HeroLevel == Hero->level) && (HeroExpPercent >= HeroExpMaxPercent))
			return;

		HeroExpPercent += 0.01f;

		if (HeroExpPercent >= 1.0f)
		{
			HeroExpPercent = 0.0f;
			HeroSlot[SlotIndex].HeroLevel++;
			SetIcon(SlotIndex);
		}
	}

	if (IsValid(HeroIcon))
		HeroIcon->SetResultProgressBar(HeroExpPercent);
}

void UUC_ResultHeroIcon::SetIcon(int32 InIndex)
{
	URGameInstance* RGameInstance = RGAMEINSTANCE(this);

	TArray<FHeroSlotInfo>& HeroSlot		=	RGameInstance->PlayStateInfo->GetSelectedCharacterArray();

	if (HeroSlot.IsValidIndex(InIndex) == false || HeroSlot[InIndex].UserKID == KID_NONE)
	{
		this->SetVisibility(ESlateVisibility::Collapsed);
		return;
	}
	else
	{
		this->SetVisibility(ESlateVisibility::Visible);
	}

	if (SlotIndex == dEMPTYSLOT)
		HeroSlot[InIndex] = RGameInstance->PlayStateInfo->GetSelectedCharacterSlot(InIndex);

	SlotIndex = InIndex;
	Hero				=	RGameInstance->RInventory->GetHeroWithUD(HeroSlot[SlotIndex].HeroUD);
	if (Hero == nullptr)
		return;
	CostGrowthInfo		=	UTableManager::GetInstancePtr()->GetCharacterCostGrowthRow(FName(*FText::AsNumber(HeroSlot[SlotIndex].HeroLevel).ToString()));
	HeroExpPercent		=	static_cast<float>(HeroSlot[InIndex].HeroExp) / static_cast<float>(CostGrowthInfo->Cost_HeroRune);
	if (IsValid(HeroIcon))
		HeroIcon->SetHeroIconInfoWithResult(HeroSlot[SlotIndex]);
	if (IsValid(Text_Level))
		Text_Level->SetText(FText::AsNumber(HeroSlot[SlotIndex].HeroLevel));
	CostGrowthInfo		=	UTableManager::GetInstancePtr()->GetCharacterCostGrowthRow(FName(*FText::AsNumber(Hero->level).ToString()));
	HeroExpMaxPercent	=	static_cast<float>(Hero->exp) / static_cast<float>(CostGrowthInfo->Cost_HeroRune);
}
