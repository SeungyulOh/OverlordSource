

#include "ProjectR.h"
#include "GuildRuneDonationScrollItem.h"
#include "GuildRuneDonationScrollItemData.h"

#include "UI/Common/HeroBaseScrollItem.h"
#include "GlobalIntegrated.h"
#include "UtilFunctionIntegrated.h"

#include "GameInfo/UserInfo.h"
#include "GameInfo/RInventory.h"
#include "Network/PacketFunctionLibrary.h"

void UUC_GuildRuneDonationScrollItem::NativeConstruct()
{
	Super::NativeConstruct();

	if (IsValid(Button_SupportRune))
	{
		Button_SupportRune->OnClicked.RemoveDynamic(this, &UUC_GuildRuneDonationScrollItem::OnClickRuneDonationButton);
		Button_SupportRune->OnClicked.AddDynamic(this, &UUC_GuildRuneDonationScrollItem::OnClickRuneDonationButton);
	}
}

void UUC_GuildRuneDonationScrollItem::NativeDestruct()
{
	if (IsValid(Button_SupportRune))
	{
		Button_SupportRune->OnClicked.RemoveDynamic(this, &UUC_GuildRuneDonationScrollItem::OnClickRuneDonationButton);
	}

	Super::NativeDestruct();
}

void UUC_GuildRuneDonationScrollItem::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);
	if (bTimeTickCheck)
	{
		const FTimespan CurrentTime = FTimespan::FromSeconds(FPlatformTime::Seconds());

		if (CurrentTime.GetTicks() - nCurrentTime >= FTimespan::FromSeconds(1.0f).GetTicks())
		{
			nCurrentTime = CurrentTime.GetTicks();

			int32 Hour = 0;
			int32 Min = 0;
			int32 Sec = 0;

			--nRemainTime;

			if (nRemainTime > 0)
			{
				Hour = (int32)(nRemainTime / 3600);
				Min = (int32)(nRemainTime / 60) % 60;
				Sec = (int32)(nRemainTime % 60);
			}

			bool bTimeOver = nRemainTime <= 0;
			if (bTimeOver)
			{
				bTimeTickCheck = false;
			}
			SetTimeText(Hour, Min, Sec);
		}
	}
}

void UUC_GuildRuneDonationScrollItem::SetScrollItemData(int32 Index, URScrollItemData* item, URScrollView* ScrollView)
{
// 	UGuildRuneDonationScrollItemData* DonationItemData = Cast<UGuildRuneDonationScrollItemData>(item);
	if (!IsValid(ItemData))
		return;
	auto RuneRequestItemData = Cast<UGuildRuneDonationScrollItemData>(item);
	if (!IsValid(RuneRequestItemData))
		return;

	URGameInstance* RGameInstance = RGAMEINSTANCE(this);
// 	if (!IsValid(RGameInstance))
// 		return;

	if (IsValid(AvatarImage))
	{
		auto Avatar = RGameInstance->UserInfo->GetAvatarData();
		FName NameID;
		int32 TestValue = Avatar.icon - 1;
		if (TestValue == 0)
		{
			NameID = "PT_Avatar_Default";
		}
		else
		{
			auto ID = FString::FromInt(TestValue);
			auto Table = UTableManager::GetInstancePtr()->GetCharacterRow(FName(*ID));
			NameID = Table->PortraitTextureKey;
		}
	
		auto Image = UUIFunctionLibrary::GetIconImageFromTable(NameID);
		UUIFunctionLibrary::SetImageBrush(Image, AvatarImage);
	}

	FGUILD_RUNE_REQUESTER& GuildRuneRequester = RuneRequestItemData->RuneRequester;

	if (IsValid(RequestHeroIcon))
	{
// 		FHERO outHero;
// 		FHERO_ICON_INFO HeroIconInfo;
// 		UUIFunctionLibrary::GetHeroIconInfoWithID(HeroIconInfo, GuildRuneRequester.heroId,EHeroOwnershipType::VE_None);
// 		RequestHeroIcon->SetHeroIconInfo(HeroIconInfo);
		RequestHeroIcon->SetHeroIconInfoWithID(GuildRuneRequester.heroId, EHeroOwnershipType::VE_None);
	}

	if (IsValid(AvatarGuildGrade))
	{
		switch (GuildRuneRequester.grade)
		{
		case EGUILD_GRADE::MEMBER:
			AvatarGuildGrade->SetText(UUtilFunctionLibrary::GetLocalizedString(EStringTableType::VE_UI, TEXT("UC_GuildMember_Member")));
			break;
		case EGUILD_GRADE::ELDER:
			AvatarGuildGrade->SetText(UUtilFunctionLibrary::GetLocalizedString(EStringTableType::VE_UI, TEXT("UC_GuildMember_Elder")));
			break;
		case EGUILD_GRADE::MASTER:
			AvatarGuildGrade->SetText(UUtilFunctionLibrary::GetLocalizedString(EStringTableType::VE_UI, TEXT("UC_GuildMember_Founder")));
			break;
		}
	}

// 	if (IsValid(DonationStateSwitcher))
// 	{
// 		if (!GuildRuneRequester.gettable)
// 		{
// 			FHERO* HeroData_rune = RGameInstance->HeroInventory->GetInventoryDataWithID(GuildRuneRequester.heroId);
// 			if (HeroData_rune != nullptr && 0 < HeroData_rune->rune)
// 			{
// 				DonationStateSwitcher->SetActiveWidgetIndex(0);
// 			}
// 			else
// 			{
// 				DonationStateSwitcher->SetActiveWidgetIndex(2);
// 			}
// 		}
// 		else
// 		{
// 			DonationStateSwitcher->SetActiveWidgetIndex(1);
// 		}
// 	}
	if(IsValid(AvatarNick))
		AvatarNick->SetText(FText::FromString(GuildRuneRequester.nick));
	
// 	FHERO* HeroData = RGameInstance->HeroInventory->GetInventoryDataWithID(GuildRuneRequester.heroId);
// 	if (HeroData != nullptr)
// 	{
// 		RuneCount->SetText(FText::AsNumber(HeroData->rune));
// 	}

	nRemainTime = GuildRuneRequester.timeLeft;
	bTimeTickCheck = nRemainTime > 0;
	int32 hour = (int32)(GuildRuneRequester.timeLeft / 3600);
	int32 minute = (int32)(GuildRuneRequester.timeLeft / 60) % 60;
	int32 seconds = (int32)(GuildRuneRequester.timeLeft % 60);

	SetTimeText(hour, minute, seconds);

	if(IsValid(CurrentCount))
		CurrentCount->SetText(FText::AsNumber(GuildRuneRequester.currentAmount));
	if(IsValid(MaxCount))
		MaxCount->SetText(FText::AsNumber(GuildRuneRequester.maxAmount));
	
//	Text_SupportAmount->SetText(FText::AsNumber(RequestCount));

	if(IsValid(CurrentRuneProgressBar))
	{
		float fPercent = 0.0f;
		if (0 < GuildRuneRequester.currentAmount)
		{
			fPercent = (float)GuildRuneRequester.currentAmount / (float)GuildRuneRequester.maxAmount;
		}
		CurrentRuneProgressBar->SetPercent(fPercent);
	}
}

void UUC_GuildRuneDonationScrollItem::OnClickRuneDonationButton()
{
// 	URGameInstance* RGameInstance = RGAMEINSTANCE(this);
// 	if (!IsValid(RGameInstance))
// 		return;
// 
// 	if (!IsValid(ItemData))
// 		return;
// 
// 	auto RuneRequestItemData = Cast<UGuildRuneDonationScrollItemData>(ItemData);
// 	if (!IsValid(RuneRequestItemData))
// 		return;
//
// 	FGUILD_RUNE_REQUESTER& GuildRuneRequester = RuneRequestItemData->RuneRequester;
// 	int32 RequestCount = 2;
// 	FHERO* HeroData = RGameInstance->HeroInventory->GetInventoryDataWithID(GuildRuneRequester.heroId);
// 	if (HeroData != nullptr)
// 	{
// 		if (2 <= HeroData->rune)
// 			RequestCount = 2;
// 		else if (1 <= HeroData->rune)
// 			RequestCount = 1;
// 		else
// 		{
// 			return;
// 		}
// 	}
// 
// 	UPacketFunctionLibrary::GUILD_RUNE_SUPPORT_RQ(RGameInstance, GuildRuneRequester.kId, GuildRuneRequester.heroId, RequestCount);
}

void UUC_GuildRuneDonationScrollItem::SetTimeText(int32 hour, int32 min, int32 sec)
{
	FText h = UUIFunctionLibrary::Get_0n_Number(hour);
	FText m = UUIFunctionLibrary::Get_0n_Number(min);
	FText s = UUIFunctionLibrary::Get_0n_Number(sec);

	if (IsValid(RemainHour))
		RemainHour->SetText(h);
	if (IsValid(RemainMinute))
		RemainMinute->SetText(m);
	if (IsValid(RemainSeconds))
		RemainSeconds->SetText(s);
}