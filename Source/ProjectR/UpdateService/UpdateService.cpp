// Fill out your copyright notice in the Description page of Project Settings.

#include "ProjectR.h"
#include "UpdateService/Auth/RAuth.h"
#include "UpdateService.h"
#include "Json.h"
#include "MobilePatchingLibrary.h"

/*
 * Currently only supposed to be instantiated on mobile
 */

#define PATCH_INSTALL_DIRECTORY TEXT("PatchContent")

UUpdateService *UUpdateService::InstancePtr = nullptr;

UUpdateService* UUpdateService::GetInstancePtr()
{
	if (InstancePtr == nullptr)
	{
		InstancePtr = NewObject< UUpdateService >((UObject*)GetTransientPackage(), UUpdateService::StaticClass());
		InstancePtr->Init();
		InstancePtr->AddToRoot();
	}
	
	return InstancePtr;
}

void UUpdateService::ReleaseSingleton()
{
	if (InstancePtr)
	{
		InstancePtr->RemoveFromRoot();
	}
	InstancePtr = nullptr;
}

void UUpdateService::Init()
{
	State = EUpdateServiceState::None;

	InstalledContent = UMobilePatchingLibrary::GetInstalledContent(PATCH_INSTALL_DIRECTORY);
	// log
	if (InstalledContent != nullptr)
	{
		UE_LOG(LogUpdateService, Log, TEXT("Found InstalledContent: %s"), *InstalledContent->InstalledManifest->GetVersionString());
	}
	else {
		UE_LOG(LogUpdateService, Log, TEXT("InstalledContent not found"));
	}
	ContentVersion = GetInstalledContentVersion();
}

void UUpdateService::CheckUpdate()
{
	if (State == EUpdateServiceState::None || State == EUpdateServiceState::Error)
	{
		RequestContent();
	}
	else
	{
		UE_LOG(LogUpdateService, Log, TEXT("CheckUpdate called while busy"));
	}
}

UUpdateService::UUpdateService()
{
	PendingContent = nullptr;
	InstalledContent = nullptr;
	State = EUpdateServiceState::None;
}

void UUpdateService::StartInstall()
{
	UE_LOG(LogUpdateService, Log, TEXT("StartInstall"));

	if (ShouldInstallPendingContent)
	{
		State = EUpdateServiceState::Installing;

		FOnContentInstallSucceeded OnContentInstallSucceeded;
		OnContentInstallSucceeded.BindDynamic(this, &UUpdateService::OnContentInstallSucceeded);
		FOnContentInstallFailed OnContentInstallFailed;
		OnContentInstallFailed.BindDynamic(this, &UUpdateService::OnContentInstallFailed);

		PendingContent->StartInstall(
			OnContentInstallSucceeded,
			OnContentInstallFailed);
	}
	else
	{
		DownloadOneFile();
	}
}

void UUpdateService::OperationFinish(bool IsSuccess, FString Message)
{
	State = (IsSuccess ? EUpdateServiceState::Success : EUpdateServiceState::Error);

	OnFinish.Broadcast(IsSuccess, Message);
	// InstalledContent = nullptr;
	PendingContent = nullptr;
}

EUpdateServiceUIState UUpdateService::GetUIState()
{
	EUpdateServiceUIState Result = EUpdateServiceUIState::None;

	switch (State)
	{
		case EUpdateServiceState::None:
			Result = EUpdateServiceUIState::None;
		break;

		case EUpdateServiceState::CheckingForUpdate:
		case EUpdateServiceState::PendingUpdate:
			Result = EUpdateServiceUIState::CheckingForUpdate;
		break;

		case EUpdateServiceState::Installing:
			Result = GetInstallingState();
		break;

		case EUpdateServiceState::DownloadingMovies:
			Result = EUpdateServiceUIState::DownloadingMovies;
		break;

		case EUpdateServiceState::Success:
			Result = EUpdateServiceUIState::Success;
		break;
		
		case EUpdateServiceState::Error:
			Result = EUpdateServiceUIState::Error;
		break;
	}

	return Result;
}

EUpdateServiceUIState UUpdateService::GetInstallingState()
{
	EUpdateServiceUIState Result = EUpdateServiceUIState::None;
	if (IsValid(PendingContent))
	{
		BuildPatchServices::EBuildPatchState BuildPatchState = PendingContent->GetInstallerState();
		switch (BuildPatchState)
		{
		case BuildPatchServices::EBuildPatchState::Queued:
		case BuildPatchServices::EBuildPatchState::Initializing:
			Result = EUpdateServiceUIState::Initializing;
			break;

		case BuildPatchServices::EBuildPatchState::Resuming:
			Result = EUpdateServiceUIState::Resuming;
			break;

		case BuildPatchServices::EBuildPatchState::Downloading:
			Result = EUpdateServiceUIState::Downloading;
			break;

		case BuildPatchServices::EBuildPatchState::Installing:
		case BuildPatchServices::EBuildPatchState::MovingToInstall:
		case BuildPatchServices::EBuildPatchState::SettingAttributes:
		case BuildPatchServices::EBuildPatchState::BuildVerification:
		case BuildPatchServices::EBuildPatchState::CleanUp:
		default:
			Result = EUpdateServiceUIState::Installing;
			break;
		}
	}
	return Result;
}

float UUpdateService::GetTotalDownloadedSize()
{
	if (PendingContent)
	{
		return PendingContent->GetTotalDownloadedSize();
	}
	return 0.f;
}

float UUpdateService::GetDownloadSpeed()
{
	if (PendingContent)
	{
		return PendingContent->GetDownloadSpeed();
	}
	return 0.f;
	
}

FText UUpdateService::GetDownloadStatusText()
{
	if (PendingContent)
	{
		return PendingContent->GetDownloadStatusText();
	}
	return FText::FromString(TEXT("Update Status"));
}

float UUpdateService::GetInstallProgress()
{
	if (PendingContent)
	{
		return PendingContent->GetInstallProgress();
	}
	return 0.f;

}

float UUpdateService::GetDownloadSize()
{
	if (PendingContent)
	{
		return PendingContent->GetDownloadSize();
	}
	return 0.f;
}

FString UUpdateService::GetManifestUrl(const FString& Cdn)
{
	FString PlatformName = UUpdateService::PlatformName();

	URAuth* RAuth = URAuth::GetInstancePtr();
	
	return UUpdateService::GetCloudURL(Cdn) / TEXT("ProjectR_ProjectR") + URAuth::GetInstancePtr()->ReleaseTarget.ManifestBuildNumber + TEXT(".manifest");
}

FString UUpdateService::GetMoviesUrl(const FString& Cdn)
{
	return Cdn / TEXT("Movies");
}

void UUpdateService::RequestContent()
{
	URAuth* RAuth = URAuth::GetInstancePtr();
	FString Cdn = RAuth->ReleaseTarget.Cdn;
	UE_LOG(LogUpdateService, Log, TEXT("Cdn=%s"), *Cdn);
	
	State = EUpdateServiceState::CheckingForUpdate;

	FOnRequestContentSucceeded OnRequestContentSucceeded;
	OnRequestContentSucceeded.BindDynamic(this, &UUpdateService::OnRequestContentSucceeded);
	FOnRequestContentFailed OnRequestContentFailed;
	OnRequestContentFailed.BindDynamic(this, &UUpdateService::OnRequestContentFailed);

	FString ManifestUrl = UUpdateService::GetManifestUrl(Cdn);
	FString CloudUrl = UUpdateService::GetCloudURL(Cdn);

	UE_LOG(LogUpdateService, Log, TEXT("RequestContent with ManifestURL=%s CloudURL=%s"), *ManifestUrl, *CloudUrl);

	UMobilePatchingLibrary::RequestContent(
		ManifestUrl,
		CloudUrl,
		PATCH_INSTALL_DIRECTORY,
		OnRequestContentSucceeded,
		OnRequestContentFailed);
}

void UUpdateService::CheckMovies()
{
	// Determine FilesToDownload

	FString ManifestUrl = UUpdateService::GetMoviesUrl(
		URAuth::GetInstancePtr()->ReleaseTarget.Cdn) / TEXT("manifest.json");

	UE_LOG(LogUpdateService, Log, TEXT("CheckMovies: downloading %s"), *ManifestUrl);


	TSharedRef<IHttpRequest> HttpRequest = FHttpModule::Get().CreateRequest();
	HttpRequest->OnProcessRequestComplete().BindUObject(this, &UUpdateService::OnDownloadManifestComplete);
	HttpRequest->SetVerb("GET");
	HttpRequest->SetHeader("User-Agent", "ProjectR/0.1");
	HttpRequest->SetURL(ManifestUrl);
	HttpRequest->ProcessRequest(); // TODO check return value
}

void UUpdateService::OnDownloadManifestComplete(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bSucceeded)
{
	UE_LOG(LogUpdateService, Log, TEXT("ENTER UUpdateService::OnDownloadManifestComplete"));

	if (!bSucceeded)
	{
		UE_LOG(LogUpdateService, Log, TEXT("OnDownloadManifestComplete: request failed with status code %d"), Response->GetResponseCode());
		OperationFinish(false, TEXT("Failed to download movies manifest."));
		return;
	}

	// Read manifest
	FString JsonString = Response->GetContentAsString();

	TArray<FFileToDownload> Files;

	TSharedPtr<FJsonObject> JsonObject;
	TSharedRef<TJsonReader<> > JsonReader = TJsonReaderFactory<>::Create(JsonString);
	if (!FJsonSerializer::Deserialize(JsonReader, JsonObject) || !JsonObject.IsValid())
	{
		UE_LOG(LogUpdateService, Log, TEXT("OnDownloadManifestComplete: Unable to deserialize. json=[%s]"), *JsonString);
		OperationFinish(false, TEXT("Failed to read movies manifest."));
		return;
	}
	
	TArray<TSharedPtr<FJsonValue>> FilesJson = JsonObject->GetArrayField(TEXT("files"));
	UE_LOG(LogUpdateService, Log, TEXT("OnDownloadManifestComplete: %d files"), FilesJson.Num());

	for (auto Element : FilesJson)
	{
		TSharedPtr<FJsonObject> FileJson = Element->AsObject();
		FString Name = FileJson->GetStringField("name");
		int32 Size = FileJson->GetIntegerField("size");
		FString Sha1 = FileJson->GetStringField("sha1");

		UE_LOG(LogUpdateService, Log, TEXT("OnDownloadManifestComplete: file name %s"), *Name);

		Files.Push(FFileToDownload(Name, Size, Sha1));
	}

	UE_LOG(LogUpdateService, Log, TEXT("OnDownloadManifestComplete: checking files that need downloading"));

	// Go through each file, determine files that need downloading

	FString MoviesDirPath = TEXT("ProjectR/Content/Movies");

	// As unreal file system iswritable union file system, everything is confused.
	UE_LOG(LogUpdateService, Log, TEXT("OnDownloadManifestComplete: movies dir exists = %d"), (int32)IFileManager::Get().DirectoryExists(*MoviesDirPath));
	if (!IFileManager::Get().MakeDirectory(*MoviesDirPath, true))
	{
		UE_LOG(LogUpdateService, Log, TEXT("OnDownloadManifestComplete: failed to make movies directory"));
	}
	UE_LOG(LogUpdateService, Log, TEXT("OnDownloadManifestComplete: movies dir exists = %d"), 
		(int32)IFileManager::Get().DirectoryExists(*MoviesDirPath));

	for (FFileToDownload& Element : Files)
	{
		int64 Size = IFileManager::Get().FileSize(*(MoviesDirPath / Element.Name));

		if (Size == INDEX_NONE) {
			UE_LOG(LogUpdateService, Log, TEXT("OnDownloadManifestComplete: doesn't exist %s"), *Element.Name);
			FilesToDownload.Push(Element);
		}
		else if (Size != Size) //?
		{
			UE_LOG(LogUpdateService, Log, TEXT("OnDownloadManifestComplete: size differ: %d != %d"), Size, Element.Size);
			FilesToDownload.Push(Element);
		}
		// TODO: check hash
	}

	// Determine if needs updating
	if (ShouldInstallPendingContent || FilesToDownload.Num() > 0)
	{
		State = EUpdateServiceState::PendingUpdate;
		OnUpdatePending.Broadcast();
	}
	else
	{
		OperationFinish(true, TEXT("Content is up-to-date."));
	}
}

void UUpdateService::DownloadOneFile()
{
	UE_LOG(LogUpdateService, Log, TEXT("ENTER UUpdateService::DownloadOneFile"));

	if (IndexToFileDownloaded == FilesToDownload.Num())
	{
		UE_LOG(LogUpdateService, Log, TEXT("DownloadOneFile: no more files to download"));
		OperationFinish(true, TEXT("Done"));
		return;
	}

	State = EUpdateServiceState::DownloadingMovies;

	FFileToDownload& File = FilesToDownload[IndexToFileDownloaded];

	FileDownloadedContentLength = File.Size;
	FileDownloadedBytesReceived = 0;

	FString FileUrl = UUpdateService::GetMoviesUrl(URAuth::GetInstancePtr()->ReleaseTarget.Cdn) / File.Name;

	UE_LOG(LogUpdateService, Log, TEXT("DownloadOneFile: start downloading %s"), *FileUrl);

	TSharedRef<IHttpRequest> HttpRequest = FHttpModule::Get().CreateRequest();
	HttpRequest->OnProcessRequestComplete().BindUObject(this, &UUpdateService::OnDownloadOneFileComplete);
	HttpRequest->OnRequestProgress().BindUObject(this, &UUpdateService::OnDownloadOneFileProgress);
	HttpRequest->SetVerb("GET");
	HttpRequest->SetHeader("User-Agent", "ProjectR/0.1");
	HttpRequest->SetURL(FileUrl);
	HttpRequest->ProcessRequest();
}

void UUpdateService::OnDownloadOneFileComplete(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bSucceeded)
{
	UE_LOG(LogUpdateService, Log, TEXT("ENTER UUpdateService::OnDownloadOneFileComplete"));

	Request->OnProcessRequestComplete().Unbind();
	Request->OnRequestProgress().Unbind();

	if (!bSucceeded)
	{
		UE_LOG(LogUpdateService, Log, TEXT("OnDownloadOneFileComplete: fail with status code %d"), Response->GetResponseCode());
		OperationFinish(false, TEXT("Failed to download a movie file"));
		return;
	}

	// Save as file
	FFileToDownload& File = FilesToDownload[IndexToFileDownloaded];

	FString FilePath = FString(TEXT("ProjectR/Content/Movies")) / File.Name;

	UE_LOG(LogUpdateService, Log, TEXT("OnDownloadOneFileComplete: writing %s"), *FilePath);

	FArchive* NewFile = IFileManager::Get().CreateFileWriter(*FilePath);
	if (NewFile == NULL)
	{
		uint32 LastError = FPlatformMisc::GetLastError();
		UE_LOG(LogUpdateService, Log, TEXT("OnDownloadOneFileComplete: failed to create file at %s with error code %u"), *FilePath, LastError);
		OperationFinish(false, TEXT("Failed to save a downloaded file"));
		return;
	}

	const TArray<uint8>& Content = Response->GetContent();

	NewFile->Serialize(const_cast<uint8*>(Content.GetData()), Content.Num());

	NewFile->Flush();
	delete NewFile; //IFileManager::Get().Delete(*FilePath);

	UE_LOG(LogUpdateService, Log, TEXT("OnDownloadOneFileComplete: done writing a file"));

	IndexToFileDownloaded++;

	// Chain
	DownloadOneFile();
}

// TODO
void UUpdateService::OnDownloadOneFileProgress(FHttpRequestPtr Request, int32 BytesSent, int32 BytesReceived)
{
	UE_LOG(LogUpdateService, Log, TEXT("ENTER UUpdateService::OnDownloadOneFileProgress with sent %d, received %d"), BytesSent, BytesReceived);

	FFileToDownload& File = FilesToDownload[IndexToFileDownloaded];

	FHttpResponsePtr Response = Request->GetResponse();
	int32 ContentLength = Response->GetContentLength();

	if (FileDownloadedContentLength != ContentLength)
	{
		UE_LOG(LogUpdateService, Warning, TEXT("OnDownloadOneFileProgress: file %s has different Content-Length from manifest: %d != %d"), *File.Name, FileDownloadedContentLength, ContentLength);
		FileDownloadedContentLength = ContentLength;
	}

	FileDownloadedBytesReceived = BytesReceived;

	UE_LOG(LogUpdateService, Log, TEXT("OnDownloadOneFileProgress: file %s progress %f"), *File.Name, (float)BytesReceived / ContentLength);
}

FString UUpdateService::GetCloudURL(const FString& Cdn)
{
	return Cdn / TEXT("PatchData") / UUpdateService::PlatformName() / TEXT("CloudDir");
}

void UUpdateService::OnRequestContentSucceeded(UMobilePendingContent *MobilePendingContent)
{
	UE_LOG(LogUpdateService, Log, TEXT("ENTER OnRequestContentSucceeded"));

	PendingContent = MobilePendingContent;

	if (InstalledContent)
	{
		// Compare version string
		FString InstalledVersion = InstalledContent->InstalledManifest->GetVersionString();
		FString RemoteVersion = PendingContent->RemoteManifest->GetVersionString();

		UE_LOG(LogUpdateService, Log, TEXT("Comparing Remote version %s with Installed version %s"), *RemoteVersion, *InstalledVersion);
		ShouldInstallPendingContent = ShouldUpdate(InstalledVersion, RemoteVersion);
	}
	else
	{
		ShouldInstallPendingContent = true;
	}

	if (!ShouldInstallPendingContent)
	{
		// Mount now if no need to update content
		if (!InstalledContent->Mount(1, TEXT("")))
		{
			UE_LOG(LogUpdateService, Log, TEXT("OnRequestContentSucceeded: failed to Mount installed Pak"));
			OperationFinish(false, TEXT("Failed to Mount"));
			return;
		}
	}

	CheckMovies();
}

void UUpdateService::OnRequestContentFailed(FText ErrorText, int32 ErrorCode)
{
	UE_LOG(LogUpdateService, Log, TEXT("OnRequestContentFailed %s"), *ErrorText.ToString());

	OperationFinish(false, ErrorText.ToString());
}

bool UUpdateService::ShouldUpdate(const FString& InstalledVersion, const FString& RemoteVersion)
{
	// TODO Version Evaluation Logic
	return InstalledVersion != RemoteVersion;
}

FString UUpdateService::GetUpdateContentDir() const
{
	return FPaths::ProjectPersistentDownloadDir() / PATCH_INSTALL_DIRECTORY;
}

FString UUpdateService::GetInstalledContentVersion()
{
	if (InstalledContent)
	{
		return InstalledContent->InstalledManifest->GetVersionString();
	}
	return TEXT("");
}

FString UUpdateService::GetPendingContentVersion()
{
	if (PendingContent)
	{
		return PendingContent->RemoteManifest->GetVersionString();
	}
	return TEXT("");
}

FString UUpdateService::GetContentVersion()
{
	return ContentVersion;
}

void UUpdateService::SetNoPatchBuild()
{
	State = EUpdateServiceState::Success;
}

bool UUpdateService::IsContentPresent()
{
	URAuth* RAuth = URAuth::GetInstancePtr();
	return RAuth->GetIsDataState();

/*#if PROJECTR_NOPATCH_BUILD
	return true;
#else
	return State == EUpdateServiceState::Success;
#endif*/
}

FString UUpdateService::PlatformName()
{
	FString PlatformName;

#if PLATFORM_ANDROID
	PlatformName = TEXT("Android_ETC2");

	TArray<FString> PossibleTargetPlatforms;
	FPlatformMisc::GetValidTargetPlatforms(PossibleTargetPlatforms);

	for (int32 FormatIndex = 0; FormatIndex < PossibleTargetPlatforms.Num(); FormatIndex++)
	{
		const FString& Format = PossibleTargetPlatforms[FormatIndex];
		UE_LOG(LogUpdateService, Log, TEXT("PossibleTargetPlatforms - %s"), *Format);

		if (Format.Contains(TEXT("ASTC")))
		{
			PlatformName = TEXT("Android_ASTC");
			break;
		}
	}
#elif PLATFORM_IOS
	PlatformName = TEXT("IOS");
#endif

	return PlatformName;
}

void UUpdateService::OnContentInstallSucceeded()
{
	ContentVersion = GetPendingContentVersion();

	InstalledContent = PendingContent;
	PendingContent = nullptr;

	if (!InstalledContent->Mount(0, TEXT("")))
	{
		UE_LOG(LogUpdateService, Log, TEXT("OnContentInstallSucceeded: failed to Mount"));
		OperationFinish(false, TEXT("Failed to Mount pak file"));
		return;
	}
	
	DownloadOneFile();
}

void UUpdateService::OnContentInstallFailed(FText ErrorText, int32 ErrorCode)
{
	UE_LOG(LogUpdateService, Log, TEXT("OnContentInstallFailed %s"), *ErrorText.ToString());
	State = EUpdateServiceState::None;
	OperationFinish(false, ErrorText.ToString());
}
