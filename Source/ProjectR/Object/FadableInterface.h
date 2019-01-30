// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "FadableInterface.generated.h"

// UInterface thing templated from BlendableInterface

/**
 * For FadableStaticMeshActor and FadableSkeletalMeshActor
 */

 /** Dummy class needed to support Cast<IBlendableInterface>(Object). */
UINTERFACE()
class PROJECTR_API UFadableInterface : public UInterface
{
	GENERATED_UINTERFACE_BODY()
};


/**
* Derive from this class
*/
class PROJECTR_API IFadableInterface
{
	GENERATED_IINTERFACE_BODY()

	virtual void FadeOut() = 0;
	virtual void FadeIn() = 0;
};