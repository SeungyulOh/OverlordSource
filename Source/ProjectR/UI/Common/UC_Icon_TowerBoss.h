// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/RBaseWidget.h"
#include "Table/CharacterTableInfo.h"
#include "UI/UIDefs.h"
#include "UI/Control/RScrollItem.h"
#include "UC_Icon_TowerBoss.generated.h"


class UTextBlock;
class UWidgetSwitcher;
class UTexture2D;
class UProgressBar;
class UImage;
class UUC_Notify;
/**
 * 
 */
UCLASS()
class PROJECTR_API UUC_Icon_TowerBoss : public URBaseWidget
{
	GENERATED_BODY()
	
public:
	void NativeConstruct() override;
	void NativeDestruct() override;
	
	void SetBossIconWithUD(FString InHeroUD, EHeroOwnershipType InOwnerType = EHeroOwnershipType::VE_None);
	void SetHeroIconInfoCopy(FHERO_ICON_INFO InHeroIconInfo);

	void HideLockPanel(bool bHide);

	UFUNCTION(BlueprintCallable, Category = UUC_Icon_TowerBoss)
		void SelectIconHero();

	void SelectBossIconBySwitcher();
	//////
	FORCEINLINE void SetIsSelectEnable(bool InbEnableSelect) { bIsSelectEnable = InbEnableSelect; }
	FORCEINLINE UWidgetSwitcher* GetWidgetSelect() { return WidgetSwitcher_Select; }

	bool	IsValidHeroIconInfo(const FHERO_ICON_INFO& InHeroIconInfo);
private:
	void SetBossIcon(FHERO_ICON_INFO InHeroIconInfo);

	///////
	void	SetImages();
	void	SetHeroImage();
	void	SetHeroClassImage();
	void	SetHeroContinentImage();
	///////
	void	SetTexts();
	void	SetHeroLevel();
	void	SetBossFloor();
	//////
	void SetIsLockedPanel();
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UUC_Icon_TowerBoss)
		UTextBlock*		Text_HeroLevel = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UUC_Icon_TowerBoss)
		UTextBlock*		TextBlock_Floor = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UUC_Icon_TowerBoss)
		UImage*			Image_Hero = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UUC_Icon_TowerBoss)
		UImage*			Image_HeroClass = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UUC_Icon_TowerBoss)
		UImage*			Image_Continent = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UUC_Icon_TowerBoss)
		UImage*			ImageBack = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UUC_Icon_TowerBoss)
		UImage*			Grade_10 = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UUC_Icon_TowerBoss)
		UImage*			BG = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UUC_Icon_TowerBoss)
		UOverlay*			DummyHero = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UUC_Icon_TowerBoss)
		UWidgetSwitcher*	WidgetSwitcher_Select = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UUC_Icon_TowerBoss)
		UWidgetSwitcher*	WidgetSwitcher_UseMark = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UUC_Icon_TowerBoss)
		UPanelWidget*	Panel_HeroSelect = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UUC_Icon_TowerBoss)
		UPanelWidget*	Panel_IsLocked = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UUC_Icon_TowerBoss)
		UImage*			Image_Seleting = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "URenewal_BaseHeroIcon")
		UImage*			Icon_Lock = nullptr;

	FHERO_ICON_INFO			HeroIconInfo;
private:
	FCharacterTableInfo*	CharacterTableInfo = nullptr;
	bool bIsSelectEnable = false;
	bool bIsSelected = false;
};
