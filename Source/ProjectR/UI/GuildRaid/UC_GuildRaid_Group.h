// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "UI/RBaseWidget.h"
#include "Global/RealTimeModManager.h"
// #if WITH_PROUD_NET_BINDING
// #include "Network/ProudNetSerializer/ProtocolTypes.h"
// #endif
#include "UC_GuildRaid_Group.generated.h"

// class UUC_HeroManagement_HeroList;
// class UUC_GuildRaid_Party;
// class URealTimeModManager;
// 
// USTRUCT(BlueprintType)
// struct PROJECTR_API FPartyUpdateReqData
// {
// 	GENERATED_USTRUCT_BODY()
// public:
// 	UPROPERTY()
// 	bool InvalidData = false;
// 	Proud::PtReqPartyUpdate	PartySlotData;
// };

/**
 * 
 */
UCLASS()
class PROJECTR_API UUC_GuildRaid_Group : public URBaseWidget
{
	GENERATED_BODY()
	
public:
	virtual void			NativeConstruct()	override;
	virtual	void			NativeDestruct()	override;
// 
// 	void RefreshGroupUI();
// 	
// 	UFUNCTION()
// 	void SelectHero(FString InHeroUD);
// 	UFUNCTION()
// 	void OnPartyMemberRefresh(int32 InPartyIndex, int32 InSlotIndex, bool IsMyData);
// 	UFUNCTION()
// 	void OnPartyMemberClick(int32 InPartyIndex, int32 InSlotIndex, bool BtnSelect);
// 	
// 	UFUNCTION(BlueprintCallable, Category = UUC_GuildRaid_Group)
// 	void OnClick_PartyMove();
// 
// 	UFUNCTION(BlueprintCallable, Category = UUC_GuildRaid_Group)
// 	void OnClick_Delegation();
// 
// 	UFUNCTION(BlueprintCallable, Category = UUC_GuildRaid_Group)
// 	void OnClick_MasterAction(bool bExecute);
// 
// 	UFUNCTION(BlueprintCallable, Category = UUC_GuildRaid_Group)
// 	void OnClick_GameStart();
// 
 	void SetGroupStateType(EGroupStateType InType) { CurGroupState = InType; }
// 
// private:
// 	UUC_GuildRaid_Party* GetPartyUI(int32 InPartyIndex);
// 	void ClearPartyMember(FPartyUpdateReqData& PartyUpdateReqData);
// 	void SetPartyMember(FPartyUpdateReqData& PartyUpdateReqData, int32 InPartyIndex, int32 InSlotIndex);
// 
// public:
// 	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UUC_GuildRaid_Group)
// 	TArray<UUC_GuildRaid_Party*>	PartyList;
// 
// 	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UUC_GuildRaid_Group)
// 	UWidgetSwitcher*	Switcher_GroupStateButton = nullptr;
// 
// 	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UUC_GuildRaid_Group)
// 	UWidgetSwitcher*	Switcher_GroupStateDesc = nullptr;
// 
// 	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UUP_BattleGroundHeroSelect)
// 	UButton*				PartyMoveButton = nullptr;
// 
// 	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UUP_BattleGroundHeroSelect)
// 	UButton*				DelegationButton = nullptr;
// 
// 	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UUP_BattleGroundHeroSelect)
// 	UButton*				GameStartButton = nullptr;
// 
// 	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UUP_BattleGroundHeroSelect)
// 	UUC_HeroManagement_HeroList*	UC_HeroList = nullptr;
// 
// 
// public:
// 	int32		MyGroupID = INDEX_NONE;
// 	int32		MyPartyIndex = INDEX_NONE;
// 	int32		MySlotIndex = INDEX_NONE;
// 
// 	FString		SelectedMyHeroUD;
// 	bool		ReadyToStartGame = false;
// 
// 	UPROPERTY()
// 	TWeakObjectPtr< URealTimeModManager >	RealTimeModManagerPtr;
// 
// 	UPROPERTY()
// 	FPartyUpdateReqData SrcMember;
// 	UPROPERTY()
// 	FPartyUpdateReqData DestMember;
// 
// 	UPROPERTY()
// 	bool SpawnedMyHero = false;
// 
private:
 	EGroupStateType CurGroupState = EGroupStateType::VE_Member;
};
