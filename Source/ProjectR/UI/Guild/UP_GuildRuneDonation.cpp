
#include "ProjectR.h"
#include "UP_GuildRuneDonation.h"

#include "GameInfo/UserInfo.h"
#include "GameInfo/GuildInfo.h"
#include "GameInfo/RInventory.h"
#include "UI/Guild/GuildRuneRequestScrollItemData.h"
#include "UI/Guild/GuildRuneDonationScrollItemData.h"
#include "UI/Guild/UC_GuildRuneRequestPopup.h"
#include "UI/Control/RScrollView.h"
#include "UI/Common/HeroBaseScrollItem.h"
#include "UtilFunctionIntegrated.h"
#include "UtilFunctionIntegrated.h"
#include "GlobalIntegrated.h"

#include "Network/PacketFunctionLibrary.h"
#include "Network/HttpRequestClient.h"

void UUP_GuildRuneDonation::NativeConstruct()
{
	Super::NativeConstruct();

	URGameInstance* RGameInstance = RGAMEINSTANCE(this);
	
	{
		RGameInstance->HttpClient->OnGuildRuneStatusRpDelegate.RemoveDynamic(this, &UUP_GuildRuneDonation::OnGuildRuneStatusRpDelegate);
		RGameInstance->HttpClient->OnGuildRuneStatusRpDelegate.AddDynamic(this, &UUP_GuildRuneDonation::OnGuildRuneStatusRpDelegate);
	}

	if (IsValid(RequestButton))
	{
		RequestButton->OnClicked.RemoveDynamic(this, &UUP_GuildRuneDonation::OnClickRequestButton);
		RequestButton->OnClicked.AddDynamic(this, &UUP_GuildRuneDonation::OnClickRequestButton);
	}

	if (IsValid(ReceiveButton))
	{
		ReceiveButton->OnClicked.RemoveDynamic(this, &UUP_GuildRuneDonation::OnClickReceiveButton);
		ReceiveButton->OnClicked.AddDynamic(this, &UUP_GuildRuneDonation::OnClickReceiveButton);
	}

	if (IsValid(RuneRequestScrollView))
	{
		RuneRequestScrollView->Init();
		RuneRequestScrollView->SetVisibility(ESlateVisibility::Hidden);
	}

	Init();
	UPacketFunctionLibrary::GUILD_RUNE_STATUS_RQ();

}

void UUP_GuildRuneDonation::NativeDestruct()
{
	URGameInstance* RGameInstance = RGAMEINSTANCE(this);
	
	{
		RGameInstance->HttpClient->OnGuildRuneStatusRpDelegate.RemoveDynamic(this, &UUP_GuildRuneDonation::OnGuildRuneStatusRpDelegate);
	}

	if (IsValid(RequestButton))
	{
		RequestButton->OnClicked.RemoveDynamic(this, &UUP_GuildRuneDonation::OnClickRequestButton);
	}

	if (IsValid(ReceiveButton))
	{
		ReceiveButton->OnClicked.RemoveDynamic(this, &UUP_GuildRuneDonation::OnClickReceiveButton);
	}

	Super::NativeDestruct();
}

void UUP_GuildRuneDonation::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);
	if (bTimeTickCheck)
	{
		const FTimespan CurrentTime = FTimespan::FromSeconds(FPlatformTime::Seconds());

		if (CurrentTime.GetTicks() - nCurrentTime >= FTimespan::FromSeconds(1.0f).GetTicks())
		{
			nCurrentTime = CurrentTime.GetTicks();

			--nRemainTime;

			if (nRemainTime <= 0)
			{
				bTimeTickCheck = false;
			}
			SetTimeInfo(nRemainTime);
		}
	}
	if (bCoolTimeCheck)
	{
		const FTimespan CurrentTime = FTimespan::FromSeconds(FPlatformTime::Seconds());

		if (CurrentTime.GetTicks() - nCurrentTime >= FTimespan::FromSeconds(1.0f).GetTicks())
		{
			nCurrentTime = CurrentTime.GetTicks();

			--nRemainTime;

			if (nRemainTime <= 0)
			{
				bCoolTimeCheck = false;
			}
			SetCoolTime(nRemainTime);
		}
	}
}

void UUP_GuildRuneDonation::Init()
{
	if (IsValid(RequestDesc))
	{
		FText text = FText::Format(UUtilFunctionLibrary::GetLocalizedString(EStringTableType::VE_UI, TEXT("UP_Guild_Management_Menu40")), FText::FromString(TEXT("3")));
		RequestDesc->SetRichText(text);
	}
	if (IsValid(MyRequestText1))
	{
		MyRequestText1->SetRichText(UUtilFunctionLibrary::GetLocalizedString(EStringTableType::VE_UI, TEXT("UP_Guild_Management_Menu42")));
	}
	if (IsValid(MyRequestText2))
	{
		MyRequestText2->SetRichText(UUtilFunctionLibrary::GetLocalizedString(EStringTableType::VE_UI, TEXT("UP_Guild_Management_Menu53")));
	}
}

void UUP_GuildRuneDonation::OnGuildRuneStatusRpDelegate()
{
	URGameInstance* RGameInstance = RGAMEINSTANCE(this);
// 	if (!IsValid(RGameInstance))
// 		return;

	if (!IsValid(RuneRequestScrollView))
		return;

	URScrollView* ScrollView_RuneRequestList = RuneRequestScrollView;

	ScrollView_RuneRequestList->ClearScrollItemData();
	ScrollView_RuneRequestList->SetVisibility(ESlateVisibility::Visible);

	int32 myRequestcount = 0;
	FGUILD_RUNE_REQUESTER MyRuneRequest;
	if (RGameInstance->GuildInfo->GetMyRuneRequest(MyRuneRequest))
	{
		myRequestcount = 1;
		SetMyAvatarInfo(MyRuneRequest);
		SetHeroRuneInfo(MyRuneRequest);


		if(MyRuneRequest.gettable)
		{
			if (IsValid(MyStateSwitcher))
				MyStateSwitcher->SetActiveWidgetIndex(0);

			if(IsValid(RequestStateSwitcher))
				RequestStateSwitcher->SetActiveWidgetIndex(0);
		}
		else
		{
			if (MyRuneRequest.currentAmount == MyRuneRequest.maxAmount) // cooltime
			{
				if (IsValid(MyStateSwitcher))
					MyStateSwitcher->SetActiveWidgetIndex(1);
				if (IsValid(RequestAbleSwitcher))
					RequestAbleSwitcher->SetActiveWidgetIndex(0);

				nRemainTime = MyRuneRequest.timeLeft;
				bCoolTimeCheck = nRemainTime > 0;
				bTimeTickCheck = false;
				SetCoolTime(MyRuneRequest.timeLeft);
			}
			else //request..
			{
				if (IsValid(MyStateSwitcher))
					MyStateSwitcher->SetActiveWidgetIndex(0);
				if (IsValid(RequestStateSwitcher))
					RequestStateSwitcher->SetActiveWidgetIndex(1);
			}
		}
	}
	else
	{
		if(IsValid(MyStateSwitcher))
			MyStateSwitcher->SetActiveWidgetIndex(1);

		if (IsValid(RequestAbleSwitcher))
			RequestAbleSwitcher->SetActiveWidgetIndex(1);

	}

	TArray<FGUILD_RUNE_REQUESTER> GuildRuneRequestList = RGameInstance->GuildInfo->RuneRequests;
	for (FGUILD_RUNE_REQUESTER& RuneRequest : GuildRuneRequestList)
	{
		if (RGameInstance->HttpClient->IsValidKID(RuneRequest.kId))
			continue;

		UGuildRuneDonationScrollItemData* ScrollItemData = Cast<UGuildRuneDonationScrollItemData>(ScrollView_RuneRequestList->AddScrollItemData(UGuildRuneDonationScrollItemData::StaticClass()));
		if (ScrollItemData)
		{
			ScrollItemData->RuneRequester = RuneRequest;
		}
	}

	ScrollView_RuneRequestList->InvalidateData();
	
	if (IsValid(RequestCount))
	{
		RequestCount->SetText(FText::AsNumber(GuildRuneRequestList.Num() - myRequestcount));
	}
}

void UUP_GuildRuneDonation::SetMyAvatarInfo(FGUILD_RUNE_REQUESTER& MyRuneRequest)
{
	URGameInstance* RGameInstance = RGAMEINSTANCE(this);
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

	if (IsValid(AvatarGuildGrade))
	{
		switch (MyRuneRequest.grade)
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
	
	if (IsValid(AvatarNick))
		AvatarNick->SetText(FText::FromString(MyRuneRequest.nick));

}

void UUP_GuildRuneDonation::SetHeroRuneInfo(FGUILD_RUNE_REQUESTER& MyRuneRequest)
{
	URGameInstance* RGameInstance = RGAMEINSTANCE(this);
	FHERO* HeroData = RGameInstance->RInventory->GetHeroWithID(MyRuneRequest.heroId);
	if (HeroData == nullptr)
		return;
	if (IsValid(RGameInstance->RInventory))
	{
// 		FHERO_ICON_INFO HeroIconInfo;
// 		UUIFunctionLibrary::GetHeroIconInfoWithUD(HeroIconInfo, HeroData->heroUD, EHeroOwnershipType::VE_Me);
// 		RequestHeroIcon->SetHeroIconInfo(HeroIconInfo);
		RequestHeroIcon->SetHeroIconInfoWithUD(HeroData->heroUD, EHeroOwnershipType::VE_Me);
	}
	
// 	if(IsValid(RuneCount))
// 		RuneCount->SetText(FText::AsNumber(HeroData->rune));
	
	if (IsValid(CurrentCount))
		CurrentCount->SetText(FText::AsNumber(MyRuneRequest.currentAmount));
	if (IsValid(MaxCount))
		MaxCount->SetText(FText::AsNumber(MyRuneRequest.maxAmount));
	
	if (IsValid(CurrentRuneProgressBar))
	{
		float fPercent = 0.0f;
		if (0 < MyRuneRequest.currentAmount)
		{
			fPercent = (float)MyRuneRequest.currentAmount / (float)MyRuneRequest.maxAmount;
		}
		CurrentRuneProgressBar->SetPercent(fPercent);
	}

	nRemainTime = MyRuneRequest.timeLeft;
	bTimeTickCheck = nRemainTime > 0;

	SetTimeInfo(MyRuneRequest.timeLeft);
}

void UUP_GuildRuneDonation::SetTimeInfo(int32 timeLeft)
{
	int32 hour = (int32)(timeLeft / 3600);
	int32 minute = (int32)(timeLeft / 60) % 60;
	int32 seconds = (int32)(timeLeft % 60);
	
	FText h = UUIFunctionLibrary::Get_0n_Number(hour);
	FText m = UUIFunctionLibrary::Get_0n_Number(minute);
	FText s = UUIFunctionLibrary::Get_0n_Number(seconds);

	if (IsValid(RemainHour))
		RemainHour->SetText(h);
	if (IsValid(RemainMinute))
		RemainMinute->SetText(m);
	if (IsValid(RemainSeconds))
		RemainSeconds->SetText(s);
}

void UUP_GuildRuneDonation::SetCoolTime(int32 timeLeft)
{
	if (IsValid(CoolTimeText))
	{
		int32 hour = (int32)(timeLeft / 3600);
		int32 minute = (int32)(timeLeft / 60) % 60;
		int32 seconds = (int32)(timeLeft % 60);

		FText h = UUIFunctionLibrary::Get_0n_Number(hour);
		FText m = UUIFunctionLibrary::Get_0n_Number(minute);
		FText s = UUIFunctionLibrary::Get_0n_Number(seconds);
		FText text1 = UUtilFunctionLibrary::GetLocalizedString(EStringTableType::VE_UI, TEXT("UP_Guild_Management_Menu51"));
		FText text2 = UUtilFunctionLibrary::GetLocalizedString(EStringTableType::VE_UI, TEXT("UP_Guild_Management_Menu52"));

		FText text = FText::FromString(text1.ToString() + " " + h.ToString() + " : " + m.ToString() + " : " + s.ToString() + " " + text2.ToString());

		CoolTimeText->SetRichText(text);
	}
}
void UUP_GuildRuneDonation::OnClickRequestButton()
{
	if (IsValid(RequestRunePopup))
	{
		RequestRunePopup->ShowRequestRunePopup();
	}

}

void UUP_GuildRuneDonation::OnClickReceiveButton()
{
	URGameInstance* RGameInstance = RGAMEINSTANCE(this);
	if (RGameInstance == nullptr)
		return;

	UPacketFunctionLibrary::GUILD_RUNE_GET_RQ();
}