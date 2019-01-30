// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "UI/RBaseWidget.h"
#include "UP_CampaignBossSpawn.generated.h"

class URGameInstance;
/**
 * 
 */
UCLASS()
class PROJECTR_API UUP_CampaignBossSpawn : public URBaseWidget
{
	GENERATED_BODY()
	
public:
	void NativeConstruct() override;
	void NativeDestruct() override;
	void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;

	void SetBossActor(AActor* InBossActor);

	UFUNCTION()
	void BP_ShowBossName(AActor* BossActor);
	UFUNCTION()
	void AfterAnimFinished();

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UP_CampaignBossSpawn)
	UTextBlock* CampaignBossName = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UP_CampaignBossSpawn)
	UTextBlock* CampaignBossName_en = nullptr;


	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UP_CampaignBossSpawn)
	UWidgetAnimation* CampaignBossNameAnim;

private:
	UPROPERTY()
	URGameInstance* RGameInstance = nullptr;

	
};