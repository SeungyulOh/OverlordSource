// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "UObject/NoExportTypes.h"
#include "Http.h"
#include "Json.h"
#include "SharedConstants/GlobalConstants.h"
#include "Network/ClientProtocols.h"
#include "PublisherAPIInterface.h"
#include "UpdateService/LoginDefs.h"
#include "UpdateService/Struct/OnlineAccountInfo.h"
#include "RAuth.generated.h"

#define API_NZIN 0

USTRUCT()
struct PROJECTR_API FReleaseTarget
{
	GENERATED_USTRUCT_BODY()

public:
	FReleaseTarget() {}
	~FReleaseTarget() {}

	FString Env;
	FString ServerHost;
	FString Cdn;
	FString TcpServerHost;
	FString TcpServerPort;

	// Patch detail
	FString ManifestBuildNumber;
};


UENUM(BlueprintType)
enum class EPreloginResult : uint8
{
	PreloginSuccess,
	PreloginFailure,
	PreloginLowVersion // requiring app store update
};

UENUM(BlueprintType)
enum class ELoginResult: uint8
{
	VE_Ok = 0,
	VE_InitClientKey			,//= 1100,	// misc errors on INIT_CLIENT_KEY
	VE_ExpiredClientKey			,//= 1101,	// 
	VE_InvalidPublisherName		,//= 1102,	// 
	VE_InvalidPublisher			,//= 1103,	// 
	VE_InvalidPublisherCode		,//= 1104,	// 
	VE_AccountNotFound			,//= 1105,	// ClientKey can be reused!
	VE_AccountSequenceError		,//= 1106,	// misc errors on LOGIN
	VE_NickAlreadyExists		,//= 1107,	// 
	VE_AccountAlreadyHave		,//= 1108,	// 
	VE_DoesNotHaveAvatar		,//= 1109,	// 
	VE_BlockedAccount			,//= 1110,	//
	VE_EmptyNick				,//= 1111,	//
	VE_AreadyHave				,//= 1112,	//
	VE_PublisherIdAlreadyUsed	,//= 1113,	//
	VE_AccountCreateNickInvalid	,//= 1114,	// This nickname can not be created.
	VE_SystemMaintenanced		,//= 1115,	// The server is under inspection.
	VE_ApiMaintenance			,//= 1116,	// This content is temporarily checked.


	//VE_InvalidProcotol,	// PDU.ver mismatch. client binary update required
	//VE_InvalidVersion,	// Client version not registered to the server versioncontrol. Tell Jae.
	//VE_LoginFail					// misc errors on LOGIN
};

UENUM(BlueprintType)
enum class ECreateAccountResult: uint8
{
	VE_Ok,
	
	VE_UnknownError,			// misc errors on INIT_CLIENT_KEY
	VE_NickAlreadyExists,			// ClientKey can be reused!
	VE_NickInvalid
};


// Transport listen to this to refresh server host
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FRAuthOnPreloginComplete, EPreloginResult, Result);

DECLARE_DYNAMIC_MULTICAST_DELEGATE_FourParams(FRAuthOnIdpLoginComplete, bool, bSuccess, FString, IdpCode, FString, ClientId, FString, AccessToken);

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FRAuthOnLoginComplete, ELoginResult, LoginResult, FString, SavedClientKey);

// Transport (HttpRequestClient) listens to this to renew clientkey
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FRAuthOnReceiveClientKey, bool, IsSuccess);

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FRAuthOnCreateAccountRp, ECreateAccountResult, Result);

// TODO: Clean up
//DECLARE_DYNAMIC_MULTICAST_DELEGATE(FRAuthOnGetNickName);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FRAuthOnShowCustomLoginButton);

// There also should be a hook for logout

class UUP_Popup;
/**

 * 
 */
UCLASS()
class PROJECTR_API URAuth : public UObject
{
	GENERATED_BODY()

public:
	URAuth();

	/**
	 * See the KMS Doc 환경 구성.VCS 관리 기능 정리
	 * the LIVE server is supposed to be used for Prelogin but currently it's DEV server
	 */ 
	static const FString PreloginServerHost;

	UFUNCTION(BlueprintCallable, category = Auth)
	static URAuth* GetInstancePtr();

	static void ReleaseSingleton();

	UFUNCTION(BlueprintCallable, category = Auth)
	void Init();

	
	//===================================================================================
	//
	// prelogin
	//
	//===================================================================================


	UFUNCTION(BlueprintCallable, category = Auth)
	void Prelogin();
	void OnGetTargetServerRp(bool bSuccess, FString ErrorCode, const FReleaseTarget* InReleaseTarget);

	UFUNCTION()
	void AvatarGameDataRp();
	void OverrideEnv(EReleaseEnv InReleaseTarget);

	UPROPERTY(BlueprintAssignable, category = Auth)
	FRAuthOnPreloginComplete OnPreloginComplete;

	// Release info for UI indication
	UFUNCTION(BlueprintPure, category = Auth)
	FString GetReleaseTargetEnv();

	// UE4 Game BuildConfig
	UFUNCTION(BlueprintPure, category = Auth)
	FString GetBuildConfig();

	// Adjust for Editor GameEnvironmentVar Override
	UFUNCTION(BlueprintPure, category = Auth)
	FString GetBuildVersion() { return BuildVersion; };

	FReleaseTarget ReleaseTarget;

	//===================================================================================
	//
	// Login process
	// Init_client_key -> Login -> create account
	//
	//===================================================================================

	/*
	 * IDP
	 */

	static IPublisherAPI* GetPublisherAPI();

	FString& GetUserID() { return AccountInfo.Id; }
	void SetUserID(FString& InValue) { AccountInfo.Id = InValue; }
	bool IsValidUserId() { return !(AccountInfo.Id.IsEmpty()); }

	void SetNickName(FString& InValue);
	void EmptyNickName();

	// load and save
	void LoadAccountLocalRecord();
	void SaveAccountLocalRecord();

	/*
	Login
	*/
	// Login to game server
	void	InitClientKey();
	void	Login(FString InPublisher, FString InClient_id, FString InAccess_token);
	void	LoginWithCachedClientkey();
	void	LoginWithCachedClientkey(FString InClientKey);

	void	PreaprePlatform(EPublisher InPublisher);
	void	ProcessLoginPlatform(int32 ControllerIndex, bool bWasSuccessful, const FUniqueNetId& UserId, const FString& ErrorString);

	UFUNCTION()
	void	LoginCompletePlatform();
	UFUNCTION()
	void	LoginFailPlatform();
	


	// Request create account
	void	RequestCreateAccount(FString InNickName, int32 InLanguage);

	UFUNCTION()
	void	OnGetUnreadMessageCount(int32 Result, int32 Count);

	/*
	Getter/Setter
	*/
	FString	GetClientKey();
	void	SetClientKey(FString& InClientKey);
	bool	IsValidClientKey();

	FString GetDeviceId() const;
	
	void	InvalidateClientKey();
	// Temp measure in order to not store keys from dev login
	// Normally should be stored at SetClientKey
	void	StoreClientKey();


	/*
	callback
	*/

	UPROPERTY(BlueprintAssignable, BlueprintCallable, category = Auth)
	FRAuthOnLoginComplete OnLoginComplete;

	UPROPERTY(BlueprintAssignable, BlueprintCallable, category = Auth)
	FRAuthOnReceiveClientKey OnReceiveClientKey; // TODO delete this

	UPROPERTY(BlueprintAssignable, BlueprintCallable, category = Auth)
	FRAuthOnCreateAccountRp OnCreateAccountRp;

	UPROPERTY(BlueprintAssignable, BlueprintCallable, category = Auth)
	FRAuthOnShowCustomLoginButton OnShowCustomLoginButton;

	// Delegate to ReceivePacketHandler
	bool OnINIT_CLIENT_KEY_RP(FINIT_CLIENT_KEY_RP& init_client_key_rp);
	bool OnLOGIN_RP(FLOGIN_RP& login_rp);
	bool OnACCOUNT_CREATE_RP(FACCOUNT_CREATE_RP& account_create_rp);

	bool GetIsDataState()						{ return IsDataState; }
	void SetIsDataState(bool InValue)			{ IsDataState = InValue; }

	EPublisher GetPublisher()					{ return Publisher; }
	void SetPublisher(EPublisher InPublisher)	{ Publisher = InPublisher; }

	FOnlineAccountInfo& GetAccountInfo()		{ return AccountInfo; }
	void SetAccountInfo(FOnlineAccountInfo& InAccountInfo);
	void MakeAccount();
	void EmptyAccount();
	ELoginProcess IsAccountState();


	void InvalidateIdpCredentials();

	void Logout();
	
	bool GetIsCheckAccount()					{ return IsCheckAccount; }
	void SetIsCheckAccount(bool InValue)		{ IsCheckAccount = InValue; }

private:
	void FinishPrelogin(EPreloginResult Result);
	// Wraps LOGIN_RQ to IdpProvider->GetUnrealMessageCount
	void	LoginToGameServer();

	FString ConversionPublisherToServer();
	FString GetPublisherName();
	void SettingPublisher(FString InString);

	//FText FormatErrorMessage(const FERROR_INFO& error_info);

	/*
	Release info
	*/

	//void LoadReleaseInfo();
	//void LoadReleaseTarget();

	//void OnReleaseInfoRetrieveComplete(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bSucceeded);

	//void OnReleaseTargetRetrieveComplete(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bSucceeded);

	// Populate ReleaseInfo
	//bool ReadReleaseTarget(const FString& ResponseString);
	

private:
	bool IsPreloggedIn = false;
	bool IsDataState = false;
	bool IsCheckAccount = false;
	EPublisher Publisher = EPublisher::Kokomo;

	UPROPERTY()
	FOnlineAccountInfo	AccountInfo;

	/*
	Login
	*/
	static URAuth* InstancePtr;

	// saved
	FString BuildVersion;
};
