// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "UI/RBaseWidget.h"
#include "Network/PacketDataEnum.h"
#include "Network/PacketDataStructures.h"
#include "UC_Popup_HeroConfig.generated.h"


class URenewal_BaseHeroIcon;

UCLASS()
class PROJECTR_API UUC_Popup_HeroConfig : public URBaseWidget
{
	GENERATED_BODY()

public:

	void NativeConstruct() override;
	void NativeDestruct() override;

	void ShowPopup(ECONTENTS_TYPE InType, FString InHeroUD1, FString InHeroUD2, FString InHeroUD3, FString InHeroUD4, FString InHeroUD5);
public:
	UFUNCTION(BlueprintCallable, Category = UUC_Popup_HeroConfig)
	void SelectHero(int32 InIndex);

	UFUNCTION(BlueprintCallable, Category = UUC_Popup_HeroConfig)
	void SelectPreferTarget(int32 InIndex);

	UFUNCTION(BlueprintCallable, Category = UUC_Popup_HeroConfig)
	void SelectAutoSkill(int32 InIndex);

	UFUNCTION(BlueprintCallable, Category = UUC_Popup_HeroConfig)
	void OnClickOK();
	UFUNCTION(BlueprintCallable, Category = UUC_Popup_HeroConfig)
	void OnClickCancel();

	void ResetSkillSelect();
	void DefaultSetting();
	void SetAutoSkill();
	void DefaultResetConfig(FHERO_CONFIG& InConfig);

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UUC_Popup_HeroConfig)
	TArray<URenewal_BaseHeroIcon*>			HeroIcons;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UUC_Popup_HeroConfig)
	TArray<UOverlay*>						HeroIcons_Box;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UUC_Popup_HeroConfig)
	TArray<UImage*>							TargetSelectImages;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UUC_Popup_HeroConfig)
	TArray<UImage*>							AutoUseSkillImages;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UUC_Popup_HeroConfig)
	TArray<UImage*>							SkillImages;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UUC_Popup_HeroConfig)
	TArray<ULocalizingTextBlock*>			SkillNameText;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UUC_Popup_HeroConfig)
	UVerticalBox*							Skill_3_Box = nullptr;
private:
	TArray<FHERO_CONFIG>		BeforeSaveConfig;
	int32						CurrentHeroIndex = -1;
	int32						CurrentTargetIndex = -1;
	ECONTENTS_TYPE				eContentsType = ECONTENTS_TYPE::CAMPAIGN;
};