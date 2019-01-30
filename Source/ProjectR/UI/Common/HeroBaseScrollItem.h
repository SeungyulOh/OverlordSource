// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "UI/Control/RScrollItem.h"
#include "HeroBaseScrollItem.generated.h"

class URenewal_BaseHeroIcon;
/**
 * 
 */

UENUM()
enum class ESelectType : uint8
{
	VE_Selected			UMETA(DisplayName = "Selected"),
	VE_Selecting		UMETA(DisPlayname = "Selecting"),
	VE_None				UMETA(DisPlayname = "None")
};

UCLASS()
class PROJECTR_API UHeroBaseScrollItem : public URScrollItem
{
	GENERATED_BODY()
	
public:
	void		NativeConstruct() override;
	void		InvalidateData() override;
	UFUNCTION(BlueprintCallable, Category = HeroBaseScrollItem)
	void		SelectIconHero();
	
// 	void		SetHeroIconInfo(const FHERO_ICON_INFO& InHeroIconInfo);
	void		SetHeroIconInfoWithUD(FString InHeroUD, EHeroOwnershipType InOwnerShip = EHeroOwnershipType::VE_Me);
	void		SetHeroIconInfoWithID(FName InHeroID, EHeroOwnershipType InOwnerShip = EHeroOwnershipType::VE_Me);
	void		SetHeroIconInfoForInven(const FHERO_ICON_INFO& InHeroIconInfo);

	void		SetSeletingImage();

	void		Selected_OnlyForDeck();
	void		DeselectthisItem();

	UFUNCTION(BlueprintCallable, Category = HeroBaseScrollItem)
	void		SetItemDataSource(int32 InIndex, URScrollItemData* InItem, URScrollView* InScrollView);

	UFUNCTION()
	virtual void SetSelected(bool bSelected) override;

	UFUNCTION()
	virtual void SetItemData(int32 Index, URScrollItemData* item, URScrollView* ScrollView) override;

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = HeroBaseScrollItem)
	URenewal_BaseHeroIcon* HeroIcon = nullptr;
	FString HeroUD;
	ESelectType SelectState = ESelectType::VE_None;
};
