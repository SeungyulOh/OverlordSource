// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Components/ActorComponent.h"
#include "BattleEntityComponent.h"
#include "ObjectBattleEntityComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class PROJECTR_API UObjectBattleEntityComponent : public UBattleEntityComponent
{
	GENERATED_BODY()
		 
	// Sets default values for this component's properties
		UObjectBattleEntityComponent();

public:
	virtual void					EnterAttack() override;
	virtual void					Dead( bool InIsDestroy = false)  override;
	virtual bool					IsDead()  override;
	virtual bool					IsSearchSkip()  override;
	virtual FVector					GetShotPointLocation(FName SocketName = EXCEPTION_NONE)  override;

};
