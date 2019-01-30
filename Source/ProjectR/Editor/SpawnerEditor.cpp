// Fill out your copyright notice in the Description page of Project Settings.

#include "ProjectR.h"
#include "SpawnerEditor.h"

#include "Blueprint/WidgetLayoutLibrary.h"
#include "GlobalIntegrated.h"

#include "UtilFunctionIntegrated.h"

#include "GameInfo/PlayStateInfo.h"


#include "Character/Interfaces/EntityBaseProperty.h"

#include "UI/UI_Editor/UP_SpawnerEditor.h"
#include "UI/UI_Editor/UP_EditorPlayTestInfo.h"
#include "UI/UI_Editor/UC_EquipItemButton.h"
#include "UI/HeroInfo/HeroManager/UC_HeroStatMgr_Info.h"
#include "CustomStructures/HeroSlotInfo.h"
#include "ContentGameMode/Modes/TestGameMode.h"

void ASpawnerEditor::BeginPlay()
{
	Super::BeginPlay();

	UWidgetLayoutLibrary::RemoveAllWidgets(this);

#if WITH_EDITOR
	APlayerController* Controller = UGameplayStatics::GetPlayerController(this, 0);
	if (IsValid(Controller))
	{
		Controller->ConsoleCommand("ShowMouseCursor");
	}	
#endif
}

void ASpawnerEditor::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
	ATestGameMode* pGameMode = Cast<ATestGameMode>(UGameplayStatics::GetGameMode(GetWorld()));
	if (!IsValid(pGameMode))
		return;

	APlayerController* Controller = UGameplayStatics::GetPlayerController(this, 0);
	if (!bFirst)
	{		
		if (IsValid(Controller))
		{
			Controller->bShowMouseCursor = true;
			Controller->ConsoleCommand("ShowMouseCursor");
			bFirst = true;
		}
	}

#if WITH_EDITOR
	URGameInstance* RGameInstance = RGAMEINSTANCE(this);
	
	if (IsValid(Controller) &&  IsValid(RGameInstance->EditorManager))
	{
		Controller->bShowMouseCursor = true;
		
		
		if (Controller->WasInputKeyJustPressed(EKeys::U))
		{
			if (IsValid(SpawnerEditorBpClass))
			{
				if (!IsValid(SpawnerEditorUI))
				{
					SpawnerEditorUI = CreateWidget<UUP_SpawnerEditor>(GetWorld(), SpawnerEditorBpClass);
					if (IsValid(SpawnerEditorUI))
					{
						SpawnerEditorUI->AddToViewport(10000);
						SpawnerEditorUI->Refresh();
					}
				}
				else
				{
					SpawnerEditorUI->ProcessOption();
					SpawnerEditorUI->RemoveFromViewport();
					SpawnerEditorUI = nullptr;
				}
			}
		}
		else if (Controller->WasInputKeyJustPressed(EKeys::I))
		{
			if (pGameMode->MyHeroList.Num() <= 0)
				return;

			if (IsValid(EditorPlayHeroStatBpClass))
			{
				if (IsValid(EditorPlayHeroStateUI))
				{
					EditorPlayHeroStateUI->RemoveFromViewport();
					EditorPlayHeroStateUI = nullptr;
				}
				else
				{
					UBattleEntityComponent* battlecomp = UCharacterHelpFunctionLibrary::GetBattleEntityComponent(pGameMode->MyHeroList[0].Get());
					EditorPlayHeroStateUI = CreateWidget<UUC_HeroStatMgr_Info>(GetWorld(), EditorPlayHeroStatBpClass);
					if (!IsValid(EditorPlayHeroStateUI) || battlecomp == nullptr)
						return;
					if (IsValid(EditorPlayHeroStateUI))
					{
						EditorPlayHeroStateUI->AddToViewport(10000);
						EditorPlayHeroStateUI->SetStatInfo(battlecomp->GetBattleInfo());
					}
				}
			}
		}
		else if (Controller->WasInputKeyJustPressed(EKeys::Tab))
		{
			if (IsValid(pGameMode))
			{
				if (bPlayTestInfoUI)
				{
					if (pGameMode->SetSelectNpcActor())
					{
						if (!IsValid(EditorPlayTestUI) && IsValid(EditorPlayTestBpClass))
						{
							EditorPlayTestUI = CreateWidget<UUP_EditorPlayTestInfo>(GetWorld(), EditorPlayTestBpClass);
						}

						if (IsValid(EditorPlayTestUI))
						{
							if (!EditorPlayTestUI->IsInViewport())
							{
								EditorPlayTestUI->AddToViewport(9000);
								
							}

							EditorPlayTestUI->SetActor(pGameMode->GetSelectActor().Get());
						}
					}
					else
					{
						if (IsValid(EditorPlayTestUI))
						{
							if (EditorPlayTestUI->IsInViewport())
							{
								EditorPlayTestUI->RemoveFromViewport();
							}
						}
					}
				}
				else
				{
					pGameMode->ClearSelectActor();

					if (IsValid(EditorPlayTestUI))
					{
						if (EditorPlayTestUI->IsInViewport())
						{
							EditorPlayTestUI->RemoveFromViewport();
						}
					}
				}
			}
		}
		else if (Controller->WasInputKeyJustPressed(EKeys::One))
		{
			if (Controller->IsInputKeyDown(EKeys::LeftShift))
			{
				if (IsValid(pGameMode))
				{
					pGameMode->ConsoleCommand("savekey 1");
				}
			}
			else
			{
				if (IsValid(pGameMode))
				{
					pGameMode->ConsoleCommand("loadkey 1");
				}
			}
		}
		else if (Controller->WasInputKeyJustPressed(EKeys::Two))
		{
			if (Controller->IsInputKeyDown(EKeys::LeftShift))
			{
				if (IsValid(pGameMode))
				{
					pGameMode->ConsoleCommand("savekey 2");
				}
			}
			else
			{
				if (IsValid(pGameMode))
				{
					pGameMode->ConsoleCommand("loadkey 2");
				}
			}
		}
		else if (Controller->WasInputKeyJustPressed(EKeys::Three))
		{
			if (Controller->IsInputKeyDown(EKeys::LeftShift))
			{
				if (IsValid(pGameMode))
				{
					pGameMode->ConsoleCommand("savekey 3");
				}
			}
			else
			{
				if (IsValid(pGameMode))
				{
					pGameMode->ConsoleCommand("loadkey 3");
				}
			}
		}
		else if (Controller->WasInputKeyJustPressed(EKeys::Four))
		{
			if (Controller->IsInputKeyDown(EKeys::LeftShift))
			{
				if (IsValid(pGameMode))
				{
					pGameMode->ConsoleCommand("savekey 4");
				}
			}
			else
			{
				if (IsValid(pGameMode))
				{
					pGameMode->ConsoleCommand("loadkey 4");
				}
			}
		}
		else if (Controller->WasInputKeyJustPressed(EKeys::Zero))
		{
		}
		else if (Controller->WasInputKeyJustPressed(EKeys::P))
		{
			bPause = !bPause;

			if (bPause)
			{
				GlobalTime = UGameplayStatics::GetGlobalTimeDilation(this);
				UGameplayStatics::SetGlobalTimeDilation(this, 0.0f);
			}
			else
			{
				UGameplayStatics::SetGlobalTimeDilation(this, GlobalTime);
			}
		}
		else if (Controller->WasInputKeyJustPressed(EKeys::L))
		{
			if (IsValid(pGameMode))
			{
				pGameMode->ExeLastCommand();
			}
		}
		else if (Controller->WasInputKeyJustPressed(EKeys::Z))
		{
			if (IsValid(pGameMode))
			{
				pGameMode->ConsoleCommand("allspawn");
			}
		}
		else if (Controller->WasInputKeyJustPressed(EKeys::LeftBracket))
		{
			if (IsValid(pGameMode))
			{
				pGameMode->ConsoleCommand("startkey 5");
			}
		}
		else if (Controller->WasInputKeyJustPressed(EKeys::RightBracket))
		{
			if (IsValid(pGameMode))
			{
				pGameMode->ConsoleCommand("endkey");
			}
		}
		else if (Controller->WasInputKeyJustPressed(EKeys::Five))
		{
			if (Controller->IsInputKeyDown(EKeys::LeftShift))
			{
				if (IsValid(pGameMode))
				{
					if (pGameMode->IsSaveKeyStart())
					{
						pGameMode->ConsoleCommand("endkey");
					}
					else
					{
						pGameMode->ConsoleCommand("startkey 5");
					}					
				}
			}
			else
			{
				if (IsValid(pGameMode))
				{
					pGameMode->ConsoleCommand("exekey 5");
				}
			}
		}
		else if (Controller->WasInputKeyJustPressed(EKeys::Six))
		{
			if (Controller->IsInputKeyDown(EKeys::LeftShift))
			{
				if (IsValid(pGameMode))
				{
					if (pGameMode->IsSaveKeyStart())
					{
						pGameMode->ConsoleCommand("endkey");
					}
					else
					{
						pGameMode->ConsoleCommand("startkey 6");
					}
				}
			}
			else
			{
				if (IsValid(pGameMode))
				{
					pGameMode->ConsoleCommand("exekey 6");
				}
			}
		}
		else if (Controller->WasInputKeyJustPressed(EKeys::Seven))
		{
			if (Controller->IsInputKeyDown(EKeys::LeftShift))
			{
				if (IsValid(pGameMode))
				{
					if (pGameMode->IsSaveKeyStart())
					{
						pGameMode->ConsoleCommand("endkey");
					}
					else
					{
						pGameMode->ConsoleCommand("startkey 7");
					}
				}
			}
			else
			{
				if (IsValid(pGameMode))
				{
					pGameMode->ConsoleCommand("exekey 7");
				}
			}
		}
		else if (Controller->WasInputKeyJustPressed(EKeys::Eight))
		{
			if (Controller->IsInputKeyDown(EKeys::LeftShift))
			{
				if (IsValid(pGameMode))
				{
					if (pGameMode->IsSaveKeyStart())
					{
						pGameMode->ConsoleCommand("endkey");
					}
					else
					{
						pGameMode->ConsoleCommand("startkey 8");
					}
				}
			}
			else
			{
				if (IsValid(pGameMode))
				{
					pGameMode->ConsoleCommand("exekey 8");
				}
			}
		}
		else if (Controller->WasInputKeyJustPressed(EKeys::M))
		{
			if (IsValid(pGameMode))
			{
				pGameMode->Test();
			}
		}
		

		if (bPause)
		{
			if (Controller->IsInputKeyDown(EKeys::O))
			{
				bFlip = !bFlip;

				if (bFlip)
				{
					UGameplayStatics::SetGlobalTimeDilation(this, GlobalTime);
				}
				else
				{
					UGameplayStatics::SetGlobalTimeDilation(this, 0.0f);
				}
			}
			else
			{
				UGameplayStatics::SetGlobalTimeDilation(this, 0.0f);
				bFlip = false;
			}
		}
	}
#endif
}

void ASpawnerEditor::ChangeSkillLevel(TArray<FHERO_SKILL>& SkillData, AActor* InTargetActor)
{
	if (IsValid(InTargetActor))
	{
		URGameInstance* RGameInstance = RGAMEINSTANCE(this);
		if ( IsValid(RGameInstance->PlayStateInfo))
		{
			ATestGameMode* pGameMode = Cast<ATestGameMode>(UGameplayStatics::GetGameMode(GetWorld()));
			if (IsValid(pGameMode))
			{
				if (pGameMode->heroSkillLevelList.Num() == 0)
				{
					IEntityBaseProperty* BaseProperty = Cast<IEntityBaseProperty>(InTargetActor);
					if (BaseProperty)
					{
						auto SelectedCharacterArray = RGameInstance->PlayStateInfo->GetSelectedCharacterArray();
						for (FHeroSlotInfo& slotInfo : SelectedCharacterArray)
						{
							if (slotInfo.HeroIDName == BaseProperty->GetCharacterID())
							{
								if (slotInfo.TestSkillLevelList.Num() > 0)
								{
									for (int32 i = 0; i < SkillData.Num(); ++i)
									{
										if (slotInfo.TestSkillLevelList.IsValidIndex(i))
										{
											if (slotInfo.TestSkillLevelList[i] > 0)
												SkillData[i].level = slotInfo.TestSkillLevelList[i];
										}
									}
								}
							}
						}
					}
				}
				else
				{
					for (int32 i = 0; i < SkillData.Num(); ++i)
					{
						if (pGameMode->heroSkillLevelList.IsValidIndex(i))
						{
							if (pGameMode->heroSkillLevelList[i] > 0)
								SkillData[i].level = pGameMode->heroSkillLevelList[i];
						}
					}
				}
			}
		}
	}
}

void ASpawnerEditor::HeroEquipItemToObject(AActor* Obj)
{
	IEntityBaseProperty* BaseProperty = Cast<IEntityBaseProperty>(Obj);
	URGameInstance* RGameInstance = RGAMEINSTANCE(this);
	if ( IsValid(RGameInstance->PlayStateInfo) && BaseProperty)
	{
		if (BaseProperty->GetCharacterCategory() == ECharacterCategoryTypeEnum::VE_Hero)
		{
			ATestGameMode* pGameMode = Cast<ATestGameMode>(UGameplayStatics::GetGameMode(GetWorld()));
			if (IsValid(pGameMode))
			{
				if (pGameMode->heroItemList.Num() == 0)
				{
					const TArray<FHeroSlotInfo>&	SelectedCharacterArray = RGameInstance->PlayStateInfo->GetSelectedCharacterArray();
					for (const FHeroSlotInfo& slotInfo : SelectedCharacterArray)
					{
						if (slotInfo.HeroIDName == BaseProperty->GetCharacterID())
						{
							if (slotInfo.TestItemList.Num() > 0)
							{
								for (int32 i = 0; i < slotInfo.TestItemList.Num(); ++i)
								{
									FITEM item;
									if (UUC_EquipItemButton::GetFItem(slotInfo.TestItemList[i], item))
									{
										UUC_EquipItemButton::EquipItem(Obj, item);
									}
								}
							}
						}
					}
				}
				else
				{
					for (int32 i = 0; i < pGameMode->heroItemList.Num(); ++i)
					{
						FITEM item;
						if (UUC_EquipItemButton::GetFItem(pGameMode->heroItemList[i], item))
						{
							UUC_EquipItemButton::EquipItem(Obj, item);
						}
					}
				}
			}
		}
	}
}