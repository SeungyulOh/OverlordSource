#include "ProjectR.h"
#include "PropertyState.h"

#include "UI/Common/UC_TopBar_Property.h"
#include "UI/RWidgetManager.h"

#include "SharedConstants/WalletConstants.h"

#include "Global/RGameInstance.h"

#include "GameInfo/RInventory.h"
#include "GameInfo/UserInfo.h"


/**********************************************************************
*		WALLET_GOLD
***********************************************************************/
void UGoldState::SetText(UUC_TopBar_Property* StateOwner)
{
	if (IsValid(StateOwner) == false)
	{
		return;
	}

	if (IsValid(StateOwner->CurrencyValue) == false)
	{
		return;
	}

	URGameInstance* RGameInstance = RGAMEINSTANCE(this);
// 	if (IsValid(RGameInstance) == false)
// 	{
// 		return;
// 	}

	auto TextValue = FText::AsNumber(RGameInstance->RInventory->GetCurrencyWithKey(WALLET_GOLD));
	StateOwner->CurrencyValue->SetText(TextValue);
}

/**********************************************************************
*		WALLET_GEM
***********************************************************************/
void UGemState::SetText(UUC_TopBar_Property* StateOwner)
{
	if (IsValid(StateOwner) == false)
	{
		return;
	}

	if (IsValid(StateOwner->CurrencyValue) == false)
	{
		return;
	}

	URGameInstance* RGameInstance = RGAMEINSTANCE(this);
// 	if (IsValid(RGameInstance) == false)
// 	{
// 		return;
// 	}

	auto TextValue = FText::AsNumber(RGameInstance->RInventory->GetCurrencyWithKey(WALLET_GEM));
	StateOwner->CurrencyValue->SetText(TextValue);
}

/**********************************************************************
*		WALLET_STAMINA_PVP
***********************************************************************/
void UStaminaPVPState::SetText(UUC_TopBar_Property* StateOwner)
{
	if (IsValid(StateOwner) == false)
	{
		return;
	}

	if (IsValid(StateOwner->CurrencyValue) == false)
	{
		return;
	}

	URGameInstance* RGameInstance = RGAMEINSTANCE(this);
	if (IsValid(RGameInstance->UserInfo) == false)
	{
		return;
	}

	StateOwner->CurrencyValue->SetText(FText::Format(FText::FromString("{0} / {1}"), FText::AsNumber(RGameInstance->RInventory->GetCurrencyWithKey(WALLET_STAMINA_PVP)), FText::AsNumber(RGameInstance->UserInfo->StaminaPVPMax)));
}

/**********************************************************************
*		WALLET_STAMINA
***********************************************************************/
void UStaminaState::SetText(UUC_TopBar_Property* StateOwner)
{
	if (IsValid(StateOwner) == false)
	{
		return;
	}

	if (IsValid(StateOwner->CurrencyValue) == false)
	{
		return;
	}

	URGameInstance* RGameInstance = RGAMEINSTANCE(this);
	if (IsValid(RGameInstance->UserInfo) == false)
	{
		return;
	}

	StateOwner->CurrencyValue->SetText(FText::Format(FText::FromString("{0} / {1}"), FText::AsNumber(RGameInstance->RInventory->GetCurrencyWithKey(WALLET_STAMINA)), FText::AsNumber(RGameInstance->UserInfo->StaminaMax)));
}

/**********************************************************************
*		WALLET_PVPTOKEN
***********************************************************************/
void UPVPTokenState::SetText(UUC_TopBar_Property* StateOwner)
{
	if (IsValid(StateOwner) == false)
	{
		return;
	}

	if (IsValid(StateOwner->CurrencyValue) == false)
	{
		return;
	}

	URGameInstance* RGameInstance = RGAMEINSTANCE(this);
// 	if (IsValid(RGameInstance) == false)
// 	{
// 		return;
// 	}

	StateOwner->CurrencyValue->SetText(FText::AsNumber(RGameInstance->RInventory->GetCurrencyWithKey(WALLET_PVPTOKEN)));
}

/**********************************************************************
*		WALLET_FLOORSTORETICKET
***********************************************************************/
void UFloorStoreTicketState::SetText(UUC_TopBar_Property* StateOwner)
{
	if (!IsValid(StateOwner))
		return;
	if (!IsValid(StateOwner->CurrencyValue))
		return;
	URGameInstance* RGameInstance = RGAMEINSTANCE(this);
// 	if (!IsValid(RGameInstance))
// 		return;

	auto TextValue = FText::AsNumber(RGameInstance->RInventory->GetCurrencyWithKey(WALLET_FLOORSTORETICKET));
	StateOwner->CurrencyValue->SetText(TextValue);
}

/**********************************************************************
*		WALLET_SUMMONTICKET01
***********************************************************************/
void USummonNomalTicketState::SetText(UUC_TopBar_Property* StateOwner)
{
	if (!IsValid(StateOwner))
		return;
	if (!IsValid(StateOwner->CurrencyValue))
		return;
	URGameInstance* RGameInstance = RGAMEINSTANCE(this);
// 	if (!IsValid(RGameInstance))
// 		return;

	auto TextValue = FText::AsNumber(RGameInstance->RInventory->GetCurrencyWithKey(WALLET_SUMMONTICKET01));
	StateOwner->CurrencyValue->SetText(TextValue);
}

/**********************************************************************
*		WALLET_SUMMONTICKET02
***********************************************************************/
void USummonLuxuryTicketState::SetText(UUC_TopBar_Property* StateOwner)
{
	if (!IsValid(StateOwner))
		return;
	if (!IsValid(StateOwner->CurrencyValue))
		return;
	URGameInstance* RGameInstance = RGAMEINSTANCE(this);
// 	if (!IsValid(RGameInstance))
// 		return;

	auto TextValue = FText::AsNumber(RGameInstance->RInventory->GetCurrencyWithKey(WALLET_SUMMONTICKET02));
	StateOwner->CurrencyValue->SetText(TextValue);
}

/**********************************************************************
*		WALLET_MISSION_ADD_SCROLL
***********************************************************************/
void UMissionAddScrollState::SetText(UUC_TopBar_Property* StateOwner)
{
	if (!IsValid(StateOwner))
		return;
	if (!IsValid(StateOwner->CurrencyValue))
		return;
	URGameInstance* RGameInstance = RGAMEINSTANCE(this);
// 	if (!IsValid(RGameInstance))
// 		return;

	auto TextValue = FText::AsNumber(RGameInstance->RInventory->GetCurrencyWithKey(WALLET_MISSION_ADD_SCROLL));
	StateOwner->CurrencyValue->SetText(TextValue);
}
