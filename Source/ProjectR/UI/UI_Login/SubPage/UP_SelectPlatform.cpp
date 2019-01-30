// Fill out your copyright notice in the Description page of Project Settings.

#include "ProjectR.h"

#include "GlobalIntegrated.h"

#include "GameInfo/UserInfo.h"
#include "Network/PacketDataStructures.h"
#include "Network/PacketFunctionLibrary.h"
#include "UtilFunctionIntegrated.h"
#include "UI/RWidgetManager.h"

#include "UP_SelectPlatform.h"
#include "UI/UI_Common/UP_CommonPopup.h"
#include "UpdateService/LoginGameMode.h"
#include "UpdateService/LoginDefs.h"
//#include "ShowLoginUICallbackProxy.h"

#define BUILD_KAKAO 0

void UUP_SelectPlatform::NativeConstruct()
{
	Super::NativeConstruct();

	if (Button_Facebook)
	{
		Button_Facebook->OnClicked.RemoveDynamic(this, &UUP_SelectPlatform::OnButton_Facebook);
		Button_Facebook->OnClicked.AddDynamic(this, &UUP_SelectPlatform::OnButton_Facebook);

		//Button_Facebook->SetIsEnabled(false);
	}
	
#if WITH_EDITOR
	if (Button_Guest)
	{
		Button_Guest->OnClicked.RemoveDynamic(this, &UUP_SelectPlatform::OnButton_Guest);
		Button_Guest->OnClicked.AddDynamic(this, &UUP_SelectPlatform::OnButton_Guest);
		Button_Guest->SetVisibility(ESlateVisibility::Visible);
	}
#else
	if (Button_Guest)
	{
		Button_Guest->SetVisibility(ESlateVisibility::Collapsed);
	}
#endif

	if (Switcher_Platform)
	{
		if (Button_Google)
		{
			Button_Google->OnClicked.RemoveDynamic(this, &UUP_SelectPlatform::OnButton_Google);
			Button_Google->OnClicked.AddDynamic(this, &UUP_SelectPlatform::OnButton_Google);
		}
		else
		{
			Switcher_Platform->SetVisibility(ESlateVisibility::Collapsed);
		}
		
		if (Button_IOS)
		{
			Button_IOS->OnClicked.RemoveDynamic(this, &UUP_SelectPlatform::OnButton_IOS);
			Button_IOS->OnClicked.AddDynamic(this, &UUP_SelectPlatform::OnButton_IOS);
		}
		else
		{
			Switcher_Platform->SetVisibility(ESlateVisibility::Collapsed);
		}
		/*
#if PLATFORM_ANDROID
		Switcher_Platform->SetActiveWidgetIndex(0);
#elif PLATFORM_IOS
		Switcher_Platform->SetActiveWidgetIndex(1);
#endif*/

#if BUILD_KAKAO
		Button_Facebook->SetVisibility(ESlateVisibility::Collapsed);
		Switcher_Platform->SetVisibility(ESlateVisibility::Collapsed);
#endif
	}

}

void UUP_SelectPlatform::NativeDestruct()
{
	Super::NativeDestruct();
}

void UUP_SelectPlatform::Refresh()
{
	Super::Refresh();

	if (HBox_Login)
	{
		HBox_Login->SetIsEnabled(true);
	}
}

void UUP_SelectPlatform::OnButton_Facebook()
{
	URAuth* RAuth = URAuth::GetInstancePtr();
	if (RAuth == nullptr)
		return;

	if (HBox_Login)
	{
		HBox_Login->SetIsEnabled(false);
	}

	
	RAuth->PreaprePlatform(EPublisher::Google);
	//OnShowExternalEvent();

	//UUIFunctionLibrary::ShowWaitingPopup(true);
	//InvalidPlatform();
}

void UUP_SelectPlatform::OnButton_Google()
{
	URAuth* RAuth = URAuth::GetInstancePtr();
	if (RAuth == nullptr)
		return;

	if (HBox_Login)
	{
		HBox_Login->SetIsEnabled(false);
	}

	//auto PlayerController = UGameplayStatics::GetPlayerController(GetWorld(), 0);
	//UShowLoginUICallbackProxy::ShowExternalLoginUI(RGameInstance, PlayerController);
	//RAuth->PreaprePlatform(EPublisher::Google);
	RAuth->SetPublisher(EPublisher::Google);
	OnShowExternalEvent();

	

	//InvalidPlatform();
}

void UUP_SelectPlatform::OnButton_IOS()
{
	URAuth* RAuth = URAuth::GetInstancePtr();
	if (RAuth)
	{
		RAuth->PreaprePlatform(EPublisher::IOS);
	}

	OnShowExternalEvent();

	InvalidPlatform();
}

void UUP_SelectPlatform::OnButton_Guest()
{
	URAuth* RAuth = URAuth::GetInstancePtr();
	if (RAuth)
	{
		RAuth->PreaprePlatform(EPublisher::Kokomo);
	}

	CallNextPage();
}


void UUP_SelectPlatform::InvalidPlatform()
{
	CallNextPage();
}

void UUP_SelectPlatform::OnRequestReceive()
{
	//ok
	InvalidPlatform();

	//fail
	//popup
}

void UUP_SelectPlatform::TestSuccess()
{
	URAuth* RAuth = URAuth::GetInstancePtr();
	if (RAuth)
	{
		RAuth->LoginCompletePlatform();
	}

	
	InvalidPlatform();
}

void UUP_SelectPlatform::TestFail()
{
	if (HBox_Login)
	{
		HBox_Login->SetIsEnabled(true);
	}

#if WITH_EDITOR
	//InvalidPlatform();
#else
	ALoginGameMode* GameMode = Cast<ALoginGameMode>(UGameplayStatics::GetGameMode(GetWorld()));
	if (GameMode)
	{
		FText InCaption = FText::FromString(TEXT("Warning"));
		FText InDesc = FText::FromString(TEXT("Platform Login Error"));
		GameMode->ShowExceptionLoginPopup(InCaption, InDesc);
	}
#endif
}

/*void CheckAlreadyPlatfromAccount()
{

	InvalidPlatform();
}*/
