// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "SharedConstants/GlobalConstants.h"
#include "Character/Components/FunctionalComponents/BattleEntityComponent.h"
#include "Character/Components/FunctionalComponents/SkillEntityComponent.h"
#include "Character/Components/FunctionalComponents/EntityAIComponent.h"
#include "Character/Components/FunctionalComponents/EntityRenderComponent.h"
#include "Character/Components/RemoteMoveComponent.h"
#include "Character/Components/AnimStateComponent.h"
#include "EntityComponentGetterInterface.generated.h"



/** Class needed to support InterfaceCast<IToStringInterface>(Object) */
UINTERFACE(MinimalAPI)
class  UEntityComponentGetterInterface : public UInterface
{
	GENERATED_UINTERFACE_BODY()
};

class IEntityComponentGetterInterface
{
	GENERATED_IINTERFACE_BODY()
	
public:

	virtual	UBattleEntityComponent* GetBattleEntityComponent() { return nullptr; }
	virtual	USkillEntityComponent*	GetSkillEntityComponent() { return nullptr; }
	virtual	UEntityAIComponent*		GetEntityAIComponent() { return nullptr; }
	virtual	UEntityRenderComponent* GetEntityRenderComponent() { return nullptr; }
	virtual UAnimStateComponent*	GetAnimStateComponent() { return nullptr; }
	virtual URemoteMoveComponent*	GetRemoteMoveComponent() { return nullptr; }
};