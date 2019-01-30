// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "UI/RBaseWidget.h"
#include "UC_GuildRaid_PartyMember.generated.h"

//class URenewal_BaseHeroIcon;

/**
 * 
 */
UCLASS()
class PROJECTR_API UUC_GuildRaid_PartyMember : public URBaseWidget
{
	GENERATED_BODY()
	
public:
	virtual void			NativeConstruct()	override;
	virtual	void			NativeDestruct()	override;

// 	UFUNCTION(BlueprintCallable, Category = UUP_RealTime_Result)
// 	void OnClick_Member();
// 
// 	void SetGroupPartyData(int32 InGroupID, int32 InPartyIndex);
// 	void RefreshGroupPartyData(int32 InPartyIndex, int32 InSlotIndex);
// 	void ClickGroupPartyData(int32 InPartyIndex, int32 InSlotIndex, bool InSelectMember);
// 	void RefreshPartyMember();
// 
// public:
// 	UPROPERTY()
// 	int32	PartyIndex = INDEX_NONE;
// 	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UUC_GuildRaid_Party)
// 	int32	SlotIndex = INDEX_NONE;
// 
// 	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UUC_GuildRaid_Party)
// 	URenewal_BaseHeroIcon*	HeroIcon = nullptr;
// 
// 	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UUC_GuildRaid_Party)
// 	UWidgetSwitcher*		Switcher_MemberName = nullptr;
// 
// 	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UUC_GuildRaid_Party)
// 	TArray<UTextBlock*>		Text_MemberName;
// 
// 	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UUC_GuildRaid_Party)
// 	UImage*					Image_IsMine = nullptr;
// 
// 	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UUC_GuildRaid_Party)
// 	UWidgetSwitcher*		Switcher_MemberSelect = nullptr;
// 
// 	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UUC_GuildRaid_Party)
// 	UPanelWidget*			GuildLeader = nullptr;
// 
// private:
// 	UPROPERTY()
// 	FHERO_ICON_INFO			HeroIconInfo;
// 	UPROPERTY()
// 	bool					SelectMember = false;
};
