 // Fill out your copyright notice in the Description page of Project Settings.

#include "ProjectR.h"
#include "GlobalIntegrated.h"
#include "GameInfo/RInventory.h"
#include "Table/CharacterTableInfo.h"
#include "UtilFunctionIntegrated.h"
#include "UC_HeroDeck.h"
#include "UI/RWidgetManager.h"
#include "UI/DeckManagement/UP_DeckManager_New.h"
#include "UI/Common/HeroBaseScrollItem.h"
#include "UI/Common/HeroScrollItemData.h"
#include "UI/Control/RScrollItem.h"
#include "Network/RTSManager.h"


bool UUC_HeroDeck::Initialize()
{
	Super::Initialize();

	Deck_SelectingAnim = GetWidgetAnimFromName(TEXT("Deck_Selecting"));

	FindChildWidgetsWithCertainName<UImage>(this, HeroStarArray, TEXT("Hero_Star_"));

	return true;
}

void UUC_HeroDeck::NativePreConstruct()
{
	Super::NativePreConstruct();
}

void UUC_HeroDeck::NativeConstruct()
{
	Super::NativeConstruct();

	if (IsValid(Button_HeroCard))
	{
		Button_HeroCard->OnClicked.RemoveDynamic(this, &UUC_HeroDeck::OnClick_Deck);
		Button_HeroCard->OnClicked.AddDynamic(this, &UUC_HeroDeck::OnClick_Deck);
	}

	if (IsValid(Button_HeroCard))
	{
		Button_X->OnClicked.RemoveDynamic(this, &UUC_HeroDeck::OnClick_X);
		Button_X->OnClicked.AddDynamic(this, &UUC_HeroDeck::OnClick_X);
	}

	

}

void UUC_HeroDeck::SetHero(FString heroUD)
{
	CurHeroUD = TEXT("");
	Grade_10->SetVisibility(ESlateVisibility::Collapsed);
	Grade_soul->SetVisibility(ESlateVisibility::Collapsed);
	Image_Back->SetVisibility(ESlateVisibility::Collapsed);
	if (IsValid(CrusaderHP))
	{
		CrusaderHP->SetVisibility(ESlateVisibility::Collapsed);
	}

	if (IsValid(WidgetSwitcher_Select))
	{
		WidgetSwitcher_Select->SetActiveWidgetIndex(1);

		if (Lock_Switcher)
		{
			Lock_Switcher->SetActiveWidgetIndex(0);
		}

		URGameInstance* RGameInstance = RGAMEINSTANCE(this);
		if ( IsValid(RGameInstance->RInventory))
		{
			
			FHERO* outHero = RGameInstance->RInventory->GetHeroWithUD(heroUD);

			FHERO_ICON_INFO iconInfo;
			if (!heroUD.IsEmpty())
			{
				UUIFunctionLibrary::GetHeroIconInfoWithUD(iconInfo, heroUD, EHeroOwnershipType::VE_Me);

				FString enchantText = FString("+") + FString::FromInt(iconInfo.enchantPoint);
				Text_Strong->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
				Text_Strong->SetText(FText::FromString(enchantText));
			}

			if (outHero != nullptr)
			{
				CurHeroUD = heroUD;
				CurHeroID = outHero->heroId;
				CurHeroLevel = outHero->level;


				FCharacterTableInfo* CharacterTableInfo = UTableManager::GetInstancePtr()->GetCharacterRow(CurHeroID);
				checkf(CharacterTableInfo, TEXT("No valid HeroID In CharacterTable"));

				int32 grade = CharacterTableInfo->Grade;
				for (UImage* ImgStar : HeroStarArray)
					ImgStar->SetVisibility(ESlateVisibility::Collapsed);

				checkf(RGAMEINSTANCE(this)->RWidgetManager, TEXT("RWidgetManager null"));
				TArray<UPaperSprite*> BGArray = URAsyncLoader::GetInstancePtr()->HeroPortraitBGArray;
				
				FLinearColor color;
				if (grade >= 1 && grade < 4)
				{
					color = FLinearColor::Black;
					Grade_BG->Brush.SetResourceObject(BGArray[0]);
				}
				else if (grade >= 4 && grade < 6)
				{
					color = FColor(67, 186, 35);
					Grade_BG->Brush.SetResourceObject(BGArray[1]);
				}
				else if (grade >= 6 && grade < 8)
				{
					color = FColor(38, 174, 238);
					Grade_BG->Brush.SetResourceObject(BGArray[2]);
				}
				else if (grade >= 8 && grade < 10)
				{
					color = FColor(216, 158, 67);
					Grade_BG->Brush.SetResourceObject(BGArray[3]);
				}
				else
				{
					Grade_BG->Brush.SetResourceObject(BGArray[3]);
				}

				if (Image_Back && grade < 10)
					Image_Back->Brush.TintColor = FSlateColor(color);

				for (int32 i = 0; i < grade; i++)
					if (HeroStarArray.IsValidIndex(i))
						HeroStarArray[i]->SetVisibility(ESlateVisibility::SelfHitTestInvisible);

				if (grade <= 5)
				{
					Image_Back->SetVisibility(ESlateVisibility::HitTestInvisible);
					WidgetSwitcherStar->SetActiveWidgetIndex(0);
				}
				else if (grade < 10)
				{
					Image_Back->SetVisibility(ESlateVisibility::HitTestInvisible);
					WidgetSwitcherStar->SetActiveWidgetIndex(1);
				}
				else
				{
					WidgetSwitcherStar->SetActiveWidgetIndex(2);
					Image_Back->SetVisibility(ESlateVisibility::Collapsed);
				}
				if (CharacterTableInfo->IsHero)
					Grade_10->SetVisibility(ESlateVisibility::HitTestInvisible);
				else
					Grade_soul->SetVisibility(ESlateVisibility::HitTestInvisible);

				RefreshHeroDeck();
			}
		}
	}
}



void UUC_HeroDeck::SetHeroWithID(FString heroID)
{
	CurHeroID = FName(*heroID);
	CurHeroUD = TEXT("");
	Grade_10->SetVisibility(ESlateVisibility::Collapsed);
	Grade_soul->SetVisibility(ESlateVisibility::Collapsed);
	Image_Back->SetVisibility(ESlateVisibility::Collapsed);
	CurHeroLevel = 1;
	if (IsValid(CrusaderHP))
	{
		CrusaderHP->SetVisibility(ESlateVisibility::Collapsed);
	}

	if (IsValid(WidgetSwitcher_Select))
	{
		if(!heroID.IsEmpty())
			WidgetSwitcher_Select->SetActiveWidgetIndex(0);
		else
		{
			WidgetSwitcher_Select->SetActiveWidgetIndex(1);
			return;
		}


		if (Lock_Switcher)
		{
			Lock_Switcher->SetActiveWidgetIndex(0);
		}

		URGameInstance* RGameInstance = RGAMEINSTANCE(this);
		if ( IsValid(RGameInstance->RInventory))
		{

			FCharacterTableInfo* CharacterTableInfo = UTableManager::GetInstancePtr()->GetCharacterRow(FName(*heroID));
			checkf(CharacterTableInfo, TEXT("No valid HeroID In CharacterTable"));

			int32 grade = CharacterTableInfo->Grade;
			for (UImage* ImgStar : HeroStarArray)
				ImgStar->SetVisibility(ESlateVisibility::Collapsed);

			checkf(RGAMEINSTANCE(this)->RWidgetManager, TEXT("RWidgetManager null"));
			TArray<UPaperSprite*>& BGArray = URAsyncLoader::GetInstancePtr()->HeroPortraitBGArray;

			FLinearColor color;
			if (grade >= 1 && grade < 4)
			{
				color = FLinearColor::Black;
				Grade_BG->Brush.SetResourceObject(BGArray[0]);
			}
			else if (grade >= 4 && grade < 6)
			{
				color = FColor(67, 186, 35);
				Grade_BG->Brush.SetResourceObject(BGArray[1]);
			}
			else if (grade >= 6 && grade < 8)
			{
				color = FColor(38, 174, 238);
				Grade_BG->Brush.SetResourceObject(BGArray[2]);
			}
			else if (grade >= 8 && grade < 10)
			{
				color = FColor(216, 158, 67);
				Grade_BG->Brush.SetResourceObject(BGArray[3]);
			}
			else
			{
				Grade_BG->Brush.SetResourceObject(BGArray[3]);
			}

			if (Image_Back && grade < 10)
				Image_Back->Brush.TintColor = FSlateColor(color);

			for (int32 i = 0; i < grade; i++)
				if (HeroStarArray.IsValidIndex(i))
					HeroStarArray[i]->SetVisibility(ESlateVisibility::SelfHitTestInvisible);

			if (grade <= 5)
			{
				Image_Back->SetVisibility(ESlateVisibility::HitTestInvisible);
				WidgetSwitcherStar->SetActiveWidgetIndex(0);
			}
			else if (grade < 10)
			{
				Image_Back->SetVisibility(ESlateVisibility::HitTestInvisible);
				WidgetSwitcherStar->SetActiveWidgetIndex(1);
			}
			else
			{
				WidgetSwitcherStar->SetActiveWidgetIndex(2);
				Image_Back->SetVisibility(ESlateVisibility::Collapsed);
			}

			if(CharacterTableInfo->IsHero)
				Grade_10->SetVisibility(ESlateVisibility::HitTestInvisible);
			else
				Grade_soul->SetVisibility(ESlateVisibility::HitTestInvisible);

			RefreshHeroDeck();
	
		}
	}
}




void UUC_HeroDeck::SetAvailable(bool inAvailable)
{
	if (inAvailable)
	{
		NotiImage->SetVisibility(ESlateVisibility::HitTestInvisible);
		UWidgetAnimation* Anim = GetWidgetAnimFromName(TEXT("Deck_Avaiable"));
		PlayAnimation(Anim, 0.f, 0);
	}
	else
	{
		NotiImage->SetVisibility(ESlateVisibility::Hidden);
		Image_Selecting->SetVisibility(ESlateVisibility::Hidden);
	}
		
	
}



void UUC_HeroDeck::SetSelecting(bool bSelecting)
{
	if (bSelecting)
	{
		Image_Selecting->SetVisibility(ESlateVisibility::HitTestInvisible);
		UWidgetAnimation* Anim = GetWidgetAnimFromName(TEXT("Deck_Selecting"));
		PlayAnimation(Anim, 0.f, 0);
	}
	else
	{
		Image_Selecting->SetVisibility(ESlateVisibility::Collapsed);
	}
}

void UUC_HeroDeck::OnClick_X()
{
	
// 	UUP_DeckManager* DeckManager = Cast<UUP_DeckManager>(ParentWidget);
// 	if (!IsValid(DeckManager))
// 		return;
// 	
// 	UUC_HeroManagement_HeroList* rHeroList = DeckManager->UC_Common_HeroList; // bOneControl ? DeckManager->UC_Common_HeroList : DeckManager_Three->UC_Common_HeroList;
// 	UUC_BaseHeroScrollView* HeroScrollView = Cast<UUC_BaseHeroScrollView>(rHeroList->GetScrollView());
// 
// 	TArray<int32>& SelectedArray = HeroScrollView->GetSelectedScrollItem();
// 	TArray<URScrollItemData*>& SelectedDataArray = HeroScrollView->GetSelectedScrollItemData();
// 	TArray<URScrollItemData*>& ItemDataArray = HeroScrollView->ItemDataList;
// 	for (size_t i = 0; i < SelectedArray.Num(); )
// 	{
// 		UHeroScrollItemData* ItemData = Cast<UHeroScrollItemData>(ItemDataArray[SelectedArray[i]]);
// 		if (!ItemData)
// 		{
// 			++i;
// 			continue;
// 		}
// 			
// 
// 		FHERO_ICON_INFO heroinfo = ItemData->HeroIconInfo;
// 		if (heroinfo.heroUD == CurHeroUD)
// 		{
// 			DeckManager->SelectHero(CurHeroUD);
// 			SelectedDataArray.Remove(ItemData);
// 			SelectedArray.RemoveAt(i);
// 			HeroScrollView->Update_ChangeItemDataList();
// 		}
// 		else
// 			++i;
// 	}
// 	
// 	this->HeroLevelStarInfo->SetVisibility(ESlateVisibility::Collapsed);
// 	Text_Strong->SetVisibility(ESlateVisibility::Collapsed);
// 
// 
// 	if(!HeroScrollView->bSelecting6thHero)
// 	{
// 		DeckManager->AdjustReasonableSpot();
// 	}
// 	
// 	DeckManager->ResetDeckSetting();
// 	ReqDeckSelect();
// 
// 	isSelecting = false;
}


void UUC_HeroDeck::ReqDeckSelect()
{
	if (URTSManager::GetInstancePtr()->IsConnectedMode(EPVP::SR) || URTSManager::GetInstancePtr()->IsConnectedMode(EPVP::MR)
		|| URTSManager::GetInstancePtr()->IsConnectedMode(EPVP::CS))
	{
		URGameInstance* RGameInstance = RGAMEINSTANCE(this);
		int32 iSelectingIdx = INDEX_NONE;
		int32 iMyIdx = INDEX_NONE;
		UUP_DeckManager_New* DeckManager = Cast<UUP_DeckManager_New>(ParentWidget);
		if (!IsValid(DeckManager))
			return;
		TArray<UUC_HeroDeck*>& rHeroDeck = DeckManager->Variables.HeroDeckList;

		for (size_t i = 0; i < rHeroDeck.Num(); ++i)
		{
			if (rHeroDeck[i]->isSelecting)
				iSelectingIdx = i;
			if (rHeroDeck[i] == this)
				iMyIdx = i;
		}
		URTSManager::GetInstancePtr()->REQ_DECK_SELECT(RGameInstance->RealTimeModManager->RTSInfo.mSlotNo, iMyIdx + 1, CurHeroUD, CurHeroID.ToString(), CurHeroLevel);
	}
}

void UUC_HeroDeck::RefreshHeroDeck()
{
	WidgetSwitcher_Select->SetActiveWidgetIndex(0);

	

	FCharacterTableInfo* CharacterTableInfo = UTableManager::GetInstancePtr()->GetCharacterRow(CurHeroID);
	if (CharacterTableInfo)
	{
		HeroLevelStarInfo->SetVisibility(ESlateVisibility::HitTestInvisible);
		Text_Strong->SetVisibility(ESlateVisibility::Collapsed);

		
		for (size_t i = 0; i < HeroStarArray.Num(); ++i)
		{
			if(i < CharacterTableInfo->Grade)
				HeroStarArray[i]->SetVisibility(ESlateVisibility::HitTestInvisible);
			else
				HeroStarArray[i]->SetVisibility(ESlateVisibility::Collapsed);
		}

		if (IsValid(Image_HeroCard))
		{
			Image_HeroCard->SetBrushFromTexture(UUtilFunctionLibrary::GetTexture2DFromTable(CharacterTableInfo->RuneCardImage));
		}

		if (IsValid(Image_HeroJob))
		{
			UPaperSprite* Sprite = UResourceFunctionLibrary::GetHeroClassTypeSprite(CharacterTableInfo->Class);
			UUIFunctionLibrary::SetImageBrush(Sprite, Image_HeroJob);
		}

		if (IsValid(Image_Continent))
		{
			UPaperSprite* Sprite = UResourceFunctionLibrary::GetHeroContinentTypeSprite(CharacterTableInfo->Continent);
			if (Sprite)
			{
				UUIFunctionLibrary::SetImageBrush(Sprite, Image_Continent);
			}
		}

		if (IsValid(TextBlock_HeroLevel))
		{
			TextBlock_HeroLevel->SetText(FText::AsNumber(CurHeroLevel));
		}

		if (IsValid(TextBlock_HeroName))
		{
			TextBlock_HeroName->SetText(UUtilFunctionLibrary::GetLocalizedString(EStringTableType::VE_Hero, CharacterTableInfo->DisplayName));
		}
	}

	if (IsValid(CrusaderHP))
	{
		CrusaderHP->SetVisibility(ESlateVisibility::Collapsed);
	}

// 	if (CurHeroUD.Len() > 0)
// 	{
// 		bool bOneControl = false;
// 		UUP_DeckManager_ThreeControl* DeckManager_Three = Cast<UUP_DeckManager_ThreeControl>(ParentWidget);
// 		if (!IsValid(DeckManager_Three))
// 		{
// 			return;
// 		}
//
// 		UUP_DeckManager_OneControl* DeckManager_One = Cast<UUP_DeckManager_OneControl>(ParentWidget);
// 		if (IsValid(DeckManager_One))
// 		{
// 			bOneControl = true;
// 		}
// 		UUP_DeckManager* DeckManager = Cast<UUP_DeckManager>(ParentWidget);
//
// 		if (IsValid(DeckManager_Three))
// 		{
// 			// crusader는 3인전투라서 1인전투일경우는 체크할필요없음
// // 			EDeckUsingTypeEnum eDeckType = bOneControl ? DeckManager_One->GetDeckType() : DeckManager_Three->GetDeckType();
// 			if (DeckManager_Three->GetDeckType() == EDeckUsingTypeEnum::VE_Crusader)
// 			{
// 				if (IsValid(CrusaderHP))
// 				{
// 					CrusaderHP->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
// 				}
// 
// 				URGameInstance* RGameInstance = RGAMEINSTANCE(this);
// 				if ( IsValid(RGameInstance->CrusaderManager))
// 				{
// 					float HpRate = 1.0f;
// 					if (RGameInstance->CrusaderManager->MyHeroHpMap.Contains(CurHeroUD))
// 					{
// 						HpRate = (float)RGameInstance->CrusaderManager->MyHeroHpMap[CurHeroUD].hp * 0.01f;
// 					}
// 
// 					if (IsValid(Hp_Progressbar))
// 					{
// 						Hp_Progressbar->SetPercent(HpRate);
// 					}
// 
// 					if (IsValid(Hp_TextBlock))
// 					{
// 						Hp_TextBlock->SetText(FText::AsPercent(HpRate));
// 					}
// 				}
// 			}
// 		}
// 	}

}

