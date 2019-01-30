// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "UI/Control/RBaseButton.h"
//#include "UI/RBaseWidget.h"
#include "UI/UIDefs.h"

#include "Network/PacketDataStructures.h"
#include "RBaseDeckWidget.h"
#include "UI/Common/ScrollView/UP_HeroScrollView_Bottom.h"

#include "UC_Arena_DeckBtn.generated.h"


class URenewal_BaseHeroIcon;
class UHeroBaseScrollItem;
/**
 * 
 */
UCLASS()
class PROJECTR_API UUC_Arena_DeckBtn : public URBaseDeckWidget
{
	GENERATED_BODY()
	
public:

	void NativeConstruct() override;
	void NativeDestruct() override;

	void SetAvailable(bool inAvailable) override;
	void SetSelecting(bool bSelecting) override;
	void SetHero(FString heroUD) override;

	UFUNCTION()
	void OnClick_DeckButton();

	void Init(FDeckManager_Blackboard* bb, int deck_pos, bool bIsMyHero);

	UFUNCTION(BlueprintCallable, Category = UUC_Arena_DeckBtn)
	void InitDeckBtnState();

	UFUNCTION(BlueprintCallable, Category = UUC_Arena_DeckBtn)
	void SetIconHeroIcon();

	UFUNCTION(BlueprintCallable, Category = UUC_Arena_DeckBtn)
	void ChangeBtnState(bool select);

	UFUNCTION(BlueprintCallable, Category = UUC_Arena_DeckBtn)
	void UpdateBtnState(UWidgetSwitcher* state_switcher, UImage* ClearImg = nullptr);

	UFUNCTION(BlueprintCallable, Category = UUC_Arena_DeckBtn)
	void SetSelected();

	UFUNCTION(BlueprintCallable, Category = UUC_Arena_DeckBtn)
	bool IsEmptyDeck() { return m_bEmptyDeck; }

	// event 
	UFUNCTION()
	void BP_InitArenaDeckBtn();

	UFUNCTION()
	void BP_SetHeroIcon();

	UFUNCTION()
	void BP_ShowDeckBtn();

	UFUNCTION()
	void BP_HideDeckBtn();

	UFUNCTION(BlueprintCallable, Category = UUC_Arena_DeckBtn)
	void OnClickDeckSelectButton();

	// 
	bool IsMyHero() { return m_bMyHero; }
	bool IsSameHero(const FString& heroUD);
	bool IsSameHeroUD(const FString& heroUD);
	void SetHeroIconInfo(FHERO_ICON_INFO& icon_info);
	FORCEINLINE FHERO_ICON_INFO& GetHeroIconInfo() { return m_HeroIconInfo; };
	void SetTargetHeroIconInfo(FBATTLE_USER& MatchList, FHERO_ICON_INFO& icon_info, FString& HeroUD);
	int32 GetHeroPower() { return m_HeroPower; }
	bool GetMyDeckHeroUD(FString& heroUD);
	void DelHeroIconInfo();
	void PlayAnimPickAvailable();
	void StopAnimPickAvailable();
	bool IsPlayingAnimPickAvailable();


private:
// 	UPROPERTY()
// 	TWeakObjectPtr<UUP_Arena_DeckSetting>	m_wpArenaDeckSetting;

	FDeckManager_Blackboard* BlackBoard;
	UPROPERTY()
	int32					m_nDeckPosition = INDEX_NONE;

	UPROPERTY()
	bool					m_bMyHero = true;

	UPROPERTY()
	bool					m_bShowSelectedMark = false;

	UPROPERTY()
	bool					m_bEmptyDeck = true;

	UPROPERTY()
	FHERO_ICON_INFO			m_HeroIconInfo;

	UPROPERTY()
	int32					m_HeroPower = 0;

	UPROPERTY()
	UButton*				Select_DeckBtn;

	UPROPERTY()
		URenewal_BaseHeroIcon* Icon_Hero;

	UPROPERTY()
	UWidgetSwitcher*		ButtonSwitcher;

	UPROPERTY()
	UImage*					Icon_Clear;

	UPROPERTY()
	UScaleBox*				Button_ScaleBox;

	UPROPERTY()
	UImage*					NotiImage;
	UPROPERTY()
		UImage*					Selecting;

};
