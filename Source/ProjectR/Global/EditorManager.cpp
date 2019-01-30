// Fill out your copyright notice in the Description page of Project Settings.

#include "ProjectR.h"
#include "EditorManager.h"

#include "GlobalIntegrated.h"

//#include "Global/JsonManager.h"

#include "Table/MapTableInfo.h"
#include "Editor/SpawnerEditor.h"
#include "UtilFunctionIntegrated.h"

UEditorManager::UEditorManager()
{
	for (int32 i = 0; i < (int32)EEditorEnum::VE_Max; ++i)
	{
#if WITH_EDITOR
		switch ((EEditorEnum)i)
		{
		case EEditorEnum::VE_SpawnerEditor:
			{
				static ConstructorHelpers::FObjectFinder<UClass> Editor_BP(TEXT("Blueprint'/Game/Blueprints/Editor/BP_SpawnerEditor.BP_SpawnerEditor_C'"));
				if (Editor_BP.Object)
				{
					EditorBlueprintList[i] = (UClass*)Editor_BP.Object;
				}
			}
		break;
		default:
		{
			EditorBlueprintList[i] = nullptr;
		}
		break;
		}
#else
		EditorBlueprintList[i] = nullptr;
#endif
	}
}

void UEditorManager::Initialize(/*URGameInstance* instance, */EEditorEnum editorType)
{
	EditorType = editorType;
//	RGameInstance = instance;

	CreateEditorActor();
}

void UEditorManager::CreateEditorActor()
{
	URGameInstance* RGameInstance = RGAMEINSTANCE(this);
	
	{
		if (!IsValid(EditorActor))
		{
			UWorld* world = RGameInstance->GetWorld();
			if (world)
			{
				TArray<AActor* > arraylist;
				UGameplayStatics::GetAllActorsOfClass(world, ABaseEditor::StaticClass(), arraylist);
				if (arraylist.Num() == 0)
				{
					FActorSpawnParameters SpawnParams;
					SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

					int32 index = (int32)EditorType;
					if (index < (int32)EEditorEnum::VE_Max)
					{
						if (IsValid(EditorBlueprintList[index]))
						{
							EditorActor = world->SpawnActor<ABaseEditor>(EditorBlueprintList[index], SpawnParams);
							if (IsValid(EditorActor))
							{
								UE_LOG(LogBattle, Log, TEXT("EditorActor Create"));
							}
						}
					}
				}
			}
		}
	}
}

void UEditorManager::LoadMap(FName keyName)
{
	URGameInstance* RGameInstance = RGAMEINSTANCE(this);
	
	{
		FMapTableInfo* MapTableInfo = UTableManager::GetInstancePtr()->GetMapRow(keyName);
		if (MapTableInfo)
		{
			LoadMapKey = keyName;
			if (IsValid(EditorActor))
			{
				EditorActor->Destroy();
				EditorActor = nullptr;
			}
			UMapFunctionLibrary::LoadLevel(MapTableInfo->P_Map, true);
			EditorState = eEditorMgrState::eNormalLoadState;
		}
	}
}

void UEditorManager::RefreshState()
{
	URGameInstance* RGameInstance = RGAMEINSTANCE(this);
	switch (EditorState)
	{
	case eEditorMgrState::eNormalLoadState:
	{
		SubLevelCount = 0;
		if (LoadMapKey.IsValid() && !LoadMapKey.IsNone())
		{
			TArray< FName >	SubLevelList;
			FMapTableInfo* MapTableInfo = UTableManager::GetInstancePtr()->GetMapRow(LoadMapKey);
			if (MapTableInfo)
			{
				SubLevelList.Emplace(MapTableInfo->Sub_Level_Art);
				SubLevelList.Emplace(MapTableInfo->Sub_Level_Design);
			}

			for (auto LevelItor = SubLevelList.CreateConstIterator(); LevelItor; ++LevelItor)
			{
				FName SubLevel = *LevelItor;
				if (!SubLevel.IsNone())
				{
					UMapFunctionLibrary::LoadLevel(SubLevel, true, FName("OnCompleteLoadingLevel"), this);
					++SubLevelCount;
				}
			}
		}

		if (SubLevelCount == 0)
			LoadComplate();
	}
	break;
	}
}

void UEditorManager::OnCompleteLoadingLevel()
{
	--SubLevelCount;

	if (SubLevelCount > 0)
		return;

	LoadComplate();
}

void UEditorManager::LoadComplate()
{
// 	switch (EditorState)
// 	{
// 	case eEditorMgrState::eNormalLoadState:
// 	{
// 		CreateEditorActor();
// 
// 		ASpawnerEditor* SpawnerEditor = Cast<ASpawnerEditor>(EditorActor);
// 		// Socket Load
// 		if (IsValid(SpawnerEditor))
// 		{
// 			SpawnerEditor->SetLoadMapName(LoadMapKey);
// 		}
// 	}
// 	break;
// 	}
}