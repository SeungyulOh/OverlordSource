// Fill out your copyright notice in the Description page of Project Settings.

#include "ProjectR.h"

#include "GlobalIntegrated.h"

#include "Utils/UIFunctionLibrary.h"
#include "Utils/UtilFunctionLibrary.h"
#include "Utils/SharedUtils/Renewal_BattleFunctionLibrary.h"

#include "UI/Common/HeroScrollItemData.h"
#include "UI/Common/HeroBaseScrollItem.h"

#include "UI/HeroInfo/HeroManager/UP_HeroManager.h"
#include "Ui/HeroInfo/HeroManager/UC_HeroManager_Info.h"

#include "UP_GenericScrollview.h"

#include "UC_HeroInfo_Upgrade.h"
#include "ResourceFunctionLibrary.h"
#include "Client_BattleFunctionLibrary.h"


void UUC_HeroInfo_Upgrade::NativeConstruct()
{
	Super::NativeConstruct();
	Init();
}

void UUC_HeroInfo_Upgrade::NativeDestruct()
{
	ResetAnim();

	SeletedHeroUDs.Empty();

	Super::NativeDestruct();
}

void UUC_HeroInfo_Upgrade::Init()
{
	Button_Upgrade->OnClicked.Clear();
	Button_Upgrade->OnClicked.AddDynamic(this, &UUC_HeroInfo_Upgrade::OnClickUpgrade);
	Button_Upgrade->SetIsEnabled(false);

	if (IsValid(MaterialHeroIcon[0]) && MaterialHeroIcon.IsValidIndex(0))
	{
		if (IsValid(MaterialHeroIcon[0]->Button_Select))
		{
			MaterialHeroIcon[0]->Button_Select->OnClicked.RemoveDynamic(this, &UUC_HeroInfo_Upgrade::OnClickIcon1);
			MaterialHeroIcon[0]->Button_Select->OnClicked.AddDynamic(this, &UUC_HeroInfo_Upgrade::OnClickIcon1);
		}
	}
	if (IsValid(MaterialHeroIcon[1]) && MaterialHeroIcon.IsValidIndex(1))
	{
		if (IsValid(MaterialHeroIcon[1]->Button_Select))
		{
			MaterialHeroIcon[1]->Button_Select->OnClicked.RemoveDynamic(this, &UUC_HeroInfo_Upgrade::OnClickIcon2);
			MaterialHeroIcon[1]->Button_Select->OnClicked.AddDynamic(this, &UUC_HeroInfo_Upgrade::OnClickIcon2);
		}
	}
	if (IsValid(MaterialHeroIcon[2]) && MaterialHeroIcon.IsValidIndex(2))
	{
		if (IsValid(MaterialHeroIcon[2]->Button_Select))
		{
			MaterialHeroIcon[2]->Button_Select->OnClicked.RemoveDynamic(this, &UUC_HeroInfo_Upgrade::OnClickIcon3);
			MaterialHeroIcon[2]->Button_Select->OnClicked.AddDynamic(this, &UUC_HeroInfo_Upgrade::OnClickIcon3);
		}
	}

	UUP_GenericScrollview* genericscrollview = UUIFunctionLibrary::GetGenericScrollview();
	if (genericscrollview)
	{
		genericscrollview->OnDisappear.Unbind();
		genericscrollview->OnDisappear.BindUObject(this, &UUC_HeroInfo_Upgrade::ResetAnim);
	}


	if (IsValid(Button_Close))
	{
		Button_Close->OnClicked.Clear();
		Button_Close->OnClicked.AddDynamic(this, &UUC_HeroInfo_Upgrade::OnClickClose);
	}
	if (IsValid(Button_Cancel))
	{
		Button_Cancel->OnClicked.Clear();
		Button_Cancel->OnClicked.AddDynamic(this, &UUC_HeroInfo_Upgrade::OnClickClose);
	}

	for (int32 i = 0; i < 5; i++)
	{
		TArray<FString>		Temp;
		Slot.Add(i, Temp);
	}

	AIndoorLobbyStateActor* LobbyStateActor = Cast<AIndoorLobbyStateActor>(RGAMEINSTANCE(this)->GetTowerLobbyGameMode()->GetLobbyStateActor(ETowerLobbyStateEnum::Indoor));
	if (LobbyStateActor)
	{
		SetHero(LobbyStateActor->CurrentHeroUD);
	}


}

void UUC_HeroInfo_Upgrade::SetHero(FString HeroUD)
{
	URGameInstance*			RGameInstance	=	RGAMEINSTANCE(this);
	checkf(RGameInstance != nullptr, TEXT("UUC_HeroInfo_Upgrade[RGameInstance Is Empty]"));

	m_Hero			=	RGameInstance->RInventory->GetHeroWithUD(HeroUD);
	checkf(m_Hero != nullptr, TEXT("UUC_HeroInfo_Upgrade HeroObject Is Empty"));

	m_PrevTable		=	UTableManager::GetInstancePtr()->GetCharacterRow(m_Hero->heroId);
	checkf(m_PrevTable != nullptr, TEXT("UUC_HeroInfo_Upgrade CharacterTable Is Empty"));

	m_NextTable		=	UTableManager::GetInstancePtr()->GetCharacterRow(m_PrevTable->UpgradeID);
	
	if (m_NextTable == nullptr)
		return;

	SetHeroIcon();
	SetStat();
	SetUpgradeCost();
}

void UUC_HeroInfo_Upgrade::SetHeroIcon()
{
	if (m_Hero == nullptr)
		return;
	if (m_PrevTable == nullptr)
		return;

	if(IsValid(PrevIcon))
		PrevIcon->SetHeroIconInfoWithUD(m_Hero->heroUD, EHeroOwnershipType::VE_Me);

	for (size_t i = 0; i < CurrentHeroStarArray.Num(); ++i)
	{
		CurrentHeroStarArray[i]->SetStar(m_Hero->heroUD);
	}

	if (IsValid(NextHeroStar))
	{
		NextHeroStar->SetStar(m_NextTable->Grade);
	}



	if (IsValid(AttackPower))
	{
		int32 nCombatStat = UClient_BattleFunctionLibrary::GetHeroCombatStat(m_Hero->heroUD);
		AttackPower->SetText(FText::AsNumber(nCombatStat));
	}

	if (IsValid(HeroJob))
	{
		UPaperSprite* Sprite = UResourceFunctionLibrary::GetHeroClassTypeSprite(m_PrevTable->Class);
		if (Sprite)
			UUIFunctionLibrary::SetImageBrush(Sprite, HeroJob);
	}

	if (IsValid(HeroName))
	{
		FText name = UUtilFunctionLibrary::GetLocalizedString(EStringTableType::VE_Hero, m_PrevTable->DisplayName);
		HeroName->SetText(name);
	}

	if (IsValid(HeroNickName))
	{
		FText nickname = UUtilFunctionLibrary::GetLocalizedString(EStringTableType::VE_Hero, m_PrevTable->Nick);
		HeroNickName->SetText(nickname);
	}

	if (LevelText)
	{
		LevelText->SetText(FText::Format(FText::FromString(TEXT("{0}/{1}")), m_Hero->level, m_PrevTable->MaxLevel));
		if (m_Hero->level < m_PrevTable->MaxLevel)
			LevelText->SetColorAndOpacity(FSlateColor(FLinearColor::Red));
		else
			LevelText->SetColorAndOpacity(FSlateColor(FLinearColor::White));
	}
		

	
	 
	
}

void UUC_HeroInfo_Upgrade::SetStat()
{
	if (StatInfo.GetAllocatedSize() == 0)
		return;

	if (m_PrevTable == nullptr || m_NextTable == nullptr)
		return;

	for (int32 i = 0; i < StatInfo.Num(); i++)
	{
		if (StatInfo.IsValidIndex(i) == false || IsValid(StatInfo[i]) == false)
			continue;

		StatInfo[i]->Refresh(m_PrevTable, m_NextTable);
	}
}

void UUC_HeroInfo_Upgrade::SetUpgradeCost()
{
	URGameInstance*		RGameInstance = RGAMEINSTANCE(this);

	if (m_Hero == nullptr)
		return;

	m_UpgradeCost = UTableManager::GetInstancePtr()->GetCharacterCostUpgradeRow(m_Hero->heroId);
	m_UpgradeCost->DataSetting();

	/*How many heros needed for upgrade*/
	int32 RequiredHeroCount = m_UpgradeCost->CharacterNum[0];
	SeletedHeroUDs.Init(TEXT(""), RequiredHeroCount);
	/**/

	const int32 MyGold = RGameInstance->RInventory->GetGold();

	if (IsValid(Text_ActiveCurrency))
	{
		if (m_UpgradeCost->Gold > MyGold)
			Text_ActiveCurrency->SetColorAndOpacity(FSlateColor(FLinearColor(1.f, 0.04f, 0.04f, 1.f)));
		else
			Text_ActiveCurrency->SetColorAndOpacity(FSlateColor(FLinearColor(1.0f, 1.0f, 1.0f, 1.0f)));

		Text_ActiveCurrency->SetText(FText::AsNumber(m_UpgradeCost->Gold));
	}

	m_MaterialIndex = 0;

	for (int32 i = 0; i < MaterialHeroIcon.Num(); i++)
	{
		if (MaterialHeroIcon.IsValidIndex(i) == false || IsValid(MaterialHeroIcon[i]) == false)
			continue;

		MaterialHeroIcon[i]->SetVisibility(ESlateVisibility::Visible);

		if (MaterialVerticalBoxArray.IsValidIndex(i))
			MaterialVerticalBoxArray[i]->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
	}

	FWalletTableInfo*	WalletInfo = UTableManager::GetInstancePtr()->GetWalletRow(WALLET_HERO_UPGRADEMATERIAL);
	checkf(WalletInfo != nullptr, TEXT("UP_RESULT WALLET_HERO_UPGRADEMATERIAL Is Empty"));

	const int32 MyUpgradeMaterial = RGameInstance->RInventory->GetHeroUpgradeMaterial();
	
	for (size_t i = 1; i < RequiredMaterialTextArray.Num(); ++i)
		RequiredMaterialTextArray[i]->SetText(FText::AsNumber(1));
	for (size_t i = 1; i < SelectedMaterialTextArray.Num(); ++i)
		SelectedMaterialTextArray[i]->SetText(FText::AsNumber(0));


	if (IsValid(MaterialIcon))
	{
		if (m_UpgradeCost->HeroUpgradeMaterial > MyUpgradeMaterial)
		{
			RequiredMaterialTextArray[0]->SetText(FText::AsNumber(m_UpgradeCost->HeroUpgradeMaterial));

			SelectedMaterialTextArray[0]->SetText(FText::AsNumber(MyUpgradeMaterial));
			SelectedMaterialTextArray[0]->SetColorAndOpacity(FSlateColor(FLinearColor(1.f, 0.04f, 0.04f, 1.f)));
		}
		else
		{
			RequiredMaterialTextArray[0]->SetText(FText::AsNumber(m_UpgradeCost->HeroUpgradeMaterial));

			SelectedMaterialTextArray[0]->SetText(FText::AsNumber(m_UpgradeCost->HeroUpgradeMaterial));
			SelectedMaterialTextArray[0]->SetColorAndOpacity(FSlateColor(FLinearColor(1.0f, 1.0f, 1.0f, 1.0f)));
		}

		MaterialIcon->Refresh(WalletInfo->ItemIconKey, MyUpgradeMaterial, false, 0);
	}

	int32 iCount = m_UpgradeCost->CharacterCondition.Num();

	EUpgradeCondition	CharacterCondition = m_UpgradeCost->CharacterCondition[0];
	FString				CharacterValue = m_UpgradeCost->CharacterValue[0];
	int32				CharacterGrade = m_UpgradeCost->CharacterGrade[0];
	int32				CharacterNum = m_UpgradeCost->CharacterNum[0];
	EClassTypeEnum Class = GetMaterialClassType(0);
	for (size_t i = 0; i < CharacterNum; ++i)
	{
		if (CharacterCondition == EUpgradeCondition::Hero_Class)
		{
			if (Class == EClassTypeEnum::VE_ClassTypeMax)
				MaterialHeroIcon[i]->SetAllIcon(CharacterCondition, CharacterGrade, 1, false);
			else
				MaterialHeroIcon[i]->SetClassIcon(CharacterCondition, Class, CharacterGrade, 1, false);
		}
	}
}



void UUC_HeroInfo_Upgrade::Filtering(TArray<FString>& outArray)
{
	URGameInstance*		RGameInstance = RGAMEINSTANCE(this);
	TArray<FString>	HeroUDs;

	EHeroInventoryType InventoryType = EHeroInventoryType::VE_InventoryTypeAll;

	if (m_UpgradeCost == nullptr)
		return;


	URInventory* Inventory = RGAMEINSTANCE(GEngine)->RInventory;
	TArray<FString> InitialHeroUDs;
	Inventory->GetHeroUDArray(InitialHeroUDs);
	for (size_t j = 0; j < InitialHeroUDs.Num();)
	{
		FHERO* hero = Inventory->GetHeroWithUD(InitialHeroUDs[j]);
		if (hero)
		{
			FCharacterTableInfo* tableinfo = UTableManager::GetInstancePtr()->GetCharacterRow(hero->heroId);
			if (tableinfo && !tableinfo->IsHero)
			{
				InitialHeroUDs.RemoveAt(j);
				continue;
			}
		}
		++j;
	}

	auto ClassFilter = [InitialHeroUDs, Inventory](TArray<FString>& OutArray, EClassTypeEnum classtype) {
		for (size_t i = 0; i < InitialHeroUDs.Num(); ++i)
		{
			FString heroUD = InitialHeroUDs[i];
			FHERO* heroinfo = Inventory->GetHeroWithUD(heroUD);
			FCharacterTableInfo* tableinfo = UTableManager::GetInstancePtr()->GetCharacterRow(heroinfo->heroId);
			if (tableinfo)
			{
				if (tableinfo->Class == classtype)
				{
					OutArray.AddUnique(heroUD);
				}
			}
		}
	};

	EClassTypeEnum type = GetMaterialClassType(0);
	int32 TargetGrade = m_UpgradeCost->CharacterGrade[0];
	TArray<FString> ClassTypeResult;
	ClassFilter(ClassTypeResult, type);

	for (size_t j = 0; j < ClassTypeResult.Num();)
	{
		FHERO* hero = Inventory->GetHeroWithUD(ClassTypeResult[j]);
		if (hero)
		{
			FCharacterTableInfo* tableinfo = UTableManager::GetInstancePtr()->GetCharacterRow(hero->heroId);
			if (tableinfo->Grade != TargetGrade)
			{
				ClassTypeResult.RemoveAt(j);
				continue;
			}
			
			int32 FoundIdx = SeletedHeroUDs.Find(hero->heroUD);
			if (FoundIdx != INDEX_NONE)
			{
				ClassTypeResult.RemoveAt(j);
				continue;
			}

			if (m_Hero->heroUD == ClassTypeResult[j])
			{
				ClassTypeResult.RemoveAt(j);
				continue;
			}

			FoundIdx = Inventory->GetHeroDeck().arena.Find(hero->heroUD);
			if (FoundIdx != INDEX_NONE)
			{
				ClassTypeResult.RemoveAt(j);
				continue;
			}

			FoundIdx = Inventory->GetHeroDeck().mirror.Find(hero->heroUD);
			if (FoundIdx != INDEX_NONE)
			{
				ClassTypeResult.RemoveAt(j);
				continue;
			}

			FoundIdx = Inventory->GetHeroDeck().defense.Find(hero->heroUD);
			if (FoundIdx != INDEX_NONE)
			{
				ClassTypeResult.RemoveAt(j);
				continue;
			}
		}
		++j;
	}

	outArray = ClassTypeResult;
}

EClassTypeEnum UUC_HeroInfo_Upgrade::GetMaterialClassType(int32 InMaterialIndex)
{
	if (InMaterialIndex == 0xffff)
		return EClassTypeEnum::VE_ClassTypeMax;

	if (m_UpgradeCost->CharacterValue[InMaterialIndex].Compare("Tanker") == 0)
		return EClassTypeEnum::VE_Tanker;
	else if (m_UpgradeCost->CharacterValue[InMaterialIndex].Compare("MeleeDealer") == 0)
		return EClassTypeEnum::VE_MeleeDealer;
	else if (m_UpgradeCost->CharacterValue[InMaterialIndex].Compare("RangeDealer") == 0)
		return EClassTypeEnum::VE_RangeDealer;
	else if (m_UpgradeCost->CharacterValue[InMaterialIndex].Compare("Healer") == 0)
		return EClassTypeEnum::VE_Healer;
	else if (m_UpgradeCost->CharacterValue[InMaterialIndex].Compare("Buffer") == 0)
		return EClassTypeEnum::VE_Buffer;
	else if (m_UpgradeCost->CharacterValue[InMaterialIndex].Compare("Supporter") == 0)
		return EClassTypeEnum::VE_Supporter;
	else if (m_UpgradeCost->CharacterValue[InMaterialIndex].Compare("MagicDealer") == 0)
		return EClassTypeEnum::VE_MagicDealer;

	return EClassTypeEnum::VE_ClassTypeMax;
}





void UUC_HeroInfo_Upgrade::ResetAnim()
{
	for (int32 i = 0; i < MaterialHeroIcon.Num(); i++)
	{
		MaterialHeroIcon[i]->StopAni(TEXT("Selecting"));
		MaterialHeroIcon[i]->Image_Selected->SetVisibility(ESlateVisibility::Collapsed);
	}
}

void UUC_HeroInfo_Upgrade::SelectHeroScrollItem(URScrollItem* scroll_item)
{
	URGameInstance*		RGameInstance = RGAMEINSTANCE(this);

	if (Slot.Find(m_MaterialIndex) == nullptr)
		return;

	if (SelectedMaterialTextArray.IsValidIndex(m_MaterialIndex + 1))
	{
		SelectedMaterialTextArray[m_MaterialIndex + 1]->SetText(FText::AsNumber(1));
		MaterialHeroIcon[m_MaterialIndex]->SetIsSelected(true);
		MaterialHeroIcon[m_MaterialIndex]->SelectIconHero();
		MaterialHeroIcon[m_MaterialIndex]->StopAni(TEXT("Selecting"));

		UUIFunctionLibrary::GetGenericScrollview()->Appear(false);

		/**/

		UUC_GenericScrollItemData* itemdata = Cast<UUC_GenericScrollItemData>(scroll_item->ItemData);
		if (itemdata)
		{
			if (SeletedHeroUDs.IsValidIndex(m_MaterialIndex))
			{
				SeletedHeroUDs[m_MaterialIndex] = itemdata->ItemUD;

				FHERO* hero = RGameInstance->RInventory->GetHeroWithUD(itemdata->ItemUD);
				if (hero)
				{
					MaterialHeroIcon[m_MaterialIndex]->SetHeroIconInfoWithID(hero->heroId, 0, false);
				}
			}
		}
	}


	bool bReadyForUpgrade = true;
	for (size_t i = 0; i < SeletedHeroUDs.Num(); ++i)
	{
		if (SeletedHeroUDs[i].IsEmpty())
			bReadyForUpgrade = false;
	}

	if (bReadyForUpgrade)
		Button_Upgrade->SetIsEnabled(true);

	//URScrollView* ScrollView = scroll_item->ParentScrollView;
	//TArray<int32> SelectedIndex = ScrollView->GetSelectedScrollItem();

	//if (SelectedIndex.Num() > m_UpgradeCost->CharacterNum[m_MaterialIndex])
	//{
	//	ScrollView->SetSelectScrollListData(SelectItemData);
	//	return;
	//}

	//FDECKS	Deck = RGameInstance->RInventory->GetHeroDeck();
	//if (Deck.defense.Find(SelectItemData->ItemUD) != INDEX_NONE)
	//{
	//	UUIFunctionLibrary::ShowCommonPopupSplash(UUtilFunctionLibrary::GetLocalizedString(EStringTableType::VE_UI, COMMONSTRING_NOTIFY), UUtilFunctionLibrary::GetLocalizedString(EStringTableType::VE_UI, FName(TEXT("UI_HeroGradeUp_SplashPopUp_Message06"))));
	//	ScrollView->SetSelectScrollListData(SelectItemData);
	//	return;
	//}

	//FHERO* Hero = RGameInstance->RInventory->GetHeroWithUD(SelectItemData->ItemUD);

	//if (Hero && Hero->locked)
	//{
	//	UUIFunctionLibrary::ShowCommonPopupSplash(UUtilFunctionLibrary::GetLocalizedString(EStringTableType::VE_UI, COMMONSTRING_NOTIFY), UUtilFunctionLibrary::GetLocalizedString(EStringTableType::VE_UI, FName(TEXT("UI_HeroGradeUp_SplashPopUp_Message06"))));
	//	ScrollView->SetSelectScrollListData(SelectItemData);
	//	return;
	//}

	///*Selection Effect*/
	//if (SelectItemData->bSelected)
	//{
	//	
	//	SelectItem->HeroIcon->WidgetSwitcher_Select->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
	//	SelectItem->HeroIcon->WidgetSwitcher_Select->SetActiveWidgetIndex(0);
	//}
	//else
	//{
	//	SelectItem->HeroIcon->WidgetSwitcher_Select->SetVisibility(ESlateVisibility::Collapsed);
	//}
	///**/

	//SelectedIndex = ScrollView->GetSelectedScrollItem();
	//Slot[m_MaterialIndex].Reset();

	//for (int32 i = 0; i < SelectedIndex.Num(); i++)
	//{
	//	UUC_GenericScrollItem* AddItem = Cast<UUC_GenericScrollItem>(ScrollView->GetScrollItem(SelectedIndex[i]));
	//	UUC_GenericScrollItemData* AddItemData = Cast<UUC_GenericScrollItemData>(AddItem->ItemData);

	//	Slot[m_MaterialIndex].Add(AddItemData->ItemUD);
	//}

	//MaterialHeroIcon[m_MaterialIndex]->SetCount(Slot[m_MaterialIndex].Num(), m_UpgradeCost->CharacterNum[m_MaterialIndex]);
}

void UUC_HeroInfo_Upgrade::OnClickUpgrade()
{
	URGameInstance*		RGameInstance	=	RGAMEINSTANCE(this);
	bool				IsAbleRegister	=	true;
	bool				bLocked			=	false;

	if (m_Hero->level < m_PrevTable->MaxLevel)
	{
		UUIFunctionLibrary::ShowCommonPopupSplash(UUtilFunctionLibrary::GetLocalizedString(EStringTableType::VE_UI, COMMONSTRING_NOTIFY), UUtilFunctionLibrary::GetLocalizedString(EStringTableType::VE_UI, FName(TEXT("UI_HeroGradeUp_SplashPopUp_Message02"))));
		return;
	}
	if (m_PrevTable->Grade >= 10)
	{
		UUIFunctionLibrary::ShowCommonPopupSplash(UUtilFunctionLibrary::GetLocalizedString(EStringTableType::VE_UI, COMMONSTRING_NOTIFY), UUtilFunctionLibrary::GetLocalizedString(EStringTableType::VE_UI, FName(TEXT("UI_HeroGradeUp_SplashPopUp_Message01"))));
		return;
	}
	const int32			MyGold				=	RGameInstance->RInventory->GetGold();
	const int32			MyUpgradeMaterial	=	RGameInstance->RInventory->GetHeroUpgradeMaterial();
	FWalletTableInfo*	WalletInfo			=	UTableManager::GetInstancePtr()->GetWalletRow(WALLET_HERO_UPGRADEMATERIAL);
	checkf(WalletInfo != nullptr, TEXT("UP_RESULT WALLET_HERO_UPGRADEMATERIAL Is Empty"));

	if (m_UpgradeCost->Gold > MyGold)
		IsAbleRegister = false;

	if (m_UpgradeCost->HeroUpgradeMaterial > MyUpgradeMaterial)
		IsAbleRegister = false;

	if (IsAbleRegister == false)
	{
		UUIFunctionLibrary::ShowCommonPopupSplash(UUtilFunctionLibrary::GetLocalizedString(EStringTableType::VE_UI, COMMONSTRING_NOTIFY), UUtilFunctionLibrary::GetLocalizedString(EStringTableType::VE_UI, FName(TEXT("UI_HeroGradeUp_SplashPopUp_Message03"))));
		return;
	}

	UPacketFunctionLibrary::INVENTORY_HERO_UPGRADE_RQ(m_Hero->heroUD, SeletedHeroUDs, TArray<FString>(), TArray<FString>(), TArray<FString>());
}

void UUC_HeroInfo_Upgrade::OnClickIcon1()
{
	UUP_GenericScrollview* GenericScrollview = UUIFunctionLibrary::GetGenericScrollview();
	if (GenericScrollview)
	{
		if (m_MaterialIndex == 0 && GenericScrollview->isInsideViewport())
		{
			GenericScrollview->Appear(false);
			MaterialHeroIcon[m_MaterialIndex]->StopAni(TEXT("Selecting"));
			return;
		}
			
	}
	

	m_MaterialIndex = 0;
	SelectMaterialIcon();
}
void UUC_HeroInfo_Upgrade::OnClickIcon2()
{
	UUP_GenericScrollview* GenericScrollview = UUIFunctionLibrary::GetGenericScrollview();
	if (GenericScrollview)
	{
		if (m_MaterialIndex == 1 && GenericScrollview->isInsideViewport())
		{
			GenericScrollview->Appear(false);
			MaterialHeroIcon[m_MaterialIndex]->StopAni(TEXT("Selecting"));
			return;
		}
	}

	m_MaterialIndex = 1;
	SelectMaterialIcon();
}
void UUC_HeroInfo_Upgrade::OnClickIcon3()
{
	UUP_GenericScrollview* GenericScrollview = UUIFunctionLibrary::GetGenericScrollview();
	if (GenericScrollview)
	{
		if (m_MaterialIndex == 2 && GenericScrollview->isInsideViewport())
		{
			GenericScrollview->Appear(false);
			MaterialHeroIcon[m_MaterialIndex]->StopAni(TEXT("Selecting"));
			return;
		}
	}

	m_MaterialIndex = 2;
	SelectMaterialIcon();
}

void UUC_HeroInfo_Upgrade::OnClickClose()
{
	UUP_GenericScrollview* GenericScrollview = UUIFunctionLibrary::GetGenericScrollview();
	if (GenericScrollview)
	{
		if (GenericScrollview->isInsideViewport())
			GenericScrollview->Appear(false);
		else
			GenericScrollview->SetState(EScrollviewType::VE_END);
	}
	RGAMEINSTANCE(this)->RWidgetManager->DetachUIPage(EUIPageEnum::UIPage_HeroRatingUp);
	
}

void UUC_HeroInfo_Upgrade::SelectMaterialIcon()
{
	for (int32 i = 0; i < MaterialHeroIcon.Num(); i++)
	{
		if (m_MaterialIndex != i)
		{
			MaterialHeroIcon[i]->Image_Seleting->SetVisibility(ESlateVisibility::Collapsed);
			MaterialHeroIcon[i]->StopAni(TEXT("Selecting"));
		}
		else if (m_MaterialIndex == i)
		{
			MaterialHeroIcon[i]->Image_Seleting->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
			MaterialHeroIcon[i]->Image_Selected->SetVisibility(ESlateVisibility::Collapsed);
		}
	}

	UUP_GenericScrollview* GenericScrollview = UUIFunctionLibrary::GetGenericScrollview();
	if (GenericScrollview)
	{
		GenericScrollview->SetState(EScrollviewType::VE_HERO_UPGRADE);
		
		if (!GenericScrollview->isInsideViewport())
			GenericScrollview->Appear(true);
			
		SeletedHeroUDs[m_MaterialIndex] = TEXT("");

		TArray<FString> outArray;
		Filtering(outArray);
		
		GenericScrollview->Recompose_Itemdatalist(outArray);
		
		MaterialHeroIcon[m_MaterialIndex]->PlayAni(TEXT("Selecting"), false, 0);
		SelectedMaterialTextArray[m_MaterialIndex+1]->SetText(FText::AsNumber(0));
		
		if (IsValid(Button_Upgrade))
			Button_Upgrade->SetIsEnabled(false);
	}

}