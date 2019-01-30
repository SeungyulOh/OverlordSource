// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Object.h"

#include "Public/XmlString.h"

#include "XmlManager.generated.h"



/**
 * 

 !!!!!!!! Check calling order!!!!!


 */
UCLASS(BlueprintType)
class PROJECTR_API UXmlManager : public UObject
{
	GENERATED_BODY()
		
public:
	UFUNCTION(BlueprintCallable, Category = XmlManager)
	static UXmlManager* GetInstancePtr();

	UFUNCTION(BlueprintCallable, Category = XmlManager)
	static void	ReleaseSingleton();

	// use blueprint function.
	UFUNCTION(BlueprintCallable, Category = XmlManager)
	void	InitData();

	UXmlString* GetAIString(FName FileName);

	UXmlString* GetAIStarategyString(FName FileName);

	UXmlString* GetEtcString(FName FileName);

	static UXmlString* LoadXmlStringFromPath(const FName& Path);

private:
	UXmlManager();

	static UXmlManager*	InstancePtr;
};
