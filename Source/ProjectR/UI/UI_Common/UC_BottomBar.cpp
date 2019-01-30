// Fill out your copyright notice in the Description page of Project Settings.

#include "ProjectR.h"
#include "UC_BottomBar.h"

#include "GlobalIntegrated.h"


#include "UI/RWidgetManager.h"

#include "UtilFunctionIntegrated.h"
#include "Table/CharacterTableInfo.h"
#include "GameInfo/RInventory.h"
#include "UI/Common/UC_Notify.h"




void UUC_BottomBar::NativeConstruct()
{
	Super::NativeConstruct();

	RGameInstance = RGAMEINSTANCE(this);

	if (Button_HeroInfo)
	{
		Button_HeroInfo->OnClicked.RemoveDynamic(this, &UUC_BottomBar::OnClickHeroInfo);
		Button_HeroInfo->OnClicked.AddDynamic(this, &UUC_BottomBar::OnClickHeroInfo);
	}

	if (Button_Daily)
	{
		Button_Daily->OnClicked.RemoveDynamic(this, &UUC_BottomBar::OnClickDaily);
		Button_Daily->OnClicked.AddDynamic(this, &UUC_BottomBar::OnClickDaily);
	}

	if (Button_Achievement)
	{
		Button_Achievement->OnClicked.RemoveDynamic(this, &UUC_BottomBar::OnClickAchievement);
		Button_Achievement->OnClicked.AddDynamic(this, &UUC_BottomBar::OnClickAchievement);
	}

	if (Button_Friend)
	{
		Button_Friend->OnClicked.RemoveDynamic(this, &UUC_BottomBar::OnClickFriend);
		Button_Friend->OnClicked.AddDynamic(this, &UUC_BottomBar::OnClickFriend);
	}

	if (Button_Item)
	{
		Button_Item->OnClicked.RemoveDynamic(this, &UUC_BottomBar::OnClickItem);
		Button_Item->OnClicked.AddDynamic(this, &UUC_BottomBar::OnClickItem);
	}

	//

	if (Button_Grind)
	{
		Button_Grind->OnClicked.RemoveDynamic(this, &UUC_BottomBar::OnClickGrind);
		Button_Grind->OnClicked.AddDynamic(this, &UUC_BottomBar::OnClickGrind);
	}
	if (Btn_HeroManager)
	{
		Btn_HeroManager->OnClicked.RemoveDynamic(this, &UUC_BottomBar::OnClickHeroManager);
		Btn_HeroManager->OnClicked.AddDynamic(this, &UUC_BottomBar::OnClickHeroManager);
	}
	if (Btn_ItemCraft)
	{
		Btn_ItemCraft->OnClicked.RemoveDynamic(this, &UUC_BottomBar::OnClickItemManufacture);
		Btn_ItemCraft->OnClicked.AddDynamic(this, &UUC_BottomBar::OnClickItemManufacture);
	}
	if (Btn_Guild)
	{
		Btn_Guild->OnClicked.RemoveDynamic(this, &UUC_BottomBar::OnClickGuild);
		Btn_Guild->OnClicked.AddDynamic(this, &UUC_BottomBar::OnClickGuild);
	}
	if (Button_Quest)
	{
		Button_Quest->OnClicked.RemoveDynamic(this, &UUC_BottomBar::OnClickQuest);
		Button_Quest->OnClicked.AddDynamic(this, &UUC_BottomBar::OnClickQuest);
	}
	if (Button_QuestGuide)
	{
		Button_QuestGuide->OnClicked.RemoveDynamic(this, &UUC_BottomBar::OnClickQuestGuide);
		Button_QuestGuide->OnClicked.AddDynamic(this, &UUC_BottomBar::OnClickQuestGuide);
	}
}

void UUC_BottomBar::SetLobbyCharacter(FName InLobbyCharacterKey)
{
	FLobbyCharacterTableInfo* LobbyCharacterTableInfo = UTableManager::GetInstancePtr()->GetLobbyCharacterTableRow(InLobbyCharacterKey);
	if (LobbyCharacterTableInfo == nullptr)
		return;

	if (IsValid(Text_HeroName))
	{
		Text_HeroName->SetText(UUtilFunctionLibrary::GetLocalizedString(EStringTableType::VE_Hero, LobbyCharacterTableInfo->DisplayName));
	}

	if (IsValid(Text_HeroNick))
	{
		Text_HeroNick->SetText(UUtilFunctionLibrary::GetLocalizedString(EStringTableType::VE_Hero, LobbyCharacterTableInfo->Nick));
	}

	HeroId = LobbyCharacterTableInfo->HeroID;

	if (IsValid(Image_HeroJob))
	{
		if (HeroId.IsNone())
		{
			Image_HeroJob->SetVisibility(ESlateVisibility::Hidden);
		}
		else
		{
			FCharacterTableInfo* CharacterTableInfo = UTableManager::GetInstancePtr()->GetCharacterRow(HeroId);
			if (CharacterTableInfo)
			{
				Image_HeroJob->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
				UUIFunctionLibrary::SetImageBrush(UResourceFunctionLibrary::GetHeroClassTypeSprite(CharacterTableInfo->Class), Image_HeroJob);
			}
		}
	}

	if (IsValid(Panel_GotoHero))
	{
		if (HeroId.IsNone())
		{
			Panel_GotoHero->SetVisibility(ESlateVisibility::Hidden);
		}
		else
		{
			Panel_GotoHero->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
		}
	}

	//temp
	if (Panel_Noti && UC_Notify_New_Hero)
	{
		/*if (UC_Notify_New_Hero->GetVisibility() != ESlateVisibility::Collapsed)
		{
			Panel_Noti->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
		}
		else*/
		{
			Panel_Noti->SetVisibility(ESlateVisibility::Collapsed);
		}
	}
}

void UUC_BottomBar::OnClickHeroInfo()
{
	if (HeroId.IsNone())
		return;

	UUIFunctionLibrary::GoContents(EUIMenuIDEnum::VE_DetailHeros, 0);
}


void UUC_BottomBar::OnClickDaily()
{
	if (RGameInstance)
	{
		RGameInstance->AchievementManager->SetIsStartAchievement(false);
	}

	UUIFunctionLibrary::GoContents(EUIMenuIDEnum::VE_Mission, 0);
}


void UUC_BottomBar::OnClickAchievement()
{
	if (RGameInstance)
	{
		RGameInstance->AchievementManager->SetIsStartAchievement(true);
	}

	UUIFunctionLibrary::GoContents(EUIMenuIDEnum::VE_Mission, 0);
}

void UUC_BottomBar::OnClickFriend()
{
	if (RGameInstance)
	{
		RGameInstance->ContentsUnLockManager->ShowLockNotificationPopup(EOVERLORD::TUTORIAL);
	}
}

void UUC_BottomBar::OnClickItem()
{
// 	if (!IsValid(RGameInstance))
// 		return;

	if (RGameInstance->RWidgetManager)
	{
		UUserWidget* Widget = RGameInstance->RWidgetManager->GetUserWidgetAutoCreate(EWidgetBluePrintEnum::WBP_ItemView);
		if (!IsValid(Widget))
			return;

		if (!Widget->IsInViewport())
			RGameInstance->RWidgetManager->AddToViewport(EWidgetBluePrintEnum::WBP_ItemView, Widget, 0);


		return;
		
		// UP_Item_Management is replaced by UP_ItemManagement_NewDesign
		//UUP_Item_Management* UP_ItemManagement = Cast<UUP_Item_Management>(Widget);
		//if (UP_ItemManagement==nullptr)
		//	return;

		//FUIEventInfo UIEventInfo;
		//UIEventInfo.EventType = EUIEventEnum::VE_ItemPopup;
		//TArray<FString> InHeroUDList;
		//RGAMEINSTANCE(this)->RInventory->GetHeroUDs(InHeroUDList);

		//UIEventInfo.StringValue = "";
		//UIEventInfo.StringValue1 = InHeroUDList[0];
		//UIEventInfo.IValue = 0;

		//RGameInstance->RWidgetManager->UIEventQueue.EmplaceAt(0, UIEventInfo);
		//UP_ItemManagement->CurrentHeroUD = InHeroUDList[0];
		////UP_ItemManagement->ItemInventory->Button_Exit->SetVisibility(ESlateVisibility::Visible);
		//UP_ItemManagement->ItemInventory->SetParentWidget(UP_ItemManagement);
		//UP_ItemManagement->SetIsCheckException(true);

		//UP_ItemManagement->SetIsRefreshDetail(true);
		//
		//UP_ItemManagement->Init();
		//
		//UP_ItemManagement->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
		//UP_ItemManagement->CheckEmptyScrollItem();
		//UP_ItemManagement->SetIsCheckException(true);
		//UP_ItemManagement->ShowItemPopup();
	
		//
		//if (!UP_ItemManagement->IsInViewport())
		//	RGameInstance->RWidgetManager->AddToViewport(EWidgetBluePrintEnum::WBP_ItemManagement_Renewal, Widget, 0);
						
	}
}

void UUC_BottomBar::OnClickGrind()
{
	UUIFunctionLibrary::GoContents(EUIMenuIDEnum::VE_HeroGrind);
}

void UUC_BottomBar::OnClickHeroManager()
{
	UUIFunctionLibrary::GoContents(EUIMenuIDEnum::VE_HeroInventory);
}

void UUC_BottomBar::OnClickItemManufacture()
{
	UUIFunctionLibrary::GoContents(EUIMenuIDEnum::VE_ItemManufacture);
}

void UUC_BottomBar::OnClickGuild()
{
	UUIFunctionLibrary::GoContents(EUIMenuIDEnum::VE_Guild);
}

void UUC_BottomBar::OnClickQuestGuide()
{
// 	UUIFunctionLibrary::GoContents(EUIMenuIDEnum::VE_Quest);
}

void UUC_BottomBar::OnClickQuest()
{
	if (isCheckedQuest)
		PlayAni(TEXT("QuestSubMenuAni"), true);
	else
		PlayAni(TEXT("QuestSubMenuAni"));

	isCheckedQuest = !isCheckedQuest;
}
