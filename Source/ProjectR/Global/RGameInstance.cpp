// Fill out your copyright notice in the Description page of Project Settings.

#include "ProjectR.h"
#include "RGameInstance.h"
#include "Runtime/Engine/Public/ProfilingDebugging/MallocLeakReporter.h"
#include "GameInfo/UserInfo.h"
#include "GameInfo/GuildInfo.h"
#include "GameInfo/PlayStateInfo.h"
#include "GameInfo/RInventory.h"
#include "GameInfo/RewardInfo.h"
#include "GameState/Stage/Spawner/BaseStageInfo.h"
#include "GameState/TowerLobbyState.h"
#include "GameState/PlayState.h"
#include "GlobalIntegrated.h"
#include "Global/ShopManager.h"
#include "Global/MirrorDungeonManager.h"
#include "Global/MissionManager.h"
#include "Global/ForceTutorialManager.h"
#include "Global/RaidManager.h"
#include "Global/XmlManager.h"

#include "LeaderControlHelper.h"

#include "NotifyList/NotifyEventList.h"


#include "Network/HttpRequestClient.h"
#include "Network/PacketFunctionLibrary.h"
#include "Network/RTSManager.h"
#include "Network/Chat/ChatManager.h"

#include "UI/RWidgetManager.h"

#include "UtilFunctionIntegrated.h"
#include "Runtime/Engine/Classes/Kismet/KismetSystemLibrary.h"
#include "Runtime/Core/Public/Templates/SharedPointer.h"
#include "Skill_V2/NetSkillManager.h"

#include "UpdateService/UpdateService.h"
#include "UpdateService/Auth/RAuth.h"
#include "UpdateService/PlatformAuth.h"
#include "Blueprint/WidgetLayoutLibrary.h"

#include "ContentGameMode/Modes/TowerLobbyGameMode.h"
#include "Lobby/IndoorLobbyStateActor.h"
#include "Lobby/OutdoorLobbyStateActor.h"
#include "GameFramework/GameUserSettings.h"
#include "GameState/Tutorial/TutorialState.h"
#include "ResourceCacheManager.h"
#include "SaveObject/RSaveGame.h"

PROJECTR_API URGameInstance* GRGameInstance = nullptr;

URGameInstance::URGameInstance(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	UE_LOG(LogGamePlay, Log, TEXT("Init game instance - %s "), *(this->GetName()) );
#ifdef WITH_EDITOR
	GRGameInstance = this;
#endif
}

URGameInstance* URGameInstance::GetRGameInstance(class UObject* OutterOwner)
{
	
#ifdef WITH_EDITOR
	if (OutterOwner == nullptr || OutterOwner->GetWorld() == nullptr)
	{
//		UE_LOG(LogGamePlay, Log, TEXT("OutterOwner == nullptr or OutterOwner->GetWorld() == nullptr"));
		return GRGameInstance;
	}

	URGameInstance* RGameInstance = Cast<URGameInstance>(OutterOwner->GetWorld()->GetGameInstance());
	if (RGameInstance == nullptr)
	{
		ensure(RGameInstance==nullptr);
		UE_LOG(LogGamePlay, Error, TEXT("EGameInstance == nullptr"));
		return GRGameInstance;
	}
	return RGameInstance;
#else
	return GRGameInstance;
#endif
}

void URGameInstance::Init()
{
// #if MALLOC_LEAKDETECTION == 1
// 	float ProfileTime = 300 * 1;
// 	FMallocLeakReporter::Get().Start(0, ProfileTime);
// 	RGAMEINSTANCE(this)->GetWorld()->GetTimerManager().ClearTimer(ProfileHandler);
// 	FTimerDelegate MemoryCallback;
// 	MemoryCallback.BindLambda([=] {
// #if MALLOC_LEAKDETECTION == 1
// 		FMallocLeakReporter::Get().WriteReports();
// 		FMallocLeakReporter::Get().Stop();
// #endif
// 	});
// 	RGAMEINSTANCE(this)->GetWorld()->GetTimerManager().SetTimer(ProfileHandler, MemoryCallback, ProfileTime, false);
// #endif
	//URAsyncLoader::GetInstancePtr();


#if LOG_TIME
	double StartTime = FPlatformTime::Seconds();
#endif

	GRGameInstance = this;
	GetSaveGame();
		
	//SaveGame = Cast<URSaveGame>(UGameplayStatics::LoadGameFromSlot(TEXT("SAVEDATA"), 0));
	RGameUserSetting = Cast< URGameUserSetting >(UGameUserSettings::GetGameUserSettings());
	if (RGameUserSetting)
	{
		//RGameUserSetting Initialize() method have to be called before OptionManager Initialize() call.
		RGameUserSetting->Initialize();
		RGameUserSetting->SetFullscreenMode(EWindowMode::Fullscreen);
		RGameUserSetting->SetVSyncEnabled(true);
		RGameUserSetting->SetFrameRateLimit(30.0f);
		EtcSetting();
		RGameUserSetting->SetIsEditorState(false);
		RGameUserSetting->ApplySettings(true);
	}
	

	InitGameStates();

	// Early initialization of HttpRequestClient before InitBeforeLaunch for use during login
	HttpClient = NewObject< UHttpRequestClient >(UHttpRequestClient::StaticClass());
	HttpClient->Initialize();

	//Common
	EventManager = NewObject< UEventManager >((UObject*)GetTransientPackage(), UEventManager::StaticClass());
	RWidgetManager = NewObject< URWidgetManager>((UObject*)GetTransientPackage(), URWidgetManager::StaticClass());
	//Lobby
	MailManager = NewObject<UMailManager>((UObject*)GetTransientPackage(), UMailManager::StaticClass());
	QuestManager = NewObject<UQuestManager>((UObject*)GetTransientPackage(), UQuestManager::StaticClass());
	//GameMode
	ArenaManager = NewObject< UArenaManager>((UObject*)GetTransientPackage(), UArenaManager::StaticClass());
	ColosseumManager = NewObject< UColosseumManager>((UObject*)GetTransientPackage(), UColosseumManager::StaticClass());
	MirrorDungeonManager = NewObject< UMirrorDungeonManager >((UObject*)GetTransientPackage(), UMirrorDungeonManager::StaticClass());
	MissionManager = NewObject< UMissionManager >((UObject*)GetTransientPackage(), UMissionManager::StaticClass());
	RealTimeModManager = NewObject< URealTimeModManager>((UObject*)GetTransientPackage(), URealTimeModManager::StaticClass());
	ShopManager = NewObject< UShopManager>((UObject*)GetTransientPackage(), UShopManager::StaticClass());
	NetSkillManager = NewObject<UNetSkillManager>((UObject*)GetTransientPackage(), UNetSkillManager::StaticClass());
	HeroRuneDungeonManager = NewObject<UHeroRunekDungeonManager>((UObject*)GetTransientPackage(), UHeroRunekDungeonManager::StaticClass());

	RaidManager = NewObject< URaidManager >((UObject*)GetTransientPackage(), URaidManager::StaticClass());

	//GamePlay
	BattleRecordManager = NewObject< UBattleRecordManager>((UObject*)GetTransientPackage(), UBattleRecordManager::StaticClass());
	//etc
	PlayModeManager = NewObject<UPlayModeManager>((UObject*)GetTransientPackage(), UPlayModeManager::StaticClass());
	//Info
	UserInfo = NewObject< UUserInfo >((UObject*)GetTransientPackage(), UUserInfo::StaticClass());
	GuildInfo = NewObject< UGuildInfo >((UObject*)GetTransientPackage(), UGuildInfo::StaticClass());
	RInventory = NewObject< URInventory >((UObject*)GetTransientPackage(), URInventory::StaticClass());
	BaseStageInfo = NewObject< UBaseStageInfo >((UObject*)GetTransientPackage(), UBaseStageInfo::StaticClass());
	RewardInfo = NewObject< URewardInfo>((UObject*)GetTransientPackage(), URewardInfo::StaticClass());
	PlayStateInfo = NewObject< UPlayStateInfo >((UObject*)GetTransientPackage(), UPlayStateInfo::StaticClass());
	ObjectPoolManager = NewObject< UObjectPoolManager>((UObject*)GetTransientPackage(), UObjectPoolManager::StaticClass());
	LoadingSceneManager = NewObject< ULoadingSceneManager>((UObject*)GetTransientPackage(), ULoadingSceneManager::StaticClass());
	PartyManager = NewObject< UPartyManager>((UObject*)GetTransientPackage(), UPartyManager::StaticClass());
	AbnormalStateGroupTable_V2 = NewObject<UAbnormalStateGroupTable_V2>((UObject*)GetTransientPackage(), UAbnormalStateGroupTable_V2::StaticClass());
	ServerInfoUpdateManager = NewObject<UServerInfoUpdateManager>((UObject*)GetTransientPackage(), UServerInfoUpdateManager::StaticClass());
	ConsoleCheatManager = NewObject< UConsoleCheatManager>((UObject*)GetTransientPackage(), UConsoleCheatManager::StaticClass());
	PointLightManager = NewObject< URPointLightManager>((UObject*)GetTransientPackage(), URPointLightManager::StaticClass());
	OtherHeroManager = NewObject<UOtherHeroManager>((UObject*)GetTransientPackage(), UOtherHeroManager::StaticClass());
	CampaignManager = NewObject<UCampaignManager>((UObject*)GetTransientPackage(), UCampaignManager::StaticClass());
	AchievementManager = NewObject<UAchievementManager>((UObject*)GetTransientPackage(), UAchievementManager::StaticClass());
	SequenceManager = NewObject<USequenceManager>((UObject*)GetTransientPackage(), USequenceManager::StaticClass());
	InputManager = NewObject<UInputManager>((UObject*)GetTransientPackage(), UInputManager::StaticClass());
	NotifyManager = NewObject<UNotifyManager>((UObject*)GetTransientPackage(), UNotifyManager::StaticClass());
	PacketErrorManager = NewObject<UPacketErrorManager>((UObject*)GetTransientPackage(), UPacketErrorManager::StaticClass());
	//Game Option
	OptionManager = NewObject< UGameOptionManager>((UObject*)GetTransientPackage(), UGameOptionManager::StaticClass());
	// because Init event is called in Super::Init()
	GameDataManager = NewObject< UGameDataManager>((UObject*)GetTransientPackage(), UGameDataManager::StaticClass());
	AutoPlayManager = NewObject< UAutoPlayManager>((UObject*)GetTransientPackage(), UAutoPlayManager::StaticClass());
	GridMapManager = NewObject< UGridMapManager>((UObject*)GetTransientPackage(), UGridMapManager::StaticClass());
	
	// Instantiate BillingManager only if the platform supports in-app billing
	//BillingManager = NewObject< UBillingManager >((UObject*)GetTransientPackage(), UBillingManager::StaticClass());
	/*if (FPlatformAuth::QueryBillingSupport())
	{
		BillingManager = NewObject< UBillingManager >((UObject*)GetTransientPackage(), UBillingManager::StaticClass());
	}
	else
	{
		BillingManager = nullptr;
	}*/
	ContentsUnLockManager = NewObject< UContentsUnLockManager>((UObject*)GetTransientPackage(), UContentsUnLockManager::StaticClass());
	NotifyEventList = NewObject< UNotifyEventList>((UObject*)GetTransientPackage(), UNotifyEventList::StaticClass());
//	TooltipEventList = NewObject< UTooltipEventList>((UObject*)GetTransientPackage(), UTooltipEventList::StaticClass());
	UIMediaFrameWorkManager = NewObject< UUIMediaFrameWorkManager>((UObject*)GetTransientPackage(), UUIMediaFrameWorkManager::StaticClass());
	AttendanceManager = NewObject< UAttendanceManager>((UObject*)GetTransientPackage(), UAttendanceManager::StaticClass());
	ChatManager = NewObject< UChatManager>((UObject*)GetTransientPackage(), UChatManager::StaticClass());
	ForceTutorialManager = NewObject<UForceTutorialManager>((UObject*)GetTransientPackage(), UForceTutorialManager::StaticClass());
	ResourceCacheManager = NewObject<UResourceCacheManager>((UObject*)GetTransientPackage(), UResourceCacheManager::StaticClass());
	

#if USE_CAFESDK_PLUGIN
	CafeSDKPluginManager = NewObject< UCafeSDKPluginManager>((UObject*)GetTransientPackage(), UCafeSDKPluginManager::StaticClass());
#endif
	Super::Init();
	OnInitComplete();
#if WITH_EDITOR
	// Notify RAuth of Editor Release Level configuration
	URAuth::GetInstancePtr()->OverrideEnv(GameEnvironmentVar.ReleaseLevel);
#endif
	
#if LOG_TIME
	double EndTime = FPlatformTime::Seconds();
	double PrintTime = EndTime - StartTime;
	UE_LOG(LogGamePlay, Log, TEXT("%f ____URGameInstance::Init()"), PrintTime);
#endif
}

void URGameInstance::Shutdown()
{
	//URAuth::GetInstancePtr()->InvalidatePrelogin();
	if (IsValid(BillingManager))
	{
		BillingManager->Shutdown();
	}

	if (IsValid(ColosseumManager))
	{
		ColosseumManager->ShutDown();
	}

	if (RGameUserSetting)
	{
		RGameUserSetting->SetIsEditorState(true);
	}

	UUpdateService::ReleaseSingleton();
	URAuth::ReleaseSingleton();
	URAsyncLoader::ReleaseSingleton();
	ULocalStringTableManager::ReleaseSingleton();
	UTableManager::ReleaseSingleton();
	USoundManager::ReleaseSingleton();
	//UJsonManager::GetInstancePtr()->ReleaseSingleton();
	UXmlManager::ReleaseSingleton();

	URTSManager::ReleaseSingleton();

	for (auto StateItor = GameStates.CreateConstIterator(); StateItor; ++StateItor)
	{
		UIGameState* State = StateItor->Value;
		State->RemoveFromRoot();
	}
	GameStates.Empty();

	Super::Shutdown();
}

void URGameInstance::SetupForFirstExecute()
{
// Set RGameUserSetting->LaguageType to system default
// 	Culture Name - ko - KR
// 	Culture DisplayName - ?œêµ­???€?œë?êµ?
// 	Culture EnglishName - Korean(South Korea)
// 	Culture Keyboard layout id - 0
// 	Culture LC id - 1042
// 	Culture Native Language - ?œêµ­??
// 	Culture Native Name - ?œêµ­???€?œë?êµ?
// 	Culture Native Reigion - ?€?œë?êµ?
// 	Culture Reigion - KR
	FCulturePtr Culture = FInternationalization::Get().GetCurrentCulture();

	int LCID = Culture->GetLCID();

	FString TwoLetterISOLanguageName = Culture->GetTwoLetterISOLanguageName();
	FString ThreeLetterISOLanguageName = Culture->GetThreeLetterISOLanguageName();
	UE_LOG(LogGamePlay, Log, TEXT("SetupForFirstExecute: LCID: %04x ISO2: %s ISO3: %s"), LCID, *TwoLetterISOLanguageName, *ThreeLetterISOLanguageName);
	UE_LOG(LogGamePlay, Log, TEXT(" Culture Name - %s "), *(Culture->GetName()));
	UE_LOG(LogGamePlay, Log, TEXT(" Culture DisplayName - %s "), *(Culture->GetDisplayName()));
	//UE_LOG(LogGamePlay, Log, TEXT(" Culture CanonicalName - %s "), *(CultureRef->GetCanonicalName()));
	UE_LOG(LogGamePlay, Log, TEXT(" Culture EnglishName - %s "), *(Culture->GetEnglishName()));
	UE_LOG(LogGamePlay, Log, TEXT(" Culture Keyboard layout id - %d "), Culture->GetKeyboardLayoutId());
	UE_LOG(LogGamePlay, Log, TEXT(" Culture LC id - %d "), Culture->GetLCID());
	UE_LOG(LogGamePlay, Log, TEXT(" Culture Native Language - %s "), *(Culture->GetNativeLanguage()));
	UE_LOG(LogGamePlay, Log, TEXT(" Culture Native Name - %s "), *(Culture->GetNativeName()));
	UE_LOG(LogGamePlay, Log, TEXT(" Culture Native Reigion - %s "), *(Culture->GetNativeRegion()));
	UE_LOG(LogGamePlay, Log, TEXT(" Culture Reigion - %s "), *(Culture->GetRegion()));

	// https://msdn.microsoft.com/en-us/library/ee825488(v=cs.20).aspx
	static const int KO_KR = 0x0412;
	static const int ZH_CN = 0x0804; // Chinese - China
	static const int ZH_HK = 0x0c04; // Chinese - Hong Kong SAR
	static const int ZH_MO = 0x1404; // Chinese - Macau SAR
	static const int ZH_SG = 0x1004; // Chinese - Singapore
	static const int ZH_TW = 0x0404; // Chinese - Taiwan
	static const int ZH_CHS = 0x0004; // Chinese (Simplified)
	static const int ZH_CHT = 0x7c04; // Chinese (Traditional)

	ELanguageType LanguageType = ELanguageType::VE_ENG;

	switch (LCID)
	{
		case KO_KR:
			LanguageType = ELanguageType::VE_KOR;
			break;

		case ZH_CN:
		case ZH_SG:
		case ZH_CHS:
			LanguageType = ELanguageType::VE_CHN;
			break;

		case ZH_TW:
		case ZH_HK:
		case ZH_MO:
		case ZH_CHT:
			LanguageType = ELanguageType::VE_TWN;
			break;
	}

	if (IsValid(RGameUserSetting))
	{
		RGameUserSetting->LaguageType = LanguageType;
	}
}

void URGameInstance::EtcSetting()
{
	// Operations on client first launch
	{
		UE_LOG(LogGamePlay, Log, TEXT("DefaultLocale: %s"), *FPlatformMisc::GetDefaultLocale());
		UE_LOG(LogGamePlay, Log, TEXT("CurrentLanguage: %s"), *FInternationalization::Get().GetCurrentLanguage()->GetName());
		UE_LOG(LogGamePlay, Log, TEXT("CurrentLocale: %s"), *FInternationalization::Get().GetCurrentLocale()->GetName());

		// Android Culture issue fix from https://udn.unrealengine.com/questions/361333/finternationalizationgetgetcurrentculture-issue-on.html
#if PLATFORM_ANDROID
		FInternationalization::Get().SetCurrentLanguageAndLocale(FAndroidMisc::GetOSLanguage());
#endif

		UE_LOG(LogGamePlay, Log, TEXT("DefaultLocale: %s"), *FPlatformMisc::GetDefaultLocale());
		UE_LOG(LogGamePlay, Log, TEXT("CurrentLanguage: %s"), *FInternationalization::Get().GetCurrentLanguage()->GetName());
		UE_LOG(LogGamePlay, Log, TEXT("CurrentLocale: %s"), *FInternationalization::Get().GetCurrentLocale()->GetName());

#if WITH_EDITOR
		// force load in order to discard cached settings in Editor
		//RGameUserSetting->LoadSettings(true);
#endif
		
		if (RGameUserSetting->FirstExecute)
		{
			SetupForFirstExecute();
			RGameUserSetting->FirstExecute = false;
			//RGameUserSetting->SaveSettings();
		}

		// Apply user language setting to engine

		FString CultureName = TEXT("en"); // Fallback language is en
		switch (RGameUserSetting->LaguageType)
		{
			case ELanguageType::VE_KOR:
				CultureName = TEXT("ko");
			break;

			case ELanguageType::VE_ENG:
				CultureName = TEXT("en");
			break;

			case ELanguageType::VE_CHN:
				CultureName = TEXT("zh_Hans");
			break;

			case ELanguageType::VE_TWN:
				CultureName = TEXT("zh_Hant");
			break;
			default:
				//CultureName = FInternationalization::Get().GetCurrentCulture()->GetName();
			break;
		}

		UE_LOG(LogGamePlay, Log, TEXT("SetCurrentCulture(%s)"), *CultureName);
		
		
#if WITH_EDITOR
		//const FString PreviewGameLanguage = 
		FTextLocalizationManager::Get().ConfigureGameLocalizationPreviewLanguage(CultureName);
		FTextLocalizationManager::Get().EnableGameLocalizationPreview();
#else
		FInternationalization::Get().SetCurrentCulture(CultureName);
		if (!FInternationalization::Get().SetCurrentLanguageAndLocale(CultureName))
		{
			UE_LOG(LogGamePlay, Log, TEXT("SetCurrentLanguageAndLocale(%s) returned false"), *CultureName);
		}

		UE_LOG(LogGamePlay, Log, TEXT("DefaultLocale: %s"), *FPlatformMisc::GetDefaultLocale());
		UE_LOG(LogGamePlay, Log, TEXT("CurrentLanguage: %s"), *FInternationalization::Get().GetCurrentLanguage()->GetName());
		UE_LOG(LogGamePlay, Log, TEXT("CurrentLocale: %s"), *FInternationalization::Get().GetCurrentLocale()->GetName());
#endif
		
	}
}

// Init before login
void URGameInstance::InitBeforeLaunch()
{
	if (IsAlreadyInitBeforeLaunch)
		return;

	LoadingSceneManager->Initialize();
	EventManager->Initialize();
	PacketErrorManager->Initialize();
	OptionManager->Initialize();
	RWidgetManager->Initialize();
	SequenceManager->Initialize();
//	SoundManager->Initialize();
	IsAlreadyInitBeforeLaunch = true;
}

void URGameInstance::InitAfterLaunch()
{
	if (RGAMEINSTANCE(this) == nullptr)
		return;
	if (IsAlreadyInitAfterLaunch)
		return;
	IsAlreadyInitAfterLaunch = true;
	ULocalStringTableManager::GetInstancePtr();
	UTableManager::GetInstancePtr();
	GameDataManager->Initialize();
	ForceTutorialManager->Initialize();
	

	ArenaManager->Initialize();
	ColosseumManager->Initialize();
	MirrorDungeonManager->Initialize();
	MissionManager->Initialize();

	NetSkillManager->Init(this);
	RealTimeModManager->Initialize();
	BattleRecordManager->Initialize();
	PartyManager->Initialize();
	PlayModeManager->Initialize();
	CampaignManager->Initialize();
	HeroRuneDungeonManager->Initialize();

	AchievementManager->Initialize();
	MailManager->Initialize();
	QuestManager->Initialize();
	NotifyManager->Initialize();
	RaidManager->Initialize();
	AbnormalStateGroupTable_V2->Init();
	ServerInfoUpdateManager->Initialize();

	URenewal_BattleFunctionLibrary::InitBattleLogicConstants(BattleLogicConstants);
	InputManager->Initialize(this);

	UserInfo->Initialize();
	GuildInfo->Initialize();
	RewardInfo->Init();
	PlayStateInfo->Init();
	RInventory->Clear();
	ObjectPoolManager->Initialize();

	AutoPlayManager->Initialize();
	GridMapManager->Initialize();

	ShopManager->Initialize();
	if (IsValid(BillingManager))
	{
		BillingManager->Initialize();
	}

	ContentsUnLockManager->Initialize();
	NotifyEventList->Initialize();
// #if SHOW_TOOLTIP == 1
// 	TooltipEventList->Initialize();
// #endif
	UIMediaFrameWorkManager->Initialize();

	AttendanceManager->Initialize();

	ChatManager->Initialize(RGAMEINSTANCE(this));
// 	RealTimeModManager->Connect(this, URAuth::GetInstancePtr()->ReleaseTarget.TcpServerHost, URAuth::GetInstancePtr()->ReleaseTarget.TcpServerPort);
// 	UE_LOG(LogRTS, Log, TEXT("RTS>> [URealTimeModManager] SetTimerRemainGame >> TcpServerHost %s TcpServerPort %s"), *URAuth::GetInstancePtr()->ReleaseTarget.TcpServerHost, *URAuth::GetInstancePtr()->ReleaseTarget.TcpServerPort);

	//////////////////////////////////////////////////////////////////////////
	UKismetSystemLibrary::ControlScreensaver(false);
	//todo qsmname - ???
	CaptureBlueprintActor = LoadObject<UClass>(this, TEXT("Blueprint'/Game/UMG/Materials/BaseMat/Capture_ParticleActor.Capture_ParticleActor_C'"));
	CaptureBlueprint = LoadObject<UClass>(this, TEXT("Blueprint'/Game/UMG/Materials/BaseMat/Capture_Particle.Capture_Particle_C'"));

	BattleInfo = NewObject< UCharacterBattleInfo >();

#if USE_CAFESDK_PLUGIN
	CafeSDKPluginManager->Initialize(RGAMEINSTANCE(this));
#endif
}

/*
Game states
*/
void URGameInstance::ChangeState(EGameStateEnum Next, bool bForce)
{
	if (!bForce)
	{
		if (CurrentState && CurrentState->GetStateType() == Next)
			return;
	}

	NextState = Next;

	SetState(NextState, bForce);
	//UTableManager::GetInstancePtr()->UnMemAllTables();
}

EGameStateEnum URGameInstance::GetCurrentStateType()
{
	if (CurrentState)
	{
		return CurrentState->GetStateType();
	}

	return EGameStateEnum::VE_None;
}

bool URGameInstance::IsAutoPlayMode()
{
	if (IsValid(CampaignManager))
	{
		return CampaignManager->IsAutoPlay();
	}
	
	return false;
}

void URGameInstance::SetAutoPlayCount(int32 InCount)
{
	if (IsValid(CampaignManager))
	{
		CampaignManager->SetAutoPlayCount(InCount);
	}
}



void URGameInstance::InitGameStates()
{
	GameStates.Empty(3);

	GameStates.Emplace(EGameStateEnum::VE_Lobby, NewObject< UTowerLobbyState >((UObject*)GetTransientPackage(), UTowerLobbyState::StaticClass()));
	GameStates.Emplace(EGameStateEnum::VE_Play, NewObject< UPlayState >((UObject*)GetTransientPackage(), UPlayState::StaticClass()));
	GameStates.Emplace(EGameStateEnum::VE_Tutorial, NewObject< UTutorialState >((UObject*)GetTransientPackage(), UTutorialState::StaticClass()));

	for (auto StateItor = GameStates.CreateConstIterator(); StateItor; ++StateItor)
	{
		UIGameState* State = StateItor->Value;
		State->SetGameInstance(this);
		State->AddToRoot();
	}

	//SetState(EGameStateEnum::VE_Login);
}

void URGameInstance::SetState(EGameStateEnum NewState, bool bForce)
{
	if (!bForce)
	{
		if (CurState == NewState)
			return;
	}

	if (CurrentState)
	{
		CurrentState->Leave();
		CurrentState = nullptr;

		CameraActor = nullptr;
	}

	UIGameState* NewGameState = nullptr;
	if (GameStates.Contains(NewState))
	{
		CurState = NewState;
		NewGameState = GameStates[NewState];
		if (NewGameState)
			NewGameState->Enter();
	}

	CurrentState = NewGameState;
}

void URGameInstance::CheckNextState()
{
	if (NextState != EGameStateEnum::VE_None)
		SetState(NextState);

	NextState = EGameStateEnum::VE_None;
}


/*
Load Level
*/

void URGameInstance::CurrentStateBeginState()
{
	if (IsValid(CurrentState))
	{
		CurrentState->BeginState();
	}
}

/*
EditorManager
*/

void URGameInstance::CreateEditorManager(EEditorEnum editorType)
{
#if WITH_EDITOR
	if (EditorManager != nullptr)
		return;

	EditorManager = NewObject<UEditorManager>((UObject*)GetTransientPackage(), UEditorManager::StaticClass());
	if (IsValid(EditorManager))
	{
		EditorManager->Initialize(editorType);
	}
#endif
}

void URGameInstance::DestoryEditorManager()
{
#if WITH_EDITOR
	if (!EditorManager)
		return;

	if (!EditorManager->IsValidLowLevel())
		return;

	EditorManager->ConditionalBeginDestroy();
	EditorManager = nullptr;
#endif
}

void URGameInstance::EditorRefresh()
{
#if WITH_EDITOR
	if (!EditorManager)
		return;

	if (!EditorManager->IsValidLowLevel())
		return;

	EditorManager->RefreshState();
#endif
}

ATowerLobbyGameMode* URGameInstance::GetTowerLobbyGameMode()
{
	UTowerLobbyState* LobbyTowerState = Cast<UTowerLobbyState>(CurrentState);
	if (!IsValid(LobbyTowerState))
	{
		UE_LOG(LogLobby, Log, TEXT("URGameInstance::GetTowerLobbyGameMode: called while not in LobbyTowerState"));
		return nullptr;
	}

	ATowerLobbyGameMode* GameMode = LobbyTowerState->GetTowerLobbyGameMode();
	if (!IsValid(GameMode))
	{
		UE_LOG(LogLobby, Log, TEXT("URGameInstance::GetTowerLobbyGameMode: failed to get game mode"));
		return nullptr;
	}

	return GameMode;
}

ERaidDayType URGameInstance::GetRaidDayType()
{
	EDayOfWeek eDay = ServerDateTime.Now().GetDayOfWeek();
	ERaidDayType eRaidDayType = ERaidDayType::WEEK;

	switch (eDay)
	{
	case EDayOfWeek::Monday:
		eRaidDayType = ERaidDayType::MON;
		break;
	case EDayOfWeek::Tuesday:
		eRaidDayType = ERaidDayType::TUE;
		break;
	case EDayOfWeek::Wednesday:
		eRaidDayType = ERaidDayType::WED;
		break;
	case EDayOfWeek::Thursday:
		eRaidDayType = ERaidDayType::THU;
		break;
	case EDayOfWeek::Friday:
		eRaidDayType = ERaidDayType::FRI;
		break;
	case EDayOfWeek::Saturday:
		eRaidDayType = ERaidDayType::SAT;
		break;
	case EDayOfWeek::Sunday:
		eRaidDayType = ERaidDayType::SUN;
		break;
	default:
		eRaidDayType = ERaidDayType::WEEK;
		break;
	}

	return eRaidDayType;
}

void URGameInstance::Logout()
{
	if (CurrentState)
	{
		CurrentState->Refresh();
	}

	SetState(EGameStateEnum::VE_None, true);
	CurState = EGameStateEnum::VE_None;


	if (RealTimeModManager)
	{
		RealTimeModManager->ClearRealTimeModData();
		RealTimeModManager->DisConnect();
	}
	URTSManager::GetInstancePtr()->Disconnect();
	URAuth::GetInstancePtr()->Logout();
	

	OptionManager->ApplySetting();
	EtcSetting();
	/* loginrp
	instance->ContentsUnLockManager->SetUnlockList(avatar_game_data_rp.unlockList);
	instance->HttpClient->SetKID(avatar_game_data_rp.kId);
	instance->UserInfo->kId = avatar_game_data_rp.kId;
	instance->UserInfo->SetAvatarData(avatar_game_data_rp.avatar);
	instance->GuildInfo->SetGuildMine(avatar_game_data_rp.guild_mine);
	instance->AttendanceManager->SetAttendanceInfo(avatar_game_data_rp.chart);
	instance->AchievementManager->SetAchievementStatus(avatar_game_data_rp.achieveStatus);
	//instance->ForceTutorialManager->nCurrentTutorialStep = avatar_game_data_rp.inventory.tut;
	instance->RInventory->Clear();
	instance->RInventory->InitInventory(avatar_game_data_rp.inventory);
	instance->QuestManager->SetDailyQuestStatus(avatar_game_data_rp.dailyQuestStatus);
	instance->ContentsUnLockManager->SetFirstEnterArr(avatar_game_data_rp.selectiveTutorial.flags);
	instance->CampaignManager->SetTowerStatus(avatar_game_data_rp.towerStatus);
	instance->MissionManager->SetMissionStatus(avatar_game_data_rp.towerStatus.floorMissionStatus);
	instance->ColosseumManager->ColosseumStatus.gradeId = avatar_game_data_rp.colosseumGradeId;*/
}

URSaveGame* URGameInstance::GetSaveGame()
{
	if (SaveGame)
	{
		return SaveGame;
	}

	SaveGame = Cast<URSaveGame>(UGameplayStatics::CreateSaveGameObject(URSaveGame::StaticClass()));
	bool bIsSaveFile = UGameplayStatics::DoesSaveGameExist(SaveGame->SaveSlotName, SaveGame->UserIndex);
	if (bIsSaveFile)
	{
		SaveGame = SaveGame->Load();
	}
#if LOG_FILE
	FString path = FString::Printf(TEXT("%sSaveGames/save.sav"), *FPaths::ProjectSavedDir());
	if (bIsSaveFile)
	{
		UE_LOG(LogGamePlay, Log, TEXT("Load ____URGameInstance::GetSaveGame(%s)"), *path);
	}
	else
	{
		UE_LOG(LogGamePlay, Log, TEXT("Empty ____URGameInstance::GetSaveGame(%s)"), *path);
	}
#endif
	
	return SaveGame;
}
