

#include "ProjectR.h"
#include "GuildRuneRequestScrollItem.h"
#include "GuildRuneRequestScrollItemData.h"
#include "UI/Common/HeroBaseScrollItem.h"
#include "UI/Common/Renewal_BaseHeroIcon.h"

void UUC_GuildRuneRequestScrollItem::NativeConstruct()
{
	Super::NativeConstruct();
}

void UUC_GuildRuneRequestScrollItem::NativeDestruct()
{
	Super::NativeDestruct();
}

void UUC_GuildRuneRequestScrollItem::SetScrollItemData(int32 Index, URScrollItemData* item, URScrollView* ScrollView)
{
	UGuildRuneRequestScrollItemData* RequestData = Cast<UGuildRuneRequestScrollItemData>(item);
	if (RequestData == nullptr)
		return;

	if(IsValid(HeroScrollIcon))
	{
		HeroScrollIcon->HeroIcon->SetHeroIconInfoCopy(RequestData->HeroIconInfo);
		HeroScrollIcon->HeroIcon->SetIsSelected(ItemData->bSelected);
		HeroScrollIcon->HeroIcon->SelectIconHero();
	}

	if (IsValid(RuneCount))
	{
		RuneCount->SetText(FText::AsNumber(RequestData->HaveCount));
	}
	
}

void UUC_GuildRuneRequestScrollItem::SetSelectedHero(bool bSelected)
{
	if (IsValid(HeroScrollIcon))
	{
		ItemData->bSelected = bSelected;
		HeroScrollIcon->HeroIcon->SetIsSelected(bSelected);
		HeroScrollIcon->HeroIcon->SelectIconHero();
	}
}
