// Fill out your copyright notice in the Description page of Project Settings.

#include "ProjectR.h"

IMPLEMENT_PRIMARY_GAME_MODULE( FDefaultGameModuleImpl, ProjectR, "ProjectR" );



//Battle Log
DEFINE_LOG_CATEGORY(LogBattle);
DEFINE_LOG_CATEGORY(LogGamePlayMode);
DEFINE_LOG_CATEGORY(Log_Debug_Damage);
//Network Log
DEFINE_LOG_CATEGORY(LogRTS);
DEFINE_LOG_CATEGORY(LogNetwork);

//Logging during game startup
DEFINE_LOG_CATEGORY(LogGamePlay);

//Logging for your AI system
DEFINE_LOG_CATEGORY(LogAI);

//Logging for Critical Errors that must always be addressed
DEFINE_LOG_CATEGORY(LogCriticalErrors);

//Logging UI
DEFINE_LOG_CATEGORY(LogUI); 

//2016.02.17 jh
//Logging Tutorial
DEFINE_LOG_CATEGORY(LogTutorial);

//InputManager Log
DEFINE_LOG_CATEGORY(LogInputMgr);

// Dialogue Log
DEFINE_LOG_CATEGORY(LogDialogue);

// StageFunctionLibrary
DEFINE_LOG_CATEGORY(LogSpawner);

// BillingManager Log
DEFINE_LOG_CATEGORY(LogBillingManager);

// Startup Log
DEFINE_LOG_CATEGORY(LogRAuth);

// Inventory Log
DEFINE_LOG_CATEGORY(LogRInventory);

// OnlineIdentityLoginUI Log
DEFINE_LOG_CATEGORY(LogOnlineIdentityLoginUI);

// FlatBuffersHelper Log
DEFINE_LOG_CATEGORY(LogFlatBuffersHelper);

// LogUpdateService Log
DEFINE_LOG_CATEGORY(LogUpdateService);

// LogContentDownloadUI Log
DEFINE_LOG_CATEGORY(LogContentDownloadUI);

// LogTcpConnManager Log
DEFINE_LOG_CATEGORY(LogTcpConnManager);

// LogChatProtocols Log
DEFINE_LOG_CATEGORY(LogChatProtocols);

// LogChatManager Log
DEFINE_LOG_CATEGORY(LogChatManager);

// LogAndroidPlatformAuth Log
DEFINE_LOG_CATEGORY(LogAndroidPlatformAuth);

DEFINE_LOG_CATEGORY(LogLobby);

DEFINE_LOG_CATEGORY(LogSound);