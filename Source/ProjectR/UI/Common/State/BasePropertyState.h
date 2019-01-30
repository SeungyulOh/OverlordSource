// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Object.h"
#include "BasePropertyState.generated.h"

class UUC_TopBar_Property;

/**
 * 
 */
UCLASS()
class PROJECTR_API UBasePropertyState : public UObject
{
	GENERATED_BODY()
	
	
public:
	
	virtual void SetText(UUC_TopBar_Property* StateOwner);

	//base method(do not call concrete state)
	void RefreshInfo(UUC_TopBar_Property* StateOwner);
	void SetImage(UUC_TopBar_Property* StateOwner);
	void Onclick_DirectLink(UUC_TopBar_Property* StateOwner);
};
