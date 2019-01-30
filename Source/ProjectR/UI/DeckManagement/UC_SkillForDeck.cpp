// Fill out your copyright notice in the Description page of Project Settings.

#include "ProjectR.h"
#include "GlobalIntegrated.h"
#include "UI/DeckManagement/UC_HeroDeck.h"
#include "GameInfo/RInventory.h"
#include "UtilFunctionIntegrated.h"
#include "UI/HeroManagement/UC_SkillManagement.h"
#include "UC_SkillForDeck.h"




void UUC_SkillForDeck::NativeConstruct()
{
	Super::NativeConstruct();

	if (IsValid(Button_Click))
	{
		Button_Click->OnClicked.RemoveDynamic(this, &UUC_SkillForDeck::PressSkill);
		Button_Click->OnClicked.AddDynamic(this, &UUC_SkillForDeck::PressSkill);
	}



}


void UUC_SkillForDeck::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);

	if (Image_SkillIcon->GetVisibility() == ESlateVisibility::Hidden)
		CurrentHeroUD.Empty();
	
}

void UUC_SkillForDeck::PressSkill()
{
	PlayAni(TEXT("Click_Pressed"));
	
}

void UUC_SkillForDeck::SetSkillIcon(FString InHeroUD, bool isEliteMode)
{
	if (InHeroUD.IsEmpty())
	{
		Image_Portrait->SetVisibility(ESlateVisibility::Collapsed);
		PanelUnlock->SetVisibility(ESlateVisibility::Visible);
		Image_SkillIcon->SetVisibility(ESlateVisibility::Collapsed);
		return;
	}

	CurrentHeroUD = InHeroUD;
	URGameInstance* RGameInstance = RGAMEINSTANCE(this);
	FHERO* pHero = RGameInstance->RInventory->GetHeroWithUD(InHeroUD);
	
	SetSkillIconWithID(pHero->heroId, isEliteMode);
}

void UUC_SkillForDeck::SetSkillIconWithID(FName HeroID, bool isEliteMode)
{
	if (HeroID.IsNone())
	{
		Image_Portrait->SetVisibility(ESlateVisibility::Collapsed);
		PanelUnlock->SetVisibility(ESlateVisibility::Visible);
		Image_SkillIcon->SetVisibility(ESlateVisibility::Collapsed);
		return;
	}

	FCharacterTableInfo* CharacterTableInfo1 = UTableManager::GetInstancePtr()->GetCharacterRow(HeroID);

	if (CharacterTableInfo1)
	{
		FString strTexKey = CharacterTableInfo1->RoundPortraitTextureKey.ToString();
		UObject* Portrait = UUIFunctionLibrary::GetIconImageFromTable(FName(*strTexKey));
		UUIFunctionLibrary::SetImageBrush(Portrait, Image_Portrait);
		Image_Portrait->SetVisibility(ESlateVisibility::HitTestInvisible); // Portrait Setting
	}


	FString SkillIDName;

	if (!isEliteMode)
	{
		SkillIDName = CharacterTableInfo1->ActiveSkill03.ToString() + "_1";
		auto* skillData = UTableManager::GetInstancePtr()->GetSkillRow_V2(FName(*SkillIDName));
		if (skillData)
		{
			FSkillResourceTableInfo* ResourceInfo = UTableManager::GetInstancePtr()->GetSkillResourceRow(skillData->SkillResourceID);
			Image_SkillIcon->SetBrushFromTexture(UUtilFunctionLibrary::GetTexture2DFromAssetID(ResourceInfo->SkillIconRef));
		}
	}
	else
	{
		SkillIDName = CharacterTableInfo1->SpiritPassive.ToString();
		FSkillTableInfo_V2* SpiritSkill = UTableManager::GetInstancePtr()->GetSkillRow_V2(CharacterTableInfo1->SpiritPassive);
		if (SpiritSkill)
		{
			FSkillResourceTableInfo* SkillResourceRow = UTableManager::GetInstancePtr()->GetSkillResourceRow(SpiritSkill->SkillResourceID);
			if (SkillResourceRow)
			{
				if (IsValid(Image_SkillIcon))
				{
					Image_SkillIcon->SetBrushFromTexture(UUtilFunctionLibrary::GetTexture2DFromAssetID(SkillResourceRow->SkillIconRef));
				}
			}
		}
	}

	if (SkillIDName.Contains(TEXT("None")))
	{
		PanelUnlock->SetVisibility(ESlateVisibility::Visible);
		Image_SkillIcon->SetVisibility(ESlateVisibility::Collapsed);

		return;
	}

	PanelUnlock->SetVisibility(ESlateVisibility::Collapsed);
	Image_SkillIcon->SetVisibility(ESlateVisibility::HitTestInvisible);
}

void UUC_SkillForDeck::InitSkillIcon()
{
	PanelUnlock->SetVisibility(ESlateVisibility::Collapsed);
	Image_Portrait->SetVisibility(ESlateVisibility::Collapsed);
	Image_SkillIcon->SetVisibility(ESlateVisibility::Collapsed);
}

void UUC_SkillForDeck::ChangeSkillIconImage(FString InHeroUD, int32 _index, bool _isRecursive)
{
	if (_index != (int32)MyRole && _isRecursive == false)
	{
		PanelUnlock->SetVisibility(ESlateVisibility::Collapsed);
		Image_Portrait->SetVisibility(ESlateVisibility::Hidden);
		Image_SkillIcon->SetVisibility(ESlateVisibility::Hidden);
		Image_SkillIcon->SetBrushFromTexture(nullptr);

		//if (MyRole == ESkillRole::Crew1_Second || MyRole == ESkillRole::Crew1_First || MyRole == ESkillRole::Leader) //when deleting crew1_second
		//{
		//	for (auto Element : ChildSkills)
		//		Element->ChangeSkillIconImage(InHeroUD, -1);
		//}
		
		return;
	}

	CurrentHeroUD = InHeroUD;

// 	UUP_DeckManager* Deck_Manager = Cast<UUP_DeckManager>(ParentWidget);
// 	if (!IsValid(Deck_Manager))
// 		return;

// 	bool bOneControl = false;
// 	UUP_DeckManager_ThreeControl*	DeckManager_Three = Cast<UUP_DeckManager_ThreeControl>(ParentWidget);
// 	UUP_DeckManager_OneControl*		DeckManager_One = Cast<UUP_DeckManager_OneControl>(ParentWidget);
// 	if (IsValid(DeckManager_One))
// 	{
// 		bOneControl = true;
// 	}

	URGameInstance* RGameInstance = RGAMEINSTANCE(this);
	FHERO* pHero = RGameInstance->RInventory->GetHeroWithUD(InHeroUD);
	UObject* Portrait = UUIFunctionLibrary::GetHeroRoundPortrait(pHero->heroId);
	UUIFunctionLibrary::SetImageBrush(Portrait, Image_Portrait);
	Image_Portrait->SetVisibility(ESlateVisibility::HitTestInvisible); // Portrait Setting

	//Skill Icon
	FCharacterTableInfo* CharacterTableInfo1 = UTableManager::GetInstancePtr()->GetCharacterRow(pHero->heroId);
	FString SkillIDName = CharacterTableInfo1->ActiveSkill03.ToString() + "_1";
	if (SkillIDName.Contains(TEXT("None")))
	{
		PanelUnlock->SetVisibility(ESlateVisibility::Visible);
		Image_SkillIcon->SetVisibility(ESlateVisibility::Collapsed);
		
		return;
	}

	PanelUnlock->SetVisibility(ESlateVisibility::Collapsed);
	Image_SkillIcon->SetVisibility(ESlateVisibility::HitTestInvisible);

	auto* skillData = UTableManager::GetInstancePtr()->GetSkillRow_V2(FName(*SkillIDName));
	if (skillData)
	{
		FSkillResourceTableInfo* ResourceInfo = UTableManager::GetInstancePtr()->GetSkillResourceRow(skillData->SkillResourceID);
		Image_SkillIcon->SetBrushFromTexture(UUtilFunctionLibrary::GetTexture2DFromAssetID(ResourceInfo->SkillIconRef));

		
	}
	
}


