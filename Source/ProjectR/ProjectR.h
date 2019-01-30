// Fill out your copyright notice in the Description page of Project Settings.

#ifndef __PROJECTR_H__
#define __PROJECTR_H__


#include "EngineMinimal.h"

//#include "Engine.h"
#include "ProjectRClasses.h"

#include "Runtime/UMG/Public/UMG.h"
#include "Runtime/UMG/Public/UMGStyle.h"
#include "Runtime/UMG/Public/Slate/SObjectWidget.h"
#include "Runtime/UMG/Public/IUMGModule.h"
#include "Runtime/UMG/Public/Blueprint/UserWidget.h"


extern PROJECTR_API class URGameInstance*	GRGameInstance;

#if WITH_EDITOR
#define RGAMEINSTANCE(p) (URGameInstance::GetRGameInstance(p))
#else
#define RGAMEINSTANCE(p) GRGameInstance
#endif



//General Log
DECLARE_LOG_CATEGORY_EXTERN(LogBattle, Log, All);
DECLARE_LOG_CATEGORY_EXTERN(LogGamePlayMode, Log, All);
DECLARE_LOG_CATEGORY_EXTERN(Log_Debug_Damage, Log, All);
//General Log
DECLARE_LOG_CATEGORY_EXTERN(LogRTS, Log, All);
DECLARE_LOG_CATEGORY_EXTERN(LogNetwork, Log, All);

//Logging during game startup
DECLARE_LOG_CATEGORY_EXTERN(LogGamePlay, Log, All);

//Logging for your AI system
DECLARE_LOG_CATEGORY_EXTERN(LogAI, Log, All);

//Logging for Critical Errors that must always be addressed
DECLARE_LOG_CATEGORY_EXTERN( LogCriticalErrors, Log, All);

//Logging UI
DECLARE_LOG_CATEGORY_EXTERN(LogUI, Log, All);


//2016.02.17 jh
DECLARE_LOG_CATEGORY_EXTERN(LogTutorial, Log, All);


//InputManager Log
DECLARE_LOG_CATEGORY_EXTERN(LogInputMgr, Log, All);


//Dialogue Log
DECLARE_LOG_CATEGORY_EXTERN(LogDialogue, Log, All);

DECLARE_LOG_CATEGORY_EXTERN(LogBillingManager, Log, All);

// StageFunctionLibrary - for reporting SpawnerInfo error to designer
DECLARE_LOG_CATEGORY_EXTERN(LogSpawner, Log, All);

// Startup Log

DECLARE_LOG_CATEGORY_EXTERN(LogRAuth, Log, All);

// Inventory Log
DECLARE_LOG_CATEGORY_EXTERN(LogRInventory, Log, All);

// OnlineIdentityLoginUI Log
DECLARE_LOG_CATEGORY_EXTERN(LogOnlineIdentityLoginUI, Log, All);

// FlatBuffersHelper Log
DECLARE_LOG_CATEGORY_EXTERN(LogFlatBuffersHelper, Log, All);

// LogUpdateService Log
DECLARE_LOG_CATEGORY_EXTERN(LogUpdateService, Log, All);

// LogContentDownloadUI Log
DECLARE_LOG_CATEGORY_EXTERN(LogContentDownloadUI, Log, All);

// LogTcpConnManager Log
DECLARE_LOG_CATEGORY_EXTERN(LogTcpConnManager, Log, All);

// LogChatProtocols Log
DECLARE_LOG_CATEGORY_EXTERN(LogChatProtocols, Log, All);

// LogChatManager Log
DECLARE_LOG_CATEGORY_EXTERN(LogChatManager, Log, All);

// LogAndroidPlatformAuth Log
DECLARE_LOG_CATEGORY_EXTERN(LogAndroidPlatformAuth, Log, All);

DECLARE_LOG_CATEGORY_EXTERN(LogLobby, Log, All);

DECLARE_LOG_CATEGORY_EXTERN(LogSound, Log, All);

#endif