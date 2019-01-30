// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Kismet/BlueprintFunctionLibrary.h"
#include "UI/UIDefs.h"
#include "Table/ItemTableInfo.h"
#include "Table/ItemLevelUpTableInfo.h"
#include "Table/LevelAppliedTables/WorkedSkillTableInfo.h"
#include "Table/CurrencyPropertyTableInfo.h"
#include "Network/PacketDataStructures.h"
#include "UIFunctionLibrary.generated.h"

class URGameInstance;
class UCharacterBattleInfo;
class UUP_CommonPopup;
class UUP_RewardPopup;
class URenewal_BaseHeroIcon;
class URInGameHUD;
enum class EBattleRepeatPopupState : uint8;
class UUC_Hero_Filter_Check;

class UUP_Popup_Avatar;
/**
 * 
 */
UCLASS()
class PROJECTR_API UUIFunctionLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()
	
public:

	//////////////////////////////////////////////////////////////////////////
	// Hero
	//////////////////////////////////////////////////////////////////////////
	UFUNCTION(BlueprintCallable, Category = UIFunctionLibrary)
	static void GetHeroIconInfo(FHERO_ICON_INFO& HeroIconInfo, int32 KID, const FString& HeroUD);

	UFUNCTION(BlueprintCallable, Category = UIFunctionLibrary)
	static void GetHeroIconInfoWithUD(FHERO_ICON_INFO& HeroIconInfo, const FString& HeroUD, EHeroOwnershipType OwnerShip);

	UFUNCTION(BlueprintCallable, Category = UIFunctionLibrary)
	static void GetHeroIconInfoWithID(FHERO_ICON_INFO& HeroIconInfo, const FName& HeroID, EHeroOwnershipType OwnerShip, int32 kId = 0);

	UFUNCTION(BlueprintCallable, Category = UIFunctionLibrary)
	static void GetHeroIconInfoForNpc(FHERO_ICON_INFO& HeroIconInfo, const FName& NpcID);

	UFUNCTION(BlueprintCallable, Category = UIFunctionLibrary)
	static UObject* GetCharacterPortrait(ECharacterCategoryTypeEnum CharacterCategory, FName RowName);

	UFUNCTION(BlueprintCallable, Category = UIFunctionLibrary)
	static UObject* GetHeroRoundPortrait(FName HeroID);

	UFUNCTION(BlueprintCallable, Category = UIFunctionLibrary)
	static UObject* GetIconImageFromTable(FName RowName);

	UFUNCTION(BlueprintCallable, Category = UIFunctionLibrary)
	static UObject* GetHeroRhombusPortrait(FName HeroID); // 마름모 초상화

	UFUNCTION(BlueprintCallable, Category = UIFunctionLibrary)
	static UPaperSprite* GetCharacterPortraitSprite(ECharacterCategoryTypeEnum CharacterCategory, FName RowName);

	UFUNCTION(BlueprintCallable, Category = UIFunctionLibrary)
	static bool SetImageBrush( UObject* InBrushResource, UImage* InImage );

	UFUNCTION(BlueprintCallable, Category = UIFunctionLibrary)
	static bool SetImageBrushFromTable(FName RowName, UImage* InImage);

	UFUNCTION(BlueprintCallable, Category = UIFunctionLibrary)
	static void GetHeroIconInfoByEquipedHero(FHERO_ICON_INFO& HeroIconInfo, FEQUIPED_HERO EquipedHero);

	static void ClearHeroIconIfo(FHERO_ICON_INFO& HeroIconInfo);

	UFUNCTION(BlueprintCallable, Category = UIFunctionLibrary)
	static FText GetCharacterName( AActor* InActor );

	static FText GetCharacterName(ECharacterCategoryTypeEnum CategoryType, FName CharacterID);

	UFUNCTION(BlueprintCallable, Category = UIFunctionLibrary)
	static FText GetNpcName(FName InNpcID);
	
	UFUNCTION(BlueprintCallable, Category = UIFunctionLibrary)
	static bool IsBossNpc(FName InNpcID);

	static bool IsOwnedTargetHeroByHeroUD(UWorld* InWorld, const FString& InHeroUD);
	
	static void SetFilterCheckCondition(UUC_Hero_Filter_Check* checkFilter);

	//////////////////////////////////////////////////////////////////////////
	// Item
	//////////////////////////////////////////////////////////////////////////
	UFUNCTION(BlueprintCallable, Category = UIFunctionLibrary)
	static bool GetItemIconInfoWithUD(FITEM_ICON_INFO& ItemIconInfo, const FString& ItemUD);

	UFUNCTION(BlueprintCallable, Category = UIFunctionLibrary)
	static bool GetItemPreviewIconInfoWithID(FITEM_ICON_INFO& ItemIconInfo, const FName& ItemID);

	UFUNCTION(BlueprintCallable, Category = UIFunctionLibrary)
	static bool GetItemIconInfoWithID(FITEM_ICON_INFO& ItemIconInfo, const FName& ItemID);

	static void ItemIconInfoSetServerData(FITEM_ICON_INFO& ItemIconInfo, FITEM& ItemData);
	static void ItemIconInfoSetTableData(FITEM_ICON_INFO& ItemIconInfo, FItemTableInfo& ItemTableInfo);

	static void ClearItemIconIfo(FITEM_ICON_INFO& ItemIconInfo);
	static int32 GetItemEnchantRowName(EItemEquipTypeEnum InEquipType, EItemEquipPositionEnum InEquipPoisition, EItemGradeEnum InGrade, int32 InEnchantLevel);
	
	// Expect lock image sprite for EItemEquipPositionEnum::VE_Max
	static UObject* GetItemEmptyTextureByEquipPosition(EItemEquipPositionEnum InItemEquipPosition);

	// TODO: remove. legacy.
	static FName GetOptionName(FString& InValue);

	// 오버로드_장비아이템_변경안.docx.1.(2).B.ii 장비별 장착 타입
	static bool IsEquipTypeArmor(EItemEquipTypeEnum EquipType);

	static FName GetItemGradeName(EItemGradeEnum ItemGrade);

	static FName ItemStatOptionToStringKey(EItemStatOption ItemStatOption);

	//////////////////////////////////////////////////////////////////////////
	// Text
	//////////////////////////////////////////////////////////////////////////
	UFUNCTION(BlueprintCallable, Category = UIFunctionLibrary)
	static FText GetSkillDescTextHeroID(FName InHeroID, FName InSkillName, int32 InLevel, bool bIsCondition = false, bool bIsRich = true);

	static FText GetItemPassiveSkillDescText(FName InSkillName);

	UFUNCTION(BlueprintCallable, Category = UIFunctionLibrary)
	static FText GetSkillDescText(const FWorkedSkillTableInfo& InWorkedSkillInfo, bool bIsCondition = false, bool bIsRich = true);

	UFUNCTION(BlueprintCallable, Category = UIFunctionLibrary)
	static FName GetDayOfWeekStringID( EDayOfWeekTypeEnum InDayOfWeek );

	UFUNCTION(BlueprintCallable, Category = UIFunctionLibrary)
	static FText GetSkillNameText(const FWorkedSkillTableInfo& InWorkedSkillInfo);

	UFUNCTION(BlueprintCallable, Category = UIFunctionLibrary)
	static FText GetSkillAttributeText(ESkillAttributeEnum InSkillAttribute);

	static FName GetSkillLevelUpKey(FString heroUD, int32 InLevel);
	static FItemLevelUpTableInfo* GetItemLevelUpTableInfo(FITEM_ICON_INFO& InTarget, int32 InLevel = 1);
	static FItemEnchantTableInfo* GetItemEnchantTableInfo(FITEM_ICON_INFO& InTarget);

	static bool TransformFTextToRichText(FText& InDescText);
	static bool ParseSkillTextParameter(const TArray<FString>& FindString, FText& InDescText, TSet<FString>& ParseMap);
	static bool RevisePluralUnit(FText& InDescText, FFormatNamedArguments& InFormatNamedArguments);
	

	//////////////////////////////////////////////////////////////////////////
	// UI Control
	//////////////////////////////////////////////////////////////////////////
	UFUNCTION(BlueprintCallable, meta = (HidePin = "WorldContextObject", DefaultToSelf = "WorldContextObject"), Category = UIFunctionLibrary)
	static void AttachHUDPage(UObject* WorldContextObject, bool InIsAttach = true);

	UFUNCTION(BlueprintCallable, meta = (HidePin = "WorldContextObject", DefaultToSelf = "WorldContextObject"), Category = UIFunctionLibrary)
	static EWidgetBluePrintEnum GetHUDType(UObject* WorldContextObject);

	UFUNCTION(BlueprintCallable, meta = (HidePin = "WorldContextObject", DefaultToSelf = "WorldContextObject"), Category = UIFunctionLibrary)
	static bool OnShowInGameHUD(UObject* WorldContextObject, bool bShow);

	UFUNCTION(BlueprintCallable, meta = (HidePin = "WorldContextObject", DefaultToSelf = "WorldContextObject"), Category = UIFunctionLibrary)
	static UUserWidget* GetInGameHUD();

	UFUNCTION(BlueprintCallable, meta = (HidePin = "WorldContextObject", DefaultToSelf = "WorldContextObject"), Category = UIFunctionLibrary)
	static URSkillSet_New* GetSkillSet(UObject* WorldContextObject);

	UFUNCTION(BlueprintCallable, meta = (HidePin = "WorldContextObject", DefaultToSelf = "WorldContextObject"), Category = UIFunctionLibrary)
	static class UUP_HeroScrollView_Bottom* GetHeroScrollView();

	UFUNCTION(BlueprintCallable, meta = (HidePin = "WorldContextObject", DefaultToSelf = "WorldContextObject"), Category = UIFunctionLibrary)
	static class UUP_GenericScrollview* GetGenericScrollview();

	
	UFUNCTION(BlueprintCallable, meta = (HidePin = "WorldContextObject", DefaultToSelf = "WorldContextObject"), Category = UIFunctionLibrary)
	static void CreateTouchInterface(UObject* WorldContextObject);

	UFUNCTION(BlueprintCallable, meta = (HidePin = "WorldContextObject", DefaultToSelf = "WorldContextObject"), Category = UIFunctionLibrary)
	static void RemoveTouchInterface(UObject* WorldContextObject);

	//////////////////////////////////////////////////////////////////////////
	// UI RewardIcon
	//////////////////////////////////////////////////////////////////////////
// 	static bool GetRewardsIconInfoByRewardKey(UWorld* InWorld, FName InRewardKey, TArray<FITEM_ICON_INFO>& RewardsItemIconInfo, int32 ExactRewardCount = 0, int32 FromTheFrontCount = 0);
	static bool GetRewardIconByRewardType(const FREWARD& InReward, FITEM_ICON_INFO& InRewardItemIconInfo);
	static bool GetRewardsIconInfoByPreview(FName InRewardPreviewKey, TArray<FITEM_ICON_INFO>& RewardsItemIconInfo, int32 ExactRewardCount = 0, int32 FromTheFrontCount = 0);
	// To parse DT_RewardPreview.CodeName1~12 which is the union of EREWARD_TYPE and ItemId
	static EREWARD_TYPE GetRewardType(FName InCodeName);
	static void GetItemIconInfoByWallet(FName InWalletID, int32 InAmt, FITEM_ICON_INFO& InItemIconInfo);
	//////////////////////////////////////////////////////////////////////////
	// UI RuneIcon
	//////////////////////////////////////////////////////////////////////////
	static bool GetRuneIconByID(UWorld* InWorld, FName InCharacterID, FITEM_ICON_INFO& InRuneItemIconInfo);


	//////////////////////////////////////////////////////////////////////////
	// Popup
	//////////////////////////////////////////////////////////////////////////

	UFUNCTION(BlueprintCallable, Category = UIFunctionLibrary)
	static UUP_CommonPopup*	ShowCommonPopup(FText InCaption, FText InContents, ECommonPopupType InPopupType);
	UFUNCTION(BlueprintCallable, Category = UIFunctionLibrary)
	static void HideCommonPopup();

	static void ShowWaitingPopup(bool bShow);

	
	static UUP_Popup_Avatar* ShowAvatarLevelUpPopup(bool bShow);

	//==================================================================================================
	// UC_GenericPopup helpers
	//==================================================================================================

	static class UUC_GenericPopup* ShowGenericPopupContent(EWidgetBluePrintEnum eWBP);
	static class UUP_GenericSplash* ShowGenericSplashContent(EWidgetBluePrintEnum eWBP, FText caption = FText());

	
// 	UFUNCTION(BlueprintCallable, meta = (HidePin = "WorldContextObject", DefaultToSelf = "WorldContextObject"), Category = UIFunctionLibrary)
// 	static UUP_CommonPopup*	ShowHeroPopup(UObject* WorldContextObject, FName InHeroID, FText InCaption, FText InContents);
// 	UFUNCTION(BlueprintCallable, meta = (HidePin = "WorldContextObject", DefaultToSelf = "WorldContextObject"), Category = UIFunctionLibrary)
// 	static void HideHeroPopup(UObject* WorldContextObject);

	
	static UUP_RewardPopup*	ShowRewardPopup();
	static class UUP_Popup_Reward_AfterGrind*	ShowRewardPopup_ScrollView(TArray<FREWARD> Rewards);
	static UUP_RewardPopup*	ShowCommonRewardPopupByRewardSet(TArray<FREWARDSET>& RewardSets);
	static UUP_RewardPopup*	ShowCommonRewardPopupByRewards(TArray<FREWARD>& Rewards, bool bFirst = false);
	static UUP_RewardPopup*	ShowQuestRewardsPopup(TArray<FREWARD>& Rewards, FText& InQuestText);

	static void HideRewardPopup();
	static void ShowRewardPopupByRewards(TArray<FREWARD>& InRewards);
	static void ShowRewardPopupByRewardsHero(TArray<FHERO>& InHeros, TArray<FREWARD>& InRewards);

	// splash popup
	static class UUP_CommonPopupSplash*	ShowCommonPopupSplash(FText InCaption, FText InDesc);
	static class UUP_CommonPopupSplash*	ShowCommonPopupHeroRuneSplash(FName InID, FText InCaption, FText InDesc, bool bLevelUp = false);
	static void					HideCommonPopupSplash();
	// Reward Popup Splash
	static class UUP_RewardPopupSplash*				ShowRewardPopupSplash(ERewardSplashType InType, FText& InOwnerName, FText& InItemName, TArray<FREWARD>& InRewardData);
	static class UUP_Popup_Splash_AttackPower*		ShowChangeAttackPowerSplash(FName InHeroID, int32 InCurrentPower, int32 InPrevPower);

	static void ShowAutoPlayPopup(EBattleRepeatPopupState InState);
	static void HideAutoPlayPopup();

	static bool					ShowMatchingWaiting(EPVP InConnecMode);
	static void					HideMatchingWaiting();

	static class UUP_ItemShortagePopup* ShowCurrencyShortagePopup(FText InCaption, FText InDesc, EREWARD_TYPE InCurrency, int32 InShortageValue);
	static void HideCurrencyShortagePopup();
	
	static void ShowQuestCompletePopup();
	static void HideQuestCompletePopup();
	
	static bool ShowHero2D();
	
	UFUNCTION(BlueprintCallable, Category = UIFunctionLibrary)
	static FText GetRewardText(FREWARD& InReward);

	UFUNCTION(BlueprintCallable, Category = UIFunctionLibrary)
	static FText GetToolTipText(EToolTipEventTypeEnum InToolTipType, FName InID);
	UFUNCTION(BlueprintCallable, Category = UIFunctionLibrary)
	static FText GetToolTipTypeText(EToolTipEventTypeEnum InToolTipType, FName InID);
	UFUNCTION(BlueprintCallable, Category = UIFunctionLibrary)
	static FText GetToolTipDescriptionText(EToolTipEventTypeEnum InToolTipType, FName InID);
	UFUNCTION(BlueprintCallable, Category = UIFunctionLibrary)
	static FText GetNpcInfoText(TArray<FName>& Tooltips);
	UFUNCTION(BlueprintCallable, Category = UIFunctionLibrary)
	static FText GetHeroInfoText(TArray<FName>& Tooltips);


	UFUNCTION(BlueprintCallable, Category = UIFunctionLibrary)
	static void ReadyToStartUI(FString phase = TEXT(""));
	static void ReadyToStartUI_Campaign(FName stage);
	static void ReadyToStartUI_PVP();
	

	//////////////////////////////////////////////////////////////////////////
	// Currency warning message
	//////////////////////////////////////////////////////////////////////////
	static FText GetCurrentyWarningMessage(FName InRewardType);


	//////////////////////////////////////////////////////////////////////////
	// Input
	//////////////////////////////////////////////////////////////////////////
	static void ActivateJoystick( UWorld* InWorld, bool InActive);
	static void InvalidateJoystick(UWorld* InWorld);	


	UFUNCTION(BlueprintCallable, Category = UIFunctionLibrary)
	static FVector2D GetWidgetPos(UUserWidget* InWidget);
	static FVector2D GetPanelWidgetPos(UWidget* InWidget);
	static FVector2D GetPanelWidgetSize(UWidget* InWidget);

	//////////////////////////////////////////////////////////////////////////
	// Contents
	//////////////////////////////////////////////////////////////////////////
	UFUNCTION(BlueprintCallable, Category = UIFunctionLibrary)
	static bool GoContents(EUIMenuIDEnum idtype, int32 idx = 0);

	UFUNCTION(BlueprintCallable, Category = UIFunctionLibrary)
	static void GoUIPage(EUIPageEnum UIPage);

	UFUNCTION(BlueprintCallable, Category = UIFunctionLibrary)
	static void ChangeUIPage(EUIPageEnum UIPage);
	//// helpers
	UFUNCTION(BlueprintCallable, Category = UIFunctionLibrary)
	static FName GetContentsName(EUIMenuIDEnum InMenuID);

	static EUIPageEnum ContentToPage(EUIMenuIDEnum Content);
	UFUNCTION()
	static void OnLobbyStateUpdated(EUIPageEnum UIPage);

	//////////////////////////////////////////////////////////////////////////
	// UI Skill Texture
	//////////////////////////////////////////////////////////////////////////
	UFUNCTION(BlueprintCallable, Category = UIFunctionLibrary)
	static UTexture2D* GetTextureSkillLevelCircle(int32 SkillLevel);

	UFUNCTION(BlueprintCallable, Category = UIFunctionLibrary)
	static UTexture2D* GetTextureSkillCircle(int32 SkillLevel);

	UFUNCTION(BlueprintCallable, Category = UIFunctionLibrary)
	static UTexture2D* GetTextureSkillAttribute(ESkillAttributeEnum InSkillAttribute);

	UFUNCTION(BlueprintCallable, Category = UIFunctionLibrary)
	static UTexture2D* GetTextureSkillShapeAttribute(const FWorkedSkillTableInfo&	CurrentWorkedSkillTableInfo);


	//////////////////////////////////////////////////////////////////////////
	// UserWidget
	//////////////////////////////////////////////////////////////////////////
	UFUNCTION(BlueprintCallable, Category = UIFunctionLibrary)
	static UUserWidget* CreateUserWidget(EWidgetBluePrintEnum WidgetBluePrint);


	/////////////////////////////////////////////////////////////////////////
	// ExpPotion
	//////////////////////////////////////////////////////////////////////////
	//static bool GetExpPotionInfo(UWorld* InWorld, const EREWARD_TYPE& InExpPotionType , FEXP_POTION_INFO& InExpPotionInfo);

	//////////////////////////////////////////////////////////////////////////
	// MediaFrameWork
	//////////////////////////////////////////////////////////////////////////
	UFUNCTION(BlueprintCallable, Category = UIFunctionLibrary)
	static bool ShowUIMediaFrameWork(FName MediaName, bool IsVisibleSkipButton = true);
	
	//////////////////////////////////////////////////////////////////////////
	// UIEvent
	//////////////////////////////////////////////////////////////////////////
	UFUNCTION(BlueprintCallable, Category = UIFunctionLibrary)
	static void	AddUIEvent(EUIEventEnum InEventType);

	UFUNCTION(BlueprintCallable, Category = UIFunctionLibrary)
	static void	AddUIEventInt(EUIEventEnum InEventType, int32 InValue);

	UFUNCTION(BlueprintCallable, Category = UIFunctionLibrary)
	static void	AddUIEventString(EUIEventEnum InEventType, FString InValue);

	UFUNCTION(BlueprintCallable, Category = UIFunctionLibrary)
	static void	AddUIEventName(EUIEventEnum InEventType, FName InValue);

	//////////////////////////////////////////////////////////////////////////
	// Currency
	//////////////////////////////////////////////////////////////////////////
	//static FCurrencyPropertyTableInfo* GetCurrencyTable(EREWARD_TYPE InRewardType);
	static bool IsEnableUsingCurrency(URGameInstance* InRGameInstance, FName InType, int32 consumeValue);
	static bool LackCurrencyPopup(URGameInstance* InRGameInstance, TArray<FString>& params);

	//////////////////////////////////////////////////////////////////////////
	// ViewPort
	//////////////////////////////////////////////////////////////////////////
	static FVector2D GetGameResolution();

	//////////////////////////////////////////////////////////////////////////
	// Topbar currency in shop
	//////////////////////////////////////////////////////////////////////////
	static void RefreshTopbarProperty_Simple(URGameInstance* InRGameInstance);
	static void RefreshTopbarProperty_InShop(URGameInstance* InRGameInstance, int32 InShopTabIndex);

	//////////////////////////////////////////////////////////////////////////
	// 0n number
	//////////////////////////////////////////////////////////////////////////
	static FText Get_0n_Number(int32 InNumber);

	static FName GetBlockAbnormalMsg(ESkillApplierTypeEnum InBlockApplierType);
};


