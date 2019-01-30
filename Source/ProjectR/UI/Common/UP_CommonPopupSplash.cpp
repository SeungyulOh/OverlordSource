// Fill out your copyright notice in the Description page of Project Settings.

#include "ProjectR.h"
#include "GlobalIntegrated.h"

#include "UI/RWidgetManager.h"
#include "UtilFunctionIntegrated.h"
#include "UP_CommonPopupSplash.h"

#include "Table/CharacterTableInfo.h"



void UUP_CommonPopupSplash::NativeConstruct()
{
	Super::NativeConstruct();
}

void UUP_CommonPopupSplash::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);

	if (bPlayStart)
	{
		if (!IsPlayAni(CurAniName))
		{
			URGameInstance* RGameInstance = RGAMEINSTANCE(this);
			if ( IsValid(RGameInstance->RWidgetManager))
			{
				UUIFunctionLibrary::HideCommonPopupSplash();
			}

			bPlayStart = false;
		}
	}
}

void UUP_CommonPopupSplash::RefreshUI(ECommonSplashType InType, FText InCaption, FText InDesc, FName InId, bool bLevelUpTextShow/* = false*/)
{
	CurSplashType = InType;

	switch (CurSplashType)
	{
	case ECommonSplashType::VE_Hero:
	{
		if (IsValid(Image_Hero) && IsValid(TextBlock_Nick) && IsValid(TextBlock_Name))
		{
			Image_Hero->SetVisibility(ESlateVisibility::Collapsed);

			FCharacterTableInfo* CharacterTableInfo = UTableManager::GetInstancePtr()->GetCharacterRow(InId);
			if (CharacterTableInfo)
			{
				Image_Hero->SetVisibility(ESlateVisibility::SelfHitTestInvisible);

				UUIFunctionLibrary::SetImageBrush(UUIFunctionLibrary::GetIconImageFromTable(CharacterTableInfo->PortraitTextureKey), Image_Hero);

				TextBlock_Nick->SetText(UUtilFunctionLibrary::GetLocalizedString(EStringTableType::VE_Hero, CharacterTableInfo->Nick));
				TextBlock_Name->SetText(UUtilFunctionLibrary::GetLocalizedString(EStringTableType::VE_Hero, CharacterTableInfo->DisplayName));
			}
		}

		if (IsValid(TextBlock_HeroTitle))
		{
			TextBlock_HeroTitle->SetText(InCaption);
		}

		if (IsValid(Levelup_Text))
		{
			Levelup_Text->SetVisibility(bLevelUpTextShow ? ESlateVisibility::SelfHitTestInvisible : ESlateVisibility::Collapsed);
		}
		
		CurAniName = TEXT("Splash_Hero");
	}
	break;
	case ECommonSplashType::VE_Normal:
	{
		if (IsValid(TextBlock_Contents))
		{
			TextBlock_Contents->SetText(InCaption);
		}

		if (IsValid(TextBlock_Desc))
		{
			TextBlock_Desc->SetText(InDesc);
		}

		CurAniName = TEXT("Splash_Text");
	}
	break;
	}

	PlayAni(CurAniName);

	bPlayStart = true;
}
