// Fill out your copyright notice in the Description page of Project Settings.

#include "ProjectR.h"

#include "HeroUIforPvp.h"

#include "GlobalIntegrated.h"
#include "UtilFunctionIntegrated.h"


void URHeroUIforPvp::NativeConstruct()
{
	Super::NativeConstruct();

	if (Leader_HP)
	{
		MID = Cast<UMaterialInstanceDynamic>(Leader_HP->Brush.GetResourceObject());
		if (!MID)
		{
			UMaterialInterface* MI = Cast<UMaterialInterface>(Leader_HP->Brush.GetResourceObject());
			if (MI)
			{
				MID = UMaterialInstanceDynamic::Create(MI, this);
				Leader_HP->SetBrushFromMaterial(MID);
			}
		}

		MID->SetVectorParameterValue(FName(TEXT("Color")), HPBlueColor);
		if (HPTexture)
			MID->SetTextureParameterValue(FName(TEXT("Tex")), HPTexture);
	}

	if (Noti_Danger_2)
		Noti_Danger_2->SetVisibility(ESlateVisibility::Collapsed);

}

void URHeroUIforPvp::NativeDestruct()
{
	Super::NativeDestruct();
}

void URHeroUIforPvp::SetHeroSlot(FHeroSlotInfo& InHeroSlotInfo)
{
	URGameInstance* RGameInstance = RGAMEINSTANCE(this);

	HeroSlotInfo = InHeroSlotInfo;

	FHERO* pHero = RGameInstance->RInventory->GetHeroWithUD(HeroSlotInfo.HeroUD);

	if (pHero == nullptr)
		return;

	UObject* Portrait = UUIFunctionLibrary::GetHeroRhombusPortrait(pHero->heroId);
	UUIFunctionLibrary::SetImageBrush(Portrait, Image_Hero);

	//Lv
	if (IsValid(Level_Text))
	{
		int32 HeroLv = HeroSlotInfo.HeroLevel;
		FText text = FText::FromString(FString::FromInt(HeroLv));
		Level_Text->SetText(text);
	}
}

void URHeroUIforPvp::SetHero(AActor* InOwnerActor, FName InHeroID, int32 InLevel)
{
	SetOwnerActor(InOwnerActor);

	UObject* Portrait = UUIFunctionLibrary::GetHeroRhombusPortrait(InHeroID);
	UUIFunctionLibrary::SetImageBrush(Portrait, Image_Hero);

	//Lv
	if (IsValid(Level_Text))
	{
		FText text = FText::FromString(FString::FromInt(InLevel));
		Level_Text->SetText(text);
	}
}

void URHeroUIforPvp::UpdateIcon()
{
	//HpBar
	if (IsValid(OwnerActor))
	{
		UBattleEntityComponent* BattleComp = UCharacterHelpFunctionLibrary::GetBattleEntityComponent(OwnerActor);

		float HPPercent = BattleComp->GetCurrentHPPercent();

		if (MID)
		{
			if (!bHurt && HPPercent <= DangerValue)
			{
				MID->SetVectorParameterValue(FName(TEXT("Color")), HPRedColor);
				PlayAni(TEXT("Danger_Noti"), false, 0);
				bHurt = true;
			}
			else if (DangerValue < HPPercent)
			{
				bHurt = false;
				MID->SetVectorParameterValue(FName(TEXT("Color")), HPBlueColor);

				if(IsPlayAni(TEXT("Danger_Noti")))
					StopAni(TEXT("Danger_Noti"));
			}

			MID->SetScalarParameterValue(FName(TEXT("Percent")), HPPercent);
		}

		HpPercentText->SetText(FText::AsPercent(HPPercent));

		if (HPPercent <= 0) // when dead
		{
			Image_Hero->SetVisibility(ESlateVisibility::Collapsed);
			Level_Text->SetVisibility(ESlateVisibility::Collapsed);

			HeroDeathOverlay->SetVisibility(ESlateVisibility::HitTestInvisible);
		}
		else
		{
			Image_Hero->SetVisibility(ESlateVisibility::HitTestInvisible);
			Level_Text->SetVisibility(ESlateVisibility::HitTestInvisible);

			HeroDeathOverlay->SetVisibility(ESlateVisibility::Collapsed);
		}
	}
}

void URHeroUIforPvp::SetEnemyHPColor()
{
	if(MID)
	{
		MID->SetVectorParameterValue(FName(TEXT("Color")), HPRedColor);
	}
}