// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/RBaseWidget.h"
#include "Network/PacketDataEnum.h"
#include "SharedConstants/GlobalConstants.h"
#include "UC_StatHeroLevelUp.generated.h"
struct FHeroEnchant;
class SharedStat;
/**
 * 
 */
UCLASS()
class PROJECTR_API UUC_StatHeroLevelUp : public URBaseWidget
{
	GENERATED_BODY()
	
public:
	virtual void NativeConstruct() override;
	virtual void NativeDestruct() override;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UUC_StatHeroLevelUp)
	UTextBlock*		TextBlock_CurrentValule;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UUC_StatHeroLevelUp)
	UTextBlock*		TextBlock_NextValule;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UUC_StatHeroLevelUp)
	UTextBlock*		TextBlock_StatName;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UUC_StatHeroLevelUp)
	UTextBlock*		Text_AddValue;

	void SetStat(const FString& CurrentValue, const FString& NextValue, const FString& StatName, const FString& AddValue , bool bMaxLevel = false);
};
