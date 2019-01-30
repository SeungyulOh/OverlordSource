// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Object.h"
//#include "Runtime/Online/HTTP/Public/Http.h"
#include "ClientProtocols.h"
#include "HttpRequestObject.h"
#include "IHttpRequest.h"
#include "UpdateService/Auth/RAuth.h"
#include "HttpRequestClient.generated.h"


/*
Delegates
*/

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FDelegate_OnHttpRp);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FDelegate_OnHttpRp_PacketResult, bool, PacketResult);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FDelegate_OnHttpRp_int32, int32, Int);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FDelegate_OnHttpRp_HeroSkillLevelUp);
//
DECLARE_MULTICAST_DELEGATE_OneParam(FDelegate_OnHttpRp_InventoryUpdate, bool /* bSuccess */);
// Item
DECLARE_MULTICAST_DELEGATE_OneParam(FDelegate_OnHttpRp_InventoryHeroEquip, bool /* bSuccess */);
DECLARE_MULTICAST_DELEGATE_OneParam(FDelegate_OnHttpRp_InventoryHeroUnequip, bool /* bSuccess */);
DECLARE_MULTICAST_DELEGATE_TwoParams(FDelegate_OnHttpRp_InventoryItemGrind, bool /* bSuccess */, TArray<FREWARD> /* rewards */);
DECLARE_MULTICAST_DELEGATE_TwoParams(FDelegate_OnHttpRp_InventoryItemEnchant, bool /* bSuccess */, int32 /* affectedEnchant */);
DECLARE_MULTICAST_DELEGATE_TwoParams(FDelegate_OnHttpRp_InventoryItemLevelUp, bool /* bSuccess */, bool /* isLucky */);
DECLARE_MULTICAST_DELEGATE_TwoParams(FDelegate_OnHttpRp_InventorySell, bool /* bSuccess */, TArray<FREWARD> /* rewards */);
DECLARE_MULTICAST_DELEGATE_ThreeParams(FDelegate_OnHttpRp_InventoryItemUpgrade, bool /* bSuccess */, TArray<FREWARD> /* rewards */, FString /* NewItemUD */);
DECLARE_MULTICAST_DELEGATE_OneParam(FDelegate_OnHttpRp_InventoryItemOptionReset, bool /* bSuccess */);
DECLARE_MULTICAST_DELEGATE_OneParam(FDelegate_OnHttpRp_InventoryUpgrade, bool /* bSuccess */);
DECLARE_MULTICAST_DELEGATE_OneParam(FDelegate_OnHttpRp_HeroInventoryUpgrade, bool /* bSuccess */);

DECLARE_MULTICAST_DELEGATE_OneParam(FDelegate_OnHttpRp_InventoryRuneMake, bool /* bSuccess */);
DECLARE_MULTICAST_DELEGATE_OneParam(FDelegate_OnHttpRp_InventoryRuneEquip, bool /* bSuccess */);
DECLARE_MULTICAST_DELEGATE_OneParam(FDelegate_OnHttpRp_InventoryRuneUnequip, bool /* bSuccess */);

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FDelegate_OnHttRp_GuildJoin, int32, GuildID, bool, AutoGradeUp);


#define JSONTYPE		"application/json"
#define STREAMTYPE		"application/octet-stream"

#define USE_COMPRESS_PACKET

// Haven't figured out how to use keep-alive on iOS/Mac
// Only enabling in on Editor and Android for now
#if PLATFORM_WINDOWS
#define USE_KEEP_ALIVE
#endif

#if PLATFORM_ANDROID
#define USE_KEEP_ALIVE
#endif

#if PLATFORM_MAC
#define USE_KEEP_ALIVE
#endif


class URGameInstance;
/**
 * 
 */
UCLASS()
class PROJECTR_API UHttpRequestClient : public UObject
{
	GENERATED_BODY()


public:
	
	void Initialize(void);
	UFUNCTION()
	void RefreshHost(EPreloginResult Result);
	void SetTargetHost(const FString& InTargetHost);

	UFUNCTION()
	void OnRAuthReceiveClientKey(bool IsSuccess);

	void TransmitRequest(FHttpRequestObject& Request);
	void TransmitRequest(FString& RequestJsonString );
	void StartTransmission();

	void TransmitRequestBinary(FString AddString, TArray<uint8>& ar);

	void OnResponseReceived(FHttpRequestPtr Request, FHttpResponsePtr Response, bool WasSucessful);
	void OnProgressBinary(FHttpRequestPtr Request, int32 BytesSent, int32 BytesReceived);

	void Closed(const FString& Message);
	void ClosedBinary(const FString& Url, const TArray<uint8>& ArData);
	void ClosedPostProcess();
	void ReplyPacketBroadcast(int32 PacketType, bool PacketLogicalResult, const FString& Message);

	void	SetKID(int32 id);
	int32	GetKID();
	bool	IsValidKID(int32 id);
	bool	IsResponseSuccess(ECRUD is);
	

	void	CompressPacket(FString InPacket, TArray<uint8>& OutCompressedData);
	void	DecompressPacket(const TArray<uint8>& InCompressedData, FString& OutData);

	UPROPERTY(EditAnyWhere, BlueprintReadOnly, Category = HttpRequestClient)
	int32					kId = 0;

	UFUNCTION(BlueprintCallable, Category = HttpRequestClient)
	bool					IsValidNetwork();


	void					SetBusy(bool bBusy);
	bool					GetBusy();

	// Cached from RAuth for RQ functions
	FString					clientkey;
	FString					deviceid;

private:	
	
	FString					TargetHost;
	bool					IsBusy;
	FString					MessageBody = "";

	//URGameInstance*						RGameInstance = nullptr;

	TArray< FHttpRequestObject >		RequestQueue;
	FHttpRequestObject					CurrentRequest;

	// store for keep-alive
	TSharedPtr< IHttpRequest >	HttpRequest;

public:
	/*
	Delegates
	*/

	// Account
	UPROPERTY(BlueprintAssignable, BlueprintCallable, Category = HttpRequestClient)
	FDelegate_OnHttpRp_PacketResult		OnLoginRpDelegate;
	UPROPERTY(BlueprintAssignable, BlueprintCallable, Category = HttpRequestClient)
	FDelegate_OnHttpRp_PacketResult		OnAccountCreateRpDelegate;

	UPROPERTY(BlueprintAssignable, BlueprintCallable, Category = HttpRequestClient)
	FDelegate_OnHttpRp					OnAvatarGameDataRpDelegate;
	
	UPROPERTY(BlueprintAssignable, BlueprintCallable, Category = HttpRequestClient)
	FDelegate_OnHttpRp					OnEndIntroMovieDelegate;
	// Hero
	//UPROPERTY(BlueprintAssignable, BlueprintCallable, Category = HttpRequestClient)
	//FDelegate_OnHttpRp_HeroSkillLevelUp		OnHeroSkillLevelUpRpDelegate;
	//
	FDelegate_OnHttpRp_InventoryUpdate		OnInventoryUpdate;
	// Item
	FDelegate_OnHttpRp_InventoryHeroEquip	OnInventoryHeroEquip;
	FDelegate_OnHttpRp_InventoryHeroUnequip	OnInventoryHeroUnequip;

	FDelegate_OnHttpRp_InventoryItemGrind OnInventoryItemGrind;
	FDelegate_OnHttpRp_InventoryItemEnchant OnInventoryItemEnchant;
	FDelegate_OnHttpRp_InventoryItemLevelUp OnInventoryItemLevelUp;
	FDelegate_OnHttpRp_InventorySell OnInventorySell;
	FDelegate_OnHttpRp_InventoryItemUpgrade OnInventoryItemUpgrade;
	FDelegate_OnHttpRp_InventoryItemOptionReset OnInventoryItemOptionReset;
	FDelegate_OnHttpRp_InventoryUpgrade OnInventoryUpgrade; // BagExtend
	FDelegate_OnHttpRp_HeroInventoryUpgrade OnHeroInventoryUpgrade; //hero inven extend
	FDelegate_OnHttpRp_InventoryRuneMake OnInventoryRuneMake;
	FDelegate_OnHttpRp_InventoryRuneEquip OnInventoryRuneEquip;
	FDelegate_OnHttpRp_InventoryRuneUnequip OnInventoryRuneUnequip;

	// Play
// 	UPROPERTY(BlueprintAssignable, BlueprintCallable, Category = HttpRequestClient)
// 	FDelegate_OnHttpRp			OnPlayModeCampRpDelegate;
// 	UPROPERTY(BlueprintAssignable, BlueprintCallable, Category = HttpRequestClient)
// 	FDelegate_OnHttpRp			OnPlayModeCampDoneRpDelegate;
	UPROPERTY(BlueprintAssignable, BlueprintCallable, Category = HttpRequestClient)
	FDelegate_OnHttpRp_PacketResult			OnPlayModeSweepRpDelegate;
	UPROPERTY(BlueprintAssignable, BlueprintCallable, Category = HttpRequestClient)
	FDelegate_OnHttpRp			OnPlayModeRaidRpDelegate;
	UPROPERTY(BlueprintAssignable, BlueprintCallable, Category = HttpRequestClient)
	FDelegate_OnHttpRp			OnPlayModeRaidDoneRpDelegate;

	// Arena
// 	UPROPERTY(BlueprintAssignable, BlueprintCallable, Category = HttpRequestClient)
// 	FDelegate_OnHttpRp			OnArenaGetMatchesInfoRpDelegate;
// 	UPROPERTY(BlueprintAssignable, BlueprintCallable, Category = HttpRequestClient)
// 	FDelegate_OnHttpRp			OnArenaSeasonDoneRpDelegate;
// 	UPROPERTY(BlueprintAssignable, BlueprintCallable, Category = HttpRequestClient)
// 	FDelegate_OnHttpRp			OnPlayModeArenaRpDelegate;
// 	UPROPERTY(BlueprintAssignable, BlueprintCallable, Category = HttpRequestClient)
// 	FDelegate_OnHttpRp			OnPlayModeArenaDoneRpDelegate;
// 	UPROPERTY(BlueprintAssignable, BlueprintCallable, Category = HttpRequestClient)
// 	FDelegate_OnHttpRp			OnArenaGetRankingListRpDelegate;
// 	UPROPERTY(BlueprintAssignable, BlueprintCallable, Category = HttpRequestClient)
// 	FDelegate_OnHttpRp			OnArenaGetHistoryRpDelegate;

	// Colosseum
// 	UPROPERTY(BlueprintAssignable, BlueprintCallable, Category = HttpRequestClient)
// 	FDelegate_OnHttpRp			OnColosseumLobbyInfoRpDelegate;
// 	UPROPERTY(BlueprintAssignable, BlueprintCallable, Category = HttpRequestClient)
// 	FDelegate_OnHttpRp			OnColosseumHistoryRpDelegate;
// 	UPROPERTY(BlueprintAssignable, BlueprintCallable, Category = HttpRequestClient)
// 	FDelegate_OnHttpRp_PacketResult		OnColosseumSeasonDoneRpDelegate;
// 	UPROPERTY(BlueprintAssignable, BlueprintCallable, Category = HttpRequestClient)
// 	FDelegate_OnHttpRp			OnSetColosseumTopRankRpDelegate;
	
	// Camp
// 	UPROPERTY(BlueprintAssignable, BlueprintCallable, Category = HttpRequestClient)
// 	FDelegate_OnHttpRp			OnCampStatusRpDelegate;

	// Guild
	UPROPERTY(BlueprintAssignable, BlueprintCallable, Category = HttpRequestClient)
	FDelegate_OnHttpRp			OnGuildCreateRpDelegate;
	UPROPERTY(BlueprintAssignable, BlueprintCallable, Category = HttpRequestClient)
	FDelegate_OnHttpRp			OnGuildFindNameRpDelegate;
	
	UPROPERTY(BlueprintAssignable, BlueprintCallable, Category = HttpRequestClient)
	FDelegate_OnHttpRp_int32	OnGuildAwaiterCancelRpDelegate;
	UPROPERTY(BlueprintAssignable, BlueprintCallable, Category = HttpRequestClient)
	FDelegate_OnHttRp_GuildJoin	OnGuildJoinRpDelegate;
	
	UPROPERTY(BlueprintAssignable, BlueprintCallable, Category = HttpRequestClient)
	FDelegate_OnHttpRp_PacketResult		OnGuildContributeRankingRpDelegate;
	UPROPERTY(BlueprintAssignable, BlueprintCallable, Category = HttpRequestClient)
	FDelegate_OnHttpRp_PacketResult		OnGuildAwaiterListRpDelegate;

	UPROPERTY(BlueprintAssignable, BlueprintCallable, Category = HttpRequestClient)
	FDelegate_OnHttpRp			OnGuildHistoryRpDelegate;
	UPROPERTY(BlueprintAssignable, BlueprintCallable, Category = HttpRequestClient)
	FDelegate_OnHttpRp			OnGuildRuneStatusRpDelegate;

	// BattleField
	FDelegate_OnHttpRp			OnChannelInfoListRpDelegate;
	FDelegate_OnHttpRp			OnChannelDetailRpDelegate;

	// Content Reset
	FDelegate_OnHttpRp_PacketResult			OnContentResetRpDelegate;
	FDelegate_OnHttpRp_int32				OnContentResetStatusRpDelegate;
};