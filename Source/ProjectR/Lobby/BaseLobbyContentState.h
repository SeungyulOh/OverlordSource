// Copyright 1998-2016 Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "BaseLobbyContentState.generated.h"


/*
 * state object that represents what UIMenuIDEnum used to represent.
 * handles level configuration per game content.
 * examples
 * * HeroInventory content wants selected hero to be displayed on the level.
 * * Raid content wants Akhan displayed.
 * * Shop may want gacha animation played
 * controlled by RWidgetManage UI Pages
 *
 * a content state may be externally configured to display a certain information on the entry
 * e.g. shortcut to the detail panel of an item
 *
 * owned by TowerLobbyGameMode
*/
UCLASS()
class PROJECTR_API UBaseLobbyContentState : public UObject {

	GENERATED_BODY()
public:
	//// demands which LobbyStateActor it wants
	//virtual void Enter();
	//virtual void Leave();

	//// Called after all sublevel loading and transition animation are finished
	//// show UI. maybe show notice popups.
	//virtual void Begin();
	//virtual void End();
};
