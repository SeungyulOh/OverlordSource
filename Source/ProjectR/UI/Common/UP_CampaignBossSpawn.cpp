// Fill out your copyright notice in the Description page of Project Settings.

#include "ProjectR.h"

#include "GlobalIntegrated.h"


#include "UtilFunctionIntegrated.h"
#include "Character/Interfaces/EntityBaseProperty.h"
#include "Table/CharacterTableInfo.h"
#include "Table/StringTableInfo.h"


#include "UP_CampaignBossSpawn.h"


void UUP_CampaignBossSpawn::NativeConstruct()
{
	Super::NativeConstruct();

	CampaignBossNameAnim = GetAni(TEXT("CampaignBossNameAni"));
	if (IsValid(CampaignBossNameAnim))
	{
		CampaignBossNameAnim->OnAnimationFinished.RemoveDynamic(this, &UUP_CampaignBossSpawn::AfterAnimFinished);
		CampaignBossNameAnim->OnAnimationFinished.AddDynamic(this, &UUP_CampaignBossSpawn::AfterAnimFinished);
	}

	RGameInstance = RGAMEINSTANCE(this);

	if (CampaignBossName)
	{
		CampaignBossName->SetText( FText::GetEmpty());
	}
	if (CampaignBossName_en)
	{
		CampaignBossName_en->SetVisibility(ESlateVisibility::Collapsed);
	}
}

void UUP_CampaignBossSpawn::NativeDestruct()
{
	Super::NativeDestruct();
}

void UUP_CampaignBossSpawn::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);

	if (this->IsInViewport())
	{
		URGameInstance* GameInstance = RGAMEINSTANCE(this);
		if (GameInstance->SequenceManager->IsPlaying() == false)
		{
			SetVisibility(ESlateVisibility::Collapsed);
		}
	}
}

void UUP_CampaignBossSpawn::SetBossActor(AActor* InBossActor)
{
	if (false == IsValid(CampaignBossName))
		return;
	URGameInstance* GameInstance = RGAMEINSTANCE(this);
	FText BossNameText = UUtilFunctionLibrary::GetBossName(InBossActor);

	CampaignBossName->SetText(BossNameText);

	//todo : del
	BP_ShowBossName(InBossActor);
}

void UUP_CampaignBossSpawn::BP_ShowBossName(AActor* BossActor)
{
	if (IsValid(BossActor))
	{
		SetVisibility(ESlateVisibility::HitTestInvisible);
		PlayAnimation(CampaignBossNameAnim);
	}
}

void UUP_CampaignBossSpawn::AfterAnimFinished()
{
	SetVisibility(ESlateVisibility::Collapsed);
}
