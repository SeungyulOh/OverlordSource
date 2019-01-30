// Fill out your copyright notice in the Description page of Project Settings.

#include "ProjectR.h"
#include "TestGameMode.h"
#include "ContentGameMode/Controllers/TestPlayerController.h"
#include "Runtime/Engine/Classes/Kismet/KismetSystemLibrary.h"
#include "Runtime/LevelSequence/Public/LevelSequenceActor.h"
#include "GlobalIntegrated.h"
#include "GameInfo/PlayStateInfo.h"
#include "Character/Interfaces/EntityBaseProperty.h"
#include "Character/Components/FunctionalComponents/BattleEntityComponent.h"
#include "Character/Components/FunctionalComponents/EntityRenderComponent.h"
#include "Character/Components/DamageFloater/SlateDamageFloaterComponentV2.h"
#include "Character/Components/SpecialMovingComponent.h"
#include "UtilFunctionIntegrated.h"
#include "Object/BaseCameraActor.h"
#include "JSONString.h"
#include "JsonObjectConverter.h"
#include "LeaderControlHelper.h"
#include "ContentGameMode/Controllers/BasePlayerController.h"
#include "ContentGameMode/Controllers/TestPlayerController.h"
#include "Controllers/DPadController.h"
#include "CustomActors/BaseSequenceActor.h"
#include "CustomActors/DropItemActor.h"
#include "Network/HttpRequestClient.h"
#include "Network/Chat/ChatManager.h"
#include "UpdateService/Auth/RAuth.h"
#include "UI/UIDefs.h"
#include "UI/RWidgetManager.h"
#include "UI/UI_Battle/RCampaignHUD_ThreeControl.h"
#include "UI/HeroInfo/HeroManager/UC_HeroStatMgr_Info.h"
#include "DamageFloater_PS.h"

ATestGameMode::ATestGameMode(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{	
	PlayerControllerClass = ATestPlayerController::StaticClass();
}

void ATestGameMode::PrepareGameMode()
{
	Super::PrepareGameMode();

	//*// test data
	FREWARD dropitemfortest;
	dropitemfortest.rewardId = FName(TEXT("WHA010003"));
	dropitemfortest.type = EREWARD_TYPE::ITEM;
	DropRewards.Emplace(dropitemfortest);
	
	dropitemfortest.rewardId = FName(TEXT("AHA010001"));
	dropitemfortest.type = EREWARD_TYPE::ITEM;
	DropRewards.Emplace(dropitemfortest);
	
	dropitemfortest.rewardId = FName(TEXT("NEC020008"));
	dropitemfortest.type = EREWARD_TYPE::ITEM;
	DropRewards.Emplace(dropitemfortest);

	dropitemfortest.rewardId = WALLET_GOLD;
	dropitemfortest.type = EREWARD_TYPE::WALLET;
	DropRewards.Emplace(dropitemfortest);

	dropitemfortest.rewardId = WALLET_RUNE001;
	dropitemfortest.type = EREWARD_TYPE::WALLET;
	DropRewards.Emplace(dropitemfortest);
	//*/ //test data


	GameInstance->EventManager->OnDeadActor.RemoveDynamic(this, &ATestGameMode::CheckItemDrop);
	GameInstance->EventManager->OnDeadActor.AddDynamic(this, &ATestGameMode::CheckItemDrop);

}

void ATestGameMode::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	UE_LOG(LogBattle, Log, TEXT("Leave ATestGameMode "));
	Super::EndPlay(EndPlayReason);
}

void ATestGameMode::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
	if (LevelSequenceActor.IsValid())
	{
		if (IsValid(LevelSequenceActor->SequencePlayer))
		{
			if (!LevelSequenceActor->SequencePlayer->IsPlaying())
			{
				LevelSequenceActor->SequencePlayer->Stop();
				LevelSequenceActor = nullptr;
			}
		}
	}

	URAsyncLoader::GetInstancePtr()->LoadFx();
}


void ATestGameMode::BeginPlay()
{
	Super::BeginPlay();
	
	UE_LOG(LogBattle, Log, TEXT("Enter ATestGameMode "));
	
	GameInstance = RGAMEINSTANCE(this);
	
	if (!GameInstance)
		return;
	
	FSlateApplication::Get().GetPlatformApplication()->Cursor->Show(true);
	
	UWorld* CurrentWorld = GameInstance->GetWorld();
	
	APlayerCameraManager* PlayerCM = nullptr;
	
	if (CurrentWorld)
		PlayerCM = UGameplayStatics::GetPlayerCameraManager(CurrentWorld, 0);

	if (PlayerCM)
	{
		ABaseCameraActor* BaseCam = Cast<ABaseCameraActor>(PlayerCM->GetViewTarget());
		if (BaseCam)
			GameInstance->CameraActor = BaseCam;
	}

	//if (IsValid(GameInstance))
	{
		auto RAuth = URAuth::GetInstancePtr();
		RAuth->Prelogin();

		GameInstance->InitBeforeLaunch();

		UTableManager::GetInstancePtr();
		
		GameInstance->PlayStateInfo->Init();
		GameInstance->GridMapManager->Initialize();
		GameInstance->AutoPlayManager->Initialize();
		GameInstance->SequenceManager->Initialize();
		GameInstance->GameDataManager->Initialize();
		GameInstance->ForceTutorialManager->Initialize();
		GameInstance->ObjectPoolManager->Initialize();
		GameInstance->AbnormalStateGroupTable_V2->Init();
		GameInstance->ChatManager->Initialize(GameInstance);
		GameInstance->HttpClient->SetKID(KID_TEST_MY);
		GameInstance->CreateEditorManager(EEditorEnum::VE_SpawnerEditor);
		if (IsValid(GameInstance->PlayStateInfo))
			GameInstance->PlayStateInfo->ClearSelectedCharacter();

		ABasePlayerController* PlayerController = UUtilFunctionLibrary::GetBasePlayerController(GameInstance->GetWorld());
		if (IsValid(PlayerController))
		{
			ABaseCameraActor* Camera = nullptr;
			TArray<AActor*> arrayList;
			UGameplayStatics::GetAllActorsOfClass(GameInstance, ABaseCameraActor::StaticClass(), arrayList);
			for (AActor* actor : arrayList)
			{
				Camera = Cast<ABaseCameraActor>(actor);
				if (IsValid(Camera))
				{
					PlayerController->SetCameraActor(Camera);
					break;
				}
			}
		}

		auto gameMode = UGameplayStatics::GetGameMode(GameInstance);
		if (!IsValid(gameMode))
			return;
		int32 Start = (int32)EGameModeStateEnum::VE_Init;
		int32 Max = (int32)EGameModeStateEnum::VE_End;
		for (; Start < Max; ++Start)
		{
			EGameModeStateEnum CurrentState = (EGameModeStateEnum)Start;
			UUtilFunctionLibrary::ChangeGameModeState(CurrentState);
			UUtilFunctionLibrary::CheckGameModeState();
		}

		if (IsValid(GameInstance) && IsValid(GameInstance->GameDataManager))
		{
			GameInstance->GameDataManager->ClearNextTargetActor();
			GameInstance->GameDataManager->SpawnNextTargetActor();
		}
	}

	if(IsValid(DamageFloaterMgr))
		DamageFloaterMgr->Init();
}

void ATestGameMode::Clear()
{
	MyHeroList.Reset();
}

void ATestGameMode::ExeLastCommand()
{
	if (LastCommand.Len() > 0)
	{
		ConsoleCommand(LastCommand);
	}
}

void ATestGameMode::ConsoleCommand(const FString& Command)
{
	const TCHAR* Delims[] = { TEXT(" ") };
	FString strLower = Command.ToLower();
	TArray<FString> Parsed;
	strLower.ParseIntoArray(Parsed, Delims, 1);

	if (Parsed.Num() > 0)
	{
		//hero my 1 20 0 0 0
		//npc enemy 1 15 abs 0 0 0
		//minion my 1 15 abs 0 0 0
		//bobject my 1 15 abs 0 0 0

		//deck
		//loadmap 1
		//savekey 1 == 1,2,3,4
		//loadkey 1 == 1,2,3,4
		//startkey 5 == 5,6,7,8
		//endkey
		//exekey 5 == 5,6,7,8
		//time 0.5
		//sequence test

		//if (IsValid(GameInstance))
		{
			UKismetSystemLibrary::PrintString(GameInstance, Command);
		}

		if (bSaveKeyStart)
		{
			if (IsValid(GameInstance) && IsValid(GameInstance->EditorManager))
			{
				if (!Command.Contains("endkey"))
				{
					GameInstance->EditorManager->ExeCommandList.Add(Command);
				}
			}
		}

		FString strComp = Parsed[0];
		if (strComp == "hero")
		{
			CommandHeroSpawn(Parsed);
			LastCommand = Command;
		}
		else if (strComp == "npc")
		{
			CommandSpawn(ECharacterCategoryTypeEnum::VE_Npc, Parsed);
			LastCommand = Command;
		}
		else if (strComp == "minion")
		{
			CommandSpawn(ECharacterCategoryTypeEnum::VE_Minion, Parsed);
			LastCommand = Command;
		}
		else if (strComp == "neutralnpc")
		{
			CommandSpawn(ECharacterCategoryTypeEnum::VE_NeutralNpc, Parsed);
			LastCommand = Command;
		}
		else if (strComp == "bobject")
		{
			CommandSpawn(ECharacterCategoryTypeEnum::VE_Object, Parsed);
			LastCommand = Command;
		}
		else if (strComp == "deck")
		{
			CommandDeck(Parsed);
		}
		else if (strComp == "loadmap")
		{
			CommandLoadMap(Parsed);
		}
		else if (strComp == "savekey")
		{
			CommandKeySave(Parsed);
		}
		else if (strComp == "loadkey")
		{
			CommandKeyLoad(Parsed);
		}
		else if (strComp == "startkey")
		{
			if (Parsed.IsValidIndex(1))
			{
				if (Parsed[1] == "5" || Parsed[1] == "6" || Parsed[1] == "7" || Parsed[1] == "8")
				{
					if (IsValid(GameInstance) && IsValid(GameInstance->EditorManager))
					{
						bSaveKeyStart = true;
						GameInstance->EditorManager->ExeCommandList.Empty();
						CurKey = Parsed[1];

						FString strText;
						strText = FString::Printf(TEXT("***************StartKey %s***************"), *Parsed[1]);
						UKismetSystemLibrary::PrintString(GameInstance, strText);

					}
				}
			}
		}
		else if (strComp == "endkey")
		{
			if (bSaveKeyStart)
			{
				bSaveKeyStart = false;

				if (IsValid(GameInstance) && IsValid(GameInstance->EditorManager))
				{
					if (GameInstance->EditorManager->ExeCommandList.Num() > 0)
					{
						FString FullPath = FPaths::ProjectSavedDir() + TEXT("SaveCommand/") + CurKey + TEXT(".txt");
						FString strInfo;

						for (FString command : GameInstance->EditorManager->ExeCommandList)
						{
							strInfo += command;
							strInfo += "\n";
						}

						if (FFileHelper::SaveStringToFile(strInfo, *FullPath))
						{
							FString strText;
							strText = FString::Printf(TEXT("***************EndKey %s***************"), *CurKey);
							UKismetSystemLibrary::PrintString(GameInstance, strText);
							GameInstance->EditorManager->ExeCommandList.Empty();
						}
					}
				}
			}
		}
		else if (strComp == "exekey")
		{
			if (Parsed.IsValidIndex(1))
			{
				if (Parsed[1] == "5" || Parsed[1] == "6" || Parsed[1] == "7" || Parsed[1] == "8")
				{
					CommandExeKey(Parsed);
				}
			}
		}
		else if (strComp == "time")
		{
			if (Parsed.IsValidIndex(1))
			{
				float GlobalTime = FCString::Atof(*Parsed[1]);
				UGameplayStatics::SetGlobalTimeDilation(this, GlobalTime);
			}
		}
		else if (strComp == "allspawn")
		{
			/*if (IsValid(GameInstance) && IsValid(GameInstance->AutoPlayManager))
			{
				UAutoSpawner* AutoSpawner = GameInstance->AutoPlayManager->GetAutoSpawner();
				if (IsValid(AutoSpawner))
				{
					AutoSpawner->AutoSpawnCharacters();
				}
			}*/
		}
		else if (strComp == "sequence")
		{
			if (Parsed.IsValidIndex(1))
			{
				if (IsValid(GameInstance) && IsValid(GameInstance->SequenceManager))
				{
					ABaseSequenceActor* SequenceActor = GameInstance->SequenceManager->SpawnSequence(FName(*Parsed[1]));
					if (IsValid(SequenceActor))
					{
						if (false == SequenceActor->OnSequenceEndEvent.IsAlreadyBound(this, &ATestGameMode::OnSequenceEnd))
						{
							SequenceActor->OnSequenceEndEvent.AddDynamic(this, &ATestGameMode::OnSequenceEnd);
						}
					}
				}
			}
		}
		else if (strComp == "damagefloater")
		{
			if (Parsed.IsValidIndex(1) && Parsed.IsValidIndex(2))
			{
				CommandDamageFloater(Parsed);
			}
		}
		else if (strComp == "ttt")
		{
			//if (IsValid(GameInstance))
			{
				ABasePlayerController* Controller = UUtilFunctionLibrary::GetBasePlayerController(GameInstance->GetWorld());
				if (IsValid(Controller))
				{
					AActor* actor = Controller->GetSelectedCharacter();
					if (IsValid(actor))
					{
						UBattleEntityComponent* battleEntityComponent = UCharacterHelpFunctionLibrary::GetBattleEntityComponent(actor);
						if (IsValid(battleEntityComponent))
						{
							AActor* TargetActor = battleEntityComponent->GetCurrentAttackTarget();
							if (TargetActor)
							{
								USpecialMovingComponent* spec_move_comp = UCharacterHelpFunctionLibrary::GetSpecialMovingComponent(actor);
								if (IsValid(spec_move_comp))
								{
									spec_move_comp->StartFear(TargetActor, 5.0f);
								}
							}
						}
					}
				}
			}
		}
	}
}

void ATestGameMode::OnSequenceEnd(FName InSequenceKey)
{
	//if (IsValid(GameInstance))
	{
		if (IsValid(GameInstance->SequenceManager))
		{
			GameInstance->SequenceManager->ClearSequence();
		}

		ABasePlayerController* Controller = UUtilFunctionLibrary::GetBasePlayerController(GameInstance->GetWorld());
		if (IsValid(Controller))
		{
			Controller->SetViewTarget(Controller->GetCameraActor());
		}
	}
}

void ATestGameMode::CheckItemDrop(AActor* DeadActor, AActor* FromActor)
{
	UBattleEntityComponent* BattleComp = UCharacterHelpFunctionLibrary::GetBattleEntityComponent(DeadActor);
	if (!BattleComp->AmIEnemyWithMyPlayer()) // ally actor
		return;

	if (BattleComp->IsBoss())
	{
		// all item drop

		while (DropRewards.Num() != 0)
		{
			DropItem(DeadActor);
		}

		return;
	}

	DropItem(DeadActor);
}

void ATestGameMode::DropItem(AActor* ItemGiveActor)
{
	if (0 < DropRewards.Num())
	{
		FREWARD& item = DropRewards[0];

		// 무기
		// 방어구
		// 악세사리
		// 룬
		// 기타

		FActorSpawnParameters SpawnParams;
		SpawnParams.Owner = ItemGiveActor;
		SpawnParams.Instigator = ItemGiveActor->Instigator;
		SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;
		SpawnParams.bNoFail = true;

		USkeletalMeshComponent* skelcomp = ItemGiveActor->FindComponentByClass<USkeletalMeshComponent>();
		FVector location = skelcomp->GetSocketLocation(SOCKET_ROOT);
		location += FMath::VRand();
		ADropItemActor* itemActor = GetWorld()->SpawnActor<ADropItemActor>(ADropItemActor::StaticClass(), location, FRotator::ZeroRotator, SpawnParams);

		FDropItemResourceInfo* DropItemResourceInfo = nullptr;
		int32 nGrade = 0;

		switch (item.type)
		{

		case EREWARD_TYPE::ITEM:
		{
			FItemTableInfo*		ItemTableInfo = UTableManager::GetInstancePtr()->GetCharacterItemRow(item.rewardId);
			if (!ItemTableInfo)
				break;

			if ((uint8)EItemEquipTypeEnum::VE_Sword <= (uint8)ItemTableInfo->EquipType && (uint8)ItemTableInfo->EquipType <= (uint8)EItemEquipTypeEnum::VE_Wand)
			{
				// case weapon
				DropItemResourceInfo = UTableManager::GetInstancePtr()->GetDropItemResourceRow(FName(TEXT("DropWeapon")));
			}
			else if ((uint8)EItemEquipTypeEnum::VE_HeavyArmor <= (uint8)ItemTableInfo->EquipType && (uint8)ItemTableInfo->EquipType <= (uint8)EItemEquipTypeEnum::VE_FabricPants)
			{
				// case armor
				DropItemResourceInfo = UTableManager::GetInstancePtr()->GetDropItemResourceRow(FName(TEXT("DropArmor")));
			}
			else if ((uint8)EItemEquipTypeEnum::VE_Necklace <= (uint8)ItemTableInfo->EquipType && (uint8)ItemTableInfo->EquipType <= (uint8)EItemEquipTypeEnum::VE_Ring)
			{
				// case necklace, ring
				DropItemResourceInfo = UTableManager::GetInstancePtr()->GetDropItemResourceRow(FName(TEXT("DropAccessory")));
			}
			nGrade = (int32)ItemTableInfo->ItemGrade;
		}
		break;

		case EREWARD_TYPE::WALLET:
		{
			FWalletTableInfo* WalletInfo = UTableManager::GetInstancePtr()->GetWalletRow(item.rewardId);
			if (!WalletInfo)
				break;
			if (WalletInfo->PropertyType == EWalletPropertyType::Rune)
			{
				// case rune
				DropItemResourceInfo = UTableManager::GetInstancePtr()->GetDropItemResourceRow(FName(TEXT("DropRune")));
			}
			else
			{
				DropItemResourceInfo = UTableManager::GetInstancePtr()->GetDropItemResourceRow(FName(TEXT("DropEtc")));
			}
			nGrade = (WalletInfo->Grade == 0) ? 0 : WalletInfo->Grade - 1;
		}
		break;

		default:
		{
			// case etc
			DropItemResourceInfo = UTableManager::GetInstancePtr()->GetDropItemResourceRow(FName(TEXT("DropEtc")));
			nGrade = 1;
		}
		break;

		}

		if (DropItemResourceInfo)
		{
			USkeletalMesh* SkelMesh = UUtilFunctionLibrary::GetSkeletalMeshFromAssetID(DropItemResourceInfo->SkeletalMesh);
			UAnimationAsset* DropAnimAsset = UUtilFunctionLibrary::GetAnimationAssetFromAssetID(DropItemResourceInfo->Animation_Drop);
			UAnimationAsset* IdleAnimAsset = UUtilFunctionLibrary::GetAnimationAssetFromAssetID(DropItemResourceInfo->Animation_Idle);

			itemActor->SetDropItemData(SkelMesh, DropAnimAsset, IdleAnimAsset, nGrade);
		}

		DropRewards.RemoveAt(0);
	}
}

void ATestGameMode::CommandLoadMap(TArray<FString>& Parsed)
{
	if (Parsed.IsValidIndex(1))
	{
		if (IsValid(GameInstance) && IsValid(GameInstance->EditorManager))
		{
			ObjectHelper.RemoveObjectAll();
			GameInstance->EditorManager->LoadMap(FName(*Parsed[1]));
		}
	}
}

void ATestGameMode::CommandExeKey(TArray<FString>& Parsed)
{
	if (bSaveKeyStart)
		return;

	if (Parsed.IsValidIndex(1))
	{
		if (Parsed[1] == "5" || Parsed[1] == "6" || Parsed[1] == "7" || Parsed[1] == "8")
		{
			if (IsValid(GameInstance) && IsValid(GameInstance->EditorManager))
			{
				GameInstance->EditorManager->ExeCommandList.Empty();
				FString FullPath = FPaths::ProjectSavedDir() + TEXT("SaveCommand/") + Parsed[1] + TEXT(".txt");
				if (IFileManager::Get().FileExists(*FullPath))
				{
					FString strInfo;
					if (FFileHelper::LoadFileToString(strInfo, *FullPath))
					{
						const TCHAR* Delims[] = { TEXT("\n") };
						strInfo.ParseIntoArray(GameInstance->EditorManager->ExeCommandList, Delims, 1);

						while (GameInstance->EditorManager->ExeCommandList.Num() > 0)
						{
							FString command = GameInstance->EditorManager->ExeCommandList[0];
							if (command.Contains("loadmap"))
							{
								ConsoleCommand(command);
								GameInstance->EditorManager->ExeCommandList.RemoveAt(0);
								break;
							}
							else
							{
								ConsoleCommand(command);
							}

							GameInstance->EditorManager->ExeCommandList.RemoveAt(0);
						}
					}
				}
			}
		}
	}
}

void ATestGameMode::CommandKeySave(TArray<FString>& Parsed)
{
	if (Parsed.IsValidIndex(1))
	{
		if (Parsed[1] == "1" || Parsed[1] == "2" || Parsed[1] == "3" || Parsed[1] == "4")
		{
			if (IsValid(GameInstance) && IsValid(GameInstance->PlayStateInfo) /*&& IsValid(GameInstance->GameObjectMgr)*/)
			{
				DeckLoadSave(Parsed[1], false);
			}
		}
	}
}

void ATestGameMode::CommandKeyLoad(TArray<FString>& Parsed)
{
	if (Parsed.IsValidIndex(1))
	{
		if (Parsed[1] == "1" || Parsed[1] == "2" || Parsed[1] == "3" || Parsed[1] == "4")
		{
			if (IsValid(GameInstance) && IsValid(GameInstance->PlayStateInfo) /*&& IsValid(GameInstance->GameObjectMgr)*/)
			{
				DeckLoadSave(Parsed[1], true);
			}
		}
	}
}

void ATestGameMode::CurDeckReLoad()
{
	if (IsValid(GameInstance) && IsValid(GameInstance->PlayStateInfo))
	{
		RGAMEINSTANCE(this)->PlayStateInfo->ClearSelectedCharacter();
	}

	if (Decks.DeckMemberList.Num() > 0)
	{
		for (int32 i = 0; i < Decks.DeckMemberList.Num(); ++i)
		{
			FPlayTestDeck deck = Decks.DeckMemberList[i];

			SpawnHero(deck.Id, deck.Level, true, FVector::ZeroVector, deck.SkillLevelList, deck.ItemList);
		}
	}
}

void ATestGameMode::DeckLoadSave(FString strName, bool bLoad)
{
	if (IsValid(GameInstance) && IsValid(GameInstance->PlayStateInfo) /*&& IsValid(GameInstance->GameObjectMgr)*/)
	{
		FString FullPath = FPaths::ProjectSavedDir() + TEXT("TestDeck/") + strName + TEXT(".json");
		if (bLoad)
		{
			if (IFileManager::Get().FileExists(*FullPath))
			{
				FString strDeckInfo;
				if (FFileHelper::LoadFileToString(strDeckInfo, *FullPath))
				{
					if (FJsonObjectConverter::JsonObjectStringToUStruct<FPlayTestDecks>(strDeckInfo, &Decks, 0, 0))
					{
						for (TWeakObjectPtr<AActor> actor : MyHeroList)
						{
							if (actor.IsValid())
								ObjectHelper.RemoveObject(actor.Get(), true);
						}
						Clear();

						FString strText;
						strText = FString::Printf(TEXT("Deck Load %s"), *FullPath);
						UKismetSystemLibrary::PrintString(GameInstance, strText);

						CurDeckReLoad();
					}
				}
			}
		}
		else
		{
			Decks.DeckMemberList.Reset();
			auto SelectedCharacterArray = GameInstance->PlayStateInfo->GetSelectedCharacterArray();
			for (int32 i = 0; i < SelectedCharacterArray.Num(); ++i)
			{
				FHeroSlotInfo info = SelectedCharacterArray[i];

				FPlayTestDeck deck;
				deck.Id = info.HeroIDName;
				deck.Level = info.HeroLevel;
				deck.SkillLevelList = info.TestSkillLevelList;
				deck.ItemList = info.TestItemList;
				Decks.DeckMemberList.Add(deck);
			}

			if (Decks.DeckMemberList.Num() > 0)
			{
				FString strDeckInfo;
				if (FJsonObjectConverter::UStructToJsonObjectString(FPlayTestDecks::StaticStruct(), &Decks, strDeckInfo, 0, 0))
				{
					if (FFileHelper::SaveStringToFile(strDeckInfo, *FullPath))
					{
						FString strText;
						strText = FString::Printf(TEXT("Deck Save %s"), *FullPath);
						UKismetSystemLibrary::PrintString(GameInstance, strText);
					}
				}
			}
		}
	}
}

bool ATestGameMode::SetSelectNpcActor()
{
	if (IsValid(GameInstance) /*&& IsValid(GameInstance->GameObjectMgr)*/)
	{
		TArray< TWeakObjectPtr< AActor > > outObjectList;
		ObjectHelper.GetObjectList(outObjectList);
		TArray< TWeakObjectPtr< AActor > > outNpcObjectList;
		for (TWeakObjectPtr< AActor > actor : outObjectList)
		{
			if (actor.IsValid())
			{
				IEntityBaseProperty* EntityBasePropery = Cast<IEntityBaseProperty>(actor.Get());
				if (EntityBasePropery)
				{
					switch (EntityBasePropery->GetCharacterCategory())
					{
					case ECharacterCategoryTypeEnum::VE_Minion:
					case ECharacterCategoryTypeEnum::VE_Npc:
					case ECharacterCategoryTypeEnum::VE_NeutralNpc:
					{
						outNpcObjectList.Add(actor);
					}
					break;
					}
				}
			}
		}

		if (SelectActor.IsValid())
		{
			++SelectIdx;

			URenewal_BattleFunctionLibrary::ComputeTargetDecal(SelectActor.Get(), false);
		}
		else
		{
			SelectIdx = 0;
		}

		SelectActor = nullptr;

		if (!outNpcObjectList.IsValidIndex(SelectIdx))
		{
			SelectIdx = 0;
		}

		if (outNpcObjectList.IsValidIndex(SelectIdx))
		{
			SelectActor = outNpcObjectList[SelectIdx];
		}

		if (SelectActor.IsValid())
		{
			URenewal_BattleFunctionLibrary::ComputeTargetDecal(SelectActor.Get(), true);
			return true;
		}
	}

	return false;
}

void ATestGameMode::ClearSelectActor()
{
	if (SelectActor.IsValid())
	{
		URenewal_BattleFunctionLibrary::ComputeTargetDecal(SelectActor.Get(), false);
		SelectActor = nullptr;
	}
}

void ATestGameMode::CommandDeck(TArray<FString>& Parsed)
{
	if (Parsed.IsValidIndex(1) && Parsed.IsValidIndex(2))
	{
		if (IsValid(GameInstance) && IsValid(GameInstance->PlayStateInfo)/* && IsValid(GameInstance->GameObjectMgr)*/)
		{
			FString FullPath = FPaths::ProjectSavedDir() + TEXT("TestDeck/") + Parsed[2] + TEXT(".json");
			if (Parsed[1] == "save")
			{
				DeckLoadSave(Parsed[2], false);
			}
			else if (Parsed[1] == "load")
			{
				DeckLoadSave(Parsed[2], true);
			}
		}
	}
}

void ATestGameMode::CommandHeroSpawn(TArray<FString>& Parsed)
{
	if (Parsed.IsValidIndex(1))
	{
		bool bMy = false;
		if (Parsed[1] == "my")
		{
			bMy = true;
		}
		else if (Parsed[1] == "enemy")
		{
			bMy = false;
		}

		if (Parsed.IsValidIndex(2))
		{
			FString heroid = Parsed[2];

			FCharacterTableInfo* characterTableInfo = UTableManager::GetInstancePtr()->GetCharacterRow(FName(*heroid));
			if (characterTableInfo)
			{
				int32 level = 1;
				if (Parsed.IsValidIndex(3))
				{
					level = FCString::Atoi(*Parsed[3]);
				}

				if (bMy)
				{
					if (MyHeroList.Num() == 5)
						return;
				}

				FVector offset = FVector::ZeroVector;
				float x = 0.0f;
				float y = 0.0f;
				float z = 0.0f;
				if (Parsed.IsValidIndex(4))
				{
					x = FCString::Atof(*Parsed[4]);
				}
				if (Parsed.IsValidIndex(5))
				{
					y = FCString::Atof(*Parsed[5]);
				}
				if (Parsed.IsValidIndex(6))
				{
					z = FCString::Atof(*Parsed[6]);
				}

				offset.X = x;
				offset.Y = y;
				offset.Z = z;

				TArray<int32> skillLevelList;
				TArray<FTestItemInfo> itemList;

				if (Parsed.IsValidIndex(7))
				{
					const TCHAR* Delims[] = { TEXT(",") };
					FString strLower = Parsed[7].ToLower();
					TArray<FString> ParsedSub;
					strLower.ParseIntoArray(ParsedSub, Delims, 1);

					if (ParsedSub.IsValidIndex(0))
					{
						if (ParsedSub[0] == TEXT("skilllevel"))
						{
							if (ParsedSub.IsValidIndex(1) && ParsedSub.IsValidIndex(2) && ParsedSub.IsValidIndex(3) && ParsedSub.IsValidIndex(4) && ParsedSub.IsValidIndex(5))
							{
								skillLevelList.Add(FCString::Atoi(*ParsedSub[1]));
								skillLevelList.Add(FCString::Atoi(*ParsedSub[2]));
								skillLevelList.Add(FCString::Atoi(*ParsedSub[3]));
								skillLevelList.Add(FCString::Atoi(*ParsedSub[4]));
								skillLevelList.Add(FCString::Atoi(*ParsedSub[5]));
							}
						}
						else if (ParsedSub[0] == TEXT("item"))
						{
							if (ParsedSub.IsValidIndex(1) && ParsedSub.IsValidIndex(2) && ParsedSub.IsValidIndex(3) && ParsedSub.IsValidIndex(4) && ParsedSub.IsValidIndex(5))
							{
								for (int32 i = 1; i < ParsedSub.Num(); ++i)
								{
									const TCHAR* Delims1[] = { TEXT(":") };
									FString strLower1 = ParsedSub[i].ToLower();
									TArray<FString> ParsedSub1;
									strLower1.ParseIntoArray(ParsedSub1, Delims1, 1);

									if (ParsedSub1.Num() > 0)
									{
										FTestItemInfo testItemInfo;
										testItemInfo.ItemKey = FName(*ParsedSub1[0]);

										for (int32 j = 1; j<ParsedSub1.Num(); ++j)
										{
											testItemInfo.GemList.Add(ParsedSub1[j]);
										}

										itemList.Add(testItemInfo);
									}
								}
							}
						}
					}
				}

				if (Parsed.IsValidIndex(8))
				{
					const TCHAR* Delims[] = { TEXT(",") };
					FString strLower = Parsed[8].ToLower();
					TArray<FString> ParsedSub;
					strLower.ParseIntoArray(ParsedSub, Delims, 1);

					if (ParsedSub.IsValidIndex(0))
					{
						if (ParsedSub[0] == TEXT("item"))
						{
							if (ParsedSub.IsValidIndex(1) && ParsedSub.IsValidIndex(2) && ParsedSub.IsValidIndex(3) && ParsedSub.IsValidIndex(4))
							{
								for (int32 i = 1; i < ParsedSub.Num(); ++i)
								{
									const TCHAR* Delims1[] = { TEXT(":") };
									FString strLower1 = ParsedSub[i].ToLower();
									TArray<FString> ParsedSub1;
									strLower1.ParseIntoArray(ParsedSub1, Delims1, 1);

									if (ParsedSub1.Num() > 0)
									{
										FTestItemInfo testItemInfo;
										testItemInfo.ItemKey = FName(*ParsedSub1[0]);

										for (int32 j = 1; j<ParsedSub1.Num(); ++j)
										{
											testItemInfo.GemList.Add(ParsedSub1[j]);
										}

										itemList.Add(testItemInfo);
									}
								}
							}
						}
					}
				}

				SpawnHero(FName(*heroid), level, bMy, offset, skillLevelList, itemList);
			}
		}
	}
}

void ATestGameMode::CommandDamageFloater(TArray<FString>& Parsed)
{
	if (Parsed.IsValidIndex(1) && Parsed.IsValidIndex(2))
	{
		for (TWeakObjectPtr<AActor>& actor : MyHeroList)
		{
			if (actor.IsValid())
			{
				UEntityRenderComponent* RenderComponent = UCharacterHelpFunctionLibrary::GetEntityRenderComponent(actor.Get());
				if (IsValid(RenderComponent))
				{
					if (IsValid(RenderComponent->GetSlateDamageFloaterComponent()))
					{
						EBattleDamageResultEnum damageType = (EBattleDamageResultEnum)FCString::Atoi(*Parsed[1]);
						int32 damage = FCString::Atoi(*Parsed[2]);

						float scale = 1.0f;

						if (Parsed.IsValidIndex(3))
						{
							scale = FCString::Atof(*Parsed[3]);
						}

						int32 addPercent = 0;
						if (Parsed.IsValidIndex(4))
						{
							addPercent = FCString::Atoi(*Parsed[4]);
						}

						RenderComponent->GetSlateDamageFloaterComponent()->AddDamageFloater(damage, addPercent, damageType, scale);
					}
				}
			}
		}
	}
}

void ATestGameMode::CommandSpawn(ECharacterCategoryTypeEnum categroyType, TArray<FString>& Parsed)
{
	if (Parsed.IsValidIndex(1))
	{
		bool bMy = false;
		if (Parsed[1] == "my")
		{
			bMy = true;
		}
		else if (Parsed[1] == "enemy")
		{
			bMy = false;
		}

		if (Parsed.IsValidIndex(2))
		{
			FString id = Parsed[2];

			bool bExe = false;

			switch (categroyType)
			{
			case ECharacterCategoryTypeEnum::VE_Minion:
			case ECharacterCategoryTypeEnum::VE_Npc:
			case ECharacterCategoryTypeEnum::VE_NeutralNpc:
			{
				FCharacterTableInfo* characterTableInfo = UTableManager::GetInstancePtr()->GetCharacterNpcRow(FName(*id));
				if (characterTableInfo)
				{
					bExe = true;
				}
			}
			break;
			case ECharacterCategoryTypeEnum::VE_Object:
			{
				FBattleObjectTableInfo* battleObjectTableInfo = UTableManager::GetInstancePtr()->GetBattleObjectRow(FName(*id));
				if (battleObjectTableInfo)
				{
					bExe = true;
				}
			}
			break;
			}

			if (bExe)
			{
				int32 level = 1;
				if (Parsed.IsValidIndex(3))
				{
					level = FCString::Atoi(*Parsed[3]);
				}

				bool bAbs = true;
				if (Parsed.IsValidIndex(4))
				{
					if (Parsed[4] == "abs")
					{
						bAbs = true;
					}
					else
					{
						bAbs = false;
					}
				}

				FVector offset = FVector::ZeroVector;
				float x = 0.0f;
				float y = 0.0f;
				float z = 0.0f;
				if (Parsed.IsValidIndex(5))
				{
					x = FCString::Atof(*Parsed[5]);
				}
				if (Parsed.IsValidIndex(6))
				{
					y = FCString::Atof(*Parsed[6]);
				}
				if (Parsed.IsValidIndex(7))
				{
					z = FCString::Atof(*Parsed[7]);
				}

				offset.X = x;
				offset.Y = y;
				offset.Z = z;

				Spawn(categroyType, FName(*id), level, bMy, bAbs, offset);
			}
		}
	}
}

void ATestGameMode::SpawnHero(FName id, int32 level, bool bMy, FVector offset, TArray<int32>& inSkillLevelList, TArray<FTestItemInfo>& inItemList)
{
	//if (IsValid(GameInstance))
	{
		heroSkillLevelList.Empty();
		heroItemList.Empty();

		FTransform transform;
		transform.SetLocation(FVector::ZeroVector);

		if (HeroSpawnList.Num() > 0)
		{
			int32 idx = HeroSpawnList[0];
			if (SpawnTransformMap.Contains(idx))
			{
				transform = SpawnTransformMap[idx].GetTransform();
			}
		}

		transform.SetLocation(transform.GetLocation() + offset);

		int32 CommandSlot = INVALID_INDEXNUM;

		bool bFirst = false;
		if (IsValid(GameInstance->PlayStateInfo))
		{
			if (bMy)
			{
				CommandSlot = GameInstance->PlayStateInfo->GetSelectedCharacterCount();
				if (CommandSlot == 0)
				{
					bFirst = true;
					GameInstance->PlayStateInfo->AddCharacterSlotForTestPlay(id, level, inSkillLevelList, inItemList);
				}
				else
				{
					GameInstance->PlayStateInfo->AddCharacterSlotForTestPlay(id, level, inSkillLevelList, inItemList);
				}
			}
			else
			{
				heroSkillLevelList = inSkillLevelList;
				heroItemList = inItemList;
			}
		}

		int32 UserKID = bMy ? KID_TEST_MY : KID_NONE;
		int32 GroupKey = bMy ? GroupKey_SINGLE_MY : GroupKey_SINGLE_ENEMY;
		int32 PartyID = bMy ? PartyID_PC_MY : PartyID_NPC_ENEMY;

		AActor* character = UObjectFunctionLibrary::SpawnCharacter(transform, id, ECharacterCategoryTypeEnum::VE_Hero,
			UserKID, HERO_UD_NONE, GroupKey, PartyID, PLAYER_UK_NONE,
			level, 1, true, CommandSlot);
		USkillEntityComponent* SkillEntityComponent = UCharacterHelpFunctionLibrary::GetSkillEntityComponent(character);
		UBattleEntityComponent* BattleComp = UCharacterHelpFunctionLibrary::GetBattleEntityComponent(character);
		if (IsValid(character))
		{
			if (bMy && bFirst)
			{
				UBaseStageInfo*	BaseStageInfo = UGamePlayBluePrintFunction::GetBaseStageInfo();
				BaseStageInfo->bPlayAlone = true;
				

				FVector SpawnPos = FVector(0, 0, 0);
				ABasePlayerController* PlayerController = UUtilFunctionLibrary::GetBasePlayerController(GameInstance->GetWorld());
				if (IsValid(PlayerController))
				{
					if (IsValid(PlayerController->GetCameraActor()))
						SpawnPos = PlayerController->GetCameraActor()->GetActorLocation();

					float CameraFocusTime = 0.5f;
					float CameraFocusExp = 0.5f;
					PlayerController->ActorSettingOnTheCamera(character, CameraFocusTime, CameraFocusExp);
				}
				URBaseIngameHUD* InGameHud = Cast<URBaseIngameHUD>(UUIFunctionLibrary::GetInGameHUD());
				if (InGameHud && IsValid(InGameHud->SkillSet))
				{
					//InGameHud->SkillSet->SetSelectCharacter(character, true);
					InGameHud->UpdateHeroIconSlot();
					//InGameHud->InitializeAfterSpawnCharacters();
				}
				GameInstance->PlayStateInfo->SetSpawnedCharacter(CommandSlot, character);
				MyHeroList.Add(character);

				ATestPlayerController* TestPC = Cast<ATestPlayerController>(PlayerController);
				if (TestPC)
					TestPC->SetSelectedCharacter(character);

				UUIFunctionLibrary::AttachHUDPage(GameInstance->GetWorld());
				UUIFunctionLibrary::OnShowInGameHUD(GameInstance, true);
				UUIFunctionLibrary::ActivateJoystick(GameInstance->GetWorld(), true);
				
			}
			

			for (int32 Num = 0; Num < inItemList.Num(); Num++)
			{
				if(inItemList.IsValidIndex(Num) == false)
					continue;
				FITEM Item;
				Item.itemUD = FString::FromInt(Num);
				Item.itemId = inItemList[Num].ItemKey;
				BattleComp->EquipItemFITEMTest(EItemEquipPositionEnum(Num), Item);
			}
			BattleComp->GetBattleInfo()->ValidateTotalItemProperty();
			BattleComp->GetBattleInfo()->ValidateFinalCharacterProperty();
		}
	}
}

void ATestGameMode::Spawn(ECharacterCategoryTypeEnum categroyType, FName id, int32 level, bool bMy, bool bAbs, FVector offset)
{
	//if (IsValid(GameInstance))
	{
		FTransform transform;
		if (bAbs)
		{
			transform.SetLocation(FVector::ZeroVector + offset);
		}
		else
		{
			if (MyHeroList.Num() > 0)
			{
				if (MyHeroList[0].IsValid())
				{
					FVector pos = MyHeroList[0]->GetActorLocation();
					FVector addOffset = MyHeroList[0]->GetActorForwardVector() * offset.X + MyHeroList[0]->GetActorRightVector() * offset.Y + MyHeroList[0]->GetActorUpVector() * offset.Z;
					transform.SetLocation(pos + addOffset);
				}
			}
			else
			{
				transform.SetLocation(FVector::ZeroVector + offset);
			}
		}

		int32 UserKID = bMy ? KID_TEST_MY : KID_NONE;
		int32 GroupKey = bMy ? GroupKey_SINGLE_MY : GroupKey_SINGLE_ENEMY;
		int32 PartyID = bMy ? PartyID_PC_MY : PartyID_NPC_ENEMY;

		AActor* SpawnedActor = UObjectFunctionLibrary::SpawnCharacter(transform, id, categroyType, UserKID, HERO_UD_NONE, GroupKey, PartyID, PLAYER_UK_NONE, level);

		UBaseStageInfo* basestageInfo = UGamePlayBluePrintFunction::GetBaseStageInfo();
		if (basestageInfo && SpawnedActor)
		{
			if(!bMy)
				basestageInfo->SetMonster(SpawnedActor);
		}
	}
}

void ATestGameMode::Test()
{
	for (TWeakObjectPtr<AActor>& actor : MyHeroList)
	{
		if (actor.IsValid())
		{
			UEntityRenderComponent* RenderComponent = UCharacterHelpFunctionLibrary::GetEntityRenderComponent(actor.Get());
			if (IsValid(RenderComponent))
			{
				if (IsValid(RenderComponent->GetSlateDamageFloaterComponent()))
				{
					int32 iRnd = FMath::RandRange(0, 6);

					//switch (iRnd)
					//{
					//case 0:
					//	RenderComponent->GetSlateDamageFloaterComponent()->AddDamageFloater(300, EBattleDamageResultEnum::VE_Hit);
					//	break;
					//case 1:
					//	RenderComponent->GetSlateDamageFloaterComponent()->AddDamageFloater(300, EBattleDamageResultEnum::VE_Critical);
					//	break;
					//case 2:
					//	RenderComponent->GetSlateDamageFloaterComponent()->AddDamageFloater(300, EBattleDamageResultEnum::VE_ReceiveHit);
					//	break;
					//case 3:
					//	RenderComponent->GetSlateDamageFloaterComponent()->AddDamageFloater(300, EBattleDamageResultEnum::VE_ReceiveCritical);
					//	break;
					//case 4:
					//	RenderComponent->GetSlateDamageFloaterComponent()->AddDamageFloater(300, EBattleDamageResultEnum::VE_Miss);
					//	break;
					//case 5:
					//	RenderComponent->GetSlateDamageFloaterComponent()->AddDamageFloater(300, EBattleDamageResultEnum::VE_Pierce);
					//	break;
					//}

					//RenderComponent->GetSlateDamageFloaterComponent()->AddDamageFloater(300, EBattleDamageResultEnum::VE_Critical);
					//RenderComponent->GetSlateDamageFloaterComponent()->AddDamageFloater(300, EBattleDamageResultEnum::VE_ReceiveHit);
					//RenderComponent->GetSlateDamageFloaterComponent()->AddDamageFloater(300, EBattleDamageResultEnum::VE_ReceiveCritical);
					//RenderComponent->GetSlateDamageFloaterComponent()->AddDamageFloater(300, EBattleDamageResultEnum::VE_Miss);
					//RenderComponent->GetSlateDamageFloaterComponent()->AddDamageFloater(300, EBattleDamageResultEnum::VE_Pierce);
				}
			}
		}
	}
}
