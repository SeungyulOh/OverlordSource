// Fill out your copyright notice in the Description page of Project Settings.

#include "ProjectR.h"
#include "UP_HeroInventory.h"
#include "GlobalIntegrated.h"
//
#include "Table/CharacterTableInfo.h"
#include "SharedConstants/WalletConstants.h"
#include "GameInfo/RInventory.h"
#include "Network/PacketDataStructures.h"
#include "Network/PacketFunctionLibrary.h"
#include "Network/HttpRequestClient.h"
#include "UtilFunctionIntegrated.h"
#include "UI/Common/HeroBaseScrollItem.h"
#include "UI/Common/HeroScrollItemData.h"
#include "UI/Component/CheckBox_Radio.h"
#include "UI/Component/CanvasPanel_RadioGroup.h"
#include "UI/Control/RScrollItem.h"
#include "UI/Control/RScrollView.h"
#include "UI/RWidgetManager.h"
#include "UI/HeroManagement/Popup/UC_Popup_SpiritInfo.h"
#include "UI/UI_Common/UP_CommonPopup.h"
#include "GlobalIntegrated.h"
#include "Global/TableManager.h"

#define DEF_HERO_INVENTORY_BREAKUP	0
#define DEF_HERO_INVENTORY_SELECT	1
#define DEF_HERO_INVENTORY_TOOLS	0
#define DEF_HERO_INVENTORY_LOCK		1
#define DEF_HERO_INVENTORY_GRIND	2
#define DEF_HERO_INVENTORY_MAX		100

void UUC_HeroInventory_ScrollItem::NativeConstruct()
{
	Super::NativeConstruct();
	WidgetSwitcher_Select->SetActiveWidgetIndex(0);
	FindChildWidgetsWithCertainName<UImage>(this, HeroStarArray, TEXT("Hero_Star_"));
}

void UUC_HeroInventory_ScrollItem::InvalidateData()
{
	//Super::InvalidateData();

	UHeroInventory_ScrollData* HeroInventoryItemData = Cast<UHeroInventory_ScrollData>(ItemData);
	if (!IsValid(HeroInventoryItemData))
	{
		UE_LOG(LogUI, Log, TEXT("UUC_HeroInventory_ScrollItem::InvalidateData: failed to get ItemData"));
		return;
	}
	FHERO* HeroData = nullptr;
	HeroData = RGAMEINSTANCE(this)->RInventory->GetHeroWithUD(HeroInventoryItemData->HeroUD);
	checkf(HeroData != nullptr, TEXT("Hero Data Is Null"));

	CurHeroUD = HeroData->heroUD;
	CurHeroID = HeroData->heroId;
	CurHeroLevel = HeroData->level;
	eHeroInventoryFunction = HeroInventoryItemData->eHeroInventoryFunction;
	FCharacterTableInfo* CharacterTableInfo = UTableManager::GetInstancePtr()->GetCharacterRow(CurHeroID);
	
	if (CharacterTableInfo)
	{
		HeroLevelStarInfo->SetVisibility(ESlateVisibility::HitTestInvisible);
		Text_Strong->SetVisibility(ESlateVisibility::HitTestInvisible);

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
			if (!HeroInventoryItemData->bIsSpirit)
			{
				Text_Hero_Level->SetVisibility(ESlateVisibility::Visible);
				TextBlock_HeroLevel->SetText(FText::AsNumber(CurHeroLevel));
			}
			else
				Text_Hero_Level->SetVisibility(ESlateVisibility::Collapsed);
		}

		if (IsValid(TextBlock_HeroName))
		{
			TextBlock_HeroName->SetText(UUtilFunctionLibrary::GetLocalizedString(EStringTableType::VE_Hero, CharacterTableInfo->DisplayName));
		}
		if (HeroData != nullptr)
			SetLock(HeroData->locked);
		SetSelected(bSelected);
		if (CharacterTableInfo->IsHero)
		{
// 			int32 power = UClient_BattleFunctionLibrary::GetHeroCombatStat(CurHeroUD);
			if(HeroData)
			{
				int32		sum = 0;

				for (int32 p : HeroData->enchantStat)
					sum += p;

				if(sum == 0)
				{
					Text_Strong->SetVisibility(ESlateVisibility::Collapsed);
				}
				else
				{
					Text_Strong->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
					Text_Strong->SetText(FText::AsNumber(sum));
				}
			}
		}
		else
		{
			Text_Strong->SetVisibility(ESlateVisibility::Collapsed);
		}

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
			

		if (ImageBack && grade < 10)
			ImageBack->Brush.TintColor = FSlateColor(color);

		for (int32 i = 0; i < grade; i++)
			if (HeroStarArray.IsValidIndex(i))
				HeroStarArray[i]->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
		
		Grade_10->SetVisibility(ESlateVisibility::Collapsed);

		if (grade <= 5)
			WidgetSwitcher_Star->SetActiveWidgetIndex(0);
		else if (grade < 10)
			WidgetSwitcher_Star->SetActiveWidgetIndex(1);
		else
		{
			WidgetSwitcher_Star->SetActiveWidgetIndex(2);
			Grade_10->SetVisibility(ESlateVisibility::HitTestInvisible);
			ImageBack->SetVisibility(ESlateVisibility::Collapsed);
			return;
		}
	}
}

void UUC_HeroInventory_ScrollItem::SetHero(FString heroUD)
{
	CurHeroUD = TEXT("");
	WidgetSwitcher_Select->SetActiveWidgetIndex(0);
}

void UUC_HeroInventory_ScrollItem::SetLock(bool IsLocked)
{
	if (IsValid(Hero_Lock))
	{
		Hero_Lock->SetVisibility(IsLocked ? ESlateVisibility::Visible : ESlateVisibility::Collapsed);
	}
}

const FString& UUC_HeroInventory_ScrollItem::GetCurHeroUD() const
{
	return CurHeroUD;
}

void UUC_HeroInventory_ScrollItem::SetSelected(bool b)
{
	Super::SetSelected(b);
	bSelected = b;

	if (IsValid(Hero_Check))
	{
		Hero_Check->SetVisibility(this->bSelected ? ESlateVisibility::HitTestInvisible : ESlateVisibility::Collapsed);
	}

	if (eHeroInventoryFunction == EHeroInventoryFunction::VE_Lock || 
		eHeroInventoryFunction == EHeroInventoryFunction::VE_Grind)
	{
		FHERO* HeroData = RGAMEINSTANCE(this)->RInventory->GetHeroWithUD(CurHeroUD);
		if(HeroData)
		{
			if (IsValid(Hero_Check))
			{
				Hero_Check->SetVisibility(ESlateVisibility::HitTestInvisible);
			}

			bool bImageShow = HeroData->locked;
			if (IsValid(Image_Check))
			{
				Image_Check->SetVisibility(this->bSelected || bImageShow ? ESlateVisibility::HitTestInvisible : ESlateVisibility::Collapsed);
			}
			if (IsValid(Icon_Check))
			{
				Icon_Check->SetVisibility(this->bSelected ? ESlateVisibility::HitTestInvisible : ESlateVisibility::Collapsed);
			}
		}
	}
}

bool UUC_HeroInventory_ScrollItem::Initialize_FloorBossCard(int32 CurrentFloor)
{
	if(CanvasPanelRoot)
		CanvasPanelRoot->SetVisibility(ESlateVisibility::SelfHitTestInvisible);

	if (Text_Strong)
		Text_Strong->SetVisibility(ESlateVisibility::Collapsed);
	if (Vertical_Job)
		Vertical_Job->SetVisibility(ESlateVisibility::Collapsed);
	if (HeroLevelStarInfo)
		HeroLevelStarInfo->SetVisibility(ESlateVisibility::Collapsed);
	if (Hero_Lock)
		Hero_Lock->SetVisibility(ESlateVisibility::Collapsed);
	if (WidgetSwitcher_Select)
		WidgetSwitcher_Select->SetVisibility(ESlateVisibility::Collapsed);
	if (Grade_BG)
		Grade_BG->SetVisibility(ESlateVisibility::Collapsed);
	if (Grade_10)
		Grade_10->SetVisibility(ESlateVisibility::Collapsed);
	if (ImageBack)
		ImageBack->SetVisibility(ESlateVisibility::Collapsed);

	
	if (Image_TowerBg)
		Image_TowerBg->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
	if (CanvasPanel_BossName)
		CanvasPanel_BossName->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
	if (FloorText)
		FloorText->SetText(FText::AsNumber(CurrentFloor));

	FFloorInfo* FloorInfo = UTableManager::GetInstancePtr()->GetFloorInfoRow(FName(*FString::FromInt(CurrentFloor)));
	if (FloorInfo)
	{
		FCharacterTableInfo* ChracterTable = UTableManager::GetInstancePtr()->GetCharacterRow(FloorInfo->FloorBoss);
		if (ChracterTable)
		{
			int32 iCurrentStageIdx = (CurrentFloor) * 14 - 1;
			TArray<FName> FloorStageIDs = UTableManager::GetInstancePtr()->GetFloorStageRowNames();
			FFloorStageDetail* FloorStageTable = UTableManager::GetInstancePtr()->GetFloorStageDetailRow(FloorStageIDs[iCurrentStageIdx]);
			
			FTOWER_STATUS TowerStatus = RGAMEINSTANCE(this)->CampaignManager->TowerStatus;
			TArray<int32> StageClearArray = TowerStatus.clearGrades;

			FString OpenKeyStr = FloorStageTable->OpenKey.ToString();

			if (IsValid(Image_HeroCard))
			{
				UTexture2D* TargetTexture = UUtilFunctionLibrary::GetTexture2DFromTable(ChracterTable->RuneCardImage);

				if (StageClearArray.IsValidIndex(FCString::Atoi(*OpenKeyStr)) && StageClearArray[FCString::Atoi(*OpenKeyStr)] != 0)
				{
					//Raid Available
					if (Image_HeroCard && TargetTexture)
						Image_HeroCard->SetBrushFromTexture(TargetTexture);

					if (IsValid(TextBlock_BossName))
					{
						TextBlock_BossName->SetText(UUtilFunctionLibrary::GetLocalizedString(EStringTableType::VE_Hero, ChracterTable->DisplayName));
						TextBlock_BossName->SetColorAndOpacity(FSlateColor(FLinearColor(0.06, 0.35, 0.98, 1)));
					}

					return true;
				}
				else
				{
					UMaterialInstance* MI = UUtilFunctionLibrary::GetMaterialInstanceFromTable(FName(TEXT("HeroPortraitInst")));
					UMaterialInstanceDynamic* MID = UMaterialInstanceDynamic::Create(MI, nullptr);
					if (MID && TargetTexture)
					{
						MID->SetTextureParameterValue(FName(TEXT("TextureIn")), TargetTexture);
						Image_HeroCard->SetBrushFromMaterial(MID);
					}

					if (IsValid(TextBlock_BossName))
					{
						TextBlock_BossName->SetText(UUtilFunctionLibrary::GetLocalizedString(EStringTableType::VE_Hero, ChracterTable->DisplayName));
						TextBlock_BossName->SetColorAndOpacity(FSlateColor(FLinearColor(0.5, 0.5, 0.5, 1)));
					}

					return false;
				}
			}
		}
	}
	return false;
}

void UUP_HeroInventory::NativeConstruct()
{
	Super::NativeConstruct();
	URGameInstance* RGameInstance = RGAMEINSTANCE(this);

	if(!HeroInventoryScrollView->ScrollItemList.Num())
		HeroInventoryScrollView->Init_NoClear(60);

	RGameInstance->HttpClient->OnInventoryUpdate.RemoveAll(this);
	RGameInstance->HttpClient->OnInventoryUpdate.AddUObject(this, &UUP_HeroInventory::OnUpdate_HeroInventory);
	RGameInstance->EventManager->AfterHeroGrind.RemoveDynamic(this, &UUP_HeroInventory::OnRecevie_Hero_Grind_Result);
	RGameInstance->EventManager->AfterHeroGrind.AddDynamic(this, &UUP_HeroInventory::OnRecevie_Hero_Grind_Result);
	RGameInstance->EventManager->OnHeroLock.RemoveDynamic(this, &UUP_HeroInventory::OnClick_HeroInventory_Hero_SelectCancel);
	RGameInstance->EventManager->OnHeroLock.AddDynamic(this, &UUP_HeroInventory::OnClick_HeroInventory_Hero_SelectCancel);
	
	UC_Hero_FilterList_new->OnSelectedHeroLightFilter.RemoveDynamic(this, &UUP_HeroInventory::OnRecevie_Hero_LightFilter_Result);
	UC_Hero_FilterList_new->OnSelectedHeroLightFilter.AddDynamic(this, &UUP_HeroInventory::OnRecevie_Hero_LightFilter_Result);

	UC_SoulInfo_FilterList->OnSelectedHeroLightFilter.RemoveDynamic(this, &UUP_HeroInventory::OnRecevie_Hero_LightFilter_Result);
	UC_SoulInfo_FilterList->OnSelectedHeroLightFilter.AddDynamic(this, &UUP_HeroInventory::OnRecevie_Hero_LightFilter_Result);
	
	UC_hero_filter_check->OnSelectedHeroDetailFilter.RemoveDynamic(this, &UUP_HeroInventory::OnRecevie_Hero_DetailFilter_Result);
	UC_hero_filter_check->OnSelectedHeroDetailFilter.AddDynamic(this, &UUP_HeroInventory::OnRecevie_Hero_DetailFilter_Result);

	HeroInventoryScrollView->OnScrollViewSelectedItem.RemoveDynamic(this, &UUP_HeroInventory::OnSelectedItem_HeroInventory_ScrollView);
	HeroInventoryScrollView->OnScrollViewSelectedItem.AddDynamic(this, &UUP_HeroInventory::OnSelectedItem_HeroInventory_ScrollView);

	Button_LightFilter_Show->OnClicked.RemoveDynamic(this, &UUP_HeroInventory::OnClick_HeroInventory_Hero_LightFilter);
	Button_LightFilter_Show->OnClicked.AddDynamic(this, &UUP_HeroInventory::OnClick_HeroInventory_Hero_LightFilter);

	Button_LightFilter_Hide->OnClicked.RemoveDynamic(this, &UUP_HeroInventory::OnClick_HeroInventory_Hero_LightFilter);
	Button_LightFilter_Hide->OnClicked.AddDynamic(this, &UUP_HeroInventory::OnClick_HeroInventory_Hero_LightFilter);

	Button_DetailFilter_Show->OnClicked.RemoveDynamic(this, &UUP_HeroInventory::OnClick_HeroInventory_Hero_DetailFilter);
	Button_DetailFilter_Show->OnClicked.AddDynamic(this, &UUP_HeroInventory::OnClick_HeroInventory_Hero_DetailFilter);

	Radio_Hero->OnCheckRadioCheck.RemoveDynamic(this, &UUP_HeroInventory::OnClick_HeroInventory_MainTab_Hero);
	Radio_Hero->OnCheckRadioCheck.AddDynamic(this, &UUP_HeroInventory::OnClick_HeroInventory_MainTab_Hero);

	Radio_Soul->OnCheckRadioCheck.RemoveDynamic(this, &UUP_HeroInventory::OnClick_HeroInventory_MainTab_Soul);
	Radio_Soul->OnCheckRadioCheck.AddDynamic(this, &UUP_HeroInventory::OnClick_HeroInventory_MainTab_Soul);

	Button_BatchSelect->OnClicked.RemoveDynamic(this, &UUP_HeroInventory::OnClick_HeroInventory_Hero_BatchSelect);
	Button_BatchSelect->OnClicked.AddDynamic(this, &UUP_HeroInventory::OnClick_HeroInventory_Hero_BatchSelect);

	Button_Select->OnClicked.RemoveDynamic(this, &UUP_HeroInventory::OnClick_HeroInventory_Hero_Select);
	Button_Select->OnClicked.AddDynamic(this, &UUP_HeroInventory::OnClick_HeroInventory_Hero_Select);

	Button_SelectCancel->OnClicked.RemoveDynamic(this, &UUP_HeroInventory::OnClick_HeroInventory_Hero_SelectCancel);
	Button_SelectCancel->OnClicked.AddDynamic(this, &UUP_HeroInventory::OnClick_HeroInventory_Hero_SelectCancel);

	Button_SelectGrind->OnClicked.RemoveDynamic(this, &UUP_HeroInventory::OnClick_HeroInventory_Hero_SelectGrind);
	Button_SelectGrind->OnClicked.AddDynamic(this, &UUP_HeroInventory::OnClick_HeroInventory_Hero_SelectGrind);

	Button_SelectLocked->OnClicked.RemoveDynamic(this, &UUP_HeroInventory::OnClick_HeroInventory_Hero_SelectLocked);
	Button_SelectLocked->OnClicked.AddDynamic(this, &UUP_HeroInventory::OnClick_HeroInventory_Hero_SelectLocked);

	Button_HeroInvenAmount_1->OnClicked.RemoveDynamic(this, &UUP_HeroInventory::OnClick_HeroInventory_Amount);
	Button_HeroInvenAmount_1->OnClicked.AddDynamic(this, &UUP_HeroInventory::OnClick_HeroInventory_Amount);

	Button_HeroInvenAmount_2->OnClicked.RemoveDynamic(this, &UUP_HeroInventory::OnClick_HeroInventory_Amount);
	Button_HeroInvenAmount_2->OnClicked.AddDynamic(this, &UUP_HeroInventory::OnClick_HeroInventory_Amount);

	if (!CachedItemDataList.Num())
	{
		for (size_t i = 0; i < DEF_HERO_INVENTORY_MAX; ++i)
		{
			URScrollItemData* ItemData = HeroInventoryScrollView->CreateScrollItemData(UHeroInventory_ScrollData::StaticClass());
			ItemData->WidgetSize = HeroInventoryScrollView->GetItemInitSize();
			CachedItemDataList.Emplace(ItemData);
		}
	}

	ResetVars(multiSelectMode);
	UUIFunctionLibrary::SetFilterCheckCondition(UC_hero_filter_check);
	UC_hero_filter_check->FilteringAllConditions(true);

	
}

void UUP_HeroInventory::NativeDestruct()
{	
	Super::NativeDestruct();
}

void UUP_HeroInventory::InvalidateData()
{
	Super::InvalidateData();
	
	SetMainTab();
	SetControlBar();
	ShowDetailFilter();
	SetCountText();
	FDECKS Decks = RGAMEINSTANCE(this)->RInventory->GetHeroDeck();
	if (EHeroInventoryObject::VE_Hero == eHeroInventoryObject)
	{
		GetHeroUDsWithFiltering(FilteringHeroList, EHeroInventoryType::VE_TypeHero);

		HeroInventoryScrollView->ClearScrollItemData(DEF_HERO_INVENTORY_MAX);

		for (size_t i = 0 ; i<FilteringHeroList.Num() ; ++i)
		{
			if (i >= DEF_HERO_INVENTORY_MAX)
				break;

			UHeroInventory_ScrollData* ScrollItemData = Cast<UHeroInventory_ScrollData>(CachedItemDataList[i]);
			//checkf(ScrollItemData == nullptr, TEXT("UUP_HeroInventory::InvalidateData: failed to AddScrollItemData"));
			
			ScrollItemData->bIsInArenaDefence = Decks.defense.Contains(FilteringHeroList[i]);

			ScrollItemData->HeroUD = FilteringHeroList[i];
			ScrollItemData->bIsSpirit = false;
			ScrollItemData->eHeroInventoryFunction = eHeroInventoryFunction;
			ScrollItemData->bSelected = SelectedHeros.Contains(FilteringHeroList[i]);

			HeroInventoryScrollView->ItemDataList.Emplace(ScrollItemData);
		}
	}
	else if (EHeroInventoryObject::VE_Spirit == eHeroInventoryObject)
	{
		GetHeroUDsWithFiltering(FilteringHeroList, EHeroInventoryType::VE_TypeSpirit);

		HeroInventoryScrollView->ClearScrollItemData(DEF_HERO_INVENTORY_MAX);

		for (size_t i = 0; i < FilteringHeroList.Num(); ++i)
		{
			UHeroInventory_ScrollData* ScrollItemData = Cast<UHeroInventory_ScrollData>(CachedItemDataList[i]);
			if (!IsValid(ScrollItemData))
			{
				UE_LOG(LogUI, Log, TEXT("UUP_HeroInventory::InvalidateData: failed to AddScrollItemData"));
				return;
			}
			
			ScrollItemData->bIsInArenaDefence = Decks.defense.Contains(FilteringHeroList[i]);

			ScrollItemData->HeroUD = FilteringHeroList[i];
			ScrollItemData->bIsSpirit = true;
			ScrollItemData->eHeroInventoryFunction = eHeroInventoryFunction;
			ScrollItemData->bSelected = SelectedSouls.Contains(FilteringHeroList[i]);

			HeroInventoryScrollView->ItemDataList.Emplace(ScrollItemData);
		}
	}
	HeroInventoryScrollView->InvalidateData();
}

// Main Tab[ Hero, Soul ] Setting
void UUP_HeroInventory::SetMainTab()
{
	if (EHeroInventoryObject::VE_Hero == eHeroInventoryObject)
	{
		Switcher_Hero->SetActiveWidgetIndex(0);
		Switcher_Soul->SetActiveWidgetIndex(1);
	}

	if (EHeroInventoryObject::VE_Spirit == eHeroInventoryObject)
	{
		Switcher_Hero->SetActiveWidgetIndex(1);
		Switcher_Soul->SetActiveWidgetIndex(0);
	}
}
// Bottom Control Bar[ Hero, Soul ] Setting
void UUP_HeroInventory::SetControlBar()
{
	Switcher_ControlPanel->SetActiveWidgetIndex(0);
	Switcher_ControlPanel->SetVisibility(ESlateVisibility::Visible);

	if (EHeroInventoryFunction::VE_Select == eHeroInventoryFunction)
		Switcher_ControlPanel->SetActiveWidgetIndex(DEF_HERO_INVENTORY_SELECT);
	else if (EHeroInventoryFunction::VE_Lock == eHeroInventoryFunction)
		Switcher_ControlPanel->SetActiveWidgetIndex(DEF_HERO_INVENTORY_LOCK);
	else
		Switcher_ControlPanel->SetActiveWidgetIndex(DEF_HERO_INVENTORY_BREAKUP);
}

void UUP_HeroInventory::ResetVars(bool bScrollViewMultiSelect)
{
	HeroInventoryScrollView->InvalidateDataWithoutResetSelect();
	HeroInventoryScrollView->MultiSelect = bScrollViewMultiSelect;
	bIsVisibleDetailFilter = false;
	UC_Hero_FilterList_new->SetVisibility(ESlateVisibility::Collapsed);
	UC_SoulInfo_FilterList->SetVisibility(ESlateVisibility::Collapsed);
	Switcher_LightFilter->SetActiveWidgetIndex(0);
	
	RGAMEINSTANCE(this)->RInventory->GetHeroUDs(FilteringHeroList, EClassTypeEnum::VE_ClassTypeMax, EHeroInventoryType::VE_InventoryTypeAll);
	SelectedHeros.Reset();
	SelectedSouls.Reset();

	RGAMEINSTANCE(this)->RInventory->classFilter.Reset();
	RGAMEINSTANCE(this)->RInventory->gradeFilter.Reset();
	RGAMEINSTANCE(this)->RInventory->continentFilter.Reset();
	UC_hero_filter_check->CheckBoxReset();
}

void UUP_HeroInventory::ShowDetailFilter()
{
	if (true == bIsVisibleDetailFilter)
	{
		UC_hero_filter_check->SetVisibility(ESlateVisibility::Visible);
		UC_Hero_FilterList_new->SetVisibility(ESlateVisibility::Collapsed);
		UC_SoulInfo_FilterList->SetVisibility(ESlateVisibility::Collapsed);
		Switcher_LightFilter->SetActiveWidgetIndex(0);

		if (eHeroInventoryObject == EHeroInventoryObject::VE_Hero)
			UC_hero_filter_check->SetHeroFilterMenu();
		else if(eHeroInventoryObject == EHeroInventoryObject::VE_Spirit)
			UC_hero_filter_check->SetSoulFilterMenu();

		Button_BatchSelect->SetIsEnabled(false);
	}
	else
	{
		UC_hero_filter_check->SetVisibility(ESlateVisibility::Collapsed);
		if (UC_Hero_FilterList_new->GetVisibility() == ESlateVisibility::Collapsed && UC_SoulInfo_FilterList->GetVisibility() == ESlateVisibility::Collapsed)
			Button_BatchSelect->SetIsEnabled(true);
	}

	Image_BlockInput->SetVisibility(bIsVisibleDetailFilter ? ESlateVisibility::Visible : ESlateVisibility::SelfHitTestInvisible);
}

void UUP_HeroInventory::SetCountText()
{
	SetAmountData();

	int32 heroCnt = 0;
	int32 maxCnt = IStart_HeroSlot_Size + ICurrent_Hero_TokenCnt * IHeroSlot_Rate;

	if (eHeroInventoryObject == EHeroInventoryObject::VE_Hero)
		heroCnt = RGAMEINSTANCE(this)->RInventory->GetHeroInventoryCount(true);
	else if (eHeroInventoryObject == EHeroInventoryObject::VE_Spirit)
		heroCnt = RGAMEINSTANCE(this)->RInventory->GetHeroInventoryCount(false);

	TB_CurrentHeroCnt_1->SetText(FText::AsNumber(heroCnt));
	TB_CurrentHeroCnt_2->SetText(FText::AsNumber(heroCnt));

	TB_MaxHeroCnt_1->SetText(FText::AsNumber(maxCnt));
	TB_MaxHeroCnt_2->SetText(FText::AsNumber(maxCnt));
}

void UUP_HeroInventory::SetAmountData()
{
	UTableManager* TM = UTableManager::GetInstancePtr();
	FConstantVariablesTableInfo* tableInfo;
	tableInfo = TM->GetConstantVariablesRow(HERO_INVENTORY_START_SIZE);
	if(tableInfo)
		tableInfo->GetValue(IStart_HeroSlot_Size);
	tableInfo = nullptr;

	tableInfo = TM->GetConstantVariablesRow(HERO_INVENTORY_AddMax);
	if (tableInfo)
		tableInfo->GetValue(IHeroSlot_Max);
	tableInfo = nullptr;

	tableInfo = TM->GetConstantVariablesRow(HERO_INVENTORY_AddRate);
	if (tableInfo)
		tableInfo->GetValue(IHeroSlot_Rate);
	tableInfo = nullptr;

	ICurrent_Hero_TokenCnt = RGAMEINSTANCE(this)->RInventory->GetCurrencyWithKey(WALLET_HERO_INVENTORY_SLOT_TOKEN);
}

void UUP_HeroInventory::GetHeroUDsWithFiltering(TArray<FString>& HeroList, EHeroInventoryType eHeroType /*= EHeroInventoryType::VE_TypeHero*/)
{
	URGameInstance* RGameInstance = RGAMEINSTANCE(this);
	eHeroInventoryFilter = RGameInstance->RInventory->sortFilter;
	//HeroList = FilteringHeroList;
	bIsDescending = RGameInstance->RInventory->isDescending;
	RGameInstance->RInventory->HeroChangeFilter(HeroList, eHeroInventoryFilter, eHeroType, bIsDescending);
}

void UUP_HeroInventory::OnRecevie_Hero_LightFilter_Result(int32 eClassTypeEnum)
{
	if (eHeroInventoryFilter == (EHeroListFilterEnum)eClassTypeEnum)
		bIsDescending = !bIsDescending;
	else
		bIsDescending = true;

	RGAMEINSTANCE(this)->RInventory->isDescending = bIsDescending;
	eHeroInventoryFilter = (EHeroListFilterEnum)eClassTypeEnum;
	RGAMEINSTANCE(this)->RInventory->sortFilter = (EHeroListFilterEnum)eClassTypeEnum;
	InvalidateData();

	if (eHeroInventoryObject == EHeroInventoryObject::VE_Hero)
		UC_Hero_FilterList_new->SetVisibility(ESlateVisibility::Collapsed);
	else if (eHeroInventoryObject == EHeroInventoryObject::VE_Spirit)
		UC_SoulInfo_FilterList->SetVisibility(ESlateVisibility::Collapsed);

	Switcher_LightFilter->SetActiveWidgetIndex(0);
}

void UUP_HeroInventory::OnRecevie_Hero_DetailFilter_Result(TArray<FString> HeroList)
{
	FilteringHeroList = HeroList;
	//bIsVisibleDetailFilter = false;
	ShowDetailFilter();
	InvalidateData();
}

void UUP_HeroInventory::OnUpdate_HeroInventory(bool bResult)
{
	InvalidateData();
}

void UUP_HeroInventory::OnRecevie_Hero_Grind_Result(const TArray<FREWARD>& Inrewards)
{
	TArray<FREWARD> reward = Inrewards;
	UUIFunctionLibrary::ShowCommonRewardPopupByRewards(reward);
	multiSelectMode = true;
	ResetVars(multiSelectMode);
	InvalidateData();
}

void UUP_HeroInventory::OnReceive_Inventory_Upgrade_Result(bool bResult)
{
	UUIFunctionLibrary::ShowWaitingPopup(false);
	if (bResult)
	{
		if (UC_Popup_HeroInfo_Warning_Splash->IsValidLowLevel())
			if (!UC_Popup_HeroInfo_Warning_Splash->IsInViewport())
				UC_Popup_HeroInfo_Warning_Splash->RefreshUI(FText::FromString(FString("success")));

		SetCountText();
	}
	else
	{
		if (UC_Popup_HeroInfo_Warning_Splash->IsValidLowLevel())
			if (!UC_Popup_HeroInfo_Warning_Splash->IsInViewport())
				UC_Popup_HeroInfo_Warning_Splash->RefreshUI(FText::FromString(FString("fail")));
	}
}

void UUP_HeroInventory::OnClick_HeroInventory_MainTab_Hero(bool InChecked)
{
	if (EHeroInventoryObject::VE_Hero == eHeroInventoryObject)
		return;
	eHeroInventoryObject = EHeroInventoryObject::VE_Hero;
	eHeroInventoryFilter = EHeroListFilterEnum::VE_AttackPower;
	eHeroInventoryFunction = EHeroInventoryFunction::VE_View;
	UC_SoulInfo_FilterList->SetVisibility(ESlateVisibility::Collapsed);
	Switcher_LightFilter->SetActiveWidgetIndex(0);
	bIsVisibleDetailFilter = false;
	ShowDetailFilter();
	UC_hero_filter_check->OnClicked_Button_Reset();
}

void UUP_HeroInventory::OnClick_HeroInventory_MainTab_Soul(bool InChecked)
{
	if (EHeroInventoryObject::VE_Spirit == eHeroInventoryObject)
		return;
	eHeroInventoryObject = EHeroInventoryObject::VE_Spirit;
	eHeroInventoryFilter = EHeroListFilterEnum::VE_Grade;
	eHeroInventoryFunction = EHeroInventoryFunction::VE_View;
	UC_Hero_FilterList_new->SetVisibility(ESlateVisibility::Collapsed);
	Switcher_LightFilter->SetActiveWidgetIndex(0);
	bIsVisibleDetailFilter = false;
	ShowDetailFilter();
	UC_hero_filter_check->OnClicked_Button_Reset();
}

void UUP_HeroInventory::OnClick_HeroInventory_Hero_BatchSelect()
{
	eHeroInventoryFunction = EHeroInventoryFunction::VE_Select;
	HeroInventoryScrollView->MultiSelect = true;
	SetControlBar();
	URGameInstance* RGameInstance = RGAMEINSTANCE(this);
	TArray<FString> selectedUDs;
	FDECKS Decks = RGameInstance->RInventory->GetHeroDeck();
	for (auto& hero : FilteringHeroList)
	{
		if(Decks.defense.Contains(hero) == false)
			selectedUDs.Emplace(hero);
	}

	if (eHeroInventoryObject == EHeroInventoryObject::VE_Hero)
	{
		SelectedHeros.Reset();
		SelectedHeros = selectedUDs;
	}
	else
	{
		SelectedSouls.Reset();
		SelectedSouls = selectedUDs;
	}
	SetSelectingAllItem();
	TArray<int32>& SelectedList = HeroInventoryScrollView->GetSelectedScrollItem();
	TArray<URScrollItemData*>& scrollDataList = HeroInventoryScrollView->ItemDataList;
	for (int32 i = 0; i < HeroInventoryScrollView->ItemDataList.Num(); i++)
	{
		for (auto hero : selectedUDs)
		{
			if(Cast<UHeroInventory_ScrollData>(scrollDataList[i])->HeroUD == hero)
				SelectedList.Emplace(i);
		}
	}
	HeroInventoryScrollView->InvalidateDataWithoutResetSelect();
}

void UUP_HeroInventory::OnClick_HeroInventory_Hero_Select()
{
	if (EHeroInventoryFunction::VE_View != eHeroInventoryFunction)	return;
	eHeroInventoryFunction = EHeroInventoryFunction::VE_Select;
	multiSelectMode = true;
	ResetVars(multiSelectMode);
	InvalidateData();
// 	if (UC_Popup_HeroInfo_Warning_Splash.IsValid())
// 		if (!UC_Popup_HeroInfo_Warning_Splash->IsInViewport())
// 			UC_Popup_HeroInfo_Warning_Splash->RefreshUI(UUtilFunctionLibrary::GetLocalizedString(EStringTableType::VE_UI, FName(TEXT("GrindSelect_SplashPopUp_Message01"))));
}

void UUP_HeroInventory::OnClick_HeroInventory_Hero_SelectCancel()
{
	if (EHeroInventoryFunction::VE_Select != eHeroInventoryFunction)	return;
	eHeroInventoryFunction = EHeroInventoryFunction::VE_View;
	HeroInventoryScrollView->MultiSelect = false;
	multiSelectMode = false;
	SelectedHeros.Reset();
	SelectedSouls.Reset();
	//HeroInventoryScrollView->InvalidateData();
	InvalidateData();
}

void UUP_HeroInventory::OnClick_HeroInventory_Hero_SelectGrind()
{
	URGameInstance* RGameInstance = RGAMEINSTANCE(this);
	TArray<FString>					SelectedUDs;

	if (eHeroInventoryObject == EHeroInventoryObject::VE_Hero)
		SelectedUDs = SelectedHeros;
	else if (eHeroInventoryObject == EHeroInventoryObject::VE_Spirit)
		SelectedUDs = SelectedSouls;

	if (SelectedUDs.Num() == 0)
	{
		if (UC_Popup_HeroInfo_Warning_Splash->IsValidLowLevel())
			UC_Popup_HeroInfo_Warning_Splash->RefreshUI(UUtilFunctionLibrary::GetLocalizedString(EStringTableType::VE_UI, FName(TEXT("GrindSelect_SplashPopUp_Message02"))));
		return;
	}

	for (const FString& HeroUD : SelectedUDs)
	{
		FHERO* Hero = RGameInstance->RInventory->GetHeroWithUD(HeroUD);
		if (Hero->locked)
		{
			if (UC_Popup_HeroInfo_Warning_Splash->IsValidLowLevel())
				UC_Popup_HeroInfo_Warning_Splash->RefreshUI(UUtilFunctionLibrary::GetLocalizedString(EStringTableType::VE_UI, FName(TEXT("GrindSelect_SplashPopUp_Message03"))));
			return;
		}
	}

	UC_Popup_HeroBreakUp->SetBreakUpList(SelectedUDs);
	UC_Popup_HeroBreakUp->SetVisibility(ESlateVisibility::Visible);
}

void UUP_HeroInventory::OnClick_HeroInventory_Hero_SelectLocked()
{
	if (EHeroInventoryFunction::VE_Select == eHeroInventoryFunction)
	{
		URGameInstance* RGameInstance = RGAMEINSTANCE(this);
		TArray<FString>					LockHeros;
		TArray<FString>					UnlockHeros;

		if (EHeroInventoryObject::VE_Spirit == eHeroInventoryObject)
		{
			if (SelectedSouls.Num() == 0)
			{
				if (UC_Popup_HeroInfo_Warning_Splash->IsValidLowLevel())
					UC_Popup_HeroInfo_Warning_Splash->RefreshUI(UUtilFunctionLibrary::GetLocalizedString(EStringTableType::VE_UI, FName(TEXT("GrindSelect_SplashPopUp_Message02"))));
				return;
			}
			for (FString& ud : SelectedSouls)
			{
				FHERO* HeroData = RGameInstance->RInventory->GetHeroWithUD(ud);

				if (HeroData->locked)
					UnlockHeros.Emplace(ud);
				else
					LockHeros.Emplace(ud);
			}
		}
		else if (eHeroInventoryObject == EHeroInventoryObject::VE_Hero)
		{
			if (SelectedHeros.Num() == 0)
			{
				if (UC_Popup_HeroInfo_Warning_Splash->IsValidLowLevel())
					UC_Popup_HeroInfo_Warning_Splash->RefreshUI(UUtilFunctionLibrary::GetLocalizedString(EStringTableType::VE_UI, FName(TEXT("GrindSelect_SplashPopUp_Message02"))));
				return;
			}
			for (FString& ud : SelectedHeros)
			{
				FHERO* HeroData = RGameInstance->RInventory->GetHeroWithUD(ud);

				if (HeroData->locked)
					UnlockHeros.Emplace(ud);
				else
					LockHeros.Emplace(ud);
			}
		}
		UC_Popup_HeroInfo_Lock->SetDataList(LockHeros, UnlockHeros);
		UC_Popup_HeroInfo_Lock->SetVisibility(ESlateVisibility::Visible);
	}
}

void UUP_HeroInventory::OnClick_HeroInventory_Hero_Locked_Confirm()
{
	if (SelectedHeros.Num() == 0)
		return;

	if (EHeroInventoryObject::VE_Spirit == eHeroInventoryObject)
		return;

	if (EHeroInventoryFunction::VE_Lock == eHeroInventoryFunction)
	{
		TArray<FString>					LockHeros;
		TArray<FString>					UnlockHeros;
		URGameInstance* RGameInstance = RGAMEINSTANCE(this);

		for (FString& ud : SelectedHeros)
		{
			FHERO* HeroData = RGameInstance->RInventory->GetHeroWithUD(ud);
		
			if (HeroData->locked)
				UnlockHeros.Emplace(ud);
			else
				LockHeros.Emplace(ud);
		}
		UPacketFunctionLibrary::INVENTORY_ITEM_HERO_LOCK_RQ({}, {}, LockHeros, UnlockHeros);
		OnClick_HeroInventory_Hero_SelectCancel();
	}
}

void UUP_HeroInventory::OnClick_HeroInventory_Hero_LightFilter()
{
	if ( 0 == Switcher_LightFilter->GetActiveWidgetIndex())
	{
		if (eHeroInventoryObject == EHeroInventoryObject::VE_Hero)
		{
			UC_Hero_FilterList_new->SetVisibility(ESlateVisibility::Visible);
			UC_Hero_FilterList_new->SetSelectedSortingBtnColorNSwitcher(eHeroInventoryFilter, bIsDescending);
			Switcher_LightFilter->SetActiveWidgetIndex(1);
			bIsVisibleDetailFilter = false;
			ShowDetailFilter();
		}
		else if (eHeroInventoryObject == EHeroInventoryObject::VE_Spirit)
		{
			UC_SoulInfo_FilterList->SetVisibility(ESlateVisibility::Visible);
			UC_SoulInfo_FilterList->SetWidgetSwitcher(bIsDescending);				
			Switcher_LightFilter->SetActiveWidgetIndex(1);
			bIsVisibleDetailFilter = false;
			ShowDetailFilter();
		}
		Button_BatchSelect->SetIsEnabled(false);
	}
	else
	{
		if (eHeroInventoryObject == EHeroInventoryObject::VE_Hero)
			UC_Hero_FilterList_new->SetVisibility(ESlateVisibility::Collapsed);
		else if (eHeroInventoryObject == EHeroInventoryObject::VE_Spirit)
			UC_SoulInfo_FilterList->SetVisibility(ESlateVisibility::Collapsed);

		Switcher_LightFilter->SetActiveWidgetIndex(0);

		if (UC_hero_filter_check->GetVisibility() == ESlateVisibility::Collapsed)
			Button_BatchSelect->SetIsEnabled(true);
	}
}

void UUP_HeroInventory::OnClick_HeroInventory_Hero_DetailFilter()
{
	bIsVisibleDetailFilter = !bIsVisibleDetailFilter;
	UC_Hero_FilterList_new->SetVisibility(ESlateVisibility::Collapsed);
	ShowDetailFilter();
	InvalidateData();
}

void UUP_HeroInventory::OnClick_HeroInventory_Amount()
{
	if (MAX_INVENTORY_AMOUNT_COUNT <= ICurrent_Hero_TokenCnt)
	{
		UUIFunctionLibrary::ShowCommonPopup(UUtilFunctionLibrary::GetLocalizedString(EStringTableType::VE_Common, FName(TEXT("Notify"))),
											UUtilFunctionLibrary::GetLocalizedString(EStringTableType::VE_UI, FName("HeroUI_InventoryExpansion_PopUp_Message03")), 
											ECommonPopupType::VE_OK);
		return;
	}
	URGameInstance* RGameInstance = RGAMEINSTANCE(this);
	int32 nNeedGem = (ICurrent_Hero_TokenCnt + 1) * 10;
	int32 nHaveGem = RGameInstance->RInventory->GetGem();
	if (nHaveGem < nNeedGem)
	{
		UUP_CommonPopup* commonpopup = UUIFunctionLibrary::ShowCommonPopup(UUtilFunctionLibrary::GetLocalizedString(EStringTableType::VE_Common, FName(TEXT("Notify"))),
											UUtilFunctionLibrary::GetLocalizedString(EStringTableType::VE_UI, FName("UI_Currency_Warning_Gem")),
											ECommonPopupType::VE_OK);
		if (commonpopup)
		{
			commonpopup->SetCurrency(WALLET_GEM, nNeedGem);
		}
		return;
	}
	
	UC_Popup_InventoryPlus->SetVisibility(ESlateVisibility::Visible);
	SetAmountData();
	UC_Popup_InventoryPlus->BindingText(IStart_HeroSlot_Size, IHeroSlot_Rate, IHeroSlot_Max, ICurrent_Hero_TokenCnt);
	RGameInstance->HttpClient->OnHeroInventoryUpgrade.AddUObject(this, &UUP_HeroInventory::OnReceive_Inventory_Upgrade_Result);

	//turn off other filters
	if (eHeroInventoryObject == EHeroInventoryObject::VE_Hero)
	{
		if (UC_Hero_FilterList_new->GetVisibility() == ESlateVisibility::Visible)
		{
			UC_Hero_FilterList_new->SetVisibility(ESlateVisibility::Collapsed);
			Switcher_LightFilter->SetActiveWidgetIndex(0);
		}
	}
	else
	{
		if (UC_SoulInfo_FilterList->GetVisibility() == ESlateVisibility::Visible)
		{
			UC_SoulInfo_FilterList->SetVisibility(ESlateVisibility::Collapsed);
			Switcher_LightFilter->SetActiveWidgetIndex(0);
		}
	}

	bIsVisibleDetailFilter = false;
	ShowDetailFilter();
}

void UUP_HeroInventory::OnSelectedItem_HeroInventory_ScrollView(URScrollItem* SelectedItem)
{
	//close filter first
	UC_Hero_FilterList_new->SetVisibility(ESlateVisibility::Collapsed);
	UC_hero_filter_check->SetVisibility(ESlateVisibility::Collapsed);
	UC_SoulInfo_FilterList->SetVisibility(ESlateVisibility::Collapsed);

	if (EHeroInventoryObject::VE_Hero == eHeroInventoryObject)
	{
		if (EHeroInventoryFunction::VE_Select == eHeroInventoryFunction)
			OnSelectedItem_HeroInventory_ScrollView_HeroGrind(SelectedItem);
		else if (EHeroInventoryFunction::VE_Lock == eHeroInventoryFunction)
			OnSelectedItem_HeroInventory_ScrollView_HeroLock(SelectedItem);
		else
			OnSelectedItem_HeroInventory_ScrollView_HeroView(SelectedItem);
	}
	else if (EHeroInventoryObject::VE_Spirit == eHeroInventoryObject)
	{
		UHeroBaseScrollItem* castItem = Cast<UHeroBaseScrollItem>(SelectedItem);
		UHeroInventory_ScrollData* ScrollItemData = Cast<UHeroInventory_ScrollData>(SelectedItem->ItemData);
		FString heroUD = ScrollItemData->HeroUD;

		if (eHeroInventoryFunction == EHeroInventoryFunction::VE_Select)
		{
			if (SelectedSouls.Contains(heroUD))
			{
				SelectedSouls.Remove(heroUD);
				castItem->SetSelected(false);
			}
			else
			{
				SelectedSouls.Emplace(heroUD);
				castItem->SetSelected(true);
			}
		}
		else
		{
			UC_Popup_SoulInfo->SetVisibility(ESlateVisibility::Visible);
			UC_Popup_SoulInfo->BindData(heroUD);
		}
	}
}

void UUP_HeroInventory::OnSelectedItem_HeroInventory_ScrollView_HeroView(URScrollItem* SelectedItem)
{
	URGameInstance* RGameInstance = RGAMEINSTANCE(this);
	if (RGameInstance == nullptr)
		return;
	UHeroInventory_ScrollData* ScrollItemData = Cast<UHeroInventory_ScrollData>(SelectedItem->ItemData);
	if (ScrollItemData == nullptr)
		return;
	FString HeroUD = ScrollItemData->HeroUD;
	if (EHeroInventoryFunction::VE_View == eHeroInventoryFunction)
	{
		// Link to Hero Manager
		UUIFunctionLibrary::GoContents(EUIMenuIDEnum::VE_DetailHeros);
	}
}

void UUP_HeroInventory::OnSelectedItem_HeroInventory_ScrollView_HeroGrind(URScrollItem* SelectedItem)
{
	URGameInstance* RGameInstance = RGAMEINSTANCE(this);
	if (RGameInstance == nullptr)
		return;
	if (SelectedItem == nullptr)
		return;

	UHeroBaseScrollItem* HeroSelectScrollItemData = Cast<UHeroBaseScrollItem>(SelectedItem);
	UHeroInventory_ScrollData* ScrollItemData = Cast<UHeroInventory_ScrollData>(SelectedItem->ItemData);
	if (ScrollItemData == nullptr)
		return;

	if (HeroSelectScrollItemData == nullptr)
		return;

	FString HeroUD = ScrollItemData->HeroUD;
	FHERO* HeroData = RGameInstance->RInventory->GetHeroWithUD(HeroUD);
	if (HeroData->locked)
		HeroSelectScrollItemData->SetSelected(false);

	if (SelectedHeros.Contains(HeroUD))
	{
		SelectedHeros.Remove(HeroUD);
		HeroSelectScrollItemData->SetSelected(false);
	}
	else
	{
		SelectedHeros.Emplace(HeroUD);
		HeroSelectScrollItemData->SetSelected(true);
	}
}
 
void UUP_HeroInventory::OnSelectedItem_HeroInventory_ScrollView_HeroLock(URScrollItem* SelectedItem)
{
	URGameInstance* RGameInstance = RGAMEINSTANCE(this);
	if (RGameInstance == nullptr)
		return;

	UUC_HeroInventory_ScrollItem* HeroSelectScrollItemData = Cast<UUC_HeroInventory_ScrollItem>(SelectedItem);
	UHeroInventory_ScrollData* ScrollItemData = Cast<UHeroInventory_ScrollData>(SelectedItem->ItemData);

	if (HeroSelectScrollItemData == nullptr)
		return;
	if (ScrollItemData == nullptr)
		return;

	FString HeroUD = ScrollItemData->HeroUD;

	if (EHeroInventoryFunction::VE_Lock == eHeroInventoryFunction)
	{
		if (SelectedHeros.Contains(HeroUD))
		{
			SelectedHeros.Remove(HeroUD);
			HeroSelectScrollItemData->SetSelected(false);
		}
		else
		{
			SelectedHeros.Emplace(HeroUD);
			HeroSelectScrollItemData->SetSelected(true);
		}
	}
}

bool UUP_HeroInventory::IsInSelectedList(FString HeroUD)
{
	for (auto& heroUD : SelectedHeros)
	{
		if( heroUD == HeroUD)
			return true;
	}
	return false;
}

void UUP_HeroInventory::SetSelectingAllItem()
{
	for (auto& scrollItem : HeroInventoryScrollView->ItemDataList)
	{
		if (Cast<UHeroInventory_ScrollData>(scrollItem)->bIsInArenaDefence)
			continue;
		Cast<UHeroInventory_ScrollData>(scrollItem)->HeroIconInfo.eIconSelectType = EHeroIconSelectTypeEnum::eHeroSelect_Selected;
	}
}

 void UUP_HeroInventory_LightFilter::NativeConstruct()
 {
 	Super::NativeConstruct();
 	
	if (Button_AttackPower->SortingBtn)
	{
		Button_AttackPower->SortingBtn->OnClicked.AddDynamic(this, &UUP_HeroInventory_LightFilter::OnClick_HeroInventory_LightFilter_AttackPower);
		Button_AttackPower->Text_Type->SetText(UUtilFunctionLibrary::GetLocalizedString(EStringTableType::VE_UI, FName(TEXT("HeroUI_ListMethod_Power"))));
	}

	if (Button_Level->SortingBtn)
	{
		Button_Level->SortingBtn->OnClicked.AddDynamic(this, &UUP_HeroInventory_LightFilter::OnClick_HeroInventory_LightFilter_Level);
		Button_Level->Text_Type->SetText(UUtilFunctionLibrary::GetLocalizedString(EStringTableType::VE_UI, FName(TEXT("HeroUI_ListMethod_Level"))));
	}
	if (Button_Grade->SortingBtn)
	{
		Button_Grade->SortingBtn->OnClicked.AddDynamic(this, &UUP_HeroInventory_LightFilter::OnClick_HeroInventory_LightFilter_Grade);
		Button_Grade->Text_Type->SetText(UUtilFunctionLibrary::GetLocalizedString(EStringTableType::VE_UI, FName(TEXT("HeroUI_ListMethod_Grade"))));
	}
	if (Button_Damage->SortingBtn)
	{
		Button_Damage->SortingBtn->OnClicked.AddDynamic(this, &UUP_HeroInventory_LightFilter::OnClick_HeroInventory_LightFilter_Damage);
		Button_Damage->Text_Type->SetText(UUtilFunctionLibrary::GetLocalizedString(EStringTableType::VE_UI, FName(TEXT("HeroUI_ListMethod_Attack"))));
	}
	if (Button_PD->SortingBtn)
	{
		Button_PD->SortingBtn->OnClicked.AddDynamic(this, &UUP_HeroInventory_LightFilter::OnClick_HeroInventory_LightFilter_PhysicalDefense);
		Button_PD->Text_Type->SetText(UUtilFunctionLibrary::GetLocalizedString(EStringTableType::VE_UI, FName(TEXT("HeroUI_ListMethod_PhyDefence"))));
	}
	if (Button_MD->SortingBtn)
	{
		Button_MD->SortingBtn->OnClicked.AddDynamic(this, &UUP_HeroInventory_LightFilter::OnClick_HeroInventory_LightFilter_MagicDefense);
		Button_MD->Text_Type->SetText(UUtilFunctionLibrary::GetLocalizedString(EStringTableType::VE_UI, FName(TEXT("HeroUI_ListMethod_MagicDefence"))));
	}
	if (Button_Health->SortingBtn)
	{
		Button_Health->SortingBtn->OnClicked.AddDynamic(this, &UUP_HeroInventory_LightFilter::OnClick_HeroInventory_LightFilter_Health);
		Button_Health->Text_Type->SetText(UUtilFunctionLibrary::GetLocalizedString(EStringTableType::VE_UI, FName(TEXT("HeroUI_ListMethod_HP"))));
	}
 }
 
 void UUP_HeroInventory_LightFilter::NativeDestruct()
 {
	if (Button_AttackPower->SortingBtn)
		Button_AttackPower->SortingBtn->OnClicked.RemoveDynamic(this, &UUP_HeroInventory_LightFilter::OnClick_HeroInventory_LightFilter_AttackPower);
	if (Button_Level->SortingBtn)
		Button_Level->SortingBtn->OnClicked.RemoveDynamic(this, &UUP_HeroInventory_LightFilter::OnClick_HeroInventory_LightFilter_Level);
	if (Button_Grade->SortingBtn)
		Button_Grade->SortingBtn->OnClicked.RemoveDynamic(this, &UUP_HeroInventory_LightFilter::OnClick_HeroInventory_LightFilter_Grade);
	if (Button_Damage->SortingBtn)
		Button_Damage->SortingBtn->OnClicked.RemoveDynamic(this, &UUP_HeroInventory_LightFilter::OnClick_HeroInventory_LightFilter_Damage);
	if (Button_PD->SortingBtn)
		Button_PD->SortingBtn->OnClicked.RemoveDynamic(this, &UUP_HeroInventory_LightFilter::OnClick_HeroInventory_LightFilter_PhysicalDefense);
	if (Button_MD->SortingBtn)
		Button_MD->SortingBtn->OnClicked.RemoveDynamic(this, &UUP_HeroInventory_LightFilter::OnClick_HeroInventory_LightFilter_MagicDefense);
	if (Button_Health->SortingBtn)
		Button_Health->SortingBtn->OnClicked.RemoveDynamic(this, &UUP_HeroInventory_LightFilter::OnClick_HeroInventory_LightFilter_Health);
 	
	Super::NativeDestruct();
 }

 void UUP_HeroInventory_LightFilter::OnClick_HeroInventory_LightFilter_AttackPower()
 {
	 OnSelectedHeroLightFilter.Broadcast((int32)EHeroListFilterEnum::VE_AttackPower);
 }

 void UUP_HeroInventory_LightFilter::OnClick_HeroInventory_LightFilter_Level()
 {
	 OnSelectedHeroLightFilter.Broadcast((int32)EHeroListFilterEnum::VE_Level);
 }

 void UUP_HeroInventory_LightFilter::OnClick_HeroInventory_LightFilter_Grade()
 {
	 OnSelectedHeroLightFilter.Broadcast((int32)EHeroListFilterEnum::VE_Grade);
 }

 void UUP_HeroInventory_LightFilter::OnClick_HeroInventory_LightFilter_Damage()
 {
	 OnSelectedHeroLightFilter.Broadcast((int32)EHeroListFilterEnum::VE_Damage);
 }

 void UUP_HeroInventory_LightFilter::OnClick_HeroInventory_LightFilter_PhysicalDefense()
 {
	 OnSelectedHeroLightFilter.Broadcast((int32)EHeroListFilterEnum::VE_PhysicalDefense);
 }

 void UUP_HeroInventory_LightFilter::OnClick_HeroInventory_LightFilter_MagicDefense()
 {
	 OnSelectedHeroLightFilter.Broadcast((int32)EHeroListFilterEnum::VE_MagicDefense);
 }

 void UUP_HeroInventory_LightFilter::OnClick_HeroInventory_LightFilter_Health()
 {
	 OnSelectedHeroLightFilter.Broadcast((int32)EHeroListFilterEnum::VE_Health);
 }

 void UUP_HeroInventory_LightFilter::SetSelectedSortingBtnColorNSwitcher(EHeroListFilterEnum currentFilter, bool isdescending)
 {
 	ResetSortingBtnColor();
 	ResetDescendingBtn();
 
 	switch (currentFilter)
 	{
 	case EHeroListFilterEnum::VE_AttackPower:
 		if (!isdescending)
			Button_AttackPower->SortTypeSwitcher->SetActiveWidgetIndex(1);
		Button_AttackPower->CheckStateSwitcher->SetActiveWidgetIndex(0);
 		break;
 	case EHeroListFilterEnum::VE_Level:
 		if (!isdescending)
			Button_Level->SortTypeSwitcher->SetActiveWidgetIndex(1);
		Button_Level->CheckStateSwitcher->SetActiveWidgetIndex(0);
 		break;
 	case EHeroListFilterEnum::VE_Grade:
 		if (!isdescending)
			Button_Grade->SortTypeSwitcher->SetActiveWidgetIndex(1);
		Button_Grade->CheckStateSwitcher->SetActiveWidgetIndex(0);
 		break;
 	case EHeroListFilterEnum::VE_Damage:
 		if (!isdescending)
			Button_Damage->SortTypeSwitcher->SetActiveWidgetIndex(1);
		Button_Damage->CheckStateSwitcher->SetActiveWidgetIndex(0);
 		break;
 	case EHeroListFilterEnum::VE_PhysicalDefense:
 		if (!isdescending)
			Button_PD->SortTypeSwitcher->SetActiveWidgetIndex(1);
		Button_PD->CheckStateSwitcher->SetActiveWidgetIndex(0);
 		break;
 	case EHeroListFilterEnum::VE_MagicDefense:
 		if (!isdescending)
			Button_MD->SortTypeSwitcher->SetActiveWidgetIndex(1);
		Button_MD->CheckStateSwitcher->SetActiveWidgetIndex(0);
 		break;
 	case EHeroListFilterEnum::VE_Health:
 		if (!isdescending)
			Button_Health->SortTypeSwitcher->SetActiveWidgetIndex(1);
		Button_Health->CheckStateSwitcher->SetActiveWidgetIndex(0);
 		break;
 	default:
 		break;
 	}
 }
 
 void UUP_HeroInventory_LightFilter::ResetSortingBtnColor()
 {
	 Button_AttackPower->CheckStateSwitcher->SetActiveWidgetIndex(1);
	 Button_Level->CheckStateSwitcher->SetActiveWidgetIndex(1);
	 Button_Grade->CheckStateSwitcher->SetActiveWidgetIndex(1);
	 Button_Damage->CheckStateSwitcher->SetActiveWidgetIndex(1);
	 Button_PD->CheckStateSwitcher->SetActiveWidgetIndex(1);
	 Button_MD->CheckStateSwitcher->SetActiveWidgetIndex(1);
	 Button_Health->CheckStateSwitcher->SetActiveWidgetIndex(1);
 }
 
 void UUP_HeroInventory_LightFilter::ResetDescendingBtn()
 {
	 Button_AttackPower->SortTypeSwitcher->SetActiveWidgetIndex(0);
	 Button_Level->SortTypeSwitcher->SetActiveWidgetIndex(0);
	 Button_Grade->SortTypeSwitcher->SetActiveWidgetIndex(0);
	 Button_Damage->SortTypeSwitcher->SetActiveWidgetIndex(0);
	 Button_PD->SortTypeSwitcher->SetActiveWidgetIndex(0);
	 Button_MD->SortTypeSwitcher->SetActiveWidgetIndex(0);
	 Button_Health->SortTypeSwitcher->SetActiveWidgetIndex(0);
 }

 void UUC_Hero_Filter_Check::NativeConstruct()
 {
 	//OnClicked_Button_Reset();
 	Button_Reset->OnClicked.AddDynamic(this, &UUC_Hero_Filter_Check::OnClicked_Button_Reset);
 	Button_Close->OnClicked.AddDynamic(this, &UUC_Hero_Filter_Check::OnClicked_Button_Close);
 	CheckBox_Tanker->OnCheckStateChanged.AddDynamic(this, &UUC_Hero_Filter_Check::FilteringAllConditions);
 	CheckBox_MeleeDealer->OnCheckStateChanged.AddDynamic(this, &UUC_Hero_Filter_Check::FilteringAllConditions);
 	CheckBox_RangeDealer->OnCheckStateChanged.AddDynamic(this, &UUC_Hero_Filter_Check::FilteringAllConditions);
 	CheckBox_MagicDealer->OnCheckStateChanged.AddDynamic(this, &UUC_Hero_Filter_Check::FilteringAllConditions);
 	CheckBox_Supporter->OnCheckStateChanged.AddDynamic(this, &UUC_Hero_Filter_Check::FilteringAllConditions);
 	CheckBox_Hero->OnCheckStateChanged.AddDynamic(this, &UUC_Hero_Filter_Check::FilteringAllConditions);
 	CheckBox_Spirit->OnCheckStateChanged.AddDynamic(this, &UUC_Hero_Filter_Check::FilteringAllConditions);
 	CheckBox_Grade_One->OnCheckStateChanged.AddDynamic(this, &UUC_Hero_Filter_Check::FilteringAllConditions);
 	CheckBox_Grade_Two->OnCheckStateChanged.AddDynamic(this, &UUC_Hero_Filter_Check::FilteringAllConditions);
 	CheckBox_Grade_Three->OnCheckStateChanged.AddDynamic(this, &UUC_Hero_Filter_Check::FilteringAllConditions);
 	CheckBox_Grade_Four->OnCheckStateChanged.AddDynamic(this, &UUC_Hero_Filter_Check::FilteringAllConditions);
 	CheckBox_Grade_Five->OnCheckStateChanged.AddDynamic(this, &UUC_Hero_Filter_Check::FilteringAllConditions);
 	CheckBox_Grade_Six->OnCheckStateChanged.AddDynamic(this, &UUC_Hero_Filter_Check::FilteringAllConditions);
 	CheckBox_Grade_Seven->OnCheckStateChanged.AddDynamic(this, &UUC_Hero_Filter_Check::FilteringAllConditions);
 	CheckBox_Grade_Eight->OnCheckStateChanged.AddDynamic(this, &UUC_Hero_Filter_Check::FilteringAllConditions);
 	CheckBox_Grade_Nine->OnCheckStateChanged.AddDynamic(this, &UUC_Hero_Filter_Check::FilteringAllConditions);
 	CheckBox_Grade_Ten->OnCheckStateChanged.AddDynamic(this, &UUC_Hero_Filter_Check::FilteringAllConditions);
 	CheckBox_Lux->OnCheckStateChanged.AddDynamic(this, &UUC_Hero_Filter_Check::FilteringAllConditions);
 	CheckBox_Solona->OnCheckStateChanged.AddDynamic(this, &UUC_Hero_Filter_Check::FilteringAllConditions);
 	CheckBox_Runa->OnCheckStateChanged.AddDynamic(this, &UUC_Hero_Filter_Check::FilteringAllConditions);
 	CheckBox_Nox->OnCheckStateChanged.AddDynamic(this, &UUC_Hero_Filter_Check::FilteringAllConditions);
 
 }
 
 void UUC_Hero_Filter_Check::NativeDestruct()
 {
 	Button_Reset->OnClicked.RemoveDynamic(this, &UUC_Hero_Filter_Check::OnClicked_Button_Reset);
 	Button_Close->OnClicked.RemoveDynamic(this, &UUC_Hero_Filter_Check::OnClicked_Button_Close);
 	CheckBox_Tanker->OnCheckStateChanged.RemoveDynamic(this, &UUC_Hero_Filter_Check::FilteringAllConditions);
 	CheckBox_MeleeDealer->OnCheckStateChanged.RemoveDynamic(this, &UUC_Hero_Filter_Check::FilteringAllConditions);
 	CheckBox_RangeDealer->OnCheckStateChanged.RemoveDynamic(this, &UUC_Hero_Filter_Check::FilteringAllConditions);
 	CheckBox_MagicDealer->OnCheckStateChanged.RemoveDynamic(this, &UUC_Hero_Filter_Check::FilteringAllConditions);
 	CheckBox_Supporter->OnCheckStateChanged.RemoveDynamic(this, &UUC_Hero_Filter_Check::FilteringAllConditions);
 	CheckBox_Hero->OnCheckStateChanged.RemoveDynamic(this, &UUC_Hero_Filter_Check::FilteringAllConditions);
 	CheckBox_Spirit->OnCheckStateChanged.RemoveDynamic(this, &UUC_Hero_Filter_Check::FilteringAllConditions);
 	CheckBox_Grade_One->OnCheckStateChanged.RemoveDynamic(this, &UUC_Hero_Filter_Check::FilteringAllConditions);
 	CheckBox_Grade_Two->OnCheckStateChanged.RemoveDynamic(this, &UUC_Hero_Filter_Check::FilteringAllConditions);
 	CheckBox_Grade_Three->OnCheckStateChanged.RemoveDynamic(this, &UUC_Hero_Filter_Check::FilteringAllConditions);
 	CheckBox_Grade_Four->OnCheckStateChanged.RemoveDynamic(this, &UUC_Hero_Filter_Check::FilteringAllConditions);
 	CheckBox_Grade_Five->OnCheckStateChanged.RemoveDynamic(this, &UUC_Hero_Filter_Check::FilteringAllConditions);
 	CheckBox_Grade_Six->OnCheckStateChanged.RemoveDynamic(this, &UUC_Hero_Filter_Check::FilteringAllConditions);
 	CheckBox_Grade_Seven->OnCheckStateChanged.RemoveDynamic(this, &UUC_Hero_Filter_Check::FilteringAllConditions);
 	CheckBox_Grade_Eight->OnCheckStateChanged.RemoveDynamic(this, &UUC_Hero_Filter_Check::FilteringAllConditions);
 	CheckBox_Grade_Nine->OnCheckStateChanged.RemoveDynamic(this, &UUC_Hero_Filter_Check::FilteringAllConditions);
 	CheckBox_Grade_Ten->OnCheckStateChanged.RemoveDynamic(this, &UUC_Hero_Filter_Check::FilteringAllConditions);
 	CheckBox_Lux->OnCheckStateChanged.RemoveDynamic(this, &UUC_Hero_Filter_Check::FilteringAllConditions);
 	CheckBox_Solona->OnCheckStateChanged.RemoveDynamic(this, &UUC_Hero_Filter_Check::FilteringAllConditions);
 	CheckBox_Runa->OnCheckStateChanged.RemoveDynamic(this, &UUC_Hero_Filter_Check::FilteringAllConditions);
 	CheckBox_Nox->OnCheckStateChanged.RemoveDynamic(this, &UUC_Hero_Filter_Check::FilteringAllConditions);
 }
 
 void UUC_Hero_Filter_Check::OnClicked_Button_Reset()
 {
	CheckBoxReset();
 	FilteringAllConditions(true);
 }
 
void UUC_Hero_Filter_Check::OnClicked_Button_Close()
{
	UUP_HeroInventory* pHeroInven = Cast<UUP_HeroInventory>(ParentWidget);
	if(pHeroInven)
	{
		pHeroInven->bIsVisibleDetailFilter = false;
		pHeroInven->ShowDetailFilter();
	}
	SetVisibility(ESlateVisibility::Collapsed);
}
 
 void UUC_Hero_Filter_Check::SetHeroFilterMenu()
 {
	 SetFilterMenuReset();
	 CheckBox_Hero->SetIsChecked(true);
	 CheckBox_Hero->SetIsEnabled(false);
	 CheckBox_Spirit->SetIsEnabled(false);
	 CheckBox_Grade_One->SetIsEnabled(false);
	 CheckBox_Grade_Two->SetIsEnabled(false);
 }

 void UUC_Hero_Filter_Check::SetSoulFilterMenu()
 {
	 SetFilterMenuReset();
	 CheckBox_Spirit->SetIsChecked(true);
	 CheckBox_Spirit->SetIsEnabled(false);
	 CheckBox_Hero->SetIsEnabled(false);
	 CheckBox_Grade_Seven->SetIsEnabled(false);
	 CheckBox_Grade_Eight->SetIsEnabled(false);
	 CheckBox_Grade_Nine->SetIsEnabled(false);
	 CheckBox_Grade_Ten->SetIsEnabled(false);
 }

 void UUC_Hero_Filter_Check::SetFilterMenuReset()
 {
	 CheckBox_Spirit->SetIsEnabled(true);
	 CheckBox_Spirit->SetIsChecked(false);
	 CheckBox_Hero->SetIsEnabled(true);
	 CheckBox_Hero->SetIsChecked(false);
	 CheckBox_Grade_One->SetIsEnabled(true);
	 CheckBox_Grade_Two->SetIsEnabled(true);
	 CheckBox_Grade_Seven->SetIsEnabled(true);
	 CheckBox_Grade_Eight->SetIsEnabled(true);
	 CheckBox_Grade_Nine->SetIsEnabled(true);
	 CheckBox_Grade_Ten->SetIsEnabled(true);
 }

void UUC_Hero_Filter_Check::Filter_Class(TArray<FString>& InHeroList, TArray<EClassTypeEnum>& InFilter)
{
	TArray<FString> FilterHeroUDList;
	for (FString& ud : InHeroList)
	{
		FHERO* HeroInvenData = RGAMEINSTANCE(this)->RInventory->GetHeroWithUD(ud);
		if (!HeroInvenData)
			continue;
		auto HeroTableInfo = UTableManager::GetInstancePtr()->GetCharacterRow(HeroInvenData->heroId);
		if (HeroTableInfo == nullptr)
			continue;

		for (EClassTypeEnum eType : InFilter)
		{
			if (eType == HeroTableInfo->Class)
			{
				FilterHeroUDList.Emplace(ud);
				break;
			}
		}
	}
	InHeroList = FilterHeroUDList;
}

void UUC_Hero_Filter_Check::Filter_Grade(TArray<FString>& InHeroList, TArray<int32>& InFilter)
{
	TArray<FString> FilterHeroUDList;
	for (FString& ud : InHeroList)
	{
		FHERO* HeroInvenData = RGAMEINSTANCE(this)->RInventory->GetHeroWithUD(ud);
		if (!HeroInvenData)
			continue;
		auto HeroTableInfo = UTableManager::GetInstancePtr()->GetCharacterRow(HeroInvenData->heroId);
		if (HeroTableInfo == nullptr)
			continue;

		for (int32 eType : InFilter)
		{
			if (eType == HeroTableInfo->Grade)
			{
				FilterHeroUDList.Emplace(ud);
				break;
			}
		}
	}
	InHeroList = FilterHeroUDList;
}

void UUC_Hero_Filter_Check::Filter_Continent(TArray<FString>& InHeroList, TArray<EContinentTypeEnum>& InFilter)
{
	TArray<FString> FilterHeroUDList;
	for (FString& ud : InHeroList)
	{
		FHERO* HeroInvenData = RGAMEINSTANCE(this)->RInventory->GetHeroWithUD(ud);
		if (!HeroInvenData)
			continue;
		auto HeroTableInfo = UTableManager::GetInstancePtr()->GetCharacterRow(HeroInvenData->heroId);
		if (HeroTableInfo == nullptr)
			continue;

		for (EContinentTypeEnum eType : InFilter)
		{
			if (eType == HeroTableInfo->Continent)
			{
				FilterHeroUDList.Emplace(ud);
				break;
			}
		}
	}
	InHeroList = FilterHeroUDList;
}

 void UUC_Hero_Filter_Check::FilteringAllConditions(bool isChecked)
 {
 	URGameInstance* RGameInstance = RGAMEINSTANCE(this);
 
 	FilteringHeroUDList.Reset();
	TArray<FString> tempAllList;
	RGameInstance->RInventory->GetHeroUDs(tempAllList, EClassTypeEnum::VE_ClassTypeMax, EHeroInventoryType::VE_InventoryTypeAll);
	EHeroInventoryType inven_Type;
	if (CheckBox_Hero->IsChecked() && CheckBox_Spirit->IsChecked())
		inven_Type = EHeroInventoryType::VE_InventoryTypeAll;
	else if (CheckBox_Hero->IsChecked() && (!CheckBox_Spirit->IsChecked()))
		inven_Type = EHeroInventoryType::VE_TypeHero;
	else if ((!CheckBox_Hero->IsChecked()) && CheckBox_Spirit->IsChecked())
		inven_Type = EHeroInventoryType::VE_TypeSpirit;
	else
		inven_Type = EHeroInventoryType::VE_InventoryTypeAll;
		

	RGameInstance->RInventory->GetHeroUDs(tempAllList, EClassTypeEnum::VE_ClassTypeMax, inven_Type);
	
	//RGameInstance->RInventory->classFilter.Reset();
	if (CheckBox_Tanker->IsChecked())
		RGameInstance->RInventory->classFilter.AddUnique(EClassTypeEnum::VE_Tanker);
	else
		RGameInstance->RInventory->classFilter.Remove(EClassTypeEnum::VE_Tanker);
	if (CheckBox_MeleeDealer->IsChecked())
		RGameInstance->RInventory->classFilter.AddUnique(EClassTypeEnum::VE_MeleeDealer);
	else
		RGameInstance->RInventory->classFilter.Remove(EClassTypeEnum::VE_MeleeDealer);
	if (CheckBox_RangeDealer->IsChecked())
		RGameInstance->RInventory->classFilter.AddUnique(EClassTypeEnum::VE_RangeDealer);
	else
		RGameInstance->RInventory->classFilter.Remove(EClassTypeEnum::VE_RangeDealer);
	if (CheckBox_MagicDealer->IsChecked())
		RGameInstance->RInventory->classFilter.AddUnique(EClassTypeEnum::VE_MagicDealer);
	else
		RGameInstance->RInventory->classFilter.Remove(EClassTypeEnum::VE_MagicDealer);
	if (CheckBox_Supporter->IsChecked())
		RGameInstance->RInventory->classFilter.AddUnique(EClassTypeEnum::VE_Supporter);
	else
		RGameInstance->RInventory->classFilter.Remove(EClassTypeEnum::VE_Supporter);

	if (0 < RGameInstance->RInventory->classFilter.Num())
		Filter_Class(tempAllList, RGameInstance->RInventory->classFilter);
	
	//RGameInstance->RInventory->gradeFilter.Reset();
	if (CheckBox_Grade_One->IsChecked())
		RGameInstance->RInventory->gradeFilter.AddUnique(1);
	else
		RGameInstance->RInventory->gradeFilter.Remove(1);
	if (CheckBox_Grade_Two->IsChecked())
		RGameInstance->RInventory->gradeFilter.AddUnique(2);
	else
		RGameInstance->RInventory->gradeFilter.Remove(2);
	if (CheckBox_Grade_Three->IsChecked())
		RGameInstance->RInventory->gradeFilter.AddUnique(3);
	else
		RGameInstance->RInventory->gradeFilter.Remove(3);
	if (CheckBox_Grade_Four->IsChecked())
		RGameInstance->RInventory->gradeFilter.AddUnique(4);
	else
		RGameInstance->RInventory->gradeFilter.Remove(4);
	if (CheckBox_Grade_Five->IsChecked())
		RGameInstance->RInventory->gradeFilter.AddUnique(5);
	else
		RGameInstance->RInventory->gradeFilter.Remove(5);
	if (CheckBox_Grade_Six->IsChecked())
		RGameInstance->RInventory->gradeFilter.AddUnique(6);
	else
		RGameInstance->RInventory->gradeFilter.Remove(6);
	if (CheckBox_Grade_Seven->IsChecked())
		RGameInstance->RInventory->gradeFilter.AddUnique(7);
	else
		RGameInstance->RInventory->gradeFilter.Remove(7);
	if (CheckBox_Grade_Eight->IsChecked())
		RGameInstance->RInventory->gradeFilter.AddUnique(8);
	else
		RGameInstance->RInventory->gradeFilter.Remove(8);
	if (CheckBox_Grade_Nine->IsChecked())
		RGameInstance->RInventory->gradeFilter.AddUnique(9);
	else
		RGameInstance->RInventory->gradeFilter.Remove(9);
	if (CheckBox_Grade_Ten->IsChecked())
		RGameInstance->RInventory->gradeFilter.AddUnique(10);
	else
		RGameInstance->RInventory->gradeFilter.Remove(10);

	if (0 < RGameInstance->RInventory->gradeFilter.Num())
		Filter_Grade(tempAllList, RGameInstance->RInventory->gradeFilter);
	
	//RGameInstance->RInventory->continentFilter.Reset();
	if (CheckBox_Lux->IsChecked())
		RGameInstance->RInventory->continentFilter.AddUnique(EContinentTypeEnum::Lux);
	else
		RGameInstance->RInventory->continentFilter.Remove(EContinentTypeEnum::Lux);
	if (CheckBox_Solona->IsChecked())
		RGameInstance->RInventory->continentFilter.AddUnique(EContinentTypeEnum::Solona);
	else
		RGameInstance->RInventory->continentFilter.Remove(EContinentTypeEnum::Solona);
	if (CheckBox_Runa->IsChecked())
		RGameInstance->RInventory->continentFilter.AddUnique(EContinentTypeEnum::Luna);
	else
		RGameInstance->RInventory->continentFilter.Remove(EContinentTypeEnum::Luna);
	if (CheckBox_Nox->IsChecked())
		RGameInstance->RInventory->continentFilter.AddUnique(EContinentTypeEnum::Nox);
	else
		RGameInstance->RInventory->continentFilter.Remove(EContinentTypeEnum::Nox);
 
	if (0 < RGameInstance->RInventory->continentFilter.Num())
		Filter_Continent(tempAllList, RGameInstance->RInventory->continentFilter);

	if (0 < tempAllList.Num())
	{
		OnSelectedHeroDetailFilter.Broadcast(tempAllList);
	}
	else
	{
		if (RGameInstance->RInventory->classFilter.Num() == 0 && RGameInstance->RInventory->gradeFilter.Num() == 0 && RGameInstance->RInventory->continentFilter.Num() == 0)
		{
			TArray<FString> allList;
			RGameInstance->RInventory->GetHeroUDs(allList, EClassTypeEnum::VE_ClassTypeMax, EHeroInventoryType::VE_InventoryTypeAll);
			OnSelectedHeroDetailFilter.Broadcast(allList);
		}
		else
 			OnSelectedHeroDetailFilter.Broadcast(TArray<FString>());
	}
 }
 
 void UUC_Hero_Filter_Check::CheckBoxReset()
 {
	 CheckBox_Tanker->SetIsChecked(false);
	 CheckBox_MeleeDealer->SetIsChecked(false);
	 CheckBox_RangeDealer->SetIsChecked(false);
	 CheckBox_MagicDealer->SetIsChecked(false);
	 CheckBox_Supporter->SetIsChecked(false);
	 CheckBox_Hero->SetIsChecked(false);
	 CheckBox_Spirit->SetIsChecked(false);
	 CheckBox_Grade_One->SetIsChecked(false);
	 CheckBox_Grade_Two->SetIsChecked(false);
	 CheckBox_Grade_Three->SetIsChecked(false);
	 CheckBox_Grade_Four->SetIsChecked(false);
	 CheckBox_Grade_Five->SetIsChecked(false);
	 CheckBox_Grade_Six->SetIsChecked(false);
	 CheckBox_Grade_Seven->SetIsChecked(false);
	 CheckBox_Grade_Eight->SetIsChecked(false);
	 CheckBox_Grade_Nine->SetIsChecked(false);
	 CheckBox_Grade_Ten->SetIsChecked(false);
	 CheckBox_Lux->SetIsChecked(false);
	 CheckBox_Solona->SetIsChecked(false);
	 CheckBox_Runa->SetIsChecked(false);
	 CheckBox_Nox->SetIsChecked(false);
 }

 void UUC_HeroInventory_SoulFilter::NativeConstruct()
 {
	 if (Button_Grade)
		 Button_Grade->OnClicked.AddDynamic(this, &UUC_HeroInventory_SoulFilter::OnClick_HeroInventory_SoulFilter_Grade);
 }

 void UUC_HeroInventory_SoulFilter::NativeDestruct()
 {
	 if (Button_Grade)
		 Button_Grade->OnClicked.RemoveDynamic(this, &UUC_HeroInventory_SoulFilter::OnClick_HeroInventory_SoulFilter_Grade);
 }

 void UUC_HeroInventory_SoulFilter::OnClick_HeroInventory_SoulFilter_Grade()
 {
	 OnSelectedHeroLightFilter.Broadcast((int32)EHeroListFilterEnum::VE_Grade);
 }

 void UUC_HeroInventory_SoulFilter::SetWidgetSwitcher(bool isDescending)
 {
	 if (isDescending)
		 GradeSwitcher->SetActiveWidgetIndex(0);
	 else
		 GradeSwitcher->SetActiveWidgetIndex(1);
 }

 void UUC_HeroInventory_SoulFilter::ResetBtn()
 {
	 GradeSwitcher->SetActiveWidgetIndex(0);
 }

 void UUC_Popup_InventoryPlus::NativeConstruct()
 {
	 if(Button_Close)
		Button_Close->OnClicked.AddDynamic(this, &UUC_Popup_InventoryPlus::OnClick_ButtonClose);
	 if (Button_Cancel)
		Button_Cancel->OnClicked.AddDynamic(this, &UUC_Popup_InventoryPlus::OnClick_ButtonCancel);
	if (Button_OK)
		Button_OK->OnClicked.AddDynamic(this, &UUC_Popup_InventoryPlus::OnClick_ButtonOK);
 }

 void UUC_Popup_InventoryPlus::NativeDestruct()
 {
	 if (Button_Close)
		Button_Close->OnClicked.RemoveDynamic(this, &UUC_Popup_InventoryPlus::OnClick_ButtonClose);
	 if (Button_Cancel)
		Button_Cancel->OnClicked.RemoveDynamic(this, &UUC_Popup_InventoryPlus::OnClick_ButtonCancel);
	 if (Button_OK)
		Button_OK->OnClicked.RemoveDynamic(this, &UUC_Popup_InventoryPlus::OnClick_ButtonOK);
 }

 void UUC_Popup_InventoryPlus::OnClick_ButtonClose()
 {
	 this->SetVisibility(ESlateVisibility::Collapsed);
 }

 void UUC_Popup_InventoryPlus::OnClick_ButtonCancel()
 {
	 this->SetVisibility(ESlateVisibility::Collapsed);
 }

 void UUC_Popup_InventoryPlus::OnClick_ButtonOK()
 {
	 UPacketFunctionLibrary::INVENTORY_UPGRADE_RQ(token + 1, (int32)(EINVENTORY_UPGRADE_TYPE::HERO));
	 this->SetVisibility(ESlateVisibility::Collapsed);
	 UUIFunctionLibrary::ShowWaitingPopup(true);
 }

void UUC_Popup_InventoryPlus::BindingText(int32 startSize, int32 slotRate, int32 tokenMax, int32 currentToken)
{
	int32 nNeedGem = (currentToken + 1) * 10;
	int32 nNextSlotSize = startSize + (slotRate * (currentToken + 1));
	int32 nCurrentSlotSize = startSize + (slotRate * currentToken);

	FText format = UUtilFunctionLibrary::GetLocalizedString(EStringTableType::VE_UI, FName(TEXT("HeroUI_InventoryExpansion_PopUp_Message01")));
	FText descText = FText::Format(format, FText::AsNumber(nNeedGem), FText::AsNumber(slotRate));
	TextBlock_Description->SetText(descText);

	TextBlock_CurrentAmount->SetText(FText::AsNumber(nCurrentSlotSize));
	TextBlock_NextAmount->SetText(FText::AsNumber(nNextSlotSize));
	Text_ActiveCurrency->SetText(FText::AsNumber(nNeedGem));
	token = currentToken;
}
