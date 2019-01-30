
#include "ProjectR.h"
#include "UP_ContentsReset.h"
#include "UI/Common/UC_CurrencyDisplay.h"
#include "GlobalIntegrated.h"


#include "Table/ContentResetTableInfo.h"
#include "UtilFunctionIntegrated.h"

void UUP_ContentsReset::NativeConstruct()
{
	Super::NativeConstruct();

	if (IsValid(Button_OK))
	{
		Button_OK->OnClicked.RemoveDynamic(this, &UUP_ContentsReset::OnClicked_ButtonOK);
		Button_OK->OnClicked.AddDynamic(this, &UUP_ContentsReset::OnClicked_ButtonOK);
	}

	if (IsValid(Button_Cancel))
	{
		Button_Cancel->OnClicked.RemoveDynamic(this, &UUP_ContentsReset::OnClicked_ButtonCancel);
		Button_Cancel->OnClicked.AddDynamic(this, &UUP_ContentsReset::OnClicked_ButtonCancel);
	}


	PlayAni(TEXT("Popup_Appear"));

	USoundBase* Sound = Cast<USoundBase>(StaticLoadObject(USoundBase::StaticClass(), nullptr, TEXT("SoundCue'/Game/Sound/ui/UP_Campaign_Campaign_Stage_Appear_Cue.UP_Campaign_Campaign_Stage_Appear_Cue'")));
	USoundManager::GetInstancePtr()->PlayComputeSound2D(GetWorld(), Sound);
	//UGameplayStatics::PlaySound2D(GetWorld(), Sound);

	RefreshUI();
	
}

void UUP_ContentsReset::SetContentsResetPopup(FText InCaption, FText InContents, FName InTableName, int32 nResetCount)
{
// 	OnClickOK.Clear();
// 	OnClickCancel.Clear();
// 
// 	Caption = InCaption;
// 	Contents = InContents;
// 
// 	bShowDescriptionArea = true;
// 
// 	FContentResetTableInfo* ContentsResetTableInfo = UTableManager::GetInstancePtr()->GetContentResetTableRow(InTableName);
// 
// 	if(ContentsResetTableInfo != nullptr)
// 	{
// 		addCount = ContentsResetTableInfo->addCount;
// 		remainCount = ContentsResetTableInfo->resetLimitCountPerDay - nResetCount;
// 		resetCount = nResetCount;
// 		maxCount = ContentsResetTableInfo->resetLimitCountPerDay;
// 		currencyQuantity = ContentsResetTableInfo->currencyValue;
// 		bExtraCharge = ContentsResetTableInfo->resetExtraCharge == 1;
// 		
// 		if (ContentsResetTableInfo->currencyType.IsEqual("REDSTONE"))
// 		{
// 			currencyType = EREWARD_TYPE::REDSTONE;
// 		}
// 		else if (ContentsResetTableInfo->currencyType.IsEqual("GEMS"))
// 		{
// 			currencyType = EREWARD_TYPE::GEM;
// 		}
// 	}
}

void UUP_ContentsReset::SetContentsRefreshPopup(FText InCaption, FText InContents, EREWARD_TYPE eType, int32 nQuantity)
{
	OnClickOK.Clear();
	OnClickCancel.Clear();

	Caption = InCaption;
	Contents = InContents;

	currencyType = eType;
	currencyQuantity = nQuantity;

	bShowDescriptionArea = false;
}

void UUP_ContentsReset::RefreshUI()
{
	if (IsValid(Text_Caption))
	{
		Text_Caption->SetText(Caption);
	}

	if (IsValid(Text_Contents))
	{
		Text_Contents->SetText(Contents);
	}
// 
// 	if (CurrencyDisplay)
// 	{
// 		int32 nQuantity = currencyQuantity;
// 		if (bShowDescriptionArea && bExtraCharge)
// 		{
// 			nQuantity *= FMath::Pow(2.0f, (float)resetCount);
// 		}
// 
// 		CurrencyDisplay->SetCurrency(currencyType, nQuantity);
// 	}

	if (bShowDescriptionArea)
	{
		if (IsValid(Desc_Area))
		{
			Desc_Area->SetVisibility(ESlateVisibility::HitTestInvisible);
		}

		if (IsValid(Text_AddCount))
		{
			Text_AddCount->SetText(FText::FromString(FString::FromInt(addCount)));
		}

		if (IsValid(Text_RemainCount))
		{
			Text_RemainCount->SetText(FText::FromString(FString::FromInt(remainCount)));

			auto RGameInstance = RGAMEINSTANCE(this);
			if( RGameInstance->GameDataManager)
			{
				FText type;
				if (remainCount <= 0)
					type = FText::FromString("warning");
				else
					type = FText::FromString("normal");

				Text_RemainCount->SetColorAndOpacity(RGameInstance->GameDataManager->GetWarningTextColor(type));
			}
		}

		if (IsValid(Text_MaxCount))
		{
			Text_MaxCount->SetText(FText::FromString(FString::FromInt(maxCount)));
		}

		if (IsValid(Text_Purpose))
		{
			Text_Purpose->SetText(UUtilFunctionLibrary::GetLocalizedString(EStringTableType::VE_UI, "UI_ContentsReset_Charge_Button"));
		}

	}
	else
	{
		if (IsValid(Desc_Area))
		{
			Desc_Area->SetVisibility(ESlateVisibility::Collapsed);
		}

		if (IsValid(Text_Purpose))
		{
			Text_Purpose->SetText(UUtilFunctionLibrary::GetLocalizedString(EStringTableType::VE_UI, "UI_ContentsReset_Refresh_Button"));
		}
	}
		
// 	ButtonOK->SetIsEnabled(0 < remainCount);
}

void UUP_ContentsReset::OnClicked_ButtonOK()
{
	USoundBase* Sound = Cast<USoundBase>(StaticLoadObject(USoundBase::StaticClass(), nullptr, TEXT("SoundCue'/Game/Sound/ui/Button_Normal_Click_Cue.Button_Normal_Click_Cue'")));
	USoundManager::GetInstancePtr()->PlayComputeSound2D(GetWorld(), Sound);
	//UGameplayStatics::PlaySound2D(GetWorld(), Sound);

	OnClickOK.Broadcast();
	
}

void UUP_ContentsReset::OnClicked_ButtonCancel()
{
	USoundBase* Sound = Cast<USoundBase>(StaticLoadObject(USoundBase::StaticClass(), nullptr, TEXT("SoundCue'/Game/Sound/ui/Button_Normal_Click_Cue.Button_Normal_Click_Cue'")));
	USoundManager::GetInstancePtr()->PlayComputeSound2D(GetWorld(), Sound);
	//UGameplayStatics::PlaySound2D(GetWorld(), Sound);

	OnClickCancel.Broadcast();

}
