// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/Control/RScrollView.h"
#include "RChatScrollView.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTR_API URChatScrollView : public URScrollView
{
	GENERATED_BODY()
	
public:
	void SetSelected(URScrollItem* SelecteItem) override;

	bool IsTailOfScroll();
	void SetScrollToTail();
	void SetScrollPos(FVector2D diff);
};
