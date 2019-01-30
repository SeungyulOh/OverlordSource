// Fill out your copyright notice in the Description page of Project Settings.

#include "ProjectR.h"
#include "GlobalIntegrated.h"
#include "WidgetObjectPool.h"

void UWidgetObjectPool::Initialize(int32 Size)
{
	PoolSize = Size;

	UnusedPool.Empty();

	URGameInstance*	RGameInstance = RGAMEINSTANCE(this);

	for (int32 i = 0; i < PoolSize; ++i)
	{
		auto UserWidget = CreateWidget<UUserWidget>(RGameInstance, ObjectType);
		if (IsValid(UserWidget))
		{ 
			UnusedPool.Emplace(UserWidget);
		}
	}
}

void UWidgetObjectPool::ResizePool(int32 Size)
{
	PoolSize += Size;

	URGameInstance*	RGameInstance = RGAMEINSTANCE(this);

	for (int32 i = 0; i < Size; ++i)
	{
		auto UserWidget = CreateWidget<UUserWidget>(RGameInstance, ObjectType);
		if (IsValid(UserWidget))
		{
			UnusedPool.Emplace(UserWidget);
		}
	}
}

void UWidgetObjectPool::ReturnObject(UObject* TargetObject)
{
	Super::ReturnObject(TargetObject);

	auto TargetWidget = Cast<UUserWidget>(TargetObject);
	if (IsValid(TargetWidget))
	{
		if (TargetWidget->IsInViewport() == true)
			TargetWidget->RemoveFromParent();
	}
}