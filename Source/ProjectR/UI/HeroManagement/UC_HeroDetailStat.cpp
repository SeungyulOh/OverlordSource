// Fill out your copyright notice in the Description page of Project Settings.

#include "ProjectR.h"
#include "UC_HeroDetailStat.h"

#include "UI/Control/RPropertyPanel.h"
#include "CustomStructures/SharedStructures/CharacterBattleInfo.h"
#include "UtilFunctionIntegrated.h"
#include "UC_HeroInfoStat.h"

void UUC_HeroDetailStat::NativeConstruct()
{
	Super::NativeConstruct();

	if (Button_Close)
	{
		Button_Close->OnClicked.RemoveDynamic(this, &UUC_HeroDetailStat::OnClick_Close);
		Button_Close->OnClicked.AddDynamic(this, &UUC_HeroDetailStat::OnClick_Close);
	}
}

void UUC_HeroDetailStat::NativeDestruct()
{
	Super::NativeDestruct();
}

void UUC_HeroDetailStat::SetPropertyData(UCharacterBattleInfo* InCharacterBattleInfo)
{
	if (!IsValid(InCharacterBattleInfo))
	{
		return;
	}

	if (CharacterBattleInfo == InCharacterBattleInfo)
	{
		// It is originally always return 
		++UpdateCount;
		if (10 > UpdateCount)
			return;
	}

	UpdateCount = 0;
	CharacterBattleInfo = InCharacterBattleInfo;

	InvalidateData();
}


void UUC_HeroDetailStat::InvalidateData()
{
	TArray< UWidget* > ChildrenWidget;
	this->WidgetTree->GetAllWidgets(ChildrenWidget);

	for (auto ChildWidget : ChildrenWidget)
	{
		auto InfoStat = Cast<UUC_HeroInfoStat>(ChildWidget);

		if (IsValid(InfoStat))
		{
			InfoStat->Refresh(CharacterBattleInfo);
		}
	}
}

void UUC_HeroDetailStat::OnClick_Close()
{
	this->SetVisibility(ESlateVisibility::Collapsed);

	/*UUP_Mediator_HeroManagement* HeroManagement = Cast<UUP_Mediator_HeroManagement>(ParentWidget);
	if (IsValid(HeroManagement))
	{
		HeroManagement->OnClick_OpenHeroDetailStat();
	}*/
}
