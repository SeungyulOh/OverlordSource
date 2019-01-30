// Fill out your copyright notice in the Description page of Project Settings.

#include "ProjectR.h"
#include "UIFunctionLibrary.h"
#include "ContentIntergrated.h"
#include "SharedConstants/SharedGlobalConstants.h"
#include "GlobalIntegrated.h"
#include "GameInfo/UserInfo.h"
#include "GameInfo/GuildInfo.h"
#include "GameInfo/RInventory.h"
#include "GameState/TowerLobbyState.h"
#include "Table/LevelAppliedTables/WorkedItemTableInfo.h"
#include "Network/HttpRequestClient.h"
#include "Network/PacketDataStructures.h"
#include "CustomStructures/SharedStructures/CharacterBattleInfo.h"
#include "UI/RWidgetManager.h"
#include "UI/UI_Battle/RCampaignHUD_ThreeControl.h"
#include "UI/UI_Battle/RCampaignHUD_RaidThreeControl.h"
#include "UI/UI_Battle/RAttackIcon.h"
#include "UI/UI_Campaign/UC_Campaign_Start.h"
#include "UI/ItemManagement/UP_ItemShortagePopup.h"
#include "UI/UI_Common/TopBar.h"
#include "UI/Campaign/UP_BattleRepeatStatePopup.h"
#include "UI/UI_Battle/RCampaignHUD_RaidOneControl.h"
#include "UI/HeroManagement/UP_HeroInventory.h"
#include "UI/UI_Common/UP_CommonPopup.h"
#include "UI/UI_Common/UP_RewardPopup.h"

#include "UI/Common/ScrollView/UP_HeroScrollView_Bottom.h"

#include "UP_GenericScrollview.h"


#include "UI/Loading/UP_WaitingPopup.h"
#include "UI/Common/Renewal_BaseHeroIcon.h"
#include "UI/InGameHUD/QuestCompletePopup.h"
#include "UtilFunctionIntegrated.h"
#include "Kismet/KismetMathLibrary.h"
#include "UObjectIterator.h"
#include "ContentGameMode/Controllers/BasePlayerController.h"
#include "ContentGameMode/Controllers/StagePlayerController.h"
#include "Blueprint/WidgetLayoutLibrary.h"
#include "Network/PacketFunctionLibrary.h"
#include "Table/SkillTableInfo_V2.h"
#include "Controllers/DPadController.h"
#include "LeaderControlHelper.h"
#include "ContentGameMode/Modes/TowerLobbyGameMode.h"
#include "Network/RTSManager.h"
#include "Engine/UserInterfaceSettings.h"
#include "GameState/Stage/Spawner/BaseStageInfo.h"

#include "UI/ItemManagement/UP_GenericSplash.h"
#include "UI/ItemManagement/UC_GenericPopup.h"
#include "UI/ItemManagement/UC_GenericPopupContent.h"
#include "UI/ItemManagement/UC_RewardAfterGrindScrollView.h"

#include "UI/Common/UP_CommonPopupSplash.h"
#include "UI/Common/UP_RewardPopupSplash.h"
#include "UI/Common/UP_Popup_Splash_AttackPower.h"
#include "UI/Common/UP_Popup_Avatar.h"

#include "UI/RealTime/UP_Matching_Waiting.h"

#include "UP_Popup_Reward_AfterGrind.h"
#include "TutorialGameMode.h"


//////////////////////////////////////////////////////////////////////////
// Hero
//////////////////////////////////////////////////////////////////////////

void UUIFunctionLibrary::GetHeroIconInfo(FHERO_ICON_INFO& HeroIconInfo, int32 KID, const FString& HeroUD)
{
	HeroIconInfo.heroUD = HeroUD;

//	UWorld* World = GEngine->GetWorldFromContextObject(WorldContextObject, EGetWorldErrorMode::Assert);
	auto RGameInstance = RGAMEINSTANCE(GEngine);
	checkf(RGameInstance != nullptr, TEXT("GetHeroIconInfo[ RGameInstance is Empty]"));


	if (RGameInstance->HttpClient->IsValidKID(KID))
	{
		FHERO* HeroData = RGameInstance->RInventory->GetHeroWithUD(HeroUD);
		if (HeroData != nullptr)
		{
			HeroIconInfo.heroId = HeroData->heroId;
			HeroIconInfo.heroUD = HeroData->heroUD;
//			HeroIconInfo.grade = HeroData->grade;
			HeroIconInfo.level = HeroData->level;

			
			UCharacterBattleInfo* CharacterBattleInfo = URenewal_BattleFunctionLibrary::MakeCharacteBattleInfoFromInven(HeroUD);
			if (CharacterBattleInfo)
			{
				HeroIconInfo.MaxHP = CharacterBattleInfo->GetCharacterFinalProperty<float>(ECharacterPropertyTypeEnum::VE_MaxHP);
				CharacterBattleInfo = nullptr;
			}
		}
	}
	else
	{
		FEQUIPED_HERO DeckHeroData;
		if (RGameInstance->OtherHeroManager->GetOtherHeroData(KID, HeroUD, DeckHeroData))
		{
			HeroIconInfo.heroId = DeckHeroData.heroId;
			HeroIconInfo.heroUD = DeckHeroData.heroUD;
			//HeroIconInfo.grade = DeckHeroData.grade;
			HeroIconInfo.level = DeckHeroData.level;

			HeroIconInfo.MaxHP = 0;
			UCharacterBattleInfo* CharacterBattleInfo = URenewal_BattleFunctionLibrary::MakeCharacteBattleInfoFromEquipedHero(DeckHeroData);
			if (CharacterBattleInfo)
				HeroIconInfo.MaxHP = CharacterBattleInfo->GetCharacterFinalProperty<float>(ECharacterPropertyTypeEnum::VE_MaxHP);
		}
	}
	
	HeroIconInfo.PortraitTextureRef = GetCharacterPortrait(ECharacterCategoryTypeEnum::VE_Hero, HeroIconInfo.heroId);

}

void UUIFunctionLibrary::GetHeroIconInfoWithUD(FHERO_ICON_INFO& HeroIconInfo, const FString& HeroUD, EHeroOwnershipType OwnerShip)
{
	HeroIconInfo.heroUD = HeroUD;
	HeroIconInfo.bIsOwned = false;
	auto RGameInstance = RGAMEINSTANCE(GEngine);
	checkf(RGameInstance != nullptr, TEXT("GetHeroIconInfoWithUD[ RGameInstance is Empty]"));

	switch (OwnerShip)
	{
		case EHeroOwnershipType::VE_None:
		{
			HeroIconInfo.heroUD = 0;
			HeroIconInfo.grade = 1;
			HeroIconInfo.level = 1;
			HeroIconInfo.exp = 0;
			HeroIconInfo.bIsOwned = false;
			HeroIconInfo.bLock = false;
		}
		break;
		case EHeroOwnershipType::VE_Me:
		{
			FHERO* HeroData = RGameInstance->RInventory->GetHeroWithUD(HeroUD);
			if (HeroData != nullptr)
			{
				HeroIconInfo.heroId = HeroData->heroId;
				HeroIconInfo.heroUD = HeroData->heroUD;
				HeroIconInfo.level = HeroData->level;
				HeroIconInfo.bLock = HeroData->locked;
				HeroIconInfo.exp = HeroData->exp;
				int32 EnchantCount = 0;
				for (int32 Num : HeroData->enchantStat)
					EnchantCount += Num;
				HeroIconInfo.enchantPoint = EnchantCount;
				HeroIconInfo.MaxHP = 0;
				UCharacterBattleInfo* CharacterBattleInfo = URenewal_BattleFunctionLibrary::MakeCharacteBattleInfoFromInven(HeroUD);
				if (CharacterBattleInfo)
				{
					HeroIconInfo.MaxHP = CharacterBattleInfo->GetCharacterFinalProperty<float>(ECharacterPropertyTypeEnum::VE_MaxHP);
					CharacterBattleInfo = nullptr;
				}
				
			}
		}
		break;
		case EHeroOwnershipType::VE_Arena:
		{
			UArenaManager* pArenaManager = RGameInstance->ArenaManager;
			TArray<FString>& rMyHeroUDs = pArenaManager->m_ArenaMyHeroUDs;
			TArray<FString>& rEnemyHeroUDs = pArenaManager->m_MatchList.decks.defense;
			FEQUIPED_HERO HeroInfo;

			for(int32 n = 0; n < rEnemyHeroUDs.Num(); ++n)
			{
				if (rEnemyHeroUDs.IsValidIndex(n) && rEnemyHeroUDs[n] == HeroUD)
				{
					if (RGameInstance->OtherHeroManager->GetOtherHeroData(pArenaManager->m_MatchList.kId, pArenaManager->m_MatchList.decks.defense[n], HeroInfo))
					{
						HeroIconInfo.heroUD = HeroInfo.heroUD;
						HeroIconInfo.heroId = HeroInfo.heroId;
						HeroIconInfo.level = HeroInfo.level;
					}

				}
			}
		}
		break;
		case EHeroOwnershipType::VE_Mirror:
		{
			UMirrorDungeonManager* pMirrorManager = RGameInstance->MirrorDungeonManager;
			TArray<FString>& rMyHeroUDs = pMirrorManager->m_MirrorMyHeroUDs;
			TArray<FString>& rEnemyHeroUDs = pMirrorManager->m_MatchList.decks.defense;
			FEQUIPED_HERO HeroInfo;

			for (int32 n = 0; n < rEnemyHeroUDs.Num(); ++n)
			{
				if (rEnemyHeroUDs.IsValidIndex(n) && rEnemyHeroUDs[n] == HeroUD)
				{
					if (RGameInstance->OtherHeroManager->GetOtherHeroData(pMirrorManager->m_MatchList.kId, rEnemyHeroUDs[n], HeroInfo))
					{
						HeroIconInfo.heroUD = HeroInfo.heroUD;
						HeroIconInfo.heroId = HeroInfo.heroId;
						HeroIconInfo.level = HeroInfo.level;

						HeroIconInfo.MaxHP = 0;
						UCharacterBattleInfo* CharacterBattleInfo = URenewal_BattleFunctionLibrary::MakeCharacteBattleInfoFromEquipedHero(HeroInfo);
						if (CharacterBattleInfo)
						{
							HeroIconInfo.MaxHP = CharacterBattleInfo->GetCharacterFinalProperty<float>(ECharacterPropertyTypeEnum::VE_MaxHP);
							CharacterBattleInfo = nullptr;
						}
					}
				}
			}
		}
		break;
		case EHeroOwnershipType::VE_Colosseum:
		{
			for (auto Actor : RGameInstance->RealTimeModManager->ActorList)
			{
				if (Actor.Value.heroUD == HeroUD)
				{
					HeroIconInfo.heroUD = Actor.Value.heroUD;
					HeroIconInfo.heroId = Actor.Value.actorId;
					HeroIconInfo.level = Actor.Value.level;
				}
			}
		}
		break;
	}

	HeroIconInfo.PortraitTextureRef = GetCharacterPortrait(ECharacterCategoryTypeEnum::VE_Hero, HeroIconInfo.heroId);
	
	FCharacterTableInfo* CharacterTableInfo = UTableManager::GetInstancePtr()->GetCharacterRow(HeroIconInfo.heroId);
	checkf(CharacterTableInfo != nullptr, TEXT("GetHeroIconInfoWithUD[ CharacterTableInfo is Empty]"));
// 	if (false == CharacterTableInfo->IsHero)
// 		return;
	HeroIconInfo.bSpirit = !CharacterTableInfo->IsHero;
	HeroIconInfo.grade = CharacterTableInfo->Grade;
}

void UUIFunctionLibrary::GetHeroIconInfoWithID(FHERO_ICON_INFO& HeroIconInfo, const FName& HeroID, EHeroOwnershipType OwnerShip, int32 kId)
{
	HeroIconInfo.heroId = HeroID;
	HeroIconInfo.bIsOwned = false;

	auto RGameInstance = RGAMEINSTANCE(GEngine);
	checkf(RGameInstance != nullptr, TEXT("GetHeroIconInfoWithID[ RGameInstance is Empty]"));

	switch (OwnerShip)
	{
		case EHeroOwnershipType::VE_None:
		{
			HeroIconInfo.heroUD = 0;
			HeroIconInfo.grade = 1;
			HeroIconInfo.level = 1;
			HeroIconInfo.exp = 0;
			HeroIconInfo.bLock = false;
		}
		break;
		case EHeroOwnershipType::VE_Boss:
		{
			HeroIconInfo.heroUD = 0;
			HeroIconInfo.grade = 1;
			//HeroIconInfo.level = 150;
			HeroIconInfo.exp = 0;
			HeroIconInfo.bLock = false;
		}
			break;
		case EHeroOwnershipType::VE_Me:
		{
			FHERO* HeroData = RGameInstance->RInventory->GetHeroWithID(HeroID);
			if (HeroData != nullptr)
			{
				HeroIconInfo.heroUD = HeroData->heroUD;
				//HeroIconInfo.grade = HeroData->grade;
				HeroIconInfo.level = HeroData->level;
				HeroIconInfo.exp = HeroData->exp;
				HeroIconInfo.bIsOwned = true;
				HeroIconInfo.bLock = HeroData->locked;

				HeroIconInfo.MaxHP = 0;
				UCharacterBattleInfo* CharacterBattleInfo = URenewal_BattleFunctionLibrary::MakeCharacteBattleInfoFromInven(HeroData->heroUD);
				if (CharacterBattleInfo)
				{
					HeroIconInfo.MaxHP = CharacterBattleInfo->GetCharacterFinalProperty<float>(ECharacterPropertyTypeEnum::VE_MaxHP);
					CharacterBattleInfo = nullptr;
				}

			}
		}
		break;
	}
	
	HeroIconInfo.PortraitTextureRef = GetCharacterPortrait(ECharacterCategoryTypeEnum::VE_Hero, HeroIconInfo.heroId);

	FCharacterTableInfo* CharacterTableInfo = UTableManager::GetInstancePtr()->GetCharacterRow(HeroIconInfo.heroId);
	checkf(CharacterTableInfo != nullptr, TEXT("GetHeroIconInfoWithUD[ CharacterTableInfo is Empty]"));
	if (false == CharacterTableInfo->IsHero)
		return;

	HeroIconInfo.grade = CharacterTableInfo->Grade;

}

void UUIFunctionLibrary::GetHeroIconInfoForNpc(FHERO_ICON_INFO& HeroIconInfo, const FName& NpcID)
{
	HeroIconInfo.heroId = NpcID;

	HeroIconInfo.heroUD = 0;
	HeroIconInfo.grade = 1;
	HeroIconInfo.level = 1;
	HeroIconInfo.exp = 0;
			
	HeroIconInfo.PortraitTextureRef = GetCharacterPortrait(ECharacterCategoryTypeEnum::VE_Npc, NpcID);

	HeroIconInfo.ToolTipEventType	= EToolTipEventTypeEnum::VE_Npc;
	HeroIconInfo.ToolTipID			= NpcID;
}

UObject* UUIFunctionLibrary::GetCharacterPortrait(ECharacterCategoryTypeEnum CharacterCategory, FName RowName)
{
	FCharacterTableInfo* CharacterTableInfo = URenewal_BattleFunctionLibrary::GetCharacterTableInfo(CharacterCategory, RowName);
	if (CharacterTableInfo)
	{
		return GetIconImageFromTable(CharacterTableInfo->PortraitTextureKey);
	}

	return nullptr;
}

UObject* UUIFunctionLibrary::GetHeroRoundPortrait(FName HeroID)
{
	FCharacterTableInfo* CharacterTableInfo = URenewal_BattleFunctionLibrary::GetCharacterTableInfo(ECharacterCategoryTypeEnum::VE_Hero, HeroID);
	if (CharacterTableInfo)
	{
		FString strTexKey = CharacterTableInfo->PortraitTextureKey.ToString();
		int32 Index = strTexKey.Find("_", ESearchCase::IgnoreCase, ESearchDir::FromEnd);
		Index = strTexKey.Len() - Index;
		//strTexKey = "PT_HUD" + strTexKey.Right(Index);
		strTexKey = "PT_HUD_" + CharacterTableInfo->DisplayName.ToString();
		return GetIconImageFromTable(FName(*strTexKey));
	}

	return nullptr;
}

// 마름모 초상화
UObject* UUIFunctionLibrary::GetHeroRhombusPortrait(FName HeroID)
{
	FCharacterTableInfo* CharacterTableInfo = URenewal_BattleFunctionLibrary::GetCharacterTableInfo(ECharacterCategoryTypeEnum::VE_Hero, HeroID);
	if(!CharacterTableInfo)
		CharacterTableInfo = URenewal_BattleFunctionLibrary::GetCharacterTableInfo(ECharacterCategoryTypeEnum::VE_Npc, HeroID);

	if (CharacterTableInfo)
	{
		FString strTexKey = CharacterTableInfo->PortraitTextureKey.ToString();
		int32 Index = strTexKey.Find("_", ESearchCase::IgnoreCase, ESearchDir::FromEnd);
		Index = strTexKey.Len() - Index;
		strTexKey = "PT_RHOMBUS_HUD" + strTexKey.Right(Index);
		return GetIconImageFromTable(FName(*strTexKey));
	}
	return nullptr;
}

UObject* UUIFunctionLibrary::GetIconImageFromTable(FName RowName)
{
	UPaperSprite* SpriteIcon = UUtilFunctionLibrary::GetSpriteFromTable(RowName);
	if (SpriteIcon)
		return SpriteIcon;

	return UUtilFunctionLibrary::GetTexture2DFromTable(RowName);
	//return nullptr;
}

UPaperSprite* UUIFunctionLibrary::GetCharacterPortraitSprite(ECharacterCategoryTypeEnum CharacterCategory, FName RowName)
{
	FCharacterTableInfo* CharacterTableInfo = URenewal_BattleFunctionLibrary::GetCharacterTableInfo(CharacterCategory, RowName);
	if (CharacterTableInfo)
	{
		return UUtilFunctionLibrary::GetSpriteFromTable(CharacterTableInfo->PortraitTextureKey);
	}

	return nullptr;
}

bool UUIFunctionLibrary::SetImageBrush(UObject* InBrushResource, UImage* InImage)
{
	if (IsValid(InBrushResource) == false)
		return false;
	if (IsValid(InImage) == false)
		return false;

	InImage->Brush.SetResourceObject(InBrushResource);

	UTexture2D* Texture2D = Cast< UTexture2D >(InBrushResource);
	if (Texture2D) // Since this texture is used as UI, don't allow it affected by budget.
	{
		Texture2D->bIgnoreStreamingMipBias = true;
	}

// 	if (InTexture)
// 	{
// 		Brush.ImageSize.X = InTexture->GetSizeX();
// 		Brush.ImageSize.Y = InTexture->GetSizeY();
// 	}

	InImage->SetBrush(InImage->Brush);

	return true;
}

bool UUIFunctionLibrary::SetImageBrushFromTable(FName RowName, UImage* InImage)
{
	return SetImageBrush(GetIconImageFromTable(RowName), InImage);
}

void UUIFunctionLibrary::GetHeroIconInfoByEquipedHero(FHERO_ICON_INFO& HeroIconInfo, FEQUIPED_HERO EquipedHero)
{
	auto RGameInstance = RGAMEINSTANCE(GEngine);
	checkf(RGameInstance != nullptr, TEXT("GetHeroIconInfoByEquipedHero[ RGameInstance is Empty]"));
	FCharacterTableInfo* CharacterTableInfo = UTableManager::GetInstancePtr()->GetCharacterRow(EquipedHero.heroId);
	checkf(CharacterTableInfo != nullptr, TEXT("GetHeroIconInfoWithUD[ CharacterTableInfo is Empty]"));

	HeroIconInfo.heroUD = EquipedHero.heroUD;
	HeroIconInfo.heroId = EquipedHero.heroId;
	HeroIconInfo.level = EquipedHero.level;
	HeroIconInfo.exp = 0;
	HeroIconInfo.PortraitTextureRef = GetCharacterPortrait(ECharacterCategoryTypeEnum::VE_Hero, HeroIconInfo.heroId);
	HeroIconInfo.grade = CharacterTableInfo->Grade;

	if (false == CharacterTableInfo->IsHero)
		return;

	HeroIconInfo.MaxHP = 0;
	UCharacterBattleInfo* CharacterBattleInfo = URenewal_BattleFunctionLibrary::MakeCharacteBattleInfoFromEquipedHero(EquipedHero);
	if (CharacterBattleInfo)
	{
		HeroIconInfo.MaxHP = CharacterBattleInfo->GetCharacterFinalProperty<float>(ECharacterPropertyTypeEnum::VE_MaxHP);
		CharacterBattleInfo = nullptr;
	}

	HeroIconInfo.ToolTipEventType = EToolTipEventTypeEnum::VE_Hero;
	HeroIconInfo.ToolTipID = HeroIconInfo.heroId;
}

void UUIFunctionLibrary::ClearHeroIconIfo(FHERO_ICON_INFO& HeroIconInfo)
{
	HeroIconInfo.heroUD.Empty();
	HeroIconInfo.heroId = FName("");

	HeroIconInfo.grade = 0;
	HeroIconInfo.level = 0;
	HeroIconInfo.exp = 0;
	HeroIconInfo.acquiredExp = 0;

	HeroIconInfo.MaxExp = 1;
	HeroIconInfo.MaxHP = 1;
	HeroIconInfo.PortraitTextureRef = nullptr;
}

FText UUIFunctionLibrary::GetCharacterName(AActor* InActor)
{
	IEntityBaseProperty* CharacterProperty = Cast< IEntityBaseProperty >(InActor);
	if (CharacterProperty)
	{
		FName CharacterID = CharacterProperty->GetCharacterID();
		FText NameText;
		FCharacterTableInfo* CharacterTableInfo = URenewal_BattleFunctionLibrary::GetCharacterTableInfo(CharacterProperty->GetCharacterCategory(), CharacterID);
		if (CharacterTableInfo)
		{
			switch (CharacterProperty->GetCharacterCategory())
			{
			case ECharacterCategoryTypeEnum::VE_Hero:
				NameText = UUtilFunctionLibrary::GetLocalizedString(EStringTableType::VE_Hero, CharacterTableInfo->DisplayName);
				break;
			case ECharacterCategoryTypeEnum::VE_Npc:
			case ECharacterCategoryTypeEnum::VE_Minion:
			case ECharacterCategoryTypeEnum::VE_NeutralNpc:
			case ECharacterCategoryTypeEnum::VE_BattleObject:
				NameText = UUtilFunctionLibrary::GetLocalizedString(EStringTableType::VE_Npc, CharacterTableInfo->DisplayName);
				break;			
			}
		}

		return NameText;
	}
	
	return FText();
}

FText UUIFunctionLibrary::GetCharacterName(ECharacterCategoryTypeEnum CategoryType, FName CharacterID)
{
	FText NameText;
	FCharacterTableInfo* CharacterTableInfo = URenewal_BattleFunctionLibrary::GetCharacterTableInfo(CategoryType, CharacterID);
	if (CharacterTableInfo)
	{
		switch (CategoryType)
		{
		case ECharacterCategoryTypeEnum::VE_Hero:
			NameText = UUtilFunctionLibrary::GetLocalizedString(EStringTableType::VE_Hero, CharacterTableInfo->DisplayName);
			break;
		case ECharacterCategoryTypeEnum::VE_Npc:
		case ECharacterCategoryTypeEnum::VE_Minion:
		case ECharacterCategoryTypeEnum::VE_NeutralNpc:
		case ECharacterCategoryTypeEnum::VE_BattleObject:
			NameText = UUtilFunctionLibrary::GetLocalizedString(EStringTableType::VE_Npc, CharacterTableInfo->DisplayName);
			break;
		}
	}
	return NameText;
}

FText UUIFunctionLibrary::GetNpcName(FName InNpcID)
{
	FText NameText;
	FCharacterTableInfo* NpcTableInfo = URenewal_BattleFunctionLibrary::GetCharacterTableInfo( ECharacterCategoryTypeEnum::VE_Npc, InNpcID );
	if (NpcTableInfo)
	{
		NameText = UUtilFunctionLibrary::GetLocalizedString(EStringTableType::VE_Npc, NpcTableInfo->DisplayName);
	}

	return NameText;
}

bool UUIFunctionLibrary::IsBossNpc(FName InNpcID)
{
	FCharacterTableInfo* NpcTableInfo = URenewal_BattleFunctionLibrary::GetCharacterTableInfo(ECharacterCategoryTypeEnum::VE_Npc, InNpcID);
	if (NpcTableInfo)
	{
		//	NpcTableInfo->MonsterGrade
		//	VE_Summon	1
		//	VE_Normal	2
		//	VE_Elite	3
		//	VE_Epic		4
		//	VE_Boss		5


		switch (NpcTableInfo->Grade)
		{
		case 4:
		case 5:
			return true;

		default:
			break;
		}
	}

	return false;
}

bool UUIFunctionLibrary::IsOwnedTargetHeroByHeroUD(UWorld* InWorld, const FString& InHeroUD)
{
	//return RGameInstance->HeroInventory->IsActivatedHero();
	if (InWorld == nullptr)
	{
		return false;
	}

	auto RGameInstance = RGAMEINSTANCE(GEngine);
	if (RGameInstance == nullptr)
		return false;
	if (nullptr == RGameInstance->RInventory->GetHeroWithUD(InHeroUD))
		return false;
	return true;
}

void UUIFunctionLibrary::SetFilterCheckCondition(UUC_Hero_Filter_Check* checkFilter)
{
	//UE_LOG(LogClass, Log, TEXT("%d"), RGAMEINSTANCE(this)->RInventory->classFilter.Num());
	TArray<EClassTypeEnum> tClassFilter = RGAMEINSTANCE(GEngine)->RInventory->classFilter;
	TArray<int32> tGradeFilter = RGAMEINSTANCE(GEngine)->RInventory->gradeFilter;
	TArray<EContinentTypeEnum> tContinentFilter = RGAMEINSTANCE(GEngine)->RInventory->continentFilter;

	for (EClassTypeEnum classType : tClassFilter)
	{
		if (classType == EClassTypeEnum::VE_Tanker)
			checkFilter->CheckBox_Tanker->SetIsChecked(true);
		else if (classType == EClassTypeEnum::VE_MeleeDealer)
			checkFilter->CheckBox_MeleeDealer->SetIsChecked(true);
		else if (classType == EClassTypeEnum::VE_RangeDealer)
			checkFilter->CheckBox_RangeDealer->SetIsChecked(true);
		else if (classType == EClassTypeEnum::VE_MagicDealer)
			checkFilter->CheckBox_MagicDealer->SetIsChecked(true);
		else if (classType == EClassTypeEnum::VE_Supporter)
			checkFilter->CheckBox_Supporter->SetIsChecked(true);
	}

	for (int32 gradeType : tGradeFilter)
	{
		if (gradeType == 1)
			checkFilter->CheckBox_Grade_One->SetIsChecked(true);
		else if (gradeType == 2)
			checkFilter->CheckBox_Grade_Two->SetIsChecked(true);
		else if (gradeType == 3)
			checkFilter->CheckBox_Grade_Three->SetIsChecked(true);
		else if (gradeType == 4)
			checkFilter->CheckBox_Grade_Four->SetIsChecked(true);
		else if (gradeType == 5)
			checkFilter->CheckBox_Grade_Five->SetIsChecked(true);
		else if (gradeType == 6)
			checkFilter->CheckBox_Grade_Six->SetIsChecked(true);
		else if (gradeType == 7)
			checkFilter->CheckBox_Grade_Seven->SetIsChecked(true);
		else if (gradeType == 8)
			checkFilter->CheckBox_Grade_Eight->SetIsChecked(true);
		else if (gradeType == 9)
			checkFilter->CheckBox_Grade_Nine->SetIsChecked(true);
		else if (gradeType == 10)
			checkFilter->CheckBox_Grade_Ten->SetIsChecked(true);
	}

	for (EContinentTypeEnum continentType : tContinentFilter)
	{
		if (continentType == EContinentTypeEnum::Lux)
			checkFilter->CheckBox_Lux->SetIsChecked(true);
		else if (continentType == EContinentTypeEnum::Solona)
			checkFilter->CheckBox_Solona->SetIsChecked(true);
		else if (continentType == EContinentTypeEnum::Luna)
			checkFilter->CheckBox_Runa->SetIsChecked(true);
		else if (continentType == EContinentTypeEnum::Nox)
			checkFilter->CheckBox_Nox->SetIsChecked(true);
	}
}


//////////////////////////////////////////////////////////////////////////
// Item
//////////////////////////////////////////////////////////////////////////
bool UUIFunctionLibrary::GetItemIconInfoWithUD(FITEM_ICON_INFO& ItemIconInfo, const FString& ItemUD)
{
	auto RGameInstance = RGAMEINSTANCE(GEngine);
	FITEM* ItemData = RGameInstance->RInventory->GetItemDataWithUD(ItemUD);
	if (ItemData == nullptr)
		return false;

	//auto ItemTableInfo = UTableManager::GetInstancePtr()->GetCharacterItemRow(ItemData->itemId);

	ItemIconInfoSetServerData(ItemIconInfo, *ItemData);
	//ItemIconInfoSetTableData(ItemIconInfo, *ItemTableInfo);

	return true;
	/*
	FWorkedItemTableInfo ItemTableInfo;
	FITEM ItemData;
	if (RGameInstance->ItemInventory->GetInventoryDataAndTableData(ItemUD, ItemData, ItemTableInfo))
	{
		ItemIconInfoSetServerData(ItemIconInfo, ItemData);
		ItemIconInfoSetTableData(ItemIconInfo, ItemTableInfo.Data);
		return true;
	}
	
	return false;
	*/
}

bool UUIFunctionLibrary::GetItemIconInfoWithID(FITEM_ICON_INFO& ItemIconInfo, const FName& ItemID)
{
	const auto ItemTableInfo = UTableManager::GetInstancePtr()->GetCharacterItemRow(ItemID);
	checkf(ItemTableInfo != nullptr, TEXT("%s[ ItemTableInfo Is Empty ]"), __FUNCTION__);
	
	ItemIconInfo.itemId = ItemID;

	ItemIconInfo.enchant = 0;
	ItemIconInfo.tuneCnt = 0;
	ItemIconInfo.count = 1;
	ItemIconInfo.bEquiped = false;		
	ItemIconInfo.bCompare = false;

	ItemIconInfo.ToolTipEventType = EToolTipEventTypeEnum::VE_Item;
	ItemIconInfo.ToolTipID = ItemID;
	ItemIconInfoSetTableData(ItemIconInfo, *ItemTableInfo);
	return true;
	
}

bool UUIFunctionLibrary::GetItemPreviewIconInfoWithID( FITEM_ICON_INFO& ItemIconInfo, const FName& ItemID)
{
	const auto ItemTableInfo = UTableManager::GetInstancePtr()->GetRewardPreviewItemRow(ItemID);
	if (ItemTableInfo)
	{
		ItemIconInfo.itemId = ItemID;

		ItemIconInfo.enchant = 0;
		ItemIconInfo.tuneCnt = 0;
		ItemIconInfo.count = 1;
		ItemIconInfo.bEquiped = false;		
		ItemIconInfo.bCompare = false;

		ItemIconInfo.itemGrade = EItemGradeEnum::Normal;
		ItemIconInfo.ItemIconRef = UUIFunctionLibrary::GetIconImageFromTable(ItemTableInfo->ItemIconKey);
		ItemIconInfo.ItemEquipType = EItemEquipTypeEnum::VE_None;
		ItemIconInfo.ItemEquipPosition = EItemEquipPositionEnum::VE_Weapon;		
		
		ItemIconInfo.ToolTipEventType = EToolTipEventTypeEnum::VE_PreviewItem;
		ItemIconInfo.ToolTipID = ItemID;
		return true;
	}

	return false;
}

void UUIFunctionLibrary::ItemIconInfoSetServerData(FITEM_ICON_INFO& ItemIconInfo, FITEM& ItemData)
{
	ItemIconInfo.itemUD = ItemData.itemUD;
	ItemIconInfo.itemId = ItemData.itemId;

	ItemIconInfo.enchant = ItemData.enchant;
//	ItemIconInfo.tuneCnt = ItemData.tuneCnt;
//	ItemIconInfo.count = ItemData.stack;
	ItemIconInfo.bEquiped = (ItemData.equipedHeroUD.IsEmpty() ? false : true);	
	
	auto ItemTableInfo = UTableManager::GetInstancePtr()->GetCharacterItemRow(ItemData.itemId);
	if (ItemTableInfo != nullptr)
	{
		ItemIconInfoSetTableData(ItemIconInfo, *ItemTableInfo);
	}
}

void UUIFunctionLibrary::ItemIconInfoSetTableData(FITEM_ICON_INFO& ItemIconInfo, FItemTableInfo& ItemTableInfo)
{
	ItemIconInfo.itemGrade = ItemTableInfo.ItemGrade;
	ItemIconInfo.ItemIconRef = UUtilFunctionLibrary::GetTexture2DFromTable( ItemTableInfo.ItemIcon );
	ItemIconInfo.ItemEquipType = ItemTableInfo.EquipType;
	ItemIconInfo.ItemEquipPosition = ItemTableInfo.EquipPosition;	
	ItemIconInfo.bCompare = false;
}

void UUIFunctionLibrary::ClearItemIconIfo(FITEM_ICON_INFO& ItemIconInfo)
{
	ItemIconInfo.itemUD.Empty();
	ItemIconInfo.itemId = FName("");

	ItemIconInfo.enchant = 0;
	ItemIconInfo.tuneCnt = 0;
	ItemIconInfo.count = 0;
	ItemIconInfo.bEquiped = false;
	ItemIconInfo.bCompare = false;

	ItemIconInfo.itemGrade = EItemGradeEnum::Normal;
	ItemIconInfo.ItemIconRef = nullptr;
	ItemIconInfo.ItemEquipType = EItemEquipTypeEnum::VE_None;
	ItemIconInfo.ItemEquipPosition = EItemEquipPositionEnum::VE_Weapon;
}

// TODO: no more encoding into index.
int32 UUIFunctionLibrary::GetItemEnchantRowName(EItemEquipTypeEnum InEquipType, EItemEquipPositionEnum InEquipPoisition, EItemGradeEnum InGrade, int32 InEnchantLevel)
{
	int32 EquipType		= static_cast<int32>(InEquipType);
	int32 EquipPosition	= static_cast<int32>(InEquipPoisition);
	int32 Grade			= static_cast<int32>(InGrade);
	int32 EnchantLevel	= static_cast<int32>(InEnchantLevel);

	if (UUIFunctionLibrary::IsEquipTypeArmor(InEquipType))
	{
		EquipType = 0;
	}

	//int32 RowNameInItemEnchantTable = EquipType * 10000 + EquipPosition * 1000 + Grade * 100 + EnchantLevel;
	int32 RowNameInItemEnchantTable = (EquipPosition + 1) * 1000 + (Grade + 1) * 100 + EnchantLevel;
	
	return RowNameInItemEnchantTable;
}

UObject* UUIFunctionLibrary::GetItemEmptyTextureByEquipPosition(EItemEquipPositionEnum InItemEquipPosition)
{
	switch (InItemEquipPosition)
	{
	case EItemEquipPositionEnum::VE_Max:
		return GetIconImageFromTable(FName(TEXT("Icon_UniqueEmpty")));
	case EItemEquipPositionEnum::VE_Weapon:
		return GetIconImageFromTable(FName(TEXT("Icon_WeaponEmpty")));
	case EItemEquipPositionEnum::VE_Armor:
		return GetIconImageFromTable(FName(TEXT("Icon_ArmorEmpty")));
	case EItemEquipPositionEnum::VE_Pants:
		return GetIconImageFromTable(FName(TEXT("Icon_PantsEmpty")));
	case EItemEquipPositionEnum::VE_Necklace:
		return GetIconImageFromTable(FName(TEXT("Icon_NecklaceEmpty")));
	case EItemEquipPositionEnum::VE_Ring:
		return GetIconImageFromTable(FName(TEXT("Icon_RingEmpty")));
	default:
		return nullptr;
	}

	return nullptr;
}

FName UUIFunctionLibrary::GetOptionName(FString& InValue)
{
	//URenewal_BattleFunctionLibrary::GetCharacterPropertyType - pair

	FName Result;

	if		(InValue == "AccuracyValue")  Result = "S_GOPT00";
	else if (InValue == "CriticalValue")  Result = "S_GOPT01";
	else if (InValue == "CriticalDamageRate_Scale")  Result = "S_GOPT02";
	//else if (InValue == "None")  Result = "S_GOPT03";
	else if (InValue == "MaxHP")  Result = "S_GOPT04";
	//else if (InValue == "None")  Result = "S_GOPT05";
	else if (InValue == "DodgeValue")  Result = "S_GOPT06";
	else if (InValue == "Armor")  Result = "S_GOPT07";
	//else if (InValue == "None")  Result = "S_GOPT08";
	else if (InValue == "ParryValue")  Result = "S_GOPT09";
	else if (InValue == "MagicResistance")  Result = "S_GOPT10";
	//else if (InValue == "None")  Result = "S_GOPT11";
	else if (InValue == "PierceValue")  Result = "S_GOPT12";
	else if (InValue == "MagicAttackPower")  Result = "S_GOPT13";
	else if (InValue == "PhysicalAttackPower")  Result = "S_GOPT14";
	else if (InValue == "MagicCriticalRate")  Result = "S_GOPT15";
	else if (InValue == "MoveSpeed")  Result = "S_GOPT16";
	//else if (InValue == "None")  Result = "S_GOPT17";
	else if (InValue == "CriticalResistValue")  Result = "S_GOPT18";
	else if (InValue == "CriticalResistDamage")  Result = "S_GOPT19";

	return Result;
}

bool UUIFunctionLibrary::IsEquipTypeArmor(EItemEquipTypeEnum EquipType)
{
	switch (EquipType)
	{
	case EItemEquipTypeEnum::VE_HeavyArmor:
	case EItemEquipTypeEnum::VE_LightArmor:
	case EItemEquipTypeEnum::VE_FabricArmor:
	case EItemEquipTypeEnum::VE_HeavyPants:
	case EItemEquipTypeEnum::VE_LightPants:
	case EItemEquipTypeEnum::VE_FabricPants:
		return true;
		break;
	}

	return false;
}

// Dubious execution
FName UUIFunctionLibrary::GetItemGradeName(EItemGradeEnum ItemGrade)
{
	switch (ItemGrade)
	{
	case EItemGradeEnum::Normal:
		return FName(TEXT("Normal"));
		break;
	case EItemGradeEnum::Rare:
		return FName(TEXT("Rare"));
		break;
	case EItemGradeEnum::Epic:
		return FName(TEXT("Epic"));
		break;
	case EItemGradeEnum::Unique:
		return FName(TEXT("Unique"));
		break;
	case EItemGradeEnum::Tower:
		return FName(TEXT("Tower"));
		break;
	}

	// unreachable
	return FName(TEXT("Normal"));
}

FName UUIFunctionLibrary::ItemStatOptionToStringKey(EItemStatOption ItemStatOption)
{
	switch (ItemStatOption)
	{
	case EItemStatOption::PhysicalAttackPower:
		return FName(TEXT("S_GOPT14"));
		break;
	case EItemStatOption::MagicAttackPower:
		return FName(TEXT("S_GOPT13"));
		break;
	case EItemStatOption::Armor:
		return FName(TEXT("S_GOPT07"));
		break;
	case EItemStatOption::MagicResistance:
		return FName(TEXT("S_GOPT10"));
		break;
	case EItemStatOption::MaxHP:
		return FName(TEXT("S_GOPT04"));
		break;
	case EItemStatOption::AttackRange:
		return NAME_None; // TODO
		break;
	case EItemStatOption::MoveSpeed:
		return FName(TEXT("S_GOPT16"));
		break;
	case EItemStatOption::AttackSpeed:
		return FName(TEXT("S_GOPT17"));
		break;
	case EItemStatOption::DodgeValue:
		return FName(TEXT("S_GOPT06"));
		break;
	case EItemStatOption::CriticalValue:
		return FName(TEXT("S_GOPT01"));
		break;
	case EItemStatOption::CriticalDamageRate_Scale:
		return FName(TEXT("S_GOPT02"));
		break;
	case EItemStatOption::CriticalResistValue:
		return FName(TEXT("S_GOPT18"));
		break;
	case EItemStatOption::PierceValue:
		return FName(TEXT("S_GOPT12"));
		break;
	case EItemStatOption::AccuracyValue:
		return FName(TEXT("S_GOPT00"));
		break;
	default:
		UE_LOG(LogUI, Log, TEXT("UUIFunctionLibrary::ItemStatOptionToStringKey: fix me"));
		return NAME_None;
	}
}


//////////////////////////////////////////////////////////////////////////
// Text
//////////////////////////////////////////////////////////////////////////
FText UUIFunctionLibrary::GetSkillDescTextHeroID(FName InHeroID, FName InSkillName, int32 InLevel, bool bIsCondition, bool bIsRich)
{
	FText SkillDesc = FText::GetEmpty();

	auto RGameInstance = RGAMEINSTANCE(GEngine);
	checkf(RGameInstance->RInventory != nullptr, TEXT("%s[ RInventory Is Empty ]"), __FUNCTION__);
	
	FHERO* outData = RGameInstance->RInventory->GetHeroWithID(InHeroID);
	checkf(outData != nullptr, TEXT("%s[ outData Is Empty ]"), __FUNCTION__);
	FWorkedSkillTableInfo SkillInfo;
	if (URenewal_BattleFunctionLibrary::GetSkillTableWorkedInfo(SkillInfo, ECharacterCategoryTypeEnum::VE_Hero, InSkillName, InLevel))
	{
		SkillDesc = GetSkillDescText(SkillInfo, bIsCondition, bIsRich);
	}

	return SkillDesc;
}

FText UUIFunctionLibrary::GetItemPassiveSkillDescText(FName InSkillName)
{
	FWorkedSkillTableInfo SkillInfo; 
	SkillInfo.SkillTableInfo_V2 = UTableManager::GetInstancePtr()->GetItemPassiveSkillRow(InSkillName);

	if (SkillInfo.SkillTableInfo_V2 == nullptr)
		return FText::FromString("");

	return GetSkillDescText(SkillInfo, false, false);
}

FText UUIFunctionLibrary::GetSkillDescText(const FWorkedSkillTableInfo& InWorkedSkillInfo, bool bIsCondition, bool bIsRich)
{
	FText SkillDesc = FText::GetEmpty();

	if (InWorkedSkillInfo.SkillTableInfo_V2->Description.IsNone() == false)
	{
		SkillDesc = UUtilFunctionLibrary::GetLocalizedString(EStringTableType::VE_Skill, InWorkedSkillInfo.SkillTableInfo_V2->Description);
	}


	if (SkillDesc.IsEmpty() == false)
	{
		//{_} {sec_} ... this make new value.
		TArray<FString> CalculateStrings;
		CalculateStrings.Emplace(UNIT_NONE_CAL);
		CalculateStrings.Emplace(UNIT_Meter_CAL);
		CalculateStrings.Emplace(UNIT_Percent_CAL);
		CalculateStrings.Emplace(UNIT_SEC_CAL);
		CalculateStrings.Emplace(UNIT_COUNT_CAL);
		CalculateStrings.Emplace(UNIT_YARD_CAL);
		TSet<FString> CalculatePropertys;

		ParseSkillTextParameter(CalculateStrings, SkillDesc, CalculatePropertys);

		FFormatNamedArguments WorkedSkillTableInfoFormatNamedArgs;
		UDescriptionFunctionLibrary::MakeFormatNamedArguments_WorkedSkillTableInfo(SkillDesc.ToString(), &InWorkedSkillInfo, WorkedSkillTableInfoFormatNamedArgs , CalculatePropertys);
		
		//Context and Active skill are used at the same time
// 		if (InWorkedSkillInfo.SkillControlType == ESkillControlTypeEnum::VE_Active)
// 		{
// 			FWorkedSkillTableInfo ActiveWorkedSkillInfo;
// 			URenewal_BattleFunctionLibrary::GetSkillTableWorkedInfo(ActiveWorkedSkillInfo, ECharacterCategoryTypeEnum::VE_UICharacter, InCharacterBattleInfo->ActiveSkill01, InCharacterBattleInfo->GetLevel());
// 			UDescriptionFunctionLibrary::MakeFormatNamedArguments_WorkedSkillTableInfo(SkillDesc.ToString(), &ActiveWorkedSkillInfo, WorkedSkillTableInfoFormatNamedArgs, InCharacterBattleInfo, CalculatePropertys, true);
// 		}

	
		UDescriptionFunctionLibrary::MakeUnitNameArgument(WorkedSkillTableInfoFormatNamedArgs);

		RevisePluralUnit(SkillDesc, WorkedSkillTableInfoFormatNamedArgs);
		if(bIsRich)
			TransformFTextToRichText(SkillDesc);
		FText Description = FText::Format(SkillDesc, WorkedSkillTableInfoFormatNamedArgs);

		return Description;
	}

	return FText::FromString("");
}

FName UUIFunctionLibrary::GetDayOfWeekStringID(EDayOfWeekTypeEnum InDayOfWeek)
{		
	FText DisplayName = UUtilFunctionLibrary::GetEnumToDisplayName( TEXT("EDayOfWeekTypeEnum"), (int32)InDayOfWeek );	
	return FName(*(DisplayName.ToString()));
}



FText UUIFunctionLibrary::GetSkillNameText(const FWorkedSkillTableInfo& InWorkedSkillInfo)
{
	//if (InWorkedSkillInfo.SkillName.IsNone() == false)
	//{
	//	
	//	FText SkillName = UUtilFunctionLibrary::GetLocalizedString(EStringTableType::VE_Skill, InWorkedSkillInfo.SkillName);

	//	/*
	//	FFormatNamedArguments WorkedSkillTableInfoFormatNamedArgs;
	//	FText Description = FText::Format(SkillName, WorkedSkillTableInfoFormatNamedArgs);
	//	return Description;
	//	*/
	//	return SkillName;
	//}

	if (InWorkedSkillInfo.SkillTableInfo_V2 && InWorkedSkillInfo.SkillTableInfo_V2->SkillName.IsNone() == false)
	{

		FText SkillName = UUtilFunctionLibrary::GetLocalizedString(EStringTableType::VE_Skill, InWorkedSkillInfo.SkillTableInfo_V2->SkillName);

		/*
		FFormatNamedArguments WorkedSkillTableInfoFormatNamedArgs;
		FText Description = FText::Format(SkillName, WorkedSkillTableInfoFormatNamedArgs);
		return Description;
		*/
		return SkillName;
	}

	return FText::FromString("");
}

bool UUIFunctionLibrary::TransformFTextToRichText(FText& InDescText)
{
	FString InDescString = InDescText.ToString();
	FString TempString = InDescString;
	
	FString StyleStr = UUtilFunctionLibrary::GetLocalizedString(EStringTableType::VE_Common, "RichText_ColorGreen").ToString();
	FString CloseStr = "</>";

	int32 FindPos	= 0;
	int32 StartPos	= 0;
	while (FindPos != -1 && StartPos < InDescString.Len())
	{
		FindPos = InDescString.Find("{", ESearchCase::IgnoreCase, ESearchDir::FromStart, StartPos);
		if (FindPos == -1)
			break;
		InDescString.InsertAt(FindPos, StyleStr);
		StartPos = FindPos + StyleStr.Len() + 1;
	}

	FindPos		= 0;
	StartPos	= 0;
	while (FindPos != -1 && StartPos < InDescString.Len())
	{
		FindPos = InDescString.Find("}", ESearchCase::IgnoreCase, ESearchDir::FromStart, StartPos);
		if (FindPos == -1)
			break;

		++FindPos;
		InDescString.InsertAt(FindPos, CloseStr);
		StartPos = FindPos + CloseStr.Len() + 1;
	}

	InDescText = FText::FromString(InDescString);
	
	return true;
}

bool UUIFunctionLibrary::ParseSkillTextParameter(const TArray<FString>& FindStrings, FText& InDescText, TSet<FString>& ParseSet)
{
	auto DescText = InDescText.ToString();

	for (auto& UnitStringELem : FindStrings)
	{
		int32	UnitStartPosition = 0;
		int32	UnitEndPosition = 0; 
		int32	PropertyStartPosition = 0;
		int32	PropertyEndPosition = 0;
		while (UnitStartPosition >= 0)
		{
			UnitStartPosition = DescText.Find(UnitStringELem, ESearchCase::IgnoreCase, ESearchDir::FromStart, UnitStartPosition);
			UnitEndPosition = DescText.Find("}", ESearchCase::IgnoreCase, ESearchDir::FromStart, UnitStartPosition);
			if (UnitStartPosition < 0 || UnitStartPosition >= UnitEndPosition)
				break;

			PropertyEndPosition		= DescText.Find("}", ESearchCase::IgnoreCase, ESearchDir::FromEnd, UnitStartPosition);
			PropertyStartPosition	= DescText.Find("{", ESearchCase::IgnoreCase, ESearchDir::FromEnd, PropertyEndPosition);

			if (PropertyStartPosition >= PropertyEndPosition)
			{
				++UnitStartPosition;
				continue;
			}

			FString PropertyString;
			for (int32 PropertyStringIndex = PropertyStartPosition + 1; PropertyStringIndex <= PropertyEndPosition - 1; ++PropertyStringIndex)
				PropertyString += DescText[PropertyStringIndex];

			ParseSet.Emplace(PropertyString);
			++UnitStartPosition;
		}
	}

	return true;
}

bool UUIFunctionLibrary::RevisePluralUnit(FText& InDescText, FFormatNamedArguments& InFormatNamedArguments)
{
	TArray<FString> UnitPluralStrings;
	UnitPluralStrings.Emplace(UNIT_SEC);	
	UnitPluralStrings.Emplace(UNIT_COUNT);
	UnitPluralStrings.Emplace(UNIT_YARD);
	UnitPluralStrings.Emplace(UNIT_SEC_CAL);
	UnitPluralStrings.Emplace(UNIT_COUNT_CAL);
	UnitPluralStrings.Emplace(UNIT_YARD_CAL);
	
	auto DescText = InDescText.ToString();
	
	for (auto& UnitStringELem : UnitPluralStrings)
	{
		int32	UnitStartPosition		= 0;
		int32	UnitEndPosition			= 0;
		int32	PropertyStartPosition	= 0;
		int32	PropertyEndPosition		= 0;
		while (UnitStartPosition >= 0)
		{
			UnitStartPosition = DescText.Find(UnitStringELem, ESearchCase::IgnoreCase, ESearchDir::FromStart, UnitStartPosition);
			UnitEndPosition = DescText.Find(TEXT("}"), ESearchCase::IgnoreCase, ESearchDir::FromStart, UnitStartPosition);
			if (UnitStartPosition < 0 || UnitStartPosition >= UnitEndPosition)
				break;

			PropertyEndPosition		=	DescText.Find(TEXT("}"), ESearchCase::IgnoreCase, ESearchDir::FromEnd, UnitStartPosition);
			PropertyStartPosition	=	DescText.Find(TEXT("{"), ESearchCase::IgnoreCase, ESearchDir::FromEnd, PropertyEndPosition);

			if (PropertyStartPosition >= PropertyEndPosition)
			{
				++UnitStartPosition;
				continue;
			}

			FString PropertyString;
			for (int32 PropertyStringIndex = PropertyStartPosition + 1; PropertyStringIndex <= PropertyEndPosition - 1; ++PropertyStringIndex)
				PropertyString += DescText[PropertyStringIndex];   
			auto PropertyValue = InFormatNamedArguments.Find(PropertyString);
			if (PropertyValue != nullptr)
			{
				const FText & PropertyValueText = PropertyValue->GetTextValue();
				//if (PropertyValueText != nullptr)
				{
					auto PropertyValueString = PropertyValueText.ToString();
					if (PropertyValueString.Equals(TEXT("1")) == false &&
						PropertyValueString.Equals(TEXT("1.0")) == false &&
						PropertyValueString.Equals(TEXT("0")) == false)
					{
						DescText.InsertAt(UnitEndPosition, TEXT("s"));
					}
				}
			}
			++UnitStartPosition;
		}
	}

	InDescText = FText::FromString(DescText);

	return true;
}


//////////////////////////////////////////////////////////////////////////
// UI Control
//////////////////////////////////////////////////////////////////////////

void UUIFunctionLibrary::AttachHUDPage(UObject* WorldContextObject, bool InIsAttach /*= true*/)
{	
	auto RGameInstance = RGAMEINSTANCE(GEngine);
	UWorld* World = GEngine->GetWorldFromContextObject(WorldContextObject, EGetWorldErrorMode::Assert);
	EGamePlayModeTypeEnum CheckGameMode = UUtilFunctionLibrary::GetGamePlayModeEnum();
	EUIPageEnum	AttachPage = EUIPageEnum::UIPage_None;
	AProjectRGameMode* GameMode = UUtilFunctionLibrary::GetGamePlayMode();
	
	
	if (EGamePlayModeTypeEnum::VE_Arena == CheckGameMode)
	{
		AttachPage = EUIPageEnum::UIPage_InGameHUD_Arena;
	}
	else if (EGamePlayModeTypeEnum::VE_MirrorDungeon == CheckGameMode)
	{
		AttachPage = EUIPageEnum::UIPage_InGameHUD_Mirror;
	}
	else if (EGamePlayModeTypeEnum::VE_RaidKillBoss == CheckGameMode)
	{
		UBaseStageInfo*	BaseStageInfo = UGamePlayBluePrintFunction::GetBaseStageInfo();
		if (BaseStageInfo->bPlayAlone)
			AttachPage = EUIPageEnum::UIPage_RaidHUD_One;
		else
			AttachPage = EUIPageEnum::UIPage_RaidHUD_Three;
	}
	else if (EGamePlayModeTypeEnum::VE_Colosseum == CheckGameMode)
	{
		AttachPage = EUIPageEnum::UIPage_RaidHUD_Three;
	}
	else
	{
		UBaseStageInfo*	BaseStageInfo = UGamePlayBluePrintFunction::GetBaseStageInfo();
		if(BaseStageInfo->bPlayAlone)
			AttachPage = EUIPageEnum::UIPage_InGameHUD_One;
		else
			AttachPage = EUIPageEnum::UIPage_InGameHUD_Three;
	}
	
	if (IsValid(GameMode))
	{
		if (Cast<ATestGameMode>(GameMode) != nullptr)
			AttachPage = EUIPageEnum::UIPage_InGameHUD_One;
	}

	checkf(RGameInstance->RWidgetManager != nullptr, TEXT("%s[ RGameInstance Is Empty ]"), __FUNCTION__);
	if (InIsAttach)
	{
		RGameInstance->RWidgetManager->AttachUIPage(AttachPage);
	}
	else
	{
		RGameInstance->RWidgetManager->DetachUIPage(AttachPage);
	}
}

EWidgetBluePrintEnum UUIFunctionLibrary::GetHUDType(UObject* WorldContextObject)
{
	UWorld* World = GEngine->GetWorldFromContextObject(WorldContextObject, EGetWorldErrorMode::Assert);
	
	AProjectRGameMode* GameMode = UUtilFunctionLibrary::GetGamePlayMode();


	EGamePlayModeTypeEnum CheckGameMode = UUtilFunctionLibrary::GetGamePlayModeEnum();
	EWidgetBluePrintEnum HUDType;

	if (EGamePlayModeTypeEnum::VE_Arena == CheckGameMode)
	{
		HUDType = EWidgetBluePrintEnum::WBP_InGameHUD_Arena;
	}
	else if (EGamePlayModeTypeEnum::VE_MirrorDungeon == CheckGameMode)
	{
		HUDType = EWidgetBluePrintEnum::WBP_InGameHUD_Mirror;
	}
	else if (EGamePlayModeTypeEnum::VE_RaidKillBoss == CheckGameMode)
	{
		UBaseStageInfo*	BaseStageInfo = UGamePlayBluePrintFunction::GetBaseStageInfo();
		if (BaseStageInfo->bPlayAlone)
			HUDType = EWidgetBluePrintEnum::WBP_Raid_One;
		else
			HUDType = EWidgetBluePrintEnum::WBP_Raid_Three;
		
	}
	else if (EGamePlayModeTypeEnum::VE_Colosseum == CheckGameMode)
	{
		HUDType = EWidgetBluePrintEnum::WBP_Raid_Three;
	}
	else
	{
//		HUDType = EWidgetBluePrintEnum::WBP_InGameHUD_CampaignNew;
		UBaseStageInfo*	BaseStageInfo = UGamePlayBluePrintFunction::GetBaseStageInfo();
		if (BaseStageInfo->bPlayAlone)
			HUDType = EWidgetBluePrintEnum::WBP_Campaign_One;
		else
			HUDType = EWidgetBluePrintEnum::WBP_Campaign_Three;
	}

	if (IsValid(GameMode))
	{
		if (Cast<ATestGameMode>(GameMode) != nullptr)
			return EWidgetBluePrintEnum::WBP_Campaign_One;
	}

	if (IsValid(GameMode))
	{
		if (Cast<ATutorialGameMode>(GameMode) != nullptr)
			return EWidgetBluePrintEnum::WBP_Campaign_Three;
	}

	return HUDType;
}

bool UUIFunctionLibrary::OnShowInGameHUD(UObject* WorldContextObject,bool bShow)
{
	UWorld* World = GEngine->GetWorldFromContextObject(WorldContextObject, EGetWorldErrorMode::Assert);
	auto RGameInstance = RGAMEINSTANCE(GEngine);
	checkf(RGameInstance->RWidgetManager != nullptr, TEXT("%s[ RWidgetManager Is Empty ]"), __FUNCTION__);
	//RGameInstance->RWidgetManager->SetUIVisibility(bShow);

	EWidgetBluePrintEnum	HUDType = UUIFunctionLibrary::GetHUDType(World);
	UUserWidget*			userWidget = RGameInstance->RWidgetManager->GetUserWidget(HUDType);
	if (IsValid(userWidget))
	{
		if (bShow)
		{
			userWidget->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
		}
		else
		{
			userWidget->SetVisibility(ESlateVisibility::Hidden);
		}

	}
	else
		return false;

	
	//bryan 180213
	/*if (!bShow)
	{
		SkillCancel(World);
	}*/
	

	return true;
}

UUserWidget* UUIFunctionLibrary::GetInGameHUD()
{
	URGameInstance*	RGameInstance			= RGAMEINSTANCE(GEngine);
	checkf(RGameInstance != nullptr, TEXT("%s[ RGameInstance Is Empty ]"), __FUNCTION__);
	UWorld* World							= GEngine->GetWorldFromContextObject(RGameInstance, EGetWorldErrorMode::Assert);
	checkf(RGameInstance->RWidgetManager != nullptr, TEXT("%s[ RWidgetManager Is Empty ]"), __FUNCTION__);
	EWidgetBluePrintEnum	HUDType			= UUIFunctionLibrary::GetHUDType(World);
	UUserWidget*			userWidget		= RGameInstance->RWidgetManager->GetUserWidget(HUDType);
	return userWidget;
}

URSkillSet_New* UUIFunctionLibrary::GetSkillSet(UObject* WorldContextObject)
{

	UUserWidget* InHUD = UUIFunctionLibrary::GetInGameHUD();
	URBaseIngameHUD* BaseHUD = Cast<URBaseIngameHUD>(InHUD);
	if (BaseHUD)
		return BaseHUD->SkillSet;
	
	URBaseIngameHUD* OneGameHUD = Cast<URBaseIngameHUD>(InHUD);
	if (OneGameHUD)
		return OneGameHUD->SkillSet;
	UHUD_RaidThreeControl* RaidGameHUD = Cast<UHUD_RaidThreeControl>(InHUD);
	if (RaidGameHUD)
		return RaidGameHUD->SkillSet;

// 	if (URTSManager::GetInstancePtr()->IsConnectedMode(EPVP::CS))
// 	{
// 		UUP_InGameHUD_Colloseum* ColoseumGameHUD = Cast<UUP_InGameHUD_Colloseum>(InHUD);
// 		if (ColoseumGameHUD)
// 			return ColoseumGameHUD->SkillSet;
// 	}
	if (URTSManager::GetInstancePtr()->IsConnectedMode(EPVP::MR))
	{
		URCampaignHUD_RaidOneControl* MultiRaidGameHUD = Cast<URCampaignHUD_RaidOneControl>(InHUD);
		if (MultiRaidGameHUD)
			return MultiRaidGameHUD->SkillSet;
	}

	return nullptr;
}


class UUP_HeroScrollView_Bottom* UUIFunctionLibrary::GetHeroScrollView()
{
	URGameInstance*	RGameInstance = RGAMEINSTANCE(GEngine);
	checkf(RGameInstance != nullptr, TEXT("%s[ RGameInstance Is Empty ]"), __FUNCTION__);
	UUserWidget* widget =  RGameInstance->RWidgetManager->GetUserWidget(EWidgetBluePrintEnum::WBP_BottomHeroScrollView);
	if (widget)
	{
		UUP_HeroScrollView_Bottom* ScrollViewWidget = Cast<UUP_HeroScrollView_Bottom>(widget);
		if (ScrollViewWidget)
			return ScrollViewWidget;
	}

	return nullptr;

}

class UUP_GenericScrollview* UUIFunctionLibrary::GetGenericScrollview()
{
	URGameInstance*	RGameInstance = RGAMEINSTANCE(GEngine);
	checkf(RGameInstance != nullptr, TEXT("%s[ RGameInstance Is Empty ]"), __FUNCTION__);
	UUserWidget* widget = RGameInstance->RWidgetManager->GetUserWidget(EWidgetBluePrintEnum::WBP_GenericScrollview);
	if (widget)
	{
		UUP_GenericScrollview* ScrollViewWidget = Cast<UUP_GenericScrollview>(widget);
		if (ScrollViewWidget)
			return ScrollViewWidget;
	}

	return nullptr;
}

void UUIFunctionLibrary::CreateTouchInterface(UObject* WorldContextObject)
{
// 	UWorld* World = GEngine->GetWorldFromContextObject(WorldContextObject, EGetWorldErrorMode::Assert);
// 	if (IsValid(World))
// 	{
// 		auto BasePlayerController = UUtilFunctionLibrary::GetBasePlayerController(World);
// 		if (BasePlayerController)
// 		{
// 			FStringAssetReference TouchInterfaceName(FString("/Game/UMG/BP_GameTouchInterface.BP_GameTouchInterface"));
// 			auto ManualTouchInterface = LoadObject<UTouchInterface>(NULL, *TouchInterfaceName.ToString());
// 			BasePlayerController->ActivateTouchInterface(ManualTouchInterface);
// 		}
// 	}
}

void UUIFunctionLibrary::RemoveTouchInterface(UObject* WorldContextObject)
{
	UWorld* World = GEngine->GetWorldFromContextObject(WorldContextObject, EGetWorldErrorMode::Assert);
	if (IsValid(World))
	{
		auto BasePlayerController = UUtilFunctionLibrary::GetBasePlayerController(World);
		if (BasePlayerController)
		{
			BasePlayerController->ActivateTouchInterface(nullptr);
		}
	}
}

//////////////////////////////////////////////////////////////////////////
// Input
//////////////////////////////////////////////////////////////////////////
void UUIFunctionLibrary::ActivateJoystick(UWorld* InWorld, bool InActive)
{
	AStagePlayerController* PlayerController = Cast<AStagePlayerController>(UUtilFunctionLibrary::GetBasePlayerController(InWorld));
	if (PlayerController)
	{
		if (InActive)
		{
			if (!PlayerController->GetJoystickActivate())
			{
				PlayerController->CreateJoystick();
			}
		}
		else
		{
			if (PlayerController->GetJoystickActivate())
			{
				PlayerController->RemoveJoystick();
			}
		}

		//if (InActive) //todo
		//	UUIFunctionLibrary::CreateTouchInterface(InWorld);
		//else
		//{
		//	UUIFunctionLibrary::RemoveTouchInterface(InWorld);
		//}
		//PlayerController->SetVirtualJoystickVisibility(InActive);
	}		
}

void UUIFunctionLibrary::InvalidateJoystick(UWorld* InWorld)
{
	AStagePlayerController* BasePlayerController = Cast<AStagePlayerController>(UUtilFunctionLibrary::GetBasePlayerController(InWorld));
	if (IsValid(BasePlayerController))
	{
		//ActivateJoystick(InWorld, (BasePlayerController->GetControlMode() == EAIControlModeEnum::VE_ManualMode) ? true : false);
		ActivateJoystick(InWorld, true);
	}
}



//////////////////////////////////////////////////////////////////////////
// UI RewardIcon
//////////////////////////////////////////////////////////////////////////
// #define REWARD_ITEM_INFO_FROM_COLUME(X)\
// if (RewardTableInfo->DropItem##X.IsNone() == false)\
// {\
// 	RewardData.amt = 0;\
// 	RewardData.prob = RewardTableInfo->DropRate##X;\
// 	RewardData.type = EREWARD_TYPE::ITEM;\
// 	RewardData.rewardId = RewardTableInfo->DropItem##X;\
// 	Rewards.Emplace(RewardData);\
// }
/*
bool UUIFunctionLibrary::GetRewardsIconInfoByRewardKey(UWorld* InWorld, FName InRewardKey, TArray<FITEM_ICON_INFO>& RewardsItemIconInfo, int32 ExactRewardCount, int32 FromTheFrontCount)
{
	auto RewardTableInfo = UTableManager::GetInstancePtr()->GetRewardRow(InRewardKey);
	if (RewardTableInfo == nullptr)
		return false;

	//Gacha Or Pack Reward Check
	auto IconTexture = UUtilFunctionLibrary::GetTexture2DFromAssetID(RewardTableInfo->ItemIconRef);
	if (IconTexture != nullptr || RewardTableInfo->DisplayOption == ERewardViewType::VE_Gacha)
	{
		FITEM_ICON_INFO NewItemIconInfo;
		NewItemIconInfo.ItemIconRef = IconTexture;
		RewardsItemIconInfo.Emplace(NewItemIconInfo);

		return true;
	}

	//reward item list
	TArray<FREWARD> Rewards;
	Rewards.Empty();
	FREWARD RewardData;
	if (RewardTableInfo->AvatarExp != 0)
	{
		RewardData.amt		= RewardTableInfo->AvatarExp;
		RewardData.prob		= 10000;
		RewardData.type		= EREWARD_TYPE::AVATAR_EXP;
		RewardData.rewardId = "";
		
		Rewards.Emplace(RewardData);
	}
	
	if (RewardTableInfo->HeroExp !=0)
	{
		RewardData.amt = RewardTableInfo->HeroExp;
		RewardData.prob = 10000;
		RewardData.type = EREWARD_TYPE::HERO_EXP;
		RewardData.rewardId = "";

		Rewards.Emplace(RewardData);
	}
	
	if (RewardTableInfo->GoldMin != 0)
	{
		RewardData.amt = RewardTableInfo->GoldMin;
		RewardData.prob = 10000;
		RewardData.type = EREWARD_TYPE::GOLD;
		RewardData.rewardId = "";

		Rewards.Emplace(RewardData);
	}
	
	if (RewardTableInfo->Gem != 0)
	{
		RewardData.amt = RewardTableInfo->Gem;
		RewardData.prob = 10000;
		RewardData.type = EREWARD_TYPE::GEM;
		RewardData.rewardId = "";

		Rewards.Emplace(RewardData);
	}
	
	if (RewardTableInfo->Stamina != 0)
	{
		RewardData.amt = RewardTableInfo->Stamina;
		RewardData.prob = 10000;
		RewardData.type = EREWARD_TYPE::STAMINA;
		RewardData.rewardId = "";

		Rewards.Emplace(RewardData);
	}
	
	if (RewardTableInfo->Sweeper != 0)
	{
		RewardData.amt = RewardTableInfo->Sweeper;
		RewardData.prob = 10000;
		RewardData.type = EREWARD_TYPE::SWEEPER;
		RewardData.rewardId = "";

		Rewards.Emplace(RewardData);
	}

	if (RewardTableInfo->HeroGroup != 0)
	{
		RewardData.amt = RewardTableInfo->HeroGroup;
		RewardData.prob = 10000;
		RewardData.type = EREWARD_TYPE::HEROTICKET;
		RewardData.rewardId = "";

		Rewards.Emplace(RewardData);
	}

	if (RewardTableInfo->HeroId.IsNone() == false)
	{
		//InRewards.Emplace(RewardData);
	}
	
	if (RewardTableInfo->SP1 != 0)
	{
		RewardData.amt = RewardTableInfo->SP1;
		RewardData.prob = 10000;
		RewardData.type = EREWARD_TYPE::REDSTONE;
		RewardData.rewardId = "";

		Rewards.Emplace(RewardData);
	}
	
	if (RewardTableInfo->SP2 != 0)
	{
		RewardData.amt = RewardTableInfo->SP2;
		RewardData.prob = 10000;
		RewardData.type = EREWARD_TYPE::SP2;
		RewardData.rewardId = "";

		Rewards.Emplace(RewardData);
	}
	
	if (RewardTableInfo->SP3 != 0)
	{
		RewardData.amt = RewardTableInfo->SP3;
		RewardData.prob = 10000;
		RewardData.type = EREWARD_TYPE::PVPTOKEN;
		RewardData.rewardId = "";

		Rewards.Emplace(RewardData);
	}
	
	if (RewardTableInfo->SP4 != 0)
	{
		RewardData.amt = RewardTableInfo->SP4;
		RewardData.prob = 10000;
		RewardData.type = EREWARD_TYPE::SP4;
		RewardData.rewardId = "";

		Rewards.Emplace(RewardData);
	}

	if (RewardTableInfo->EP1 != 0)
	{

		RewardData.amt = RewardTableInfo->EP1;
		RewardData.prob = 10000;
		RewardData.type = EREWARD_TYPE::EP1;
		RewardData.rewardId = "";

		Rewards.Emplace(RewardData);
	}

	if (RewardTableInfo->EP2 != 0)
	{

		RewardData.amt = RewardTableInfo->EP2;
		RewardData.prob = 10000;
		RewardData.type = EREWARD_TYPE::EP2;
		RewardData.rewardId = "";

		Rewards.Emplace(RewardData);
	}

	if (RewardTableInfo->EP3 != 0)
	{

		RewardData.amt = RewardTableInfo->EP3;
		RewardData.prob = 10000;
		RewardData.type = EREWARD_TYPE::EP3;
		RewardData.rewardId = "";

		Rewards.Emplace(RewardData);
	}

	if (RewardTableInfo->Titan != 0)
	{
		RewardData.amt = RewardTableInfo->Titan;
		RewardData.prob = 10000;
		RewardData.type = EREWARD_TYPE::TITAN;
		RewardData.rewardId = "";

		Rewards.Emplace(RewardData);
	}

	if (RewardTableInfo->StaminaPVP != 0)
	{
		RewardData.amt = RewardTableInfo->StaminaPVP;
		RewardData.prob = 10000;
		RewardData.type = EREWARD_TYPE::STAMINA_PVP;
		RewardData.rewardId = "";

		Rewards.Emplace(RewardData);
	}

	REWARD_ITEM_INFO_FROM_COLUME(1)
	REWARD_ITEM_INFO_FROM_COLUME(2)
	REWARD_ITEM_INFO_FROM_COLUME(3)
	REWARD_ITEM_INFO_FROM_COLUME(4)
	REWARD_ITEM_INFO_FROM_COLUME(5)
	REWARD_ITEM_INFO_FROM_COLUME(6)
	REWARD_ITEM_INFO_FROM_COLUME(7)
	REWARD_ITEM_INFO_FROM_COLUME(8)
	REWARD_ITEM_INFO_FROM_COLUME(9)
	REWARD_ITEM_INFO_FROM_COLUME(10)
	REWARD_ITEM_INFO_FROM_COLUME(11)
	REWARD_ITEM_INFO_FROM_COLUME(12)

	if (0 < ExactRewardCount)
	{
		if (Rewards.Num() != ExactRewardCount)
		{
			return false;
		}
	}

	RewardsItemIconInfo.Empty();
	int32 Index = 0;
	for (auto& RewardElem : Rewards)
	{
		if (0 < FromTheFrontCount)
		{
			if (Index >= FromTheFrontCount)
				break;
		}

		FITEM_ICON_INFO NewItemIconInfo;
		RewardsItemIconInfo.Emplace(NewItemIconInfo);
		GetRewardIconByRewardType(InWorld, RewardElem, RewardsItemIconInfo[Index]);
		++Index;
	}

	return true;
}

//*/
#define SET_REWARD_PREVIEW_DATA( X )\
if( RewardPreviewTableInfo->CodeName##X.IsNone() == false )\
{\
	resultType = GetRewardType(RewardPreviewTableInfo->CodeName##X); \
	if (resultType == EREWARD_TYPE::ITEM)\
	{\
		RewardData.amt = 0; \
		RewardData.prob = 10000; \
		RewardData.type = EREWARD_TYPE::ITEM; \
		RewardData.rewardId = RewardPreviewTableInfo->CodeName##X; \
	}\
	else if( resultType == EREWARD_TYPE::NONE || resultType == EREWARD_TYPE::WALLET)\
	{\
		RewardData.amt = RewardPreviewTableInfo->max_amount##X;\
		RewardData.prob = 10000;\
		RewardData.type = resultType;\
		RewardData.rewardId = RewardPreviewTableInfo->CodeName##X; \
	}\
	else\
	{\
		RewardData.amt = RewardPreviewTableInfo->max_amount##X;\
		RewardData.prob = 10000;\
		RewardData.type = resultType;\
		RewardData.rewardId = "";\
	}\
	Rewards.Emplace(RewardData);\
}



bool UUIFunctionLibrary::GetRewardsIconInfoByPreview(FName InRewardPreviewKey, TArray<FITEM_ICON_INFO>& RewardsItemIconInfo, int32 ExactRewardCount , int32 FromTheFrontCount )
{
	auto RewardPreviewTableInfo = UTableManager::GetInstancePtr()->GetRewardPreviewRow(InRewardPreviewKey);
	checkf(RewardPreviewTableInfo != nullptr, TEXT("%s[ RewardPreviewTableInfo Is Empty ]"), __FUNCTION__);
	TArray<FREWARD> Rewards;
	//Rewards.Empty();
	Rewards.Reserve(12);
	FREWARD RewardData;
	EREWARD_TYPE resultType;

	SET_REWARD_PREVIEW_DATA(1);
	SET_REWARD_PREVIEW_DATA(2);
	SET_REWARD_PREVIEW_DATA(3);
	SET_REWARD_PREVIEW_DATA(4);
	SET_REWARD_PREVIEW_DATA(5);
	SET_REWARD_PREVIEW_DATA(6);
	SET_REWARD_PREVIEW_DATA(7);
	SET_REWARD_PREVIEW_DATA(8);
	SET_REWARD_PREVIEW_DATA(9);
	SET_REWARD_PREVIEW_DATA(10);
	SET_REWARD_PREVIEW_DATA(11);
	SET_REWARD_PREVIEW_DATA(12);
//#ifndef RULE_TODOMEMORY
// 	SET_REWARD_PREVIEW_DATA(13);
// 	SET_REWARD_PREVIEW_DATA(14);
// 	SET_REWARD_PREVIEW_DATA(15);
// 	SET_REWARD_PREVIEW_DATA(16);
// 	SET_REWARD_PREVIEW_DATA(17);
// 	SET_REWARD_PREVIEW_DATA(18);
// 	SET_REWARD_PREVIEW_DATA(19);
// 	SET_REWARD_PREVIEW_DATA(20);
//#endif

	if (0 < ExactRewardCount)
	{
		if (Rewards.Num() != ExactRewardCount)
		{
			return false;
		}
	}

	RewardsItemIconInfo.Empty();
	RewardsItemIconInfo.Reserve(Rewards.Num());
	int32 Index = 0;
	for (auto& RewardElem : Rewards)
	{
		if (0 < FromTheFrontCount)
		{
			if (Index >= FromTheFrontCount)
				break;
		}

		FITEM_ICON_INFO NewItemIconInfo;
		RewardsItemIconInfo.Emplace(NewItemIconInfo);
		GetRewardIconByRewardType(RewardElem, RewardsItemIconInfo[Index]);
		++Index;
	}
	return true;
}

EREWARD_TYPE UUIFunctionLibrary::GetRewardType(FName InCodeName)
{
	FWalletTableInfo* walletInfo = UTableManager::GetInstancePtr()->GetWalletRow(InCodeName);

	if (walletInfo != nullptr)
		return EREWARD_TYPE::WALLET;

	EREWARD_TYPE resultType = UUtilFunctionLibrary::GetEnumValueFromString<EREWARD_TYPE>(TEXT("EREWARD_TYPE"), InCodeName.ToString());

	if ( (int32)resultType == 255 )
	{
		if (InCodeName.ToString().StartsWith(TEXT("RUNE")) )
			return EREWARD_TYPE::NONE;

		FRewardPreviewItemTableInfo* RewardPrewItemInfo = UTableManager::GetInstancePtr()->GetRewardPreviewItemRow(InCodeName);
		if (RewardPrewItemInfo)
			return EREWARD_TYPE::NONE;

		return EREWARD_TYPE::ITEM;
	}

	return resultType;
}

void UUIFunctionLibrary::GetItemIconInfoByWallet(FName InWalletID, int32 InAmt, FITEM_ICON_INFO& InItemIconInfo)
{
	FWalletTableInfo * walletInfo = UTableManager::GetInstancePtr()->GetWalletRow(InWalletID);

	if (!walletInfo)
	{
		UE_LOG(LogUI, Log, TEXT("UUIFunctionLibrary::GetItemIconInfoByWallet: Invalid id %s"), *InWalletID.ToString());
		return;
	}

	InItemIconInfo.count = InAmt;
	InItemIconInfo.ItemCategory = EREWARD_TYPE::WALLET;
	InItemIconInfo.ToolTipEventType = EToolTipEventTypeEnum::VE_None;
	InItemIconInfo.ToolTipID = InWalletID;

	InItemIconInfo.ItemIconRef = UUIFunctionLibrary::GetIconImageFromTable(walletInfo->ItemIconKey);
}

bool UUIFunctionLibrary::GetRuneIconByID(UWorld* InWorld, FName InCharacterID, FITEM_ICON_INFO& InRuneItemIconInfo)
{
//
// 	FString runeID = FString::Printf(TEXT("RUNE_%s"), *(InCharacterID.ToString()));
// 	//if (GetItemPreviewIconInfoWithID(InRuneItemIconInfo, FName(*runeID)))
// 
// 	InRuneItemIconInfo.itemId = FName(*runeID);
// 
// 	InRuneItemIconInfo.enchant = 0;
// 	InRuneItemIconInfo.enchantValue = 0;
// 	InRuneItemIconInfo.tuneCnt = 0;
// 	InRuneItemIconInfo.count = 1;
// 	InRuneItemIconInfo.bEquiped = false;
// 	InRuneItemIconInfo.bCompare = false;
// 
// 	InRuneItemIconInfo.itemGrade = EItemGradeEnum::VE_None;	
// 	InRuneItemIconInfo.ItemEquipType = EItemEquipTypeEnum::VE_EveryOne;
// 	InRuneItemIconInfo.ItemEquipPosition = EItemEquipPositionEnum::VE_EquipPositionMax;
// 
// 	{
// 		InRuneItemIconInfo.ItemCategory = EREWARD_TYPE::RUNE;
// 		InRuneItemIconInfo.heroId = InCharacterID;
// 
// 		FString rewardId = InCharacterID.ToString();
// 		if (rewardId.Len() == 1)
// 		{
// 			InRuneItemIconInfo.itemUD = FString::Printf(TEXT("Her00000%s"), *rewardId);
// 		}
// 		else if (rewardId.Len() == 2)
// 		{
// 			InRuneItemIconInfo.itemUD = FString::Printf(TEXT("Her0000%s"), *rewardId);
// 		}
// 		else if (rewardId.Len() == 3)
// 		{
// 			InRuneItemIconInfo.itemUD = FString::Printf(TEXT("Her000%s"), *rewardId);
// 		}
// 
// 		InRuneItemIconInfo.ToolTipEventType = EToolTipEventTypeEnum::VE_Goods;
// 		InRuneItemIconInfo.ToolTipID = FName(*InRuneItemIconInfo.itemUD);
// 
// 		FCurrencyPropertyTableInfo* CurrencyTableInfo = UTableManager::GetInstancePtr()->GetCurrencyPropertyRow(FName(*InRuneItemIconInfo.itemUD));
// 		if (CurrencyTableInfo)
// 		{
// 			if (!CurrencyTableInfo->ItemIconKey.IsNone())
// 			{
// 				InRuneItemIconInfo.ItemIconRef = UUIFunctionLibrary::GetIconImageFromTable(CurrencyTableInfo->ItemIconKey);
// 				return true;
// 			}
// 		}
// 	}
//
	return false;
}

bool UUIFunctionLibrary::GetRewardIconByRewardType(const FREWARD& InReward, FITEM_ICON_INFO& InRewardItemIconInfo)
{
	EREWARD_TYPE rewardType = InReward.type;
	FName rewardID = InReward.rewardId;
	/*
	Rune, Hero is special..
	DT_Reward table use HERXXXX style encoded format.
	But DT_RewardPreview table use RUNE_XX, HERO_XX format
	So you should decode GEngine infomations.
	*/
	if (rewardType == EREWARD_TYPE::NONE )
	{
		const TCHAR* Delims[] = { TEXT("_") };
		FString strLower = rewardID.ToString().ToUpper();
		TArray<FString> Parsed;
		strLower.ParseIntoArray(Parsed, Delims, 1);

		if (Parsed.Num() == 2)
		{
			
			if (Parsed[0] == TEXT("HERO"))
			{
				rewardType = EREWARD_TYPE::HERO;
				rewardID = FName(*Parsed[1]);
			}
// 			else if (Parsed[0] == TEXT("RUNE"))
// 			{
// 				rewardType = EREWARD_TYPE::RUNE;
// 				rewardID = FName(*Parsed[1]);
// 			}
		}
	}

	switch (rewardType)
	{
		case EREWARD_TYPE::ITEM:
			GetItemIconInfoWithID(InRewardItemIconInfo, rewardID);
			InRewardItemIconInfo.count = (InReward.amt > 0)? InReward.amt :1;		
			break;
		case EREWARD_TYPE::NONE:
			GetItemPreviewIconInfoWithID( InRewardItemIconInfo, rewardID);		
			return false;
		case EREWARD_TYPE::WALLET:
		{
			FWalletTableInfo * walletInfo = UTableManager::GetInstancePtr()->GetWalletRow(rewardID);

			if (walletInfo == nullptr)
				break;

			InRewardItemIconInfo.count = InReward.amt;
			InRewardItemIconInfo.ItemCategory = InReward.type;
			InRewardItemIconInfo.ToolTipEventType = EToolTipEventTypeEnum::VE_None;
			InRewardItemIconInfo.ToolTipID = rewardID;

			InRewardItemIconInfo.ItemIconRef = UUIFunctionLibrary::GetIconImageFromTable(walletInfo->ItemIconKey);

			return false;
		}

		case EREWARD_TYPE::HERO:
		{
			FString runeID = FString::Printf(TEXT("HERO_%s"), *(rewardID.ToString()));

			FRewardPreviewItemTableInfo* PreviewItemTableInfo = UTableManager::GetInstancePtr()->GetRewardPreviewItemRow(FName(*runeID));
			if (PreviewItemTableInfo)
			{
			}

			InRewardItemIconInfo.count = 0;
			InRewardItemIconInfo.ToolTipEventType = EToolTipEventTypeEnum::VE_Hero;
			InRewardItemIconInfo.ToolTipID = rewardID;

			FCharacterTableInfo* CharacterTableInfo = UTableManager::GetInstancePtr()->GetCharacterRow(rewardID);
			if (CharacterTableInfo)
			{
				InRewardItemIconInfo.ItemIconRef = UUIFunctionLibrary::GetIconImageFromTable(CharacterTableInfo->PortraitTextureKey);
			}
		}
		return true;

	// 	case EREWARD_TYPE::RUNE:
	// 		{
	// 			FString runeID = FString::Printf(TEXT("RUNE_%s"), *(rewardID.ToString()) );
	// 			GetRuneIconByID(InWorld, rewardID, InRewardItemIconInfo );
	// 			InRewardItemIconInfo.count = InReward.amt;
	// 		}
	// 
	// 		return true;
		default:
			InRewardItemIconInfo.count = InReward.amt;
			InRewardItemIconInfo.ItemCategory = InReward.type;
			InRewardItemIconInfo.ToolTipEventType = EToolTipEventTypeEnum::VE_None;
			break;
	}

	if (InRewardItemIconInfo.ItemCategory != EREWARD_TYPE::NONE)
	{
		UEnum* REWARD_TYPE				= FindObject<UEnum>(ANY_PACKAGE, TEXT("EREWARD_TYPE"), true);
		FString EnumString				= REWARD_TYPE->GetNameStringByIndex((int32)InRewardItemIconInfo.ItemCategory);
		FName	CurrencyPropertyKey		= FName(*EnumString);
		auto CurrencyPropertyTableInfo	= UTableManager::GetInstancePtr()->GetCurrencyPropertyRow(CurrencyPropertyKey);
		if (CurrencyPropertyTableInfo == nullptr)
		{
			return false;
		}

		if (!CurrencyPropertyTableInfo->ItemIconKey.IsNone())
		{
			InRewardItemIconInfo.ItemIconRef = UUtilFunctionLibrary::GetTexture2DFromTable(CurrencyPropertyTableInfo->ItemIconKey);
			if (InRewardItemIconInfo.ItemIconRef == nullptr)
			{
				InRewardItemIconInfo.ItemIconRef = UUIFunctionLibrary::GetIconImageFromTable(CurrencyPropertyTableInfo->ItemIconKey);
			}
		}
		/*else
		{
			return false;
		}*/

		InRewardItemIconInfo.ToolTipEventType = EToolTipEventTypeEnum::VE_Goods;
		InRewardItemIconInfo.ToolTipID = CurrencyPropertyKey;
	}
	else
	{
		return true;
	}

	return true;
}

/*
FString UUIFunctionLibrary::GetRewardTextureAssetName(EREWARD_TYPE rewardType)
{

}
*/


//////////////////////////////////////////////////////////////////////////
// Popup
//////////////////////////////////////////////////////////////////////////
UUP_CommonPopup* UUIFunctionLibrary::ShowCommonPopup(FText InCaption, FText InContents, ECommonPopupType InPopupType)
{
	auto RGameInstance = RGAMEINSTANCE(GEngine);
	checkf(RGameInstance->RWidgetManager != nullptr, TEXT("%s[ RWidgetManager Is Empty ]"), __FUNCTION__);
	URWidgetManager* WM = RGameInstance->RWidgetManager;
	
	UUP_CommonPopup* popup = WM->GetUserWidgetT<UUP_CommonPopup>(EWidgetBluePrintEnum::WBP_CommonPopup);
	if (!popup->IsValidLowLevel())
		popup = Cast< UUP_CommonPopup >(WM->CreateUserWidget(EWidgetBluePrintEnum::WBP_CommonPopup));
	
	if (popup->IsValidLowLevel())
	{
		WM->AddToViewport(EWidgetBluePrintEnum::WBP_CommonPopup, popup, COMMONPOPUP_ZORDER);
		popup->SetCommonPopup(InCaption, InContents, InPopupType);
	}

	return popup;
}
void UUIFunctionLibrary::HideCommonPopup()
{
	auto RGameInstance = RGAMEINSTANCE(GEngine);
	checkf(RGameInstance->RWidgetManager != nullptr, TEXT("%s[ RWidgetManager Is Empty ]"), __FUNCTION__);
	RGameInstance->RWidgetManager->HideUserWidget(EWidgetBluePrintEnum::WBP_CommonPopup);
}


void UUIFunctionLibrary::ShowWaitingPopup(bool bShow)
{
	URWidgetManager* WM = RGAMEINSTANCE(GEngine)->RWidgetManager;
	if (!WM->IsValidLowLevel())
		return;

	if (bShow)
	{
		UUserWidget* WaitingPopup = WM->GetUserWidget(EWidgetBluePrintEnum::WBP_WaitingPopup);
		if (!WaitingPopup->IsValidLowLevel())
			WaitingPopup = WM->CreateUserWidget(EWidgetBluePrintEnum::WBP_WaitingPopup);

		if (WaitingPopup->IsValidLowLevel() && !WaitingPopup->IsInViewport())
			WM->AddToViewport(EWidgetBluePrintEnum::WBP_WaitingPopup, WaitingPopup, WAITING_ZORDER);
	}
	else
		WM->HideUserWidget(EWidgetBluePrintEnum::WBP_WaitingPopup);
}

UUP_Popup_Avatar* UUIFunctionLibrary::ShowAvatarLevelUpPopup(bool bShow)
{
	URWidgetManager* WM = RGAMEINSTANCE(GEngine)->RWidgetManager;
	if (!WM->IsValidLowLevel())
		return nullptr;

	if (bShow)
	{
		UUserWidget* AvatarLevelup = WM->GetUserWidget(EWidgetBluePrintEnum::WBP_AvatarLevelupPopup);
		if (!AvatarLevelup->IsValidLowLevel())
			AvatarLevelup = WM->CreateUserWidget(EWidgetBluePrintEnum::WBP_AvatarLevelupPopup);

		if (AvatarLevelup->IsValidLowLevel() && !AvatarLevelup->IsInViewport())
			WM->AddToViewport(EWidgetBluePrintEnum::WBP_AvatarLevelupPopup, AvatarLevelup, AVATARLEVELUP_ZORDER);
		return Cast<UUP_Popup_Avatar>(AvatarLevelup);
	}
	else
	{
		WM->HideUserWidget(EWidgetBluePrintEnum::WBP_AvatarLevelupPopup);
		return nullptr;
	}
}

UUC_GenericPopup* UUIFunctionLibrary::ShowGenericPopupContent(EWidgetBluePrintEnum eWBP)
{
	URWidgetManager* WM = RGAMEINSTANCE(GEngine)->RWidgetManager;
	if (WM->IsValidLowLevel())
	{
		WM->ShowUserWidgetAutoCreate(eWBP,true , HEROSCROLLVIEW_ZORDER + 1);
		UUC_GenericPopup* Popup = Cast<UUC_GenericPopup>(WM->GetUserWidget(eWBP));
		return Popup;
	}

	return nullptr;
}

class UUP_GenericSplash* UUIFunctionLibrary::ShowGenericSplashContent(EWidgetBluePrintEnum eWBP, FText caption)
{
	//Need to fix!! everytime this function is called , allocated memory will keep increasing.
	URWidgetManager* WM = RGAMEINSTANCE(GEngine)->RWidgetManager;
	if (WM->IsValidLowLevel())
	{
		// Not using CreateUserWidget because GenericSplash closes itself
		FString StringWBP = WM->GetWBPEnumAsString(eWBP);
		FWidgetBluePrintTableInfo* WidgetInfo = UTableManager::GetInstancePtr()->GetWidgetBluePrintRow(FName(*StringWBP));
		if (!ensure(WidgetInfo))
			return nullptr;

		UUP_GenericSplash* Splash = Cast<UUP_GenericSplash>(WM->GetUserWidget(eWBP));
		if (!IsValid(Splash))
		{
			Splash = Cast<UUP_GenericSplash>(WM->CreateUserWidget(eWBP)); //CreateWidget<UUP_GenericSplash>(RGAMEINSTANCE(GEngine), WidgetInfo->GetWidgetClass());
		}
			

		Splash->AddToViewport(SPLASHPOPUP_ZORDER);
		if (!caption.IsEmpty())
		{
			Splash->Switcher_Header->SetActiveWidgetIndex(0);
			Splash->Text_Caption->SetText(caption);
		}
		return Splash;
	}
	return nullptr;

	
}

void UUIFunctionLibrary::ShowAutoPlayPopup(EBattleRepeatPopupState InState)
{
	auto RGameInstance = RGAMEINSTANCE(GEngine);
	checkf(RGameInstance->RWidgetManager != nullptr, TEXT("%s[ RWidgetManager Is Empty ]"), __FUNCTION__);
	URWidgetManager* WM = RGameInstance->RWidgetManager;
	if (WM->IsValidLowLevel())
	{
		UUP_BattleRepeatStatePopup* AutoPlayPopup = WM->GetUserWidgetT<UUP_BattleRepeatStatePopup>(EWidgetBluePrintEnum::WBP_AutoPlayPopup);
		if(!AutoPlayPopup->IsValidLowLevel())
			AutoPlayPopup = Cast< UUP_BattleRepeatStatePopup >(CreateUserWidget(EWidgetBluePrintEnum::WBP_AutoPlayPopup));

		if (AutoPlayPopup->IsValidLowLevel())
			WM->AddToViewport(EWidgetBluePrintEnum::WBP_AutoPlayPopup, AutoPlayPopup, COMMONPOPUP_ZORDER);
	}

	
}

void UUIFunctionLibrary::HideAutoPlayPopup()
{
	auto RGameInstance = RGAMEINSTANCE(GEngine);
	checkf(RGameInstance->RWidgetManager != nullptr, TEXT("%s[ RWidgetManager Is Empty ]"), __FUNCTION__);
	RGameInstance->RWidgetManager->HideUserWidget(EWidgetBluePrintEnum::WBP_AutoPlayPopup);

}

bool UUIFunctionLibrary::ShowMatchingWaiting(EPVP InConnecMode)
{
	auto RGameInstance = RGAMEINSTANCE(GEngine);
	checkf(RGameInstance->RWidgetManager != nullptr, TEXT("%s[ RWidgetManager Is Empty ]"), __FUNCTION__);
	URWidgetManager* WM = RGameInstance->RWidgetManager;
	if (WM->IsValidLowLevel())
	{
		UUP_Matching_Waiting* MatchingWaitingWidget = Cast<UUP_Matching_Waiting>(WM->GetUserWidget(EWidgetBluePrintEnum::WBP_MatchingWaiting));
		if (!MatchingWaitingWidget->IsValidLowLevel())
			MatchingWaitingWidget = Cast< UUP_Matching_Waiting >(WM->CreateUserWidget(EWidgetBluePrintEnum::WBP_MatchingWaiting));

		if (MatchingWaitingWidget->IsValidLowLevel())
		{
			WM->AddToViewport(EWidgetBluePrintEnum::WBP_MatchingWaiting, MatchingWaitingWidget, WAITING_ZORDER);
			MatchingWaitingWidget->RefreshUI(InConnecMode);
			return true;
		}
	}
	return false;
}

void UUIFunctionLibrary::HideMatchingWaiting()
{
	auto RGameInstance = RGAMEINSTANCE(GEngine);
	checkf(RGameInstance->RWidgetManager != nullptr, TEXT("%s[ RWidgetManager Is Empty ]"), __FUNCTION__);
	RGameInstance->RWidgetManager->HideUserWidget(EWidgetBluePrintEnum::WBP_MatchingWaiting);
}

UUP_ItemShortagePopup* UUIFunctionLibrary::ShowCurrencyShortagePopup(FText InCaption, FText InDesc, EREWARD_TYPE InCurrency, int32 InShortageValue)
{
	auto RGameInstance = RGAMEINSTANCE(GEngine);
	checkf(RGameInstance->RWidgetManager != nullptr, TEXT("%s[ RWidgetManager Is Empty ]"), __FUNCTION__);
	
	URWidgetManager* WM = RGameInstance->RWidgetManager;
	if (WM->IsValidLowLevel())
	{
		UUP_ItemShortagePopup* CurrencyShortagePopup = Cast< UUP_ItemShortagePopup >(WM->GetUserWidget(EWidgetBluePrintEnum::WBP_Popup_ItemShortage));
		if (!CurrencyShortagePopup->IsValidLowLevel())
			CurrencyShortagePopup = Cast<UUP_ItemShortagePopup>(WM->CreateUserWidget(EWidgetBluePrintEnum::WBP_Popup_ItemShortage));

		if (CurrencyShortagePopup->IsValidLowLevel() && !CurrencyShortagePopup->IsInViewport())
		{
			WM->AddToViewport(EWidgetBluePrintEnum::WBP_Popup_ItemShortage, CurrencyShortagePopup, COMMONPOPUP_ZORDER);
			CurrencyShortagePopup->SetPopup(InCaption, InDesc, InCurrency, InShortageValue);

			return CurrencyShortagePopup;
		}
	}
	return nullptr;
	
}

void UUIFunctionLibrary::HideCurrencyShortagePopup()
{
	auto RGameInstance = RGAMEINSTANCE(GEngine);
	checkf(RGameInstance->RWidgetManager != nullptr, TEXT("%s[ RWidgetManager Is Empty ]"), __FUNCTION__);
	RGameInstance->RWidgetManager->HideUserWidget(EWidgetBluePrintEnum::WBP_Popup_ItemShortage);
}

void UUIFunctionLibrary::ShowQuestCompletePopup()
{
	auto RGameInstance = RGAMEINSTANCE(GEngine);
	checkf(RGameInstance->RWidgetManager != nullptr, TEXT("%s[ RWidgetManager Is Empty ]"), __FUNCTION__);

	URWidgetManager* WM = RGameInstance->RWidgetManager;
	if (WM->IsValidLowLevel())
	{
		UQuestCompletePopup* QuestCompletePopup = Cast< UQuestCompletePopup >(WM->GetUserWidget(EWidgetBluePrintEnum::WBP_QuestCompletePopup));
		if (!QuestCompletePopup->IsValidLowLevel())
			QuestCompletePopup = Cast<UQuestCompletePopup>(WM->CreateUserWidget(EWidgetBluePrintEnum::WBP_QuestCompletePopup));

		if (QuestCompletePopup->IsValidLowLevel() && !QuestCompletePopup->IsInViewport())
		{
			WM->AddToViewport(EWidgetBluePrintEnum::WBP_QuestCompletePopup, QuestCompletePopup, COMMONPOPUP_ZORDER);
//			QuestCompletePopup->SetPopup(InCaption, InDesc, InCurrency, InShortageValue);

		}
	}
}

void UUIFunctionLibrary::HideQuestCompletePopup()
{
	auto RGameInstance = RGAMEINSTANCE(GEngine);
	RGameInstance->RWidgetManager->HideUserWidget(EWidgetBluePrintEnum::WBP_QuestCompletePopup);
}

UUP_RewardPopup* UUIFunctionLibrary::ShowRewardPopup()
{
	auto RGameInstance = RGAMEINSTANCE(GEngine);
	checkf(RGameInstance->RWidgetManager != nullptr, TEXT("%s[ RWidgetManager Is Empty ]"), __FUNCTION__);
	
	URWidgetManager* WM = RGameInstance->RWidgetManager;
	if (WM->IsValidLowLevel())
	{
		UUP_RewardPopup* RewardPopup = WM->GetUserWidgetT<UUP_RewardPopup>(EWidgetBluePrintEnum::WBP_RewardPopup);
		if (!RewardPopup->IsValidLowLevel())
			RewardPopup = Cast<UUP_RewardPopup>(WM->CreateUserWidget(EWidgetBluePrintEnum::WBP_RewardPopup));

		if (RewardPopup->IsValidLowLevel())
		{
			RewardPopup->InitRewardPopup();
			if (!RewardPopup->IsInViewport())
				WM->AddToViewport(EWidgetBluePrintEnum::WBP_RewardPopup, RewardPopup, REWARDPOPUP_ZORDER);
		}
		return RewardPopup;
	}
	
	return nullptr;
	
}

UUP_Popup_Reward_AfterGrind* UUIFunctionLibrary::ShowRewardPopup_ScrollView(TArray<FREWARD> Rewards)
{
	if (RGAMEINSTANCE(GEngine) == nullptr)
		return nullptr;

	URWidgetManager* WM = RGAMEINSTANCE(GEngine)->RWidgetManager;
	if (WM->IsValidLowLevel())
	{
		UUP_Popup_Reward_AfterGrind* RewardPopup = WM->GetUserWidgetT<UUP_Popup_Reward_AfterGrind>(EWidgetBluePrintEnum::WBP_GrindRewardPopup);
		if (!RewardPopup->IsValidLowLevel())
			RewardPopup = Cast<UUP_Popup_Reward_AfterGrind>(WM->CreateUserWidget(EWidgetBluePrintEnum::WBP_GrindRewardPopup));

		if (RewardPopup->IsValidLowLevel() && !RewardPopup->IsInViewport())
		{
			RewardPopup->GetScrollView()->SetReward(Rewards);
			WM->AddToViewport(EWidgetBluePrintEnum::WBP_GrindRewardPopup, RewardPopup, REWARDPOPUP_ZORDER);
		}

		return RewardPopup;
	}

	return nullptr;
}

UUP_RewardPopup* UUIFunctionLibrary::ShowCommonRewardPopupByRewardSet(TArray<FREWARDSET>& RewardSets)
{
	if (RGAMEINSTANCE(GEngine) == nullptr)
		return nullptr;

	URWidgetManager* WM = RGAMEINSTANCE(GEngine)->RWidgetManager;
	if (WM->IsValidLowLevel())
	{
		UUP_RewardPopup* rewardPopup = Cast<UUP_RewardPopup>(WM->GetUserWidget(EWidgetBluePrintEnum::WBP_RewardPopup));
		if (!rewardPopup->IsValidLowLevel())
			rewardPopup = Cast<UUP_RewardPopup>(WM->CreateUserWidget(EWidgetBluePrintEnum::WBP_RewardPopup));

		if (rewardPopup->IsValidLowLevel() && !rewardPopup->IsInViewport())
		{
			rewardPopup->InitRewardPopup();
			WM->AddToViewport(EWidgetBluePrintEnum::WBP_RewardPopup, rewardPopup, REWARDPOPUP_ZORDER);
		}
		
		TArray<FREWARD> Rewards;
		for (auto& RewardSetElem : RewardSets)
		{
			for (auto& RewardElem : RewardSetElem.rewards)
			{
				Rewards.Emplace(RewardElem);
			}
		}

		//Reward Icon
		TArray<FITEM_ICON_INFO> Reward_Item_Icon_Info;
		Reward_Item_Icon_Info.Empty(Rewards.Num());
		for (auto& RewardsElem : Rewards)
		{
			FITEM_ICON_INFO RewardItemIconInfo;
			UUIFunctionLibrary::GetRewardIconByRewardType(RewardsElem, RewardItemIconInfo);
			Reward_Item_Icon_Info.Emplace(RewardItemIconInfo);

		}
		rewardPopup->SetRewardItemItemInfo(Reward_Item_Icon_Info, &Rewards);
		return rewardPopup;
	}
	return nullptr;
}

UUP_RewardPopup* UUIFunctionLibrary::ShowCommonRewardPopupByRewards(TArray<FREWARD>& Rewards, bool bFirst /*= false*/)
{
	//Reward Icon
	if (RGAMEINSTANCE(GEngine) == nullptr)
		return nullptr;
	TArray<FITEM_ICON_INFO> Reward_Item_Icon_Info;
	Reward_Item_Icon_Info.Empty();
	for (auto& RewardsElem : Rewards)
	{
		FITEM_ICON_INFO RewardItemIconInfo;
		UUIFunctionLibrary::GetRewardIconByRewardType(RewardsElem, RewardItemIconInfo);

		if (IsValid(RewardItemIconInfo.ItemIconRef))
		{
			Reward_Item_Icon_Info.Emplace(RewardItemIconInfo);
		}
	}

	if (0 < Reward_Item_Icon_Info.Num())
	{
		URWidgetManager* WM = RGAMEINSTANCE(GEngine)->RWidgetManager;
		if (WM->IsValidLowLevel())
		{
			UUP_RewardPopup* RewardPopup = Cast< UUP_RewardPopup >(WM->GetUserWidget(EWidgetBluePrintEnum::WBP_RewardPopup));
			if (!RewardPopup->IsValidLowLevel())
				RewardPopup = Cast< UUP_RewardPopup >(WM->CreateUserWidget(EWidgetBluePrintEnum::WBP_RewardPopup));

			if (RewardPopup->IsValidLowLevel() && !RewardPopup->IsInViewport())
			{
				WM->AddToViewport(EWidgetBluePrintEnum::WBP_RewardPopup, RewardPopup, REWARDPOPUP_ZORDER);
				RewardPopup->InitRewardPopup();
				RewardPopup->SetRewardItemItemInfo(Reward_Item_Icon_Info, &Rewards, bFirst);
			}

			return RewardPopup;
		}
	}

	return nullptr;
}

UUP_RewardPopup* UUIFunctionLibrary::ShowQuestRewardsPopup(TArray<FREWARD>& Rewards, FText& InQuestText)
{
	//Reward Icon
	if (RGAMEINSTANCE(GEngine) == nullptr)
		return nullptr;
	TArray<FITEM_ICON_INFO> Reward_Item_Icon_Info;
	Reward_Item_Icon_Info.Empty();
	for (auto& RewardsElem : Rewards)
	{
		FITEM_ICON_INFO RewardItemIconInfo;
		UUIFunctionLibrary::GetRewardIconByRewardType(RewardsElem, RewardItemIconInfo);

		if (IsValid(RewardItemIconInfo.ItemIconRef))
		{
			Reward_Item_Icon_Info.Emplace(RewardItemIconInfo);
		}
	}

	if (0 < Reward_Item_Icon_Info.Num())
	{
		URWidgetManager* WM = RGAMEINSTANCE(GEngine)->RWidgetManager;
		if (WM->IsValidLowLevel())
		{
			UUP_RewardPopup* RewardPopup = Cast< UUP_RewardPopup >(WM->GetUserWidget(EWidgetBluePrintEnum::WBP_RewardPopup));
			if (!RewardPopup->IsValidLowLevel())
				RewardPopup = Cast< UUP_RewardPopup >(WM->CreateUserWidget(EWidgetBluePrintEnum::WBP_RewardPopup));

			if (RewardPopup->IsValidLowLevel() && !RewardPopup->IsInViewport())
			{
				WM->AddToViewport(EWidgetBluePrintEnum::WBP_RewardPopup, RewardPopup, REWARDPOPUP_ZORDER);
				RewardPopup->InitRewardPopup();
				FText text = UUtilFunctionLibrary::GetLocalizedString(EStringTableType::VE_UI, TEXT("UI_Important_Task_Complete"));
				text = FText::Format(text, InQuestText);
				RewardPopup->SetQuestRewardItem(Reward_Item_Icon_Info, &Rewards, text);
			}

			return RewardPopup;
		}
	}

	return nullptr;

}

void UUIFunctionLibrary::HideRewardPopup()
{
	auto RGameInstance = RGAMEINSTANCE(GEngine);
	checkf(RGameInstance->RWidgetManager != nullptr, TEXT("%s[ RWidgetManager Is Empty ]"), __FUNCTION__);
	RGameInstance->RWidgetManager->HideUserWidget(EWidgetBluePrintEnum::WBP_RewardPopup);
}

FText UUIFunctionLibrary::GetRewardText(FREWARD& InReward)
{
	FText text;
	FITEM_ICON_INFO RewardItemIconInfo;
	if (UUIFunctionLibrary::GetRewardIconByRewardType(InReward, RewardItemIconInfo))
	{
		return GetToolTipText(RewardItemIconInfo.ToolTipEventType, RewardItemIconInfo.ToolTipID);
	}
	return text;
}

FText UUIFunctionLibrary::GetToolTipText(EToolTipEventTypeEnum InToolTipType, FName InID)
{
	FText rtText;

	switch (InToolTipType)
	{
	case EToolTipEventTypeEnum::VE_Item:
	case EToolTipEventTypeEnum::VE_ItemDetail:
	{
		FItemTableInfo* TableInfo = UTableManager::GetInstancePtr()->GetCharacterItemRow(InID);
		if (TableInfo)
		{
			rtText = UUtilFunctionLibrary::GetLocalizedString(EStringTableType::VE_Item, TableInfo->ItemName);
		}
	}
	break;
	case EToolTipEventTypeEnum::VE_Goods:
	{
		FCurrencyPropertyTableInfo* TableInfo = UTableManager::GetInstancePtr()->GetCurrencyPropertyRow(InID);
		if (TableInfo)
		{
			rtText = UUtilFunctionLibrary::GetLocalizedString(EStringTableType::VE_Common, TableInfo->CommonStringKey);
		}
	}
	break;
	case EToolTipEventTypeEnum::VE_Npc:
	{
		FCharacterTableInfo* TableInfo = UTableManager::GetInstancePtr()->GetCharacterNpcRow(InID);
		if (TableInfo)
		{
			rtText = UUtilFunctionLibrary::GetLocalizedString(EStringTableType::VE_Npc, TableInfo->DisplayName);
		}
	}
	break;
	case EToolTipEventTypeEnum::VE_Hero:
	{
		FCharacterTableInfo* TableInfo = UTableManager::GetInstancePtr()->GetCharacterRow(InID);
		if (TableInfo)
		{
			rtText = UUtilFunctionLibrary::GetLocalizedString(EStringTableType::VE_Hero, TableInfo->DisplayName);
		}
	}
	break;
	case EToolTipEventTypeEnum::VE_ShopItem:
	{
		FShopProductTableInfo* TableInfo = UTableManager::GetInstancePtr()->GetShopProductRow(InID);
		if (TableInfo)
		{
			rtText = UUtilFunctionLibrary::GetLocalizedString(EStringTableType::VE_Shop, TableInfo->productName);
		}
	}
	break;
	case EToolTipEventTypeEnum::VE_PreviewItem:
	{
		auto CurrencyTableInfo = UTableManager::GetInstancePtr()->GetCurrencyPropertyRow(InID);
		if (CurrencyTableInfo)
		{
			rtText = UUtilFunctionLibrary::GetLocalizedString(EStringTableType::VE_Common, CurrencyTableInfo->CommonStringKey);
			break;
		}

		auto ItemTableInfo = UTableManager::GetInstancePtr()->GetCharacterItemRow(InID);
		if (ItemTableInfo)
		{
			rtText = UUtilFunctionLibrary::GetLocalizedString(EStringTableType::VE_Item, ItemTableInfo->ItemName);
			break;
		}

		auto PreviewItemTableInfo = UTableManager::GetInstancePtr()->GetRewardPreviewItemRow(InID);
		if (PreviewItemTableInfo)
		{
			rtText = UUtilFunctionLibrary::GetLocalizedString(EStringTableType::VE_Name, PreviewItemTableInfo->NameStringKey);
			break;
		}

		//Direct Tooltip ID(Rune... etc.)

		rtText = UUtilFunctionLibrary::GetLocalizedString(EStringTableType::VE_Tooltip, InID);
	}
	break;
	default:
		break;
	}

	return rtText;
}

FText UUIFunctionLibrary::GetToolTipTypeText(EToolTipEventTypeEnum InToolTipType, FName InID)
{
	FText rtText;

	switch (InToolTipType)
	{
	case EToolTipEventTypeEnum::VE_Item:
	case EToolTipEventTypeEnum::VE_ItemDetail:
	{
		FItemTableInfo* TableInfo = UTableManager::GetInstancePtr()->GetCharacterItemRow(InID);
		if (TableInfo)
		{
			rtText = UUtilFunctionLibrary::GetLocalizedString(EStringTableType::VE_Item, TableInfo->TooltipType);
		}
	}
	break;
	case EToolTipEventTypeEnum::VE_Goods:
	{
		FCurrencyPropertyTableInfo* TableInfo = UTableManager::GetInstancePtr()->GetCurrencyPropertyRow(InID);
		if (TableInfo)
		{
			rtText = UUtilFunctionLibrary::GetLocalizedString(EStringTableType::VE_Common, TableInfo->TooltipType);
		}
	}
	break;
	case EToolTipEventTypeEnum::VE_Npc:
	{
		FCharacterTableInfo* TableInfo = UTableManager::GetInstancePtr()->GetCharacterNpcRow(InID);
		if (TableInfo)
		{
			rtText = UUtilFunctionLibrary::GetLocalizedString(EStringTableType::VE_Npc, TableInfo->TooltipType);
		}
	}
	break;
	case EToolTipEventTypeEnum::VE_PreviewItem:
	{
		auto PreviewItemTableInfo = UTableManager::GetInstancePtr()->GetRewardPreviewItemRow(InID);
		if (PreviewItemTableInfo)
		{
			rtText = UUtilFunctionLibrary::GetLocalizedString(EStringTableType::VE_Item, PreviewItemTableInfo->TooltipType);
		}
	}
	break;
	default:
		break;
	}

	return rtText;
}

FText UUIFunctionLibrary::GetToolTipDescriptionText(EToolTipEventTypeEnum InToolTipType, FName InID)
{
	FText rtText;

	switch (InToolTipType)
	{
	case EToolTipEventTypeEnum::VE_Item:
	case EToolTipEventTypeEnum::VE_ItemDetail:
	{
		FItemTableInfo* TableInfo = UTableManager::GetInstancePtr()->GetCharacterItemRow(InID);
		if (TableInfo)
		{
			rtText = UUtilFunctionLibrary::GetLocalizedString(EStringTableType::VE_Tooltip, TableInfo->Tooltip);
		}
	}
	break;
	case EToolTipEventTypeEnum::VE_Goods:
	{
		FCurrencyPropertyTableInfo* TableInfo = UTableManager::GetInstancePtr()->GetCurrencyPropertyRow(InID);
		if (TableInfo)
		{
			rtText = UUtilFunctionLibrary::GetLocalizedString(EStringTableType::VE_Tooltip, TableInfo->Tooltip);
		}
	}
	break;
	case EToolTipEventTypeEnum::VE_Npc:
	{
		FCharacterTableInfo* TableInfo = UTableManager::GetInstancePtr()->GetCharacterNpcRow(InID);
		if (TableInfo)
		{
			rtText = GetNpcInfoText(TableInfo->Tooltip);// UUtilFunctionLibrary::GetLocalizedString(EStringTableType::VE_Tooltip, TableInfo->Tooltip);
		}
	}
	break;
	case EToolTipEventTypeEnum::VE_Hero:
	{
		FCharacterTableInfo* TableInfo = UTableManager::GetInstancePtr()->GetCharacterRow(InID);
		if (TableInfo)
		{
			rtText = GetHeroInfoText(TableInfo->Tooltip);//UUtilFunctionLibrary::GetLocalizedString(EStringTableType::VE_Tooltip, TableInfo->Tooltip);
		}
	}
	break;
	case EToolTipEventTypeEnum::VE_ShopItem:
	{
		FShopProductTableInfo* TableInfo = UTableManager::GetInstancePtr()->GetShopProductRow(InID);
		if (TableInfo)
		{
			rtText = UUtilFunctionLibrary::GetLocalizedString(EStringTableType::VE_Tooltip, TableInfo->Tooltip);
		}
	}
	break;
	case EToolTipEventTypeEnum::VE_PreviewItem:
	{
		auto CurrencyTableInfo = UTableManager::GetInstancePtr()->GetCurrencyPropertyRow(InID);
		if (CurrencyTableInfo)
		{
			rtText = UUtilFunctionLibrary::GetLocalizedString(EStringTableType::VE_Tooltip, CurrencyTableInfo->Tooltip);
			break;
		}

		auto ItemTableInfo = UTableManager::GetInstancePtr()->GetCharacterItemRow(InID);
		if (ItemTableInfo)
		{
			rtText = UUtilFunctionLibrary::GetLocalizedString(EStringTableType::VE_Tooltip, ItemTableInfo->Tooltip);
			break;
		}

		auto PreviewItemTableInfo = UTableManager::GetInstancePtr()->GetRewardPreviewItemRow(InID);
		if (PreviewItemTableInfo)
		{
			rtText = UUtilFunctionLibrary::GetLocalizedString(EStringTableType::VE_Tooltip, PreviewItemTableInfo->Tooltip);
			break;
		}

		//Direct Tooltip ID(Rune... etc.)

		rtText = UUtilFunctionLibrary::GetLocalizedString(EStringTableType::VE_Tooltip, InID);
		break;
	}
	break;
	default:
		break;
	}

	return rtText;
}

FText UUIFunctionLibrary::GetNpcInfoText(TArray<FName>& Tooltips)
{
	FText rtText;

	if (Tooltips.Num() == 0)
		return rtText;

	bool bFirst = true;
	for (FName& tooltip : Tooltips)
	{
		if (bFirst)
		{
			bFirst = false;
			rtText = FText::FromString(rtText.ToString() + UUtilFunctionLibrary::GetLocalizedString(EStringTableType::VE_Tooltip, tooltip).ToString());
			continue;
		}
		rtText = FText::FromString(rtText.ToString() + TEXT(", ") + UUtilFunctionLibrary::GetLocalizedString(EStringTableType::VE_Tooltip, tooltip).ToString());
	}

	return rtText;
}

FText UUIFunctionLibrary::GetHeroInfoText(TArray<FName>& Tooltips)
{
	FText rtText;

	if (Tooltips.Num() == 0)
		return rtText;

	if(Tooltips.IsValidIndex(0))
		rtText = UUtilFunctionLibrary::GetLocalizedString(EStringTableType::VE_Tooltip, Tooltips[0]);

	return rtText;
}

void UUIFunctionLibrary::ShowRewardPopupByRewards(TArray<FREWARD>& InRewards)
{
	auto RGameInstance = RGAMEINSTANCE(GEngine);
	checkf(RGameInstance->RWidgetManager != nullptr, TEXT("%s[ RWidgetManager Is Empty ]"), __FUNCTION__);
	checkf(RGameInstance->RInventory != nullptr, TEXT("%s[ RInventory Is Empty ]"), __FUNCTION__);
	int32 RewardMax = InRewards.Num();
	if (RewardMax == 0)
		return;

	UUP_RewardPopup* rewardPopup = UUIFunctionLibrary::ShowCommonRewardPopupByRewards(InRewards);
	if (IsValid(rewardPopup))
	{
		rewardPopup->bCheckHeroRuneSplash = true;
	}
}

void UUIFunctionLibrary::ShowRewardPopupByRewardsHero(TArray<FHERO>& InHeros, TArray<FREWARD>& InRewards)
{
	URGameInstance* RGameInstance = RGAMEINSTANCE(GEngine);
	checkf(RGameInstance->RWidgetManager != nullptr, TEXT("%s[ RWidgetManager Is Empty ]"), __FUNCTION__);
	checkf(RGameInstance->RInventory != nullptr, TEXT("%s[ RInventory Is Empty ]"), __FUNCTION__);
	int32 num = RGameInstance->RInventory->NewHeroArray.Num();
	RGameInstance->RInventory->UpdateHeroData(InHeros);
	int32 newNum = RGameInstance->RInventory->NewHeroArray.Num();
	if (newNum > num)
	{
		int32 idx = newNum - 1;
		if (RGameInstance->RInventory->NewHeroArray.IsValidIndex(idx))
		{
			FString ud = RGameInstance->RInventory->NewHeroArray[idx];
			FHERO* outHero = RGameInstance->RInventory->GetHeroWithUD(ud);
			if (outHero != nullptr)
			{
				UUIFunctionLibrary::ShowCommonPopupHeroRuneSplash(outHero->heroId,
					UUtilFunctionLibrary::GetLocalizedString(EStringTableType::VE_UI, TEXT("UI_Notic_000000")),
					UUtilFunctionLibrary::GetLocalizedString(EStringTableType::VE_UI, TEXT("USK_up_getNewHero")));
			}
		}
	}
	else
	{
		if (InRewards.Num() > 0)
		{
			ShowRewardPopupByRewards(InRewards);
		}			
	}
}



UUP_CommonPopupSplash* UUIFunctionLibrary::ShowCommonPopupSplash(FText InCaption, FText InDesc)
{
	URWidgetManager* WM = RGAMEINSTANCE(GEngine)->RWidgetManager;
	if (WM->IsValidLowLevel())
	{
		UUP_CommonPopupSplash* CommonSplash = Cast<UUP_CommonPopupSplash>(WM->GetUserWidget(EWidgetBluePrintEnum::WBP_CommonPopupSplash));
		if (!CommonSplash->IsValidLowLevel())
			CommonSplash = Cast<UUP_CommonPopupSplash>(WM->CreateUserWidget(EWidgetBluePrintEnum::WBP_CommonPopupSplash));

		if (CommonSplash->IsValidLowLevel() && !CommonSplash->IsInViewport())
		{
			WM->AddToViewport(EWidgetBluePrintEnum::WBP_CommonPopupSplash, CommonSplash, SPLASHPOPUP_ZORDER);
			CommonSplash->RefreshUI(ECommonSplashType::VE_Normal, InCaption, InDesc);

			return CommonSplash;
		}
	}
	return nullptr;
}

UUP_CommonPopupSplash* UUIFunctionLibrary::ShowCommonPopupHeroRuneSplash(FName InID, FText InCaption, FText InDesc, bool bLevelUp /*= false*/)
{

	URWidgetManager* WM = RGAMEINSTANCE(GEngine)->RWidgetManager;
	if (WM->IsValidLowLevel())
	{
		WM->HideUserWidget(EWidgetBluePrintEnum::WBP_CommonPopupSplash);

		UUP_CommonPopupSplash* CommonSplash = Cast<UUP_CommonPopupSplash>(WM->GetUserWidget(EWidgetBluePrintEnum::WBP_CommonPopupSplash));
		if (!CommonSplash->IsValidLowLevel())
			CommonSplash = Cast<UUP_CommonPopupSplash>(WM->CreateUserWidget(EWidgetBluePrintEnum::WBP_CommonPopupSplash));

		if (CommonSplash->IsValidLowLevel() && !CommonSplash->IsInViewport())
		{
			WM->AddToViewport(EWidgetBluePrintEnum::WBP_CommonPopupSplash, CommonSplash, SPLASHPOPUP_ZORDER);
			CommonSplash->RefreshUI(ECommonSplashType::VE_Hero, InCaption, InDesc , InID , bLevelUp);

			return CommonSplash;
		}
	}
	return nullptr;
}

void UUIFunctionLibrary::HideCommonPopupSplash()
{
	URWidgetManager* WM = RGAMEINSTANCE(GEngine)->RWidgetManager;
	if (WM->IsValidLowLevel())
		WM->HideUserWidget(EWidgetBluePrintEnum::WBP_CommonPopupSplash);
}

class UUP_RewardPopupSplash* UUIFunctionLibrary::ShowRewardPopupSplash(ERewardSplashType InType, FText& InOwnerName, FText& InItemName, TArray<FREWARD>& InRewardData)
{

	URWidgetManager* WM = RGAMEINSTANCE(GEngine)->RWidgetManager;
	if (WM->IsValidLowLevel())
	{
		UUP_RewardPopupSplash* RewardSplash = Cast<UUP_RewardPopupSplash>(WM->GetUserWidget(EWidgetBluePrintEnum::WBP_RewardPopupSplash));
		if (!RewardSplash->IsValidLowLevel())
			RewardSplash = Cast<UUP_RewardPopupSplash>(WM->CreateUserWidget(EWidgetBluePrintEnum::WBP_RewardPopupSplash));

		if (RewardSplash->IsValidLowLevel() && !RewardSplash->IsInViewport())
		{
			WM->AddToViewport(EWidgetBluePrintEnum::WBP_RewardPopupSplash, RewardSplash, SPLASHPOPUP_ZORDER);
			RewardSplash->RefreshUI(InType, InOwnerName, InItemName, InRewardData);

			return RewardSplash;
		}
	}
	return nullptr;
}

class UUP_Popup_Splash_AttackPower* UUIFunctionLibrary::ShowChangeAttackPowerSplash(FName InHeroID, int32 InCurrentPower, int32 InPrevPower)
{
	if (InCurrentPower == InPrevPower)
		return nullptr;

	URWidgetManager* WM = RGAMEINSTANCE(GEngine)->RWidgetManager;
	if (WM->IsValidLowLevel())
	{
		UUP_Popup_Splash_AttackPower* AttackSplash = Cast<UUP_Popup_Splash_AttackPower>(WM->GetUserWidget(EWidgetBluePrintEnum::WBP_ChangeAttackPowerSplash));
		if (!AttackSplash->IsValidLowLevel())
			AttackSplash = Cast<UUP_Popup_Splash_AttackPower>(WM->CreateUserWidget(EWidgetBluePrintEnum::WBP_ChangeAttackPowerSplash));

		if (AttackSplash->IsValidLowLevel())
		{
			if(!AttackSplash->IsInViewport())
				WM->AddToViewport(EWidgetBluePrintEnum::WBP_ChangeAttackPowerSplash, AttackSplash, SPLASHPOPUP_ZORDER + 1);
			

			AttackSplash->RefreshUI(InHeroID, InCurrentPower, InPrevPower);

			return AttackSplash;
		}
	}

	return nullptr;

}

bool UUIFunctionLibrary::ShowHero2D()
{
	URGameInstance* RGameInstance = RGAMEINSTANCE(GEngine);
	checkf(RGameInstance->RWidgetManager != nullptr, TEXT("%s[ RWidgetManager Is Empty ]"), __FUNCTION__);
	checkf(RGameInstance->RInventory != nullptr, TEXT("%s[ RInventory Is Empty ]"), __FUNCTION__);
	FString ud;
	if (RGameInstance->RInventory->GetNewHeroData(ud))
	{
		FHERO* outHero = RGameInstance->RInventory->GetHeroWithUD(ud);
		if (outHero != nullptr)
		{
			UUIFunctionLibrary::ShowCommonPopupHeroRuneSplash(outHero->heroId,
				UUtilFunctionLibrary::GetLocalizedString(EStringTableType::VE_UI, TEXT("UI_Notic_000000")),
				UUtilFunctionLibrary::GetLocalizedString(EStringTableType::VE_UI, TEXT("USK_up_getNewHero")));
			return true;
		}
	}

	return false;
}



void UUIFunctionLibrary::ReadyToStartUI(FString phase)
{
	auto RGameInstance = RGAMEINSTANCE(GEngine);
	checkf(RGameInstance->RWidgetManager != nullptr, TEXT("%s[ RWidgetManager Is Empty ]"), __FUNCTION__);
	checkf(RGameInstance->EventManager != nullptr, TEXT("%s[ EventManager Is Empty ]"), __FUNCTION__);
	RGameInstance->RWidgetManager->ShowUserWidgetAutoCreate(EWidgetBluePrintEnum::WBP_ReadyToStart);

	if (phase.Len() == 0)
	{
		RGameInstance->EventManager->OnUI_ReadyToStart.Broadcast();
	}
	else
	{
		RGameInstance->EventManager->OnPhaseEvent.Broadcast(phase);
	}
}

void UUIFunctionLibrary::ReadyToStartUI_Campaign(FName stage)
{
	auto RGameInstance = RGAMEINSTANCE(GEngine);
	checkf(RGameInstance->RWidgetManager != nullptr, TEXT("%s[ RWidgetManager Is Empty ]"), __FUNCTION__);
	checkf(RGameInstance->EventManager != nullptr, TEXT("%s[ EventManager Is Empty ]"), __FUNCTION__);
	RGameInstance->RWidgetManager->ShowUserWidgetAutoCreate(EWidgetBluePrintEnum::WBP_CampaignStart);

	UUserWidget* widget = RGameInstance->RWidgetManager->GetUserWidget(EWidgetBluePrintEnum::WBP_CampaignStart);
	UUC_Campaign_Start* CampStart = Cast<UUC_Campaign_Start>(widget);
	if (IsValid(CampStart))
	{
		CampStart->Init(stage);
	}	

	RGameInstance->EventManager->OnUI_ReadyToStart.Broadcast();
}

void UUIFunctionLibrary::ReadyToStartUI_PVP()
{
	auto RGameInstance = RGAMEINSTANCE(GEngine);
	checkf(RGameInstance->RWidgetManager != nullptr, TEXT("%s[ RWidgetManager Is Empty ]"), __FUNCTION__);
	checkf(RGameInstance->EventManager != nullptr, TEXT("%s[ EventManager Is Empty ]"), __FUNCTION__);
	RGameInstance->RWidgetManager->ShowUserWidgetAutoCreate(EWidgetBluePrintEnum::WBP_PVP_ReadyToStart);
	RGameInstance->EventManager->OnUI_ReadyToStart.Broadcast();
}

//////////////////////////////////////////////////////////////////////////
// Currency warning message
//////////////////////////////////////////////////////////////////////////
FText UUIFunctionLibrary::GetCurrentyWarningMessage(FName InRewardType)
{
	if( InRewardType == WALLET_GOLD)
		return UUtilFunctionLibrary::GetLocalizedString(EStringTableType::VE_UI, TEXT("USK_up_ItemManage_tip07"));
	else if( InRewardType == WALLET_GEM)
		return UUtilFunctionLibrary::GetLocalizedString(EStringTableType::VE_UI, TEXT("UI_Currency_Warning_Gem"));
// 	switch (InRewardType)
// 	{
// 		case EWALLET_TYPE::GOLD:
// 			
// 		case EWALLET_TYPE::GEM:
// 			
// // 		case EREWARD_TYPE::SP1:
// // 		case EREWARD_TYPE::REDSTONE:
// // 			return UUtilFunctionLibrary::GetLocalizedString(EStringTableType::VE_UI, TEXT("UI_Currency_Warning_SP1"));
// // 		case EREWARD_TYPE::SP2:
// // 			return UUtilFunctionLibrary::GetLocalizedString(EStringTableType::VE_UI, TEXT("UI_Currency_Warning_SP2"));
// // 		case EREWARD_TYPE::SP3:
// // 		case EREWARD_TYPE::PVPTOKEN:
// // 			return UUtilFunctionLibrary::GetLocalizedString(EStringTableType::VE_UI, TEXT("UI_Currency_Warning_SP3"));
// // 		case EREWARD_TYPE::SP4:
// // 			return UUtilFunctionLibrary::GetLocalizedString(EStringTableType::VE_UI, TEXT("UI_Currency_Warning_SP4"));
// 	}

	return FText::FromString( TEXT("Not enough currency") );
}

FVector2D UUIFunctionLibrary::GetWidgetPos(UUserWidget* InWidget)
{
	if (IsValid(InWidget) == false)
		return FVector2D::ZeroVector;

	UCanvasPanelSlot* CanvasPanel = Cast< UCanvasPanelSlot >(InWidget->Slot);
	if (CanvasPanel)
	{
		FVector2D canvasPos = CanvasPanel->GetPosition();
		FAnchors slotAnchor = CanvasPanel->GetAnchors();
		FVector2D slotSize = CanvasPanel->GetSize();
		FVector2D slotAlignment = CanvasPanel->GetAlignment();
		
		FVector2D viewSize;
		InWidget->GetWorld()->GetGameViewport()->GetViewportSize(viewSize);
		float viewScale = UWidgetLayoutLibrary::GetViewportScale(InWidget->GetWorld());

		float xPos = (slotAnchor.Minimum.X * viewSize.X / viewScale) + canvasPos.X - (slotSize.X * slotAlignment.X);
		float yPos = (slotAnchor.Minimum.Y * viewSize.Y / viewScale) + canvasPos.Y - (slotSize.Y * slotAlignment.Y);

		return FVector2D(xPos, yPos);
	}

	return FVector2D::ZeroVector;
}

FVector2D UUIFunctionLibrary::GetPanelWidgetPos(UWidget* InWidget)
{
	FVector2D widgetPos = FVector2D::ZeroVector;

	if (IsValid(InWidget))
	{
		UCanvasPanelSlot* CanvasPanel = Cast< UCanvasPanelSlot >(InWidget->Slot);
		if (CanvasPanel)
		{
			FVector2D canvasPos = CanvasPanel->GetPosition();
			FAnchors slotAnchor = CanvasPanel->GetAnchors();
			FVector2D slotSize = CanvasPanel->GetSize();
			FVector2D slotAlignment = CanvasPanel->GetAlignment();

			FVector2D viewSize;
			InWidget->GetWorld()->GetGameViewport()->GetViewportSize(viewSize);
			float viewScale = UWidgetLayoutLibrary::GetViewportScale(InWidget->GetWorld());

			float xPos = (slotAnchor.Minimum.X * viewSize.X / viewScale) + canvasPos.X - (slotSize.X * slotAlignment.X);
			float yPos = (slotAnchor.Minimum.Y * viewSize.Y / viewScale) + canvasPos.Y - (slotSize.Y * slotAlignment.Y);

			widgetPos = FVector2D(xPos, yPos);
		}
	}

	return widgetPos;
}

FVector2D UUIFunctionLibrary::GetPanelWidgetSize(UWidget* InWidget)
{
	FVector2D widgetSize = FVector2D::ZeroVector;

	if (IsValid(InWidget))
	{
		UCanvasPanelSlot* CanvasPanel = Cast< UCanvasPanelSlot >(InWidget->Slot);
		if (CanvasPanel)
		{
			FVector2D slotSize = CanvasPanel->GetSize();
			float viewScale = UWidgetLayoutLibrary::GetViewportScale(InWidget->GetWorld());

			widgetSize = slotSize * viewScale;
		}
	}

	return widgetSize;
}

bool UUIFunctionLibrary::GoContents(EUIMenuIDEnum idtype, int32 idx)
{
	auto RGameInstance = RGAMEINSTANCE(GEngine);
	FTimerDelegate ContentsCallback;
	ContentsCallback.BindLambda([RGameInstance, idtype]
	{
		UArenaManager* ArenaManager = RGameInstance->ArenaManager;
		URWidgetManager* RWidgetManager = RGameInstance->RWidgetManager;
		URInventory*	RInventory = RGameInstance->RInventory;
		checkf(RGameInstance->ContentsUnLockManager != nullptr, TEXT("%s[ RGameInstance->ContentsUnLockManager Is Empty ]"), __FUNCTION__);
		checkf(RGameInstance->NotifyManager != nullptr, TEXT("%s[ RGameInstance->NotifyManager Is Empty ]"), __FUNCTION__);
		checkf(RWidgetManager != nullptr, TEXT("%s[ RWidgetManager Is Empty ]"), __FUNCTION__);
		checkf(ArenaManager != nullptr, TEXT("%s[ ArenaManager Is Empty ]"), __FUNCTION__);
		checkf(RInventory != nullptr, TEXT("%s[ RInventory Is Empty ]"), __FUNCTION__);

		switch (idtype)
		{
		case EUIMenuIDEnum::VE_Raid_List:
			if (RGameInstance->NotifyManager->IsNotify(ENotifyStateFlagEnum::BM_Raid))
			{
				RGameInstance->NotifyManager->SetNotify(ENotifyStateFlagEnum::BM_Raid, false);
			}
			break;
		}


		EUIPageEnum UIPage = ContentToPage(idtype);

		/*if (UIPage == EUIPageEnum::UIPage_Shop || UIPage == EUIPageEnum::UIPage_ShopHero)
		{
			RWidgetManager->ChangeUIPage(UIPage);
			return;
		}*/

		if (UIPage == EUIPageEnum::UIPage_MiniGame)
		{
			UUIFunctionLibrary::ShowCommonPopupSplash(UUtilFunctionLibrary::GetLocalizedString(EStringTableType::VE_UI, TEXT("Content_Lock_Result")), FText());
			return;
		}

		if (UIPage == EUIPageEnum::UIPage_MissionSystem)
		{
			RGameInstance->EventManager->GetNoMissionSignal.Broadcast();
			return;
		}
		auto CurUIPage = RWidgetManager->GetCurrentUIPage();

		if (CurUIPage == EUIPageEnum::UIPage_HeroInventory)
			if (UIPage == EUIPageEnum::UIPage_LobbyOutdoor)
				RGameInstance->GetTowerLobbyGameMode()->CurrentLobbyState = ETowerLobbyStateEnum::Indoor;
		if(CurUIPage == EUIPageEnum::UIPage_TowerBossLobby)
			RGameInstance->GetTowerLobbyGameMode()->CurrentLobbyState = ETowerLobbyStateEnum::Indoor;

		GoUIPage(UIPage);
	});
	
	RGameInstance->GetWorld()->GetTimerManager().SetTimerForNextTick(ContentsCallback);//SetTimer(RGameInstance->TimerHandler, ContentsCallback, 0.01f, false);
	return true;
}

EUIPageEnum UUIFunctionLibrary::ContentToPage(EUIMenuIDEnum Content)
{
	switch (Content)
	{
	case EUIMenuIDEnum::VE_MainLobby:
		if (RGAMEINSTANCE(GEngine)->GetTowerLobbyGameMode())
		{
			// content VE_MainLobby means either LobbyOutdoor or LobbyIndoor
			switch (RGAMEINSTANCE(GEngine)->GetTowerLobbyGameMode()->CurrentLobbyState)
			{
			case ETowerLobbyStateEnum::Outdoor:
				return EUIPageEnum::UIPage_LobbyOutdoor;
				break;
			case ETowerLobbyStateEnum::Indoor:
				return EUIPageEnum::UIPage_LobbyIndoor;
				break;
			default:
				UE_LOG(LogLobby, Log, TEXT("UUIFunctionLibrary::ContentToPage: fix me please"));
				return EUIPageEnum::UIPage_LobbyOutdoor;
			}
		}
		return EUIPageEnum::UIPage_LobbyOutdoor;
		break;
	case EUIMenuIDEnum::VE_DetailHeros:
		return EUIPageEnum::UIPage_HeroManagement_Renewal;
		break;
	case EUIMenuIDEnum::VE_HeroInventory:
		return EUIPageEnum::UIPage_HeroManagement_Renewal;
		break;
	case EUIMenuIDEnum::VE_HeroGrind:
		return EUIPageEnum::UIPage_HeroGrind;
		break;
	case EUIMenuIDEnum::VE_Mission:
		return EUIPageEnum::UIPage_Daily_Quest;
		break;
	case EUIMenuIDEnum::VE_Campaign:
		return EUIPageEnum::UIPage_Campaign;
		break;
	case EUIMenuIDEnum::VE_Arena:
		if (RGAMEINSTANCE(GEngine)->ArenaManager->isDefenceSetting())
			return EUIPageEnum::UIPage_ArenaDeckSetting;
		else
			return EUIPageEnum::UIPage_ArenaLobby;
		break;
	case EUIMenuIDEnum::VE_ItemManagement:
		return EUIPageEnum::UIPage_ItemManagement;
		break;
	case EUIMenuIDEnum::VE_ItemManufacture:
		return EUIPageEnum::UIPage_ItemManufacture;
		break;
	case EUIMenuIDEnum::VE_Shop:
		return EUIPageEnum::UIPage_Shop;
		break;
	case EUIMenuIDEnum::VE_ShopHero:
		return EUIPageEnum::UIPage_ShopHero;
	case EUIMenuIDEnum::VE_TimeTreasure:
		return EUIPageEnum::UIPage_TimeTreasure;
	case EUIMenuIDEnum::VE_Colosseum:
		return EUIPageEnum::UIPage_ColosseumLobby;
		break;
	case EUIMenuIDEnum::VE_Crusader:
		return EUIPageEnum::UIPage_CrusaderLobby;
		break;
	case EUIMenuIDEnum::VE_BattleField:
		return EUIPageEnum::UIPage_BattleFieldLobby;
		break;
	case EUIMenuIDEnum::VE_Guild:
		return EUIPageEnum::UIPage_GuildLobby;
		break;
	case EUIMenuIDEnum::VE_WeekDungeon:
		return EUIPageEnum::UIPage_WeekDungeon;
		break;
	case EUIMenuIDEnum::VE_WeekStageSelect:
		return EUIPageEnum::UIPage_WeekStageSelect;
		break;
	case EUIMenuIDEnum::VE_GigantRaid:
	case EUIMenuIDEnum::VE_AkhanRaid:
	case EUIMenuIDEnum::VE_MorsRaid:
		return EUIPageEnum::UIPage_Raid_Select;
		break;
	case EUIMenuIDEnum::VE_Raid_List:
		return EUIPageEnum::UIPage_Raid_List;
		break;
// 	case EUIMenuIDEnum::VE_Quest:
// 		return EUIPageEnum::UIPage_QuestGuide;
// 		break;
	case EUIMenuIDEnum::VE_HeroRune:
		return EUIPageEnum::UIPage_HeroRune_EpisodeDetail;
		break;
	case EUIMenuIDEnum::VE_HeroRuneSelect:
		return EUIPageEnum::UIPage_HeroRune_SelectHero;
		break;
	case EUIMenuIDEnum::VE_Mail:
		return EUIPageEnum::UIPage_MailBox;
		break;
	case EUIMenuIDEnum::VE_BattleGround:
		return EUIPageEnum::UIPage_BattleGroundGuide;
		break;
	case EUIMenuIDEnum::VE_Avatar:
		return EUIPageEnum::UIPage_Avatar;
		break;
	case EUIMenuIDEnum::VE_Item:
		return EUIPageEnum::UIPage_Bag;
		break;
	case EUIMenuIDEnum::VE_MirrorDungeon:
		if (RGAMEINSTANCE(GEngine)->MirrorDungeonManager->IsMirrorDefenceSetting())
			return EUIPageEnum::UIPage_MirrorDeckSetting;
		else
			return EUIPageEnum::UIPage_MirrorDungeon;
	case EUIMenuIDEnum::VE_FloorShop:
		return EUIPageEnum::UIPage_FloorShop;
	case EUIMenuIDEnum::VE_RaildWorldBoss:
		return EUIPageEnum::UIPage_RaidWorldBoss;
	case EUIMenuIDEnum::VE_TowerBoss:
		return EUIPageEnum::UIPage_TowerBossLobby;
	case EUIMenuIDEnum::VE_MissionSystem:
		return EUIPageEnum::UIPage_MissionSystem;
	case EUIMenuIDEnum::VE_Achievements:
		return EUIPageEnum::UIPage_Daily_Quest;
	case EUIMenuIDEnum::VE_MiniGame:
		return EUIPageEnum::UIPage_MiniGame;
	case EUIMenuIDEnum::VE_SingleRaid:
		return EUIPageEnum::UIPage_RaidLobby;
	case EUIMenuIDEnum::VE_MultiRaid:
		return EUIPageEnum::UIPage_RaidLobby;
	default:
		UE_LOG(LogLobby, Log, TEXT("UUIFunctionLibrary::ContentToPage: fix me"));
		return EUIPageEnum::UIPage_LobbyOutdoor;
	}
}

void UUIFunctionLibrary::GoUIPage(EUIPageEnum UIPage)
{
	//RGAMEINSTANCE(GEngine)->LoadingSceneManager->HideLoadingScene();

	UTowerLobbyState* LobbyTowerState = Cast<UTowerLobbyState>(RGAMEINSTANCE(GEngine)->GetCurrentState());
	if (!IsValid(LobbyTowerState))
	{
		UE_LOG(LogLobby, Log, TEXT("UUIFunctionLibrary::UpdateLobbyStateByUIPage: called while not in LobbyTowerState"));
		return;
	}
	if (IsValid(LobbyTowerState->GetTowerLobbyGameMode()))
	{
		// cancel LoadingSceneManager->HideLoadingScene if TowerLobbyGameMode entry is not yet done
		if (LobbyTowerState->GetTowerLobbyGameMode()->CurrentLobbyState == ETowerLobbyStateEnum::None
			&& LobbyTowerState->IsEntryState())
		{
			RGAMEINSTANCE(GEngine)->LoadingSceneManager->ShowLoadingScene(EGamePlayModeTypeEnum::VE_Campaign);
		}
	}

	LobbyTowerState->OnLobbyStateUpdated.BindStatic(&UUIFunctionLibrary::OnLobbyStateUpdated, UIPage);
	
	if (IsValid(RGAMEINSTANCE(GEngine)->RWidgetManager))
		RGAMEINSTANCE(GEngine)->RWidgetManager->SetUIVisibility(false);

	switch (UIPage)
	{
	case EUIPageEnum::UIPage_Campaign:
		if (LobbyTowerState->IsIndoorStart())
			LobbyTowerState->GetTowerLobbyGameMode()->CurrentLobbyState = ETowerLobbyStateEnum::Campaign;
		LobbyTowerState->SetIndoorStart(false);
		LobbyTowerState->CampaignInTransition();
		break;
	case EUIPageEnum::UIPage_LobbyIndoor:
		LobbyTowerState->ChangeLobbyState(ETowerLobbyStateEnum::Indoor, ETowerLobbySubstateEnum::NormalView);
		break;
	case EUIPageEnum::UIPage_LobbyOutdoor:
		LobbyTowerState->ChangeLobbyState(ETowerLobbyStateEnum::Outdoor, ETowerLobbySubstateEnum::NormalView);
		break;
	case EUIPageEnum::UIPage_LobbyOutdoorPortraitMode:
		LobbyTowerState->ChangeLobbyState(ETowerLobbyStateEnum::Outdoor, ETowerLobbySubstateEnum::PortraitView);
		break;
	case EUIPageEnum::UIPage_HeroManagement_Renewal:
		LobbyTowerState->ChangeLobbyState(ETowerLobbyStateEnum::Indoor, ETowerLobbySubstateEnum::HeroManagementView);
		break;
	case EUIPageEnum::UIPage_ShopHero:
		LobbyTowerState->ChangeLobbyState(ETowerLobbyStateEnum::Indoor, ETowerLobbySubstateEnum::HeroShop);
		break;
	case EUIPageEnum::UIPage_TowerBossLobby:
		LobbyTowerState->ChangeLobbyState(ETowerLobbyStateEnum::Indoor, ETowerLobbySubstateEnum::RaidTowerBoss);
		break;
	default:
		// TODO: currently not going anywhere...
		LobbyTowerState->OnLobbyStateUpdated.Unbind();
		OnLobbyStateUpdated(UIPage);
		return;
	}
}

void UUIFunctionLibrary::ChangeUIPage(EUIPageEnum UIPage)
{
	URGameInstance* RGameInstance = RGAMEINSTANCE(GEngine);
	if (!IsValid(RGAMEINSTANCE(GEngine)->RWidgetManager))
		return;

	RGAMEINSTANCE(GEngine)->RWidgetManager->ChangeUIPage(UIPage);
}

void UUIFunctionLibrary::OnLobbyStateUpdated(EUIPageEnum UIPage)
{
	UTowerLobbyState* LobbyTowerState = Cast<UTowerLobbyState>(RGAMEINSTANCE(GEngine)->GetCurrentState());
	if(LobbyTowerState)
		LobbyTowerState->OnLobbyStateUpdated.Unbind();

	URGameInstance* RGameInstance = RGAMEINSTANCE(GEngine);
	if (!IsValid(RGAMEINSTANCE(GEngine)->RWidgetManager))
		return;

	if(UIPage != EUIPageEnum::UIPage_Campaign)
		RGameInstance->RWidgetManager->SetUIVisibility(true);

	if (RGameInstance->RWidgetManager->IsInUIEvent(EUIEventEnum::VE_GoContent))
	{
		FUIEventInfo* EventInfo = RGameInstance->RWidgetManager->GetUIEventInfo(EUIEventEnum::VE_GoContent);
		if (EventInfo != nullptr)
		{
			EUIMenuIDEnum IDEnum = (EUIMenuIDEnum)(EventInfo->IValue);
			RGameInstance->RWidgetManager->RemoveUIEvent(EUIEventEnum::VE_GoContent);
			GoContents(IDEnum);
		}
		return;
	}
	//else if (RGameInstance->RWidgetManager->IsInUIEvent(EUIEventEnum::VE_HeroShop)) // need to be fixed later
	//{
	//	URGameInstance* GameInst = RGAMEINSTANCE(GEngine);
	//	FStreamableDelegate Callback;
	//	Callback.BindLambda([GameInst]() {
	//		GameInst->ResourceCacheManager->OnAsyncLoadedCompleted.ExecuteIfBound();

	//		UUIFunctionLibrary::GoUIPage(EUIPageEnum::UIPage_ShopHero);
	//	});
	//	GameInst->ResourceCacheManager->cache_HeroShop(Callback);
	//	return;
	//}

	
	RGameInstance->RWidgetManager->ChangeUIPage(UIPage);
}

FName UUIFunctionLibrary::GetContentsName(EUIMenuIDEnum InMenuID)
{
	UEnum* UEnumType = FindObject<UEnum>(ANY_PACKAGE, TEXT("EUIMenuIDEnum"), true);
	FString EnumString = UEnumType->GetNameStringByIndex((int32)InMenuID);

	return FName(*EnumString);
}

UTexture2D* UUIFunctionLibrary::GetTextureSkillLevelCircle(int32 SkillLevel)
{
	auto SkillLevelStr = FString::FromInt(SkillLevel);

	auto LevelCircleTextureKey = HEROSKILLLEVELCIRCLE + SkillLevelStr;

	auto Texture = UUtilFunctionLibrary::GetTexture2DFromTable(FName(*LevelCircleTextureKey));
	if (Texture == nullptr)
	{
		auto TempLevelCircleTextureKey = HEROSKILLLEVELCIRCLE + FString::FromInt(1);
		Texture = UUtilFunctionLibrary::GetTexture2DFromTable(FName(*TempLevelCircleTextureKey));
	}
	return Texture;
}

UTexture2D* UUIFunctionLibrary::GetTextureSkillCircle(int32 SkillLevel)
{
	FString SkillLevelStr = FString::FromInt(SkillLevel);

	auto CircleTextureKey = HEROSKILLCIRCLE + SkillLevelStr;

	auto Texture = UUtilFunctionLibrary::GetTexture2DFromTable(FName(*CircleTextureKey));
	if (Texture == nullptr)
	{
		auto TempCircleTextureKey = HEROSKILLCIRCLE + FString::FromInt(1);
		Texture = UUtilFunctionLibrary::GetTexture2DFromTable(FName(*TempCircleTextureKey));
	}
	return Texture;
}

UTexture2D* UUIFunctionLibrary::GetTextureSkillAttribute(ESkillAttributeEnum InSkillAttribute)
{
	switch (InSkillAttribute)
	{
	case ESkillAttributeEnum::VE_Magical:
		return UUtilFunctionLibrary::GetTexture2DFromTable("Skill_Attribute_Lightning");
	case ESkillAttributeEnum::VE_Physical:
		return UUtilFunctionLibrary::GetTexture2DFromTable("Skill_Attribute_Physical");
	default:
		return nullptr;
	}

	return nullptr;
}

UTexture2D* UUIFunctionLibrary::GetTextureSkillShapeAttribute(const FWorkedSkillTableInfo&	CurrentWorkedSkillTableInfo)
{
	//Skill_Coverage
	float Degree = 0.0f;

	switch (CurrentWorkedSkillTableInfo.SkillTableInfo_V2->EffectShape)
	{
	case ESearchShapeTypeEnum::VE_Circle:
		return UUtilFunctionLibrary::GetTexture2DFromTable("Skill_Shape_Circle");
		break;
	case ESearchShapeTypeEnum::VE_Sector:
		if (CurrentWorkedSkillTableInfo.SkillTableInfo_V2->ShapeScope.IsValidIndex(1) == false)
		{
			return nullptr;
		}

		Degree = CurrentWorkedSkillTableInfo.SkillTableInfo_V2->ShapeScope[1];
		if (0.0f <= Degree && Degree <= 120.f)
		{
			return UUtilFunctionLibrary::GetTexture2DFromTable("Skill_Shape_Sector_90Degree");
		}
		else
		{
			return UUtilFunctionLibrary::GetTexture2DFromTable("Skill_Shape_Sector_180Degree");
		}
		break;
	case ESearchShapeTypeEnum::VE_Box:
		return UUtilFunctionLibrary::GetTexture2DFromTable("Skill_Shape_Box");
		break;
	default:
		break;
	}

	return nullptr;
}

FText UUIFunctionLibrary::GetSkillAttributeText(ESkillAttributeEnum InSkillAttribute)
{
	switch (InSkillAttribute)
	{
	case ESkillAttributeEnum::VE_Magical:
		return UUtilFunctionLibrary::GetLocalizedString(EStringTableType::VE_UI, "UI_HeroUI_Attribute_Magical");
	case ESkillAttributeEnum::VE_Physical:
		return UUtilFunctionLibrary::GetLocalizedString(EStringTableType::VE_UI, "UI_HeroUI_Attribute_Physical");
	default:
		return FText();
	}

	return FText();
}


UUserWidget* UUIFunctionLibrary::CreateUserWidget(EWidgetBluePrintEnum WidgetBluePrint)
{
	auto RGameInstance = RGAMEINSTANCE(GEngine);
	checkf(RGameInstance->RWidgetManager != nullptr, TEXT("%s[ RGameInstance Is Empty ]"), __FUNCTION__);
	UUserWidget* Widget = RGameInstance->RWidgetManager->GetUserWidget(WidgetBluePrint);
	if (!IsValid( Widget) )
		Widget = RGameInstance->RWidgetManager->CreateUserWidget(WidgetBluePrint);
	if(IsValid(Widget))
		RGameInstance->RWidgetManager->ShowUserWidget(WidgetBluePrint);
	return Widget;
}

FName UUIFunctionLibrary::GetSkillLevelUpKey(FString heroUD, int32 InLevel)
{
	FName returnValue;
	auto RGameInstance = RGAMEINSTANCE(GEngine);
	checkf(RGameInstance->RInventory != nullptr, TEXT("%s[ RInventory Is Empty ]"), __FUNCTION__);
	auto HeroInfo = RGameInstance->RInventory->GetHeroWithUD(heroUD);
	if (HeroInfo == nullptr)
		return returnValue;

	auto UICharacterTableInfo = URenewal_BattleFunctionLibrary::GetCharacterTableInfo(ECharacterCategoryTypeEnum::VE_Hero, HeroInfo->heroId);
	checkf(UICharacterTableInfo != nullptr, TEXT("%s[ UICharacterTableInfo Is Empty ]"), __FUNCTION__);

	EClassTypeEnum HeroClass = UICharacterTableInfo->Class;
	FString HeroClassText;
	switch (HeroClass)
	{
	case EClassTypeEnum::VE_Tanker:
		HeroClassText = FString(TEXT("VE_Tanker"));
		break;
	case EClassTypeEnum::VE_MeleeDealer:
		HeroClassText = FString(TEXT("VE_MeleeDealer"));
		break;
	case EClassTypeEnum::VE_RangeDealer:
		HeroClassText = FString(TEXT("VE_RangeDealer"));
		break;
	case EClassTypeEnum::VE_Healer:
		HeroClassText = FString(TEXT("VE_Healer"));
		break;
	case EClassTypeEnum::VE_Buffer:
		HeroClassText = FString(TEXT("VE_Buffer"));
		break;
	case EClassTypeEnum::VE_Supporter:
		HeroClassText = FString(TEXT("VE_Supporter"));
		break;
	case EClassTypeEnum::VE_MagicDealer:
		HeroClassText = FString(TEXT("VE_MagicDealer"));
		break;

	default:
		break;
	}

	if (HeroClassText.IsEmpty() == false)
		returnValue = FName(*(HeroClassText + FString("_") + FString::FromInt(InLevel)));

	return returnValue;
}

FItemLevelUpTableInfo* UUIFunctionLibrary::GetItemLevelUpTableInfo(FITEM_ICON_INFO& InTarget, int32 InLevel /*= 1*/)
{
	int32 ResultValue = 0;
	FString ChnageString = FString::FromInt(ResultValue);
	FName Key = FName(*ChnageString);
	return UTableManager::GetInstancePtr()->GetItemLevelUpTableRow(Key);
}

FItemEnchantTableInfo* UUIFunctionLibrary::GetItemEnchantTableInfo(FITEM_ICON_INFO& InTarget)
{
	int32 ItemEnchantTableRowName = UUIFunctionLibrary::GetItemEnchantRowName(InTarget.ItemEquipType, InTarget.ItemEquipPosition, InTarget.itemGrade, InTarget.enchant);
	return UTableManager::GetInstancePtr()->GetItemEnchantRow(FName(*FString::FromInt(ItemEnchantTableRowName)));
}


bool UUIFunctionLibrary::ShowUIMediaFrameWork(FName MediaName, bool IsVisibleSkipButton)
{
	auto RGameInstance = RGAMEINSTANCE(GEngine);
	checkf(RGameInstance->UIMediaFrameWorkManager != nullptr, TEXT("%s[ UIMediaFrameWorkManager Is Empty ]"), __FUNCTION__);
	RGameInstance->UIMediaFrameWorkManager->ShowMedia(MediaName, IsVisibleSkipButton);
	return true;
}

void UUIFunctionLibrary::AddUIEvent(EUIEventEnum InEventType)
{
	auto RGameInstance = RGAMEINSTANCE(GEngine);
	checkf(RGameInstance->RWidgetManager != nullptr, TEXT("%s[ RGameInstance Is Empty ]"), __FUNCTION__);
	RGameInstance->RWidgetManager->AddUIEvent(InEventType);
}

void UUIFunctionLibrary::AddUIEventInt(EUIEventEnum InEventType, int32 InValue)
{
	auto RGameInstance = RGAMEINSTANCE(GEngine);
	checkf(RGameInstance->RWidgetManager != nullptr, TEXT("%s[ RGameInstance Is Empty ]"), __FUNCTION__);
	RGameInstance->RWidgetManager->AddUIEventInt(InEventType, InValue);
}

void UUIFunctionLibrary::AddUIEventString(EUIEventEnum InEventType, FString InValue)
{
	auto RGameInstance = RGAMEINSTANCE(GEngine);
	checkf(RGameInstance->RWidgetManager != nullptr, TEXT("%s[ RGameInstance Is Empty ]"), __FUNCTION__);
	RGameInstance->RWidgetManager->AddUIEventString(InEventType, InValue);
}

void UUIFunctionLibrary::AddUIEventName(EUIEventEnum InEventType, FName InValue)
{
	auto RGameInstance = RGAMEINSTANCE(GEngine);
	checkf(RGameInstance->RWidgetManager != nullptr, TEXT("%s[ RGameInstance Is Empty ]"), __FUNCTION__);
	RGameInstance->RWidgetManager->AddUIEventName(InEventType, InValue);
}

FVector2D UUIFunctionLibrary::GetGameResolution()
{
	FVector2D Result = FVector2D(1, 1);

	Result.X = GSystemResolution.ResX;
	Result.Y = GSystemResolution.ResY;

	return Result;
}

bool UUIFunctionLibrary::IsEnableUsingCurrency(URGameInstance* InRGameInstance, FName InType, int32 consumeValue)
{
	checkf(InRGameInstance != nullptr, TEXT("%s[ RGameInstance Is Empty ]"), __FUNCTION__);
	checkf(InRGameInstance->RWidgetManager != nullptr, TEXT("%s[ RGameInstance Is RWidgetManager ]"), __FUNCTION__);
	FAVATAR& Avatar = InRGameInstance->UserInfo->GetAvatarData();
	if (InType == WALLET_GOLD && InRGameInstance->RInventory->GetGold() > consumeValue)
	{
		InRGameInstance->RWidgetManager->ShowUserWidgetAutoCreate(EWidgetBluePrintEnum::WBP_Popup_ItemShortage);
		auto Widget = InRGameInstance->RWidgetManager->GetUserWidget(EWidgetBluePrintEnum::WBP_Popup_ItemShortage);
		if (Widget)
		{
			auto ShortagePopup = Cast<UUP_ItemShortagePopup>(Widget);
			ShortagePopup->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
			float GoldValue = consumeValue - InRGameInstance->RInventory->GetGold();
			ShortagePopup->SetNecessaryGold(GoldValue);
			auto Caption = UUtilFunctionLibrary::GetLocalizedString(EStringTableType::VE_UI, COMMONSTRING_NOTIFY);
			auto Desc = UUtilFunctionLibrary::GetLocalizedString(EStringTableType::VE_UI, "UI_HeroManage_Notify2");
			FFormatOrderedArguments Args;
			auto DescTemp = UUtilFunctionLibrary::GetLocalizedString(EStringTableType::VE_UI, "USK_up_Result_gold");
			Args.Emplace(DescTemp);
			Args.Emplace(GoldValue);
			Desc = FText::Format(Desc, Args);
			ShortagePopup->LocalText_Caption->SetText(Caption);
			ShortagePopup->LocalText_Desc->SetText(Desc);
		}
		return false;
	}

	if (InType == WALLET_GEM && InRGameInstance->RInventory->GetGem() < consumeValue )
	{
		InRGameInstance->RWidgetManager->ShowUserWidgetAutoCreate(EWidgetBluePrintEnum::WBP_Popup_ItemShortage);
		auto Widget = InRGameInstance->RWidgetManager->GetUserWidget(EWidgetBluePrintEnum::WBP_Popup_ItemShortage);
		if(Widget)
		{
			auto ShortagePopup = Cast<UUP_ItemShortagePopup>(Widget);
			ShortagePopup->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
			float GemValue = consumeValue - InRGameInstance->RInventory->GetGem();
			ShortagePopup->SetNecessaryGem(GemValue);
			ShortagePopup->LocalText_Caption->SetText(UUtilFunctionLibrary::GetLocalizedString(EStringTableType::VE_UI, COMMONSTRING_NOTIFY));
			ShortagePopup->LocalText_Desc->SetText(UUtilFunctionLibrary::GetLocalizedString(EStringTableType::VE_UI, "USK_up_ItemManage_OptionWarning"));
		}
		return false;
	}
		
	return true;
}

bool UUIFunctionLibrary::LackCurrencyPopup(URGameInstance* InRGameInstance, TArray<FString>& params)
{
	checkf(InRGameInstance != nullptr, TEXT("%s[ RGameInstance Is Empty ]"), __FUNCTION__);
	checkf(InRGameInstance->RWidgetManager != nullptr, TEXT("%s[ RGameInstance Is RWidgetManager ]"), __FUNCTION__);
	if (params[0] == TEXT("GEMS"))
	{
		InRGameInstance->RWidgetManager->ShowUserWidgetAutoCreate(EWidgetBluePrintEnum::WBP_Popup_ItemShortage);
		auto Widget = InRGameInstance->RWidgetManager->GetUserWidget(EWidgetBluePrintEnum::WBP_Popup_ItemShortage);
		if(Widget)
		{
			auto ShortagePopup = Cast<UUP_ItemShortagePopup>(Widget);
			ShortagePopup->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
			float GemValue = FCString::Atof(*params[2]) * -1;
			ShortagePopup->SetNecessaryGem(GemValue);
			ShortagePopup->LocalText_Caption->SetText(UUtilFunctionLibrary::GetLocalizedString(EStringTableType::VE_UI, COMMONSTRING_NOTIFY));
			ShortagePopup->LocalText_Desc->SetText(UUtilFunctionLibrary::GetLocalizedString(EStringTableType::VE_UI, "USK_up_ItemManage_OptionWarning"));
			return true;
		}
	}
	else if (params[0] == TEXT("GOLD"))
	{
		InRGameInstance->RWidgetManager->ShowUserWidgetAutoCreate(EWidgetBluePrintEnum::WBP_Popup_ItemShortage);
		auto Widget = InRGameInstance->RWidgetManager->GetUserWidget(EWidgetBluePrintEnum::WBP_Popup_ItemShortage);
		if(Widget)
		{
			auto ShortagePopup = Cast<UUP_ItemShortagePopup>(Widget);
			ShortagePopup->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
			float GoldValue = FCString::Atof(*params[2]) * -1;
			ShortagePopup->SetNecessaryGold(GoldValue);
			auto Caption = UUtilFunctionLibrary::GetLocalizedString(EStringTableType::VE_UI, COMMONSTRING_NOTIFY);
			auto Desc = UUtilFunctionLibrary::GetLocalizedString(EStringTableType::VE_UI, "UI_HeroManage_Notify2");
			FFormatOrderedArguments Args;
			auto DescTemp = UUtilFunctionLibrary::GetLocalizedString(EStringTableType::VE_UI, "USK_up_Result_gold");
			Args.Emplace(DescTemp);
			Args.Emplace(GoldValue);
			Desc = FText::Format(Desc, Args);
			ShortagePopup->LocalText_Caption->SetText(Caption);
			ShortagePopup->LocalText_Desc->SetText(Desc);
			return true;
		}
	}
	else if (params[0] == TEXT("S_TOWERBOSS_F1"))
	{
		UUIFunctionLibrary::ShowCommonPopup(UUtilFunctionLibrary::GetLocalizedString(EStringTableType::VE_UI, COMMONSTRING_NOTIFY), UUtilFunctionLibrary::GetLocalizedString(EStringTableType::VE_UI, TEXT("UI_Common_NoTry")), ECommonPopupType::VE_Error);
		return true;
	}

	return false;
}

void UUIFunctionLibrary::RefreshTopbarProperty_Simple(URGameInstance* InRGameInstance)
{
	checkf(InRGameInstance != nullptr, TEXT("%s[ RGameInstance Is Empty ]"), __FUNCTION__);
	checkf(InRGameInstance->RWidgetManager != nullptr, TEXT("%s[ RGameInstance Is RWidgetManager ]"), __FUNCTION__);
	auto UserWidget = InRGameInstance->RWidgetManager->GetUserWidget(EWidgetBluePrintEnum::WBP_TopBar);
	if (UserWidget)
	{
		auto TopbarWidget = Cast<UTopBar>(UserWidget);
		TopbarWidget->RefreshProperty();
	}
}

void UUIFunctionLibrary::RefreshTopbarProperty_InShop(URGameInstance* InRGameInstance, int32 InShopTabIndex)
{
	checkf(InRGameInstance != nullptr, TEXT("%s[ RGameInstance Is Empty ]"), __FUNCTION__);
	checkf(InRGameInstance->RWidgetManager != nullptr, TEXT("%s[ RGameInstance Is RWidgetManager ]"), __FUNCTION__);
	auto RWidgetManager = InRGameInstance->RWidgetManager;
	auto UserWidget = RWidgetManager->GetUserWidget(EWidgetBluePrintEnum::WBP_TopBar);
	if (UserWidget)
	{
		auto TopbarWidget = Cast<UTopBar>(UserWidget);
		TopbarWidget->RefreshProperty_InShop(InShopTabIndex);
	}
}

FText UUIFunctionLibrary::Get_0n_Number(int32 InNumber)
{
	FString NumString = FString::FromInt(InNumber);

	if (InNumber < 10)
		NumString = FString(TEXT("0")) + NumString;

	return FText::FromString(NumString);
}

FName UUIFunctionLibrary::GetBlockAbnormalMsg(ESkillApplierTypeEnum InBlockApplierType)
{
	FName Key;
	switch (InBlockApplierType)
	{
	case ESkillApplierTypeEnum::VE_p_block_abnormal_debuff:		Key = FName("Block_Abnormal_Debuff");		break;
	case ESkillApplierTypeEnum::VE_p_block_abnormal_buff:		Key = FName("Block_Abnormal_Buff");			break;
	case ESkillApplierTypeEnum::VE_p_block_group_helplessness:	Key = FName("Block_Group_Helplessness");	break;
	case ESkillApplierTypeEnum::VE_p_block_group_dot_bleeding:	Key = FName("Block_Group_Dot_Bleeding");	break;
	case ESkillApplierTypeEnum::VE_p_block_group_dot_fire:		Key = FName("Block_Group_Dot_Fire");		break;
	case ESkillApplierTypeEnum::VE_p_block_group_dot_poison:	Key = FName("Block_Group_Dot_Poison");		break;
	case ESkillApplierTypeEnum::VE_p_block_slot_fear:			Key = FName("Block_Slot_Fear");				break;
	case ESkillApplierTypeEnum::VE_p_block_slot_ice:			Key = FName("Block_Slot_Ice");				break;
	case ESkillApplierTypeEnum::VE_p_block_slot_freezing:		Key = FName("Block_Slot_Freezing");			break;
	case ESkillApplierTypeEnum::VE_p_block_slot_down:			Key = FName("Block_Slot_Down");				break;
	case ESkillApplierTypeEnum::VE_p_block_slot_stun:			Key = FName("Block_Slot_Stun");				break;
	case ESkillApplierTypeEnum::VE_p_block_slot_hold:			Key = FName("Block_Slot_Hold");				break;
	case ESkillApplierTypeEnum::VE_p_block_slot_silence:		Key = FName("Block_Slot_Silence");			break;
	case ESkillApplierTypeEnum::VE_p_block_slot_provoke:		Key = FName("Block_Slot_Provoke");			break;
	case ESkillApplierTypeEnum::VE_p_block_slot_airborne:			Key = FName("Block_Airborne");				break;
	case ESkillApplierTypeEnum::VE_p_block_instant_damage:		Key = FName("Block_Instant_Damage");		break;
	case ESkillApplierTypeEnum::VE_p_block_physical_damage:		Key = FName("Block_Physical_Damage");		break;
	case ESkillApplierTypeEnum::VE_p_block_magical_damage:		Key = FName("Block_Magical_Damage");		break;
	case ESkillApplierTypeEnum::VE_p_block_melee_damage:		Key = FName("Block_Melee_Damage");			break;
	case ESkillApplierTypeEnum::VE_p_block_range_damage:		Key = FName("Block_Range_Damage");			break;
	case ESkillApplierTypeEnum::VE_p_block_pull:				Key = FName("Block_Pull_Push");				break;
	case ESkillApplierTypeEnum::VE_p_block_push:				Key = FName("Block_Pull_Push");				break;
	case ESkillApplierTypeEnum::VE_p_block_stat_up_ModelScale:			Key = FName("Block_Smallbody");				break;
	case ESkillApplierTypeEnum::VE_p_block_slot_captivate:			Key = FName("Block_Captivate");				break;
	}
	return Key;
}
