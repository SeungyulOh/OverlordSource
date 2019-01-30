#pragma once

/**
 * Property class들 한곳에 모음(상문)
 */

#include "UI/Common/State/BasePropertyState.h"
#include "PropertyState.generated.h"

class UUC_TopBar_Property;

/**********************************************************************
*		WALLET_GOLD
***********************************************************************/
UCLASS()
class PROJECTR_API UGoldState : public UBasePropertyState
{
	GENERATED_BODY()

public:
	void SetText(UUC_TopBar_Property* StateOwner) override;
};

/**********************************************************************
*		WALLET_GEM
***********************************************************************/
UCLASS()
class PROJECTR_API UGemState : public UBasePropertyState
{
	GENERATED_BODY()

public:
	void SetText(UUC_TopBar_Property* StateOwner) override;
};

/**********************************************************************
*		WALLET_STAMINA_PVP
***********************************************************************/
UCLASS()
class PROJECTR_API UStaminaPVPState : public UBasePropertyState
{
	GENERATED_BODY()

public:
	void SetText(UUC_TopBar_Property* StateOwner) override;
};

/**********************************************************************
*		WALLET_STAMINA
***********************************************************************/
UCLASS()
class PROJECTR_API UStaminaState : public UBasePropertyState
{
	GENERATED_BODY()

public:
	void SetText(UUC_TopBar_Property* StateOwner) override;
};

/**********************************************************************
*		WALLET_PVPTOKEN
***********************************************************************/
UCLASS()
class PROJECTR_API UPVPTokenState : public UBasePropertyState
{
	GENERATED_BODY()

public:
	void SetText(UUC_TopBar_Property* StateOwner) override;
};

/**********************************************************************
*		WALLET_FLOORSTORETICKET
***********************************************************************/
UCLASS()
class PROJECTR_API UFloorStoreTicketState : public UBasePropertyState
{
	GENERATED_BODY()

public:
	void SetText(UUC_TopBar_Property* StateOwner) override;
};

/**********************************************************************
*		WALLET_SUMMONTICKET01
***********************************************************************/
UCLASS()
class PROJECTR_API USummonNomalTicketState : public UBasePropertyState
{
	GENERATED_BODY()

public:
	void SetText(UUC_TopBar_Property* StateOwner) override;
};

/**********************************************************************
*		WALLET_SUMMONTICKET02
***********************************************************************/
UCLASS()
class PROJECTR_API USummonLuxuryTicketState : public UBasePropertyState
{
	GENERATED_BODY()

public:
	void SetText(UUC_TopBar_Property* StateOwner) override;
};

/**********************************************************************
*		WALLET_MISSION_ADD_SCROLL
***********************************************************************/
UCLASS()
class PROJECTR_API UMissionAddScrollState : public UBasePropertyState
{
	GENERATED_BODY()

public:
	void SetText(UUC_TopBar_Property* StateOwner) override;
};