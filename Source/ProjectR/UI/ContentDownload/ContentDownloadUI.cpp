// Fill out your copyright notice in the Description page of Project Settings.

#include "ProjectR.h"
#include "ContentDownloadUI.h"
#include "UpdateService/UpdateService.h"
#include "UpdateService/Auth/RAuth.h"

#include "UpdateService/PlatformAuth.h"

#define LOCTEXT_NAMESPACE "Startup" 

#define CONTENT_DOWNLOAD_REFRESH_INTERVAL 0.1

// Can be called multiple times - UContentDownloadUI::AskUserWhetherToRetry
void UContentDownloadUI::Init()
{
	RAuth = URAuth::GetInstancePtr();

	UpdateService = UUpdateService::GetInstancePtr();
	if (IsValid(ExtraVersionInfoText))
	{
		ExtraVersionInfoText->SetText(FText::FromString(TEXT("")));
	}
	UpdateProgressBar->SetPercent(0.f);

	UpdateService->OnFinish.AddDynamic(this, &UContentDownloadUI::OnUpdateServiceFinish);

	RefreshUI();
	UpdateVersionString();

	if (!UMobilePatchingLibrary::HasActiveWiFiConnection())
	{
		UE_LOG(LogContentDownloadUI, Log, TEXT("OnUpdatePending: ShowPopup"));
		ShowPopup(FText::FromString(TEXT("Warning")),
			LOCTEXT("patch_no_wifi_warning", "No Wi-Fi connection. Proceed to download Update?"),
			ECommonPopupType::VE_OKCANCEL);
		PopupUI->OnClickOK.AddDynamic(UpdateService, &UUpdateService::StartInstall);
		PopupUI->OnClickCancel.AddDynamic(this, &UContentDownloadUI::OnExit);
	}
	else
	{
		UpdateService->StartInstall();
	}

	// [DEBUG] Debugging
	//bool IsBillingSupported = FPlatformAuth::QueryBillingSupport();
	//UE_LOG(LogContentDownloadUI, Log, TEXT("IsBillingSupported = %s"), IsBillingSupported ? TEXT("true") : TEXT("false"));

}

void UContentDownloadUI::RefreshUI()
{
	auto State = UpdateService->GetUIState();
	FText StatusText;
	float Progress = PreviousProgress;

	switch (State)
	{
		case EUpdateServiceUIState::CheckingForUpdate:
		case EUpdateServiceUIState::Initializing:
			UpdateProgressStatusText->SetText(LOCTEXT("patch_progress_checking_for_update", "Checking for update"));
		break;

		case EUpdateServiceUIState::Resuming:
		case EUpdateServiceUIState::Downloading:
		case EUpdateServiceUIState::Installing:
		{
			FString ProgressText = FString::Printf(
				TEXT("%4.1f/%4.1fMB (%4.1fMB/s)"),
				UpdateService->GetTotalDownloadedSize(),
				UpdateService->GetDownloadSize(),
				UpdateService->GetDownloadSpeed());

			StatusText = FText::FromString(FString::Printf(TEXT("%s %s"), *UpdateService->GetDownloadStatusText().ToString(), *ProgressText));

			if (IsValid(ExtraVersionInfoText))
			{
				ExtraVersionInfoText->SetText(FText::FromString(
					FString::Printf(TEXT("%s -> %s"), *UpdateService->GetInstalledContentVersion(), *UpdateService->GetPendingContentVersion())));
			}

			Progress = UpdateService->GetInstallProgress();
		}
		break;

		case EUpdateServiceUIState::DownloadingMovies:
		{
			FFileToDownload& File = UpdateService->FilesToDownload[UpdateService->IndexToFileDownloaded];
			
			StatusText = FText::FromString(FString::Printf(
				TEXT("Downloading %s (%d/%d) %4.1f/%4.1fMB"),
				*File.Name,
				UpdateService->IndexToFileDownloaded + 1,
				UpdateService->FilesToDownload.Num(),
				(float)FUnitConversion::Convert(UpdateService->FileDownloadedBytesReceived, EUnit::Bytes, EUnit::Megabytes),
				(float)FUnitConversion::Convert(UpdateService->FileDownloadedContentLength, EUnit::Bytes, EUnit::Megabytes)
			));

			Progress = (float)UpdateService->FileDownloadedBytesReceived / UpdateService->FileDownloadedContentLength;
		}
		break;

		case EUpdateServiceUIState::Success:
			StatusText = LOCTEXT("patch_progress_starting_the_game", "Starting the game");
			Progress = 1.f;
		break;

		case EUpdateServiceUIState::Error:
			StatusText = LOCTEXT("patch_progress_error", "Error");
		break;

		default:
			StatusText = FText::FromString(TEXT("FIXME"));
		break;
	}

	UpdateProgressStatusText->SetText(StatusText);

	UpdateProgressBar->SetPercent(Progress);
	Text_CurrentSuccessValue->SetText(FText::FromString(FString::Printf(TEXT("%d%%"), (int)(Progress * 100.f))));

	PreviousProgress = Progress;
}

void UContentDownloadUI::NativeConstruct()
{
	Super::NativeConstruct();
}

void UContentDownloadUI::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	TickDuration += InDeltaTime;
	if (TickDuration > CONTENT_DOWNLOAD_REFRESH_INTERVAL)
	{
		RefreshUI();
		TickDuration = 0.;
	}
	 
	Super::NativeTick(MyGeometry, InDeltaTime);
}

UUP_Popup *UContentDownloadUI::ShowPopup(FText InCaption, FText InContents, ECommonPopupType InPopupType)
{
	UE_LOG(LogContentDownloadUI, Log, TEXT("ENTER ShowPopup"));
	if (!PopupUI)
	{
		UE_LOG(LogContentDownloadUI, Log, TEXT("ShowPopup: Creating widget"));

		UClass* WidgetUClass = PopupUIClass.Get();
		if (WidgetUClass == nullptr)
		{
			PopupUIClass.ToSoftObjectPath().TryLoad();
			WidgetUClass = PopupUIClass.Get();
		}

		PopupUI = CreateWidget<UUP_Popup>(GetWorld(), WidgetUClass);
	}
	UE_LOG(LogContentDownloadUI, Log, TEXT("ShowPopup: SetPopup"));
	PopupUI->SetPopup(InCaption, InContents, InPopupType);
	UE_LOG(LogContentDownloadUI, Log, TEXT("ShowPopup: Add to viewport"));
	PopupUI->AddToViewport(999999);

	//UE_LOG(LogUI, Log, TEXT("ShowPopup %s %d"), *PopupUIClass->GetName(), PopupUI->IsVisible());

	return PopupUI;
}

void UContentDownloadUI::HidePopup()
{
	PopupUI->RemoveFromParent();
}

void UContentDownloadUI::UpdateVersionString()
{
	// Update Version display
	/*
	Reveal
	* Release level
	* Build config
	* Build version
	* Installed content version if exists
	*/

	VersionText->SetText(FText::FromString(FString::Printf(
		TEXT("%s %s Build %s Installed Content rev. %s"),
		*RAuth->GetReleaseTargetEnv(),
		*RAuth->GetBuildConfig(),
		*RAuth->GetBuildVersion(),
		*UpdateService->GetInstalledContentVersion())));
}

void UContentDownloadUI::OnUpdateServiceFinish(bool IsSuccess, FString Message)
{
	UE_LOG(LogContentDownloadUI, Log, TEXT("OnUpdateServiceFinish: %s"), IsSuccess ? TEXT("success") : TEXT("fail"));

	UpdateService->OnFinish.RemoveAll(this);

	if (!IsSuccess)
	{
		AskUserWhetherToRetry(FText::FromString(Message));
	}
	else
	{
		// Transition
		
		// UGameplayStatics::OpenLevel(GetWorld(), TEXT("/Game/Maps/Login_P"));
		// URGameInstance* RGameInstance = RGAMEINSTANCE(this);
		// RGameInstance->ChangeState(EGameStateEnum::VE_Login);
		OnContentDownloadUIFinish.Broadcast();
	}
}

void UContentDownloadUI::OnExit()
{
	GetWorld()->GetFirstPlayerController()->ConsoleCommand("quit");
}

void UContentDownloadUI::AskUserWhetherToRetry(FText Message)
{
	ShowPopup(LOCTEXT("UI_patch_error_popup_title", "Error (retry?)"), Message, ECommonPopupType::VE_OKCANCEL);
	PopupUI->OnClickOK.AddDynamic(this, &UContentDownloadUI::Init);
	PopupUI->OnClickCancel.AddDynamic(this, &UContentDownloadUI::OnExit);
}

#undef LOCTEXT_NAMESPACE 