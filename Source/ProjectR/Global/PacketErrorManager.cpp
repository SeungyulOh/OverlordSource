// Fill out your copyright notice in the Description page of Project Settings.

#include "ProjectR.h"
#include "UI/RWidgetManager.h"
#include "GlobalIntegrated.h"

#include "UI/UI_Common/UP_CommonPopup.h"
#include "Network/HttpRequestClient.h"


#include "UtilFunctionIntegrated.h"
#include "Classes/Kismet/KismetSystemLibrary.h"
#include "PacketErrorManager.h"

#define LACK_CURRENCY_ERROR_CODE 1303

void UPacketErrorManager::Initialize(void)
{
	//RGameInstance = InGameInstance;
}

void UPacketErrorManager::ProcessError(bool InCommon /*= true*/)
{
	if (RGAMEINSTANCE(this) == nullptr)
		return;
	if (IsValid(RGAMEINSTANCE(this)->RWidgetManager))
	{
		int32 iErrorCode = FCString::Atoi(*LastRequestPacketErrorInfo.code);
		if (iErrorCode == LACK_CURRENCY_ERROR_CODE)
		{
			UUIFunctionLibrary::LackCurrencyPopup(RGAMEINSTANCE(this), LastRequestPacketErrorInfo.desc);
			return;
		}

		auto ErrorStringCode	= FString(TEXT("NE")) + LastRequestPacketErrorInfo.code;
		FName ErrorStringKey	= FName(*ErrorStringCode);
		FText LocalizedText		= UUtilFunctionLibrary::GetLocalizedString(EStringTableType::VE_Error, ErrorStringKey);

		FText TempArg;
		FFormatOrderedArguments DescArguments;
		for (auto& DescArg : LastRequestPacketErrorInfo.desc)
		{
			TempArg = FText::FromString(DescArg);
			DescArguments.Emplace(MoveTemp(TempArg));
		}

		FText ErrorCaption = UUtilFunctionLibrary::GetLocalizedString(EStringTableType::VE_UI, COMMONSTRING_NOTIFY);
		FText ErrorText = FText::Format(LocalizedText, DescArguments);
		if (InCommon)
		{
			UUIFunctionLibrary::ShowCommonPopup(ErrorCaption, ErrorText, ECommonPopupType::VE_Error);
		}
		else
		{
			UUIFunctionLibrary::ShowCommonPopupSplash(ErrorCaption, ErrorText);
		}
	}
}

bool UPacketErrorManager::IsCheckError(EPacketType InLastRequestPacketType, ECRUD is, FERROR_INFO InErrorInfo)
{
	SetLastRequestPacketType(InLastRequestPacketType);
	SetLastRequestPacketErrorInfo(InErrorInfo);
	if (is != ECRUD::CRUD_OK)
	{ 
		FString errString = TEXT("Name[") + InErrorInfo.name + TEXT("] Code[") + InErrorInfo.code + TEXT("]");
// #if WITH_EDITOR
		UE_LOG(LogCriticalErrors, Log, TEXT("Packet Error %s"), *errString);
		UUIFunctionLibrary::ShowCommonPopup(FText::FromString(InErrorInfo.code), FText::FromString(InErrorInfo.name), ECommonPopupType::VE_OK);
// #else

// 		URGameInstance* RGameInstance = RGAMEINSTANCE(this);
// 		RGameInstance->RWidgetManager->ShowCommonPopupSplash(FText::FromString(TEXT("Chaos")), FText::FromString(errString));
// 		UUIFunctionLibrary::GoContents(EUIMenuIDEnum::VE_MainLobby, 0);
//#endif
		return true;
	}
	return false;
}

void UPacketErrorManager::OnHTTPDisconnect()
{
	if (RGAMEINSTANCE(this) == nullptr)
		return;
	if (IsValid(RGAMEINSTANCE(this)->RWidgetManager))
	{
		UUIFunctionLibrary::ShowWaitingPopup(false);
		auto ErrorText		= UUtilFunctionLibrary::GetLocalizedString(EStringTableType::VE_Error, FName("NE1000"));
		auto UP_CommonPopup = UUIFunctionLibrary::ShowCommonPopup(FText::FromString(TEXT("")), ErrorText, ECommonPopupType::VE_Error);
		if (IsValid(UP_CommonPopup))
		{
			UP_CommonPopup->OnClickOK.RemoveDynamic(this, &UPacketErrorManager::Retransmit);
			UP_CommonPopup->OnClickOK.AddDynamic(this, &UPacketErrorManager::Retransmit);

			UP_CommonPopup->OnClickCancel.RemoveDynamic(this, &UPacketErrorManager::GameRestart);
			UP_CommonPopup->OnClickCancel.AddDynamic(this, &UPacketErrorManager::GameRestart);
		}
	}
}

void UPacketErrorManager::SetLastRequestJsonString(FString& InRequestJsonString)
{
	LastRequestJsonString = InRequestJsonString;
}

void UPacketErrorManager::Retransmit()
{
	if (RGAMEINSTANCE(this) == nullptr)
		return;
	if (LastRequestJsonString.IsEmpty() == false)
	{
		if (IsValid(RGAMEINSTANCE(this)->HttpClient))
		{
			RGAMEINSTANCE(this)->HttpClient->SetBusy(false);
			RGAMEINSTANCE(this)->HttpClient->TransmitRequest(LastRequestJsonString);
		}
	}
}

void UPacketErrorManager::GameRestart()
{
	if (RGAMEINSTANCE(this) == nullptr)
		return;
	UWorld* TargetWorld = RGAMEINSTANCE(this)->GetWorld();
	auto PlayerController = UUtilFunctionLibrary::GetPlayerController(TargetWorld);
	UKismetSystemLibrary::QuitGame(RGAMEINSTANCE(this), PlayerController, EQuitPreference::Quit);
}

//Setter
void UPacketErrorManager::SetLastRequestPacketErrorInfo(FERROR_INFO InErrorInfo)
{
	LastRequestPacketErrorInfo = InErrorInfo;
}

void UPacketErrorManager::SetLastRequestPacketType(EPacketType InLastRequestPacketType)
{
	LastRequestPacketType = InLastRequestPacketType;
}



//Getter
FERROR_INFO	UPacketErrorManager::GetLastPacketErrorInfo()
{
	return LastRequestPacketErrorInfo;
}

EPacketType UPacketErrorManager::GetLastRequestPacketType()
{
	return LastRequestPacketType;
}