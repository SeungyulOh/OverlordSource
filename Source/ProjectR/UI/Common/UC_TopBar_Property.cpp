// Fill out your copyright notice in the Description page of Project Settings.

#include "ProjectR.h"
#include "UC_TopBar_Property.h"

#include "UI/Common/State/BasePropertyState.h"
#include "UI/Common/State/PropertyState.h"
#include "GlobalIntegrated.h"
#include "UtilFunctionIntegrated.h"


void UUC_TopBar_Property::NativeConstruct()
{
	Super::NativeConstruct();

	if (Button_Click)
	{
		Button_Click->OnClicked.RemoveDynamic(this, &UUC_TopBar_Property::OnClick_DirectLink);
		Button_Click->OnClicked.AddDynamic(this, &UUC_TopBar_Property::OnClick_DirectLink);
	}
}

void UUC_TopBar_Property::NativeDestruct()
{
	PropertyState = nullptr;
	Super::NativeConstruct();
}

void UUC_TopBar_Property::SetPropertyType(FName InType)
{
	PropertyType = InType;
	if (!SetState())
		return;
	Refresh();
}

void UUC_TopBar_Property::RefreshCurrent()
{
	Refresh();
}

bool UUC_TopBar_Property::SetState()
{
	if (PropertyState)
	{
		PropertyState->ConditionalBeginDestroy();
		PropertyState = nullptr;
	}
	if (PropertyType == WALLET_GEM)
		PropertyState = NewObject<UGemState>(this);
	else if (PropertyType == WALLET_GOLD)
		PropertyState = NewObject<UGoldState>(this);
	else if (PropertyType == WALLET_STAMINA_PVP)
		PropertyState = NewObject<UStaminaPVPState>(this);
	else if (PropertyType == WALLET_STAMINA)
		PropertyState = NewObject<UStaminaState>(this);
	else if (PropertyType == WALLET_PVPTOKEN)
		PropertyState = NewObject<UPVPTokenState>(this);
	else if (PropertyType == WALLET_FLOORSTORETICKET)
		PropertyState = NewObject<UFloorStoreTicketState>(this);
	else if (PropertyType == WALLET_SUMMONTICKET01)
		PropertyState = NewObject<USummonNomalTicketState>(this);
	else if(PropertyType == WALLET_SUMMONTICKET02)
		PropertyState = NewObject<USummonLuxuryTicketState>(this);
	else if(PropertyType == WALLET_MISSION_ADD_SCROLL)
		PropertyState = NewObject<UMissionAddScrollState>(this);

	if (IsValid(PropertyState) == false)
	{
		return false;
	}
	return true;
}

void UUC_TopBar_Property::Refresh()
{
	if (IsValid(PropertyState) == false)
	{
		this->SetVisibility(ESlateVisibility::Collapsed);
		return;
	}

	PropertyState->RefreshInfo(this);
}

void UUC_TopBar_Property::OnClick_DirectLink()
{
	URGameInstance* RGameInstance = RGAMEINSTANCE(this);
	
	{
		if (RGameInstance->IsAutoPlayMode())
			return;

	}
	EUIMenuIDEnum ContentType = EUIMenuIDEnum::VE_Shop;
	UUIFunctionLibrary::GoContents(ContentType);
}
