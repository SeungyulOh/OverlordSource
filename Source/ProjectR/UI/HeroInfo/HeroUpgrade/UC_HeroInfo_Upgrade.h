// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "UI/RBaseWidget.h"
#include "UI/UIDefs.h"


#include "UI/Common/Renewal_BaseHeroIcon.h"
#include "UI/Common/UC_ItemIcon.h"
#include "UI/HeroInfo/HeroUpgrade/UC_HeroInfoUpgrade_Stat.h"
#include "UI/HeroInfo/HeroUpgrade/UC_HeroInfo_Upgrade_Material.h"

#include "UC_HeroInfo_Upgrade.generated.h"

UCLASS()
class PROJECTR_API UUC_HeroInfo_Upgrade : public URBaseWidget
{
	GENERATED_BODY()
//	¸É¹ö ÇÔ¼ö
public:
	void NativeConstruct() override;
	void NativeDestruct() override;

	void Init();

	void SetHero(FString HeroUD);
	void SetHeroIcon();
	void SetStat();
	void SetUpgradeCost();
	void Filtering(TArray<FString>& outArray);
	EClassTypeEnum GetMaterialClassType(int32 InMaterialIndex);

	UFUNCTION()
	void ResetAnim();

	UFUNCTION()
	void SelectHeroScrollItem(URScrollItem* scroll_item);
	UFUNCTION()
	void OnClickUpgrade();
	UFUNCTION()
	void OnClickIcon1();
	UFUNCTION()
	void OnClickIcon2();
	UFUNCTION()
	void OnClickIcon3();

	UFUNCTION()
	void OnClickClose();

	void SelectMaterialIcon();

protected:
private:

//	¸É¹ö º¯¼ö
public:
	UPROPERTY()
	URenewal_BaseHeroIcon*				PrevIcon			=	nullptr;

	UPROPERTY()
	TArray<URScrollItemData*>			CachedItemDataList;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "HeroUpgrade")
	UUC_MaterialIcon*					MaterialIcon		=	nullptr;

	UPROPERTY(EditAnywhere , BlueprintReadWrite , Category = "HeroUpgrade")
	TArray<UUpgrade_BaseHeroIcon*>		MaterialHeroIcon;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "HeroUpgrade")
	TArray<UVerticalBox*>				MaterialVerticalBoxArray;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "HeroUpgrade")
	TArray<class UUC_HeroStar*>			CurrentHeroStarArray;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "HeroUpgrade")
	class UUC_HeroStar*					NextHeroStar;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "HeroUpgrade")
	TArray<UTextBlock*>					SelectedMaterialTextArray;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "HeroUpgrade")
	TArray<UTextBlock*>					RequiredMaterialTextArray;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "HeroUpgrade")
	TArray<UUC_HeroInfoUpgrade_Stat*>	StatInfo;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "HeroUpgrade")
	UTextBlock* HeroName;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "HeroUpgrade")
	UTextBlock* HeroNickName;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "HeroUpgrade")
	UImage*		HeroJob;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "HeroUpgrade")
	UTextBlock* AttackPower;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "HeroUpgrade")
	UTextBlock* LevelText;






	UPROPERTY()
	UTextBlock*							Text_ActiveCurrency	=	nullptr;
	UPROPERTY()
	UButton*							Button_Upgrade		=	nullptr;
	UPROPERTY()
	UButton*							Button_Close = nullptr;
	UPROPERTY()
	UButton*							Button_Cancel = nullptr;
protected:
private:
	FHERO*								m_Hero				=	nullptr;
	FCharacterTableInfo*				m_PrevTable			=	nullptr;
	FCharacterTableInfo*				m_NextTable			=	nullptr;
	int32								m_MaterialIndex		=	0;
	FCharacterCostUpgrade*				m_UpgradeCost		=	nullptr;

	UPROPERTY()
	TArray<FString>						SeletedHeroUDs;

	TMap<int32, TArray<FString>>		Slot;
};