// Fill out your copyright notice in the Description page of Project Settings.

#include "ProjectR.h"

#include "HeroUIforLeader.h"

#include "GlobalIntegrated.h"
#include "UtilFunctionIntegrated.h"

#include "GameInfo/RInventory.h"

#include "Network/RTSManager.h"

#include "UI/UI_Battle/RCampaignHUD_ThreeControl.h"

#include "UI/HeroManagement/UC_Hero_Skill_Icon.h"

#include "Character/Components/FunctionalComponents/BattleEntityComponent.h"

void URHeroUIforLeader::NativeConstruct()
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
		if(HPTexture)
			MID->SetTextureParameterValue(FName(TEXT("Tex")), HPTexture);
		
	}

	StopAni(TEXT("Danger_Noti"));
		
}

void URHeroUIforLeader::NativeDestruct()
{
	Super::NativeDestruct();
}

void URHeroUIforLeader::SetHeroSlot(FHeroSlotInfo& InHeroSlotInfo)
{
	URGameInstance* RGameInstance = RGAMEINSTANCE(this);

	HeroSlotInfo = InHeroSlotInfo;

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

void URHeroUIforLeader::UpdateIcon()
{
	FHERO* pHero = nullptr;
	URGameInstance* RGameInstance = RGAMEINSTANCE(this);
	pHero = RGameInstance->RInventory->GetHeroWithUD(HeroSlotInfo.HeroUD);
	//portrait
	if (pHero == nullptr)
		return;

	//HpBar
	if (IsValid(HeroSlotInfo.SpawnedTargetActor))
	{
		UBattleEntityComponent* BattleComp = UCharacterHelpFunctionLibrary::GetBattleEntityComponent(HeroSlotInfo.SpawnedTargetActor);

		float HPPercent = BattleComp->GetCurrentHPPercent();

		if (MID)
		{
			if (!bHurt && HPPercent <= DangerValue)
			{
				bHurt = true;
				MID->SetVectorParameterValue(FName(TEXT("Color")), HPRedColor);
				PlayAni(TEXT("Danger_Noti"), false, 0);
			}
			else if(DangerValue < HPPercent)
			{
				bHurt = false;
				MID->SetVectorParameterValue(FName(TEXT("Color")), HPBlueColor);
				StopAni(TEXT("Danger_Noti"));
			}

			MID->SetScalarParameterValue(FName(TEXT("Percent")), HPPercent);
		}

		URBaseIngameHUD* ThreeGameHUD = Cast<URBaseIngameHUD>(ParentWidget);
		if (IsValid(ThreeGameHUD))
		{
			if (HPPercent <= DangerValue)
			{
				if (!ThreeGameHUD->IsPlayAni(TEXT("Screen_Danger_Noti")))
					ThreeGameHUD->PlayAni(TEXT("Screen_Danger_Noti"), false, 0);
			}
			else
			{
				ThreeGameHUD->StopAni(TEXT("Screen_Danger_Noti"));
			}
		}


		HpPercentText->SetText(FText::AsPercent(HPPercent));

		if (HPPercent <= 0) // when dead
		{
			Image_Hero->SetVisibility(ESlateVisibility::Collapsed);
// 			HpPercentText->SetVisibility(ESlateVisibility::Collapsed);
//			HpProgerssBar->SetVisibility(ESlateVisibility::Collapsed);
			Level_Text->SetVisibility(ESlateVisibility::Collapsed);

			HeroDeathOverlay->SetVisibility(ESlateVisibility::HitTestInvisible);
		}
		else
		{
			Image_Hero->SetVisibility(ESlateVisibility::HitTestInvisible);
// 			HpPercentText->SetVisibility(ESlateVisibility::HitTestInvisible);
// 			HpProgerssBar->SetVisibility(ESlateVisibility::HitTestInvisible);
			Level_Text->SetVisibility(ESlateVisibility::HitTestInvisible);

			HeroDeathOverlay->SetVisibility(ESlateVisibility::Collapsed);
		}
	}
}

void URHeroUIforLeader::RTSUpdateIcon()
{
	FHERO* pHero = nullptr;
	URGameInstance* RGameInstance = RGAMEINSTANCE(this);
	FRTS_PLAYER* RTSPlayer = RGameInstance->RealTimeModManager->Get_RTS_Player(HeroSlotInfo.UserKID);
	
	//portrait
	if (RTSPlayer == nullptr)
		return;

	//HpBar
	if (IsValid(HeroSlotInfo.SpawnedTargetActor))
	{
		UBattleEntityComponent* BattleComp = UCharacterHelpFunctionLibrary::GetBattleEntityComponent(HeroSlotInfo.SpawnedTargetActor);

		if (IsValid(UserNameText))
			UserNameText->SetText(FText::FromString(RTSPlayer->nick));

		float HPPercent = BattleComp->GetCurrentHPPercent();

		if (MID)
		{
			if (!bHurt && HPPercent <= DangerValue)
			{
				bHurt = true;
				MID->SetVectorParameterValue(FName(TEXT("Color")), HPRedColor);
				PlayAni(TEXT("Danger_Noti"), false, 0);
			}
			else if (DangerValue < HPPercent)
			{
				bHurt = false;
				MID->SetVectorParameterValue(FName(TEXT("Color")), HPBlueColor);
				StopAni(TEXT("Danger_Noti"));
			}

			MID->SetScalarParameterValue(FName(TEXT("Percent")), HPPercent);
		}

		URBaseIngameHUD* OneCampaignHUD = Cast<URBaseIngameHUD>(ParentWidget);
		if (IsValid(OneCampaignHUD))
		{
			if (HPPercent <= DangerValue)
			{
				if (!OneCampaignHUD->IsPlayAni(TEXT("Screen_Danger_Noti")))
					OneCampaignHUD->PlayAni(TEXT("Screen_Danger_Noti"), false, 0);
			}
		}

		HpPercentText->SetText(FText::AsPercent(HPPercent));

		if (HPPercent <= 0) // when dead
		{
			Image_Hero->SetVisibility(ESlateVisibility::Collapsed);
			// 			HpPercentText->SetVisibility(ESlateVisibility::Collapsed);
			//			HpProgerssBar->SetVisibility(ESlateVisibility::Collapsed);
			Level_Text->SetVisibility(ESlateVisibility::Collapsed);

			HeroDeathOverlay->SetVisibility(ESlateVisibility::HitTestInvisible);
		}
		else
		{
			Image_Hero->SetVisibility(ESlateVisibility::HitTestInvisible);
			// 			HpPercentText->SetVisibility(ESlateVisibility::HitTestInvisible);
			// 			HpProgerssBar->SetVisibility(ESlateVisibility::HitTestInvisible);
			Level_Text->SetVisibility(ESlateVisibility::HitTestInvisible);

			HeroDeathOverlay->SetVisibility(ESlateVisibility::Collapsed);
		}
	}
}

void URHeroUIforLeader::SetRTSHeroSlot(FHeroSlotInfo& InHeroSlotInfo)
{
	SetHeroSlot(InHeroSlotInfo);
}

void URHeroUIforLeader::SetUIForCampaign(bool InbPlayAlone)
{
	bPlayAlone = InbPlayAlone;

	if(bPlayAlone)
	{
		ActiveSpiritSkill01->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
		ActiveSpiritSkill02->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
	}

	UserNameText->SetVisibility(ESlateVisibility::Collapsed);

}

void URHeroUIforLeader::SetUIForSingleRaid()
{
	bPlayAlone = false;
	UserNameText->SetVisibility(ESlateVisibility::Collapsed);
	HideUnUsedSpiritSkillIcon(false, false);
}

void URHeroUIforLeader::SetUIForMultiRaid()
{
	bPlayAlone = true;
	ActiveSpiritSkill01->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
	ActiveSpiritSkill02->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
	UserNameText->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
}

void URHeroUIforLeader::SetActiveSpiritSkillIcon(int32 InIndex, FName InSpiritSkillName)
{
	if (bPlayAlone)
	{
		if (InIndex == 0)
			ActiveSpiritSkill01->SetSpiritSkill(InSpiritSkillName);
		else if (InIndex == 1)
			ActiveSpiritSkill02->SetSpiritSkill(InSpiritSkillName);
	}
}

void URHeroUIforLeader::HideUnUsedSpiritSkillIcon(bool InbUsedSpirit1, bool InbUsedSpirit2)
{
	ActiveSpiritSkill01->SetVisibility(InbUsedSpirit1 ? ESlateVisibility::SelfHitTestInvisible : ESlateVisibility::Collapsed);
	ActiveSpiritSkill02->SetVisibility(InbUsedSpirit2 ? ESlateVisibility::SelfHitTestInvisible : ESlateVisibility::Collapsed);
}
