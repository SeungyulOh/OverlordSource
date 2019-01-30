// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "ProjectR.h"
#include "MobilePatchingLibrary.h"
#include "Http.h"
#include "UpdateService.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FUpdateServiceOnUpdatePending);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FUpdateServiceOnFinish, bool, IsSuccess, FString, Message);

/*
UpdateService states
*/
UENUM(BlueprintType)		//"BlueprintType" is essential to include
enum class EUpdateServiceState : uint8
{
	None,
	CheckingForUpdate,
	PendingUpdate,
	Installing,
	DownloadingMovies,
	Success,
	Error
};

/*
Patching progress state for UI
*/
UENUM(BlueprintType)		//"BlueprintType" is essential to include
enum class EUpdateServiceUIState: uint8
{
	None,
	CheckingForUpdate,
	Initializing,
	Resuming,
	Downloading,
	Installing,
	DownloadingMovies,
	Success,
	Error
};

struct FFileToDownload
{
public:
	FString Name;
	int32 Size;
	FString Sha1;

public:
	FFileToDownload(const FString& InName, int32 InSize, const FString& InSha1)
		: Name(InName)
		, Size(InSize)
		, Sha1(InSha1)
	{
	}
};

/**
 * 
 */
UCLASS(BlueprintType)
class PROJECTR_API UUpdateService : public UObject
{
	GENERATED_BODY()

public:
	UUpdateService();

	UFUNCTION(BlueprintCallable, category = UpdateService)
	static UUpdateService* GetInstancePtr();

	UFUNCTION(BlueprintCallable, category = UpdateService)
	static void ReleaseSingleton();

	UFUNCTION(BlueprintCallable, category = UpdateService)
	void Init();

	UFUNCTION(BlueprintCallable, category = UpdateService)
	void CheckUpdate();

	UFUNCTION(BlueprintCallable, category = UpdateService)
	void StartInstall();

	UFUNCTION()
	void OperationFinish(bool IsSuccess, FString Message);

	UFUNCTION(BlueprintCallable, category = UpdateService)
	EUpdateServiceUIState GetUIState();

	EUpdateServiceUIState GetInstallingState();

	/** Get the total downloaded size in megabytes. Valid during installation */
	UFUNCTION(BlueprintPure, Category = "Mobile Patching|Progress")
	float GetTotalDownloadedSize();

	/** Get the current download speed in megabytes per second. Valid during installation */
	UFUNCTION(BlueprintPure, Category = "Mobile Patching|Progress")
	float GetDownloadSpeed();

	/** Get the current installation status text. Valid during installation */
	UFUNCTION(BlueprintPure, Category = "Mobile Patching|Progress")
	FText GetDownloadStatusText();

	/** Get the current installation progress. Between 0 and 1 for known progress, or less than 0 for unknown progress */
	UFUNCTION(BlueprintPure, Category = "Mobile Patching|Progress")
	float GetInstallProgress();

	/** Get the current installation progress. Between 0 and 1 for known progress, or less than 0 for unknown progress */
	UFUNCTION(BlueprintPure, Category = "Mobile Patching|Progress")
	float GetDownloadSize();

	UPROPERTY(BlueprintAssignable, category = UpdateService)
	FUpdateServiceOnUpdatePending OnUpdatePending;
	UPROPERTY(BlueprintAssignable, category = UpdateService)
	FUpdateServiceOnFinish OnFinish;

	UFUNCTION(BlueprintPure, category = UpdateService)
	FString GetUpdateContentDir() const;

	// Content version
	UFUNCTION(BlueprintPure, category = UpdateService)
	FString GetInstalledContentVersion();

	UFUNCTION(BlueprintPure, category = UpdateService)
	FString GetPendingContentVersion();

	// Settled after update
	UFUNCTION(BlueprintPure, category = UpdateService)
	FString GetContentVersion();

	// Movies download status read by UI
	TArray<FFileToDownload> FilesToDownload;
	int32 IndexToFileDownloaded = 0;
	// Progress
	int32 FileDownloadedContentLength = 0;
	int32 FileDownloadedBytesReceived = 0;

	void SetNoPatchBuild();
	bool IsContentPresent();

private:

	static FString PlatformName();
	
	static FString GetCloudURL(const FString& Cdn);
	static FString GetManifestUrl(const FString& Cdn);
	static FString GetMoviesUrl(const FString& Cdn);
	
	UFUNCTION()
	void OnRequestContentSucceeded(UMobilePendingContent *MobilePendingContent);
	UFUNCTION()
	void OnRequestContentFailed(FText ErrorText, int32 ErrorCode);

	UFUNCTION()
	void OnContentInstallSucceeded();
	UFUNCTION()
	void OnContentInstallFailed(FText ErrorText, int32 ErrorCode);

	bool ShouldUpdate(const FString& InstalledVersion, const FString& RemoteVersion);

	UFUNCTION()
	void RequestContent();

	void CheckMovies();

	void OnDownloadManifestComplete(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bSucceeded);

	void DownloadOneFile();
	void OnDownloadOneFileComplete(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bSucceeded);
	void OnDownloadOneFileProgress(FHttpRequestPtr Request, int32 BytesSent, int32 BytesReceived);

	// Installation Status

	// Empty for no installed content
	FString ContentVersion;

	UPROPERTY()
	UMobileInstalledContent* InstalledContent = nullptr;
	UPROPERTY()
	UMobilePendingContent* PendingContent = nullptr;

	bool ShouldInstallPendingContent = false;

	EUpdateServiceState State = EUpdateServiceState::None;
	

	static UUpdateService* InstancePtr;
};
