// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Utils/ObjectPool.h"
#include "WidgetObjectPool.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTR_API UWidgetObjectPool : public UObjectPool
{
	GENERATED_BODY()

public:
	void			Initialize(int32 Size = 100) override;
	void			ResizePool(int32 Size /* = 5 */) override;
	virtual void	ReturnObject(UObject* TargetObject) override;
};
