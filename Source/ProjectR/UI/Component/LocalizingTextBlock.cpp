// Fill out your copyright notice in the Description page of Project Settings.

#include "ProjectR.h"
#include "LocalizingTextBlock.h"

#include "Global/RGameInstance.h"
#include "UI/RWidgetManager.h"
#include "UtilFunctionLibrary.h"

//#include "Table/StringTableInfo.h"


void ULocalizingTextBlock::SynchronizeProperties()
{
	Super::SynchronizeProperties();

	ApplyLocalizedText();
}

void ULocalizingTextBlock::ReleaseSlateResources(bool bReleaseChildren)
{
	Super::ReleaseSlateResources(bReleaseChildren);
}

void ULocalizingTextBlock::SetText(FText InText)
{
#if !WITH_EDITOR
	FName key;
	EStringTableType type;
	URGameInstance* RGameInstance = RGAMEINSTANCE(this);
	if(IsValid(RGameInstance) && IsValid(RGameInstance->RWidgetManager))
	{
		if (RGameInstance->RWidgetManager->GetLocalizingKeyAndType(InText, key, type))
		{
			LocalizingKey = key;
			StringTableType = type;
		}
	}
#endif

	Super::SetText(InText);
}

void ULocalizingTextBlock::SetText(FName key, EStringTableType type)
{
	LocalizingKey = key;
	StringTableType = type;

	Super::SetText(UUtilFunctionLibrary::GetLocalizedString(type, key));
}

void ULocalizingTextBlock::ApplyLocalizedText()
{
	if (CustomControl)
		return;

	if (IsStartupContents)
	{
		/*if (!LocalizingKey.IsNone())
		{
			//UStringTable
			//if (ULocalStringTableManager::GetInstancePtr()->GetLocalizedText(StringTableType, LocalizingKey, Text) == false)
			{
				Text = FText();
			}

			SetText(Text);
		}
		return;*/
	}

	//auto Data = Cast<ULocalStringTableManager>(GEngine->GameSingleton);
	
	//ELanguageType CurLanguageType = UGameOptionManager::GetLanguageType();

	if (!LocalizingKey.IsNone())
	{
		if (ULocalStringTableManager::GetInstancePtr()->GetLocalizedText(StringTableType, LocalizingKey, Text) == false)
		{
			Text = FText();
		}

		SetText(Text);
	}
}