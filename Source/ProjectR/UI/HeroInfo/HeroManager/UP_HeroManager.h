// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "UC_HeroManager_Info.h"

#include "UI/RBaseWidget.h"
#include "UI/UIDefs.h"

#include "UI/HeroManagement/UC_HeroItems.h"
#include "UI/HeroManagement/UC_SkillManagement.h"

#include "UI/HeroInfo/HeroManager/UC_Popup_HeroInfo_BreakUp.h"
#include "UI/HeroInfo/HeroManager/UC_HeroStatMgr_Info.h"
#include "UI/HeroInfo/HeroLevelUp/UC_HeroLevelUp.h"
#include "UI/HeroInfo/HeroLevelUp/UC_Popup_HeroInfo_LevelUp.h"
#include "UI/HeroInfo/HeroLevelUp/UC_Popup_HeroLevelUp_Splash.h"

#include "UI/ItemManagement/UC_HeroItemInfo.h"
#include "UI/ItemManagement/UC_ItemInventoryFilter.h"

#include "UP_HeroManager.generated.h"

UCLASS()
class PROJECTR_API UUC_HeroStar : public URBaseWidget
{
	GENERATED_BODY()

public:
	void SetStar(FString heroUD);
	void SetStar(int32 StarCount);

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UUC_HeroStar")
	UWidgetSwitcher*				WidgetSwitcher_Star = nullptr;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UUC_HeroStar")
	TArray<UImage*>					HeroStar;
};



UCLASS()
class PROJECTR_API UUP_HeroManager : public URBaseWidget
{
	GENERATED_BODY()
//	맴버 함수
public:
	void NativeConstruct() override;
	void NativeDestruct() override;
	void Init();
	void SetHeroInfo();
	bool IsLock();
	bool IsMaxLevel();

	FString GetCurrentHeroUD() {return m_CurHeroUD;}

	UFUNCTION()
	void OnClick_HeroIcon(FString InHeroUD);
	

	UFUNCTION()
	void OnClick_DetailStat();
	UFUNCTION()
	void OnClick_Grind();
	UFUNCTION()
	void OnClick_LevelUp();
	UFUNCTION()
	void OnClick_Upgrade();
	UFUNCTION()
	void OnClick_Enchant();
	UFUNCTION()
	void OnClick_Skills();
	UFUNCTION()
	void OnClickLevelUpPopup();
	UFUNCTION()
	void OnClickLevelUpOk();
	UFUNCTION()
	void OnClickLevelUpCancel();
	
	UFUNCTION()
	void OnClickChange();
	UFUNCTION()
	void OnClickBag();

	UFUNCTION()
	void OnHeroUpgrade();
	UFUNCTION()
	void OnRewardPopup();
	UFUNCTION()
	void OnGrindPopup(const TArray<FREWARD>& Inrewards);
	UFUNCTION()
	void OnHeroLock();
	UFUNCTION()
	void OnButtonFilterShowClicked();
	UFUNCTION()
	void OnItemInfoItemInventoryUpdated(EItemInventoryUpdate ItemInventoryUpdateEnum, bool bWidgetComplete, bool IsItemInfoRight);
protected:
	UFUNCTION()
	void OnItemInventoryUpdated();
	

private:

//	맴버 변수
public:
	UPROPERTY()
	UUC_HeroItems*							UC_HeroItems				=	nullptr;
	UPROPERTY()
	UUC_HeroManager_Info*					Hero_Info					=	nullptr;
	UPROPERTY()
	UTextBlock*								Text_Name					=	nullptr;
	UPROPERTY()
	UTextBlock*								Text_Nick					=	nullptr;
	UPROPERTY()
	UTextBlock*								Text_MinLevel				=	nullptr;
	UPROPERTY()
	UTextBlock*								Text_MaxLevel				=	nullptr;
	UPROPERTY()
	UTextBlock*								Text_ReinForce				=	nullptr;
	UPROPERTY()
	UTextBlock*								Text_AttackPower			=	nullptr;

	UPROPERTY()
	UUC_HeroStatMgr_Info*					HeroStatInfo				=	nullptr;
	UPROPERTY()
	UUC_HeroLevelUp*						LevelUpPopup				=	nullptr;
	
	UPROPERTY()
	UUC_PopupContent_ItemInventoryFilter*	ItemInventoryFilter			=	nullptr;

	/*-Renewal-*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "HeroManager")
	UButton* ChangeButton;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "HeroManager")
	UButton* BagButton;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "HeroManager")
	UUC_HeroStar* UC_heroStar;
	/**/
	
private:
	UPROPERTY()
	FString							m_CurHeroUD;
	FHERO*							m_Hero;
	UPROPERTY()
	FHERO_ICON_INFO					m_PrevHeroIcon;
	UPROPERTY()
	UCharacterBattleInfo*			m_UIBattleInfo;
	FCharacterTableInfo*			m_UITableInfo;
	EItemEquipPositionEnum			CurrentEquipPosition		=	EItemEquipPositionEnum::VE_Weapon;
	//
	FHERO BeforeHeroUpgradeData;

	const int32 MaxGrade = 10;
};