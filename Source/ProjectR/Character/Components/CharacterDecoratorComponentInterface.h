// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CharacterDecoratorComponentInterface.generated.h"

/** Class needed to support InterfaceCast<IToStringInterface>(Object) */
UINTERFACE(MinimalAPI)
class  UCharacterDecoratorComponentInterface : public UInterface
{
	GENERATED_UINTERFACE_BODY()
};

class ICharacterDecoratorComponentInterface
{
	GENERATED_IINTERFACE_BODY()
	
public:

	virtual void	SetOwner(AActor* TargetActor);

};