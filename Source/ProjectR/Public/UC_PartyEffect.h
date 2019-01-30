// Copyright(C) 2018 KOKOMO Games. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "UI/RBaseWidget.h"
#include "UC_PartyEffect.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTR_API UUC_PartyEffect : public URBaseWidget
{
	GENERATED_BODY()

public:
	void Update(TArray<FString> heroUDs);
	
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UUC_PartyEffect")
	TArray<UTextBlock*>	TextEffectArray;
	
};
