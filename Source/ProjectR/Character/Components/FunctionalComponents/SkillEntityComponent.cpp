// Fill out your copyright notice in the Description page of Project Settings.

#include "ProjectR.h"
#include "ProjectRGameMode.h"
#include "SkillEntityComponent.h"

#include "ContentGameMode/Controllers/StagePlayerController.h"
#include "Character/Interfaces/EntityBaseProperty.h"
#include "Character/Bullets/NormalBullet.h"
#include "Character/Components/BossComponents/BossBaseComponent.h"
#include "Character/Components/BossComponents/GigantComponent.h"
#include "Character/Components/BossComponents/AkhanComponent.h"
#include "Character/Components/AnimStateComponent.h"
#include "Character/Components/FunctionalComponents/BattleEntityComponent.h"
#include "Character/Components/FunctionalComponents/EntityRenderComponent.h"
#include "Character/Components/SkillAreaIndicatorMeshComponent.h"
#include "Character/Components/SkillCastingEffectComponent.h"
#include "Character/Components/SpecialMovingComponent.h"
#include "Character/BattleObject/BattleBaseObject.h"
#include "Character/BattleBaseAnimInstance.h"
#include "Character/SharedBattle/BattleOperator.h"
#include "CustomStructures/SharedStructures/CharacterBattleInfo.h"

#include "GlobalIntegrated.h"

#include "RSkillSet_New.h"
#include "GameInfo/UserInfo.h"
#include "GameInfo/RInventory.h"
#include "GameInfo/PlayStateInfo.h"




// #include "Utils/AIFunctionLibrary.h"
#include "UtilFunctionIntegrated.h"

#include "Skill_V2/NetSkillManager.h"
#include "Skill_V2/SharedSkill/ISkill_V2.h"
#include "Skill_V2/SharedSkill/Skill_Passive_V2.h"
#include "Skill_V2/SharedSkill/SkillSlotContainer.h"
#include "Skill_V2/SharedSkill/SkillPassiveSlotContainer.h"
#include "Skill_V2/SharedSkill/SkillEventReactor.h"
#include "Skill_V2/SharedSkill/Skill_Channel.h"

#include "Skill_V2/SkillSyncApplyData.h"

#include "AI/AI_Skill/RAI_SkillAICaster_HeroDefault.h"
#include "AI/AI_Skill/RAI_SkillAICaster_NpcDefault.h"

#include "LeaderControlHelper.h"
#include "Global/RGameInstance.h"



#if WITH_EDITOR

#include "Editor/BaseEditor.h"
#include "Editor/SpawnerEditor.h"
#include "ContentGameMode/Modes/TestGameMode.h"
#include "UI/UI_Editor/UP_SpawnerEditor.h"
#include "UI/Control/RScrollView.h"

#endif



// Sets default values for OwnerActor.Get() component's properties
USkillEntityComponent::USkillEntityComponent()
{
	// Set OwnerActor.Get() component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	bWantsInitializeComponent = true;
	PrimaryComponentTick.bCanEverTick = false;

	// ...
}


// Called when the game starts
void USkillEntityComponent::InitializeComponent()
{
	Super::InitializeComponent();

	// ...
	OwnerActor = GetOwner();

	SkillEventReactor = NewObject<USkillEventReactor>((UObject*)GetTransientPackage(), USkillEventReactor::StaticClass());
	checkf(SkillEventReactor != nullptr, TEXT("USkillEntityComponent::InitializeComponent[SkillEventReactor Is Empty]"));
	SkillEventReactor->Attach(OwnerActor.Get());

	AISkillAutoModeOn.Init(true, 3);
	AISkillAutoModeOn[2] = false;
}

void USkillEntityComponent::UninitializeComponent()
{
	checkf(SkillEventReactor != nullptr, TEXT("USkillEntityComponent::UninitializeComponent[SkillEventReactor Is Empty]"));
	SkillEventReactor->Detach();

	Super::UninitializeComponent();
}

void USkillEntityComponent::BeginPlay()
{
	Super::BeginPlay();
}

void USkillEntityComponent::EndPlay(const EEndPlayReason::Type EEndPlayReason)
{
	ClearPassiveSkillAndSlot();

	EndSkillAction_V2();

	EntitySkills_V2.Empty();

	Super::EndPlay(EEndPlayReason);
}

void USkillEntityComponent::SetBattleEntityComponent(UBattleEntityComponent* BttleEntityComp)
{
	checkf(BttleEntityComp != nullptr, TEXT("USkillEntityComponent::SetBattleEntityComponent[BttleEntityComp Is Empty]"));

	if (IsValid(BttleEntityComp))
	{
		OwnerBattleEntityComponent = BttleEntityComp;
	}
}



void USkillEntityComponent::UpdateSkillComponent(float DeltaSecond)
{
	if (!bActivated)
		return;

	UpdateSkillInst(DeltaSecond);

	if (IsAutoMode() && AISkillCaster)
	{
		UEntityAIComponent* EntityAIComponent = UCharacterHelpFunctionLibrary::GetEntityAIComponent(OwnerActor.Get());
		if( EntityAIComponent && EntityAIComponent->IsActiveAI() )
			AISkillCaster->CheckReadyToCastSkill_V2(DeltaSecond);
	}

}

void USkillEntityComponent::ClearSkillEntityComponent()
{
	if (CurrentSkill_V2)
	{
		CurrentSkill_V2->End();
		CurrentSkill_V2 = nullptr;
	}

	ClearPassiveSkillAndSlot();
	ClearAllSlots();
}


void  USkillEntityComponent::StartSkillApply() 
{
	if (OwnerActor.IsValid() == false)
		return;

	if (IsSkillActing())
		return;
	
	bSkillApply = true;

	if (OwnerBattleEntityComponent.IsValid())
	{
		if (IsValid(OwnerBattleEntityComponent->GetBossComponent()))
			OwnerBattleEntityComponent->GetBossComponent()->StartSkillApply();
	}
}

void  USkillEntityComponent::EndSkillApply() 
{
	if (OwnerActor.IsValid() == false)
		return;

	if (OwnerBattleEntityComponent.IsValid())
	{
		if (IsValid(OwnerBattleEntityComponent->GetBossComponent()))
			OwnerBattleEntityComponent->GetBossComponent()->EndSkillApply();
	}
}

void USkillEntityComponent::SetNeedTargetSkillID(FName InSkillID)
{
	NeedTargetSkill = InSkillID;
}
bool USkillEntityComponent::IsSkillCasting()
{
	if (URealTimeModManager::IsRealTimeGameMode())
	{
		return bSkillCasting;
	}

	return bSkillCasting;
}

bool USkillEntityComponent::IsSkillActing()
{
	if (URealTimeModManager::IsRealTimeGameMode())
	{
		return (bNetSkillSending || bSkillActing);
	}
	
	return bSkillActing;
}

void USkillEntityComponent::SetSkillCasting(bool InSkillCasting) 
{ 
	bSkillCasting = InSkillCasting;

	if (bSkillCasting)
	{
		if (SkillAniType != ESkillAniTypeEnum::VE_None)
		{
			auto OwnerActor_AnimStateComponent = UCharacterHelpFunctionLibrary::GetAnimStateComponent(OwnerActor.Get());
			if (IsValid(OwnerActor_AnimStateComponent))
			{
				FVariantData SkillData = FVariantData((int32)SkillAniType);
				OwnerActor_AnimStateComponent->DoCommand(ECharacterCommandEnum::VE_CastingSkill, SkillData);
			}
		}
	}
}

void USkillEntityComponent::SetSkillActing(bool InSkillActing)
{
	bSkillActing = InSkillActing;

#if LOG_SKILL
	UE_LOG(LogBattle, Log, TEXT("Skill Acting - %s - SkillAniType %d bSkillActing %d"), *(OwnerActor->GetName()), (int32)SkillAniType, bSkillActing );
#endif

	if (SkillAniType != ESkillAniTypeEnum::VE_None)
	{
		auto OwnerActor_AnimStateComponent = UCharacterHelpFunctionLibrary::GetAnimStateComponent(OwnerActor.Get());
		if (IsValid(OwnerActor_AnimStateComponent))
		{
			FVariantData SkillData = FVariantData((int32)SkillAniType);
			if (bSkillActing)
			{
				OwnerActor_AnimStateComponent->DoCommand(ECharacterCommandEnum::VE_ActionSkill, SkillData);
			}
			else
			{
				OwnerActor_AnimStateComponent->DoCommand(ECharacterCommandEnum::VE_ActionSkillEnd, SkillData);
			}
		}
	}
}

bool USkillEntityComponent::IsAutoMode()
{
	if (OwnerActor.IsValid())
	{
		UEntityAIComponent* AIComp = UCharacterHelpFunctionLibrary::GetEntityAIComponent(OwnerActor.Get());
		if (AIComp)
		{
			if (EAIControlModeEnum::VE_AutoMode == AIComp->GetAIControlMode())
				return true;
		}
	}

	return false;
}

void  USkillEntityComponent::SetSkillAniType(ESkillAniTypeEnum InSkillAniType)
{
	if (OwnerActor.IsValid() == false)
		return;

	this->SkillAniType = InSkillAniType;

	
}

void USkillEntityComponent::ClearLastAppliedSkillInformations()
{
	if (OwnerBattleEntityComponent.IsValid())
	{
		if (IsValid(OwnerBattleEntityComponent->GetBattleInfoBlackboard()))
			OwnerBattleEntityComponent->GetBattleInfoBlackboard()->ClearLastAppliedSkillInformations();
	}
}

void USkillEntityComponent::SetSkillAutoModeOnOff(TArray<bool>& OnOffList)
{
	AISkillAutoModeOn = OnOffList;
}

float USkillEntityComponent::GetSkillRange(FName TargetSkillID)
{
	float Result = 0.0f;
	UISkill_V2* CheckSkill = GetSkill_V2(TargetSkillID.IsNone() ? NeedTargetSkill : TargetSkillID);
	if (IsValid(CheckSkill))
	{
		FSkillTableInfo_V2* SkillShapeInfo = CheckSkill->GetSkillInfo();
		if (SkillShapeInfo != nullptr)
		{
			Result = SkillShapeInfo->CastingRange;
		}
	}
	return Result;
}

bool USkillEntityComponent::IsValidSkillRange_V2(const FVector& SkillTargetLocation, FName TargetSkillID)
{
	UISkill_V2* CheckSkill = GetSkill_V2(TargetSkillID.IsNone() ? NeedTargetSkill : TargetSkillID);
	if (IsValid(CheckSkill))
	{
		FSkillTableInfo_V2* SkillInfo = CheckSkill->GetSkillInfo();
		if (SkillInfo)
		{
			float Range = SkillInfo->CastingRange;
			if (FMath::IsNearlyZero(Range))
			{
				return true;
			}
			else
			{
				const bool bCheckRange = FVector::PointsAreNear(OwnerActor->GetActorLocation(), SkillTargetLocation, Range);
				if (!bCheckRange)
				{
					return false;
				}

			}
			return true;
		}
	}

	return false;
}


void USkillEntityComponent::TryToAttachSkills_V2()
{
	if (OwnerActor.IsValid() == false)
		return;

	if (nullptr == SkillSlotContainer)
	{
		SkillSlotContainer = NewObject< USkillSlotContainer >((UObject*)GetTransientPackage(), USkillSlotContainer::StaticClass());
		SkillSlotContainer->Init(OwnerActor.Get());
	}

	if (nullptr == SkillPassiveSlotContainer)
	{
		SkillPassiveSlotContainer = NewObject< USkillPassiveSlotContainer >((UObject*)GetTransientPackage(), USkillPassiveSlotContainer::StaticClass());
		SkillPassiveSlotContainer->Init(OwnerActor.Get());
	}

	if (bAttachSkill)
	{
		if (!OwnerBattleEntityComponent.IsValid())
			return;

		UCharacterBattleInfo* BattleInfo = OwnerBattleEntityComponent->GetBattleInfo();
		if (nullptr == BattleInfo)
			return;

		auto OwnerActor_BaseProperty = Cast<IEntityBaseProperty>(OwnerActor.Get());
		if (nullptr == OwnerActor_BaseProperty)
			return;

		if (ECharacterCategoryTypeEnum::VE_Hero == OwnerActor_BaseProperty->GetCharacterCategory())
		{
			TArray<FHERO_SKILL> HeroSkillInfos;
			GetPlayerHeroSkillInfos(
				HeroSkillInfos,
				OwnerActor_BaseProperty->GetUserKID(),
				OwnerActor_BaseProperty->GetHeroUD(),
				OwnerActor.Get());

			EHERO_SKILL_SLOT SkillSlot = EHERO_SKILL_SLOT::ActiveSkill01;
			FName SkillID;
			ESkillCategoryTypeEnum SkillCategory = ESkillCategoryTypeEnum::VE_None;
			UISkill_V2* SkillInst = nullptr;
			for (auto& SkillInfo : HeroSkillInfos)
			{
				SkillSlot = (EHERO_SKILL_SLOT)SkillInfo.slotNo;
				switch (SkillSlot)
				{
					case EHERO_SKILL_SLOT::ActiveSkill01:
						SkillID = BattleInfo->ActiveSkill01;
						SkillCategory = ESkillCategoryTypeEnum::VE_ActiveSkill01;
						AttachSkill_V2(SkillID, SkillInfo.level, SkillCategory);
						break;

					case EHERO_SKILL_SLOT::ActiveSkill02:
						SkillID = BattleInfo->ActiveSkill02;
						SkillCategory = ESkillCategoryTypeEnum::VE_ActiveSkill02;
						AttachSkill_V2(SkillID, SkillInfo.level, SkillCategory);
						break;

					case EHERO_SKILL_SLOT::ActiveSkill03:
						SkillID = BattleInfo->ActiveSkill03;
						SkillCategory = ESkillCategoryTypeEnum::VE_ActiveSkill03;
						AttachSkill_V2(SkillID, SkillInfo.level, SkillCategory);
						break;

					case EHERO_SKILL_SLOT::PassiveSkill01:
						SkillID = BattleInfo->PassiveSkill01;
						SkillCategory = ESkillCategoryTypeEnum::VE_PassiveSkill01;
						AttachSkill_V2(SkillID, SkillInfo.level, SkillCategory);
						break;

					case EHERO_SKILL_SLOT::PassiveSkill02:
						SkillID = BattleInfo->PassiveSkill02;
						SkillCategory = ESkillCategoryTypeEnum::VE_PassiveSkill02;
// 						AttachSkill_V2(SkillID, SkillInfo.level, SkillCategory);
						break;
				}
			}

			/*TrytoAdd SpiritSkills*/
			if (UGamePlayBluePrintFunction::GetBaseStageInfo()->bPlayAlone && 
				OwnerBattleEntityComponent->CommandSlotIndex == ROLE_LEADER_INDEX)
			{
				UPlayStateInfo* playstateinfo = RGAMEINSTANCE(GEngine)->PlayStateInfo;
				if (playstateinfo)
				{
					TArray<FHeroSlotInfo> heroslotArray = playstateinfo->GetSelectedCharacterArray();
					for (size_t i = 0; i < heroslotArray.Num(); ++i)
					{
						if (i == ROLE_LEADER_INDEX)
							continue;

						FCharacterTableInfo* CharacterTableInfo = URenewal_BattleFunctionLibrary::GetCharacterTableInfo(ECharacterCategoryTypeEnum::VE_Hero, heroslotArray[i].HeroIDName);
						if (!CharacterTableInfo)
							continue;

						SkillID = CharacterTableInfo->SpiritPassive;
						SkillCategory = ESkillCategoryTypeEnum((size_t)ESkillCategoryTypeEnum::VE_PassiveSkill02 + i);

						FSkillTableInfo_V2* spiritInfo = UTableManager::GetInstancePtr()->GetSkillRow_V2(SkillID);
						if (spiritInfo)
						{
							AttachSkill_V2(SkillID, spiritInfo->CasterSlotLevel, SkillCategory);
						}
					}
				}
			}

#if WITH_EDITOR
			if(Cast<ATestGameMode>(UGameplayStatics::GetGameMode(RGAMEINSTANCE(this)->GetWorld())))
			{
				if (IsValid(RGAMEINSTANCE(this)->EditorManager))
				{
					if (IsValid(RGAMEINSTANCE(this)->EditorManager->GetEditorActor()))
					{
						ASpawnerEditor* Editor = Cast<ASpawnerEditor>(RGAMEINSTANCE(this)->EditorManager->GetEditorActor());
						if (Editor)
						{
							URScrollView* scrollview = Editor->SpawnerEditorUI->UC_SpiritSettings->Scrollview;

							TArray<URScrollItemData*> itemdatalist = scrollview->GetSelectedScrollItemData();
							for (size_t i = 0; i < itemdatalist.Num(); ++i)
							{
								UUC_SpiritScrollItemData* itemdata = Cast<UUC_SpiritScrollItemData>(itemdatalist[i]);

								SkillID = itemdata->SkillTableID;
								SkillCategory = ESkillCategoryTypeEnum((size_t)ESkillCategoryTypeEnum::VE_Spirit01 + i);
								FSkillTableInfo_V2* spiritInfo = UTableManager::GetInstancePtr()->GetSkillRow_V2(SkillID);
								if (spiritInfo)
								{
									AttachSkill_V2(SkillID, spiritInfo->CasterSlotLevel, SkillCategory);
								}
							}
						}
					}
				}
			}
#endif
		}
		else
		{
			int32 SkillLevel = URenewal_BattleFunctionLibrary::GetNpcSkillLevel(BattleInfo->GetLevel() );

			if (IsCreateSkillEnable())
			{
				AttachSkill_V2(BattleInfo->ActiveSkill01, SkillLevel, ESkillCategoryTypeEnum::VE_ActiveSkill01);
				AttachSkill_V2(BattleInfo->ActiveSkill02, SkillLevel, ESkillCategoryTypeEnum::VE_ActiveSkill02);
				AttachSkill_V2(BattleInfo->ActiveSkill03, SkillLevel, ESkillCategoryTypeEnum::VE_ActiveSkill03);
				AttachSkill_V2(BattleInfo->ActiveSkill04, SkillLevel, ESkillCategoryTypeEnum::VE_ActiveSkill04);

				AttachSkill_V2(BattleInfo->PassiveSkill01, SkillLevel, ESkillCategoryTypeEnum::VE_PassiveSkill01);
				AttachSkill_V2(BattleInfo->PassiveSkill02, SkillLevel, ESkillCategoryTypeEnum::VE_PassiveSkill02);
				
#if WITH_EDITOR
				if (Cast<ATestGameMode>(UGameplayStatics::GetGameMode(RGAMEINSTANCE(this)->GetWorld())))
				{
					if (IsValid(RGAMEINSTANCE(this)->EditorManager))
					{
						if (IsValid(RGAMEINSTANCE(this)->EditorManager->GetEditorActor()))
						{
							ASpawnerEditor* Editor = Cast<ASpawnerEditor>(RGAMEINSTANCE(this)->EditorManager->GetEditorActor());
							if (Editor)
							{
								if (IsValid(Editor->SpawnerEditorUI))
								{
									URScrollView* scrollview = Editor->SpawnerEditorUI->UC_SpiritSettings->Scrollview;

									TArray<URScrollItemData*> itemdatalist = scrollview->GetSelectedScrollItemData();
									for (size_t i = 0; i < itemdatalist.Num(); ++i)
									{
										UUC_SpiritScrollItemData* itemdata = Cast<UUC_SpiritScrollItemData>(itemdatalist[i]);

										FName SkillID = itemdata->SkillTableID;
										ESkillCategoryTypeEnum SkillCategory = ESkillCategoryTypeEnum((size_t)ESkillCategoryTypeEnum::VE_Spirit01 + i);
										FSkillTableInfo_V2* spiritInfo = UTableManager::GetInstancePtr()->GetSkillRow_V2(SkillID);
										if (spiritInfo)
										{
											AttachSkill_V2(SkillID, spiritInfo->CasterSlotLevel, SkillCategory);
										}
									}
								}
							}
						}
					}
				}
				else
#endif
				{
					AttachSkill_V2(BattleInfo->SpiritSkill01, SkillLevel, ESkillCategoryTypeEnum::VE_Spirit01);
					AttachSkill_V2(BattleInfo->SpiritSkill02, SkillLevel, ESkillCategoryTypeEnum::VE_Spirit02);
					AttachSkill_V2(BattleInfo->SpiritSkill03, SkillLevel, ESkillCategoryTypeEnum::VE_Spirit03);
					AttachSkill_V2(BattleInfo->SpiritSkill04, SkillLevel, ESkillCategoryTypeEnum::VE_Spirit04);
				}
			}
			else
			{
				if (OwnerBattleEntityComponent->GetBossComponent())
				{
					// TODO : refactoring check.. move bosskilllentitycomponent
					//temp
					AttachSkill_V2(BattleInfo->PassiveSkill01, SkillLevel, ESkillCategoryTypeEnum::VE_PassiveSkill01);
				}
			}
		}

		if (OwnerBattleEntityComponent->GetBossComponent() == nullptr )
			PrepareAICastingSkills();
	}
}

void USkillEntityComponent::AttachSkill_V2(FName SkillID, int32 SkillLevel, ESkillCategoryTypeEnum SkillCategoryType)
{
	if (OwnerActor.IsValid() == false)
		return;

	if (SkillID.IsNone())
		return;

	auto OwnerActor_BaseProperty = Cast<IEntityBaseProperty>(OwnerActor.Get());
	if (OwnerActor_BaseProperty == nullptr)
		return;

	switch (SkillCategoryType)
	{
		case ESkillCategoryTypeEnum::VE_None:
		case ESkillCategoryTypeEnum::VE_ActiveSkill01:
		case ESkillCategoryTypeEnum::VE_ActiveSkill02:
		case ESkillCategoryTypeEnum::VE_ActiveSkill03:
		case ESkillCategoryTypeEnum::VE_ActiveSkill04:
			{
				UISkill_V2* SkillInst = CreateSkill(OwnerActor.Get(), SkillID, SkillLevel);
				if (SkillInst)
				{
					SkillInst->SetSkillCategoryType(SkillCategoryType);
					EntitySkills_V2.Emplace(SkillID, SkillInst);
				}
				else
				{
					UE_LOG(LogBattle, Log, TEXT("Invalid Skill %s"), *(SkillID.ToString()));
					return;
				}
			}
			break;

		case ESkillCategoryTypeEnum::VE_PassiveSkill02:
		case ESkillCategoryTypeEnum::VE_PassiveSkill01:
		{
			USkill_Passive_V2* SkillInst = CreatePassiveSkill(OwnerActor.Get(), SkillID, SkillLevel);
			if (SkillInst)
			{
				SkillInst->SetSkillCategoryType(SkillCategoryType);
				EntitySkills_V2.Emplace(SkillID, SkillInst);

				SkillInst->AttachPassiveSkill();
			}
			else
			{
				UE_LOG(LogBattle, Log, TEXT("Invalid PassiveSkill %s"), *(SkillID.ToString()));
				return;
			}
		}break;

		case ESkillCategoryTypeEnum::VE_Spirit01:
		case ESkillCategoryTypeEnum::VE_Spirit02:
		case ESkillCategoryTypeEnum::VE_Spirit03:
		case ESkillCategoryTypeEnum::VE_Spirit04:
		{
			USkill_Passive_V2* SkillInst = CreateSpiritSkill(OwnerActor.Get(), SkillID, SkillLevel);
			if (SkillInst)
			{
				SkillInst->SetSkillCategoryType(SkillCategoryType);
				EntitySkills_V2.Emplace(SkillID, SkillInst);

				SkillInst->AttachPassiveSkill();
			}
			else
			{
				UE_LOG(LogBattle, Log, TEXT("Invalid PassiveSkill %s"), *(SkillID.ToString()));
				return;
			}
		}break;
	}	
}

void USkillEntityComponent::ChangeAISkillMode(EAIControlModeEnum InSkillModeType)
{
	//todo del
	SkillModeType = InSkillModeType;

	PrepareAICastingSkills();
}

void USkillEntityComponent::PrepareAICastingSkills()
{
	IEntityBaseProperty* EntityBaseProperty = Cast< IEntityBaseProperty >(OwnerActor.Get());
	if (EntityBaseProperty == nullptr)
		return;
	
	if (EntityBaseProperty->GetCharacterCategory() == ECharacterCategoryTypeEnum::VE_Hero)
	{
		TArray< FName > SkillIDList;
		UISkill_V2* SkillInst = nullptr;
		
		bool bAutoChangeSkill = true;
		bool bAutoComboSkill = true;
		auto BasePlayerController = UUtilFunctionLibrary::GetBasePlayerController(OwnerActor->GetWorld());
		if (IsValid(BasePlayerController))
		{
			bool isMyHero = RGAMEINSTANCE(GEngine)->PlayStateInfo->IsPlayHero(OwnerActor.Get());
			bool isLeader = BasePlayerController->IsSelectedCharacter(OwnerActor.Get());
// 			if (isMyHero && !isLeader)
// 				bAutoChangeSkill = false;
			if (isMyHero)
				bAutoComboSkill = false;
		}
		
		if (AISkillAutoModeOn[0])
		{
			SkillInst = GetSkillByCategory_V2(ESkillCategoryTypeEnum::VE_ActiveSkill01);
			if (SkillInst)
				SkillIDList.Emplace(SkillInst->GetSkillID());
		}
		
		if (AISkillAutoModeOn[1])
		{
			SkillInst = GetSkillByCategory_V2(ESkillCategoryTypeEnum::VE_ActiveSkill02);
			if (SkillInst)
				SkillIDList.Emplace(SkillInst->GetSkillID());
		}

		if (AISkillAutoModeOn[2] && bAutoComboSkill)
		{
			SkillInst = GetSkillByCategory_V2(ESkillCategoryTypeEnum::VE_ActiveSkill03);
			if (SkillInst)
				SkillIDList.Emplace(SkillInst->GetSkillID());
		}

		if (SkillIDList.Num() > 0)
		{
			if(AISkillCaster == nullptr)
				AISkillCaster = NewObject< URAI_SkillAICaster_HeroDefault >((UObject*)GetTransientPackage(), URAI_SkillAICaster_HeroDefault::StaticClass());

			if (AISkillCaster)
				AISkillCaster->Init(OwnerActor.Get(), SkillIDList, 5.0f);
		}
	}
	else
	{		
		TArray< FName > SkillIDList;
		UISkill_V2* SkillInst = nullptr;

		SkillInst = GetSkillByCategory_V2(ESkillCategoryTypeEnum::VE_ActiveSkill01);
		if (SkillInst)
			SkillIDList.Emplace(SkillInst->GetSkillID());

		SkillInst = GetSkillByCategory_V2(ESkillCategoryTypeEnum::VE_ActiveSkill02);
		if (SkillInst)
			SkillIDList.Emplace(SkillInst->GetSkillID());

		SkillInst = GetSkillByCategory_V2(ESkillCategoryTypeEnum::VE_ActiveSkill03);
		if (SkillInst)
			SkillIDList.Emplace(SkillInst->GetSkillID());

		SkillInst = GetSkillByCategory_V2(ESkillCategoryTypeEnum::VE_ActiveSkill04);
		if (SkillInst)
			SkillIDList.Emplace(SkillInst->GetSkillID());

		if (SkillIDList.Num() > 0)
		{
			AISkillCaster = NewObject< URAI_SkillAICaster_NpcDefault >((UObject*)GetTransientPackage(), URAI_SkillAICaster_NpcDefault::StaticClass());
			if (AISkillCaster)
			{
				AISkillCaster->Init(OwnerActor.Get(), SkillIDList, 5.0f);
			}
		}
		
	}
}

void USkillEntityComponent::SetVisibleSkillArea_V2(bool bShow)
{
	if (OwnerActor.IsValid() == false)
		return;

	if (IsVisibleSkillArea() == false && bShow == false)
	{
		return;
	}

	//cowspirit : Heroselection mode don't active when set visible skill area
	AStagePlayerController* BasePlayerController = Cast<AStagePlayerController>(UUtilFunctionLibrary::GetBasePlayerController(OwnerActor->GetWorld()));
// 	if (IsValid(BasePlayerController))
// 	{
// 		BasePlayerController->GetLeaderControlHelper()->GetLeaderSkillControl()->IsStartSkillArea = bShow;
// 	}

	SetIsVisibleSkillArea(bShow);

	auto AreaIndicatorComponents = UCharacterHelpFunctionLibrary::GetSkillAreaArray(OwnerActor.Get());
	if (bShow)
	{
		float SkillCastRange = 0.0f;
		float SkillApplyRangeX = 0.0f;
		float SkillApplyRangeY = 0.0f;
		float SectorAngle = 0.0f;
		FSkillTableInfo_V2* SkillInfo = nullptr;
		ESkillTargetTypeEnum SkillTargetType = ESkillTargetTypeEnum::VE_None;
		ESearchShapeTypeEnum ShapeType = ESearchShapeTypeEnum::VE_None;
		TArray<float> ShapeScope;
		ShapeScope.Init(0.0f, 3);

		UISkill_V2* SkillInst = GetSkill_V2(NeedTargetSkill);
		if (SkillInst)
		{
			SkillInfo = SkillInst->GetSkillInfo();
			if (SkillInfo)
			{
				SkillTargetType = SkillInfo->TargetType;
				ShapeType = SkillInfo->EffectShape;
				if (0 < SkillInfo->ShapeScope.Num())
				{
					ShapeScope = SkillInfo->ShapeScope;
				}
				SkillCastRange = SkillInfo->CastingRange;
			}
		}

		if (ESkillTargetTypeEnum::VE_Target == SkillTargetType)
		{
			ShapeType = ESearchShapeTypeEnum::VE_Line;
			SkillApplyRangeX = SkillInfo->CastingRange;
			SkillApplyRangeY = 30.0f; //0.3
		}
		else
		{
			if (ESearchShapeTypeEnum::VE_Circle == ShapeType)
			{
				SkillApplyRangeX = ShapeScope[0]; // Radius
				SkillApplyRangeY = ShapeScope[0]; // Radius
			}
			else if (ESearchShapeTypeEnum::VE_Sector == ShapeType)
			{
				SkillApplyRangeX = ShapeScope[0]; // Radius
				SkillApplyRangeY = ShapeScope[0]; // Radius
				SectorAngle = ShapeScope[1];	  // Angle
			}
			else if (ESearchShapeTypeEnum::VE_Box == ShapeType)
			{
				SkillApplyRangeX = ShapeScope[0];
				SkillApplyRangeY = ShapeScope[1]; // Width
			}
		}

		
		for (auto Element : AreaIndicatorComponents)
		{
			bool bVisible = false;

			ESkillAreaMeshTypeEnum SkillAreaMeshType = Element->GetSkillAreaMeshType();
			if (ESkillAreaMeshTypeEnum::VE_SkillArea == SkillAreaMeshType)
			{
				bVisible = true;
				Element->ShowSkillArea(SkillCastRange, SkillCastRange);
			}
			else if (ESkillAreaMeshTypeEnum::VE_SkillPickArea == SkillAreaMeshType)
			{
				if (ESkillTargetTypeEnum::VE_Target == SkillTargetType ||
					ESkillTargetTypeEnum::VE_TargetLocation == SkillTargetType ||
					ESearchShapeTypeEnum::VE_Box == ShapeType ||
					ESearchShapeTypeEnum::VE_Sector == ShapeType ||
					ESearchShapeTypeEnum::VE_Line == ShapeType)
				{
					Element->SetShapeType(ShapeType, SectorAngle);
					Element->ShowSkillArea(SkillApplyRangeX, SkillApplyRangeY);
					Element->SetLocationOriginal();
					bVisible = true;
					
					if (ShapeType == ESearchShapeTypeEnum::VE_Line)
						bVisible = false;
				}
			}
			Element->SetVisibility(bVisible);
		}
	}
	else
	{
		for (auto Element : AreaIndicatorComponents)
		{
			Element->ClearAreaSkillTarget();
			Element->SetVisibility(false);
		}
	}
}

void USkillEntityComponent::SetVisibleCrewSpawnArea(bool bShow, float InRange)
{
	if (OwnerActor.IsValid() == false)
		return;

	if (IsVisibleSkillArea() == false && bShow == false)
	{
		return;
	}

	auto BasePlayerController = Cast<AStagePlayerController>(UUtilFunctionLibrary::GetBasePlayerController(OwnerActor->GetWorld()));
// 	if (IsValid(BasePlayerController))
// 	{
// 		BasePlayerController->GetLeaderControlHelper()->GetLeaderSkillControl()->IsStartSkillArea = bShow;
// 	}

	SetIsVisibleSkillArea(bShow);

	auto AreaIndicatorComponents = UCharacterHelpFunctionLibrary::GetSkillAreaArray(OwnerActor.Get());
	if (bShow)
	{
		for (auto Element : AreaIndicatorComponents)
		{
			bool bVisible = false;
			
			ESkillAreaMeshTypeEnum SkillAreaMeshType = Element->GetSkillAreaMeshType();
			if (ESkillAreaMeshTypeEnum::VE_SkillArea == SkillAreaMeshType)
			{
				Element->ShowSkillArea(500.f, 500.f);
				bVisible = true;
			}
			else if (ESkillAreaMeshTypeEnum::VE_SkillPickArea == SkillAreaMeshType)
			{
				Element->SetShapeType(ESearchShapeTypeEnum::VE_Circle, 0.f);
				Element->ShowSkillArea(InRange, InRange);
				Element->SetLocationOriginal();
				bVisible = true;
			}
			Element->SetVisibility(bVisible);
		}
	}
	else
	{
		for (auto Element : AreaIndicatorComponents)
		{
			Element->ClearAreaSkillTarget();
			Element->SetVisibility(false);
		}
	}
}

bool USkillEntityComponent::CastNeedTargetSkill_V2( FName InSkillID, AActor* ActorTarget, FVector LocationTarget)
{
	if (OwnerActor.IsValid() == false)
		return false;

	if (IsSkillCasting() || IsSkillActing())
		return false;

	bool SuccessCasting = false;
	UISkill_V2* Skill = GetSkill_V2(InSkillID);
	if (Skill)
	{
		if (URealTimeModManager::IsRealTimeGameMode())
		{
			URGameInstance* RGameInstance = RGAMEINSTANCE(GEngine);
			if (RGameInstance)
			{
				if (RGameInstance->NetSkillManager)
				{
					SuccessCasting = RGameInstance->NetSkillManager->TrySkillCasting(OwnerActor.Get(), Skill, ActorTarget, LocationTarget);
				}
			}
		}
		else
		{
			SuccessCasting = Skill->Casting(ActorTarget, LocationTarget);
		}
	}
	return SuccessCasting;
}

void USkillEntityComponent::SetCurrentSkill_V2(UISkill_V2* InSkill)
{
	if (OwnerActor.IsValid() == false)
		return;

	if (!IsValid(InSkill))
		return;

	// Other skill is activated?
	if (IsValid(CurrentSkill_V2))
	{
		if (CurrentSkill_V2 != InSkill)
		{
			if (CurrentSkill_V2->GetSkillInfo()->CasterDuration <= 0)
			{
				UE_LOG(LogBattle, Log, TEXT("Override CurrentSkillEnd - %s(%s)"), *(this->GetName()), *CurrentSkill_V2->GetSkillID().ToString());
				CurrentSkill_V2->End();
			}
		}
		else
		{
			return;
		}
	}

	CurrentSkill_V2 = InSkill;


}

bool USkillEntityComponent::IsCurrentSkillChanneling()
{
	if (CurrentSkill_V2)
	{
		FSkillTableInfo_V2* SkillInfo = CurrentSkill_V2->GetSkillInfo();
		if (SkillInfo)
		{
			return (ESkillCastingTypeEnum::VE_Channeling == SkillInfo->CastingType &&
					ESkillStateEnum::VE_Casting != CurrentSkill_V2->GetSkillState());
		}
	}
	return false;
}

void USkillEntityComponent::FireCurrentSkill_V2()
{
	if (nullptr != CurrentSkill_V2)
	{
		CurrentSkill_V2->Fire();
	}
}

void USkillEntityComponent::DummyCurrentSkill_V2()
{
	if (nullptr != CurrentSkill_V2)
	{
		CurrentSkill_V2->DummyFire();
	}
}


void USkillEntityComponent::CastingActionSkill_V2()
{
	if (nullptr != CurrentSkill_V2)
	{
		CurrentSkill_V2->CastingAction();
	}
}

void USkillEntityComponent::EndSkillAction_V2()
{
	if (OwnerActor.IsValid() == false)
		return;

#if LOG_SKILL
	UE_LOG(LogBattle, Log, TEXT("AActor::EndSkillAction_V2"));
#endif

	TArray<USkillEntityComponent*> AppliedTargetSkillCompList; // for passive skill condition check
	FName curSkillID;
	if (IsValid(CurrentSkill_V2))
	{
		CurrentSkill_V2->GetAppliedTargetSkillComponentList(AppliedTargetSkillCompList);
		curSkillID = CurrentSkill_V2->GetSkillID();

		CurrentSkill_V2->End();
		CurrentSkill_V2 = nullptr;


	}

	SetSkillCasting(false);
	SetSkillActing(false);

	auto OwnerActor_AnimStateComponent = UCharacterHelpFunctionLibrary::GetAnimStateComponent(OwnerActor.Get());
	if (IsValid(OwnerActor_AnimStateComponent))
	{
		OwnerActor_AnimStateComponent->DoCommand(ECharacterCommandEnum::VE_StopSkill);
	}


	// Target passive skill condition check
	if (0 < AppliedTargetSkillCompList.Num())
	{
		for (auto TargetSkillComp : AppliedTargetSkillCompList)
		{
			TargetSkillComp->CheckPassiveSkillCondition();
		}
	}	
}

void USkillEntityComponent::EndSkillActionForced_V2()
{
	if (OwnerActor.IsValid() == false)
		return;

#if LOG_SKILL
	UE_LOG(LogBattle, Log, TEXT("AActor::EndSkillActionForced"));
#endif

	FName curSkillID;

	if (IsValid(CurrentSkill_V2))
	{
		auto OwnerActor_RenderComponent = UCharacterHelpFunctionLibrary::GetEntityRenderComponent(OwnerActor.Get());
		auto SkillCastingEffectComponent = OwnerActor_RenderComponent->GetSkillCastingEffectComponent();
		if (SkillCastingEffectComponent)
		{
			SkillCastingEffectComponent->EndSkillCasting();
		}

		curSkillID = CurrentSkill_V2->GetSkillID();
		CurrentSkill_V2->End();
		CurrentSkill_V2->ClearSkillEffectForced();
		CurrentSkill_V2 = nullptr;


	}
	ForceDeleteContinuousActor();

	SetSkillCasting(false);
	SetSkillActing(false);

	auto OwnerActor_AnimStateComponent = UCharacterHelpFunctionLibrary::GetAnimStateComponent(OwnerActor.Get());
	if (IsValid(OwnerActor_AnimStateComponent))
	{
		OwnerActor_AnimStateComponent->DoCommand(ECharacterCommandEnum::VE_StopSkill);
	}

}

void USkillEntityComponent::ForceDeleteContinuousActor()
{
	for (auto& Elem : EntitySkills_V2)
	{
		if (Elem.Value)
		{
			Elem.Value->DeleteContinuousActor();
		}
	}
}

void USkillEntityComponent::ForceProcessCurrentSkill()
{
	if (IsValid(CurrentSkill_V2))
	{
		ESkillStateEnum CurrentSkillState = CurrentSkill_V2->GetSkillState();

		if (ESkillStateEnum::VE_Casting == CurrentSkillState)
		{
			EndSkillActionForced_V2();
		}
		else if (ESkillStateEnum::VE_DoAction == CurrentSkillState)
		{
			FireCurrentSkill_V2();
			EndSkillAction_V2();
		}
		else if (ESkillStateEnum::VE_Fire == CurrentSkillState)
		{
			EndSkillAction_V2();
		}
	}
}

void USkillEntityComponent::ApplySkill(UISkill_V2* AppliedSkill, AActor* Caster, bool InbApplyCaster )
{
	if (IsValid(SkillSlotContainer) && IsValid(AppliedSkill) && IsValid(Caster))
	{
		SkillSlotContainer->AddSkillSlot(AppliedSkill, InbApplyCaster, Caster);
	}
}

UISkill_V2* USkillEntityComponent::GetSkill_V2(FName SkillID)
{
	if (EntitySkills_V2.Contains(SkillID))
		return EntitySkills_V2[SkillID];

	return nullptr;
}

UISkill_V2* USkillEntityComponent::GetSkillByCategory_V2(ESkillCategoryTypeEnum InSkillCategory)
{	
	for (auto& SkillInstPair : EntitySkills_V2)
	{
		if (SkillInstPair.Value->GetSkillCategoryType() == InSkillCategory)
			return SkillInstPair.Value;
	}

	return nullptr;
}

float USkillEntityComponent::GetSkillCoolTime_V2(FName SkillID)
{
	if (OwnerActor.IsValid() == false)
		return 1.0f;

	if (EntitySkills_V2.Contains(SkillID))
	{
		return EntitySkills_V2[SkillID]->GetCurrentCoolTime();
	}

	return 0.0f;
}

float USkillEntityComponent::GetCrewSkillCoolTime_V2(FName SkillID)
{
	if (OwnerActor.IsValid() == false)
		return 1.0f;

	if (EntitySkills_V2.Contains(SkillID))
	{
		return EntitySkills_V2[SkillID]->GetCurrentCrewSkillCoolTime();
	}

	return 0.0f;
}

void USkillEntityComponent::DecreaseCoolTime(int32 TargetPercent, int32 TargetValue)
{
	for( auto& SkillPair : EntitySkills_V2)
	{
		if (SkillPair.Value && SkillPair.Value->IsPassiveSkill() == false )
		{
			float CoolTime = SkillPair.Value->GetCurrentCoolTime();
			float CoolTimeBonus = CoolTime * ((float)TargetPercent / 100.0f) + TargetValue;
			SkillPair.Value->DecreaseCoolTime(CoolTimeBonus);
		}
	}
}



void USkillEntityComponent::EnterGlobalSkillCoolTime()
{
	auto TargetSkill = GetSkillByCategory_V2(ESkillCategoryTypeEnum::VE_ActiveSkill03);
	if (TargetSkill)
	{
		float CoolTime = 1.0f;
		if (TargetSkill->GetCurrentCoolTime() <= CoolTime)
		{
			TargetSkill->DecreaseCoolTime((-CoolTime));
		}
	}
}

void USkillEntityComponent::UpdateRealTimeSkillSlot(FSkillSyncApplyData& SyncData)
{
	if (IsValid(SkillSlotContainer))
	{
		int32 SlotCount = SyncData.ApplySlots.Num();
		for (int32 n = 0; n < SlotCount; ++n)
		{
			if (SyncData.RemoveSlots[n])
			{
				SkillSlotContainer->RemoveSlot(SyncData.ApplySlots[n]);
			}
		}
	}
}
void USkillEntityComponent::ClearPassiveSkillAndSlot()
{
// 	FSkillTableInfo_V2* SkillInfo = nullptr;
	USkill_Passive_V2* PassiveSkill = nullptr;
	for (auto& SkillInstPair : EntitySkills_V2)
	{
		if (ESkillCategoryTypeEnum::VE_PassiveSkill01 == SkillInstPair.Value->GetSkillCategoryType()/* ||
			ESkillCategoryTypeEnum::VE_ItemSkill01 == SkillInstPair.Value->GetSkillCategoryType()*/)
		{
			PassiveSkill = Cast<USkill_Passive_V2>(SkillInstPair.Value);
			if (PassiveSkill)
			{
				PassiveSkill->End();
				PassiveSkill->ClearSkillEffectForced();
			}
		}
	}
	
	checkf(SkillPassiveSlotContainer != nullptr, TEXT("USkillEntityComponent::ClearPassiveSkillAndSlot[SkillPassiveSlotContainer Is Empty]"));
	
	SkillPassiveSlotContainer->RemoveAllSlots();
}

void USkillEntityComponent::ForceClearSkillEffect()
{
	checkf(SkillPassiveSlotContainer != nullptr, TEXT("USkillEntityComponent::ForceClearSkillEffect[SkillPassiveSlotContainer Is Empty]"));
	SkillPassiveSlotContainer->RemoveAllSlots();

	ClearAllSlots();

	for (auto& SkillInstPair : EntitySkills_V2)
	{
		auto TargetSkillValue = SkillInstPair.Value;
		if (TargetSkillValue)
		{
			TargetSkillValue->End();
			TargetSkillValue->ClearSkillEffectForced();
		}
	}
	if (OwnerActor.IsValid())
	{
		auto ParticleComp = OwnerActor->GetComponentsByClass(UParticleSystemComponent::StaticClass());
		for (auto Element : ParticleComp)
		{
			auto Target = Cast<UParticleSystemComponent>(Element);
			if (Target)
			{
				Target->ActivateSystem(false);
				Target->DeactivateSystem();
				Target->KillParticlesForced();
			}
		}
	}

	ClearPassiveSkillAndSlot();
	ClearAllSlots();
}

void USkillEntityComponent::RemoveItemSkill(FName& ItemSkillID)
{
	if (EntitySkills_V2.Contains(ItemSkillID))
	{
		if (SkillPassiveSlotContainer)
			SkillPassiveSlotContainer->RemoveSlot(ItemSkillID);

		USkill_Passive_V2* ItemSkill = Cast<USkill_Passive_V2>(EntitySkills_V2[ItemSkillID]);
		if (ItemSkill)
		{
			ItemSkill->End();
			EntitySkills_V2.Remove(ItemSkillID);
		}
	}
}

/*
Dispell slots
*/
void USkillEntityComponent::ClearAllSlots()
{
	checkf(SkillSlotContainer != nullptr, TEXT("USkillEntityComponent::ClearAllSlots[SkillSlotContainer Is Empty]"));
// 	if (IsValid(SkillSlotContainer))
	{
		SkillSlotContainer->RemoveAllSlots();
	}
}

void USkillEntityComponent::ClearSlotsByBuffType(EAbnormalApplyTypeEnum InApplyType, int32 InSlotLevel)
{
	checkf(SkillSlotContainer != nullptr, TEXT("USkillEntityComponent::ClearSlotsByBuffType[SkillSlotContainer Is Empty]"));
// 	if (IsValid(SkillSlotContainer))
	{
		SkillSlotContainer->RemoveAllSlotsByBuffType(InApplyType, InSlotLevel );
	}
}

void USkillEntityComponent::ClearSlotsByGroup(EAbnormalGroupTypeEnum InGroupType, int32 InSlotLevel)
{
	checkf(SkillSlotContainer != nullptr, TEXT("USkillEntityComponent::ClearSlotsByGroup[SkillSlotContainer Is Empty]"));
//	if (IsValid(SkillSlotContainer))
	{
		SkillSlotContainer->RemoveAllSlotsByGroup( InGroupType, InSlotLevel);
	}
}

void USkillEntityComponent::ClearSlot(EAbnormalSlotTypeEnum InSlotType)
{
	checkf(SkillSlotContainer != nullptr, TEXT("USkillEntityComponent::ClearSlot[SkillSlotContainer Is Empty]"));
// 	if (IsValid(SkillSlotContainer))
	{
		SkillSlotContainer->RemoveSlot( InSlotType );
	}
}

void USkillEntityComponent::ReservePassiveSkillCondition(ESkillApplierTypeEnum ConditionType)
{
	checkf(SkillEventReactor != nullptr, TEXT("USkillEntityComponent::ReservePassiveSkillCondition[SkillEventReactor Is Empty]"));
// 	if (IsValid(SkillEventReactor))
	{
		SkillEventReactor->AddPassiveSkillConditionCheckList(ConditionType);
	}
}

void USkillEntityComponent::CheckPassiveSkillCondition()
{
	checkf(SkillEventReactor != nullptr, TEXT("USkillEntityComponent::CheckPassiveSkillCondition[SkillEventReactor Is Empty]"));
// 	if (IsValid(SkillEventReactor))
	{
		if (SkillEventReactor->IsNeedToCheckPassiveSkillCondition())
		{
			SkillEventReactor->CheckPassiveSkillCondition(SkillPassiveSlotContainer);
		}
	}
}


void USkillEntityComponent::UpdateSkillInst(float DeltaTime)
{
	if (IsValid(SkillSlotContainer))
	{
		SkillSlotContainer->Update(DeltaTime);
	}

	if (IsValid(SkillPassiveSlotContainer))
	{
		SkillPassiveSlotContainer->Update(DeltaTime);
	}

	for (auto& Elem : EntitySkills_V2)
	{
		if (Elem.Value)
		{
			Elem.Value->Update(DeltaTime);
		}
	}
}

/*
Check state
*/
bool USkillEntityComponent::IsInSlot(EAbnormalSlotTypeEnum InSlotType)
{
	if (IsValid(SkillSlotContainer))
	{
		return SkillSlotContainer->IsInSlot(InSlotType);
	}

	return false;
}

bool USkillEntityComponent::IsInGroup(EAbnormalGroupTypeEnum InGroupType)
{
	if (IsValid(SkillSlotContainer))
	{
		return SkillSlotContainer->IsInGroup(InGroupType);
	}

	return false;
}

bool USkillEntityComponent::IsInBuff(EAbnormalApplyTypeEnum InBuffType)
{
	if (IsValid(SkillSlotContainer))
	{
		return SkillSlotContainer->IsInBuff(InBuffType);
	}

	return false;
}

void USkillEntityComponent::GetPlayerHeroSkillInfos(TArray<FHERO_SKILL>& Infos, int32 kID, FString HeroUD, AActor* InTargetActor)
{
	Infos.Empty(5);

	if (nullptr == InTargetActor)
		return;

	URGameInstance* RGameInstance = RGAMEINSTANCE(GEngine);
// 	if (!IsValid(RGameInstance))
// 		return;

	if (kID == 0 || RGameInstance->UserInfo->kId == kID)
	{
		RGameInstance->RInventory->GetSkillDataFromHeroUD(HeroUD, Infos);
	}
	else
	{
		TArray<FEQUIPED_HERO> HeroDataList;
		if (RGameInstance->OtherHeroManager->GetOtherHeroList(kID, HeroDataList))
		{
			for (auto& HeroData : HeroDataList)
			{
				if (HeroData.heroUD == HeroUD)
				{
					int32 SkillCount = HeroData.skills.Num();
					Infos.Init(FHERO_SKILL(), SkillCount);
					for (int32 n = 0; n < SkillCount; ++n)
					{
						UCharacterHelpFunctionLibrary::MakeHeroSkillStructure(HeroData.heroId, HeroData.skills[n], n, Infos[n]);
					}

					break;
				}
			}
		}
	}

	if (0 == Infos.Num() || Infos.Num() != 4)
	{
		CreateDummySkills(Infos, InTargetActor);
	}
}

void USkillEntityComponent::CreateDummySkills(TArray<FHERO_SKILL>& SkillData, AActor* InTargetActor)
{
	checkf(InTargetActor != nullptr, TEXT("USkillEntityComponent::CreateDummySkills[InTargetActor Is Empty]"));
	auto InTargetActor_BattleEntityComponent = UCharacterHelpFunctionLibrary::GetBattleEntityComponent(InTargetActor);
	auto InTargetActor_BaseProperty = Cast<IEntityBaseProperty>(InTargetActor);
	if (IsValid(InTargetActor_BattleEntityComponent) && InTargetActor_BaseProperty != nullptr)
	{
		bool IsCustomGame = UUtilFunctionLibrary::IsCustomGame(InTargetActor->GetWorld());

		if (InTargetActor_BattleEntityComponent->GetBattleInfo()->ActiveSkill01.IsNone() == false)
		{
			FHERO_SKILL Skill;
			Skill.slotNo = (int32)EHERO_SKILL_SLOT::ActiveSkill01;
			if (IsCustomGame)
			{
				Skill.level = 4;
			}
			else
			{
				Skill.level = 1;
				if (SkillData.IsValidIndex(Skill.slotNo))
					Skill.level += SkillData[Skill.slotNo].level;
			}

			Skill.skillId = InTargetActor_BattleEntityComponent->GetBattleInfo()->ActiveSkill01.ToString();
			Skill.activated = true;
			
			if (!SkillData.IsValidIndex(Skill.slotNo))
				SkillData.Emplace(Skill);
			else
				SkillData[Skill.slotNo] = Skill;
		}
		if (InTargetActor_BattleEntityComponent->GetBattleInfo()->ActiveSkill02.IsNone() == false)
		{
			FHERO_SKILL Skill;
			Skill.slotNo = (int32)EHERO_SKILL_SLOT::ActiveSkill02;
			if (IsCustomGame)
			{
				Skill.level = 4;
			}
			else
			{
				Skill.level = 1;
				if (SkillData.IsValidIndex(Skill.slotNo))
					Skill.level += SkillData[Skill.slotNo].level;
			}

			Skill.skillId = InTargetActor_BattleEntityComponent->GetBattleInfo()->ActiveSkill02.ToString();
			Skill.activated = true;
			if (!SkillData.IsValidIndex(Skill.slotNo))
				SkillData.Emplace(Skill);
			else
				SkillData[Skill.slotNo] = Skill;
		}
		if (InTargetActor_BattleEntityComponent->GetBattleInfo()->ActiveSkill03.IsNone() == false)
		{
			FHERO_SKILL Skill;
			Skill.slotNo = (int32)EHERO_SKILL_SLOT::ActiveSkill03;
			if (IsCustomGame)
			{
				Skill.level = 4;
			}
			else
			{
				Skill.level = 1;
				if (SkillData.IsValidIndex(Skill.slotNo))
					Skill.level += SkillData[Skill.slotNo].level;
			}

			Skill.skillId = InTargetActor_BattleEntityComponent->GetBattleInfo()->ActiveSkill03.ToString();
			Skill.activated = true;
			if (!SkillData.IsValidIndex(Skill.slotNo))
				SkillData.Emplace(Skill);
			else
				SkillData[Skill.slotNo] = Skill;
		}

		if (InTargetActor_BattleEntityComponent->GetBattleInfo()->PassiveSkill01.IsNone() == false)
		{
			FHERO_SKILL Skill;
			Skill.slotNo = (int32)EHERO_SKILL_SLOT::PassiveSkill01;
			if (IsCustomGame)
			{
				Skill.level = 4;
			}
			else
			{
				Skill.level = 1;
				if (SkillData.IsValidIndex(Skill.slotNo))
					Skill.level += SkillData[Skill.slotNo].level;
			}

			Skill.skillId = InTargetActor_BattleEntityComponent->GetBattleInfo()->PassiveSkill01.ToString();
			Skill.activated = true;
			if (!SkillData.IsValidIndex(Skill.slotNo))
				SkillData.Emplace(Skill);
			else
				SkillData[Skill.slotNo] = Skill;
		}

		if (InTargetActor_BattleEntityComponent->GetBattleInfo()->PassiveSkill02.IsNone() == false)
		{
			FHERO_SKILL Skill;

			if (IsCustomGame)
			{
				Skill.level = 4;
			}
			else
			{
				Skill.level = InTargetActor_BaseProperty->GetLevel() / 10 + 1;
			}

			Skill.skillId = InTargetActor_BattleEntityComponent->GetBattleInfo()->PassiveSkill02.ToString();
			Skill.slotNo = (int32)EHERO_SKILL_SLOT::PassiveSkill02;
			Skill.activated = true;
			SkillData.Emplace(Skill);
		}

#if WITH_EDITOR
		if (IsValid(InTargetActor))
		{
			URGameInstance* RGameInstance = RGAMEINSTANCE(GEngine);
			if ( IsValid(RGameInstance->EditorManager))
			{
				if (IsValid(RGameInstance->EditorManager->GetEditorActor()))
				{
					ASpawnerEditor* Editor = Cast<ASpawnerEditor>(RGameInstance->EditorManager->GetEditorActor());
					if (IsValid(Editor))
					{
						Editor->ChangeSkillLevel(SkillData, InTargetActor);
					}
				}
			}
		}
#endif
	}
}

FVector USkillEntityComponent::GetSearchShapeInfobySkill(UISkill_V2* Skill)
{
	FVector SearchValue = FVector::ZeroVector;
	//FVector SearchValue = FVector(400, 90, 200);

	if (nullptr == Skill)
		return SearchValue;

	ESearchShapeTypeEnum		EffectShape = ESearchShapeTypeEnum::VE_None;
	TArray<float> ShapeScope;
	int32 CastingRange = 100;
	switch (Skill->eSkillType)
	{
	case ESkillType::eSkillType_Normal:
	{
		FSkillTableInfo_V2* SkillInfo = Skill->GetSkillInfo();
		EffectShape = SkillInfo->EffectShape;
		ShapeScope = SkillInfo->ShapeScope;
		CastingRange = SkillInfo->CastingRange;
	}
	break;
	}

	if (ESearchShapeTypeEnum::VE_Circle == EffectShape)
	{
		if (3 <= ShapeScope.Num())
		{
			SearchValue.X = ShapeScope[0]; // Radius
			SearchValue.Y = ShapeScope[2]; // Height
		}
	}
	else if (ESearchShapeTypeEnum::VE_Sector == EffectShape)
	{
		if (3 <= ShapeScope.Num())
		{
			SearchValue.X = ShapeScope[0]; // Radius
			SearchValue.Y = ShapeScope[1]; // Angle
			SearchValue.Z = ShapeScope[2]; // Height
		}
	}
	else if (ESearchShapeTypeEnum::VE_Box == EffectShape)
	{
		if (3 <= ShapeScope.Num())
		{
			SearchValue.X = ShapeScope[0];  // Length
			SearchValue.Y = ShapeScope[1]; // Width
			SearchValue.Z = ShapeScope[2]; // Height
		}
	}
	return SearchValue;
}

// from manager
FVector USkillEntityComponent::GetSearchShapeInfo(FSkillTableInfo_V2* SkillInfo)
{
	FVector SearchValue = FVector::ZeroVector;
	//FVector SearchValue = FVector(400, 90, 200);

	if (nullptr == SkillInfo)
		return SearchValue;

	if (ESearchShapeTypeEnum::VE_Circle == SkillInfo->EffectShape)
	{
		if (2 <= SkillInfo->ShapeScope.Num())
		{
			SearchValue.X = SkillInfo->ShapeScope[0]; // Radius
			SearchValue.Y = SkillInfo->ShapeScope[2]; // Height
		}
	}
	else if (ESearchShapeTypeEnum::VE_Sector == SkillInfo->EffectShape)
	{
		if (3 <= SkillInfo->ShapeScope.Num())
		{
			SearchValue.X = SkillInfo->ShapeScope[0]; // Radius
			SearchValue.Y = SkillInfo->ShapeScope[1]; // Angle
			SearchValue.Z = SkillInfo->ShapeScope[2]; // Height
		}
	}
	else if (ESearchShapeTypeEnum::VE_Box == SkillInfo->EffectShape)
	{
		if (3 <= SkillInfo->ShapeScope.Num())
		{
			SearchValue.X = SkillInfo->CastingRange;  // Length
			SearchValue.Y = SkillInfo->ShapeScope[1]; // Width
			SearchValue.Z = SkillInfo->ShapeScope[2]; // Height
		}
	}
	return SearchValue;
}

AActor* USkillEntityComponent::FindEnemySkillTarget(FSkillTableInfo_V2* SkillInfo, AActor* Caster)
{
	if (nullptr == SkillInfo || !IsValid(Caster))
		return nullptr;

	auto CasterBattleComp = UCharacterHelpFunctionLibrary::GetBattleEntityComponent(Caster);
	if (nullptr == CasterBattleComp)
		return nullptr;

	AActor* CurrentAttackTarget = CasterBattleComp->GetCurrentAttackTarget();
	if (CurrentAttackTarget)
	{
		if (CasterBattleComp->IsTargetInRange(CurrentAttackTarget, SkillInfo->CastingRange))
		{
			return CurrentAttackTarget;
		}
	}
	else
	{
		AActor* NearEnemy = FindNearestSkillTarget(Caster, EFriendshipTypeEnum::VE_Enemy);
		if (IsValid(NearEnemy))
		{
			if (CasterBattleComp->IsTargetInRange(NearEnemy, SkillInfo->CastingRange))
			{
				return NearEnemy;
			}
		}
	}
	return nullptr;
}

AActor* USkillEntityComponent::FindFriendSkillTarget(FSkillTableInfo_V2* SkillInfo, AActor* Caster)
{
	if (nullptr == SkillInfo || !IsValid(Caster))
		return nullptr;

	AActor* NearFriend = FindNearestSkillTarget(Caster, EFriendshipTypeEnum::VE_FriendExceptMe);
	if (IsValid(NearFriend))
	{
		auto CasterBattleComp = UCharacterHelpFunctionLibrary::GetBattleEntityComponent(Caster);
		if (nullptr == CasterBattleComp)
			return nullptr;

		if (CasterBattleComp->IsTargetInRange(NearFriend, SkillInfo->CastingRange))
		{
			return NearFriend;
		}
	}

	return Caster;
}

AActor* USkillEntityComponent::FindNearestSkillTarget(AActor* Caster, EFriendshipTypeEnum FindType)
{
	TArray< TWeakObjectPtr<AActor> > TargetList;
	URenewal_BattleFunctionLibrary::FindFriendshipType(TargetList, Caster, FindType);

	AActor* NearTarget = nullptr;
	if (0 < TargetList.Num())
	{
		FVector CasterLoc = Caster->GetActorLocation();
		FVector TargetLoc = FVector::ZeroVector;

		float NearDist = 999999.0f;
		float Dist = 999999.0f;
		for (auto Target : TargetList)
		{
			TargetLoc = Target->GetActorLocation();
			Dist = FVector::DistSquared(CasterLoc, TargetLoc);
			if (Dist < NearDist)
			{
				NearDist = Dist;
				NearTarget = Target.Get();
			}
		}
	}
	return NearTarget;
}


FVector USkillEntityComponent::FindSkillTargetLocation(FSkillTableInfo_V2* SkillInfo, AActor* Caster)
{
	if (nullptr == SkillInfo || !IsValid(Caster))
		return FVector::ZeroVector;

	AActor* Target = nullptr;
	if (EFriendshipTypeEnum::VE_Enemy == SkillInfo->TargetApply)
	{
		Target = FindEnemySkillTarget(SkillInfo, Caster);
	}
	else if (EFriendshipTypeEnum::VE_FriendParty == SkillInfo->TargetApply)
	{
		Target = FindNearestSkillTarget(Caster, EFriendshipTypeEnum::VE_FriendParty);
	}
	else if (EFriendshipTypeEnum::VE_Friend == SkillInfo->TargetApply)
	{
		Target = FindFriendSkillTarget(SkillInfo, Caster);
	}
	else
	{
		Target = Caster;
	}

	FVector Location = FVector::ZeroVector;

	auto TargetBattleEntityComp = UCharacterHelpFunctionLibrary::GetBattleEntityComponent(Target);
	if (!IsValid(TargetBattleEntityComp))
		return Location;

	if (IsValid(Target))
	{
		auto BossComp = TargetBattleEntityComp->GetBossComponent();
		if (BossComp)
		{
			UGigantComponent* GigantComp = Cast<UGigantComponent>(BossComp);
			UAkhanComponent* AkhanComp = Cast<UAkhanComponent>(BossComp);
			if (GigantComp || AkhanComp) // They can fly, so process exception
			{
				Location = UMapFunctionLibrary::GetNavAgentLocation(Target);
				FVector CasterLoc = UMapFunctionLibrary::GetNavAgentLocation(Caster);;
				Location.Z = CasterLoc.Z;
			}
			else
			{
				Location = UMapFunctionLibrary::GetNavAgentLocation(Target);
			}
		}
		else
		{
			Location = UMapFunctionLibrary::GetNavAgentLocation(Target);
		}
	}
	return Location;
}

bool USkillEntityComponent::BlockMoveingCatetoryApplierCheck(AActor* Caster, AActor* Target, FSkillApplierBase& InSkillApplier)
{
	UBattleEntityComponent* TargetBattleEntityComponent = UCharacterHelpFunctionLibrary::GetBattleEntityComponent(Target);
	if (!IsValid(TargetBattleEntityComponent))
		return false;

	
	switch (InSkillApplier.SkillApplierType)
	{
	case ESkillApplierTypeEnum::VE_i_pull:
	case ESkillApplierTypeEnum::VE_i_pull_Amon:
	case ESkillApplierTypeEnum::VE_i_push:
	{
		
		if (TargetBattleEntityComponent->IsInAbnormalStateApplier(ESkillApplierTypeEnum::VE_p_block_pull))
		{
			TargetBattleEntityComponent->OnSetBlockAbnormalMsg.Broadcast(ESkillApplierTypeEnum::VE_p_block_pull);
			return true;
		}

		if (TargetBattleEntityComponent->IsInAbnormalStateApplier(ESkillApplierTypeEnum::VE_p_block_push))
		{
			TargetBattleEntityComponent->OnSetBlockAbnormalMsg.Broadcast(ESkillApplierTypeEnum::VE_p_block_push);
			return true;
		}
	}
	break;
	}

	return false;
}

UISkill_V2* USkillEntityComponent::CreateSkill(AActor* Caster, FName SkillID, int32 Level)
{
	if (IsValid(Caster) == false)
		return nullptr;

	IEntityBaseProperty* CasterEntityBaseProperty = Cast<IEntityBaseProperty>(Caster);
	if (CasterEntityBaseProperty == nullptr)
		return nullptr;

	FName SkillTableID = UUtilFunctionLibrary::GetResultSkillID(CasterEntityBaseProperty->GetCharacterCategory(), SkillID, Level);
	//SkillTableID.AppendString(SkillLevelString);

	FSkillTableInfo_V2* SkillInfo = URenewal_BattleFunctionLibrary::GetSkillTableInfo_V2(CasterEntityBaseProperty->GetCharacterCategory(), SkillTableID);
	if (SkillInfo == nullptr)
		return nullptr;

	UISkill_V2* Skill = nullptr;
	if (ESkillCastingTypeEnum::VE_Casting == SkillInfo->CastingType ||
		ESkillCastingTypeEnum::VE_Immediate == SkillInfo->CastingType)
	{
		Skill = NewObject< UISkill_V2 >((UObject*)GetTransientPackage(), UISkill_V2::StaticClass());
	}
	else if (ESkillCastingTypeEnum::VE_Channeling == SkillInfo->CastingType)
	{
		Skill = NewObject< USkill_Channel >((UObject*)GetTransientPackage(), USkill_Channel::StaticClass());
	}

	if (nullptr == Skill)
		return nullptr;

	Skill->InitSkill(SkillID, SkillTableID, Caster, Level, SkillInfo);

	return Skill;
}

USkill_Passive_V2* USkillEntityComponent::CreatePassiveSkill(AActor* Caster, FName SkillID, int32 Level)
{
	if (IsValid(Caster) == false)
		return nullptr;

	IEntityBaseProperty* CasterEntityBaseProperty = Cast<IEntityBaseProperty>(Caster);
	if (CasterEntityBaseProperty == nullptr)
		return nullptr;

	FName SkillTableID = UUtilFunctionLibrary::GetResultSkillID(CasterEntityBaseProperty->GetCharacterCategory(), SkillID, Level);
	FSkillTableInfo_V2* SkillInfo = URenewal_BattleFunctionLibrary::GetSkillTableInfo_V2(CasterEntityBaseProperty->GetCharacterCategory(), SkillTableID);
	if (SkillInfo == nullptr)
		return nullptr;

	USkill_Passive_V2* Skill = NewObject< USkill_Passive_V2 >((UObject*)GetTransientPackage(), USkill_Passive_V2::StaticClass());
	if (nullptr == Skill)
		return nullptr;

	Skill->InitSkill(SkillID, SkillTableID, Caster, Level, SkillInfo);

	return Skill;
}


USkill_Passive_V2* USkillEntityComponent::CreateSpiritSkill(AActor* Caster, FName SkillID, int32 Level)
{
	if (IsValid(Caster) == false)
		return nullptr;

	IEntityBaseProperty* CasterEntityBaseProperty = Cast<IEntityBaseProperty>(Caster);
	if (CasterEntityBaseProperty == nullptr)
		return nullptr;

	FSkillTableInfo_V2* SkillInfo = URenewal_BattleFunctionLibrary::GetSkillTableInfo_V2(CasterEntityBaseProperty->GetCharacterCategory(), SkillID);
	if (SkillInfo == nullptr)
		return nullptr;

	USkill_Passive_V2* Skill = NewObject< USkill_Passive_V2 >((UObject*)GetTransientPackage(), USkill_Passive_V2::StaticClass());
	if (nullptr == Skill)
		return nullptr;

	Skill->InitSkill(SkillID, SkillID, Caster, Level, SkillInfo);

	return Skill;
}

bool USkillEntityComponent::CastInputSkill(FName SkillNO, AActor* Caster, AActor* ActorTarget, FVector LocationTarget /*= FVector::ZeroVector*/)
{
	if (!IsValid(Caster))
		return false;

	auto BattleEntityComponent = UCharacterHelpFunctionLibrary::GetBattleEntityComponent(Caster);
	if (!IsValid(BattleEntityComponent))
		return false;

	auto SkillEntityComponent = UCharacterHelpFunctionLibrary::GetSkillEntityComponent(Caster);
	if (!IsValid(SkillEntityComponent))
		return false;

	URGameInstance* RGameInstance = RGAMEINSTANCE(GEngine);
// 	if (!IsValid(RGameInstance))
// 		return false;

	ABasePlayerController* PlayerController = UUtilFunctionLibrary::GetBasePlayerController(Caster->GetWorld());
	if (!IsValid(PlayerController))
		return false;

	UISkill_V2* OwnerSkill = SkillEntityComponent->GetSkill_V2(SkillNO);
	if (!IsValid(OwnerSkill))
		return false;

	FSkillTableInfo_V2* OwnerSkillInfo = OwnerSkill->GetSkillInfo();
	if (nullptr == OwnerSkillInfo)
		return false;

	if (SkillEntityComponent->IsSkillActing())
		return false;

	if (ESkillTargetTypeEnum::VE_Me == OwnerSkillInfo->TargetType && LocationTarget.IsNearlyZero())
	{
		UEntityAIComponent* EntityAIComponent = UCharacterHelpFunctionLibrary::GetEntityAIComponent(Caster);
		if (EntityAIComponent)
		{
			AActor* ForceTarget = EntityAIComponent->GetBlackboardActorValue(ERAIBalckboardValueEnum::VE_ForceTarget);
			if (ForceTarget)
			{
				BattleEntityComponent->SetCurrentAttackTarget(ForceTarget);
			}
		}
	}

	// bryan 180322 - enable skill fire even if there is no target
	if (false == CheckSkillTarget(OwnerSkill, Caster, ActorTarget, LocationTarget))
		return false;



	return SkillEntityComponent->CastNeedTargetSkill_V2(SkillNO, ActorTarget, LocationTarget);
}

bool USkillEntityComponent::CheckSkillTarget(UISkill_V2* CastSkill, AActor* Caster, AActor* &ActorTarget, FVector& LocationTarget)
{
	if (!IsValid(CastSkill))
		return false;

	if (!IsValid(Caster))
		return false;

	auto BattleEntityComponent = UCharacterHelpFunctionLibrary::GetBattleEntityComponent(Caster);
	if (!IsValid(BattleEntityComponent))
		return false;

	FSkillTableInfo_V2* CastSkillInfo = CastSkill->GetSkillInfo();
	if (!CastSkillInfo)
		return false;

	if (ESkillTargetTypeEnum::VE_Target == CastSkillInfo->TargetType)
	{
		if (EFriendshipTypeEnum::VE_Enemy == CastSkillInfo->TargetApply ||
			EFriendshipTypeEnum::VE_EnemyLowestHP == CastSkillInfo->TargetApply)
		{
			if (!IsValid(ActorTarget) || BattleEntityComponent->IsEnemy(ActorTarget) == false)
			{
				ActorTarget = FindEnemySkillTarget(CastSkillInfo, Caster);
			}
		}
		else if (EFriendshipTypeEnum::VE_FriendParty == CastSkillInfo->TargetApply)
		{
			if (!IsValid(ActorTarget) || !URenewal_BattleFunctionLibrary::IsPartyActor(Caster, ActorTarget))
			{
				ActorTarget = FindNearestSkillTarget(Caster, EFriendshipTypeEnum::VE_FriendParty);
			}
		}
		else if (EFriendshipTypeEnum::VE_All == CastSkillInfo->TargetApply)
		{
			if (!IsValid(ActorTarget))
			{
				return false;
			}
		}
		else
		{
			if (!IsValid(ActorTarget) || BattleEntityComponent->IsEnemy(ActorTarget) == true)
			{
				ActorTarget = FindFriendSkillTarget(CastSkillInfo, Caster);
			}
		}

		if (!IsValid(ActorTarget))
		{
#if LOG_SKILL
			UE_LOG(LogBattle, Log, TEXT("SkillCancel : ESkillTargetTypeEnum::VE_Target : Target Actor null"));
#endif
			return false;
		}
		else
		{
			if (!BattleEntityComponent->IsTargetInRange(ActorTarget, CastSkillInfo->CastingRange))
			{
				return false;
			}
		}
	}
	else if (ESkillTargetTypeEnum::VE_TargetLocation == CastSkillInfo->TargetType)
	{
		if (LocationTarget.IsNearlyZero())
		{
			LocationTarget = FindSkillTargetLocation(CastSkillInfo, Caster);
		}

		if (LocationTarget.IsNearlyZero())
		{
			LocationTarget = Caster->GetActorLocation() + (Caster->GetActorForwardVector() * 100.0f);
		}
	}
	else if (ESkillTargetTypeEnum::VE_Me == CastSkillInfo->TargetType)
	{
		ActorTarget = Caster;
		if (LocationTarget.IsNearlyZero())
		{
			LocationTarget = FindSkillTargetLocation(CastSkillInfo, Caster);

			if (ESearchShapeTypeEnum::VE_Box == CastSkillInfo->EffectShape)
			{
				FVector CasterLoc = UMapFunctionLibrary::GetNavAgentLocation(Caster);
				if (LocationTarget.IsNearlyZero())
				{
					LocationTarget = CasterLoc + Caster->GetActorForwardVector() * CastSkillInfo->CastingRange * 0.5f;
				}
				else
				{
					FVector Dir = LocationTarget - CasterLoc;
					Dir.Normalize();
					LocationTarget = CasterLoc + Dir * CastSkillInfo->CastingRange * 0.5f;
				}
			}
			else
			{
				if (LocationTarget.IsNearlyZero())
				{
					LocationTarget = UMapFunctionLibrary::GetNavAgentLocation(Caster);
				}
			}
		}
		else if (ESearchShapeTypeEnum::VE_Box == CastSkillInfo->EffectShape)
		{
			FVector CasterLoc = UMapFunctionLibrary::GetNavAgentLocation(Caster);
			FVector Dir = LocationTarget - CasterLoc;
			Dir.Normalize();
			LocationTarget = CasterLoc + Dir * CastSkillInfo->CastingRange * 0.5f;
		}
	}

	return true;
}

void USkillEntityComponent::GetSkillTarget(TArray< TWeakObjectPtr<AActor> >& outArray, UISkill_V2* Skill)
{
	if (Skill)
	{
		GetSkillApplyTarget(outArray, Skill, Skill->GetTarget(), Skill->GetTargetLocation());
	}
}

void USkillEntityComponent::GetSkillApplyTarget(TArray< TWeakObjectPtr<AActor> >& outArray, UISkill_V2* Skill, AActor* ActorTarget, FVector LocationTarget)
{
	outArray.Empty();

	if (nullptr == Skill)
		return;

	AActor* Caster = Skill->GetCaster();
	if (!Skill->IsValidCaster())
		return;

	IEntityBaseProperty* CasterEntityBaseProperty = Cast<IEntityBaseProperty>(Caster);
	if (CasterEntityBaseProperty == nullptr)
		return;

	auto Caster_BattleEntityComponent = UCharacterHelpFunctionLibrary::GetBattleEntityComponent(Caster);
	if (Caster_BattleEntityComponent == nullptr)
		return;

	ESearchShapeTypeEnum SkillShapeType = ESearchShapeTypeEnum::VE_None;
	ESkillTargetTypeEnum TargetType = ESkillTargetTypeEnum::VE_None;
	EFriendshipTypeEnum ApplyTargetType = EFriendshipTypeEnum::VE_None;

	FSkillTableInfo_V2* SkillInfo = nullptr;//Skill->GetSkillInfo();
	FRuneWordSkillTable* RuneSkillInfo = nullptr;
	FTowerSkillTable* TowerSKillInfo = nullptr;
	int32 CastingRange = 0;

	switch (Skill->eSkillType)
	{
	case ESkillType::eSkillType_Normal:
	{
		SkillInfo = Skill->GetSkillInfo();
		SkillShapeType = SkillInfo->EffectShape;
		TargetType = SkillInfo->TargetType;
		ApplyTargetType = SkillInfo->TargetApply;
		CastingRange = SkillInfo->CastingRange;

	}
	break;

	default:	break;
	}

	if (ESearchShapeTypeEnum::VE_None == SkillShapeType)
	{
		if (ESkillTargetTypeEnum::VE_Target == TargetType ||
			ESkillTargetTypeEnum::VE_Me == TargetType)
		{
			if (!IsValid(ActorTarget))
			{

			}

			if (EFriendshipTypeEnum::VE_Enemy == ApplyTargetType)
			{
				if (URenewal_BattleFunctionLibrary::IsEnemyActor(Caster, ActorTarget))
				{
					if (Caster_BattleEntityComponent->IsTargetInRange(ActorTarget, CastingRange))
					{
						outArray.Emplace(ActorTarget);
					}
				}
			}
			else if (EFriendshipTypeEnum::VE_FriendParty == ApplyTargetType)
			{
				if (URenewal_BattleFunctionLibrary::IsPartyActor(Caster, ActorTarget))
				{
					if (Caster_BattleEntityComponent->IsTargetInRange(ActorTarget, CastingRange))
					{
						outArray.Emplace(ActorTarget);
					}
				}
			}
			else if (EFriendshipTypeEnum::VE_Friend == ApplyTargetType)
			{
				if (URenewal_BattleFunctionLibrary::IsEnemyActor(Caster, ActorTarget) == false)
				{
					if (Caster_BattleEntityComponent->IsTargetInRange(ActorTarget, CastingRange))
					{
						outArray.Emplace(ActorTarget);
					}
				}
			}
			else if (EFriendshipTypeEnum::VE_All == ApplyTargetType)
			{
				if (Caster_BattleEntityComponent->IsTargetInRange(ActorTarget, CastingRange))
				{
					outArray.Emplace(ActorTarget);
				}
			}
			else if (EFriendshipTypeEnum::VE_None == ApplyTargetType && ESkillTargetTypeEnum::VE_Me == TargetType)
			{

				if (IsValid(Caster))
					outArray.Emplace(Caster);
			}
		}
	}
	else
	{
		FVector TargetLocation = Caster->GetActorLocation();
		if (ESearchShapeTypeEnum::VE_Circle == SkillShapeType)
		{
			if (ESkillTargetTypeEnum::VE_TargetLocation == TargetType)
			{
				TargetLocation = LocationTarget;
			}
			else if (ESkillTargetTypeEnum::VE_Target == TargetType)
			{
				if (IsValid(ActorTarget))
				{
					TargetLocation = ActorTarget->GetActorLocation();
				}
			}
		}
		else if (ESearchShapeTypeEnum::VE_Box == SkillShapeType)
		{
			// adjust center pivot.. ( move box length / 2 )
			ABattleBaseObject* BattleObject = Cast<ABattleBaseObject>(Caster);
			if (IsValid(BattleObject))
			{
				TargetLocation += BattleObject->GetBattleObjectForward() * CastingRange * 0.5f;
			}
			else
			{
				FVector Direction = LocationTarget - Caster->GetActorLocation();
				Direction = Direction.GetSafeNormal2D();
				float HalfLength = SkillInfo->ShapeScope[0] * 0.5f;

				TargetLocation += HalfLength * Direction;
			}
		}
		else if (ESearchShapeTypeEnum::VE_Sector == SkillShapeType)
			TargetLocation = LocationTarget;

		FVector SearchValue = GetSearchShapeInfobySkill(Skill);

		AProjectRGameMode* pGameMode = Cast<AProjectRGameMode>(UGameplayStatics::GetGameMode(RGAMEINSTANCE(GEngine)));
		if (pGameMode)
		{
			pGameMode->ObjectHelper.FindTarget(outArray, Caster, TargetLocation,
				ApplyTargetType, SkillShapeType, SearchValue, 0.0f,
				CasterEntityBaseProperty->GetEntityBasePropertyInfo()->bDrawSkillShape);
		}

	}
}

ANormalBullet* USkillEntityComponent::FireTargetBullet(UISkill_V2* Skill, AActor* Caster, AActor* Target, bool IsRealBullet)
{
	checkf(Skill != nullptr, TEXT("USkillEntityComponent::FireTargetBullet[Skill Is Empty]"));
	if (Target == nullptr)
		return nullptr;

	FSkillTableInfo_V2* SkillInfo = Skill->GetSkillInfo();
	if (SkillInfo == nullptr)
		return nullptr;

	FSkillResourceTableInfo* SkillResourceInfo = Skill->GetSkillResourceInfo();
	if (SkillResourceInfo == nullptr)
		return nullptr;

	UClass* CheckBulletClass = nullptr;
	FBulletTableInfo* BulletTableInfo = UTableManager::GetInstancePtr()->GetBulletRow(SkillResourceInfo->BulletID);
	if (BulletTableInfo == nullptr)
		return nullptr;

	CheckBulletClass = UUtilFunctionLibrary::GetBulletBaseFromAssetID(BulletTableInfo->BulletClass);
	if (CheckBulletClass == nullptr || !CheckBulletClass->IsChildOf(ANormalBullet::StaticClass()))
	{
		UE_LOG(LogCriticalErrors, Log, TEXT("SkillResourceInfo->BulletClass Is Not ANormalBullet Class!!! %s"), *(BulletTableInfo->BulletClass.ToSoftObjectPath().ToString()));
		return nullptr;
	}

	auto Caster_BattleEntityComponent = UCharacterHelpFunctionLibrary::GetBattleEntityComponent(Caster);
	if (Caster_BattleEntityComponent == nullptr)
		return nullptr;

	FVector ShotLocation = Caster_BattleEntityComponent->GetShotPointLocation(BulletTableInfo->BulletSocket);

	FActorSpawnParameters SpawnParams;
	SpawnParams.Owner = Caster;
	SpawnParams.Instigator = Caster->Instigator;
	SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;
	SpawnParams.bNoFail = true;

	ANormalBullet* Projectile = Caster->GetWorld()->SpawnActor<ANormalBullet>(CheckBulletClass, ShotLocation, FRotator::ZeroRotator, SpawnParams);
	if (Projectile)
	{
		Projectile->InitBullet(Caster, BulletTableInfo->BulletSpeed, ShotLocation, Target, FVector::ZeroVector);

		// Real Bullet
		if (IsRealBullet)
		{
			Projectile->SetSkillAttackBullet(Skill);
		}

		if (UUtilFunctionLibrary::IsLeaderCharacter(Caster))
		{
			checkf(RGAMEINSTANCE(GEngine), TEXT("GameInstance null"));

			if (RGAMEINSTANCE(GEngine)->PointLightManager)
			{
				RGAMEINSTANCE(GEngine)->PointLightManager->TargetBulletArray.Emplace(Projectile);
			}
		}
	}

	return Projectile;
}

void USkillEntityComponent::FireMultiBullet(UISkill_V2* Skill, AActor* Caster, AActor* ActorTarget, FVector LocationTarget, bool IsRealBullet)
{
	checkf(Skill != nullptr, TEXT("USkillEntityComponent::FireMultiBullet[Skill Is Empty]"));
// 	if (Skill == nullptr)
// 		return;

	FSkillTableInfo_V2* SkillInfo = Skill->GetSkillInfo();
	if (SkillInfo == nullptr)
		return;

	FSkillResourceTableInfo* SkillResourceInfo = Skill->GetSkillResourceInfo();
	if (SkillResourceInfo == nullptr)
		return;

	UClass* CheckBulletClass = nullptr;
	FBulletTableInfo* BulletTableInfo = UTableManager::GetInstancePtr()->GetBulletRow(SkillResourceInfo->BulletID);
	if (BulletTableInfo == nullptr)
		return;

	CheckBulletClass = UUtilFunctionLibrary::GetBulletBaseFromAssetID(BulletTableInfo->BulletClass);
	if (CheckBulletClass == nullptr || !CheckBulletClass->IsChildOf(ANormalBullet::StaticClass()))
	{
		UE_LOG(LogCriticalErrors, Log, TEXT("SkillResourceInfo->BulletClass Is Not ANormalBullet Class!!! %s"), *(BulletTableInfo->BulletClass.ToSoftObjectPath().ToString()));
		return;
	}

	auto Caster_BattleEntityComponent = UCharacterHelpFunctionLibrary::GetBattleEntityComponent(Caster);
	if (Caster_BattleEntityComponent == nullptr)
		return;

	FVector ForwardDir = Caster->GetActorForwardVector();
	FVector ShotLocation = Caster_BattleEntityComponent->GetShotPointLocation(BulletTableInfo->BulletSocket);
	float TargetDist = 0.0f;
	float BulletFlyDist = 0.0f;
	if (ESkillTargetTypeEnum::VE_Target == SkillInfo->TargetType)
	{
		if (!IsValid(ActorTarget))
			return;

		ForwardDir = ActorTarget->GetActorLocation() - ShotLocation;
		TargetDist = ForwardDir.Size();
		ForwardDir.Normalize();
		BulletFlyDist = SkillInfo->CastingRange;
	}
	else if (ESkillTargetTypeEnum::VE_TargetLocation == SkillInfo->TargetType)
	{
		if (LocationTarget.IsNearlyZero())
			return;

		ForwardDir = LocationTarget - ShotLocation;
		BulletFlyDist = ForwardDir.Size();
		ForwardDir.Normalize();
	}
	else
	{
		BulletFlyDist = SkillInfo->CastingRange;
	}

	FActorSpawnParameters SpawnParams;
	SpawnParams.Owner = Caster;
	SpawnParams.Instigator = Caster->Instigator;
	SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;
	SpawnParams.bNoFail = true;

	float SectorAngle = 60.0f;
	if (ESearchShapeTypeEnum::VE_Sector == SkillInfo->EffectShape && SkillInfo->ShapeScope.IsValidIndex(1))
	{
		SectorAngle = SkillInfo->ShapeScope[1];
	}
	float DegreeInterval = 10.0f;
	int32 BulletMax = int32((SectorAngle / DegreeInterval) + 1);

	FVector ShotDir = ForwardDir;
	int32 CenterIndex = BulletMax >> 1;
	ANormalBullet* Projectile = nullptr;
	float FireDegree = 0.0f;
	FVector AimLocation = FVector::ZeroVector;
	for (int32 n = 0; n < BulletMax; ++n)
	{
		Projectile = Caster->GetWorld()->SpawnActor<ANormalBullet>(CheckBulletClass, ShotLocation, FRotator::ZeroRotator, SpawnParams);
		if (Projectile)
		{
			FireDegree = (SectorAngle * 0.5f) - (n * DegreeInterval);
			ShotDir = ForwardDir.RotateAngleAxis(FireDegree, FVector::UpVector);
			AimLocation = ShotLocation + (ShotDir * BulletFlyDist);

			Projectile->InitBullet(Caster, BulletTableInfo->BulletSpeed, ShotLocation, nullptr, AimLocation);

			// Real Bullet
			if (IsRealBullet)
			{
				if (CenterIndex == n)
				{
					Projectile->SetSkillAttackBullet(Skill, TargetDist);
				}
			}

			if (UUtilFunctionLibrary::IsLeaderCharacter(Caster))
			{
				checkf(RGAMEINSTANCE(GEngine), TEXT("GameInstance null"));

				if (RGAMEINSTANCE(GEngine)->PointLightManager)
				{
					RGAMEINSTANCE(GEngine)->PointLightManager->TargetBulletArray.Emplace(Projectile);
				}
			}
		}
	}
	return;
}

bool USkillEntityComponent::FireLocationBullet(UISkill_V2* Skill, AActor* Caster, FVector location, bool IsRealBullet)
{
	checkf(Skill != nullptr, TEXT("USkillEntityComponent::FireLocationBullet[Skill Is Empty]"));
// 	if (Skill == nullptr)
// 		return false;

	FSkillTableInfo_V2* SkillInfo = Skill->GetSkillInfo();
	if (SkillInfo == nullptr)
		return false;

	FSkillResourceTableInfo* SkillResourceInfo = Skill->GetSkillResourceInfo();
	if (SkillResourceInfo == nullptr)
		return false;

	UClass* CheckBulletClass = nullptr;
	FBulletTableInfo* BulletTableInfo = UTableManager::GetInstancePtr()->GetBulletRow(SkillResourceInfo->BulletID);
	if (BulletTableInfo == nullptr)
		return false;

	CheckBulletClass = UUtilFunctionLibrary::GetBulletBaseFromAssetID(BulletTableInfo->BulletClass);
	if (CheckBulletClass == nullptr || !CheckBulletClass->IsChildOf(ANormalBullet::StaticClass()))
	{
		UE_LOG(LogCriticalErrors, Log, TEXT("SkillResourceInfo->BulletClass Is Not ANormalBullet Class!!! %s"), *(BulletTableInfo->BulletClass.ToSoftObjectPath().ToString()));
		return false;
	}

	auto Caster_BattleEntityComponent = UCharacterHelpFunctionLibrary::GetBattleEntityComponent(Caster);
	if (Caster_BattleEntityComponent == nullptr)
		return false;

	FVector ShotLocation = Caster_BattleEntityComponent->GetShotPointLocation();

	FActorSpawnParameters SpawnParams;
	SpawnParams.Owner = Caster;
	SpawnParams.Instigator = Caster->Instigator;
	SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;
	SpawnParams.bNoFail = true;

	ABattleBaseObject* battleObject = Cast<ABattleBaseObject>(Caster);
	if (IsValid(battleObject))
	{
		ShotLocation = battleObject->GetSocketLocation(BulletTableInfo->BulletSocket);
	}
	return true;
}

void USkillEntityComponent::SetDelayAreaForAreaEffect(float RangeRadius, UParticleSystemComponent* InTargetEffect, bool IsSkipVisible, int32 InSkipIndex)
{
	checkf(InTargetEffect != nullptr, TEXT("USkillEntityComponent::SetDelayAreaForAreaEffect[InTargetEffect Is Empty]"));
// 	if (!IsValid(InTargetEffect))
// 		return;

	InTargetEffect->AutoPopulateInstanceProperties();
	InTargetEffect->SetVectorParameter(PARAMETER_EFFECT_AREA, FVector(RangeRadius, RangeRadius, 1.0f));
	InTargetEffect->SetFloatParameter(PARAMETER_EFFECT_PARTICLE, RangeRadius);

	int32 i = 0;
	for (auto& Element : InTargetEffect->EmitterInstances)
	{
		if (Element == nullptr)
		{
			++i;
			continue;
		}

		if (IsSkipVisible)
		{
			if (i > InSkipIndex)
			{
				Element->SetHaltSpawning(IsSkipVisible);
			}
		}
		else
		{
			Element->ResetParticleParameters(0.0f);
			Element->SetHaltSpawning(IsSkipVisible);
		}

		++i;
	}
}

bool USkillEntityComponent::ApplyCastingActionApplier(AActor* Caster, AActor* Target, ESkillApplierTypeEnum& InSkillApplier, float CastingDuration)
{
	checkf(Caster != nullptr, TEXT("USkillEntityComponent::ApplyCastingActionApplier[Caster Is Empty]"));

	AStagePlayerController* StagePC = Cast<AStagePlayerController>(UUtilFunctionLibrary::GetBasePlayerController(Caster->GetWorld()));
	if (!StagePC)
		return false;

	ULeaderControlHelper* LCH = StagePC->GetLeaderControlHelper();
	if (!LCH)
		return false;

	switch (InSkillApplier)
	{

	case ESkillApplierTypeEnum::VE_i_dash:
	{
		UBattleEntityComponent* CasterBattleEntityComponent = UCharacterHelpFunctionLibrary::GetBattleEntityComponent(Caster);
		if (IsValid(CasterBattleEntityComponent))
		{
			FVector TargetLocation = FVector::ZeroVector;
			USkillEntityComponent* CasterSkillEntityComponent = UCharacterHelpFunctionLibrary::GetSkillEntityComponent(Caster);
			if (CasterSkillEntityComponent)
			{
				UISkill_V2* skill = CasterSkillEntityComponent->GetCurrentSkill_V2();
				if (skill)
				{
					TargetLocation = skill->GetTargetLocation();
					FVector TargetDir = FVector(TargetLocation.X, TargetLocation.Y, 0.f) - FVector(Caster->GetActorLocation().X, Caster->GetActorLocation().Y, 0.f);
					float Dist = TargetDir.Size();
					TargetDir = TargetDir.GetSafeNormal();

					UBattleEntityComponent* BattleEntityComponent = UCharacterHelpFunctionLibrary::GetBattleEntityComponent(Target);
					if (BattleEntityComponent)
					{
						float BoundingRadius = BattleEntityComponent->GetBoundingRadius();
						if (BoundingRadius < Dist)
							TargetLocation = Caster->GetActorLocation() + TargetDir * (Dist - BoundingRadius);
						else
							TargetLocation = Caster->GetActorLocation();
					}
				}
				else if (TargetLocation == FVector::ZeroVector)
					TargetLocation = Caster->GetActorLocation();
			}

			FVector		FloorPos;

			if (UMapFunctionLibrary::GetValidPlacableProjectionPos(Caster,TargetLocation, FloorPos, Caster) == false)
				return false;

			if (UMapFunctionLibrary::isTargetPointonNavMesh(FloorPos))
			{
				USpecialMovingComponent* spec_move_comp = UCharacterHelpFunctionLibrary::GetSpecialMovingComponent(Caster);
				if (IsValid(spec_move_comp))
				{
					float fDist = (FloorPos - Caster->GetActorLocation()).Size();

					FVector ForwardVec = Caster->GetActorForwardVector();
					ForwardVec.Z = 0;
					FVector TargetDir = FVector(FloorPos.X, FloorPos.Y, 0.f) - FVector(Caster->GetActorLocation().X, Caster->GetActorLocation().Y, 0.f);
					
					ForwardVec = ForwardVec.GetSafeNormal2D();
					TargetDir = TargetDir.GetSafeNormal2D();

					if (ForwardVec == FVector::ZeroVector || TargetDir == FVector::ZeroVector)
					{
						GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("Normalized Vector is too small!"));
						return false;
					}

					float Degree = UUtilFunctionLibrary::GetBetweenDegree(ForwardVec, TargetDir);
					FRotator rot = FRotator(0.f, Degree, 0.f);
					if (!rot.ContainsNaN())
						Caster->AddActorWorldRotation(rot);
					else
					{
						GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("rot cantainsNan"));
						return false;
					}

					spec_move_comp->StartDash(FloorPos, CastingDuration, fDist);
					return true;
				}
			}
		}
	}
	break;

	case ESkillApplierTypeEnum::VE_i_pull:
	{
		auto PullProcess = [CastingDuration](AActor* InCaster, AActor* InTarget) {
			if (IsValid(InTarget))
			{
				UBattleEntityComponent* CasterBattleEntityComponent = UCharacterHelpFunctionLibrary::GetBattleEntityComponent(InCaster);
				UBattleEntityComponent* TargetBattleEntityComponent = UCharacterHelpFunctionLibrary::GetBattleEntityComponent(InTarget);
				if (TargetBattleEntityComponent)
				{
					if (TargetBattleEntityComponent->IsInAbnormalStateApplier(ESkillApplierTypeEnum::VE_p_block_state_change))
						return;
				}

				FVector TargetDir = InTarget->GetActorLocation() - InCaster->GetActorLocation();
				TargetDir = TargetDir.GetSafeNormal();
				FVector TargetLocation = InCaster->GetActorLocation() + TargetDir * CasterBattleEntityComponent->GetBoundingRadius();

				FVector FloorPos;
				if (UMapFunctionLibrary::GetValidPlacableProjectionPos(InCaster, TargetLocation, FloorPos, InCaster) == false)
					return;

				if (UAIFunctionLibrary::CanMoveToTargetPosition(InTarget, FloorPos))
				{
					USpecialMovingComponent* spec_move_comp = UCharacterHelpFunctionLibrary::GetSpecialMovingComponent(InTarget);
					if(spec_move_comp)
						spec_move_comp->StartPull(TargetLocation, CastingDuration);
				}
			}
		};


		USkillEntityComponent* SkillEntityComponent = UCharacterHelpFunctionLibrary::GetSkillEntityComponent(Caster);
		if (SkillEntityComponent)
		{
			UISkill_V2* CurrentSkill = SkillEntityComponent->GetCurrentSkill_V2();
			CurrentSkill->GetApplyTarget();


			ESearchShapeTypeEnum Shape = CurrentSkill->GetSkillInfo()->EffectShape;
			if (Shape == ESearchShapeTypeEnum::VE_Circle)
			{
				TArray< TWeakObjectPtr<AActor> > Targets = SkillEntityComponent->GetCurrentSkill_V2()->GetApplyTargetArray();
				for (size_t i = 0; i < Targets.Num(); ++i)
				{
					if(Targets.IsValidIndex(i) && Targets[i].IsValid())
						PullProcess(Caster, Targets[i].Get());
				}
			}
			else
			{
				PullProcess(Caster, Target);
			}
		}	
	}
		break;

	case ESkillApplierTypeEnum::VE_i_blink_front:
	{
		if (IsValid(Target))
		{
			UBattleEntityComponent* CasterBattleEntityComponent = UCharacterHelpFunctionLibrary::GetBattleEntityComponent(Caster);
			UBattleEntityComponent* TargetBattleEntityComponent = UCharacterHelpFunctionLibrary::GetBattleEntityComponent(Target);
			if (IsValid(CasterBattleEntityComponent) && IsValid(TargetBattleEntityComponent))
			{
				FVector TargetDir = Caster->GetActorLocation() - Target->GetActorLocation();
				TargetDir.Normalize();
				FVector TargetLocation = Target->GetActorLocation() + TargetDir * (CasterBattleEntityComponent->GetBoundingRadius() + TargetBattleEntityComponent->GetBoundingRadius());

				FVector FloorPos;
				if (UMapFunctionLibrary::GetValidPlacableProjectionPos(Caster, TargetLocation, FloorPos, Caster) == false)
					return false;

				if (UAIFunctionLibrary::CanMoveToTargetPosition(Caster, FloorPos))
				{
					USpecialMovingComponent* spec_move_comp = UCharacterHelpFunctionLibrary::GetSpecialMovingComponent(Caster);
					if (spec_move_comp)
					{
						spec_move_comp->StartBlink(FloorPos, Caster->GetActorRotation(), CastingDuration);
					}
				}
					
				return true;
			}
		}
	}
	break;
	case ESkillApplierTypeEnum::VE_i_blink_back:
	{
		if (IsValid(Target))
		{
			UBattleEntityComponent* CasterBattleEntityComponent = UCharacterHelpFunctionLibrary::GetBattleEntityComponent(Caster);
			UBattleEntityComponent* TargetBattleEntityComponent = UCharacterHelpFunctionLibrary::GetBattleEntityComponent(Target);
			if (IsValid(CasterBattleEntityComponent) && IsValid(TargetBattleEntityComponent))
			{
				FVector TargetDir = Target->GetActorLocation() - Caster->GetActorLocation();
				TargetDir.Z = 0.0f;
				TargetDir.Normalize();
				FVector TargetLocation = Target->GetActorLocation() + TargetDir * (CasterBattleEntityComponent->GetBoundingRadius() + TargetBattleEntityComponent->GetBoundingRadius());

				FVector FloorPos;
				if (UMapFunctionLibrary::GetValidPlacableProjectionPos(Caster, TargetLocation, FloorPos, Caster) == false)
					return false;

				if (UAIFunctionLibrary::CanMoveToTargetPosition(Caster, FloorPos))
				{
					TargetDir *= -1.0f;
					FRotator TargetRot = TargetDir.Rotation();

					USpecialMovingComponent* spec_move_comp = UCharacterHelpFunctionLibrary::GetSpecialMovingComponent(Caster);
					if (spec_move_comp)
					{
						spec_move_comp->StartBlink(FloorPos, TargetRot, CastingDuration);
					}
				}
				return true;
			}
		}
	}
	break;

	case ESkillApplierTypeEnum::VE_i_blink:
	{
		UBattleEntityComponent* CasterBattleEntityComponent = UCharacterHelpFunctionLibrary::GetBattleEntityComponent(Caster);
		if (CasterBattleEntityComponent)
		{
			FVector TargetDir = Caster->GetActorLocation() - Target->GetActorLocation();
			TargetDir = TargetDir.GetSafeNormal2D();

			FVector TargetLocation = Caster->GetActorLocation() + TargetDir * CasterBattleEntityComponent->GetBoundingRadius() * 3.f;

			FVector FloorPos;
			if (UMapFunctionLibrary::GetValidPlacableProjectionPos(Caster, TargetLocation, FloorPos, Caster) == false)
				return false;

			if (UAIFunctionLibrary::CanMoveToTargetPosition(Caster, FloorPos))
			{
				TargetDir *= -1.0f;
				FRotator TargetRot = TargetDir.Rotation();

				USpecialMovingComponent* spec_move_comp = UCharacterHelpFunctionLibrary::GetSpecialMovingComponent(Caster);
				if (spec_move_comp)
				{
					spec_move_comp->StartBlink(FloorPos, TargetRot, CastingDuration);
				}
			}

			return true;
		}
	}
	break;

	}

	return false;
}

bool USkillEntityComponent::ApplyInstanceNoneTargetApplier(AActor* Caster, AActor* Target, FSkillApplierBase& InSkillApplier, UISkill_V2* AppliedSkill)
{
	checkf(Caster != nullptr, TEXT("USkillEntityComponent::ApplyInstanceNoneTargetApplier[Caster Is Empty]"));
// 	if (Caster == nullptr)
// 		return false;

	switch (InSkillApplier.SkillApplierType)
	{
	case ESkillApplierTypeEnum::VE_i_summon_1Max1:
	case ESkillApplierTypeEnum::VE_i_summon_1Max2:
	case ESkillApplierTypeEnum::VE_i_summon_1Max3:
	case ESkillApplierTypeEnum::VE_i_summon_1Max4:
	case ESkillApplierTypeEnum::VE_i_summon_1Max5:
	case ESkillApplierTypeEnum::VE_i_summon_Once2:
	case ESkillApplierTypeEnum::VE_i_summon_Once3:
	case ESkillApplierTypeEnum::VE_i_summon_Once4:
	case ESkillApplierTypeEnum::VE_i_summon_Once5:
	{
		if (AppliedSkill)
		{
			AppliedSkill->SummonMinion(InSkillApplier);
			return true;
		}
	}
	break;

	case ESkillApplierTypeEnum::VE_i_dash_forward:
	{
		UBattleEntityComponent* CasterBattleEntityComponent = UCharacterHelpFunctionLibrary::GetBattleEntityComponent(Caster);
		if (IsValid(CasterBattleEntityComponent))
		{
			FVector TargetDir = Caster->GetActorForwardVector();
			TargetDir.Normalize();
			FVector TargetLocation = Caster->GetActorLocation() + TargetDir * InSkillApplier.TargetValue;
			FVector		FloorPos;
			if (UMapFunctionLibrary::GetValidPlacableProjectionPos(Caster, TargetLocation, FloorPos, Caster) == false)
				return false;

			if (UAIFunctionLibrary::CanMoveToTargetPosition(Caster, FloorPos))
			{
				Caster->SetActorLocation(FloorPos);
				return true;
			}
		}
	}
	break;

	}

	return false;
}

bool USkillEntityComponent::ApplyInstanceApplier_Delegator(bool bNoneTargetApplier, AActor* Caster, AActor* Target, FSkillApplierBase& InSkillApplier, UISkill_V2* AppliedSkill)
{
	checkf(Caster != nullptr, TEXT("USkillEntityComponent::ApplyInstanceApplier_Delegator[Caster Is Empty]"));
// 	if (nullptr == Caster)
// 		return false;

	if (bNoneTargetApplier)
	{
		return ApplyInstanceNoneTargetApplier(Caster, Target, InSkillApplier, AppliedSkill);
	}
	else
	{
		return ApplyInstanceApplier(Caster, Target, InSkillApplier, AppliedSkill);
	}

	return false;
}

void USkillEntityComponent::ApplyInstanceApplierGroup(AActor* Caster, AActor* Target, FSkillTableInfo_V2* InSkillTableInfo, bool IsCasterSlot, UISkill_V2* AppliedSkill, bool bNoneTargetApplier)
{
	checkf(Caster != nullptr, TEXT("USkillEntityComponent::ApplyInstanceApplierGroup[Caster Is Empty]"));
	if (nullptr == Caster)
		return;
	//if (nullptr == Target)
		//return;
	ESkillApplierTypeEnum ApplierTypes[6];
	float Percents[6];
	int32 Values[6];
	if (IsCasterSlot)
	{
		ApplierTypes[0] = InSkillTableInfo->CasterEffect1;
		ApplierTypes[1] = InSkillTableInfo->CasterEffect2;
		ApplierTypes[2] = InSkillTableInfo->CasterEffect3;
		ApplierTypes[3] = InSkillTableInfo->CasterEffect4;
		ApplierTypes[4] = InSkillTableInfo->CasterEffect5;
		ApplierTypes[5] = InSkillTableInfo->CasterEffect6;

		Percents[0] = InSkillTableInfo->CasterPercent1;
		Percents[1] = InSkillTableInfo->CasterPercent2;
		Percents[2] = InSkillTableInfo->CasterPercent3;
		Percents[3] = InSkillTableInfo->CasterPercent4;
		Percents[4] = InSkillTableInfo->CasterPercent5;
		Percents[5] = InSkillTableInfo->CasterPercent6;

		Values[0] = InSkillTableInfo->CasterValue1;
		Values[1] = InSkillTableInfo->CasterValue2;
		Values[2] = InSkillTableInfo->CasterValue3;
		Values[3] = InSkillTableInfo->CasterValue4;
		Values[4] = InSkillTableInfo->CasterValue5;
		Values[5] = InSkillTableInfo->CasterValue6;
	}
	else
	{
		ApplierTypes[0] = InSkillTableInfo->TargetEffect1;
		ApplierTypes[1] = InSkillTableInfo->TargetEffect2;
		ApplierTypes[2] = InSkillTableInfo->TargetEffect3;
		ApplierTypes[3] = InSkillTableInfo->TargetEffect4;
		ApplierTypes[4] = InSkillTableInfo->TargetEffect5;
		ApplierTypes[5] = InSkillTableInfo->TargetEffect6;

		Percents[0] = InSkillTableInfo->TargetPercent1;
		Percents[1] = InSkillTableInfo->TargetPercent2;
		Percents[2] = InSkillTableInfo->TargetPercent3;
		Percents[3] = InSkillTableInfo->TargetPercent4;
		Percents[4] = InSkillTableInfo->TargetPercent5;
		Percents[5] = InSkillTableInfo->TargetPercent6;

		Values[0] = InSkillTableInfo->TargetValue1;
		Values[1] = InSkillTableInfo->TargetValue2;
		Values[2] = InSkillTableInfo->TargetValue3;
		Values[3] = InSkillTableInfo->TargetValue4;
		Values[4] = InSkillTableInfo->TargetValue5;
		Values[5] = InSkillTableInfo->TargetValue6;
	}

	FSkillApplierBase SkillApplier;
	if (!URealTimeModManager::IsRealTimeGameMode())
	{
		for (int32 n = 0; n < 6; ++n)
		{
			if (ESkillApplierTypeEnum::VE_None != ApplierTypes[n])
			{
				SkillApplier.SkillApplierType = ApplierTypes[n];
				SkillApplier.TargetPercent = Percents[n];
				SkillApplier.TargetValue = Values[n];
				if (bNoneTargetApplier)
				{
					ApplyInstanceNoneTargetApplier(Caster, Target, SkillApplier, AppliedSkill);
				}
				else
				{
					ApplyInstanceApplier(Caster, Target, SkillApplier, AppliedSkill);
				}
			}
		}
	}
	else
	{
		if (nullptr == AppliedSkill)
			return;

		FSkillSyncApplyData* SyncData = nullptr;
		if (IsCasterSlot)
		{
			SyncData = &(AppliedSkill->SkillSyncData.CasterApplyData);
		}
		else
		{
			SyncData = AppliedSkill->GetTargetSkillSyncData(Target);
		}

		if (nullptr == SyncData)
			return;

		int32 ApplierInfoIndex = -1;
		int32 ApplierCount = SyncData->Appliers.Num();
		for (int32 n = 0; n < ApplierCount; ++n)
		{
			if (!SyncData->Appliers[n].Remove)
			{
				ApplierInfoIndex = -1;
				for (int32 k = 0; k < 6; ++k)
				{
					if (ApplierTypes[k] == SyncData->Appliers[n].SlotApplier.SkillApplierType)
					{
						ApplierInfoIndex = k;
					}
				}

				if (0 <= ApplierInfoIndex)
				{
					SkillApplier.SkillApplierType = ApplierTypes[ApplierInfoIndex];
					SkillApplier.TargetPercent = Percents[ApplierInfoIndex];
					SkillApplier.TargetValue = Values[ApplierInfoIndex];

					bool isSummonSkill = FMath::IsWithin<ESkillApplierTypeEnum>(SkillApplier.SkillApplierType, ESkillApplierTypeEnum::VE_i_summon_1Max1, ESkillApplierTypeEnum::VE_i_summon_Once5);
					if (isSummonSkill)
					{
						CS_ApplyInstanceNoneTargetApplier(Caster, Target, SyncData->Appliers[n], SkillApplier, AppliedSkill);
					}
					else
					{
						CS_ApplyInstanceApplier(Caster, Target, SyncData->Appliers[n], SkillApplier, AppliedSkill);
					}
				}
			}
		}
	}
}

bool USkillEntityComponent::ApplyInstanceApplier(AActor* Caster, AActor* Target, FSkillApplierBase& InSkillApplier, UISkill_V2* AppliedSkill)
{
	checkf(Caster != nullptr, TEXT("USkillEntityComponent::ApplyInstanceApplier[Caster Is Empty]"));
	checkf(Target != nullptr, TEXT("USkillEntityComponent::ApplyInstanceApplier[Target Is Empty]"));
	checkf(AppliedSkill != nullptr, TEXT("USkillEntityComponent::ApplyInstanceApplier[AppliedSkill Is Empty]"));

	ESkillAttributeEnum		SkillAttribute = ESkillAttributeEnum::VE_None;

	switch (AppliedSkill->eSkillType)
	{
	case ESkillType::eSkillType_Normal:
	{
		FSkillTableInfo_V2* SkillInfo = AppliedSkill->GetSkillInfo();
		SkillAttribute = SkillInfo->SkillAttribute;
	}
	break;
	}

	// block push, pull ...
	if (BlockMoveingCatetoryApplierCheck(Caster, Target, InSkillApplier))
		return false;

	switch (InSkillApplier.SkillApplierType)
	{
	case ESkillApplierTypeEnum::VE_i_dispel_debuff:
	{
		USkillEntityComponent* SkillEntityComponent = UCharacterHelpFunctionLibrary::GetSkillEntityComponent(Target);
		if (SkillEntityComponent)
		{
			SkillEntityComponent->ClearSlotsByBuffType(EAbnormalApplyTypeEnum::VE_DeBuff, InSkillApplier.TargetValue);
			return true;
		}
	}
	break;
	case ESkillApplierTypeEnum::VE_i_dispel_buff:
	{
		USkillEntityComponent* SkillEntityComponent = UCharacterHelpFunctionLibrary::GetSkillEntityComponent(Target);
		if (SkillEntityComponent)
		{
			SkillEntityComponent->ClearSlotsByBuffType(EAbnormalApplyTypeEnum::VE_Buff, InSkillApplier.TargetValue);
			return true;
		}
	}
	break;

	case ESkillApplierTypeEnum::VE_t_attack_pile_up:
	{
		if (Caster == Target)
			break;
		FResourceParticleTableInfo* info = UTableManager::GetInstancePtr()->GetResourceParticleRow(FName(TEXT("FXID_AttackPileUp_Fire")));
		UParticleSystem* ps = UUtilFunctionLibrary::GetParticleSystemFromAssetID(info->Asset);
		UUtilFunctionLibrary::SpawnEmitterAttached(ps, Caster, SOCKET_ROOT);

	}
//  	break; // skip break;
	case ESkillApplierTypeEnum::VE_i_p_attack:
	case ESkillApplierTypeEnum::VE_i_m_attack:
	{
		UBattleEntityComponent* CasterBattleEntityComponent = UCharacterHelpFunctionLibrary::GetBattleEntityComponent(Caster);
		UBattleEntityComponent* TargetBattleEntityComponent = UCharacterHelpFunctionLibrary::GetBattleEntityComponent(Target);
		if (IsValid(CasterBattleEntityComponent) && IsValid(TargetBattleEntityComponent))
		{
			EAttackTypeEnum AttackType = CasterBattleEntityComponent->GetBattleInfo()->CharacterProperty.AttackType;
			if (EAttackTypeEnum::VE_Melee == AttackType)
			{
				if (TargetBattleEntityComponent->IsInAbnormalStateApplier(ESkillApplierTypeEnum::VE_p_block_melee_damage))
				{
					TargetBattleEntityComponent->OnSetBlockAbnormalMsg.Broadcast(ESkillApplierTypeEnum::VE_p_block_melee_damage);
					return false;
				}
			}

			if (EAttackTypeEnum::VE_Range == AttackType)
			{
				if (TargetBattleEntityComponent->IsInAbnormalStateApplier(ESkillApplierTypeEnum::VE_p_block_range_damage))
				{
					TargetBattleEntityComponent->OnSetBlockAbnormalMsg.Broadcast(ESkillApplierTypeEnum::VE_p_block_range_damage);
					return false;
				}
			}

			if (TargetBattleEntityComponent->IsInAbnormalStateApplier(ESkillApplierTypeEnum::VE_p_block_instant_damage) == false)
			{
				if (CasterBattleEntityComponent->GetBattleInfoBlackboard() != nullptr)
				{
					CasterBattleEntityComponent->GetBattleInfoBlackboard()->SetInstantAttackSkillApplier(InSkillApplier);
					CasterBattleEntityComponent->SetCurrentAttackTarget(Target);

					bool bPhysicalAttack = (ESkillApplierTypeEnum::VE_i_p_attack == InSkillApplier.SkillApplierType);

					CasterBattleEntityComponent->HitTarget(bPhysicalAttack, true);
					CasterBattleEntityComponent->GetBattleInfoBlackboard()->ClearInstantAttackSkillApplier();

					if (TargetBattleEntityComponent->IsDead())
					{
						UBattleBaseAnimInstance* BattleBaseAnimInstance = UCharacterHelpFunctionLibrary::GetCharacterBattleBaseAnimInstance(Target);
						if (IsValid(BattleBaseAnimInstance))
						{
							BattleBaseAnimInstance->SetIsForceDie2(true);
						}
					}
					return true;
				}
			}
			else
			{
				TargetBattleEntityComponent->OnSetBlockAbnormalMsg.Broadcast(ESkillApplierTypeEnum::VE_p_block_instant_damage);
			}
		}
	}
	break;
	case ESkillApplierTypeEnum::VE_i_hp_percent_attack:
	{
		UBattleEntityComponent* CasterBattleEntityComponent = UCharacterHelpFunctionLibrary::GetBattleEntityComponent(Caster);
		UBattleEntityComponent* TargetBattleEntityComponent = UCharacterHelpFunctionLibrary::GetBattleEntityComponent(Target);
		if (IsValid(CasterBattleEntityComponent) && IsValid(TargetBattleEntityComponent))
		{
			EAttackTypeEnum AttackType = CasterBattleEntityComponent->GetBattleInfo()->CharacterProperty.AttackType;
			if (EAttackTypeEnum::VE_Melee == AttackType)
			{
				if (TargetBattleEntityComponent->IsInAbnormalStateApplier(ESkillApplierTypeEnum::VE_p_block_melee_damage))
				{
					TargetBattleEntityComponent->OnSetBlockAbnormalMsg.Broadcast(ESkillApplierTypeEnum::VE_p_block_melee_damage);
					return false;
				}
			}

			if (EAttackTypeEnum::VE_Range == AttackType)
			{
				if (TargetBattleEntityComponent->IsInAbnormalStateApplier(ESkillApplierTypeEnum::VE_p_block_range_damage))
				{
					TargetBattleEntityComponent->OnSetBlockAbnormalMsg.Broadcast(ESkillApplierTypeEnum::VE_p_block_range_damage);
					return false;
				}
			}

			if (TargetBattleEntityComponent->IsInAbnormalStateApplier(ESkillApplierTypeEnum::VE_p_block_instant_damage) == false)
			{
				USkillEntityComponent* CasterSkillEntityComponent = UCharacterHelpFunctionLibrary::GetSkillEntityComponent(Caster);
				if (IsValid(CasterBattleEntityComponent) && CasterSkillEntityComponent &&
					CasterBattleEntityComponent->GetBattleInfoBlackboard() != nullptr)
				{
					CasterBattleEntityComponent->GetBattleInfoBlackboard()->SetInstantAttackSkillApplier(InSkillApplier);
					CasterBattleEntityComponent->SetCurrentAttackTarget(Target);

					bool bPhysicalAttack = (ESkillAttributeEnum::VE_Physical == SkillAttribute);

					CasterBattleEntityComponent->HitTarget(bPhysicalAttack, true);
					CasterBattleEntityComponent->GetBattleInfoBlackboard()->ClearInstantAttackSkillApplier();
					return true;
				}
			}
			else
			{
				TargetBattleEntityComponent->OnSetBlockAbnormalMsg.Broadcast(ESkillApplierTypeEnum::VE_p_block_instant_damage);
			}
		}
	}
	break;
	case ESkillApplierTypeEnum::VE_i_heal:
	{
		UBattleEntityComponent* CasterBattleEntityComponent = UCharacterHelpFunctionLibrary::GetBattleEntityComponent(Caster);
		USkillEntityComponent* CasterSkillEntityComponent = UCharacterHelpFunctionLibrary::GetSkillEntityComponent(Caster);
		UBattleEntityComponent* TargetBattleEntityComponent = UCharacterHelpFunctionLibrary::GetBattleEntityComponent(Target);
		if (CasterBattleEntityComponent && CasterSkillEntityComponent && TargetBattleEntityComponent)
		{
			int32 FinalValue = CasterBattleEntityComponent->GetBattleInfo()->GetCharacterFinalProperty<int32>(ECharacterPropertyTypeEnum::VE_MagicAttackPower) + static_cast<int32>(InSkillApplier.TargetPercent);
			if (TargetBattleEntityComponent->IsInAbnormalStateApplier(ESkillApplierTypeEnum::VE_p_heal_bonus))
			{
				USkillEntityComponent* TargetSkillEntityComponent = UCharacterHelpFunctionLibrary::GetSkillEntityComponent(Target);
				if (TargetSkillEntityComponent)
				{
					bool ApplyBonus = false;
					float ApplierPercent = 0.0f;
					float ApplierValue = 0.0f;
					FSkillApplierBase OutSkillApplier;
					if (TargetSkillEntityComponent->GetSkillSlotContainer()->GatherPersistentAppilerValue(ESkillApplierTypeEnum::VE_p_heal_bonus, OutSkillApplier))
					{
						ApplierPercent = OutSkillApplier.TargetPercent;
						ApplierValue = OutSkillApplier.TargetValue;
						ApplyBonus = true;
					}

					if (TargetSkillEntityComponent->GetPassiveSkillSlotContainer()->GatherPersistentAppilerValue(ESkillApplierTypeEnum::VE_p_heal_bonus, OutSkillApplier))
					{
						ApplierPercent += OutSkillApplier.TargetPercent;
						ApplierValue += OutSkillApplier.TargetValue;
						ApplyBonus = true;
					}

					if (ApplyBonus)
					{
						FinalValue = FinalValue + FinalValue * (1.0f + (ApplierPercent / 100.0f)) + ApplierValue;
					}
				}
			}
			FinalValue = FMath::Min(FinalValue, InSkillApplier.TargetValue);
			TargetBattleEntityComponent->ApplyFinalDamage(-FinalValue, Caster, EBattleDamageResultEnum::VE_Heal);
			TargetBattleEntityComponent->NotifyReceiveHeal(FinalValue, Caster);
			return true;
		}
	}
	break;

	case ESkillApplierTypeEnum::VE_i_hp_percent_heal:
	{
		USkillEntityComponent* CasterSkillEntityComponent = UCharacterHelpFunctionLibrary::GetSkillEntityComponent(Caster);
		UBattleEntityComponent* TargetBattleEntityComponent = UCharacterHelpFunctionLibrary::GetBattleEntityComponent(Target);
		if (CasterSkillEntityComponent && TargetBattleEntityComponent)
		{
			UCharacterBattleInfo* TargetBattleInfo = TargetBattleEntityComponent->GetBattleInfo();

			int32 OwnerMaxHP = TargetBattleInfo->GetCharacterFinalProperty<int32>(ECharacterPropertyTypeEnum::VE_MaxHP);
			float PercentBonus = (float)OwnerMaxHP * (InSkillApplier.TargetPercent / 100.0f);

			if (TargetBattleEntityComponent->IsInAbnormalStateApplier(ESkillApplierTypeEnum::VE_p_heal_bonus))
			{
				USkillEntityComponent* TargetSkillEntityComponent = UCharacterHelpFunctionLibrary::GetSkillEntityComponent(Target);
				if (TargetSkillEntityComponent)
				{
					bool ApplyBonus = false;
					float ApplierPercent = 0.0f;
					float ApplierValue = 0.0f;
					FSkillApplierBase OutSkillApplier;
					if (TargetSkillEntityComponent->GetSkillSlotContainer()->GatherPersistentAppilerValue(ESkillApplierTypeEnum::VE_p_heal_bonus, OutSkillApplier))
					{
						ApplierPercent = OutSkillApplier.TargetPercent;
						ApplierValue = OutSkillApplier.TargetValue;
						ApplyBonus = true;
					}
					if (TargetSkillEntityComponent->GetPassiveSkillSlotContainer()->GatherPersistentAppilerValue(ESkillApplierTypeEnum::VE_p_heal_bonus, OutSkillApplier))
					{
						ApplierPercent += OutSkillApplier.TargetPercent;
						ApplierValue += OutSkillApplier.TargetValue;
						ApplyBonus = true;
					}

					if (ApplyBonus)
					{
						PercentBonus = PercentBonus + PercentBonus * (1.0f + (ApplierPercent / 100.0f)) + ApplierValue;
					}
				}
			}

			int32 FinalValue = (int32)(PercentBonus + (PercentBonus * TargetBattleInfo->GetAvatarAbilityProperty(EAvatarAbilityType::VE_ReceivedHeal) / 100.0f));
			if (0 < InSkillApplier.TargetValue)
			{
				FinalValue = FMath::Min(FinalValue, InSkillApplier.TargetValue);
			}

			//CasterSkillEntityComponent->CheckAccumulationSkillCost(AppliedSkill);
			TargetBattleEntityComponent->ApplyFinalDamage(-FinalValue, Caster, EBattleDamageResultEnum::VE_Heal);
			TargetBattleEntityComponent->NotifyReceiveHeal(FinalValue, Caster);
			return true;
		}
	}
	break;
	
	case ESkillApplierTypeEnum::VE_i_pull_Amon:
	{
		if (IsValid(Target))
		{
			UBattleEntityComponent* CasterBattleEntityComponent = UCharacterHelpFunctionLibrary::GetBattleEntityComponent(Caster);
			UBattleEntityComponent* TargetBattleEntityComponent = UCharacterHelpFunctionLibrary::GetBattleEntityComponent(Target);
			if (IsValid(CasterBattleEntityComponent) && IsValid(TargetBattleEntityComponent))
			{
				FVector TargetLocation;
				if (AppliedSkill == nullptr)
					TargetLocation = Caster->GetActorLocation() + Caster->GetActorForwardVector() * (CasterBattleEntityComponent->GetBoundingRadius() + TargetBattleEntityComponent->GetBoundingRadius());
				else
					TargetLocation = AppliedSkill->GetTargetLocation();

				FVector FloorPos;
				if (UMapFunctionLibrary::GetValidPlacableProjectionPos(Caster, TargetLocation, FloorPos, Target) == false)
					return false;

				if (UAIFunctionLibrary::CanMoveToTargetPosition(Target, FloorPos))
				{
					//Target->SetActorLocation(FloorPos);
					ACharacter* CasterCharacter = Cast< ACharacter >(Target);
					if (CasterCharacter)
					{
						URGameInstance* RGameInstance = RGAMEINSTANCE(GEngine);
						USpecialMovingComponent* spec_move_comp = UCharacterHelpFunctionLibrary::GetSpecialMovingComponent(Target);
						if ( IsValid(spec_move_comp))
						{
							//spec_move_comp->StartKnockBack(FloorPos, InSkillApplier.TargetPercent);
						}
					}
				}
				return true;
			}
		}
	}
	break;


	case ESkillApplierTypeEnum::VE_i_push:
	{
		if (IsValid(Target))
		{
			UBattleEntityComponent* CasterBattleEntityComponent = UCharacterHelpFunctionLibrary::GetBattleEntityComponent(Caster);
			UBattleEntityComponent* TargetBattleEntityComponent = UCharacterHelpFunctionLibrary::GetBattleEntityComponent(Target);
			if (IsValid(CasterBattleEntityComponent) && IsValid(TargetBattleEntityComponent))
			{
				auto TargetLocation = Target->GetActorLocation();
				auto CasterLocation = Caster->GetActorLocation();

				FVector TargetDir = Target->GetActorLocation() - Caster->GetActorLocation();
				TargetDir.Normalize();

				FVector PushLoc = TargetLocation + (TargetDir * InSkillApplier.TargetValue);
				if (UAIFunctionLibrary::CanMoveToTargetPosition(Target, PushLoc))
				{
					ACharacter* CasterCharacter = Cast< ACharacter >(Target);
					if (CasterCharacter)
					{
						URGameInstance* RGameInstance = RGAMEINSTANCE(GEngine);
						USpecialMovingComponent* spec_move_comp = UCharacterHelpFunctionLibrary::GetSpecialMovingComponent(Target);
						if ( IsValid(spec_move_comp))
						{
							//spec_move_comp->StartKnockBack(PushLoc, InSkillApplier.TargetPercent, InSkillApplier.TargetValue);
						}
						//CasterCharacter->LaunchCharacter(TargetDir * 5000.0f, true, true );
					}

					TargetBattleEntityComponent->SetAbnormalState(EAbonormalStateFlagEnum::BM_KnockBack, true);
				}
				return true;
			}
		}
	}
	break;

	
	case ESkillApplierTypeEnum::VE_i_switch:
	{
		if (IsValid(Target))
		{
			UBattleEntityComponent* CasterBattleEntityComponent = UCharacterHelpFunctionLibrary::GetBattleEntityComponent(Caster);
			UBattleEntityComponent* TargetBattleEntityComponent = UCharacterHelpFunctionLibrary::GetBattleEntityComponent(Target);
			if (IsValid(CasterBattleEntityComponent) && IsValid(TargetBattleEntityComponent))
			{
				FVector CasterFloorPos;
				FVector TargetFloorPos;

				FVector CasterLocation = Target->GetActorLocation();
				if (UMapFunctionLibrary::GetValidPlacableProjectionPos(Caster, CasterLocation, CasterFloorPos, Caster) == false)
					return false;

				FVector TargetLocation = Caster->GetActorLocation();
				if (UMapFunctionLibrary::GetValidPlacableProjectionPos(Caster, TargetLocation, TargetFloorPos, Target) == false)
					return false;

				if (UAIFunctionLibrary::CanMoveToTargetPosition(Caster, CasterFloorPos))
					Caster->SetActorLocation(CasterFloorPos);

				if (UAIFunctionLibrary::CanMoveToTargetPosition(Target, TargetFloorPos))
					Target->SetActorLocation(TargetFloorPos);

				return true;
			}
		}
	}
	break;


	case ESkillApplierTypeEnum::VE_i_recall_friend:
		// recall all party members..
		break;

	
	case ESkillApplierTypeEnum::VE_i_cooltime_bonus:
	{
		UBattleEntityComponent* TargetBattleEntityComponent = UCharacterHelpFunctionLibrary::GetBattleEntityComponent(Target);
		if (TargetBattleEntityComponent)
		{
			if (TargetBattleEntityComponent->IsInAbnormalStateApplier(ESkillApplierTypeEnum::VE_p_block_cooltime_bonus))
			{
				return false;
			}
		}

		USkillEntityComponent* TargetSkillEntityComponent = UCharacterHelpFunctionLibrary::GetSkillEntityComponent(Target);
		if (TargetSkillEntityComponent)
		{
			TargetSkillEntityComponent->DecreaseCoolTime(InSkillApplier.TargetPercent, InSkillApplier.TargetValue);
			return true;
		}
	}
	break;

	case ESkillApplierTypeEnum::VE_i_p_split_attack:
	{
		UBattleEntityComponent* CasterBattleEntityComponent = UCharacterHelpFunctionLibrary::GetBattleEntityComponent(Caster);
		UBattleEntityComponent* TargetBattleEntityComponent = UCharacterHelpFunctionLibrary::GetBattleEntityComponent(Target);
		if (TargetBattleEntityComponent->IsDead())
			return false;

		auto CurrentAttackTarget = CasterBattleEntityComponent->GetCurrentAttackTarget();
		if (IsValid(CurrentAttackTarget) && CurrentAttackTarget == Target)
		{
			return false;
		}

		if (IsValid(CasterBattleEntityComponent) && IsValid(TargetBattleEntityComponent))
		{
			EAttackTypeEnum AttackType = CasterBattleEntityComponent->GetBattleInfo()->CharacterProperty.AttackType;
			if (EAttackTypeEnum::VE_Melee == AttackType)
			{
				if (TargetBattleEntityComponent->IsInAbnormalStateApplier(ESkillApplierTypeEnum::VE_p_block_melee_damage))
				{
					TargetBattleEntityComponent->OnSetBlockAbnormalMsg.Broadcast(ESkillApplierTypeEnum::VE_p_block_melee_damage);
					return false;
				}
			}

			if (EAttackTypeEnum::VE_Range == AttackType)
			{
				if (TargetBattleEntityComponent->IsInAbnormalStateApplier(ESkillApplierTypeEnum::VE_p_block_range_damage))
				{
					TargetBattleEntityComponent->OnSetBlockAbnormalMsg.Broadcast(ESkillApplierTypeEnum::VE_p_block_range_damage);
					return false;
				}
			}

			if (!TargetBattleEntityComponent->IsInAbnormalStateApplier(ESkillApplierTypeEnum::VE_p_block_instant_damage))
			{
				int32 CasterPhysicalAttackPower = CasterBattleEntityComponent->GetBattleInfo()->GetCharacterFinalProperty<int32>(ECharacterPropertyTypeEnum::VE_PhysicalAttackPower);
				int32 PercentBonus = (float)CasterPhysicalAttackPower * (1.0f + (InSkillApplier.TargetPercent / 100.0f));
				int32 ValueBonus = InSkillApplier.TargetValue;
				int32 FinalValue = PercentBonus + ValueBonus;

				auto BattleOperator = TargetBattleEntityComponent->GetBattleOperator();
				if (BattleOperator)
				{
					FinalValue = BattleOperator->CheckProtectState(FinalValue, Caster);
				}

				EBattleDamageResultEnum DmgResult = EBattleDamageResultEnum::VE_Miss;
				if (0 != FinalValue)
				{
					DmgResult = EBattleDamageResultEnum::VE_Hit;
				}
				
				TargetBattleEntityComponent->ApplyFinalDamage(FinalValue, Caster, DmgResult);
				TargetBattleEntityComponent->OnNotifyDamageDelegate.Broadcast(Caster, Target, FinalValue, true);

				return true;
			}
			else
			{
				TargetBattleEntityComponent->OnSetBlockAbnormalMsg.Broadcast(ESkillApplierTypeEnum::VE_p_block_instant_damage);
			}
		}
	}
	break;
	case ESkillApplierTypeEnum::VE_i_despawn:
	{
		IEntityBaseProperty* BaseProperty = Cast<IEntityBaseProperty>(Target);
		if (BaseProperty->GetCharacterCategory() == ECharacterCategoryTypeEnum::VE_Minion)
		{
			UBattleEntityComponent* TargetBattleEntityComponent = UCharacterHelpFunctionLibrary::GetBattleEntityComponent(Target);
			if (TargetBattleEntityComponent)
			{
				auto SummonOwner = TargetBattleEntityComponent->GetSummonOwner();
				UBattleEntityComponent* CasterBattleEntityComponent = UCharacterHelpFunctionLibrary::GetBattleEntityComponent(SummonOwner);
				if (SummonOwner && CasterBattleEntityComponent)
				{
					{
						TargetBattleEntityComponent->GetBattleInfo()->SetHP(0);
						TargetBattleEntityComponent->ForceDead(Caster);

						return true;
					}
				}
			}
		}
	}
	break;
	}

	return false;
}
bool USkillEntityComponent::CS_ApplyInstanceApplier(AActor* Caster, AActor* Target, FSkillSyncSlotApplierData& SyncApplierData, FSkillApplierBase& InSkillApplier, UISkill_V2* AppliedSkill)
{
	checkf(Caster != nullptr, TEXT("USkillEntityComponent::CS_ApplyInstanceApplier[Caster Is Empty]"));
	checkf(Target != nullptr, TEXT("USkillEntityComponent::CS_ApplyInstanceApplier[Target Is Empty]"));
	checkf(AppliedSkill != nullptr, TEXT("USkillEntityComponent::CS_ApplyInstanceApplier[AppliedSkill Is Empty]"));

	if (!AppliedSkill->SkillSyncData.IsValid())
		return false;

	switch (InSkillApplier.SkillApplierType)
	{
	case ESkillApplierTypeEnum::VE_i_dispel_debuff:
	{
		USkillEntityComponent* SkillEntityComponent = UCharacterHelpFunctionLibrary::GetSkillEntityComponent(Target);
		if (SkillEntityComponent)
		{
			SkillEntityComponent->ClearSlotsByBuffType(EAbnormalApplyTypeEnum::VE_DeBuff, InSkillApplier.TargetValue);
			return true;
		}
	}
	break;
	case ESkillApplierTypeEnum::VE_i_dispel_buff:
	{
		USkillEntityComponent* SkillEntityComponent = UCharacterHelpFunctionLibrary::GetSkillEntityComponent(Target);
		if (SkillEntityComponent)
		{
			SkillEntityComponent->ClearSlotsByBuffType(EAbnormalApplyTypeEnum::VE_Buff, InSkillApplier.TargetValue);
			return true;
		}
	}
	break;

	case ESkillApplierTypeEnum::VE_i_p_attack:
	case ESkillApplierTypeEnum::VE_i_m_attack:
	case ESkillApplierTypeEnum::VE_i_hp_percent_attack:
	{
		UBattleEntityComponent* CasterBattleEntityComponent = UCharacterHelpFunctionLibrary::GetBattleEntityComponent(Caster);
		UBattleEntityComponent* TargetBattleEntityComponent = UCharacterHelpFunctionLibrary::GetBattleEntityComponent(Target);
		if (IsValid(CasterBattleEntityComponent) && IsValid(TargetBattleEntityComponent))
		{
			EAttackTypeEnum AttackType = CasterBattleEntityComponent->GetBattleInfo()->CharacterProperty.AttackType;
			if (EAttackTypeEnum::VE_Melee == AttackType)
			{
				if (TargetBattleEntityComponent->IsInAbnormalStateApplier(ESkillApplierTypeEnum::VE_p_block_melee_damage))
				{
					TargetBattleEntityComponent->OnSetBlockAbnormalMsg.Broadcast(ESkillApplierTypeEnum::VE_p_block_melee_damage);
					return false;
				}
			}

			if (EAttackTypeEnum::VE_Range == AttackType)
			{
				if (TargetBattleEntityComponent->IsInAbnormalStateApplier(ESkillApplierTypeEnum::VE_p_block_range_damage))
				{
					TargetBattleEntityComponent->OnSetBlockAbnormalMsg.Broadcast(ESkillApplierTypeEnum::VE_p_block_range_damage);
					return false;
				}
			}

			if (TargetBattleEntityComponent->IsInAbnormalStateApplier(ESkillApplierTypeEnum::VE_p_block_instant_damage) == false)
			{
				if (CasterBattleEntityComponent->GetBattleInfoBlackboard() != nullptr)
				{
					CasterBattleEntityComponent->GetBattleInfoBlackboard()->SetInstantAttackSkillApplier(InSkillApplier);
					CasterBattleEntityComponent->SetCurrentAttackTarget(Target);

					if (EBattleDamageResultEnum::VE_None != SyncApplierData.DamageResult)
					{
						FCSNormalAttackSyncData AttackData;
						AttackData.Target = Target;
						AttackData.HitResult = SyncApplierData.DamageResult;
						AttackData.HitDamage = SyncApplierData.Damage;
						AttackData.SyncHP = SyncApplierData.SyncHP;

						TargetBattleEntityComponent->CSOnDamage(Caster, AttackData);
						TargetBattleEntityComponent->OnNotifyDamageDelegate.Broadcast(Caster, Target, (int32)SyncApplierData.Damage, true);

						CasterBattleEntityComponent->OnHitDelegate.Broadcast(Caster, AttackData.HitResult, Target, false);
					}
					return true;
				}
			}
			else
			{
				TargetBattleEntityComponent->OnSetBlockAbnormalMsg.Broadcast(ESkillApplierTypeEnum::VE_p_block_instant_damage);
			}
		}
	}
	break;
	case ESkillApplierTypeEnum::VE_i_heal:
	case ESkillApplierTypeEnum::VE_i_hp_percent_heal:
	{
		UBattleEntityComponent* TargetBattleEntityComponent = UCharacterHelpFunctionLibrary::GetBattleEntityComponent(Target);
		USkillEntityComponent* CasterSkillEntityComponent = UCharacterHelpFunctionLibrary::GetSkillEntityComponent(Caster);
		if (TargetBattleEntityComponent && CasterSkillEntityComponent)
		{
			if (EBattleDamageResultEnum::VE_None != SyncApplierData.DamageResult)
			{
				FCSNormalAttackSyncData ChangedHPData;
				ChangedHPData.Target = Target;
				ChangedHPData.HitResult = SyncApplierData.DamageResult;
				ChangedHPData.HitDamage = SyncApplierData.Damage;
				ChangedHPData.SyncHP = SyncApplierData.SyncHP;
					
				TargetBattleEntityComponent->CSOnDamage(Caster, ChangedHPData);
				TargetBattleEntityComponent->NotifyReceiveHeal(SyncApplierData.Damage, Caster);
			}
			return true;
		}
	}
	break;


	/*case ESkillApplierTypeEnum::VE_i_blink_front:
	{
		UBattleEntityComponent* CasterBattleEntityComponent = UCharacterHelpFunctionLibrary::GetBattleEntityComponent(Caster);
		if (IsValid(CasterBattleEntityComponent))
		{
			FVector TargetLocation = SyncApplierData.TargetLoc;
			FVector FloorPos;
			if (UMapFunctionLibrary::GetValidPlacableProjectionPos(Caster, TargetLocation, FloorPos, Caster))
			{
				Caster->SetActorLocation(FloorPos, false, nullptr, ETeleportType::TeleportPhysics);
				return true;
			}
		}
	}
	break;

	case ESkillApplierTypeEnum::VE_i_blink_back:
	{
		UBattleEntityComponent* CasterBattleEntityComponent = UCharacterHelpFunctionLibrary::GetBattleEntityComponent(Caster);
		if (IsValid(CasterBattleEntityComponent))
		{
			FVector TargetLocation = SyncApplierData.TargetLoc;
			FVector FloorPos;
			if (UMapFunctionLibrary::GetValidPlacableProjectionPos(Caster, TargetLocation, FloorPos, Caster))
			{
				Caster->SetActorLocation(FloorPos, false, nullptr, ETeleportType::TeleportPhysics);
				return true;
			}
		}
	}
	break;*/

	case ESkillApplierTypeEnum::VE_i_pull_Amon:
	{
		if (IsValid(Target))
		{
			FVector TargetLocation = SyncApplierData.TargetLoc;
			FVector FloorPos;
			if (UMapFunctionLibrary::GetValidPlacableProjectionPos(Caster, TargetLocation, FloorPos, Target))
			{
				URGameInstance* RGameInstance = RGAMEINSTANCE(GEngine);
				USpecialMovingComponent* spec_move_comp = UCharacterHelpFunctionLibrary::GetSpecialMovingComponent(Target);
				if ( IsValid(spec_move_comp))
				{
					//spec_move_comp->StartKnockBack(FloorPos, InSkillApplier.TargetPercent);
					return true;
				}
			}
		}
	}
	break;


	case ESkillApplierTypeEnum::VE_i_push:
	{
		if (IsValid(Target))
		{
			FVector TargetLocation = SyncApplierData.TargetLoc;
			FVector FloorPos;
			if (UMapFunctionLibrary::GetValidPlacableProjectionPos(Caster, TargetLocation, FloorPos, Target))
			{
				URGameInstance* RGameInstance = RGAMEINSTANCE(GEngine);
				USpecialMovingComponent* spec_move_comp = UCharacterHelpFunctionLibrary::GetSpecialMovingComponent(Target);
				if ( IsValid(spec_move_comp))
				{
					//spec_move_comp->StartKnockBack(FloorPos, InSkillApplier.TargetPercent, InSkillApplier.TargetValue);
					return true;
				}
			}
		}
	}
	break;

	case ESkillApplierTypeEnum::VE_i_recall_friend:
		// recall all party members..
		break;


	case ESkillApplierTypeEnum::VE_i_cooltime_bonus:
	{
		USkillEntityComponent* TargetSkillEntityComponent = UCharacterHelpFunctionLibrary::GetSkillEntityComponent(Target);
		if (TargetSkillEntityComponent)
		{
			TargetSkillEntityComponent->DecreaseCoolTime(InSkillApplier.TargetPercent, InSkillApplier.TargetValue);
			return true;
		}
	}
	break;

	case ESkillApplierTypeEnum::VE_i_p_split_attack:
	{
		UBattleEntityComponent* CasterBattleEntityComponent = UCharacterHelpFunctionLibrary::GetBattleEntityComponent(Caster);
		UBattleEntityComponent* TargetBattleEntityComponent = UCharacterHelpFunctionLibrary::GetBattleEntityComponent(Target);
		if (IsValid(CasterBattleEntityComponent) && IsValid(TargetBattleEntityComponent))
		{
			EAttackTypeEnum AttackType = CasterBattleEntityComponent->GetBattleInfo()->CharacterProperty.AttackType;
			if (EAttackTypeEnum::VE_Melee == AttackType)
			{
				if (TargetBattleEntityComponent->IsInAbnormalStateApplier(ESkillApplierTypeEnum::VE_p_block_melee_damage))
				{
					TargetBattleEntityComponent->OnSetBlockAbnormalMsg.Broadcast(ESkillApplierTypeEnum::VE_p_block_melee_damage);
					return false;
				}
			}

			if (EAttackTypeEnum::VE_Range == AttackType)
			{
				if (TargetBattleEntityComponent->IsInAbnormalStateApplier(ESkillApplierTypeEnum::VE_p_block_range_damage))
				{
					TargetBattleEntityComponent->OnSetBlockAbnormalMsg.Broadcast(ESkillApplierTypeEnum::VE_p_block_range_damage);
					return false;
				}
			}

			if (TargetBattleEntityComponent->IsInAbnormalStateApplier(ESkillApplierTypeEnum::VE_p_block_instant_damage) == false)
			{
				if (EBattleDamageResultEnum::VE_None != SyncApplierData.DamageResult)
				{
					FCSNormalAttackSyncData AttackData;
					AttackData.Target = Target;
					AttackData.HitResult = SyncApplierData.DamageResult;
					AttackData.HitDamage = SyncApplierData.Damage;
					AttackData.SyncHP = SyncApplierData.SyncHP;

					//CasterSkillEntityComponent->CheckAccumulationSkillCost(AppliedSkill);
					TargetBattleEntityComponent->CSOnDamage(Caster, AttackData);
					TargetBattleEntityComponent->OnNotifyDamageDelegate.Broadcast(Caster, Target, SyncApplierData.Damage, true);
				}
				return true;
			}
			else
			{
				TargetBattleEntityComponent->OnSetBlockAbnormalMsg.Broadcast(ESkillApplierTypeEnum::VE_p_block_instant_damage);
			}
		}
	}
	break;

	case ESkillApplierTypeEnum::VE_i_despawn:
	{
		
	}
	break;


	}

	return false;
}

bool USkillEntityComponent::CS_ApplyInstanceNoneTargetApplier(AActor* Caster, AActor* Target, FSkillSyncSlotApplierData& SyncApplierData, FSkillApplierBase& InSkillApplier, UISkill_V2* AppliedSkill)
{
	if (Caster == nullptr)
		return false;

	switch (InSkillApplier.SkillApplierType)
	{
	case ESkillApplierTypeEnum::VE_i_summon_1Max1:
	case ESkillApplierTypeEnum::VE_i_summon_1Max2:
	case ESkillApplierTypeEnum::VE_i_summon_1Max3:
	case ESkillApplierTypeEnum::VE_i_summon_1Max4:
	case ESkillApplierTypeEnum::VE_i_summon_1Max5:
	case ESkillApplierTypeEnum::VE_i_summon_Once2:
	case ESkillApplierTypeEnum::VE_i_summon_Once3:
	case ESkillApplierTypeEnum::VE_i_summon_Once4:
	case ESkillApplierTypeEnum::VE_i_summon_Once5:
	{
		return true;
	}
	break;

	case ESkillApplierTypeEnum::VE_i_dash_forward:
	{
		FVector TargetLocation = SyncApplierData.TargetLoc;
		FVector FloorPos;
		if (UMapFunctionLibrary::GetValidPlacableProjectionPos(Caster, TargetLocation, FloorPos, Caster))
		{
			Caster->SetActorLocation(FloorPos, false, nullptr, ETeleportType::TeleportPhysics);
			return true;
		}
	}
	break;

	}

	return false;
}

bool USkillEntityComponent::GetSynergySkillTarget(AActor* InOwnerActor, TArray< TWeakObjectPtr<AActor> >& OutTargets, FName InSkillTableID, FSkillTableInfo_V2* InSkillTableInfo)
{
	FSkillTableInfo_V2* SkillTableInfo = InSkillTableInfo;
	if (SkillTableInfo == nullptr)
	{
		SkillTableInfo = URenewal_BattleFunctionLibrary::GetSkillTableInfo_V2(ECharacterCategoryTypeEnum::VE_Hero, InSkillTableID);
		if (SkillTableInfo == nullptr)
			return false;
	}

	UBattleEntityComponent* BattleEntityComponent = UCharacterHelpFunctionLibrary::GetBattleEntityComponent(InOwnerActor);
	if (BattleEntityComponent)
	{
		float SightRange = URenewal_BattleFunctionLibrary::GetCustomizedSightRange(InOwnerActor->GetWorld(), BattleEntityComponent->GetBattleInfo());
		TArray< TWeakObjectPtr<AActor> > TempTargets;
		URenewal_BattleFunctionLibrary::FindTarget(TempTargets, InOwnerActor, InOwnerActor->GetActorLocation(), EFriendshipTypeEnum::VE_Enemy, ESearchShapeTypeEnum::VE_Circle, FVector(SightRange, 0, 0));

		for (auto& searchedTarget : TempTargets)
		{
			if (IsSkillSynergyTarget(SkillTableInfo, searchedTarget.Get()))
				OutTargets.Add(searchedTarget);
		}

		if (OutTargets.Num() > 0)
			return true;
	}

	return false;
}

bool USkillEntityComponent::IsSkillSynergyTarget(FSkillTableInfo_V2* InSkillTableInfo, AActor* InTargetActor)
{
	return false;
}


