// Fill out your copyright notice in the Description page of Project Settings.

#include "ProjectR.h"

#include "GameInfo/RInventory.h"

#include "GlobalIntegrated.h"
#include "Network/RTSManager.h"

#include "UtilFunctionIntegrated.h"

#include "UI/UI_Battle/RCampaignHUD_ThreeControl.h"

#include "Character/Components/FunctionalComponents/BattleEntityComponent.h"

#include "RHeroIcon_ForInGame.h"

void URHeroIcon_ForInGame::NativeConstruct()
{
	Super::NativeConstruct();
}

bool URHeroIcon_ForInGame::Initialize()
{
	Super::Initialize();
	return true;
}

void URHeroIcon_ForInGame::SetHeroSlot(FHeroSlotInfo& InHeroSlotInfo)
{
	URGameInstance* RGameInstance = RGAMEINSTANCE(this);

	HeroSlotInfo = InHeroSlotInfo;

	FHERO* pHero = RGameInstance->RInventory->GetHeroWithUD(HeroSlotInfo.HeroUD);

	if (pHero == nullptr)
		return;

	UObject* Portrait = UUIFunctionLibrary::GetCharacterPortrait(ECharacterCategoryTypeEnum::VE_Hero, pHero->heroId);
	UUIFunctionLibrary::SetImageBrush(Portrait, Image_Hero);

	//Lv
	if (IsValid(Level_Text))
	{
		int32 HeroLv = HeroSlotInfo.HeroLevel;
		FText text = FText::FromString(FString::FromInt(HeroLv));
		Level_Text->SetText(text);
	}

	if (IsValid(BG))
	{
		FCharacterTableInfo* CharacterTableInfo = URenewal_BattleFunctionLibrary::GetCharacterTableInfo(ECharacterCategoryTypeEnum::VE_Hero, pHero->heroId);
		
		checkf(RGameInstance->RWidgetManager, TEXT("RWidgetManager null"));
		TArray<UPaperSprite*> BGArray = URAsyncLoader::GetInstancePtr()->PortraitBGArray;
		
		FLinearColor color;
		if (CharacterTableInfo->Grade >= 1 && CharacterTableInfo->Grade < 4)
		{
			color = FLinearColor::Black;
			BG->Brush.SetResourceObject(BGArray[0]);
		}
		else if (CharacterTableInfo->Grade >= 4 && CharacterTableInfo->Grade < 6)
		{
			color = FColor(67, 186, 35);
			BG->Brush.SetResourceObject(BGArray[1]);
		}
		else if (CharacterTableInfo->Grade >= 6 && CharacterTableInfo->Grade < 8)
		{
			color = FColor(38, 174, 238);
			BG->Brush.SetResourceObject(BGArray[2]);
		}
		else if (CharacterTableInfo->Grade >= 8 && CharacterTableInfo->Grade < 10)
		{
			color = FColor(216, 158, 67);
			BG->Brush.SetResourceObject(BGArray[3]);
		}
		else
			BG->Brush.SetResourceObject(BGArray[4]);

		if (ImageBack /*&& CharacterTableInfo->Grade < 10*/)
			ImageBack->Brush.TintColor = FSlateColor(color);
	}
}

void URHeroIcon_ForInGame::UpdateIcon()
{
	FHERO* pHero = nullptr;
	URGameInstance* RGameInstance = RGAMEINSTANCE(this);
	pHero = RGameInstance->RInventory->GetHeroWithUD(HeroSlotInfo.HeroUD);
	//portrait
	if (pHero == nullptr)
		return;

// 	UObject* Portrait = UUIFunctionLibrary::GetHeroRoundPortrait(pHero->heroId);
// 	UUIFunctionLibrary::SetImageBrush(Portrait, Image_Hero);

	//HpBar
	if (IsValid(HeroSlotInfo.SpawnedTargetActor))
	{
		UBattleEntityComponent* BattleComp = UCharacterHelpFunctionLibrary::GetBattleEntityComponent(HeroSlotInfo.SpawnedTargetActor);

		float HPPercent = BattleComp->GetCurrentHPPercent();
		if (HpProgerssBar->Percent != HPPercent)
		{
			HpProgerssBar->SetPercent(HPPercent);
			float CurrentHP = BattleComp->GetCurrentHP();

			FText text = FText::FromString(FString::SanitizeFloat(CurrentHP));
			HpText->SetText(text);


			if (HPPercent < DangerValue)
			{
				Noti_Danger->SetVisibility(ESlateVisibility::HitTestInvisible);
				if (!this->IsPlayAni(TEXT("Danger_Noti")))
					this->PlayAni(TEXT("Danger_Noti"), false, 0);
			}
			else
			{
				Noti_Danger->SetVisibility(ESlateVisibility::Collapsed);
			}
			

			//
			URBaseIngameHUD* ThreeGameHUD = Cast<URBaseIngameHUD>(ParentWidget);
			if (IsValid(ThreeGameHUD))
			{
				if (HPPercent < DangerValue && HeroSlotInfo.CharacterRole == ECharacterRoleEnum::VE_Leader)
				{
					ThreeGameHUD->Noti_Danger->SetVisibility(ESlateVisibility::HitTestInvisible);
					if (!ThreeGameHUD->IsPlayAni(TEXT("Danger_Noti")))
						ThreeGameHUD->PlayAni(TEXT("Danger_Noti"), false, 0);
				}
				else
				{
					ThreeGameHUD->Noti_Danger->SetVisibility(ESlateVisibility::Collapsed);
				}
			}			
		}

		if (HPPercent <= 0) // when dead
		{
			Image_Hero->SetVisibility(ESlateVisibility::Collapsed);
			HpText->SetVisibility(ESlateVisibility::Collapsed);
			HpProgerssBar->SetVisibility(ESlateVisibility::Collapsed);
			Level_Text->SetVisibility(ESlateVisibility::Collapsed);

			HeroDeathPanel->SetVisibility(ESlateVisibility::HitTestInvisible);
		}
		else
		{
			Image_Hero->SetVisibility(ESlateVisibility::HitTestInvisible);
			HpText->SetVisibility(ESlateVisibility::HitTestInvisible);
			HpProgerssBar->SetVisibility(ESlateVisibility::HitTestInvisible);
			Level_Text->SetVisibility(ESlateVisibility::HitTestInvisible);

			HeroDeathPanel->SetVisibility(ESlateVisibility::Collapsed);
		}	
	}

// 	//Lv
// 	if (IsValid(Level_Text))
// 	{
// 		int32 HeroLv = HeroSlotInfo.HeroLevel;
// 		FText text = FText::FromString(FString::FromInt(HeroLv));
// 		Level_Text->SetText(text);
// 	}
}

void URHeroIcon_ForInGame::SetRTSHeroSlot(FHeroSlotInfo& InHeroSlotInfo)
{
	HeroSlotInfo = InHeroSlotInfo;

	FRTS_ACTOR* pHero = RGAMEINSTANCE(this)->RealTimeModManager->Get_RTS_Actor(HeroSlotInfo.PlayerUK);

	if (pHero == nullptr)
		return;

	UObject* Portrait = UUIFunctionLibrary::GetCharacterPortrait(ECharacterCategoryTypeEnum::VE_Hero, pHero->actorId);
	UUIFunctionLibrary::SetImageBrush(Portrait, Image_Hero);

	//Lv
	if (IsValid(Level_Text))
	{
		int32 HeroLv = HeroSlotInfo.HeroLevel;
		FText text = FText::FromString(FString::FromInt(HeroLv));
		Level_Text->SetText(text);
	}

	if (IsValid(BG))
	{
		FCharacterTableInfo* CharacterTableInfo = URenewal_BattleFunctionLibrary::GetCharacterTableInfo(ECharacterCategoryTypeEnum::VE_Hero, pHero->actorId);
		URGameInstance* RGameInstance = RGAMEINSTANCE(this);
		checkf(RGameInstance->RWidgetManager, TEXT("RWidgetManager null"));
		TArray<UPaperSprite*> BGArray = URAsyncLoader::GetInstancePtr()->PortraitBGArray;

		FLinearColor color;
		if (CharacterTableInfo->Grade >= 1 && CharacterTableInfo->Grade < 4)
		{
			color = FLinearColor::Black;
			BG->Brush.SetResourceObject(BGArray[0]);
		}
		else if (CharacterTableInfo->Grade >= 4 && CharacterTableInfo->Grade < 6)
		{
			color = FColor(67, 186, 35);
			BG->Brush.SetResourceObject(BGArray[1]);
		}
		else if (CharacterTableInfo->Grade >= 6 && CharacterTableInfo->Grade < 8)
		{
			color = FColor(38, 174, 238);
			BG->Brush.SetResourceObject(BGArray[2]);
		}
		else if (CharacterTableInfo->Grade >= 8 && CharacterTableInfo->Grade < 10)
		{
			color = FColor(216, 158, 67);
			BG->Brush.SetResourceObject(BGArray[3]);
		}
		else
			BG->Brush.SetResourceObject(BGArray[4]);

		if (ImageBack /*&& CharacterTableInfo->Grade < 10*/)
			ImageBack->Brush.TintColor = FSlateColor(color);
	}
}

void URHeroIcon_ForInGame::RTSUpdateIcon()
{
	FRTS_ACTOR* pHero = nullptr;
	URGameInstance* RGameInstance = RGAMEINSTANCE(this);
	pHero = RGameInstance->RealTimeModManager->Get_RTS_Actor(HeroSlotInfo.PlayerUK);
	//portrait
	if (pHero == nullptr)
		return;

	// 	UObject* Portrait = UUIFunctionLibrary::GetHeroRoundPortrait(pHero->heroId);
	// 	UUIFunctionLibrary::SetImageBrush(Portrait, Image_Hero);

	//HpBar
	if (IsValid(HeroSlotInfo.SpawnedTargetActor))
	{
		UBattleEntityComponent* BattleComp = UCharacterHelpFunctionLibrary::GetBattleEntityComponent(HeroSlotInfo.SpawnedTargetActor);

		float HPPercent = BattleComp->GetCurrentHPPercent();
		if (HpProgerssBar->Percent != HPPercent)
		{
			HpProgerssBar->SetPercent(HPPercent);
			float CurrentHP = BattleComp->GetCurrentHP();

			FText text = FText::FromString(FString::SanitizeFloat(CurrentHP));
			HpText->SetText(text);

			if (HPPercent < DangerValue)
			{
				Noti_Danger->SetVisibility(ESlateVisibility::HitTestInvisible);
				if (!this->IsPlayAni(TEXT("Danger_Noti")))
					this->PlayAni(TEXT("Danger_Noti"), false, 0);
			}
			else
			{
				Noti_Danger->SetVisibility(ESlateVisibility::Collapsed);
			}


			//
			URBaseIngameHUD* ThreeGameHUD = Cast<URBaseIngameHUD>(ParentWidget);
			if (IsValid(ThreeGameHUD))
			{
				if (HPPercent < DangerValue && HeroSlotInfo.CharacterRole == ECharacterRoleEnum::VE_Leader)
				{
					ThreeGameHUD->Noti_Danger->SetVisibility(ESlateVisibility::HitTestInvisible);
					if (!ThreeGameHUD->IsPlayAni(TEXT("Danger_Noti")))
						ThreeGameHUD->PlayAni(TEXT("Danger_Noti"), false, 0);
				}
				else
				{
					ThreeGameHUD->Noti_Danger->SetVisibility(ESlateVisibility::Collapsed);
				}
			}
		}

		if (HPPercent <= 0) // when dead
		{
			Image_Hero->SetVisibility(ESlateVisibility::Collapsed);
			HpText->SetVisibility(ESlateVisibility::Collapsed);
			HpProgerssBar->SetVisibility(ESlateVisibility::Collapsed);
			Level_Text->SetVisibility(ESlateVisibility::Collapsed);

			HeroDeathPanel->SetVisibility(ESlateVisibility::HitTestInvisible);
		}
		else
		{
			Image_Hero->SetVisibility(ESlateVisibility::HitTestInvisible);
			HpText->SetVisibility(ESlateVisibility::HitTestInvisible);
			HpProgerssBar->SetVisibility(ESlateVisibility::HitTestInvisible);
			Level_Text->SetVisibility(ESlateVisibility::HitTestInvisible);

			HeroDeathPanel->SetVisibility(ESlateVisibility::Collapsed);
		}
	}
}

void URHeroIcon_ForInGame::SetTeamColorSwitcherIndex(int32 InIndex)
{
	if (IsValid(TeamColorOutLineSwitcher))
	{
		TeamColorOutLineSwitcher->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
		TeamColorOutLineSwitcher->SetActiveWidgetIndex(InIndex);
	}

	if (IsValid(HpProgerssBar))
	{
		bool bEnemy = InIndex != 0;
		FLinearColor enemyColor;
		enemyColor = HpProgerssBar->FillColorAndOpacity;
		if(bEnemy)
		{
			enemyColor = FLinearColor(1.0f, 0.088656f, 0.046665f);
			HpProgerssBar->WidgetStyle.FillImage.TintColor = FSlateColor(FLinearColor(enemyColor));
		}
	}
}
