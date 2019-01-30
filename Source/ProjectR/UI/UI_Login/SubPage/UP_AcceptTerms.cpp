// Fill out your copyright notice in the Description page of Project Settings.

#include "ProjectR.h"

#include "GlobalIntegrated.h"

#include "GameInfo/UserInfo.h"
#include "Network/PacketDataStructures.h"
#include "Network/PacketFunctionLibrary.h"
#include "UtilFunctionIntegrated.h"
#include "UI/RWidgetManager.h"

#include "UP_AcceptTerms.h"

#define CLICK_ACCEPT1 1
#define CLICK_ACCEPT2 2

void UUP_AcceptTerms::NativeConstruct()
{
	Super::NativeConstruct();

	if (Button_Accept1)
	{
		Button_Accept1->OnClicked.RemoveDynamic(this, &UUP_AcceptTerms::OnButton_Accept1);
		Button_Accept1->OnClicked.AddDynamic(this, &UUP_AcceptTerms::OnButton_Accept1);
	}

	if (Button_Accept2)
	{
		Button_Accept2->OnClicked.RemoveDynamic(this, &UUP_AcceptTerms::OnButton_Accept2);
		Button_Accept2->OnClicked.AddDynamic(this, &UUP_AcceptTerms::OnButton_Accept2);
	}

	Internal_Init();
}

void UUP_AcceptTerms::NativeDestruct()
{
	IsInitActive = false;

	Super::NativeDestruct();
}

void UUP_AcceptTerms::Refresh()
{
	bIsAccept1 = false;
	bIsAccept2 = false;

	Internal_Accept(CLICK_ACCEPT1);
	Internal_Accept(CLICK_ACCEPT2);

	//OnRefreshEvent();

	AddWebViewer();

	Super::Refresh();
}


void UUP_AcceptTerms::CallNextPage()
{
	Super::CallNextPage();
}

void UUP_AcceptTerms::OnButton_Close()
{
	bIsAccept1 = false;
	bIsAccept2 = false;

	//OnCloseEvent();
	
	//RebuildWidget();
	RemoveWebViewer();

	Super::OnButton_Close();
}

void UUP_AcceptTerms::OnButton_Accept1()
{
	bIsAccept1 = !(bIsAccept1);

	Internal_Accept(CLICK_ACCEPT1);
}

void UUP_AcceptTerms::OnButton_Accept2()
{
	bIsAccept2 = !(bIsAccept2);

	Internal_Accept(CLICK_ACCEPT2);
}

void UUP_AcceptTerms::Internal_Init()
{
	if (Button_Accept1)
	{
		auto& TargetStyle = Button_Accept1->WidgetStyle;
		AcceptColor = TargetStyle.Normal.TintColor;
	}
	
	AddWebViewer();
	
	Internal_Accept(CLICK_ACCEPT1);
	Internal_Accept(CLICK_ACCEPT2);
}

void UUP_AcceptTerms::Internal_Accept(int32 InClickIndex)
{
	bool bIsTargetAccept;
	UButton* TargetButton;
	UHorizontalBox*	TargetHBox;

	switch (InClickIndex)
	{
		case CLICK_ACCEPT1:
		{
			bIsTargetAccept = bIsAccept1;
			TargetButton = Button_Accept1;
			TargetHBox = HBox_Accept1;
		}
		break;
		case CLICK_ACCEPT2:
		{
			bIsTargetAccept = bIsAccept2;
			TargetButton = Button_Accept2;
			TargetHBox = HBox_Accept2;
		}
		break;
		default:
			return; //error
		break;
	}

	if (bIsTargetAccept)
	{
		if (TargetButton)
		{
			TargetButton->WidgetStyle.Normal.TintColor = AcceptColor;
			TargetButton->WidgetStyle.Hovered.TintColor = AcceptColor;
			TargetButton->WidgetStyle.Pressed.TintColor = AcceptColor;
		}

		if (TargetHBox)
		{
			TargetHBox->SetRenderOpacity(1.0f);
		}

	}
	else
	{
		if (TargetButton)
		{
			TargetButton->WidgetStyle.Normal.TintColor = NoAcceptColor;
			TargetButton->WidgetStyle.Hovered.TintColor = NoAcceptColor;
			TargetButton->WidgetStyle.Pressed.TintColor = NoAcceptColor;
		}

		if (TargetHBox)
		{
			TargetHBox->SetRenderOpacity(0.5f);
		}
	}


	if (bIsAccept1 & bIsAccept2)
	{
		CallNextPage();
	}
}

void UUP_AcceptTerms::AddWebViewer()
{
	if (Overlay_Web && CachedWidget == nullptr)
	{
		UClass* WidgetUClass = WebViewer.Get();
		if (WidgetUClass == nullptr)
		{
			WebViewer.ToSoftObjectPath().TryLoad();
			WidgetUClass = WebViewer.Get();
		}

		CachedWidget = CreateWidget<UUserWidget>(GetWorld(), WidgetUClass);
		if (CachedWidget)
		{
			UOverlaySlot* OverlayWebSlot = Overlay_Web->AddChildToOverlay(CachedWidget);
			if (OverlayWebSlot)
			{
				OverlayWebSlot->SetHorizontalAlignment(EHorizontalAlignment::HAlign_Center);
				OverlayWebSlot->SetVerticalAlignment(EVerticalAlignment::VAlign_Center);
			}
		}
	}
}

void UUP_AcceptTerms::RemoveWebViewer()
{
	if (CachedWidget)
	{
		Overlay_Web->RemoveChild(CachedWidget);
		CachedWidget->SetVisibility(ESlateVisibility::Collapsed);
		CachedWidget->RemoveFromParent();
		CachedWidget = nullptr;
	}
}
