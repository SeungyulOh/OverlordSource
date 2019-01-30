// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "UI/RBaseWidget.h"
#include "UC_Guild_LevelUpDesc.generated.h"

class URScrollView;
/**
 * 
 */
UCLASS()
class PROJECTR_API UUC_Guild_LevelUpDesc : public URBaseWidget
{
	GENERATED_BODY()
	
public:
	virtual void			NativeConstruct()	override;
	virtual	void			NativeDestruct()	override;

	void OpenLevelUpDesc();
	UFUNCTION(BlueprintCallable, Category = UUC_Guild_LevelUpDesc)
	void CloseLevelUpDesc();

	void MakeLevelUpInfo();

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UUC_Guild_LevelUpDesc)
	URScrollView*	BP_ScrollView_GuildLevel;
};
