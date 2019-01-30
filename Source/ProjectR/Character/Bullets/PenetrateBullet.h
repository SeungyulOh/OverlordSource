// Fill out your copyright notice in the Description page of Project Settings.

#pragma once


#include "NormalBullet.h"
#include "PenetrateBullet.generated.h"





/**
 * 
 */
UCLASS()
class PROJECTR_API APenetrateBullet : public ANormalBullet
{
	GENERATED_BODY()
	
public:
	APenetrateBullet(const FObjectInitializer& ObjectInitializer);

};
