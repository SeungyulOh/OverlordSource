// Fill out your copyright notice in the Description page of Project Settings.

#include "ProjectR.h"
//#include "Network/PacketFunctionLibrary.h"
//#include "Network/HttpRequestClient.h"
//#include "Network/PacketDataStructures.h"
#include "UtilFunctionIntegrated.h"
#include "GlobalIntegrated.h"

#include "UI/RWidgetManager.h"
#include "OnlineIdentityLoginUI.h"
#include "UpdateService/UpdateService.h"
#include "Engine/UserInterfaceSettings.h"

#include "UI/UI_Login/SubPage/BaseSubPage.h"
#include "UpdateService/LoginDefs.h"
#include "UI/UI_Login/SubPage/UP_SelectPlatform.h"

UENUM(BlueprintType)
enum class ELoginSubPageType : uint8
{
	VE_AcceptTerns		UMETA(DisplayName = "AcceptTerns"),
	VE_SelectPlatform	UMETA(DisplayName = "SelectPlatform"),
	VE_CreateNickName	UMETA(DisplayName = "CreateNickName"),
};

UENUM(BlueprintType)
enum class EMainPageStateType : uint8
{
	VE_Waiting			UMETA(DisplayName = "Waiting"),
	VE_Touch			UMETA(DisplayName = "Touch"),
	VE_SubPage			UMETA(DisplayName = "SubPage"),
};


void UOnlineIdentityLoginUI::NativeConstruct()
{
	UE_LOG(LogUI, Log, TEXT("ENTER UOnlineIdentityLoginUI::NativeContruct"));

	Super::NativeConstruct();
	
	if (Button_Click)
	{
		Button_Click->OnClicked.RemoveDynamic(this, &UOnlineIdentityLoginUI::OnClickButton);
		Button_Click->OnClicked.AddDynamic(this, &UOnlineIdentityLoginUI::OnClickButton);
	}
}

void UOnlineIdentityLoginUI::NativeDestruct()
{
	URGameInstance* RGameInstance = RGAMEINSTANCE(this);
	URGameInstance* WorldGameInstance = Cast<URGameInstance>(UGameplayStatics::GetGameInstance(GetWorld()));
	if (WorldGameInstance)
	{
		if (WorldGameInstance->EventManager->OnUpdatePage.IsBound())
		{
			WorldGameInstance->EventManager->OnUpdatePage.RemoveDynamic(this, &UOnlineIdentityLoginUI::RequestFromSubPage);
		}
	}
	//todo : move qsmname
	URAuth* RAuth = URAuth::GetInstancePtr();
	if (RAuth)
	{
		RAuth->SetIsDataState(true);
	}

	Super::NativeDestruct();
}

void UOnlineIdentityLoginUI::Init()
{
	URGameInstance* RGameInstance = RGAMEINSTANCE(this);
	URGameInstance* WorldGameInstance = Cast<URGameInstance>(UGameplayStatics::GetGameInstance(GetWorld()));
	RGameInstance->EventManager->OnUpdatePage.RemoveDynamic(this, &UOnlineIdentityLoginUI::RequestFromSubPage);
	RGameInstance->EventManager->OnUpdatePage.AddDynamic(this, &UOnlineIdentityLoginUI::RequestFromSubPage);

	// Display version info for mobile dev build
	/*if (IsValid(Text_CopyrightNotice))
	{
		Text_CopyrightNotice->SetText(FText::FromString(TEXT("Copyright(C) 2018 KOKOMO Games. All rights reserved.")));//"Copyright(C) 2018 KakaoGames Corp. | KOKOMO Games. All rights reserved."
	}*/
	/*if (IsValid(Text_DevVersionString))
	{
		URAuth* RAuth = URAuth::GetInstancePtr();
		Text_DevVersionString->SetText(FText::FromString(FString::Printf(
			TEXT("%s %s Build rev. %s"),
			*RAuth->GetReleaseTargetEnv(),
			*RAuth->GetBuildConfig(),
			*RAuth->GetBuildVersion())));

		Text_DevVersionString->SetVisibility(ESlateVisibility::HitTestInvisible);
	}*/

	Show_Panel_Touch();
	/*if (Button_Click)
	{
		Button_Click->SetVisibility(ESlateVisibility::Visible);
	}
	this->PlayAni(TEXT("Ani_Touch"));*/
}

void UOnlineIdentityLoginUI::Show_Panel_Login()
{
	Hide_Panel_Waiting();
	
	//	Panel_Login->SetVisibility(ESlateVisibility::Visible);
	//Panel_DevLogin->SetVisibility(ESlateVisibility::Visible);
	Button_Click->SetVisibility(ESlateVisibility::Visible);
}

void UOnlineIdentityLoginUI::Show_Panel_Touch()
{
	Hide_Panel_Waiting();
	/*if (Panel_Waiting)
	{
		Panel_Waiting->SetVisibility(ESlateVisibility::Collapsed);
	}*/

	if (Button_Click)
	{
		Button_Click->SetVisibility(ESlateVisibility::Visible);
	}

	PlayAni(TEXT("Ani_Touch"), false, 0);
}

void UOnlineIdentityLoginUI::Show_Panel_Waiting()
{
	Panel_Waiting->SetVisibility(ESlateVisibility::Visible);

	if (Button_Click)
	{
		Button_Click->SetVisibility(ESlateVisibility::Collapsed);
	}
}

void UOnlineIdentityLoginUI::Hide_Panel_Waiting()
{
	Panel_Waiting->SetVisibility(ESlateVisibility::Collapsed);
}

void UOnlineIdentityLoginUI::HideAllPanels()
{
	//	Panel_Login->SetVisibility(ESlateVisibility::Hidden);
	//Panel_DevLogin->SetVisibility(ESlateVisibility::Collapsed);
	//	Panel_NickName->SetVisibility(ESlateVisibility::Hidden);

	Hide_Panel_Waiting();

	if (Button_Click)
	{
		Button_Click->SetVisibility(ESlateVisibility::Hidden);
	}
}

void UOnlineIdentityLoginUI::SetLoginError()
{
	//URAuth* RAuth = URAuth::GetInstancePtr();
	//RAuth->EmptyAccount();

	SubPageIndex = int32(ELoginSubPageType::VE_SelectPlatform);
	RefreshSubPage();
}

void UOnlineIdentityLoginUI::OnClickButton()
{
	ELoginProcess Process = URAuth::GetInstancePtr()->IsAccountState();
	switch (Process)
	{
		case ELoginProcess::VE_NeedLoginId:
		{
			SubPageIndex = int32(ELoginSubPageType::VE_AcceptTerns);
			RefreshSubPage();
		}
		break;
		case ELoginProcess::VE_NeedCheckAccount:
		{
			//Show_Panel_Waiting();
			//URAuth::GetInstancePtr()->Login();
			OnPanelTouchClicked.Broadcast();
		}
		break;
		case ELoginProcess::VE_NeedPlatformLogin:
		{
			SubPageIndex = int32(ELoginSubPageType::VE_SelectPlatform);
			if (Switcher_SubPage)
			{
				//todo qsmname - temp design
				auto SubWidget = Switcher_SubPage->GetWidgetAtIndex(SubPageIndex);
				auto TargetWidget = Cast<UUP_SelectPlatform>(SubWidget);
				if (TargetWidget)
				{
					SubPageIndex = 9; //force 
					TargetWidget->OnButton_Google();
				}
			}
		}
		break;
		case ELoginProcess::VE_NeedNickName:
		{
			SubPageIndex = int32(ELoginSubPageType::VE_CreateNickName);
			RefreshSubPage();
		}
		break;
		case ELoginProcess::VE_NeedReTry:
		{
			SubPageIndex = int32(ELoginSubPageType::VE_SelectPlatform);
			RefreshSubPage();
		}
		break;
		case ELoginProcess::VE_OK:
		default:
		{
			OnPanelTouchClicked.Broadcast();
		}
		break;
	}
}

void UOnlineIdentityLoginUI::RequestFromSubPage()
{
	if (Switcher_SubPage == nullptr)
		return;

	++SubPageIndex;
	//bool bIsRefresh = ((!URAuth::GetInstancePtr()->GetIsCheckAccount()) && Switcher_SubPage->GetNumWidgets() > SubPageIndex);	
	bool bIsRefresh = (Switcher_SubPage->GetNumWidgets() > SubPageIndex);
	if (bIsRefresh)
	{
		RefreshSubPage();
	}
	else
	{
		Switcher_SubPage->SetVisibility(ESlateVisibility::Collapsed);
		OnClickButton();
	}
}

void UOnlineIdentityLoginUI::RefreshSubPage()
{
	if (Switcher_SubPage)
	{
		Switcher_SubPage->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
		Switcher_SubPage->SetActiveWidgetIndex(SubPageIndex);
		UBaseSubPage* SubPage = Cast<UBaseSubPage>(Switcher_SubPage->GetActiveWidget());
		if (SubPage)
		{
			SubPage->Refresh();
		}
	}
}

UContentDownloadUI* UOnlineIdentityLoginUI::ShowContentDownloadUI()
{
	HideAllPanels();

	UClass* WidgetUClass = ContentDownloadUIClass.Get();
	if (WidgetUClass == nullptr)
	{
		ContentDownloadUIClass.ToSoftObjectPath().TryLoad();
		WidgetUClass = ContentDownloadUIClass.Get();
	}

	auto ContentDownloadUI = CreateWidget<UContentDownloadUI>(GetWorld(), WidgetUClass);
	if (ContentDownloadUI)
	{
		ContentDownloadUI->AddToViewport(99999);
		ContentDownloadUI->Init();
	}
	return ContentDownloadUI;
}

/*UUP_Popup* UOnlineIdentityLoginUI::ShowExceptionLoginPopup(FText InCaption, FText InContents,
	ECommonPopupType InPopupType)
{
	if (!IsValid(PopupUI))
	{
		UClass* WidgetUClass = LoginUI->PopupUIClass.Get();
		if (WidgetUClass == nullptr)
		{
			PopupUIClass.ToSoftObjectPath().TryLoad();
			WidgetUClass = LoginUI->PopupUIClass.Get();
		}

		PopupUI = CreateWidget<UUP_Popup>(RGameInstance, WidgetUClass);
	}
	PopupUI->SetPopup(InCaption, InContents, InPopupType);
	PopupUI->AddToViewport(99999);

	UPROPERTY()
	UUP_Popup* PopupUI = nullptr;

	return PopupUI;
}*/


