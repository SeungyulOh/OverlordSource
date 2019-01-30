// Fill out your copyright notice in the Description page of Project Settings.

#include "ProjectR.h"
#include "UC_SkillManagement.h"

#include "GlobalIntegrated.h"



#include "Network/PacketDataStructures.h"
#include "UI/Control/RScrollView.h"

#include "GameInfo/RInventory.h"


#include "UtilFunctionIntegrated.h"

#include "UI/HeroManagement/UC_SkillDetail.h"
#include "UI/HeroManagement/SkillInfoScrollItemData.h"
#include "UC_SkillInfoScrollItem.h"
#include "Table/CharacterTableInfo.h"
#include "Table/SkillTableInfo_V2.h"
#include "Table/SkillResourceTableInfo.h"
#include "UI/UI_Common/TopBar.h"

#define SKILL_ACTIVE_1 0
#define SKILL_ACTIVE_2 1
#define SKILL_ACTIVE_3 2
#define SKILL_PASSIVE_1 3

void UUC_SkillManagement::NativeConstruct()
{
	Super::NativeConstruct();
	   
	if (SkillDataArray.Num() == 0)
	{
		for (int32 i = SKILL_ACTIVE_1; i <= SKILL_PASSIVE_1; i++)
		{
			SkillDataArray.Emplace(NewObject<USkillInfoScrollItemData>((UObject*)GetTransientPackage(), USkillInfoScrollItemData::StaticClass()));
			SkillDataArray[i]->SkillIndex = i;
		}
	}

	FindChildWidgetsWithCertainName<UUC_SkillInfoScrollItem>(this, SkillItemArray, TEXT("UC_Hero_Skill_"));

	for (int32 i = SKILL_ACTIVE_1; i <= SKILL_PASSIVE_1; i++)
	{
		SkillItemArray[i]->ManagerUI = this;
		SkillItemArray[i]->ItemData = Cast<USkillInfoScrollItemData>(SkillDataArray[i]);
	}
	CurSelectIdx = 0;
	if (IsValid(SkillDetail) && IsValid(SkillLevelUp_Splash))
	{
		SkillDetail->UC_SkillLvUpSuccessPopup = SkillLevelUp_Splash;
	}

	if (RGAMEINSTANCE(this)->EventManager)
	{
		RGAMEINSTANCE(this)->EventManager->OnSkillUpgradeSuccess.RemoveDynamic(this, &UUC_SkillManagement::OnSkillLvUpSuccess);
		RGAMEINSTANCE(this)->EventManager->OnSkillUpgradeSuccess.AddDynamic(this, &UUC_SkillManagement::OnSkillLvUpSuccess);
	}
	   
	FString CurrentHeroUD = UUtilFunctionLibrary::GetCurrentIndoorLobbyHero();
	OnClick_HeroIcon(CurrentHeroUD);


	if (IsValid(Button_Back) && Button_Close)
	{
		Button_Back->OnClicked.Clear();
		Button_Back->OnClicked.AddDynamic(this, &UUC_SkillManagement::OnClick_BackButton);

		Button_Close->OnClicked.Clear();
		Button_Close->OnClicked.AddDynamic(this, &UUC_SkillManagement::OnClick_BackButton);
	}
	
}

void UUC_SkillManagement::OnClick_HeroIcon(FString InHeroUD)
{
	InvalidateByHeroUD(InHeroUD);
}

void UUC_SkillManagement::NativeDestruct()
{
// 	if (IsValid(SkillScrollView))
// 	{
// 		SkillScrollView->OnScrollViewSelectedItemData.RemoveDynamic(this, &UUC_SkillManagement::OnSelectScrollItem);
// 	}

	Super::NativeDestruct();
}

/*
========================================================================================================================
Invalidate Data By HeroUD
========================================================================================================================
*/
void UUC_SkillManagement::InvalidateByHeroUD(const FString& InHeroUD)
{
	CurHeroString = InHeroUD;
	URGameInstance* RGameInstance = RGAMEINSTANCE(this);
	if ( IsValid(RGameInstance->RInventory))
	{
		FHERO* outHero = RGameInstance->RInventory->GetHeroWithUD(InHeroUD);
		if (outHero != nullptr)
		{
// 			if (IsValid(SkillScrollView))
// 			{				
// 				SkillScrollView->ClearScrollItemData();

			FCharacterTableInfo* CharacterTableInfo = UTableManager::GetInstancePtr()->GetCharacterRow(outHero->heroId);
			if (CharacterTableInfo)
			{
				// spirit skill
				//spirit passive don't showing by kkh
				//USkillInfoScrollItemData* ScrollItemData = nullptr;
				//spirit passive
// 					ScrollItemData = Cast<USkillInfoScrollItemData>(SkillScrollView->AddScrollItemData(USkillInfoScrollItemData::StaticClass()));
// 					if (!CharacterTableInfo->SpiritPassive.IsNone())
// 						SetSpiritSkillData(ScrollItemData, CharacterTableInfo->SpiritPassive, 1);

				//active 01
				//ScrollItemData = Cast<USkillInfoScrollItemData>(SkillScrollView->AddScrollItemData(USkillInfoScrollItemData::StaticClass()));
				if (!CharacterTableInfo->ActiveSkill01.IsNone())
				{
					if(outHero->skills.IsValidIndex(SKILL_ACTIVE_1))
						SetSkillData(SkillDataArray[SKILL_ACTIVE_1], CharacterTableInfo->ActiveSkill01, outHero->skills[0] + 1, EHERO_SKILL_SLOT::ActiveSkill01);
					else
						SetSkillData(SkillDataArray[SKILL_ACTIVE_1], CharacterTableInfo->ActiveSkill01, 1, EHERO_SKILL_SLOT::ActiveSkill01);
				}
				SkillItemArray[SKILL_ACTIVE_1]->SetSkillInfo(SkillDataArray[SKILL_ACTIVE_1]);
				//active 02
				//ScrollItemData = Cast<USkillInfoScrollItemData>(SkillScrollView->AddScrollItemData(USkillInfoScrollItemData::StaticClass()));
				if (!CharacterTableInfo->ActiveSkill02.IsNone())
				{
					if (outHero->skills.IsValidIndex(SKILL_ACTIVE_2))
						SetSkillData(SkillDataArray[SKILL_ACTIVE_2], CharacterTableInfo->ActiveSkill02, outHero->skills[1] + 1, EHERO_SKILL_SLOT::ActiveSkill02);
					else
						SetSkillData(SkillDataArray[SKILL_ACTIVE_2], CharacterTableInfo->ActiveSkill02, 1, EHERO_SKILL_SLOT::ActiveSkill02);
				}
				SkillItemArray[SKILL_ACTIVE_2]->SetSkillInfo(SkillDataArray[SKILL_ACTIVE_2]);

				//active 03
				//ScrollItemData = Cast<USkillInfoScrollItemData>(SkillScrollView->AddScrollItemData(USkillInfoScrollItemData::StaticClass()));
				if (!CharacterTableInfo->ActiveSkill03.IsNone())
				{
					if (outHero->skills.IsValidIndex(SKILL_ACTIVE_3))
						SetSkillData(SkillDataArray[SKILL_ACTIVE_3], CharacterTableInfo->ActiveSkill03, outHero->skills[2] + 1, EHERO_SKILL_SLOT::ActiveSkill03);
					else
						SetSkillData(SkillDataArray[SKILL_ACTIVE_3], CharacterTableInfo->ActiveSkill03, 1, EHERO_SKILL_SLOT::ActiveSkill03);
				}
				else
				{
					FName skillname = FName(*(CharacterTableInfo->DisplayName.ToString() + TEXT("Active3")));
					SetSkillData(SkillDataArray[SKILL_ACTIVE_3], skillname, 0, EHERO_SKILL_SLOT::ActiveSkill03);
				}
				SkillItemArray[SKILL_ACTIVE_3]->SetSkillInfo(SkillDataArray[SKILL_ACTIVE_3]);

				//passive 01
				//ScrollItemData = Cast<USkillInfoScrollItemData>(SkillScrollView->AddScrollItemData(USkillInfoScrollItemData::StaticClass()));
				if (!CharacterTableInfo->PassiveSkill01.IsNone())
				{
					if (outHero->skills.IsValidIndex(SKILL_PASSIVE_1))
						SetSkillData(SkillDataArray[SKILL_PASSIVE_1], CharacterTableInfo->PassiveSkill01, outHero->skills[3] + 1, EHERO_SKILL_SLOT::PassiveSkill01);
					else
						SetSkillData(SkillDataArray[SKILL_PASSIVE_1], CharacterTableInfo->PassiveSkill01, 1, EHERO_SKILL_SLOT::PassiveSkill01);
				}
				else
				{
					FName skillname = FName(*(CharacterTableInfo->DisplayName.ToString() + TEXT("Passive1")));
					SetSkillData(SkillDataArray[SKILL_PASSIVE_1], skillname, 0, EHERO_SKILL_SLOT::PassiveSkill01);
				}
				SkillItemArray[SKILL_PASSIVE_1]->SetSkillInfo(SkillDataArray[SKILL_PASSIVE_1]);

				//current no passive 02 data
				/*ScrollItemData = Cast<USkillInfoScrollItemData>(SkillScrollView->AddScrollItemData(USkillInfoScrollItemData::StaticClass()));
				if (!CharacterTableInfo->PassiveSkill02.IsNone())
				{
					if (outHero->skills.IsValidIndex(4))
						SetSkillData(ScrollItemData, CharacterTableInfo->PassiveSkill02, outHero->skills[4] + 1, EHERO_SKILL_SLOT::PassiveSkill02);
					else
						SetSkillData(ScrollItemData, CharacterTableInfo->PassiveSkill02, 1, EHERO_SKILL_SLOT::PassiveSkill02);
				}
				else
				{
					FName skillname = FName(*(CharacterTableInfo->DisplayName.ToString() + TEXT("Passive2")));
					SetSkillData(ScrollItemData, skillname, 0, EHERO_SKILL_SLOT::PassiveSkill02);
				}*/
			}
// 				SkillScrollView->InvalidateData();
// 			}
		}
		CurSelectScrollItemData = SkillDataArray[SKILL_ACTIVE_1];
		OnSelectScrollItem(CurSelectScrollItemData);
		if ( IsValid(RGameInstance->NotifyManager))
		{
			RGameInstance->NotifyManager->CheckHeroSkillUpgradeEach(InHeroUD);
		}
	}
}

void UUC_SkillManagement::SetSkillData(USkillInfoScrollItemData* InItemData, FName InSkillName, int32 InLevel, EHERO_SKILL_SLOT InSkillSlot)
{
	if (IsValid(InItemData))
	{
		InItemData->bSpiritSkill = false;
		InItemData->SkillSlot = InSkillSlot;
		InItemData->CurHeroUD = CurHeroString;
		InItemData->SkillName = InSkillName;
		InItemData->SkillNameTableId = UUtilFunctionLibrary::GetResultSkillID(ECharacterCategoryTypeEnum::VE_Hero, InSkillName, InLevel);
		InItemData->SkillLevel = InLevel;
	}
}

void UUC_SkillManagement::SetSpiritSkillData(USkillInfoScrollItemData* InItemData, FName InSpiritSkillName, int32 InLevel)
{
	if (IsValid(InItemData))
	{
		InItemData->bSpiritSkill = true;
		InItemData->SkillSlot = EHERO_SKILL_SLOT::PassiveSkill02;//temp
		InItemData->CurHeroUD = CurHeroString;
		InItemData->SkillName = InSpiritSkillName;
		InItemData->SkillNameTableId = InSpiritSkillName;//UUtilFunctionLibrary::GetResultSkillID(InSpiritSkillName, InLevel);
		InItemData->SkillLevel = InLevel;
	}
}

void UUC_SkillManagement::RefreshScrollItemData()
{
	if (IsValid(CurSelectScrollItemData))
	{
		URGameInstance* RGameInstance = RGAMEINSTANCE(this);
		if ( IsValid(RGameInstance->RInventory))
		{
			FHERO* outHero = RGameInstance->RInventory->GetHeroWithUD(CurHeroString);
			if (outHero != nullptr)
			{
				if (outHero->skills.IsValidIndex((int32)CurSelectScrollItemData->SkillSlot))
				{
					CurSelectScrollItemData->SkillLevel = outHero->skills[(int32)CurSelectScrollItemData->SkillSlot] + 1;
					if (CurSelectScrollItemData->bSpiritSkill)
						CurSelectScrollItemData->SkillNameTableId = CurSelectScrollItemData->SkillName;
					else
						CurSelectScrollItemData->SkillNameTableId = UUtilFunctionLibrary::GetResultSkillID(ECharacterCategoryTypeEnum::VE_Hero, CurSelectScrollItemData->SkillName, CurSelectScrollItemData->SkillLevel);
					URScrollItemData* selectitem = Cast<URScrollItemData>(CurSelectScrollItemData);
					if (IsValid(selectitem))
					{
						SkillItemArray[CurSelectScrollItemData->SkillIndex]->SetSkillInfo(CurSelectScrollItemData);
// 						if (IsValid(SkillScrollView))
// 						{
// 							SkillScrollView->UpdateScrollItemData(selectitem);
// 						}
						OnSelectScrollItem(selectitem);
					}
				}
			}
		}
	}
}

void UUC_SkillManagement::OnSelectScrollItem(URScrollItemData* selectItem)
{
	SkillItemArray[CurSelectIdx]->Image_Select->SetVisibility(ESlateVisibility::Collapsed);
	USkillInfoScrollItemData* SkillItemData = Cast<USkillInfoScrollItemData>(selectItem);
	CurSelectIdx = SkillItemData->SkillIndex;
	SkillItemArray[CurSelectIdx]->Image_Select->SetVisibility(ESlateVisibility::Visible);
	if (IsValid(SkillItemData))
	{
		CurSelectScrollItemData = SkillItemData;

		URGameInstance* RGameInstance = RGAMEINSTANCE(this);
		if ( IsValid(RGameInstance->RInventory))
		{
			FHERO* outHero = RGameInstance->RInventory->GetHeroWithUD(CurHeroString);
			if (outHero != nullptr)
			{
				FHERO_SKILL OutHeroSkill;
				UCharacterHelpFunctionLibrary::MakeHeroSkillStructure(outHero->heroId, SkillItemData->SkillLevel, (int32)SkillItemData->SkillSlot, OutHeroSkill);

				if(!SkillItemData->bSpiritSkill)
				{
					FSkillTableInfo_V2* SkillInfo = UTableManager::GetInstancePtr()->GetSkillRow_V2(SkillItemData->SkillNameTableId);
					if (SkillInfo)
					{
						FSkillResourceTableInfo* SkillResourceRow = UTableManager::GetInstancePtr()->GetSkillResourceRow(SkillInfo->SkillResourceID);
						{
							if (IsValid(SkillDetail))
								SkillDetail->InvalidateSkillDetail(CurHeroString, OutHeroSkill, *SkillResourceRow, *SkillInfo);
						}
					}
				}
				/*else
				{
					FSkillTableInfo_V2* SkillInfo = UTableManager::GetInstancePtr()->GetSkillRow_V2(SkillItemData->SkillNameTableId);
					if (SkillInfo)
					{
						FSkillResourceTableInfo* SkillResourceRow = UTableManager::GetInstancePtr()->GetSkillResourceRow(SkillInfo->SkillResourceID);
						{
							if (IsValid(SkillDetail))
								SkillDetail->InvalidateSpiritSkillDetail(CurHeroString, OutHeroSkill, *SkillResourceRow, *SkillInfo);
						}
					}
				}*/
			}
		}
	}
}

void UUC_SkillManagement::OnSkillLvUpSuccess()
{
	SkillLevelUp_Splash->SetVisibility(ESlateVisibility::Visible);
	SkillLevelUp_Splash->PlayAni(TEXT("Splash_SkillLevelUp"));
	RefreshScrollItemData();
}

void UUC_SkillManagement::OnClick_BackButton()
{
	URGameInstance* RGameInst = RGAMEINSTANCE(this);

	URWidgetManager* WidgetManager = RGameInst->RWidgetManager;
	if (WidgetManager)
	{
		UTopBar* TopBar = WidgetManager->GetUserWidgetT<UTopBar>(EWidgetBluePrintEnum::WBP_TopBar);
		if (TopBar)
			TopBar->PrevButton();
	}
}

void UUC_SkillLvUpSuccessPopup::NativeConstruct()
{
	Super::NativeConstruct();

	AppearAnim = GetAni(TEXT("Splash_SkillLevelUp"));

	if (IsValid(Button_Close))
	{
		Button_Close->OnClicked.Clear();
		Button_Close->OnClicked.AddDynamic(this, &UUC_SkillLvUpSuccessPopup::OnClick_Close);
	}
}

void UUC_SkillLvUpSuccessPopup::NativeDestruct()
{
	Super::NativeDestruct();
}

void UUC_SkillLvUpSuccessPopup::OnClick_Close()
{
	PlayAnimation(AppearAnim, 0.f, 1.f, EUMGSequencePlayMode::Reverse);
	SetVisibility(ESlateVisibility::HitTestInvisible);
}
