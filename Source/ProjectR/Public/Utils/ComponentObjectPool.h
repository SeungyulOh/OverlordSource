// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Utils/ObjectPool.h"
#include "ComponentObjectPool.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTR_API UComponentObjectPool : public UObjectPool
{
	GENERATED_BODY()
	
	
public:
	void Initialize(int32 Size) override;

};
