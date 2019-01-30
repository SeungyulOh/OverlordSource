// Fill out your copyright notice in the Description page of Project Settings.

#include "ProjectR.h"

#include "HeroUIforCrew.h"

#include "GlobalIntegrated.h"
#include "UtilFunctionIntegrated.h"


void URHeroUIforCrew::NativeConstruct()
{
	Super::NativeConstruct();

// 	if(SwapAni != nullptr)
// 		SwapAni->OnAnimationFinished.AddDynamic(this, &URHeroUIforCrew::OnSwapAniFinished);

	if (Image_HP_Progress)
	{
		MID = Cast<UMaterialInstanceDynamic>(Image_HP_Progress->Brush.GetResourceObject());
		if (!MID)
		{
			UMaterialInterface* MI = Cast<UMaterialInterface>(Image_HP_Progress->Brush.GetResourceObject());
			if (MI)
			{
				MID = UMaterialInstanceDynamic::Create(MI, this);
				Image_HP_Progress->SetBrushFromMaterial(MID);
			}
		}
		MID->SetVectorParameterValue(FName(TEXT("Color")), HPBlueColor);
		if (HPTexture)
			MID->SetTextureParameterValue(FName(TEXT("Tex")), HPTexture);

		bHurt = false;
		StopAni(TEXT("Danger_Noti"));
		MID->SetScalarParameterValue(FName(TEXT("Percent")), 1.f);
		HpPercentText->SetText(FText::AsPercent(1.f));
		Image_Hero->SetVisibility(ESlateVisibility::HitTestInvisible);
		Level_Text->SetVisibility(ESlateVisibility::HitTestInvisible);
		HeroDeathOverlay->SetVisibility(ESlateVisibility::Collapsed);
	}

	
}

void URHeroUIforCrew::NativeDestruct()
{
// 	if (SwapAni)
// 		SwapAni->OnAnimationFinished.RemoveDynamic(this, &URHeroUIforCrew::OnSwapAniFinished);

	Super::NativeDestruct();
}

void URHeroUIforCrew::SetHeroSlot(FHeroSlotInfo& InHeroSlotInfo)
{
	URGameInstance* RGameInstance = RGAMEINSTANCE(this);

	HeroSlotInfo = InHeroSlotInfo;
	if (HeroSlotInfo.CharacterCrewState == ECharacterCrewStateEnum::VE_Battle)
		bBattleCrew = true;
	else
		bBattleCrew = false;

	UObject* Portrait = UUIFunctionLibrary::GetHeroRhombusPortrait(HeroSlotInfo.HeroIDName);
	UUIFunctionLibrary::SetImageBrush(Portrait, Image_Hero);

	//Lv
	if (IsValid(Level_Text))
	{
		int32 HeroLv = HeroSlotInfo.HeroLevel;
		FText text = FText::FromString(FString::FromInt(HeroLv));
		Level_Text->SetText(text);
	}

// 	if (IsValid(BG))
// 	{
// 		FCharacterTableInfo* CharacterTableInfo = URenewal_BattleFunctionLibrary::GetCharacterTableInfo(ECharacterCategoryTypeEnum::VE_Hero, pHero->heroId);
// 
// 		checkf(RGameInstance->RWidgetManager, TEXT("RWidgetManager null"));
// 		TArray<UPaperSprite*> BGArray = RGameInstance->RWidgetManager->PortraitBGArray;
// 
// 		FLinearColor color;
// 		if (CharacterTableInfo->Grade >= 1 && CharacterTableInfo->Grade < 4)
// 		{
// 			color = FLinearColor::Black;
// 			BG->Brush.SetResourceObject(BGArray[0]);
// 		}
// 		else if (CharacterTableInfo->Grade >= 4 && CharacterTableInfo->Grade < 6)
// 		{
// 			color = FColor(67, 186, 35);
// 			BG->Brush.SetResourceObject(BGArray[1]);
// 		}
// 		else if (CharacterTableInfo->Grade >= 6 && CharacterTableInfo->Grade < 8)
// 		{
// 			color = FColor(38, 174, 238);
// 			BG->Brush.SetResourceObject(BGArray[2]);
// 		}
// 		else if (CharacterTableInfo->Grade >= 8 && CharacterTableInfo->Grade < 10)
// 		{
// 			color = FColor(216, 158, 67);
// 			BG->Brush.SetResourceObject(BGArray[3]);
// 		}
// 		else
// 			BG->Brush.SetResourceObject(BGArray[4]);
// 
// 		if (ImageBack /*&& CharacterTableInfo->Grade < 10*/)
// 			ImageBack->Brush.TintColor = FSlateColor(color);
// 	}

}

void URHeroUIforCrew::UpdateIcon()
{
	//HpBar

	if (HeroSlotInfo.CharacterCrewState == ECharacterCrewStateEnum::VE_Battle)
	{
		if (HeroSlotInfo.SlotSpawnState != EHeroSpawnStateEnum::VE_Dead)
		{
			if (!IsValid(HeroSlotInfo.SpawnedTargetActor))
				return;

			if (HeroSlotInfo.SpawnedTargetActor->IsPendingKill())
				return;

			UBattleEntityComponent* BattleComp = UCharacterHelpFunctionLibrary::GetBattleEntityComponent(HeroSlotInfo.SpawnedTargetActor);
			if (BattleComp)
			{
				if (HeroSlotInfo.SpawnedTargetActor->GetName().Contains(TEXT("Rayna")))
				{
					//UE_LOG(LogClass, Log, TEXT("Check"));
				}
				float HPPercent = BattleComp->GetCurrentHPPercent();

				UpdateHp(HPPercent);

				if (HPPercent > 0)
				{
					Image_Hero->SetVisibility(ESlateVisibility::HitTestInvisible);
					Level_Text->SetVisibility(ESlateVisibility::HitTestInvisible);
					HeroDeathOverlay->SetVisibility(ESlateVisibility::Collapsed);
				}
				else
				{
					Image_Hero->SetVisibility(ESlateVisibility::Collapsed);
					Level_Text->SetVisibility(ESlateVisibility::Collapsed);
					HeroDeathOverlay->SetVisibility(ESlateVisibility::HitTestInvisible);
				}
				
			}
		}
		else
		{
			UpdateHp(0.f);
			Image_Hero->SetVisibility(ESlateVisibility::Collapsed);
			Level_Text->SetVisibility(ESlateVisibility::Collapsed);
			HeroDeathOverlay->SetVisibility(ESlateVisibility::HitTestInvisible);
		}
	}
	else
	{
		if (HeroSlotInfo.SlotSpawnState == EHeroSpawnStateEnum::VE_Dead)
		{
			UpdateHp(0.f);
			Image_Hero->SetVisibility(ESlateVisibility::Collapsed);
			Level_Text->SetVisibility(ESlateVisibility::Collapsed);
			HeroDeathOverlay->SetVisibility(ESlateVisibility::HitTestInvisible);
		}
	}

}

void URHeroUIforCrew::UpdateHp(float InHp)
{
	if (MID)
	{
		if (!bHurt && InHp <= DangerValue)
		{
			MID->SetVectorParameterValue(FName(TEXT("Color")), HPRedColor);
			PlayAni(TEXT("Danger_Noti"), false, 0);
			bHurt = true;
		}
		else if (DangerValue < InHp)
		{
			bHurt = false;
			MID->SetVectorParameterValue(FName(TEXT("Color")), HPBlueColor);
			StopAni(TEXT("Danger_Noti"));
		}

		MID->SetScalarParameterValue(FName(TEXT("Percent")), InHp);
	}

	HpPercentText->SetText(FText::AsPercent(InHp));

	if (InHp <= 0)
	{
		Image_Hero->SetVisibility(ESlateVisibility::Collapsed);
		Level_Text->SetVisibility(ESlateVisibility::Collapsed);

		HeroDeathOverlay->SetVisibility(ESlateVisibility::HitTestInvisible);
	}
}

void URHeroUIforCrew::SetRTSHeroSlot(FHeroSlotInfo& InHeroSlotInfo)
{
	SetHeroSlot(InHeroSlotInfo);
}

void URHeroUIforCrew::RTSUpdateIcon()
{
	UpdateIcon();
}

