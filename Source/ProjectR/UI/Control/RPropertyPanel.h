// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Blueprint/UserWidget.h"
#include "CustomStructures/SharedStructures/CharacterBattleInfo.h"
#include "RPropertyPanel.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTR_API URPropertyPanel : public UUserWidget
{
	GENERATED_BODY()

public:
	void NativeConstruct() override;

public:
	UPROPERTY()
	UCharacterBattleInfo*	CharacterBattleInfo = nullptr;

private:
	void					MakeChildMap();
	UPROPERTY()
	TMap< FString, UTextBlock* >		ChildTextMap;
	UPROPERTY()
	TMap< FString, UTextBlock* >		ChildDetailTextMap;	
	UPROPERTY()
	TMap< FString, UTextBlock* >		ChildPercentTextMap;
	
	UPROPERTY()
	TMap< FString, UTextBlock* >		ChildLeftBracketTextMap;
	UPROPERTY()
	TMap< FString, UTextBlock* >		ChildRightBracketTextMap;
	UPROPERTY()
	TMap< FString, UTextBlock* >		ChildOperatorTextMap;
	UPROPERTY()
	TMap< FString, UTextBlock* >		ChildUnitTextMap;

	bool IsStringZero(FString& InPropertyStr);
	void SetVisibilityPropertyText(FString& InPropertyName, bool IsVisible);
};
