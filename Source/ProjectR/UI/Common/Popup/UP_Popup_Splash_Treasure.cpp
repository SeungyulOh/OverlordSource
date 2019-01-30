// Fill out your copyright notice in the Description page of Project Settings.

#include "ProjectR.h"
#include "UP_Popup_Splash_Treasure.h"

#include "GlobalIntegrated.h"

#include "UtilFunctionIntegrated.h"
#include "UtilFunctionIntegrated.h"
#include "UI/Common/UC_Item_Icons.h"
#include "UI/RWidgetManager.h"
#include "GameInfo/UserInfo.h"


void UUP_Popup_Splash_Treasure::NativeConstruct()
{
	Super::NativeConstruct();


	if (Button_Click)
	{
		Button_Click->OnClicked.RemoveDynamic(this, &UUP_Popup_Splash_Treasure::OnClick_Close);
		Button_Click->OnClicked.AddDynamic(this, &UUP_Popup_Splash_Treasure::OnClick_Close);
	}
	//WBP_TreasurePopupSplash
}

void UUP_Popup_Splash_Treasure::RefreshUI(ETreasureSplashType InType)
{
	/*if (PopupType == InType)
	{
		PlayAni("Popup_Appear");
		return;
	}

	PopupType = InType;*/
	
	bool bArgExe = true;
	bool bCaptionExe = false;
	FText Caption;
	FText Desc;
	FName ImageKey;
	switch (InType)
	{
		case ETreasureSplashType::VE_GetMap:
		{
			Caption = UUtilFunctionLibrary::GetLocalizedString(EStringTableType::VE_UI, "Reward_Popup_window");
			Desc = UUtilFunctionLibrary::GetLocalizedString(EStringTableType::VE_UI, "UI_Treasure_Map");
			ImageKey = "Icon_Treasure_Map";
			bCaptionExe = true;
		}
		break;
		case ETreasureSplashType::VE_GetKey:
		{
			Caption = UUtilFunctionLibrary::GetLocalizedString(EStringTableType::VE_UI, "UI_Treasure_KeyAcquisition");
			Desc = UUtilFunctionLibrary::GetLocalizedString(EStringTableType::VE_UI, "UI_Treasure_KeyGotIt");
			ImageKey = "Icon_Treasure_Key";
			bArgExe = false;
		}
		break;
		case ETreasureSplashType::VE_NeedKey:
		{
			Caption = UUtilFunctionLibrary::GetLocalizedString(EStringTableType::VE_UI, "UI_Treasure_KeyShortage");
			Desc = UUtilFunctionLibrary::GetLocalizedString(EStringTableType::VE_UI, "UI_Treasure_Warning");
			ImageKey = "Icon_Treasure_Key";
		}
		break;
	}

	if (bArgExe)
	{
		URGameInstance* RGameInstance = RGAMEINSTANCE(this);
		FFormatOrderedArguments Args;
		FString TreasureID = RGameInstance->UserInfo->GetTreasureID();
		Args.Emplace(FText::FromString(TreasureID));
		Desc = FText::Format(Desc, Args);
	}
	if (bCaptionExe)
	{
		URGameInstance* RGameInstance = RGAMEINSTANCE(this);
		FFormatOrderedArguments Args;
		FString TreasureID = RGameInstance->UserInfo->GetTreasureID();
		Args.Emplace(FText::FromString(TreasureID));
		Caption = FText::Format(Caption, Args);
	}

	if (IsValid(Text_Caption))
	{
		Text_Caption->SetText(Caption);
	}
	if (IsValid(LocalText_Desc))
	{
		LocalText_Desc->SetText(Desc);
	}
	if (IsValid(Image_Icon))
	{
		auto Image = UUIFunctionLibrary::GetIconImageFromTable(ImageKey);
		UUIFunctionLibrary::SetImageBrush(Image, Image_Icon);
	}

	PlayAni("Popup_Appear");
	//PlayAnimation(SplashAnim);

	//this->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
}

void UUP_Popup_Splash_Treasure::OnClick_Close()
{
	URGameInstance* RGameInstance = RGAMEINSTANCE(this);
	//RGameInstance->RWidgetManager->HideTreasurePopupSplash();

	//this->SetVisibility(ESlateVisibility::Collapsed);
}
