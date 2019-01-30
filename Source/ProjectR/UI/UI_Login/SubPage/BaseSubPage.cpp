// Fill out your copyright notice in the Description page of Project Settings.

#include "ProjectR.h"

#include "GlobalIntegrated.h"

#include "GameInfo/UserInfo.h"
#include "Network/PacketDataStructures.h"
#include "Network/PacketFunctionLibrary.h"
#include "UtilFunctionIntegrated.h"
#include "UI/RWidgetManager.h"

#include "BaseSubPage.h"


void UBaseSubPage::NativeConstruct()
{
	Super::NativeConstruct();


	if (Button_Click)
	{
		Button_Click->OnClicked.RemoveDynamic(this, &UBaseSubPage::OnButton_Click);
		Button_Click->OnClicked.AddDynamic(this, &UBaseSubPage::OnButton_Click);
	}

	if (Button_Close)
	{
		Button_Close->OnClicked.RemoveDynamic(this, &UBaseSubPage::OnButton_Close);
		Button_Close->OnClicked.AddDynamic(this, &UBaseSubPage::OnButton_Close);
	}
}

void UBaseSubPage::NativeDestruct()
{
	Super::NativeDestruct();
}

void UBaseSubPage::Refresh()
{
	this->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
}


void UBaseSubPage::CallNextPage()
{
	OnButton_Close();
	
	//delegatetest
	/*DelegateHandle = AddOnUpdatePageDelegate_Handle(FOnUpdatePageDelegate::CreateUObject(this, &ThisClass::OnButton_Close));

	this->TriggerOnUpdatePageDelegates();

	ClearOnUpdatePageDelegate_Handle(DelegateHandle);*/


	RGAMEINSTANCE(this)->EventManager->OnUpdatePage.Broadcast();
}


void UBaseSubPage::OnButton_Click()
{
	
}

void UBaseSubPage::OnButton_Close()
{
	this->SetVisibility(ESlateVisibility::Collapsed);
}

