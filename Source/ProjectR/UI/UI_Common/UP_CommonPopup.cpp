// Fill out your copyright notice in the Description page of Project Settings.

#include "ProjectR.h"
#include "UP_CommonPopup.h"

#include "UI/Common/Renewal_BaseHeroIcon.h"
#include "GlobalIntegrated.h"
//#include "GameInfo/RInventory.h"

#include "UtilFunctionIntegrated.h"
#include "UI/Common/UC_CurrencyDisplay.h"


void UUP_CommonPopup::NativeConstruct()
{
	Super::NativeConstruct();

// 	PlayAni(TEXT("Popup_Appear"));

	USoundBase* Sound = Cast<USoundBase>(StaticLoadObject(USoundBase::StaticClass(), nullptr, TEXT("SoundCue'/Game/Sound/ui/UP_Campaign_Campaign_Stage_Appear_Cue.UP_Campaign_Campaign_Stage_Appear_Cue'")));
	USoundManager::GetInstancePtr()->PlayComputeSound2D(GetWorld(), Sound);
	//UGameplayStatics::PlaySound2D(GetWorld(), Sound);

	if (IsValid(Button_OK))
	{
		Button_OK->OnClicked.RemoveDynamic(this, &UUP_CommonPopup::OnClickButtonOK);
		Button_OK->OnClicked.AddDynamic(this, &UUP_CommonPopup::OnClickButtonOK);
	}

	if (IsValid(Button_Cancel))
	{
		Button_Cancel->OnClicked.RemoveDynamic(this, &UUP_CommonPopup::OnClickButtonClose);
		Button_Cancel->OnClicked.AddDynamic(this, &UUP_CommonPopup::OnClickButtonClose);
	}

	if (IsValid(Button_Close))
	{
		Button_Close->OnClicked.RemoveDynamic(this, &UUP_CommonPopup::OnClickButtonClose);
		Button_Close->OnClicked.AddDynamic(this, &UUP_CommonPopup::OnClickButtonClose);
	}

	if (IsValid(CurrencyBox))
	{
		CurrencyBox->SetVisibility(ESlateVisibility::Collapsed);
	}
}

void UUP_CommonPopup::NativeDestruct()
{
	if (IsValid(LocalizingTextBlock_OK) && LocalizingTextBlock_OK->CustomControl == true)
	{
		LocalizingTextBlock_OK->CustomControl = false;
	}

	if (IsValid(LocalizingTextBlock_Cancel) && LocalizingTextBlock_Cancel->CustomControl == true)
	{
		LocalizingTextBlock_Cancel->CustomControl = false;
	}

	OnClickOK.Clear();
	OnClickCancel.Clear();

	Super::NativeDestruct();
}

void UUP_CommonPopup::SetCommonPopup(FText InCaption, FText InContents, ECommonPopupType InPopupType)
{
	OnClickOK.Clear();
	OnClickCancel.Clear();

	PlayAni(TEXT("Popup_Appear"));

	Caption = InCaption;
	Contents = InContents;
	PupupType = InPopupType;

	HeroID = TEXT("");

	if (IsValid(ButtonSwitcher))
		ButtonSwitcher->SetActiveWidgetIndex(0);
	
	RefreshUI();
}

void UUP_CommonPopup::SetHeroPopup(FName InHeroID, FText InCaption, FText InContents)
{
	OnClickOK.Clear();
	OnClickCancel.Clear();

	Caption = InCaption;
	Contents = InContents;
	PupupType = ECommonPopupType::VE_OKCANCEL;

	HeroID = InHeroID;

	if (IsValid(ButtonSwitcher))
		ButtonSwitcher->SetActiveWidgetIndex(1);

	SetHeroCurrency();

	RefreshUI();
}

void UUP_CommonPopup::RefreshUI()
{
	if (IsValid(Text_Caption))
	{
		Text_Caption->SetText(Caption);
	}
	if (IsValid(BP_Text_Body))
	{
		BP_Text_Body->SetText(Contents);
	}

	if (IsValid(ContentsRichTextBlock))
	{
		ContentsRichTextBlock->SetRichText(Contents);
	}

	if (IsValid(Button_OK))
	{
		Button_OK->SetVisibility(ESlateVisibility::Visible);
	}

	if (IsValid(SizeBox_Cancel))
	{
		if (PupupType == ECommonPopupType::VE_OKCANCEL)
		{
			SizeBox_Cancel->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
		}
		else
		{
			SizeBox_Cancel->SetVisibility(ESlateVisibility::Collapsed);
		}
	}

	if (IsValid(HeroIcon))
	{
		URGameInstance * RGameInstance = RGAMEINSTANCE(this);
		if ( IsValid(RGameInstance->RInventory))
		{
			FHERO* outHero = RGameInstance->RInventory->GetHeroWithID(HeroID);
			if (outHero != nullptr)
			{
				HeroIcon->SetHeroIconInfoWithUD(outHero->heroUD, EHeroOwnershipType::VE_Me);
// 				FHERO_ICON_INFO TargetHeroIconInfo;
// 				UUIFunctionLibrary::GetHeroIconInfoWithUD(TargetHeroIconInfo, outHero->heroUD, EHeroOwnershipType::VE_Me);
// 
// 				HeroIcon->SetHeroIconInfo(TargetHeroIconInfo);
			}
		}
	}
}

void UUP_CommonPopup::SetHeroCurrency()
{
	if (IsValid(Currency2Controls))
	{
		Currency2Controls->SetVisibility(ESlateVisibility::Collapsed);
	}
	FCharacterTableInfo* CharacterTableInfo = UTableManager::GetInstancePtr()->GetCharacterRow(HeroID);
	if (CharacterTableInfo == nullptr)
		return;
	
}

void UUP_CommonPopup::SetShortcutPopupType()
{
	if (IsValid(Currency1))
	{
		Currency1->OnlyTextDisplay();
	}
}

void UUP_CommonPopup::SetCurrency(FName InCurrencyType, int32 InPrice)
{
	if (IsValid(CurrencyBox))
	{
		CurrencyBox->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
	}

	if (IsValid(CurrencyImage))
	{
		FWalletTableInfo*	Wallet_table = UTableManager::GetInstancePtr()->GetWalletRow(InCurrencyType);
		if (Wallet_table)
		{
			UUIFunctionLibrary::SetImageBrush(UUIFunctionLibrary::GetIconImageFromTable(Wallet_table->ItemIconKey), CurrencyImage);
		}
	}

	if (IsValid(PriceText))
	{
		PriceText->SetText(FText::AsNumber(InPrice));
	}
}

void UUP_CommonPopup::HideCurrency()
{
	if (IsValid(CurrencyBox))
	{
		CurrencyBox->SetVisibility(ESlateVisibility::Collapsed);
	}
}

void UUP_CommonPopup::OnClickButtonOK()
{
	USoundBase* Sound = Cast<USoundBase>(StaticLoadObject(USoundBase::StaticClass(), nullptr, TEXT("SoundCue'/Game/Sound/ui/Button_Normal_Click_Cue.Button_Normal_Click_Cue'")));
	USoundManager::GetInstancePtr()->PlayComputeSound2D(GetWorld(), Sound);
	//UGameplayStatics::PlaySound2D(GetWorld(), Sound);

	OnClickOK.Broadcast();

	if(!bBlockHidePopup)
		UUIFunctionLibrary::HideCommonPopup();

	HideCurrency();
	bBlockHidePopup = false;
}

void UUP_CommonPopup::OnClickButtonClose()
{
	USoundBase* Sound = Cast<USoundBase>(StaticLoadObject(USoundBase::StaticClass(), nullptr, TEXT("SoundCue'/Game/Sound/ui/Button_Normal_Click_Cue.Button_Normal_Click_Cue'")));
	USoundManager::GetInstancePtr()->PlayComputeSound2D(GetWorld(), Sound);
	//UGameplayStatics::PlaySound2D(GetWorld(), Sound);

	OnClickCancel.Broadcast();

	if(!bBlockHidePopup)
		UUIFunctionLibrary::HideCommonPopup();
	
	HideCurrency();
	bBlockHidePopup = false;
}
