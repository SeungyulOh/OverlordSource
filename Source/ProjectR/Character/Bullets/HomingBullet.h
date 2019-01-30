// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "NormalBullet.h"
#include "HomingBullet.generated.h"





/**
*
*/
UCLASS()
class PROJECTR_API AHomingBullet : public ANormalBullet
{
	GENERATED_BODY()

public:
	AHomingBullet(const FObjectInitializer& ObjectInitializer);

};
