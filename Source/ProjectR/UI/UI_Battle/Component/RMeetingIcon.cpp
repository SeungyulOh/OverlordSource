// Fill out your copyright notice in the Description page of Project Settings.

#include "ProjectR.h"
#include "ProjectRGameMode.h"

#include "ContentGameMode/Controllers/BasePlayerController.h"
#include "UtilFunctionIntegrated.h"

#include "Gameinfo/PlayStateInfo.h"

#include "RMeetingIcon.h"

void URMeetingIcon::NativeConstruct()
{
	Super::NativeConstruct();
	
	if (Button_On)
	{
		Button_On->OnClicked.RemoveDynamic(this, &URMeetingIcon::BtnEventOnClicked);
		Button_On->OnClicked.AddDynamic(this, &URMeetingIcon::BtnEventOnClicked);
	}

	if (Image_CoolGauge)
	{
		UMaterialInterface* MI = Cast<UMaterialInterface>(Image_CoolGauge->Brush.GetResourceObject());
		if (MI)
		{
			MID = UMaterialInstanceDynamic::Create(MI, this);
			Image_CoolGauge->SetBrushFromMaterial(MID);
		}
			
	}
	
}

void URMeetingIcon::NativeDestruct()
{
	Super::NativeDestruct();
}



void URMeetingIcon::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);

	if (RemainingTimeToReuse > 0)
	{
		RemainingTimeToReuse -= InDeltaTime;

		if (Text_CoolTime)
			Text_CoolTime->SetText(FText::AsNumber((int32)RemainingTimeToReuse));

		if (MID)
		{
			MID->SetScalarParameterValue(FName(TEXT("Percent")), RemainingTimeToReuse * 0.1f);
		}
	}
	else
	{
		if (Cool->GetVisibility() == ESlateVisibility::Collapsed)
			return;

		RemainingTimeToReuse = 0;
		if (Cool)
			Cool->SetVisibility(ESlateVisibility::Collapsed);
	}
}

void URMeetingIcon::BtnEventOnClicked()
{
	if (RemainingTimeToReuse > 0)
		return;

	RemainingTimeToReuse = CoolTime;
	if (Cool)
		Cool->SetVisibility(ESlateVisibility::Visible);

	UPlayStateInfo* playinfo = RGAMEINSTANCE(GEngine)->PlayStateInfo;
	if (playinfo)
	{
		TArray<FHeroSlotInfo> heroinfoArray = playinfo->GetSelectedCharacterArray();
		for (size_t i = 0; i < heroinfoArray.Num(); ++i)
		{
			if (heroinfoArray[i].CharacterRole == ECharacterRoleEnum::VE_Crew1 ||
				heroinfoArray[i].CharacterRole == ECharacterRoleEnum::VE_Crew2)
			{
				if (heroinfoArray[i].CharacterCrewState == ECharacterCrewStateEnum::VE_Battle)
				{
					AActor* Crew = heroinfoArray[i].SpawnedTargetActor;
					
					if (!Crew)
						continue;
					
					UEntityAIComponent* CrewAIComp = UCharacterHelpFunctionLibrary::GetEntityAIComponent(Crew);
					if (!CrewAIComp)
						continue;

					CrewAIComp->SetBlackboardBoolValue(ERAIBalckboardValueEnum::VE_CrewTacticsMove, true);
				}
			}
		}
	}
}

