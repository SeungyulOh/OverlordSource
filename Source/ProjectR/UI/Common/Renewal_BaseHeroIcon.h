// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "UI/RBaseWidget.h"
#include "Table/CharacterTableInfo.h"
#include "UI/UIDefs.h"
#include "UI/Control/RScrollItem.h"
#include "CustomStructures/HeroSlotInfo.h"
#include "Renewal_BaseHeroIcon.generated.h"

class UTextBlock;
class UWidgetSwitcher;
class UTexture2D;
class UProgressBar;
class UImage;
class UUC_Notify;

/**
 * BP: UC_Icon_Hero
 * embedded in a number of widgets
 */
UCLASS()
class PROJECTR_API URenewal_BaseHeroIcon : public URBaseWidget
{
	GENERATED_BODY()

public:
	void NativeConstruct() override;
	void NativeDestruct() override;


	UFUNCTION(BlueprintCallable, Category = URenewal_BaseHeroIcon)
	void SetHeroIconInfo(FHERO_ICON_INFO InHeroIconInfo);

public:

	void SetHeroIconInfoWithUD(FString InHeroUD, EHeroOwnershipType InOwnerType = EHeroOwnershipType::VE_None);

	void SetHeroIconInfoWithResult(FHeroSlotInfo InHeroSlot);
	void SetResultProgressBar(float InPercent);

	bool GetIsInWidget() { return IsInWidget; }

	void SetHeroIconInfo(FString InHeroUD, EHeroOwnershipType InOwnerType = EHeroOwnershipType::VE_None);
	
	void SetHeroIconInfoWithID(FName InHeroID, EHeroOwnershipType InOwnerType = EHeroOwnershipType::VE_None);

	void SetHeroIconInfoWithFHero(FHERO* PrevHero, FName InNextID);

	void SetHeroIconInfoWithEquipedHero(FEQUIPED_HERO InEquipedHero);

	void SetHeroIconInfoCopy(FHERO_ICON_INFO InHeroIconInfo);

	void SetHeroIconInfoForInven(FHERO_ICON_INFO InHeroIconInfo);

	UFUNCTION(BlueprintCallable, Category = URenewal_BaseHeroIcon)
	void SetOtherHeroIconInfo(const FEQUIPED_HERO& EquippedHero, const FHERO_ICON_INFO& InHeroIconInfo);

	UFUNCTION(BlueprintCallable, Category = URenewal_BaseHeroIcon)
	void SelectIconHero();

	void SelectIconHero(bool bSelected);

	UFUNCTION(BlueprintCallable, Category = URenewal_BaseHeroIcon)
	void SetSwitcherUseMark();

	UFUNCTION(BlueprintCallable, Category = URenewal_BaseHeroIcon)
	void ShowOnlyHeroImage(bool bOnly);

	UFUNCTION(BlueprintCallable, Category = URenewal_BaseHeroIcon)
	void HidePowerPanel(bool bHide);
	
	void HideLockPanel(bool bHide);

	void HideUI();

	void HideIcon();

	void SetIsLockedPanel();

	void SetDummyHero(bool bDummy);
	void SetUsedHeroIcon();
	FORCEINLINE void SetIsSelectEnable(bool InbEnableSelect)			{ bIsSelectEnable = InbEnableSelect; }
	FORCEINLINE void SetIsSelected(bool InbIsSelected)					{ bIsSelected = InbIsSelected; }
	FORCEINLINE bool GetIsSelectEnable()								{ return bIsSelectEnable; }
	FORCEINLINE bool GetIsSelected()									{ return bIsSelected; }
	FORCEINLINE void GetHeroIconInfo(FHERO_ICON_INFO& InHeroIconInfo)	{ InHeroIconInfo = HeroIconInfo; }
	FORCEINLINE FHERO_ICON_INFO* GetHeroIconInfoRef() { return &HeroIconInfo; }
	FORCEINLINE UWidgetSwitcher* GetWidgetSelect() { return WidgetSwitcher_Select; }
	/*
	//////////////////////////////////////////////////////////////////////////////////////
	Related Hero Exp
	//////////////////////////////////////////////////////////////////////////////////////
	*/
	void SetIsAbled(bool bIsAbled);



	UFUNCTION(BlueprintImplementableEvent, Category = URenewal_BaseHeroIcon)
	void OnSetHeroIconSprite(UPaperSprite* InSprite);
	void SetHeroIcon(FString heroID , int32 Level);
	void SetUMGHeroExp();
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "URenewal_BaseHeroIcon")
	UImage*			Image_Seleting = nullptr;
	

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "URenewal_BaseHeroIcon")
	UImage*			Icon_Lock = nullptr;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "URenewal_BaseHeroIcon")
	UImage*			ImageBack = nullptr;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "URenewal_BaseHeroIcon")
	UImage*			Grade_10 = nullptr;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "URenewal_BaseHeroIcon")
	UImage*			BG = nullptr;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "URenewal_BaseHeroIcon")
	UImage*			Grade_soul = nullptr;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "URenewal_BaseHeroIcon")
	TArray<UImage*>	ImgStars;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "URenewal_BaseHeroIcon")
	UHorizontalBox*			HBox_AttackPower = nullptr;
	/*
	//////////////////////////////////////////////////////////////////////////////////////
	Text
	//////////////////////////////////////////////////////////////////////////////////////
	*/

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "URenewal_BaseHeroIcon")
	UTextBlock*		Text_HeroName = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "URenewal_BaseHeroIcon")
	UTextBlock*		Text_HeroLevel = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "URenewal_BaseHeroIcon")
		UTextBlock*		TextBlock_Enchant = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "URenewal_BaseHeroIcon")
	UTextBlock*		Text_CurrentTitan = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "URenewal_BaseHeroIcon")
	UTextBlock*		Text_NeedTitan = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "URenewal_BaseHeroIcon")
	UTextBlock*		Text_AttackPower = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "URenewal_BaseHeroIcon")
	UTextBlock*		Text_Exp = nullptr;

	/*
	//////////////////////////////////////////////////////////////////////////////////////
	Panel
	//////////////////////////////////////////////////////////////////////////////////////
	*/


	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "URenewal_BaseHeroIcon")
	UPanelWidget*	Panel_NewHeroTag = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "URenewal_BaseHeroIcon")
	UPanelWidget*	Panel_AbleWaken = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "URenewal_BaseHeroIcon")
	UPanelWidget*	Panel_IsLocked = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "URenewal_BaseHeroIcon")
	UPanelWidget*	Panel_JobLevel = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "URenewal_BaseHeroIcon|Ref")
	UPanelWidget*	Panel_Power = nullptr;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "URenewal_BaseHeroIcon|Ref")
	UVerticalBox*	VerticalBox_Panel = nullptr;

	/*
	//////////////////////////////////////////////////////////////////////////////////////
	Image
	//////////////////////////////////////////////////////////////////////////////////////
	*/

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "URenewal_BaseHeroIcon")
	UImage*			Image_Hero = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "URenewal_BaseHeroIcon")
	UImage*			Image_IsAbleAwaken = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "URenewal_BaseHeroIcon")
	UImage*			Image_HeroClass = nullptr;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "URenewal_BaseHeroIcon")
	UImage*			Image_Continent = nullptr;
	

	/*
	//////////////////////////////////////////////////////////////////////////////////////
	Switcher
	//////////////////////////////////////////////////////////////////////////////////////
	*/

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "URenewal_BaseHeroIcon")
	UWidgetSwitcher*	WidgetSwitcher_OwnStatus = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "URenewal_BaseHeroIcon")
	UWidgetSwitcher*	WidgetSwitcher_UnOwned = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "URenewal_BaseHeroIcon")
	UWidgetSwitcher*	WidgetSwitcher_Owned = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "URenewal_BaseHeroIcon")
	UWidgetSwitcher*	WidgetSwitcher_Select = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "URenewal_BaseHeroIcon")
	UWidgetSwitcher*	WidgetSwitcher_Titan = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "URenewal_BaseHeroIcon")
	UWidgetSwitcher*	WidgetSwitcher_UseMark = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "URenewal_BaseHeroIcon")
	UWidgetSwitcher*	WidgetSwitcher_Star = nullptr;

	/*
	//////////////////////////////////////////////////////////////////////////////////////
	ProgressBar
	//////////////////////////////////////////////////////////////////////////////////////
	*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "URenewal_BaseHeroIcon")
	UProgressBar*	ProgressBar_RewardExp = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "URenewal_BaseHeroIcon")
	UProgressBar*	ProgressBar_OwnedExp = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "URenewal_BaseHeroIcon")
	UUC_Notify*		Notify = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "URenewal_BaseHeroIcon")
	UPanelWidget*		HeroHpPanel = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "URenewal_BaseHeroIcon")
	UProgressBar*		HpProgressBar = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "URenewal_BaseHeroIcon")
	UTextBlock*			HpPercentText = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "URenewal_BaseHeroIcon")
	UPanelWidget*		HeroDeathPanel = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "URenewal_BaseHeroIcon")
	UOverlay*			DummyHero = nullptr;
private:
	FHERO_ICON_INFO			HeroIconInfo;
	FCharacterTableInfo*	CharacterTableInfo = nullptr;
	bool bIsSelectEnable		= false;
	bool bIsSelected			= false;
	/*
	//////////////////////////////////////////////////////////////////////////////////////
	Common
	//////////////////////////////////////////////////////////////////////////////////////
	*/

	/*
	//////////////////////////////////////////////////////////////////////////////////////
	Image
	//////////////////////////////////////////////////////////////////////////////////////
	*/
	void	SetImages();
	void	SetHeroImage();
	void	SetHeroClassImage();
	void	SetHeroContinentImage();
	void	SetHeroGradeImage();

	/*
	//////////////////////////////////////////////////////////////////////////////////////
	Text
	//////////////////////////////////////////////////////////////////////////////////////
	*/
	void	SetTexts();
	void	SetHeroName();
	void	SetHeroLevel();
	void	SetAttackPower();
	void	SetHeroEnchantPoint();

	/*
	//////////////////////////////////////////////////////////////////////////////////////
	Switcher
	//////////////////////////////////////////////////////////////////////////////////////
	*/
	void	SetSwitchers();
	void	SetOwnStatus();
	void	SetUnOwnedStatus();
	void	SetOwnedStatus();

	/*
	//////////////////////////////////////////////////////////////////////////////////////
	ProgressBar
	//////////////////////////////////////////////////////////////////////////////////////
	*/
	void	SetProgressBars();
	void	SetRewardExp();
	void	SetOwnedExp();
	void	SetHeroExp();
	bool	IsValidHeroIconInfo(const FHERO_ICON_INFO& InHeroIconInfo);

	/*
	//////////////////////////////////////////////////////////////////////////////////////
	Image
	//////////////////////////////////////////////////////////////////////////////////////
	*/
	void	SetPanels();

	/*
	//////////////////////////////////////////////////////////////////////////////////////
	Select Hero Icon
	//////////////////////////////////////////////////////////////////////////////////////
	*/
	void SelectHeroIconBySwitcher();
	
	/*
	//////////////////////////////////////////////////////////////////////////////////////
	Notify
	//////////////////////////////////////////////////////////////////////////////////////
	*/
	void SetNotify();


	void SetHp();
	void SetDeath();

};


UCLASS()
class PROJECTR_API UUpgrade_BaseHeroIcon : public URBaseWidget
{
	GENERATED_BODY()

public:
	void NativeConstruct() override;
	void NativeDestruct() override;
private:
	UFUNCTION(BlueprintCallable, Category = UUpgrade_BaseHeroIcon)
	void SetHeroIconInfo(FHERO_ICON_INFO InHeroIconInfo);

public:

	bool GetIsInWidget() { return IsInWidget; }

	void SetHeroIconInfoWithID(FName InHeroID, int32 MaxCount, bool IsSprit);
	void SetClassIcon(EUpgradeCondition InHeroType, EClassTypeEnum InClassType, int32 InGrade, int32 InMaxCount, bool IsSprit);
	void SetAllIcon(EUpgradeCondition InHeroType, int32 InGrade, int32 InMaxCount, bool IsSprit);
	void SetCount(int32 InCount, int32 InMax);

	UFUNCTION(BlueprintCallable, Category = UUpgrade_BaseHeroIcon)
	void SelectIconHero();

	void HideUI();

	void HideIcon();

	void SetUsedHeroIcon(bool use);
	FORCEINLINE void SetIsSelectEnable(bool InbEnableSelect) { bIsSelectEnable = InbEnableSelect; }
	FORCEINLINE void SetIsSelected(bool InbIsSelected) { bIsSelected = InbIsSelected; }
	FORCEINLINE bool GetIsSelectEnable() { return bIsSelectEnable; }
	FORCEINLINE bool GetIsSelected() { return bIsSelected; }
	FORCEINLINE void GetHeroIconInfo(FHERO_ICON_INFO& InHeroIconInfo) { InHeroIconInfo = HeroIconInfo; }
	FORCEINLINE FHERO_ICON_INFO* GetHeroIconInfoRef() { return &HeroIconInfo; }
	FORCEINLINE UWidgetSwitcher* GetWidgetSelect() { return WidgetSwitcher_Select; }

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UUpgrade_BaseHeroIcon")
	UImage*			Image_Seleting		=	nullptr;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "URenewal_BaseHeroIcon")
	UImage*			Image_Selected = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UUpgrade_BaseHeroIcon")
	UImage*			ImageBack			=	nullptr;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UUpgrade_BaseHeroIcon")
	UImage*			Grade_10			=	nullptr;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UUpgrade_BaseHeroIcon")
	UImage*			BG					=	nullptr;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UUpgrade_BaseHeroIcon")
	UImage*			Grade_soul			=	nullptr;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UUpgrade_BaseHeroIcon")
	UButton*		Button_Select		=	nullptr;
protected:

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UUpgrade_BaseHeroIcon|Ref")
	UVerticalBox*	VerticalBox_Panel = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UUpgrade_BaseHeroIcon")
	UImage*			Image_Hero = nullptr;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UUpgrade_BaseHeroIcon")
	UImage*			Image_Continent = nullptr;


	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UUpgrade_BaseHeroIcon|Ref")
	UWidgetSwitcher*	WidgetSwitcher_Select = nullptr;


	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UUpgrade_BaseHeroIcon")
	UWidgetSwitcher*	WidgetSwitcher_Potrait		=	nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UUpgrade_BaseHeroIcon")
	UImage*				Image_HeroClass				=	nullptr;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UUpgrade_BaseHeroIcon")
	UTextBlock*			HeroMinCount				=	nullptr;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UUpgrade_BaseHeroIcon")
	UTextBlock*			HeroMaxCount				=	nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UUpgrade_BaseHeroIcon")
	class UUC_HeroStar* StarWidget;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UUpgrade_BaseHeroIcon")
	UImage*				Image_ClassPortrait			=	nullptr;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UUpgrade_BaseHeroIcon")
	UTextBlock*			ClassMinCount				=	nullptr;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UUpgrade_BaseHeroIcon")
	UTextBlock*			ClassMaxCount				=	nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UUpgrade_BaseHeroIcon")
	UTextBlock*			AllMinCount					=	nullptr;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UUpgrade_BaseHeroIcon")
	UTextBlock*			AllMaxCount					=	nullptr;


private:
	FHERO_ICON_INFO			HeroIconInfo;
	FCharacterTableInfo*	CharacterTableInfo = nullptr;
	bool bIsSelectEnable = false;
	bool bIsSelected = false;

	void	SetImages();
	void	SetHeroImage();
	void	SetHeroClassImage();
	void	SetHeroContinentImage();
	bool	IsValidHeroIconInfo(const FHERO_ICON_INFO& InHeroIconInfo);
	void	SelectHeroIconBySwitcher();
};
