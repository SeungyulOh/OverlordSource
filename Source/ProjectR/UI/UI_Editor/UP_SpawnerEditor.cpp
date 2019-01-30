// Fill out your copyright notice in the Description page of Project Settings.

#include "ProjectR.h"
#include "SharedConstants/GlobalConstants.h"
#include "UP_SpawnerEditor.h"
#include "Editor/SpawnerEditor.h"
#include "Editor/BaseEditor.h"
#include "UI/Control/RScrollView.h"
#include "UI/Control/RScrollItemData.h"
#include "UI/UI_Editor/EditorScrollItemData.h"
#include "UI/UI_Editor/UC_EquipItemButton.h"
#include "GlobalIntegrated.h"
#include "UtilFunctionIntegrated.h"
#include "GameInfo/PlayStateInfo.h"
#include "CustomStructures/SpawnStructures/SpawnerInfo.h"
#include "CustomStructures/SpawnStructures/StageSpawnInfo.h"
#include "Character/Interfaces/EntityBaseProperty.h"
#include "Character/Components/FunctionalComponents/BattleEntityComponent.h"
#include "UtilFunctionIntegrated.h"
#include "JsonObjectConverter.h"
#include "XmlNode.h"
#include "XmlFile.h"
#include "GameState/IGameState.h"
#include "ContentGameMode/Modes/TestGameMode.h"
#include "Network/PacketDataStructures.h"
#include "Runtime/Engine/Classes/Kismet/KismetSystemLibrary.h"

void UUP_SpawnerEditor::NativeConstruct()
{
	Super::NativeConstruct();

	InvalidateData();
}

void UUP_SpawnerEditor::InvalidateData()
{
	TestRightScrollInvlidate();
	TestSpawnInvlidate();

	if (IsValid(PopupPanel))
	{
		PopupPanel->SetVisibility(ESlateVisibility::Collapsed);
	}
}

void UUP_SpawnerEditor::ProcessOption()
{
	if (CoolTimeZeroCheckBox)
	{
		bool bCheck = (CoolTimeZeroCheckBox->GetCheckedState() == ECheckBoxState::Checked);
		
		URGameInstance* instance = RGAMEINSTANCE(this);
		if (instance)
		{
			auto ConsoleCheatManager = instance->ConsoleCheatManager;
			if (ConsoleCheatManager)
			{
				if (bCheck)
				{
					ConsoleCheatManager->CoolTimeZero();
				}
				else
				{
					ConsoleCheatManager->bCoolTimeZero = false;
				}
			}
		}
	}
}

void UUP_SpawnerEditor::TestRightScrollInvlidate()
{
	if (IsValid(ScrollList))
	{
		ScrollList->ItemDataList.Reset();

		switch (CurTestRightScrollType)
		{
			case ETestRightScrollTypeEnum::VE_Sequence:
			{
				int32 count = 1;
				TArray<FName> tableList = UTableManager::GetInstancePtr()->GetResourceSequenceRowNames();
				for (FName& name : tableList)
				{
					FResourceSequenceTableInfo* SequenceTableInfo = UTableManager::GetInstancePtr()->GetResourceSequenceRow(name);
					if (SequenceTableInfo)
					{
						UEditorScrollItemData* ItemData = NewObject< UEditorScrollItemData >((UObject*)GetTransientPackage(), UEditorScrollItemData::StaticClass());
						ItemData->SItemType = EEditorSItemTypeEnum::VE_TestSequence;
						ItemData->KeyName = name;
						ItemData->DisplayName = name;
						ItemData->BuffStackCount = count++;

						ScrollList->ItemDataList.Add(ItemData);
					}
				}
			}
			break;
		}
		ScrollList->Init();
		ScrollList->InvalidateData();
	}
}

void UUP_SpawnerEditor::TestSpawnInvlidate()
{
	EnemyCheckBoxOnOff();

	if (IsValid(TestSpawnScrollList))
	{
		TestSpawnScrollList->ItemDataList.Reset();
		if (CurTestSpawnType == ETestSpawnTypeEnum::VE_Npc || CurTestSpawnType == ETestSpawnTypeEnum::VE_Minion || CurTestSpawnType == ETestSpawnTypeEnum::VE_NeutralNpc)
		{
			TArray<FName> nameList = UTableManager::GetInstancePtr()->GetCharacterNpcRowNames();

			for (FName& name : nameList)
			{
				FCharacterTableInfo* tableInfo = UTableManager::GetInstancePtr()->GetCharacterNpcRow(name);
				if (tableInfo)
				{
					UEditorScrollItemData* ItemData = NewObject< UEditorScrollItemData >((UObject*)GetTransientPackage(), UEditorScrollItemData::StaticClass());
					ItemData->SItemType = EEditorSItemTypeEnum::VE_TestSpawn;
					ItemData->KeyName = name;
					ItemData->DisplayName = FName(*tableInfo->DisplayName.ToString());
					ItemData->BuffStackCount = FCString::Atoi(*name.ToString());
					ItemData->PortraitTextureRef = nullptr;
					ItemData->PortraitRef = UUIFunctionLibrary::GetIconImageFromTable(tableInfo->PortraitTextureKey);
					if (CurTestSpawnType == ETestSpawnTypeEnum::VE_Npc)
						ItemData->CharacterType = ECharacterCategoryTypeEnum::VE_Npc;
					else if (CurTestSpawnType == ETestSpawnTypeEnum::VE_Minion)
						ItemData->CharacterType = ECharacterCategoryTypeEnum::VE_Minion;
					else if (CurTestSpawnType == ETestSpawnTypeEnum::VE_NeutralNpc)
						ItemData->CharacterType = ECharacterCategoryTypeEnum::VE_NeutralNpc;

					TestSpawnScrollList->ItemDataList.Add(ItemData);
				}
			}
		}
		//else if(CurTestSpawnType == ETestSpawnTypeEnum::VE_BObject)
		//{
		//	TArray<FName> nameList = UTableManager::GetInstancePtr()->GetBattleObjectRowNames();
		//	for (FName& name : nameList)
		//	{
		//		FBattleObjectTableInfo* tableInfo = UTableManager::GetInstancePtr()->GetBattleObjectRow(name);
		//		if (tableInfo)
		//		{
		//			UEditorScrollItemData* ItemData = NewObject< UEditorScrollItemData >((UObject*)GetTransientPackage(), UEditorScrollItemData::StaticClass());
		//			ItemData->SItemType = EEditorSItemTypeEnum::VE_TestSpawn;
		//			ItemData->KeyName = name;
		//			ItemData->DisplayName = FName(*tableInfo->DisplayName.ToString());
		//			ItemData->BuffStackCount = FCString::Atoi(*name.ToString());
		//			ItemData->PortraitTextureRef = nullptr;
		//			ItemData->PortraitRef = UUIFunctionLibrary::GetIconImageFromTable(tableInfo->PortraitTextureKey);
		//			ItemData->CharacterType = ECharacterCategoryTypeEnum::VE_Object;

		//			TestSpawnScrollList->ItemDataList.Add(ItemData);
		//		}
		//	}
		//}
		else if (CurTestSpawnType == ETestSpawnTypeEnum::VE_Hero)
		{
			TArray<FName> nameList = UTableManager::GetInstancePtr()->GetCharacterRowNames();
			for (FName& name : nameList)
			{
				FCharacterTableInfo* tableInfo = UTableManager::GetInstancePtr()->GetCharacterRow(name);
				if (tableInfo)
				{
					UEditorScrollItemData* ItemData = NewObject< UEditorScrollItemData >((UObject*)GetTransientPackage(), UEditorScrollItemData::StaticClass());
					ItemData->SItemType = EEditorSItemTypeEnum::VE_TestSpawn;
					ItemData->KeyName = name;
					ItemData->DisplayName = FName(*tableInfo->DisplayName.ToString());
					ItemData->BuffStackCount = FCString::Atoi(*name.ToString());
					ItemData->PortraitTextureRef = nullptr;
					ItemData->PortraitRef = UUIFunctionLibrary::GetIconImageFromTable(tableInfo->PortraitTextureKey);
					ItemData->CharacterType = ECharacterCategoryTypeEnum::VE_Hero;

					TestSpawnScrollList->ItemDataList.Add(ItemData);

				}
			}
		}

		TestSpawnScrollList->Init();
		TestSpawnScrollList->InvalidateData();
	}
}

void UUP_SpawnerEditor::TestSpawn()
{
	if (IsValid(TestSpawnScrollList))
	{
		UEditorScrollItemData* ItemData = Cast<UEditorScrollItemData>(TestSpawnScrollList->GetSelectedScrollItemDataOnly());
		if (IsValid(ItemData))
		{
			if (ItemData->SItemType == EEditorSItemTypeEnum::VE_TestSpawn)
			{
				URGameInstance* instance = RGAMEINSTANCE(this);
				if (IsValid(instance))
				{
					if (IsValid(instance->EditorManager))
					{
						ATestGameMode* pGameMode = Cast<ATestGameMode>(UGameplayStatics::GetGameMode(GetWorld()));
						if (IsValid(pGameMode))
						{
							FString strText;
							switch (ItemData->CharacterType)
							{
							case ECharacterCategoryTypeEnum::VE_Npc:
								strText = "npc ";
								break;
							case ECharacterCategoryTypeEnum::VE_Minion:
								strText = "minion ";
								break;
							case ECharacterCategoryTypeEnum::VE_NeutralNpc:
								strText = "neutralnpc ";
								break;
							case ECharacterCategoryTypeEnum::VE_Object:
								strText = "bobject ";
								break;
							case ECharacterCategoryTypeEnum::VE_Hero:
								strText = "hero ";
								break;
							}

							if (IsValid(EnemyCheckBox))
							{
								if (EnemyCheckBox->GetCheckedState() == ECheckBoxState::Checked)
								{
									strText += "enemy ";
								}
								else
								{
									strText += "my ";
								}
							}
							else
							{
								strText += "enemy ";
							}

							strText += ItemData->KeyName.ToString();
							strText += " ";

							if (IsValid(LevelTextBox))
							{
								strText += LevelTextBox->GetText().ToString();
								strText += " ";
							}
							else
							{
								strText += "1 ";
							}

							if (ItemData->CharacterType != ECharacterCategoryTypeEnum::VE_Hero)
							{
								if (IsValid(HeroOffsetCheckBox))
								{
									if (HeroOffsetCheckBox->GetCheckedState() == ECheckBoxState::Checked)
									{
										strText += "off ";
									}
									else
									{
										strText += "abs ";
									}
								}
								else
								{
									strText += "abs ";
								}
							}

							if (IsValid(XTextBox) && IsValid(YTextBox) && IsValid(ZTextBox))
							{
								strText += XTextBox->GetText().ToString();
								strText += " ";
								strText += YTextBox->GetText().ToString();
								strText += " ";
								strText += ZTextBox->GetText().ToString();
							}
							else
							{
								strText += "0 0 0";
							}

							if (ItemData->CharacterType == ECharacterCategoryTypeEnum::VE_Hero)
							{
								if (IsValid(HeroLevelCheckBox) && HeroLevelCheckBox->GetCheckedState() == ECheckBoxState::Checked)
								{
									if (IsValid(A1LevelTextBox) && IsValid(A2LevelTextBox) && IsValid(A3LevelTextBox) && IsValid(P1LevelTextBox) && IsValid(P2LevelTextBox))
									{
										strText += " skilllevel,";
										strText += A1LevelTextBox->GetText().ToString();
										strText += ",";
										strText += A2LevelTextBox->GetText().ToString();
										strText += ",";
										strText += A3LevelTextBox->GetText().ToString();
										strText += ",";
										strText += P1LevelTextBox->GetText().ToString();
										strText += ",";
										strText += P2LevelTextBox->GetText().ToString();
									}
									else
									{
										strText += " skilllevel,-1,-1,-1,-1,-1";
									}
								}
								else
								{
									strText += " skilllevel,-1,-1,-1,-1,-1";
								}

								if (IsValid(HeroItemCheckBox) && HeroItemCheckBox->GetCheckedState() == ECheckBoxState::Checked)
								{
									if (IsValid(ItemButton01) && IsValid(ItemButton02) && IsValid(ItemButton03) && IsValid(ItemButton04) && IsValid(ItemButton05))
									{
										strText += " item,";
										if (ItemButton01->CurSelectItemKey.IsNone())
										{
											strText += "none:,";
										}
										else
										{
											strText += ItemButton01->CurSelectItemKey.ToString();
											strText += ":";
											for (int32 k = 0; k < ItemButton01->GemKeyList.Num(); ++k)
											{
												strText += ItemButton01->GemKeyList[k].ToString();
												strText += ":";
											}
											strText += ",";
										}

										if (ItemButton02->CurSelectItemKey.IsNone())
										{
											strText += "none:,";
										}
										else
										{
											strText += ItemButton02->CurSelectItemKey.ToString();
											strText += ":";
											for (int32 k = 0; k < ItemButton02->GemKeyList.Num(); ++k)
											{
												strText += ItemButton02->GemKeyList[k].ToString();
												strText += ":";
											}
											strText += ",";
										}

										if (ItemButton03->CurSelectItemKey.IsNone())
										{
											strText += "none:,";
										}
										else
										{
											strText += ItemButton03->CurSelectItemKey.ToString();
											strText += ":";
											for (int32 k = 0; k < ItemButton03->GemKeyList.Num(); ++k)
											{
												strText += ItemButton03->GemKeyList[k].ToString();
												strText += ":";
											}
											strText += ",";
										}

										if (ItemButton04->CurSelectItemKey.IsNone())
										{
											strText += "none:";
										}
										else
										{
											strText += ItemButton04->CurSelectItemKey.ToString();
											strText += ":";
											for (int32 k = 0; k < ItemButton04->GemKeyList.Num(); ++k)
											{
												strText += ItemButton04->GemKeyList[k].ToString();
												strText += ":";
											}
										}

										if (ItemButton05->CurSelectItemKey.IsNone())
										{
											strText += "none:";
										}
										else
										{
											strText += ItemButton05->CurSelectItemKey.ToString();
											strText += ":";
											for (int32 k = 0; k < ItemButton05->GemKeyList.Num(); ++k)
											{
												strText += ItemButton05->GemKeyList[k].ToString();
												strText += ":";
											}
										}
									}
								}
								else
								{
									strText += " item,none:,none:,none:,none:";
								}
							}

							pGameMode->ConsoleCommand(strText);
						}
					}
				}
			}
		}
	}
}

void UUP_SpawnerEditor::PlaySequence()
{
	if (IsValid(ScrollList))
	{
		UEditorScrollItemData* ItemData = Cast<UEditorScrollItemData>(ScrollList->GetSelectedScrollItemDataOnly());
		if (IsValid(ItemData))
		{
			if (ItemData->SItemType == EEditorSItemTypeEnum::VE_TestSequence)
			{
				URGameInstance* instance = RGAMEINSTANCE(this);
				if (IsValid(instance))
				{
					if (IsValid(instance->EditorManager))
					{
						ATestGameMode* pGameMode = Cast<ATestGameMode>(UGameplayStatics::GetGameMode(GetWorld()));
						if (IsValid(pGameMode))
						{
							pGameMode->ConsoleCommand("sequence " + ItemData->KeyName.ToString());
						}
					}
				}
			}
		}
	}
}

// void UUP_SpawnerEditor::PlayInfoUIRefresh()
// {
// 	if (IsValid(PlayInfoCheckBox))
// 	{
// 		URGameInstance* instance = RGAMEINSTANCE(this);
// 		if (IsValid(instance))
// 		{
// 			if (IsValid(instance->EditorManager))
// 			{
// 				ASpawnerEditor* SpawnerEditor = Cast<ASpawnerEditor>(instance->EditorManager->GetEditorActor());
// 				if (IsValid(SpawnerEditor))
// 				{
// 					bool bCheck = false;
// 					if (PlayInfoCheckBox->GetCheckedState() == ECheckBoxState::Checked)
// 						bCheck = true;
// 
// 					SpawnerEditor->SetPlayTestInfoUI(bCheck);
// 				}
// 			}
// 		}
// 	}
// }

void UUP_SpawnerEditor::MapLoad()
{
	if (IsValid(ScrollList))
	{
		UEditorScrollItemData* ItemData = Cast<UEditorScrollItemData>(ScrollList->GetSelectedScrollItemDataOnly());
		if (IsValid(ItemData))
		{
			if (ItemData->SItemType == EEditorSItemTypeEnum::VE_TestPlay)
			{
				URGameInstance* instance = RGAMEINSTANCE(this);
				if (IsValid(instance))
				{
					if (IsValid(instance->EditorManager))
					{
						ATestGameMode* pGameMode = Cast<ATestGameMode>(UGameplayStatics::GetGameMode(GetWorld()));
						if (IsValid(pGameMode))
						{
							pGameMode->ConsoleCommand("loadmap " + ItemData->KeyName.ToString());
						}
					}
				}
			}
		}
	}
}

void UUP_SpawnerEditor::Refresh()
{
#if WITH_EDITOR
	FSlateApplication::Get().GetPlatformApplication()->Cursor->Show(true);
#endif
}

void UUP_SpawnerEditor::ReSave()
{
	TArray<FString> files;
	//FString FullPath = FPaths::GameSavedDir() + "Spawner/*.json";	
	//IFileManager::Get().FindFiles(files, *FullPath, true, false);

	FString FullPath = FPaths::ProjectSavedDir() + "../../../DesignDocument/Tables/Xmls/";
	IFileManager::Get().FindFilesRecursive(files, *FullPath, TEXT("*.json"), true, false);	

	for (FString& str : files)
	{
		FString strSpawnerInfo;
		//FString FullPath1 = FPaths::GameSavedDir() + "Spawner/" + str;
		FString FullPath1 = str;
		if (FFileHelper::LoadFileToString(strSpawnerInfo, *FullPath1))
		{
			FStageSpawnInfo info;
			if (FJsonObjectConverter::JsonObjectStringToUStruct<FStageSpawnInfo>(strSpawnerInfo, &info, 0, 0))
			{
				info.UniqueName = FName(*FPaths::GetBaseFilename(FullPath1));
				if (FJsonObjectConverter::UStructToJsonObjectString(FStageSpawnInfo::StaticStruct(), &info, strSpawnerInfo, 0, 0))
				{					
					if (FFileHelper::SaveStringToFile(strSpawnerInfo, *FullPath1))
					{
#if WITH_EDITOR
						FString strText;
						strText = FString::Printf(TEXT("UUP_SpawnerEditor::ReSave %s"), *FullPath1);
						UKismetSystemLibrary::PrintString(this, strText);
#endif

					}
				}
			}
		}
	}
}

void UUP_SpawnerEditor::SaveStageSpawn()
{
	TArray<FString> files;
	FString FullPath = FPaths::ProjectSavedDir() + "Spawner/*.json";
	IFileManager::Get().FindFiles(files, *FullPath, true, false);

	FStageSpawnInfoList infoList;

	for (FString& str : files)
	{
		FString strSpawnerInfo;
		FString FullPath1 = FPaths::ProjectSavedDir() + "Spawner/" + str;
		if (FFileHelper::LoadFileToString(strSpawnerInfo, *FullPath1))
		{
			FStageSpawnInfo info;
			if (FJsonObjectConverter::JsonObjectStringToUStruct<FStageSpawnInfo>(strSpawnerInfo, &info, 0, 0))
			{
				infoList.InfoList.Add(info);
			}
		}
	}

	if (infoList.InfoList.Num() > 0)
	{
		FString strSpawnerInfo;
		if (FJsonHelper::TArrayUStructToJsonObjectString<FStageSpawnInfo>(FStageSpawnInfo::StaticStruct(), infoList.InfoList, strSpawnerInfo, 0, 0))
		{
			FullPath = FPaths::ProjectSavedDir() + "Spawners/Spawners.json";
			if (FFileHelper::SaveStringToFile(strSpawnerInfo, *FullPath))
			{
#if WITH_EDITOR
				FString strText;
				strText = FString::Printf(TEXT("UUP_SpawnerEditor::SaveStageSpawn %s Save"), *FullPath);
				UKismetSystemLibrary::PrintString(this, strText);
#endif
			}
		}
	}
}

bool UUP_SpawnerEditor::CopyStageSpawners()
{
	TArray<FString> files;
	FString FullPath = FPaths::ProjectSavedDir() + "Spawners/*.json";
	IFileManager::Get().FindFiles(files, *FullPath, true, false);

	for (FString& str : files)
	{
		FString strBlockInfoList;
		FString FullPath1 = FPaths::ProjectSavedDir() + "Spawners/" + str;
		if (FFileHelper::LoadFileToString(strBlockInfoList, *FullPath1))
		{
			FString FullPath2 = FPaths::ProjectSavedDir() + "../../../DesignDocument/Tables/Jsons/" + str;
			if (FFileHelper::SaveStringToFile(strBlockInfoList, *FullPath2))
			{
				UE_LOG(LogBattle, Log, TEXT("UUP_SpawnerEditor::CopyBlocks %s Save"), *FullPath2);

#if WITH_EDITOR
				FString strText;
				strText = FString::Printf(TEXT("UUP_SpawnerEditor::CopyBlocks %s Save"), *FullPath2);
				UKismetSystemLibrary::PrintString(this, strText);
#endif
			}
		}
	}

	return true;
}

bool UUP_SpawnerEditor::CopyStageSpawnersUnreal()
{
	TArray<FString> files;
	FString FullPath = FPaths::ProjectSavedDir() + "../../../DesignDocument/Tables/Jsons/*.json";
	IFileManager::Get().FindFiles(files, *FullPath, true, false);

	for (FString& str : files)
	{
		FString strBlockInfoList;
		FString FullPath1 = FPaths::ProjectSavedDir() + "../../../DesignDocument/Tables/Jsons/" + str;
		if (FFileHelper::LoadFileToString(strBlockInfoList, *FullPath1))
		{
			FString FullPath2 = FPaths::ProjectSavedDir() + "Spawners/" + str;
			if (FFileHelper::SaveStringToFile(strBlockInfoList, *FullPath2))
			{

				UE_LOG(LogBattle, Log, TEXT("UUP_SpawnerEditor::CopyBlocks %s Save"), *FullPath2);
#if WITH_EDITOR
				FString strText;
				strText = FString::Printf(TEXT("UUP_SpawnerEditor::CopyBlocksUnreal %s Save"), *FullPath2);
				UKismetSystemLibrary::PrintString(this, strText);
#endif
			}
		}
	}

	return true;
}

void UUP_SpawnerEditor::ConvertCSVtoJSON()
{
	//FString FullPath = FPaths::GameSavedDir() + "../../../DesignDocument/Tables/Campaignspawn/DT_CampaignSpawn.csv";
	//FString strCsv;
	//if (FFileHelper::LoadFileToString(strCsv, *FullPath))
	//{
	//	UDataTable* NewTable = NewObject<UDataTable>((UObject*)GetTransientPackage(), UDataTable::StaticClass());
	//	NewTable->RowStruct = FSpawnTableInfo::StaticStruct();
	//	//NewTable->AssetImportData->Update(FullPath);

	//	TArray<FString> strArray;
	//	strArray = NewTable->CreateTableFromCSVString(strCsv);

	//	static const FString ContextString(TEXT("GENERAL"));
	//	TArray<FName> nameList = NewTable->GetRowNames();

	//	TMap< FName, TArray<FName> > map;
	//	for (FName name : nameList)
	//	{
	//		FSpawnTableInfo* RowInfo = NewTable->FindRow<FSpawnTableInfo>(name, ContextString);
	//		if (RowInfo)
	//		{
	//			if (map.Find(RowInfo->Name))
	//			{
	//				map[RowInfo->Name].Add(name);
	//			}
	//			else
	//			{
	//				TArray<FName> list;
	//				map.Add(RowInfo->Name, list);
	//				map[RowInfo->Name].Add(name);
	//			}
	//		}
	//	}
	//}
}

void UUP_SpawnerEditor::ConvertJSONtoXML()
{
	TArray<FString> files;
	FString FullPath = FPaths::ProjectSavedDir() + "../../../DesignDocument/Tables/Jsons/spawnInfoManager/spawnInfoManager/JsonFiles/";
	IFileManager::Get().FindFilesRecursive(files, *FullPath, TEXT("*.json"), true, false);	

	for (FString& str : files)
	{
		FString strInfoList;
		if (FFileHelper::LoadFileToString(strInfoList, *str))
		{
			FStageSpawnInfo info;
			if (FJsonObjectConverter::JsonObjectStringToUStruct<FStageSpawnInfo>(strInfoList, &info, 0, 0))
			{
			}
		}
	}
}

void UUP_SpawnerEditor::FindNameList()
{
	if (IsValid(TestSpawnScrollList) && IsValid(NameFindTextBox))
	{
		FString text = NameFindTextBox->GetText().ToString();
		if (text.Len() > 0)
		{
			int32 SelectIdx = -1;
			URScrollItemData* SelectItemData = TestSpawnScrollList->GetSelectedScrollItemDataOnly();
			if (IsValid(SelectItemData))
			{
				for (int32 i = 0; i < TestSpawnScrollList->ItemDataList.Num(); ++i)
				{
					if (IsValid(TestSpawnScrollList->ItemDataList[i]) && TestSpawnScrollList->ItemDataList[i] == SelectItemData)
					{
						SelectIdx = i;
					}
				}
			}

			if (SelectIdx == -1 || !TestSpawnScrollList->ItemDataList.IsValidIndex(SelectIdx))
			{
				for (int32 i = 0; i < TestSpawnScrollList->ItemDataList.Num(); ++i)
				{
					UEditorScrollItemData* ItemData = Cast<UEditorScrollItemData>(TestSpawnScrollList->ItemDataList[i]);
					if (IsValid(ItemData))
					{
						//if (ItemData->DisplayName.ToString() == text)
						//{
						//	TestSpawnScrollList->SetScrollPos(ItemData);
						//	break;
						//}
						if (ItemData->DisplayName.ToString().Contains(text))
						{
							TestSpawnScrollList->SetScrollPos(ItemData);
							TestSpawnScrollList->SetSelectScrollListData(ItemData);
							break;
						}
					}
				}			
			}
			else
			{
				bool bFind = false;
				for (int32 i = SelectIdx + 1; i < TestSpawnScrollList->ItemDataList.Num(); ++i)
				{
					if (TestSpawnScrollList->ItemDataList.IsValidIndex(i))
					{
						UEditorScrollItemData* ItemData = Cast<UEditorScrollItemData>(TestSpawnScrollList->ItemDataList[i]);
						if (IsValid(ItemData))
						{
							//if (ItemData->DisplayName.ToString() == text)
							//{
							//	TestSpawnScrollList->SetScrollPos(ItemData);
							//	break;
							//}
							if (ItemData->DisplayName.ToString().Contains(text))
							{
								TestSpawnScrollList->SetScrollPos(ItemData);
								TestSpawnScrollList->SetSelectScrollListData(ItemData);
								bFind = true;
								break;
							}
						}
					}
				}
			}
		}
	}
}

void UUP_SpawnerEditor::IdNameList()
{
	if (IsValid(TestSpawnScrollList) && IsValid(IdFindTextBox))
	{
		FString text = IdFindTextBox->GetText().ToString();
		if (text.Len() > 0)
		{
			int32 id = FCString::Atoi(*text);
			for (int32 i = 0; i < TestSpawnScrollList->ItemDataList.Num(); ++i)
			{
				UEditorScrollItemData* ItemData = Cast<UEditorScrollItemData>(TestSpawnScrollList->ItemDataList[i]);
				if (IsValid(ItemData))
				{					
					if(ItemData->BuffStackCount == id)
					{
						TestSpawnScrollList->SetScrollPos(ItemData); 
						TestSpawnScrollList->SetSelectScrollListData(ItemData);
						break;
					}
				}
			}
		}
	}
}

void UUP_SpawnerEditor::EnemyCheckBoxOnOff()
{
	if (IsValid(HeroInfoPanel) && IsValid(EnemyCheckBox))
	{
		if (CurTestSpawnType == ETestSpawnTypeEnum::VE_Hero)
		{
			HeroInfoPanel->SetVisibility(ESlateVisibility::Visible);
		}
		else
		{
			HeroInfoPanel->SetVisibility(ESlateVisibility::Collapsed);
		}
	}
}

void UUP_SpawnerEditor::CheckBoxOnOff()
{
	if (IsValid(HeroLevelCheckBox))
	{
		if (HeroLevelCheckBox->GetCheckedState() == ECheckBoxState::Checked)
		{
			if (IsValid(A1LevelTextBox))
				A1LevelTextBox->SetIsEnabled(true);
			if (IsValid(A2LevelTextBox))
				A2LevelTextBox->SetIsEnabled(true);
			if (IsValid(A3LevelTextBox))
				A3LevelTextBox->SetIsEnabled(true);
			if (IsValid(P1LevelTextBox))
				P1LevelTextBox->SetIsEnabled(true);
			if (IsValid(P2LevelTextBox))
				P2LevelTextBox->SetIsEnabled(true);
		}
		else
		{
			if (IsValid(A1LevelTextBox))
				A1LevelTextBox->SetIsEnabled(false);
			if (IsValid(A2LevelTextBox))
				A2LevelTextBox->SetIsEnabled(false);
			if (IsValid(A3LevelTextBox))
				A3LevelTextBox->SetIsEnabled(false);
			if (IsValid(P1LevelTextBox))
				P1LevelTextBox->SetIsEnabled(false);
			if (IsValid(P2LevelTextBox))
				P2LevelTextBox->SetIsEnabled(false);
		}
	}
}

void UUP_SpawnerEditor::ItemCheckBoxOnOff()
{
	if (IsValid(HeroItemCheckBox))
	{
		if (HeroItemCheckBox->GetCheckedState() == ECheckBoxState::Checked)
		{
			if (IsValid(ItemButton01))
				ItemButton01->SetIsEnabled(true);
			if (IsValid(ItemButton02))
				ItemButton02->SetIsEnabled(true);
			if (IsValid(ItemButton03))
				ItemButton03->SetIsEnabled(true);
			if (IsValid(ItemButton04))
				ItemButton04->SetIsEnabled(true);
			if (IsValid(ItemButton05))
				ItemButton05->SetIsEnabled(true);
		}
		else
		{
			if (IsValid(ItemButton01))
				ItemButton01->SetIsEnabled(false);
			if (IsValid(ItemButton02))
				ItemButton02->SetIsEnabled(false);
			if (IsValid(ItemButton03))
				ItemButton03->SetIsEnabled(false);
			if (IsValid(ItemButton04))
				ItemButton04->SetIsEnabled(false);
			if (IsValid(ItemButton05))
				ItemButton05->SetIsEnabled(false);
		}
	}
}

void UUP_SpawnerEditor::ShowSelectItem(UUC_EquipItemButton* InButton)
{
	CurEquipItemButton = InButton;

	EItemEquipPositionEnum EquipPosition = EItemEquipPositionEnum::VE_Weapon;

	if (CurEquipItemButton == ItemButton01)
		EquipPosition = EItemEquipPositionEnum::VE_Weapon;
	if(CurEquipItemButton == ItemButton02)
		EquipPosition = EItemEquipPositionEnum::VE_Armor;
	if (CurEquipItemButton == ItemButton03)
		EquipPosition = EItemEquipPositionEnum::VE_Pants;
	if (CurEquipItemButton == ItemButton04)
		EquipPosition = EItemEquipPositionEnum::VE_Ring;
	if (CurEquipItemButton == ItemButton05)
		EquipPosition = EItemEquipPositionEnum::VE_Necklace;

	if (IsValid(CurEquipItemButton))
	{
		if (IsValid(Gem01ScrollView) && IsValid(Gem02ScrollView) && IsValid(Gem03ScrollView))
		{
			Gem01ScrollView->SetVisibility(ESlateVisibility::Collapsed);
			Gem02ScrollView->SetVisibility(ESlateVisibility::Collapsed);
			Gem03ScrollView->SetVisibility(ESlateVisibility::Collapsed);
		}
		if (IsValid(PopupPanel) && IsValid(PopupScrollList))
		{
			PopupPanel->SetVisibility(ESlateVisibility::Visible);
			PopupScrollList->OnScrollViewSelectedItemData.RemoveDynamic(this, &UUP_SpawnerEditor::OnPopupSelectScrollItem);
			PopupScrollList->OnScrollViewSelectedItemData.AddDynamic(this, &UUP_SpawnerEditor::OnPopupSelectScrollItem);
			PopupScrollList->ItemDataList.Empty();
			if (IsValid(TestSpawnScrollList))
			{
				UEditorScrollItemData* ItemDataNone = NewObject< UEditorScrollItemData >((UObject*)GetTransientPackage(), UEditorScrollItemData::StaticClass());
				ItemDataNone->SItemType = EEditorSItemTypeEnum::VE_TestItem;
				ItemDataNone->KeyName = TEXT("none");
				ItemDataNone->DisplayName = TEXT("none");
				ItemDataNone->PortraitTextureRef = nullptr;
				ItemDataNone->PortraitRef = nullptr;
				ItemDataNone->ItemGrade = EItemGradeEnum::Normal;
				PopupScrollList->ItemDataList.Add(ItemDataNone);
				UEditorScrollItemData* ItemData = Cast<UEditorScrollItemData>(TestSpawnScrollList->GetSelectedScrollItemDataOnly());
				if (IsValid(ItemData))
				{
					if (ItemData->CharacterType == ECharacterCategoryTypeEnum::VE_Hero)
					{
						FCharacterTableInfo* CharacterTableInfo = UTableManager::GetInstancePtr()->GetCharacterRow(ItemData->KeyName);
						if (CharacterTableInfo)
						{
							TArray<FName> itemNameList = UTableManager::GetInstancePtr()->GetCharacterItemRowNames();
							for (FName itemName : itemNameList)
							{
								bool bExe = false;
								FItemTableInfo* itemTableInfo = UTableManager::GetInstancePtr()->GetCharacterItemRow(itemName);
								if (itemTableInfo)
								{
									if (itemTableInfo->EquipPosition == EquipPosition)
									{
										switch (EquipPosition)
										{
										case EItemEquipPositionEnum::VE_Weapon:
										{
											if (itemTableInfo->ItemType == EItemTypeEnum::VE_EquipItem)
											{
												if (CharacterTableInfo->EquipType == itemTableInfo->EquipType)
												{
													bExe = true;
												}
											}
										}
										break;
										case EItemEquipPositionEnum::VE_Armor:
										{
											bExe = true;
										}
										break;
										case EItemEquipPositionEnum::VE_Pants:
										{
											bExe = true;
										}
										break;
										case EItemEquipPositionEnum::VE_Ring:
										{
											bExe = true;
										}
										case EItemEquipPositionEnum::VE_Necklace:
										{
											bExe = true;
										}
										break;
										}
									}
									if (bExe)
									{
										UEditorScrollItemData* ItemDataNew = NewObject< UEditorScrollItemData >((UObject*)GetTransientPackage(), UEditorScrollItemData::StaticClass());
										ItemDataNew->SItemType = EEditorSItemTypeEnum::VE_TestItem;
										ItemDataNew->KeyName = itemName;
										FText itemFTextName = UUtilFunctionLibrary::GetLocalizedString(EStringTableType::VE_Item, itemTableInfo->ItemName);
										ItemDataNew->DisplayName = FName(*itemFTextName.ToString());
										ItemDataNew->PortraitTextureRef = UUtilFunctionLibrary::GetTexture2DFromTable(itemTableInfo->ItemIcon);
										ItemDataNew->PortraitRef = nullptr;
										ItemDataNew->ItemGrade = itemTableInfo->ItemGrade;
										PopupScrollList->ItemDataList.Add(ItemDataNew);
									}
								}
							}
						}
					}
				}
			}
			PopupScrollList->Init();
			PopupScrollList->InvalidateData();
		}
	}
}

void UUP_SpawnerEditor::PopupCancel()
{
	if (IsValid(PopupPanel))
	{
		PopupPanel->SetVisibility(ESlateVisibility::Collapsed);
	}

	CurEquipItemButton = nullptr;
}

void UUP_SpawnerEditor::PopupOk()
{
	if (IsValid(PopupPanel))
	{
		PopupPanel->SetVisibility(ESlateVisibility::Collapsed);
	}

	if (IsValid(PopupScrollList))
	{
		UEditorScrollItemData* ItemData = Cast<UEditorScrollItemData>(PopupScrollList->GetSelectedScrollItemDataOnly());
		if (IsValid(ItemData))
		{
			if (IsValid(CurEquipItemButton))
			{
				CurEquipItemButton->SetCurItem(ItemData->KeyName);
				CurEquipItemButton->GemKeyList.Empty();

				if (IsValid(Gem01ScrollView) && IsValid(Gem02ScrollView) && IsValid(Gem03ScrollView))
				{
					if (Gem01ScrollView->GetVisibility() == ESlateVisibility::Visible)
					{
						UEditorScrollItemData* ItemDataNew = Cast<UEditorScrollItemData>(Gem01ScrollView->GetSelectedScrollItemDataOnly());
						if (IsValid(ItemDataNew))
						{
							FItemTableInfo* GemItemTable = UTableManager::GetInstancePtr()->GetCharacterItemRow(ItemDataNew->KeyName);
							if (GemItemTable)
							{
								CurEquipItemButton->GemKeyList.Add(ItemDataNew->KeyName);
							}
						}
					}

					if (Gem02ScrollView->GetVisibility() == ESlateVisibility::Visible)
					{
						UEditorScrollItemData* ItemDataNew = Cast<UEditorScrollItemData>(Gem02ScrollView->GetSelectedScrollItemDataOnly());
						if (IsValid(ItemDataNew))
						{
							FItemTableInfo* GemItemTable = UTableManager::GetInstancePtr()->GetCharacterItemRow(ItemDataNew->KeyName);
							if (GemItemTable)
							{
								CurEquipItemButton->GemKeyList.Add(ItemDataNew->KeyName);
							}
						}
					}

					if (Gem03ScrollView->GetVisibility() == ESlateVisibility::Visible)
					{
						UEditorScrollItemData* ItemDataNew = Cast<UEditorScrollItemData>(Gem03ScrollView->GetSelectedScrollItemDataOnly());
						if (IsValid(ItemDataNew))
						{
							FItemTableInfo* GemItemTable = UTableManager::GetInstancePtr()->GetCharacterItemRow(ItemDataNew->KeyName);
							if (GemItemTable)
							{
								CurEquipItemButton->GemKeyList.Add(ItemDataNew->KeyName);
							}
						}
					}
				}

				CurEquipItemButton = nullptr;
			}
		}
	}
}

void UUP_SpawnerEditor::OnPopupSelectScrollItem(URScrollItemData* selectItem)
{
	UEditorScrollItemData* ItemData = Cast<UEditorScrollItemData>(selectItem);
	if (IsValid(ItemData))
	{
		if (IsValid(Gem01ScrollView) && IsValid(Gem02ScrollView) && IsValid(Gem03ScrollView))
		{
			FItemTableInfo* itemTableInfo = UTableManager::GetInstancePtr()->GetCharacterItemRow(ItemData->KeyName);
			if (itemTableInfo)
			{
				int32 GemNum = 0;// itemTableInfo->itemJewelSlotCount;

				Gem01ScrollView->SetVisibility(ESlateVisibility::Collapsed);
				Gem02ScrollView->SetVisibility(ESlateVisibility::Collapsed);
				Gem03ScrollView->SetVisibility(ESlateVisibility::Collapsed);

				switch (GemNum)
				{
				case 1:
				{
					Gem01ScrollView->SetVisibility(ESlateVisibility::Visible);
					Gem01ScrollView->ItemDataList.Empty();

					GemScrollRefresh(Gem01ScrollView, ItemData->KeyName);
				}
				break;
				case 2:
				{
					Gem01ScrollView->SetVisibility(ESlateVisibility::Visible);
					Gem01ScrollView->ItemDataList.Empty();
					Gem02ScrollView->SetVisibility(ESlateVisibility::Visible);
					Gem02ScrollView->ItemDataList.Empty();

					GemScrollRefresh(Gem01ScrollView, ItemData->KeyName);
					GemScrollRefresh(Gem02ScrollView, ItemData->KeyName);
				}
				break;
				case 3:
				{
					Gem01ScrollView->SetVisibility(ESlateVisibility::Visible);
					Gem01ScrollView->ItemDataList.Empty();
					Gem02ScrollView->SetVisibility(ESlateVisibility::Visible);
					Gem02ScrollView->ItemDataList.Empty();
					Gem03ScrollView->SetVisibility(ESlateVisibility::Visible);
					Gem03ScrollView->ItemDataList.Empty();

					GemScrollRefresh(Gem01ScrollView, ItemData->KeyName);
					GemScrollRefresh(Gem02ScrollView, ItemData->KeyName);
					GemScrollRefresh(Gem03ScrollView, ItemData->KeyName);
				}
				break;
				}
			}
		}
	}
}

void UUP_SpawnerEditor::OnClick_SpiritSettings()
{
	if (IsValid(UC_SpiritSettings))
	{
		if (UC_SpiritSettings->GetVisibility() == ESlateVisibility::SelfHitTestInvisible)
			UC_SpiritSettings->SetVisibility(ESlateVisibility::Collapsed);
		else
			UC_SpiritSettings->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
	}
}

void UUP_SpawnerEditor::GemScrollRefresh(URScrollView* scrollView, FName itemKey)
{
	if (IsValid(scrollView))
	{
		FItemTableInfo* itemTableInfo = UTableManager::GetInstancePtr()->GetCharacterItemRow(itemKey);
		if (itemTableInfo)
		{
			UEditorScrollItemData* ItemDataNone = NewObject< UEditorScrollItemData >((UObject*)GetTransientPackage(), UEditorScrollItemData::StaticClass());
			ItemDataNone->SItemType = EEditorSItemTypeEnum::VE_TestGem;
			ItemDataNone->KeyName = TEXT("none");
			ItemDataNone->DisplayName = TEXT("none");
			ItemDataNone->PortraitTextureRef = nullptr;
			ItemDataNone->PortraitRef = nullptr;
			ItemDataNone->ItemGrade = EItemGradeEnum::Normal;

			scrollView->ItemDataList.Add(ItemDataNone);

			TArray<FName> keyList /*= UTableManager::GetInstancePtr()->GetGemItemOptionRowNames()*/;
			for (FName key : keyList)
			{
				bool bExe = false;
// 				FGemItemOptionTableInfo* GemItemOption = UTableManager::GetInstancePtr()->GetGemItemOptionRow(key);
// 				FItemTableInfo* GemItemTable = UTableManager::GetInstancePtr()->GetCharacterItemRow(key);
// 				if (GemItemOption && GemItemTable)
// 				{
// 					bExe = true;
// 
// 					if (bExe)
// 					{
// 						UEditorScrollItemData* ItemDataNew = NewObject< UEditorScrollItemData >((UObject*)GetTransientPackage(), UEditorScrollItemData::StaticClass());
// 						ItemDataNew->SItemType = EEditorSItemTypeEnum::VE_TestGem;
// 						ItemDataNew->KeyName = key;
// 						FText itemFTextName = UUtilFunctionLibrary::GetLocalizedString(EStringTableType::VE_Item, GemItemTable->ItemName);
// 						ItemDataNew->DisplayName = FName(*itemFTextName.ToString());
// 						ItemDataNew->PortraitTextureRef = UUtilFunctionLibrary::GetTexture2DFromAssetID(GemItemTable->ItemIconRef);
// 						ItemDataNew->PortraitRef = nullptr;
// 						ItemDataNew->ItemGrade = EItemGradeEnum::Normal;
// 
// 						scrollView->ItemDataList.Add(ItemDataNew);
// 					}
// 				}
			}

			scrollView->Init();
			scrollView->InvalidateData();
		}
	}
}

void UUC_SpiritSettings::NativeConstruct()
{
	Super::NativeConstruct();

	if (IsValid(Scrollview))
	{
		if (!Scrollview->ScrollItemList.Num())
			Scrollview->Init_NoClear(MAXSCROLLITEM);

		if (!Scrollview->ItemDataList.Num())
		{
			TArray<FName> SpiritSkills = UTableManager::GetInstancePtr()->GetSpiritSkillRowNames();
			for (size_t i = 0; i < SpiritSkills.Num(); ++i)
			{
				UUC_SpiritScrollItemData* itemdata = Cast<UUC_SpiritScrollItemData>(Scrollview->CreateScrollItemData(UUC_SpiritScrollItemData::StaticClass()));
				if (itemdata)
				{
					itemdata->SkillTableID = SpiritSkills[i];
				}
				Scrollview->ItemDataList.Emplace(itemdata);
			}
		}

		Scrollview->InvalidateData();
	}

	
}

void UUC_SpiritSettings::NativeDestruct()
{
	Super::NativeDestruct();
}

void UUC_SpiritScrollItem::InvalidateData()
{
	Super::InvalidateData();

	UUC_SpiritScrollItemData* itemdata = Cast<UUC_SpiritScrollItemData>(ItemData);
	if (itemdata)
	{
		FSkillTableInfo_V2* skillinfo= UTableManager::GetInstancePtr()->GetSkillRow_V2(itemdata->SkillTableID);
		if (skillinfo)
		{
			if (IsValid(TableKey))
				TableKey->SetText(FText::FromName(itemdata->SkillTableID));
			if (IsValid(CasterEffectName))
			{
				UEnum* pEnum = FindObject<UEnum>(ANY_PACKAGE, TEXT("ESkillApplierTypeEnum"));
				FString str = pEnum->GetNameStringByIndex((int32)skillinfo->CasterEffect1);
				str = pEnum->GetNameStringByValue((int32)skillinfo->CasterEffect1);
				CasterEffectName->SetText(FText::FromString(str));
			}
			if (IsValid(CasterPercent))
			{
				CasterPercent->SetText(FText::AsNumber(skillinfo->CasterPercent1));
			}
		}

		if (ItemData->bSelected)
			Overlay_Selected->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
		else
			Overlay_Selected->SetVisibility(ESlateVisibility::Collapsed);
	}
}

void UUC_SpiritScrollItem::Selected()
{
	if (ParentScrollView->GetSelectedScrollItem().Num() >= MAXSPIRIT)
	{
		if (!ItemData->bSelected)
			return;
	}

	Super::Selected();

	if (ItemData->bSelected)
		Overlay_Selected->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
	else
		Overlay_Selected->SetVisibility(ESlateVisibility::Collapsed);
}
