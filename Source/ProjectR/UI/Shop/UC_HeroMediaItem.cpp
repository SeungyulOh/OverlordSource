
#include "ProjectR.h"
#include "UC_HeroMediaItem.h"

#include "GlobalIntegrated.h"

#include "Table/HeroMovieTableInfo.h"
#include "UtilFunctionIntegrated.h"
#include "UI/Shop/UC_HeroMediaData.h"

void UUC_HeroMediaItem::SetScrollItemData(int32 Index, URScrollItemData* item, URScrollView* ScrollView)
{
	if (false == IsValid(item))
		return;

	Refresh();

}

void UUC_HeroMediaItem::Refresh()
{
	if (false == IsValid(this->ItemData))
		return;

	UC_HeroMediaData* ScrollItemData = Cast<UC_HeroMediaData>(this->ItemData);
	if (false == IsValid(ScrollItemData))
		return;

	if (IsValid(MediaImage))
	{
		FString text = ScrollItemData->MovieThumbnailRef.ToString();
		MediaImage->SetBrushFromTexture(UUtilFunctionLibrary::GetTexture2DFromTable(FName(*text)));
	}

	FCharacterTableInfo* CharacterTableInfo = UTableManager::GetInstancePtr()->GetCharacterRow(ScrollItemData->IndividualType);
	if (IsValid(FrontHeroText))
	{
		if (CharacterTableInfo != nullptr)
		{
			FrontHeroText->SetText(UUtilFunctionLibrary::GetLocalizedString(EStringTableType::VE_Hero, CharacterTableInfo->Nick));
		}
		else
		{
			if (ScrollItemData->IndividualType == "0")// intro movie
				FrontHeroText->SetText(FText::FromString(""));
		}
	}

	if (IsValid(BackHeroText))
	{
		if (CharacterTableInfo != nullptr)
		{
			BackHeroText->SetText(UUtilFunctionLibrary::GetLocalizedString(EStringTableType::VE_Hero, CharacterTableInfo->DisplayName));
		}
		else
		{
			if (ScrollItemData->IndividualType == "0")// intro movie
				BackHeroText->SetText(UUtilFunctionLibrary::GetLocalizedString(EStringTableType::VE_UI, ScrollItemData->NameString));
		}
	}
}
