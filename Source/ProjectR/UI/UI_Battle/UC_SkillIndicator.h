// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "SharedConstants/GlobalConstants.h"
#include "UI/RBaseWidget.h"
#include "UI/UIDefs.h"

#include "UC_SkillIndicator.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTR_API UUC_SkillIndicator : public URBaseWidget
{
	GENERATED_BODY()

public:
	void	NativeConstruct() override;
	void	NativeDestruct() override;

	UFUNCTION()
	void	OnSkillIndicator(FName id);
	void	SetIndicator(FName& id, float InShowTime = 0.0f);

	UFUNCTION()
	void DelayEvent();

	FORCEINLINE void SetIndex(int32 InIndex) { Index = InIndex; }

public:
	//UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UUC_SkillIndicator)
	//UImage*					HeroImage = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UC_SkillIndicator)
	UImage*					SkillImage = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UC_SkillIndicator)
	UTextBlock*				HeroNameText = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UC_SkillIndicator)
	UTextBlock*				HeroSkillNameText = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UC_SkillIndicator)
	UTextBlock*				HeroSkillDescText = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UC_SkillIndicator)
	bool					IsNotEvent = false;

private:
	UPROPERTY()
	FTimerHandle			DelayTimeHandle;

	int32					Index = -1;
};
