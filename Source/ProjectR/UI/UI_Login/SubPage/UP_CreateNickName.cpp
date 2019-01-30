// Fill out your copyright notice in the Description page of Project Settings.

#include "ProjectR.h"

#include "GlobalIntegrated.h"


#include "GameInfo/UserInfo.h"
#include "Network/PacketDataStructures.h"
#include "Network/PacketFunctionLibrary.h"

#include "UtilFunctionIntegrated.h"
#include "UI/RWidgetManager.h"
#include "UI/Component/CheckBox_Radio.h"
#include "UI/Component/CanvasPanel_RadioGroup.h"

#include "UpdateService/LoginGameMode.h"
#include "UP_CreateNickName.h"

UENUM(BlueprintType)
enum class EIDCheckType : uint8
{
	VE_LengthLimit		UMETA(DisplayName = "VE_LengthLimit"),
	VE_Slang			UMETA(DisplayName = "VE_Slang"),
	VE_OverlapNickName	UMETA(DisplayName = "VE_OverlapNickName"),
	VE_MAX				UMETA(DisplayName = "VE_MAX"),
};


void UUP_CreateNickName::NativeConstruct()
{
	Super::NativeConstruct();


	CheckState.SetNum(int32(EIDCheckType::VE_MAX));
}

void UUP_CreateNickName::NativeDestruct()
{
	
	IsInitActive = false;

	Super::NativeDestruct();
}

void UUP_CreateNickName::Refresh()
{
	FText Empty;
	EditText_NickName->SetText(Empty);

	Super::Refresh();
}

/*void UUP_CreateNickName::ErrorNickPopup(ELoginResult InError)
{
	//switch(InError)
	{
	//case ELoginResult::VE_NickAlreadyExists:
	//case ELoginResult::VE_EmptyNick:
	//case ELoginResult::VE_AccountCreateNickInvalid:
	}
}*/

void UUP_CreateNickName::CallProcess()
{
	TargetString = EditText_NickName->GetText().ToString();
	if (ExceptionLengthLimit())
	{
		//UUP_Popup* UAuthStateMachine::ShowPopup(InCaption, InDesc, ECommonPopupType::OK);
		ALoginGameMode* GameMode = Cast<ALoginGameMode>(UGameplayStatics::GetGameMode(GetWorld()));
		if (GameMode)
		{
			FText InCaption = FText::FromString(TEXT("Warning"));
			//change config ref
			FText InDesc = FText::FromStringTable("/Game/StartupContent/Data/UI_StringTable.UI_StringTable", "UI_Start_Nickname_Desc");
			
			GameMode->ShowExceptionLoginPopup(InCaption, InDesc);
			//UUIFunctionLibrary::ShowCommonPopupSplash(InCaption, InDesc);
		}
		return;
	}

	//ExceptionSpecialChar();
	ExceptionSlang();
	ExceptionOverlapNickName();

	//CheckProcess Delegate
}

bool UUP_CreateNickName::CheckProcess()
{
	int32 Count = 0;
	for (auto Element : CheckState)
	{
		if (Element)
		{
			break;
		}
		else
		{
			++Count;
		}
	}

	//GetWorldTimerManager().SetTimerForNextTick(this, &UUP_CreateNickName::LateUpdate);

	return (Count == int32(EIDCheckType::VE_MAX));
}

void UUP_CreateNickName::InValidNickName(bool bInSuccess)
{

	if (bInSuccess)
	{
		//Success
		//CallNextPage();
	}
	else
	{
		//OnErrorPopup
	}
	
}

bool UUP_CreateNickName::ExceptionLengthLimit()
{
	int32 Index = TargetString.Len();
	if (2 > Index || Index > 12)
	{
		//popup
		CheckState[int32(EIDCheckType::VE_LengthLimit)] = true;
		return true;
	}
	CheckState[int32(EIDCheckType::VE_LengthLimit)] = false;
	return false;
}


bool UUP_CreateNickName::ExceptionSlang()
{
	CheckState[int32(EIDCheckType::VE_Slang)] = false;
	return false;
}

bool UUP_CreateNickName::ExceptionOverlapNickName()
{
	CheckState[int32(EIDCheckType::VE_OverlapNickName)] = false;
	return false;
}

void UUP_CreateNickName::OnButton_Close()
{
	this->SetVisibility(ESlateVisibility::Collapsed);
}


void UUP_CreateNickName::OnButton_Click()
{
	CallProcess();

	if (CheckProcess())
	{
		CallNextPage();
	}
}


void UUP_CreateNickName::CallNextPage()
{
	auto RAuth = URAuth::GetInstancePtr();
	FString NickID = EditText_NickName->GetText().ToString();
	RAuth->SetNickName(NickID);
	//RAuth->SaveAccountLocalRecord(); //todo del

	Super::CallNextPage();
}

