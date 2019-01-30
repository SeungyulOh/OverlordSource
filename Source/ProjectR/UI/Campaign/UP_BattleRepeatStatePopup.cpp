
#include "ProjectR.h"
#include "UP_BattleRepeatStatePopup.h"

#include "UtilFunctionIntegrated.h"
#include "GlobalIntegrated.h"


#include "UI/Component/CheckBox_Radio.h"
#include "UI/RWidgetManager.h"
#include "UI/Common/UC_Item_Icons.h"
#include "UtilFunctionIntegrated.h"


void UUP_BattleRepeatStatePopup::NativeConstruct()
{
	Super::NativeConstruct();
	RepeatStateSwitcher->SetActiveWidgetIndex(0);

	FindChildWidgetsWithCertainName<UUC_Item_Icons>(this, RuneRewardList, TEXT("UC_Item_Icons"));
	FindChildWidgetsWithCertainName<USizeBox>(this, RuneRewardSizeBoxes, TEXT("RuneRewardSizebox"));
	FindChildWidgetsWithCertainName<UTextBlock>(this, CollectItemCountList, TEXT("CollectItemCount"));
	FindChildWidgetsWithCertainName<UCheckBox_Radio>(this, CountCheckList, TEXT("CountCheckCase_"));
	FindChildWidgetsWithCertainName<UWidgetSwitcher>(this, CheckedSwitcherList, TEXT("CheckedSwitcher"));

	for (auto Element : CountCheckList)
	{
		Element->OnCheckStateChanged.RemoveDynamic(this, &UUP_BattleRepeatStatePopup::AfterCheckStateChanged);
		Element->OnCheckStateChanged.AddDynamic(this, &UUP_BattleRepeatStatePopup::AfterCheckStateChanged);
	}


	if (IsValid(Button_Cancel))
	{
		Button_Cancel->OnClicked.RemoveDynamic(this, &UUP_BattleRepeatStatePopup::OnClickCancel);
		Button_Cancel->OnClicked.AddDynamic(this, &UUP_BattleRepeatStatePopup::OnClickCancel);
	}

	if (IsValid(Button_OK))
	{
		Button_OK->OnClicked.RemoveDynamic(this, &UUP_BattleRepeatStatePopup::OnClickOK);
		Button_OK->OnClicked.AddDynamic(this, &UUP_BattleRepeatStatePopup::OnClickOK);
	}
	if (IsValid(RepeatStateButton))
	{
		RepeatStateButton->OnClicked.RemoveDynamic(this, &UUP_BattleRepeatStatePopup::OnClickStateButton);
		RepeatStateButton->OnClicked.AddDynamic(this, &UUP_BattleRepeatStatePopup::OnClickStateButton);
	}
}

void UUP_BattleRepeatStatePopup::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry,InDeltaTime);
	
	if (bAutoPlayCount)
	{
		const FTimespan CurrentTime = FTimespan::FromSeconds(FPlatformTime::Seconds());

		if (CurrentTime.GetTicks() - nCurrentTime >= FTimespan::FromSeconds(1.0f).GetTicks())
		{
			nCurrentTime = CurrentTime.GetTicks();
// 			CountDownText->SetText(FText::AsNumber(nRemainTime));

			--nRemainTime;

			if (nRemainTime < 0)
			{
				bAutoPlayCount = false;
				URGameInstance* RGameInstance = RGAMEINSTANCE(this);
				if ( IsValid(RGameInstance->EventManager))
					RGameInstance->EventManager->OnAutoPlayCoolTime.Broadcast();
			}
			
		}
	}
}

void UUP_BattleRepeatStatePopup::OnClickStateButton()
{
	URGameInstance* RGameInstance = RGAMEINSTANCE(this);
	if (RGameInstance->IsAutoPlayMode())
		RGameInstance->CampaignManager->SetAutoPlayCount(0);
	this->SetVisibility(ESlateVisibility::Collapsed);
	UUIFunctionLibrary::ShowCommonPopupSplash(UUtilFunctionLibrary::GetLocalizedString(EStringTableType::VE_UI, COMMONSTRING_NOTIFY),
		FText::FromString(TEXT("Cancel Auto Play")));
	//ShowAutoPlayPopup(EBattleRepeatPopupState::VE_ItemGainState);
}

void UUP_BattleRepeatStatePopup::OnCheckRepeatCountSelect(int32 InCount)
{
	nSelectedRepeatCount = InCount;
	int32 nIndex = 0;

	if (InCount == 5) nIndex = 0;
	else if (InCount == 10) nIndex = 1;
	else nIndex = 2;
	
	int32 findindex = 0;
	
	for (UCheckBox_Radio* check : CountCheckList)
	{
		if(findindex == nIndex)
			check->SetCheckedState(ECheckBoxState::Checked);
		else
			check->SetCheckedState(ECheckBoxState::Unchecked);
		
		++findindex;
	}

	findindex = 0;
	
	for (UWidgetSwitcher* switcher : CheckedSwitcherList)
	{
		if (findindex == nIndex)
			switcher->SetActiveWidgetIndex(1);
		else
			switcher->SetActiveWidgetIndex(0);
		++findindex;
	}
}

void UUP_BattleRepeatStatePopup::OnClickOK()
{
	URGameInstance* RGameInstance = RGAMEINSTANCE(this);
// 	if (!IsValid(RGameInstance))
// 		return;
	switch (PopupState)
	{
	case EBattleRepeatPopupState::VE_CountSelect:
	{
		
		if (IsValid(RGameInstance->EventManager))
			RGameInstance->EventManager->OnClickAutoPlayOptionOK.Broadcast();

		this->SetVisibility(ESlateVisibility::Collapsed);
	}
	break;
	case EBattleRepeatPopupState::VE_ItemGainState:
	{
		this->SetVisibility(ESlateVisibility::Collapsed);

		UUIFunctionLibrary::ShowCommonPopupSplash(UUtilFunctionLibrary::GetLocalizedString(EStringTableType::VE_UI, COMMONSTRING_NOTIFY),
			UUtilFunctionLibrary::GetLocalizedString(EStringTableType::VE_UI, FName(TEXT("UI_AutoPlay_Finished"))));
	}
	break;
	case EBattleRepeatPopupState::VE_FailResult_Defeat:
	case EBattleRepeatPopupState::VE_SuccessResult:
	{
		this->SetVisibility(ESlateVisibility::Collapsed);
		//ShowAutoPlayPopup(EBattleRepeatPopupState::VE_GamePlay);
// 		RepeatStateButton->SetVisibility(ESlateVisibility::Visible);
// 		InfoPopup->SetVisibility(ESlateVisibility::Collapsed);
	}
	break;
	case EBattleRepeatPopupState::VE_FailResult_Stamina:
	{
		this->SetVisibility(ESlateVisibility::Collapsed);
	}
	break;
	default:
		break;
	}
	

}

void UUP_BattleRepeatStatePopup::OnClickCancel()
{

	switch (PopupState)
	{
	case EBattleRepeatPopupState::VE_CountSelect:
	{
		URGameInstance* RGameInstance = RGAMEINSTANCE(this);
		if( IsValid(RGameInstance->CampaignManager))
		{
			nSelectedRepeatCount = 0;
		}

		this->SetVisibility(ESlateVisibility::Collapsed);
	}
	break;
	case EBattleRepeatPopupState::VE_GamePlay:
		break;
	case EBattleRepeatPopupState::VE_ItemGainState:
		ShowAutoPlayPopup(EBattleRepeatPopupState::VE_GamePlay);
		break;

	}
}

void UUP_BattleRepeatStatePopup::ShowAutoPlayPopup(EBattleRepeatPopupState InState)
{
	this->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
	PopupState = InState;

	switch (PopupState)
	{
	case EBattleRepeatPopupState::VE_GamePlay:
	{
		InfoPopup->SetVisibility(ESlateVisibility::Collapsed);
		RepeatStateButton->SetVisibility(ESlateVisibility::Visible);
		PlayAni(TEXT("AutoPlay"), false, 0);
	}
	break;
	case EBattleRepeatPopupState::VE_CountSelect:
	{
		Text_Caption->SetText(UUtilFunctionLibrary::GetLocalizedString(EStringTableType::VE_UI, TEXT("UI_AutoPlay_Option")));
		InfoPopup->SetVisibility(ESlateVisibility::Visible);
		RepeatStateButton->SetVisibility(ESlateVisibility::Collapsed);
		CancelButton_Area->SetVisibility(ESlateVisibility::Visible);
		PopupTypeSwitcher->SetActiveWidgetIndex(1);
		OnCheckRepeatCountSelect(5);
		PlayAni(TEXT("Popup_Appear"));
	}
	break;
	case EBattleRepeatPopupState::VE_ItemGainState:
	case EBattleRepeatPopupState::VE_FailResult_Stamina:
	case EBattleRepeatPopupState::VE_FailResult_Defeat:
	case EBattleRepeatPopupState::VE_SuccessResult:
	{
		InvalidateRewardData();
		Text_Caption->SetText(UUtilFunctionLibrary::GetLocalizedString(EStringTableType::VE_UI, TEXT("UI_AutoPlay_Title")));
		if(PopupState == EBattleRepeatPopupState::VE_ItemGainState)
		{
			DescriptionText->SetText(UUtilFunctionLibrary::GetLocalizedString(EStringTableType::VE_UI, TEXT("UI_AutoPlay_String01")));
		}
		else if(PopupState == EBattleRepeatPopupState::VE_FailResult_Stamina)
		{
			DescriptionText->SetText(UUtilFunctionLibrary::GetLocalizedString(EStringTableType::VE_UI, TEXT("UI_AutoPlay_String02")));
			CancelButton_Area->SetVisibility(ESlateVisibility::Collapsed);
		}
		else if (PopupState == EBattleRepeatPopupState::VE_FailResult_Defeat)
		{
			DescriptionText->SetText(UUtilFunctionLibrary::GetLocalizedString(EStringTableType::VE_UI, TEXT("UI_AutoPlay_String04")));
			CancelButton_Area->SetVisibility(ESlateVisibility::Collapsed);
		}
		else if (PopupState == EBattleRepeatPopupState::VE_SuccessResult)
		{
			DescriptionText->SetText(UUtilFunctionLibrary::GetLocalizedString(EStringTableType::VE_UI, TEXT("UI_AutoPlay_String05")));
			CancelButton_Area->SetVisibility(ESlateVisibility::Collapsed);
		}

		InfoPopup->SetVisibility(ESlateVisibility::Visible);
		RepeatStateButton->SetVisibility(ESlateVisibility::Collapsed);
		PopupTypeSwitcher->SetActiveWidgetIndex(0);
// 		CancelButtonArea->SetVisibility(ESlateVisibility::Collapsed);
		PlayAni(TEXT("Popup_Appear"));
	}
	break;
	default:
		break;
	}
}

void UUP_BattleRepeatStatePopup::AfterCheckStateChanged(bool isChecked)
{
	for (size_t i = 0; i < CountCheckList.Num(); ++i)
	{
		if (CountCheckList[i]->IsChecked() && isChecked)
		{
			OnCheckRepeatCountSelect(5 * FMath::Pow(5, i - 1));
		}
	}
}

void UUP_BattleRepeatStatePopup::InvalidateRewardData()
{
	URGameInstance* RGameInstance = RGAMEINSTANCE(this);
// 	if (!IsValid(RGameInstance))
// 		return;
	
	if (IsValid(Exp_Text))
	{
		//Exp_Text->SetText(FText::AsNumber(RGameInstance->CampaignManager->AccumulateEXP));
	}
	
	if (IsValid(Gold_Text))
	{
		//Gold_Text->SetText(FText::AsNumber(RGameInstance->CampaignManager->AccumulateGold));
	}

	for (auto box : RuneRewardSizeBoxes)
	{
		box->SetVisibility(ESlateVisibility::Collapsed);
	}

	for (auto text : CollectItemCountList)
	{
		text->SetText(FText::AsNumber(0));
	}


}

void UUP_BattleRepeatStatePopup::AutoPlayCountStart()
{
//	RepeatStateSwitcher->SetActiveWidgetIndex(1);
// 	NowPlayImage->SetVisibility(ESlateVisibility::Collapsed);

	bAutoPlayCount = true;
	nRemainTime = 3;
	CountDownText->SetText(FText::AsNumber(nRemainTime));
}