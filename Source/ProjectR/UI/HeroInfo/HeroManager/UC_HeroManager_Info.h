// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "UI/ItemManagement/UC_ItemInventoryBase.h"

#include "UC_HeroManager_Info.generated.h"

class UUC_Hero_Skill_Icon;
class UUC_HeroInfoStat;
class UCharacterBattleInfo;
struct FCharacterTableInfo;
struct FCharacterCostGrowth;

UCLASS()
class PROJECTR_API UUC_HeroManager_Info : public UUC_ItemInventoryBase
{
	GENERATED_BODY()
//	맴버 함수
public:
	void NativeConstruct() override;
	void NativeDestruct() override;
	void InvalidateData() override;

	void Init();
	void SetHero(FString InHeroUD);
	void SetHeroInfo();
	void SetHeroSkill();
	void SetHeroStat();

	void SetCurrentTab(EItemInventoryEnum InEnum);

	UFUNCTION()
	void OnClick_Lock();

	void OnItemInventoryUpdated(EItemInventoryUpdate Result, bool bWidgetFinished, URScrollItem* UpdatedScrollItem, EWidgetBluePrintEnum WidgetReturnedFrom) override;
	virtual void InvalidateItemInventoryScrollView_Heros() override;
protected:
	void OnTabCheckStateChanged(bool bIsChecked, int32 Index) override;
	void OnInventoryScrollViewSelectedItem_EquipmentItem(UUC_ItemInventory_ScrollItem* SelectedItem) override;
	EItemInventoryEnum TabIndexToInventoryEnum(int32 Index) override;
	bool IsIncludingEquipmentItem(const FItemTableInfo* ItemInfo, const FITEM* ItemData) override;
	
private:

//	맴버 변수
public:
	UPROPERTY()
	UWidgetSwitcher*				Switcher_ItemInventory	=	nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "HeroUpgrade")
	UTextBlock*						CounterSkillText;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "HeroUpgrade")
	UTextBlock*						CounterSynergyText;

	UPROPERTY()
	UTextBlock*						Text_HeroName			=	nullptr;
	UPROPERTY()
	UTextBlock*						Text_HeroNickName		=	nullptr;
	UPROPERTY()
	UTextBlock*						Text_AttackPower		=	nullptr;
	UPROPERTY()
	UTextBlock*						Text_Level				=	nullptr;
	UPROPERTY()
	UTextBlock*						Text_ReinForce			= nullptr;
	UPROPERTY()
	UTextBlock*						Text_CombatStat			= nullptr;
	UPROPERTY()
	UTextBlock*						Text_HeroExp			= nullptr;
	UPROPERTY()
	UProgressBar*					ProgressBar_HeroExp		= nullptr;
	UPROPERTY()
	UImage*							Image_Job				=	nullptr;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "HeroInfo")
	class UUC_HeroStar*				UC_herostar;

	UPROPERTY()
	TArray<UUC_Hero_Skill_Icon*>	HeroSkillIcons;

	UPROPERTY()
	TArray<UUC_HeroInfoStat*>		HeroStat;

	UPROPERTY()
	UWidgetSwitcher*				Switcher_FilterOn		=	nullptr;
	

	UPROPERTY()
	UButton*						DetailStat_Button		=	nullptr;
	UPROPERTY()
	UButton*						Button_Rock				=	nullptr;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "HeroInfo")
		UImage*						Img_Favorite = nullptr;
	UPROPERTY()
	UButton*						Button_Grind			=	nullptr;
	UPROPERTY()
	UButton*						Button_LevelUp			=	nullptr;
	UPROPERTY()
	UButton*						Button_Upgrade			=	nullptr;
	UPROPERTY()
	UButton*						Button_Enchant			=	nullptr;
	UPROPERTY()
	UButton*						Button_Skills			=	nullptr;

	DECLARE_MULTICAST_DELEGATE(FOnItemInventoryUpdated);
	FOnItemInventoryUpdated OnItemInventoryUpdatedDelegate;

	DECLARE_DELEGATE_OneParam(FOnEquipmentItemSelected, FString);
	FOnEquipmentItemSelected OnEquipmentItemSelected;

protected:
private:
	FString							m_CurHeroUD;
	FHERO*							m_Hero;
	UCharacterBattleInfo*			m_UIBattleInfo;
	FCharacterTableInfo*			m_UITableInfo;
	FCharacterCostGrowth*			m_CostGrowthInfo;

	FString							m_SelectItemUD;
	FString							m_SelectItemCurrentHeroUD;
};