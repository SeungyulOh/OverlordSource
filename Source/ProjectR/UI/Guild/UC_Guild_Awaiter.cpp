 // Fill out your copyright notice in the Description page of Project Settings.

#include "ProjectR.h"
#include "UC_Guild_Awaiter.h"
#include "GlobalIntegrated.h"

#include "GameInfo/GuildInfo.h"
#include "UtilFunctionIntegrated.h"
#include "Network/HttpRequestClient.h"
#include "Network/PacketFunctionLibrary.h"
#include "UI/Control/RScrollView.h"
#include "UI/Guild/GuildAwaiter_ScrollItemData.h"

void UUC_Guild_Awaiter::NativeConstruct()
{
	Super::NativeConstruct();

	this->SetVisibility(ESlateVisibility::Hidden);

	URGameInstance* RGameInstance = RGAMEINSTANCE(this);
	
	{
		RGameInstance->HttpClient->OnGuildAwaiterListRpDelegate.AddDynamic(this, &UUC_Guild_Awaiter::OnGuildAwaiterListRpDelegate);
	}

	CurrentPage = 1;
	RequestPage = 1;
	BackUpData.Empty();

	if (ScrollView_AwaiterList)
	{
		ScrollView_AwaiterList->Init();
		ScrollView_AwaiterList->SetVisibility(ESlateVisibility::Hidden);
	}
	Button_NextAwaiterPage->OnClicked.AddDynamic(this, &UUC_Guild_Awaiter::OnClick_AwaiterNext);
	Button_PrevAwaiterPage->OnClicked.AddDynamic(this, &UUC_Guild_Awaiter::OnClick_AwaiterPrev);
	Button_Close->OnClicked.AddDynamic(this, &UUC_Guild_Awaiter::CloseAwaiter);
}

void UUC_Guild_Awaiter::NativeDestruct()
{
	RGAMEINSTANCE(this)->HttpClient->OnGuildAwaiterListRpDelegate.RemoveDynamic(this, &UUC_Guild_Awaiter::OnGuildAwaiterListRpDelegate);

	Button_NextAwaiterPage->OnClicked.RemoveDynamic(this, &UUC_Guild_Awaiter::OnClick_AwaiterNext);
	Button_PrevAwaiterPage->OnClicked.RemoveDynamic(this, &UUC_Guild_Awaiter::OnClick_AwaiterPrev);
	Button_Close->OnClicked.RemoveDynamic(this, &UUC_Guild_Awaiter::CloseAwaiter);

	Super::NativeDestruct();
}

void UUC_Guild_Awaiter::OpenAwaiter()
{
	RequestAwaiterList();
}

void UUC_Guild_Awaiter::CloseAwaiter()
{
	this->SetVisibility(ESlateVisibility::Hidden);
	RGAMEINSTANCE(this)->EventManager->OnRequestGuildMember.Broadcast();
}

void UUC_Guild_Awaiter::OnClick_AwaiterPrev()
{
	if (Button_PrevAwaiterPage)
	{
		if (Button_PrevAwaiterPage->GetIsEnabled())
		{
			--RequestPage;

			RequestAwaiterList();
		}
	}
}

void UUC_Guild_Awaiter::OnClick_AwaiterNext()
{
	if (Button_NextAwaiterPage)
	{
		if (Button_NextAwaiterPage->GetIsEnabled())
		{
			++RequestPage;

			RequestAwaiterList();
		}
	}
}

void UUC_Guild_Awaiter::RequestAwaiterList()
{
	URGameInstance* RGameInstance = RGAMEINSTANCE(this);

	if (Button_PrevAwaiterPage)
	{
		Button_PrevAwaiterPage->SetIsEnabled(false);
	}
	if (Button_NextAwaiterPage)
	{
		Button_NextAwaiterPage->SetIsEnabled(false);
	}

	if (0 >= RequestPage)
		RequestPage = 1;
	if (99 < RequestPage)
		RequestPage = 99;

	BackUpData = RGameInstance->GuildInfo->AwaiterList;

	UPacketFunctionLibrary::GUILD_AWAITER_LIST_RQ(RequestPage);
}

void UUC_Guild_Awaiter::OnGuildAwaiterListRpDelegate(bool ApplyData)
{
	this->SetVisibility(ESlateVisibility::Visible);

	URGameInstance* RGameInstance = RGAMEINSTANCE(this);

	if (Button_PrevAwaiterPage)
	{
		Button_PrevAwaiterPage->SetIsEnabled(true);
	}
	if (Button_NextAwaiterPage)
	{
		Button_NextAwaiterPage->SetIsEnabled(true);
	}

	if (ApplyData)
	{
		CurrentPage = RequestPage;
	}
	else
	{
		RequestPage = CurrentPage;
		RGameInstance->GuildInfo->AwaiterList = BackUpData;
	}

	if (Text_AwaiterPage)
	{
		Text_AwaiterPage->SetText(FText::AsNumber(CurrentPage));
	}

	SetScrollDataGuildAwaiterList();
}

void UUC_Guild_Awaiter::SetScrollDataGuildAwaiterList()
{
	URGameInstance* RGameInstance = RGAMEINSTANCE(this);

	if (!IsValid(ScrollView_AwaiterList))
		return;

	ScrollView_AwaiterList->ClearScrollItemData();
	ScrollView_AwaiterList->SetVisibility(ESlateVisibility::Visible);

	TArray<FGUILD_AWAITER> GuildAwaiterList = RGameInstance->GuildInfo->AwaiterList;
	EGUILD_GRADE MyGrade = RGameInstance->GuildInfo->MyMemberData.grade;

	for (FGUILD_AWAITER& GuildAwaiter : GuildAwaiterList)
	{
		UGuildAwaiter_ScrollItemData* ScrollItemData = Cast<UGuildAwaiter_ScrollItemData>(ScrollView_AwaiterList->AddScrollItemData(UGuildAwaiter_ScrollItemData::StaticClass()));
		if (ScrollItemData)
		{
			ScrollItemData->GuildAwaiterData = GuildAwaiter;
			ScrollItemData->MyGuildGrade = MyGrade;
		}
	}

	ScrollView_AwaiterList->InvalidateData();
}
