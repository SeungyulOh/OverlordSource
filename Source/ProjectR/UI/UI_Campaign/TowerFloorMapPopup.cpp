
#include "ProjectR.h"
#include "TowerFloorMapPopup.h"

#include "Global/RGameInstance.h"
#include "Global/CampaignManager.h"
#include "Global/TableManager.h"
#include "Global/ContentsUnLockManager.h"

#include "Utils/UtilFunctionLibrary.h"
#include "Utils/UIFunctionLibrary.h"

#include "UI/Control/RScrollView.h"

void UTowerFloorScrollItem::NativeConstruct()
{
	Super::NativeConstruct();

	if(IsValid(SelectButton))
	{
		SelectButton->OnClicked.Clear();
		SelectButton->OnClicked.AddDynamic(this, &UTowerFloorScrollItem::OnClickFloorSelectButton);
	}
}

void UTowerFloorScrollItem::SetFloorScrollItem()
{
	if (IsValid(ItemData))
	{
		UTowerFloorScrollData* pData = Cast<UTowerFloorScrollData>(ItemData);
		if (IsValid(pData))
		{
			FloorText->SetText(FText::Format(FText::AsNumber(pData->nFloor + 1), FText::FromString(TEXT("F")) ));
			FloorTitle->SetText(UUtilFunctionLibrary::GetLocalizedString(EStringTableType::VE_Campaign, pData->FloorTitleLocalKey));
			FloorDescription->SetText(UUtilFunctionLibrary::GetLocalizedString(EStringTableType::VE_Campaign, pData->FloorDescLocalKey));
			
			LockImage->SetVisibility(pData->bLock ? ESlateVisibility::Visible : ESlateVisibility::Collapsed);
		}
	}
}

void UTowerFloorScrollItem::OnClickFloorSelectButton()
{
	if (IsValid(ItemData))
	{
		UTowerFloorScrollData* pData = Cast<UTowerFloorScrollData>(ItemData);
		if (IsValid(pData))
		{
			URGameInstance* RGameInstance = RGAMEINSTANCE(this);
			if (RGameInstance->ContentsUnLockManager->IsUnlockFloor(pData->nFloor + 1))
			{
				RGameInstance->CampaignManager->OnClickFloorShortCut.Broadcast(pData->nFloor + 1);
			}
			else
			{
				FText TitleText;
				FText DescText;

				RGameInstance->ContentsUnLockManager->GetUnlockReasonText(EGamePlayModeTypeEnum::VE_Campaign, TitleText, DescText, pData->nFloor + 1);
				UUIFunctionLibrary::ShowCommonPopupSplash(TitleText, DescText);
			}
			
		}
	}
}

void UTowerFloorScrollItem::SetItemData(int32 Index, URScrollItemData* item, URScrollView* ScrollView)
{
	SetFloorScrollItem();
}

void UTowerFloorMapPopup::NativeConstruct()
{
	Super::NativeConstruct();

	if (IsValid(GotoWorldButton))
	{
		GotoWorldButton->OnClicked.Clear();
		GotoWorldButton->OnClicked.AddDynamic(this, &UTowerFloorMapPopup::OnClickGotoWorld);
	}

	if (IsValid(CloseButton))
	{
		CloseButton->OnClicked.Clear();
		CloseButton->OnClicked.AddDynamic(this, &UTowerFloorMapPopup::OnClickClose);
	}

	if (IsValid(BackButton))
	{
		BackButton->OnClicked.Clear();
		BackButton->OnClicked.AddDynamic(this, &UTowerFloorMapPopup::OnClickClose);
	}

	if (IsValid(PlayIntroMovieButton))
	{
		PlayIntroMovieButton->OnClicked.Clear();
		PlayIntroMovieButton->OnClicked.AddDynamic(this, &UTowerFloorMapPopup::OnClickPlayIntroMovie);
	}

	RGAMEINSTANCE(this)->CampaignManager->OnClickFloorShortCut.RemoveDynamic(this, &UTowerFloorMapPopup::OnChangedFloor);
	RGAMEINSTANCE(this)->CampaignManager->OnClickFloorShortCut.AddDynamic(this, &UTowerFloorMapPopup::OnChangedFloor);
	InitScrollList();
	SetCurrentFloorInfo();
}

void UTowerFloorMapPopup::NativeDestruct()
{
	if (IsValid(GotoWorldButton))
		GotoWorldButton->OnClicked.Clear();

	if (IsValid(CloseButton))
		CloseButton->OnClicked.Clear();

	if (IsValid(BackButton))
		BackButton->OnClicked.Clear();

	if (IsValid(PlayIntroMovieButton))
		PlayIntroMovieButton->OnClicked.Clear();

	RGAMEINSTANCE(this)->CampaignManager->OnClickFloorShortCut.RemoveDynamic(this, &UTowerFloorMapPopup::OnChangedFloor);

	Super::NativeDestruct();
}

void UTowerFloorMapPopup::InitScrollList()
{
	if (IsValid(TowerFloorScrollView))
	{
		if (TowerFloorScrollView->ScrollItemList.Num() == 0)
			TowerFloorScrollView->Init_NoClear(MAXSCROLLITEM);

		if (CachedItemDataList.Num() == 0)
		{
			for (size_t i = 0; i < MAXSCROLLITEMDATA; ++i)
			{
				UTowerFloorScrollData* ItemData = Cast<UTowerFloorScrollData>(TowerFloorScrollView->CreateScrollItemData(UTowerFloorScrollData::StaticClass()));
				ItemData->WidgetSize = TowerFloorScrollView->GetItemInitSize();
				CachedItemDataList.Emplace(ItemData);
			}
		}

		URGameInstance* RGameInstance = RGAMEINSTANCE(this);
		UCampaignManager* CM = RGameInstance->CampaignManager;
		bool bHard = (CM->GetStageDifficulty() == EStageDifficultyEnum::Hard);
		int32 nFloor = bHard ? CM->GetCurrentHardFloor() : CM->GetCurrentNormalFloor();

		SetFloorScrollData(nFloor);
	}
}

void UTowerFloorMapPopup::SetFloorScrollData(int32 InFloor)
{
	if (!IsValid(TowerFloorScrollView))
		return;

	TowerFloorScrollView->ClearScrollItemData();

	for (size_t i = 0; i < MAXSCROLLITEMDATA; ++i)
	{
		UTowerFloorScrollData* pData = Cast<UTowerFloorScrollData>(CachedItemDataList[i]);
		if (pData)
		{
			FFloorInfo* FloorInfo = UTableManager::GetInstancePtr()->GetFloorInfoRow(FName(*FString::FromInt(i + 1)));

			if (FloorInfo)
			{
				pData->nFloor = i;
				pData->FloorTitleLocalKey = FloorInfo->FloorTitle;
				pData->FloorDescLocalKey = FloorInfo->FloorSubTilte;
				pData->bLock = (InFloor < i + 1);

				TowerFloorScrollView->ItemDataList.Emplace(pData);
			}
		}
	}
	TowerFloorScrollView->InvalidateData();

}

void UTowerFloorMapPopup::SetCurrentFloorInfo()
{
	URGameInstance* RGameInstance = RGAMEINSTANCE(this);
	UCampaignManager* CM = RGameInstance->CampaignManager;
	if (IsValid(CurrentFloor))
	{
		if(CM)
		{
			FText floorText = UUtilFunctionLibrary::GetLocalizedString(EStringTableType::VE_UI, FName(TEXT("UI_Tower_Popup_Location")));
			floorText = FText::Format(floorText, CM->TowerStatus.currentFloor);
			CurrentFloor->SetText(floorText);
		}
	}
}

void UTowerFloorMapPopup::OnClickPlayIntroMovie()
{
	UUIFunctionLibrary::ShowUIMediaFrameWork(FName(TEXT("IntroMovie")));
}

void UTowerFloorMapPopup::OnClickGotoWorld()
{

}

void UTowerFloorMapPopup::OnClickClose()
{
	this->SetVisibility(ESlateVisibility::Collapsed);
}

void UTowerFloorMapPopup::OnChangedFloor(int32 InFloor)
{
	OnClickClose();
}
