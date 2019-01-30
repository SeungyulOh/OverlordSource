// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "UI/Control/RScrollItem.h"
#include "UI/RBaseWidget.h"
#include "BaseMonsterIcon.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTR_API UBaseMonsterIcon : public URBaseWidget
{
	GENERATED_BODY()
	
	
public:
	virtual void NativeConstruct()	override;
	virtual void NativeDestruct()	override;

	void SetMonsterIconByIconInfo(const FHERO_ICON_INFO& InHeroIconInfo, bool bIsBossMonster = false);

	void				SetDeath(bool bDeath);
	FORCEINLINE bool	IsDeath() { return bDeath; }
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UBaseMonsterIcon)
	UImage*	BP_Monster_Image;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UBaseMonsterIcon)
	UTextBlock*	BP_Text_MonsterLevel;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UBaseMonsterIcon)
	UImage*	Boss_Name_Tag;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UBaseMonsterIcon)
	UPanelWidget* Level_panel = nullptr;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UBaseMonsterIcon)
	UOverlay*		Death = nullptr;
private:
	bool			bDeath = false;
};
