// Fill out your copyright notice in the Description page of Project Settings.

#include "ProjectR.h"
#include "ConsoleCheatManager.h"
#include "SharedConstants/GlobalConstants.h"
#include "GlobalIntegrated.h"

#include "Character/AnimNotification/SkillFireNotify.h"


#include "GameInfo/RInventory.h"
#include "UI/RWidgetManager.h"
#include "Ui/Common/UP_ControlBlock.h"
#include "TutorialLevelScriptActor.h"
#include "Character/Components/FunctionalComponents/BattleEntityComponent.h"
#include "Character/Components/FunctionalComponents/SkillEntityComponent.h"
#include "Character/Components/FunctionalComponents/EntityAIComponent.h"


// #include "Utils/GamePlayFunctionLibrary.h"

#include "ContentGameMode/Modes/TestGameMode.h"
#include "ContentGameMode/Controllers/BasePlayerController.h"
#include "ContentGameMode/Controllers/LobbyPlayerController.h"
#include "ContentGameMode/Controllers/StagePlayerController.h"
#include "NotifyList/NotifyEventList.h"

#include "Runtime/UMG/Public/Blueprint/WidgetLayoutLibrary.h"

#include "LeaderControlHelper.h"
#include "UI/UI_Battle/RSkillIcon.h"
#include "UI/UI_Battle/RCampaignHUD_ThreeControl.h"

#include "ProjectRGameMode.h"
#include "Gameinfo/PlayStateInfo.h"

#include "ContentGameMode/Modes/TowerLobbyGameMode.h"
#include "Lobby/IndoorLobbyStateActor.h"
#include "Network/PacketFunctionLibrary.h"

#include "UI/Common/UC_ItemIcon.h"

#include "UtilFunctionIntegrated.h"

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Example
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

UConsoleCheatManager::UConsoleCheatManager()
{
	
	//////////////////////////////////////////////////////////////////////////
	// 1. Engine Initialize Create
	/*
	// 1-1 ConsoleVariable
	static TAutoConsoleVariable<int32> CVarCheatManagerConsoleVariable(TEXT("CM_ConsoleVariableStatic"),
		0,
		TEXT("This is CheatManager ConsoleVariableStatic.\n")
		TEXT("<=0: Zero (default)\n")
		TEXT("  1: one \n")
		TEXT("  2: two"),
		ECVF_Default);

	IConsoleManager::Get().RegisterConsoleVariable(TEXT("CM_ConsoleVariableRegister"),
		0,
		TEXT("This is CheatManager ConsoleVariableRegister.\n")
		TEXT("<=0: Zero (default)\n")
		TEXT("  1: one \n")
		TEXT("  2: two"),
		ECVF_Default);

	// 1-2 ConsoleCommand Regist
	static FAutoConsoleCommand FCheatManagerConsoleCommandDelegate(TEXT("CM_ConsoleCommandStatic"),
		TEXT("This is CheatManager ConsoleCommandStatic."),
		FConsoleCommandDelegate::CreateStatic(&CM_ConsoleCommandStatic),
		ECVF_Default);

	IConsoleManager::Get().RegisterConsoleCommand(TEXT("CM_ConsoleCommandDelegate"),
		TEXT("This is CheatManager ConsoleCommandDelegate.\n")
		TEXT("Register a console command that takes no arguments."),
		FConsoleCommandDelegate::CreateStatic(&CM_ConsoleCommandDelegate),
		ECVF_Default);

	IConsoleManager::Get().RegisterConsoleCommand(TEXT("CM_ConsoleCommandWithArgsDelegate"),
		TEXT("This is CheatManager ConsoleCommandWithArgsDelegate.\n")
		TEXT("Register a console command that takes arguments.\n")
		TEXT("const TArray< FString >& params"),
		FConsoleCommandWithArgsDelegate::CreateStatic(&CM_ConsoleCommandWithArgsDelegate),
		ECVF_Default);
	*/

	
}
/*
void UConsoleCheatManager::CM_ConsoleCommandStatic()
{
	UE_LOG(LogGamePlay, Warning, TEXT("EXEC CM_ConsoleCommandStatic"));
}

void UConsoleCheatManager::CM_ConsoleCommandDelegate()
{
	UE_LOG(LogGamePlay, Warning, TEXT("EXEC CM_ConsoleCommandDelegate"));
}

void UConsoleCheatManager::CM_ConsoleCommandWithArgsDelegate(const TArray< FString >& params)
{
	UE_LOG(LogGamePlay, Warning, TEXT("EXEC CM_ConsoleCommandWithArgsDelegate"));

	for (auto It = params.CreateConstIterator(); It; ++It)
	{
		UE_LOG(LogGamePlay, Warning, TEXT("Args %s"), *(*It));
	}
}
*/
//////////////////////////////////////////////////////////////////////////
// 2. EXEC Function
/*
void UConsoleCheatManager::CM_ConsoleCommandExecFunction()
{
	UE_LOG(LogGamePlay, Warning, TEXT("EXEC CM_ConsoleCommandExecFunction"));
}

void UConsoleCheatManager::CM_ConsoleCommandExecFunctionInt(int32 cheatInt)
{
	UE_LOG(LogGamePlay, Warning, TEXT("EXEC CM_ConsoleCommandExecFunctionInt %d"), cheatInt);
}

void UConsoleCheatManager::CM_ConsoleCommandExecFunctionFloat(float cheatFloat)
{
	UE_LOG(LogGamePlay, Warning, TEXT("EXEC CM_ConsoleCommandExecFunctionFloat %f"), cheatFloat);
}

void UConsoleCheatManager::CM_ConsoleCommandExecFunctionString(FString& cheatString)
{
	UE_LOG(LogGamePlay, Warning, TEXT("EXEC CM_ConsoleCommandExecFunctionString %s"), *cheatString);
}
*/

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////////
// SERVER CHEAT

void UConsoleCheatManager::ServerCheat(FString& cheatCommand)
{
	//bool bRes = ULegoNetwork::GetLegoNetWork()->RequestAdminCommand(cheatCommand);
	UE_LOG(LogGamePlay, Warning, TEXT("EXEC ServerCheat %s Result"), *cheatCommand);
}

//////////////////////////////////////////////////////////////////////////
// CLIENT CHEAT

void UConsoleCheatManager::VisibleUI()
{
	UE_LOG(LogGamePlay, Warning, TEXT("EXEC HideAllUI"));

	URGameInstance* GameInstance = RGAMEINSTANCE(this);

	bVisibleUI = !bVisibleUI;

	GameInstance->RWidgetManager->SetUIVisibility(bVisibleUI);

	ABasePlayerController* BasePlayerController = UUtilFunctionLibrary::GetBasePlayerController(GameInstance->GetWorld());
	if (IsValid(BasePlayerController))
	{
		if (bVisibleUI)
		{
			BasePlayerController->SetVirtualJoystickVisibility(true);
		}
		else
		{
			BasePlayerController->SetVirtualJoystickVisibility(false);
		}
	}
}

void UConsoleCheatManager::VisibleHPBar()
{
	bVisibleInGameHPBar = !bVisibleInGameHPBar;
}

void UConsoleCheatManager::VisibleUIAll()
{
	URGameInstance* GameInstance = RGAMEINSTANCE(this);
	if (false == IsValid(GameInstance))
		return;

	if (bVisibleInGameHPBar)
	{
		bVisibleInGameHPBar = false;
		GameInstance->RWidgetManager->SetUIVisibility(false);

		ABasePlayerController* BasePlayerController = UUtilFunctionLibrary::GetBasePlayerController(GameInstance->GetWorld());
		if (IsValid(BasePlayerController))
		{
			BasePlayerController->SetVirtualJoystickVisibility(false);
		}
	}
	else
	{
		bVisibleInGameHPBar = true;
		GameInstance->RWidgetManager->SetUIVisibility(true);

		ABasePlayerController* BasePlayerController = UUtilFunctionLibrary::GetBasePlayerController(GameInstance->GetWorld());
		if (IsValid(BasePlayerController))
		{
			BasePlayerController->SetVirtualJoystickVisibility(true);
		}
	}
}

void UConsoleCheatManager::CoolTimeZero()
{
	UE_LOG(LogGamePlay, Warning, TEXT("EXEC CoolTimeZero"));

	bCoolTimeZero = true;

	
}

void UConsoleCheatManager::SkillCostZero()
{
	bSkillCostZero = !bSkillCostZero;
}

void UConsoleCheatManager::SpawnObject(FName& CharacterID, int32 Level, ECharacterCategoryTypeEnum Category)
{
	URGameInstance* GameInstance = RGAMEINSTANCE(this);
	//if (IsValid(GameInstance))
	{
		ABasePlayerController* BasePlayerController = UUtilFunctionLibrary::GetBasePlayerController(GameInstance->GetWorld());
		if (IsValid(BasePlayerController))
		{
			AActor* SelectedCharacter = BasePlayerController->GetSelectedCharacter();
			if (IsValid(SelectedCharacter))
			{
// 				GameInstance->GameObjectMgr->SpawnCharacter(SelectedCharacter->GetActorTransform(), CharacterID, Category, 
// 											KID_NONE, FString(""), GroupKey_SINGLE_ENEMY, PartyID_NPC_ENEMY, PLAYER_UK_NONE,
// 											Level);
				UObjectFunctionLibrary::SpawnCharacter(SelectedCharacter->GetActorTransform(), CharacterID, Category,
											KID_NONE, FString(""), GroupKey_SINGLE_ENEMY, PartyID_NPC_ENEMY, PLAYER_UK_NONE,
											Level);
				
			}
		}
	}
}

void UConsoleCheatManager::CreateHero(FName& CharacterID, int32 Level)
{
	UE_LOG(LogGamePlay, Warning, TEXT("EXEC CreateCharacter %s %d"), *(CharacterID.ToString()), Level);
	SpawnObject(CharacterID, Level, ECharacterCategoryTypeEnum::VE_Hero);
}

void UConsoleCheatManager::CreateNPC(FName& CharacterID, int32 Level)
{
	UE_LOG(LogGamePlay, Warning, TEXT("EXEC CreateNPC %s %d"), *(CharacterID.ToString()), Level);
	SpawnObject(CharacterID, Level, ECharacterCategoryTypeEnum::VE_Npc);
}

void UConsoleCheatManager::CreateMinion(FName& CharacterID, int32 Level)
{
	UE_LOG(LogGamePlay, Warning, TEXT("EXEC CreateMinion %s %d"), *(CharacterID.ToString()), Level);
	SpawnObject(CharacterID, Level, ECharacterCategoryTypeEnum::VE_Minion);
}

void UConsoleCheatManager::CreateNeutralNpc(FName& CharacterID, int32 Level)
{
	UE_LOG(LogGamePlay, Warning, TEXT("EXEC CreateNeutralNpc %s %d"), *(CharacterID.ToString()), Level);
	SpawnObject(CharacterID, Level, ECharacterCategoryTypeEnum::VE_NeutralNpc);
}

void UConsoleCheatManager::SlotMode(int32 mode)
{
	UE_LOG(LogGamePlay, Warning, TEXT("EXEC SlotMode %d"), mode);
	URGameUserSetting* RGameUserSetting = URGameUserSetting::GetRGameUserSettings();
	if (IsValid(RGameUserSetting))
	{
		if (mode == 0)
		{
			RGameUserSetting->SlotAction = false;
		}
		else
		{
			RGameUserSetting->SlotAction = true;
		}
	}
}

void UConsoleCheatManager::StageClear(int32 state)
{
	bStageClear = true;
	StageClearState = state;

	UGamePlayBluePrintFunction::StageClearSuccess();
}

void UConsoleCheatManager::GetNewHero(FName& CharacterID)
{
// 	URGameInstance* GameInstance = RGAMEINSTANCE(this);
// 	//if (IsValid(GameInstance))
// 	{
// 		FHERO* FindHeroData = GameInstance->HeroInventory->GetInventoryDataWithID(CharacterID);
// 		if (FindHeroData != nullptr)
// 		{
// 			FindHeroData->activated = false;
// 			
// 			FHERO NewHeroData = *FindHeroData;
// 			NewHeroData.activated = true;
// 			GameInstance->HeroInventory->UpdateInventoryData(NewHeroData);
// 		}
// 		GameInstance->RWidgetManager->ShowNewHeroUI();
// 	}
}

void UConsoleCheatManager::Spawn()
{
	UWorld*					GameWorld = GEngine->GetWorldFromContextObject(RGAMEINSTANCE(this), EGetWorldErrorMode::Assert);
	checkf(GameWorld != nullptr, TEXT("UWorld is Empty"));

	FVector SpawnPos = FVector(0, 0, 0);
	ABaseCameraActor* Camera = nullptr;
	TArray<AActor*> arrayList;
	UGameplayStatics::GetAllActorsOfClass(RGAMEINSTANCE(this), ABaseCameraActor::StaticClass(), arrayList);
	for (AActor* actor : arrayList)
	{
		Camera = Cast<ABaseCameraActor>(actor);
	}

	if (Camera)
		SpawnPos = Camera->GetActorLocation();
	FHeroSlotInfo CharacterSlot;
	int32 Index = 0;
	FName CharacterRowID(TEXT("1"));
	FCharacterTableInfo* CharacterTableInfo = UTableManager::GetInstancePtr()->GetCharacterRow(CharacterRowID);

	CharacterSlot.HeroLevel = 10;
	CharacterSlot.HeroGrade = 1;
	CharacterSlot.PortraitTextureKey = CharacterTableInfo->PortraitTextureKey;
	CharacterSlot.HeroIDName = CharacterRowID;
	CharacterSlot.SlotSpawnState = EHeroSpawnStateEnum::VE_Ready;
// 		CharacterSlot.MaxSpawnCount		= CharacterTableInfo->SpawnCount;
// 		CharacterSlot.SpawnCount		= CharacterSlot.MaxSpawnCount;
	CharacterSlot.CharacterClassType = CharacterTableInfo->Class;
	CharacterSlot.CharacterCategoryType = ECharacterCategoryTypeEnum::VE_Hero;

	TArray<FHeroSlotInfo>& array = RGAMEINSTANCE(this)->PlayStateInfo->GetSelectedCharacterArray();
	array.Emplace(CharacterSlot);

	UUtilFunctionLibrary::FirstSpawnPos(GameWorld, SpawnPos);
}

void UConsoleCheatManager::ToggleIgnoreDebugger()
{
	extern CORE_API bool GIgnoreDebugger;
	
	GIgnoreDebugger = !GIgnoreDebugger;
	UE_LOG(LogGamePlay, Log, TEXT("Toggled GIgnoreDebugger to %d"), (int32)GIgnoreDebugger);
}

void UConsoleCheatManager::ForceLod(int32 LodModel)
{
	UE_LOG(LogGamePlay, Log, TEXT("ENTER ForceLod with %d"), LodModel);

	// Iterate over all static mesh components... then SetForcedLodModel
	for (TObjectIterator<UStaticMeshComponent> Itr; Itr; ++Itr)
	{
		// Access the subclass instance with the * or -> operators.
		UStaticMeshComponent *Component = *Itr;
		UE_LOG(LogGamePlay, Log, TEXT("Applying SetForcedLodModel to %s"), *Component->GetName());

		Component->SetForcedLodModel(LodModel);
	}
}

void UConsoleCheatManager::ConsoleCommand(const FString& Command, UWorld* _world)
{
	const TCHAR* Delims[] = { TEXT(" ") };
	FString strLower = Command.ToLower();
	TArray<FString> Parsed;
	strLower.ParseIntoArray(Parsed, Delims, 1);

	if (Parsed.Num() > 0)
	{
		FString strComp = Parsed[0];
#if WITH_EDITOR
		if (strComp == "globaltime")
		{
			if (Parsed.IsValidIndex(1))
			{
				float GlobalTime = FCString::Atof(*Parsed[1]);
				UGameplayStatics::SetGlobalTimeDilation(this, GlobalTime);
			}
		}
		else if (strComp == TEXT("EndTuto"))
		{
			UUtilFunctionLibrary::GetTutorialLevelScriptActor()->EndTutorial();
		}
		else if (strComp == "Test")
		{
			if (Parsed.IsValidIndex(1))
			{
				if (Parsed[1] == "NonTime")
				{
					ABasePlayerController* BasePC = UUtilFunctionLibrary::GetBasePlayerController(_world);
					if (!IsValid(BasePC))
						return;

					BasePC->SetIsGameTimerPause(!BasePC->GetIsGameTimerPause());
					UGamePlayFunctionLibrary::PauseInGamePlayTimer(BasePC->GetIsGameTimerPause());
				}
			}
		}
		else if (strComp == "Capture")
		{
			UClass* CaptureBlueprint = LoadObject<UClass>(this, TEXT("Blueprint'/Game/UMG/Materials/BaseMat/Capture_Scene.Capture_Scene_C'"));

			APlayerCameraManager* PCM = UGameplayStatics::GetPlayerCameraManager(RGAMEINSTANCE(this)->GetWorld(), 0);
			AActor* CamActor = PCM->GetViewTarget();


			/*FTransform Transform;
			Transform.SetTranslation(FVector(480.f, 1100.f, 0.f));
			Transform.SetRotation(FQuat(FRotator(0.f, -180.f, 0.f)));
			Transform.SetScale3D(FVector(1.f, 1.f, 1.f));*/

			RGAMEINSTANCE(this)->GetWorld()->SpawnActor<AActor>(CaptureBlueprint, CamActor->GetActorTransform());
			TargetTexture->SetVisibility(ESlateVisibility::HitTestInvisible);
			
		}
		else if (strComp == "Pointlight")
		{
			if (RGAMEINSTANCE(this)->GetWorld())
			{
				AStagePlayerController* BasePC = Cast<AStagePlayerController>(UGameplayStatics::GetPlayerController(RGAMEINSTANCE(this)->GetWorld(), 0));
				if (BasePC && BasePC->GetLeaderControlHelper())
				{
					FVector Location = BasePC->GetLeaderControlHelper()->LeaderCharacter.Get()->GetActorLocation();
					if (Parsed.IsValidIndex(1))
					{
						int32 CreateCount = FCString::Atoi(*Parsed[1]);

						for (int32 i = 0; i < CreateCount; ++i)
							RGAMEINSTANCE(this)->PointLightManager->AddPointLight(Location, 0.5f, 750.f, FLinearColor::Red, 200.f);
						
					}
				}
				
			}
		}
		else if (strComp == "dmg")
		{
			

			if (Parsed.IsValidIndex(1))
			{
				TArray<FHeroSlotInfo>& heroinfo = RGAMEINSTANCE(this)->PlayStateInfo->GetSelectedCharacterArray();
				if (Parsed[1].Contains(TEXT("leader")))
				{
					UBattleEntityComponent* BattleComp = UCharacterHelpFunctionLibrary::GetBattleEntityComponent(heroinfo[0].SpawnedTargetActor);
					BattleComp->SetHP(BattleComp->GetHP() * 0.5f);
				}
				else if (Parsed[1].Contains(TEXT("crew1")))
				{
					for (auto& Element : heroinfo)
					{
						if (Element.CharacterRole == ECharacterRoleEnum::VE_Crew1 && Element.CharacterCrewState == ECharacterCrewStateEnum::VE_Battle)
						{
							UBattleEntityComponent* BattleComp = UCharacterHelpFunctionLibrary::GetBattleEntityComponent(Element.SpawnedTargetActor);
							BattleComp->SetHP(BattleComp->GetHP() * 0.5f);
							break;
						}
					}
				}
				else if (Parsed[1].Contains(TEXT("crew2")))
				{
					for (auto& Element : heroinfo)
					{
						if (Element.CharacterRole == ECharacterRoleEnum::VE_Crew2 && Element.CharacterCrewState == ECharacterCrewStateEnum::VE_Battle)
						{
							UBattleEntityComponent* BattleComp = UCharacterHelpFunctionLibrary::GetBattleEntityComponent(Element.SpawnedTargetActor);
							BattleComp->SetHP(BattleComp->GetHP() * 0.5f);
							break;
						}
					}
				}
			}
		}
		else if (strComp == "heal")
		{
			if (Parsed.IsValidIndex(1))
			{
				TArray<FHeroSlotInfo>& heroinfo = RGAMEINSTANCE(this)->PlayStateInfo->GetSelectedCharacterArray();
				if (Parsed[1].Contains(TEXT("leader")))
				{
					UBattleEntityComponent* BattleComp = UCharacterHelpFunctionLibrary::GetBattleEntityComponent(heroinfo[0].SpawnedTargetActor);
					BattleComp->SetHP(BattleComp->GetHP() * 1.5f);
				}
				else if (Parsed[1].Contains(TEXT("crew1")))
				{
					for (auto& Element : heroinfo)
					{
						if (Element.CharacterRole == ECharacterRoleEnum::VE_Crew1 && Element.CharacterCrewState == ECharacterCrewStateEnum::VE_Battle)
						{
							UBattleEntityComponent* BattleComp = UCharacterHelpFunctionLibrary::GetBattleEntityComponent(Element.SpawnedTargetActor);
							BattleComp->SetHP(BattleComp->GetHP() * 1.5f);
							break;
						}
					}
				}
				else if (Parsed[1].Contains(TEXT("crew2")))
				{
					for (auto& Element : heroinfo)
					{
						if (Element.CharacterRole == ECharacterRoleEnum::VE_Crew2 && Element.CharacterCrewState == ECharacterCrewStateEnum::VE_Battle)
						{
							UBattleEntityComponent* BattleComp = UCharacterHelpFunctionLibrary::GetBattleEntityComponent(Element.SpawnedTargetActor);
							BattleComp->SetHP(BattleComp->GetHP() * 1.5f);
							break;
						}
					}
				}
			}
		}
		else if (strComp == "Spawn")
		{
			Spawn();
		}
		else if (strComp == "TryLoadCount")
		{
			int32 TryCnt = 0;
			AProjectRGameMode* RGameMode = Cast<AProjectRGameMode>(UGameplayStatics::GetGameMode(RGAMEINSTANCE(GEngine)->GetWorld()));
			if (RGameMode)
				TryCnt = RGameMode->GetTryCntLoadSync();
			
//			GEngine->AddOnScreenDebugMessage(-1, 2, FColor::Red, FString::FromInt(TryCnt));
		}
		else if (strComp == "ShowDebugRT")
		{
			URCampaignHUD_ThreeControl* IngameHUD = Cast<URCampaignHUD_ThreeControl>(UUIFunctionLibrary::GetInGameHUD());
			if (IngameHUD)
			{
				IngameHUD->isDebugRenderTarget = !IngameHUD->isDebugRenderTarget;
				if (IngameHUD->isDebugRenderTarget)
					IngameHUD->Image_RT->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
				else
					IngameHUD->Image_RT->SetVisibility(ESlateVisibility::Collapsed);
			}		
		}
		else if (strComp == "ShowDebugMap")
		{
			TArray<AActor*> OutActors;
			UGameplayStatics::GetAllActorsOfClass(RGAMEINSTANCE(GEngine)->GetWorld(), AMapCreatorActor::StaticClass(), OutActors);
			if (OutActors.IsValidIndex(0))
			{
				AMapCreatorActor* MapCreator = Cast<AMapCreatorActor>(OutActors[0]);
				if (MapCreator)
					MapCreator->DrawDebugInfo();
			}
		}
		else if (strComp == "DebugLight")
		{
			URGameInstance* RGameInstance = RGAMEINSTANCE(GEngine);
			UWorld*			GameWorld = GEngine->GetWorldFromContextObject(RGameInstance, EGetWorldErrorMode::Assert);
			checkf(GameWorld != nullptr, TEXT("%s[UWorld is Empty]"), __FUNCTION__);

			for (auto& postProcessVolume : GameWorld->PostProcessVolumes)
			{
				APostProcessVolume* PPVolume = Cast<APostProcessVolume>(postProcessVolume);
				if (PPVolume)
				{
					if (Parsed.Num() < 5)
						return;

					FRotator Dir = FRotator(FCString::Atof(*Parsed[1]), FCString::Atof(*Parsed[2]), FCString::Atof(*Parsed[3]));
					float Intensity = FCString::Atof(*Parsed[4]);

					PPVolume->Settings.bOverride_DirectionalLightColorMovable = true;
					PPVolume->Settings.bOverride_DirectionalLightDirectionMovable = true;
					PPVolume->Settings.bOverride_DirectionalLightIntensityMovable = true;

					APlayerCameraManager* PCM = UGameplayStatics::GetPlayerCameraManager(PPVolume->GetWorld(), 0);

					PPVolume->Settings.DirectionalLightDirectionMovable = Dir;
					PPVolume->Settings.DirectionalLightIntensityMovable = Intensity;
					PPVolume->Settings.DirectionalLightColorMovable = FLinearColor(1.f, 1.f, 1.f);
				}
			}
		}
		else if (strComp == "ShowHero")
		{
			if (Parsed.IsValidIndex(1))
			{
				ATowerLobbyGameMode* TowerGameMode = Cast<ATowerLobbyGameMode>(UGameplayStatics::GetGameMode(RGAMEINSTANCE(GEngine)->GetWorld()));
				if (TowerGameMode)
				{
					AIndoorLobbyStateActor* IndoorLobbyStateActor = Cast<AIndoorLobbyStateActor>(TowerGameMode->GetLobbyStateActor(ETowerLobbyStateEnum::Indoor));
					if (IndoorLobbyStateActor)
					{
						IndoorLobbyStateActor->HeroNameOverride = FName(*Parsed[1]);
					}
				}
			}
		}
		else if (strComp == "UnlockPopupTest")
		{
			if (Parsed.IsValidIndex(1))
			{
				URGameInstance* RGameInstance = RGAMEINSTANCE(this);
				RGameInstance->ContentsUnLockManager->AddUnlockList(FCString::Atoi(*Parsed[1]));
				RGameInstance->ContentsUnLockManager->OnShowUnlockNotification();
			}
		}
#endif
	}

#if WITH_EDITOR

	ATestGameMode* pGameMode = Cast<ATestGameMode>(UGameplayStatics::GetGameMode(GetWorld()));
	if( IsValid(pGameMode))
	{
		pGameMode->ConsoleCommand(Command);
	}

	// 	if (IsValid(RGAMEINSTANCE(this)->ForceTutorialManager))
	// 	{
	// 		RGAMEINSTANCE(this)->ForceTutorialManager->ConsoleCommand(Command);
	// 	}
#endif



	//From LobbyPlayerController//

	ALobbyPlayerController* LobbyPC = UUtilFunctionLibrary::GetLobbyPlayerController(_world);
	if (!IsValid(LobbyPC))
		return;

	if (Parsed.Num() > 0)
	{
		FString strComp = Parsed[0];
		
		if (strComp == TEXT("testsplash"))
		{
			URGameInstance* gameInstance = RGAMEINSTANCE(this);
			if (IsValid(gameInstance) && IsValid(gameInstance->RWidgetManager))
			{
				if (Parsed.IsValidIndex(1))
				{
					int32 id = FCString::Atoi(*Parsed[1]);

					if (id == 0)
					{
						UUIFunctionLibrary::ShowCommonPopupHeroRuneSplash(TEXT("1"), FText::AsNumber(0), FText::AsNumber(1));
					}
					else
					{
						UUIFunctionLibrary::ShowCommonPopupSplash(FText::AsNumber(0), FText::AsNumber(1));
					}
				}
			}
		}
		else if (strComp == TEXT("aspect"))
		{
			if (Parsed.IsValidIndex(1))
			{
				float fRatio = FCString::Atof(*Parsed[1]);
				APlayerCameraManager* PCM = UGameplayStatics::GetPlayerCameraManager(RGAMEINSTANCE(GEngine)->GetWorld(), 0);
				if (PCM)
				{
					UCameraComponent* CamComp = PCM->ViewTarget.Target->FindComponentByClass<UCameraComponent>();
					CamComp->SetConstraintAspectRatio(true);
					CamComp->SetAspectRatio(fRatio);
				}
			}
		}
		else if (strComp == TEXT("t1"))
		{
			float scale = UWidgetLayoutLibrary::GetViewportScale(LobbyPC);
			if (IsValid(_world))
			{
				UGameViewportClient* ViewportClient = _world->GetGameViewport();
				if (IsValid(ViewportClient))
				{
					FVector2D ViewportSize;
					ViewportClient->GetViewportSize(ViewportSize);

					//UE_LOG(LogBattle, Log, TEXT("**** GetViewportScale %f [%f:%f]"), scale, ViewportSize.X, ViewportSize.Y);

					FString strText;
					strText = FString::Printf(TEXT("**** GetViewportScale %f [%f:%f]"), scale, ViewportSize.X, ViewportSize.Y);
					UKismetSystemLibrary::PrintString(_world, strText);
				}
			}
		}
		else if (strComp == TEXT("to"))
		{
			LobbyPC->SetbTickTest(true);
		}
		else if (strComp == TEXT("tc"))
		{
			LobbyPC->SetbTickTest(false);
		}
		else if (strComp == TEXT("option"))
		{
			URGameInstance* gameInstance = RGAMEINSTANCE(this);
			if (IsValid(gameInstance) && IsValid(gameInstance->RWidgetManager))
			{
				gameInstance->RWidgetManager->ShowUserWidgetAutoCreate(EWidgetBluePrintEnum::WBP_Setting);
			}
		}
		else if (strComp == TEXT("testunlock"))
		{
			// 			URGameInstance* gameInstance = RGAMEINSTANCE(this);
			// 			if (IsValid(gameInstance) && IsValid(gameInstance->ContentsUnLockManager) && IsValid(gameInstance->UserInfo))
			// 			{
			// 				//gameInstance->UserInfo->Avatar.unlocked.Add((int32)EOVERLORD::ARENA);
			// 				gameInstance->UserInfo->Avatar.unlocked.Add((int32)EOVERLORD::COLOSSEUM);
			// 				gameInstance->ContentsUnLockManager->SetContentsUnlockNotification();
			// 			}
		}
		else if (strComp == TEXT("t0"))
		{
			URGameInstance* gameInstance = RGAMEINSTANCE(this);
			if (IsValid(gameInstance) && IsValid(gameInstance->RWidgetManager))
			{
				UUP_ControlBlock* BlockUI = Cast<UUP_ControlBlock>(gameInstance->RWidgetManager->GetUserWidget(EWidgetBluePrintEnum::WBP_ControlBlock));
				if (IsValid(BlockUI))
				{
					//BlockUI->FadeIn(1.0f);
					//BlockUI->FadeOut(1.0f);
					BlockUI->StartAni(2.0f);
					//BlockUI->NextAni(2.0f);
				}
			}
		}
		else if (strComp == TEXT("ttt"))
		{
			URGameInstance* gameInstance = RGAMEINSTANCE(this);
			if (IsValid(gameInstance) && IsValid(gameInstance->RWidgetManager))
			{
				gameInstance->RWidgetManager->ShowUserWidgetAutoCreate(EWidgetBluePrintEnum::WBP_ControlBlock);

				UUP_ControlBlock* BlockUI = Cast<UUP_ControlBlock>(gameInstance->RWidgetManager->GetUserWidget(EWidgetBluePrintEnum::WBP_ControlBlock));
				if (IsValid(BlockUI))
				{
				
					BlockUI->SetPosition(BlockUI->ScreenSize.X * 0.5f, BlockUI->ScreenSize.Y * 0.5f, 0, 0);
				}
			}
		}
		else if (strComp == TEXT("t7"))
		{
			
		}
		else if (strComp == TEXT("testsplashap"))
		{
			URGameInstance* gameInstance = RGAMEINSTANCE(this);
			if (IsValid(gameInstance) && IsValid(gameInstance->RInventory) && IsValid(gameInstance->RWidgetManager) && IsValid(gameInstance->NotifyEventList))
			{
				FName id = FName(*FString::FromInt(1));
				int32 current = 0;
				int32 prev = 0;


				if (Parsed.IsValidIndex(1))
				{
					id = FName(*Parsed[1]);
				}
				if (Parsed.IsValidIndex(2))
				{
					current = FCString::Atoi(*Parsed[2]);
				}
				if (Parsed.IsValidIndex(3))
				{
					prev = FCString::Atoi(*Parsed[3]);
				}

				FHERO* outHero = gameInstance->RInventory->GetHeroWithID(id);
				if (outHero != nullptr)
				{
					UUIFunctionLibrary::ShowChangeAttackPowerSplash(outHero->heroId, current, prev);
				}

			}
		}
		else if (strComp == TEXT("testlevelup"))
		{
			URGameInstance* gameInstance = RGAMEINSTANCE(this);
			if (IsValid(gameInstance) && IsValid(gameInstance->RWidgetManager))
			{
			}
		}
		else if (strComp == TEXT("crusader"))
		{
			UUIFunctionLibrary::GoContents(EUIMenuIDEnum::VE_Crusader);
		}
		else if (strComp == TEXT("testindicator"))
		{
			URGameInstance* gameInstance = RGAMEINSTANCE(this);
			//if (IsValid(GameInstance))
			{
				FName text = FName(*FString::FromInt(1));

				if (Parsed.IsValidIndex(1))
				{
					text = FName(*Parsed[1]);
				}

				UUtilFunctionLibrary::ShowIndicator(UUtilFunctionLibrary::GetLocalizedString(EStringTableType::VE_Dialogue, text));
			}
		}
		else if (strComp == TEXT("tutorialtest"))
		{
			URGameInstance* gameInstance = RGAMEINSTANCE(this);
			//if (IsValid(GameInstance))
			{
				int32 nTutorialLevel = 0;

				if (Parsed.IsValidIndex(1))
				{
					nTutorialLevel = FCString::Atoi(*Parsed[1]);
				}

				// 				if (nTutorialLevel == 0)
				// 				{
				// 					gameInstance->ForceTutorialManager->SkipUnlockTutorial();
				// 				}
				// 				else
				// 				{
				// 					gameInstance->ForceTutorialManager->UnlockTutorialTriggerSetting((EOVERLORD)nTutorialLevel);
				// 					gameInstance->ForceTutorialManager->OnStartUnlockTutorial();
				// 				}
			}
		}
#if WITH_EDITOR
		else if (strComp == TEXT("Settutorial"))
		{
			if (Parsed.IsValidIndex(1))
			{
				UPacketFunctionLibrary::AVATAR_SELECTIVE_TUTORIAL_UPDATE_RQ((int32)EFirstCheckContentsType::FirstForceTutorial, FCString::Atoi(*Parsed[1]));
			}
		}
		else if (strComp == TEXT("Setunlocktutorial"))
		{
// 			enum class EFirstCheckContentsType : uint8
// 			{
// 				FirstForceTutorial = 0,
//				FirstStageNormal2_5,
// 				FirstArena,
// 				FirstColosseum,
// 				FirstSingleRaid,
// 				FirstStageHard1_1,
// 				FirstSingleRaid_DeckSetting,
// 			}

			int32 nType = 0;
			if (Parsed.IsValidIndex(1))
			{
				nType = FCString::Atoi(*Parsed[1]);
			}

			if (Parsed.IsValidIndex(2))
			{
				URGameInstance* RGameInstance = RGAMEINSTANCE(this);
				RGameInstance->ContentsUnLockManager->SetIsFirstEnterArr(nType, FCString::Atoi(*Parsed[2]));
				//UPacketFunctionLibrary::AVATAR_SELECTIVE_TUTORIAL_UPDATE_RQ(nType, FCString::Atoi(*Parsed[2]));
			}
		}
#endif // WITH_EDITOR
		else if (strComp == TEXT("Spawn"))
		{
			Spawn();
		}
		else if (strComp == TEXT("CreateGenericIcon"))
		{
			if (Parsed.IsValidIndex(1))
			{
				int32 iCount = FCString::Atoi(*Parsed[1]);
				for (size_t i = 0; i < iCount; ++i)
				{
					CreateWidget<UUC_GenericItemIcon>(RGAMEINSTANCE(this), UUC_GenericItemIcon::StaticClass());
				}
			}
		}
		else if (strComp == TEXT("DebugLightStreaming"))
		{
			TArray<class ULevelStreaming*> LSArray = RGAMEINSTANCE(GEngine)->GetWorld()->StreamingLevels;
			for (size_t i = 0; i < LSArray.Num(); ++i)
			{
				FString str = LSArray[i]->GetWorldAssetPackageName() + TEXT("bShouldbeLoaded : ") + FString::FromInt(LSArray[i]->bShouldBeLoaded) + TEXT(" bShouldbeVisible : ") + FString::FromInt(LSArray[i]->bShouldBeVisible);
				GEngine->AddOnScreenDebugMessage(-1, 10.f, FColor::Emerald, str);
			}
		}
		else if (strComp == TEXT("ForceGC"))
		{
			GEngine->ForceGarbageCollection();
		}
		else if (strComp == TEXT("SkillFireTime"))
		{
			TArray<TSubclassOf<ABattleBaseCharacter>> CharacterClassArray;

			TArray<FName> CharacterNames = UTableManager::GetInstancePtr()->GetCharacterRowNames();
			for (size_t i = 0; i < CharacterNames.Num(); ++i)
			{
				FCharacterTableInfo* CharacterInfo = UTableManager::GetInstancePtr()->GetCharacterRow(CharacterNames[i]);
				TSubclassOf<ABattleBaseCharacter> HeroBPClass = CharacterInfo->GetHeroBlueprintClass();
				CharacterClassArray.AddUnique(HeroBPClass);
			}

			TArray<FName> CharacterNPCNames = UTableManager::GetInstancePtr()->GetCharacterNpcRowNames();
			for (size_t i = 0; i < CharacterNPCNames.Num(); ++i)
			{
				FCharacterTableInfo* CharacterInfo = UTableManager::GetInstancePtr()->GetCharacterNpcRow(CharacterNPCNames[i]);
				TSubclassOf<ABattleBaseCharacter> HeroBPClass = CharacterInfo->GetHeroBlueprintClass();
				CharacterClassArray.AddUnique(HeroBPClass);
			}

			for (size_t i = 0; i < CharacterClassArray.Num(); ++i)
			{
				if (CharacterClassArray[i] == nullptr)
					continue;

				AActor* Character_CDO = CharacterClassArray[i]->GetDefaultObject<AActor>();
				if (Character_CDO)
				{
					USkeletalMeshComponent* SkelComp = Character_CDO->FindComponentByClass<USkeletalMeshComponent>();
					if (SkelComp && SkelComp->AnimClass)
					{
						TSubclassOf<UAnimInstance> AnimInstanceClass = SkelComp->AnimClass;
						UBattleBaseAnimInstance* BattleAnimInst_CDO = AnimInstanceClass->GetDefaultObject<UBattleBaseAnimInstance>();

						if (BattleAnimInst_CDO)
						{
							if (BattleAnimInst_CDO->AnimMontageSkillMap.Num())
							{
								for (auto& Element : BattleAnimInst_CDO->AnimMontageSkillMap)
								{
									UAnimMontage* Montage = UUtilFunctionLibrary::GetAnimMontageFromAssetID(Element.Value);
									if (Montage)
									{
										int32 SectionIdx = Montage->GetSectionIndex(TEXT("SkillAction"));

										//Montage Notify 
										for (auto& Elem : Montage->Notifies)
										{
											USkillFireNotify* SkillFireNotify = Cast<USkillFireNotify>(Elem.Notify);
											if (SkillFireNotify)
											{
												float Length = Montage->GetSectionLength(SectionIdx);
												float MontageLength = Montage->SequenceLength;
												float Delta = MontageLength - Length;
												float LinkTime = Elem.GetTriggerTime();
												float DeltaLinkTime = LinkTime - Delta;

												if(DeltaLinkTime < 0)
													FString str = TEXT("");

												FString str = TEXT("");
												FString SkillName = Montage->GetName();
												str = SkillName + TEXT("    Notify : ") + FString::SanitizeFloat(DeltaLinkTime) + TEXT("     Length : ") + FString::SanitizeFloat(Length);
												UE_LOG(LogClass, Log, TEXT("%s"), *str);
											}
										}

										//AnimSequence Notify
										FCompositeSection& Section = Montage->GetAnimCompositeSection(SectionIdx);
										const UAnimSequenceBase* AnimSequence = Section.GetLinkedSequence();
										if (AnimSequence)
										{
											for (auto& Elem : AnimSequence->Notifies)
											{
												USkillFireNotify* SkillFireNotify = Cast<USkillFireNotify>(Elem.Notify);
												if (SkillFireNotify)
												{
													float Length = Montage->GetSectionLength(SectionIdx);
													float LinkTime = Elem.GetTriggerTime();

													FString str = TEXT("");
													FString SkillName = Montage->GetName();
													str = SkillName + TEXT("    Notify : ") + FString::SanitizeFloat(LinkTime) + TEXT("     Length : ") + FString::SanitizeFloat(Length);
													UE_LOG(LogClass, Log, TEXT("%s"), *str);
												}
											}
										}
									}
								}
							}
						}
					}
				}
			}
		}
	}

	// 	URGameInstance* gameInstance = RGAMEINSTANCE(this);
	// 	if (IsValid(gameInstance) && IsValid(gameInstance->ForceTutorialManager))
	// 	{
	// 		gameInstance->ForceTutorialManager->ConsoleCommand(Command);
	// 	}
}