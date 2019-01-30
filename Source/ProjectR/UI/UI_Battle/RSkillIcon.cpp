// Fill out your copyright notice in the Description page of Project Settings.

#include "ProjectR.h"
#include "RSkillIcon.h"

#include "GlobalIntegrated.h"

#include "GameInfo/PlayStateInfo.h"
#include "Skill_V2/SharedSkill/ISkill_V2.h"

#include "Table/SkillResourceTableInfo.h"

#include "UtilFunctionIntegrated.h"

#include "Character/Interfaces/EntityBaseProperty.h"
#include "Character/Components/FunctionalComponents/BattleEntityComponent.h"
#include "Character/Components/FunctionalComponents/SkillEntityComponent.h"
#include "ContentGameMode/Controllers/BasePlayerController.h"
#include "ContentGameMode/Controllers/StagePlayerController.h"
#include "CustomStructures/HeroSlotInfo.h"

#include "UI/UI_Battle/UC_SkillIndicator.h"
#include "UI/UI_Battle/Component/RBuffIcon.h"
#include "Blueprint/WidgetLayoutLibrary.h"
#include "Network/RTSManager.h"
#include "LeaderControlHelper.h"
#include "DirectionTrackManager.h"

void URSkillIcon::NativeConstruct()
{
	Super::NativeConstruct();

	if (Button_Click)
	{
// 		if( URTSManager::GetInstancePtr()->IsConnectedMode(EPVP::BG)
// 			|| URTSManager::GetInstancePtr()->IsConnectedMode(EPVP::BF)
// 			|| URTSManager::GetInstancePtr()->IsConnectedMode(EPVP::MR) 
// 			|| URTSManager::GetInstancePtr()->IsConnectedMode(EPVP::SR) 
// 			|| URTSManager::GetInstancePtr()->IsConnectedMode(EPVP::GR))
// 		{
// 			this->SetVisibility(ESlateVisibility::Collapsed);
// 			return;
// 		}

		this->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
// 		Button_Click->SetVisibility(ESlateVisibility::Visible);
		if (IsCrewChangeIcon == false)
		{
			Button_Click->OnPressed.RemoveDynamic(this, &URSkillIcon::PressIcon);
			Button_Click->OnPressed.AddDynamic(this, &URSkillIcon::PressIcon);
			Button_Click->OnReleased.RemoveDynamic(this, &URSkillIcon::ReleaseIcon);
			Button_Click->OnReleased.AddDynamic(this, &URSkillIcon::ReleaseIcon);
		}
		else
		{
			Button_Click->OnReleased.RemoveDynamic(this, &URSkillIcon::ReleaseIcon);
			Button_Click->OnReleased.AddDynamic(this, &URSkillIcon::ReleaseIcon);
		}

		
	}

	RGameInstance = RGAMEINSTANCE(this);
	if ( RGameInstance->EventManager)
	{
		RGameInstance->EventManager->OnDeadActor.AddDynamic(this, &URSkillIcon::OnDeadActor);
	}

	//bryan works
	UWidget* PortraitWidget = WidgetTree->FindWidget(TEXT("Portrait"));
	PortraitOverlay = Cast<UOverlay>(PortraitWidget);

	if (IsCrewChangeIcon)
	{
		this->SetVisibility(ESlateVisibility::HitTestInvisible);
		//Button_Click->SetVisibility(ESlateVisibility::Visible);
// 		PortraitOverlay->SetVisibility(ESlateVisibility::HitTestInvisible);
	}
// 	else
// 		PortraitOverlay->SetVisibility(ESlateVisibility::Hidden);
	//

	/*if (SkillBuffIcon)
	{
		auto StartCanvasSlot = UWidgetLayoutLibrary::SlotAsCanvasSlot(SkillBuffIcon);
		StartCanvasSlot->GetSize()
		SkillBuffIcon->Image_Icon-();
	}*/
}

void URSkillIcon::NativeDestruct()
{
	if (RGameInstance && RGameInstance->EventManager)
	{
		RGameInstance->EventManager->OnDeadActor.RemoveDynamic(this, &URSkillIcon::OnDeadActor);
		RGameInstance->EventManager->OnSynergyEnter.RemoveDynamic(this, &URSkillIcon::CheckSynergy);
	}

	Super::NativeDestruct();
}

bool URSkillIcon::IsCoolTime()
{
	return (0.0f < CoolTime);
}

void URSkillIcon::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);

	if (SkillIconState == ESkillIconState::VE_Locked)
		return;

	if (SkillIconState == ESkillIconState::VE_InCrewCoolTime)
	{
		auto SkillEntityComponent = UCharacterHelpFunctionLibrary::GetSkillEntityComponent(PrevCharacter.Get());
		if (IsValid(SkillEntityComponent))
		{
			//UISkill_V2* Skill = SkillEntityComponent->GetSkill_V2(PrevSkillID);
			CoolTime = SkillEntityComponent->GetCrewSkillCoolTime_V2(PrevSkillID);
			if (CoolTime > 0)
			{
				if (CoolTime > MaxCoolTime)
				{
					MaxCoolTime = CoolTime;
				}
				UpdateCooling();
			}
			else// if (CoolTime <= 0)
			{
				SetSkillIconState(ESkillIconState::VE_Normal);
				EndCrewSkillCooling();
				InvalidateOwnerCharacter();
			}
		}
	}
	else if (SkillIconState == ESkillIconState::VE_InSpiritCoolTime)
	{
		//USkillEntityComponent* SkillEntityComponent = UCharacterHelpFunctionLibrary::GetSkillEntityComponent(PrevCharacter.Get());

		if (CoolTime > 0)
		{
			if (CoolTime > MaxCoolTime)
			{
				MaxCoolTime = CoolTime;
			}

			CoolTime -= InDeltaTime;

			UpdateCooling();
		}
		else// if (CoolTime <= 0)
		{
			SetSkillIconState(ESkillIconState::VE_Normal);
			EndCrewSkillCooling();
			//InvalidateOwnerCharacter();
		}
	}
	if (IsCrewChangeIcon)
		return;

	if (OwnerCharacter.IsValid())
	{
		//Check CoolTime
		auto OwnerCharacter_SkillEntityComponent = UCharacterHelpFunctionLibrary::GetSkillEntityComponent(OwnerCharacter.Get());
		if (IsValid(OwnerCharacter_SkillEntityComponent))
		{
			float TempCompare = OwnerCharacter_SkillEntityComponent->GetSkillCoolTime_V2(CurrentSkillID);
			if (TempCompare > 0)
			{
				if (SkillIconState != ESkillIconState::VE_InCoolTime)
				{
					SetSkillIconState(ESkillIconState::VE_InCoolTime);
				}
				CoolTime = TempCompare;
			}

			if (CoolTime > 0)
			{
				if (CoolTime > MaxCoolTime)
				{
					MaxCoolTime = CoolTime;
				}

				CoolTime = OwnerCharacter_SkillEntityComponent->GetSkillCoolTime_V2(CurrentSkillID);
				UpdateCooling();
			}
			else if (CoolTime == 0)
			{
				SetSkillIconState(ESkillIconState::VE_Normal);
				EndCooling();
				
			}
			else if (INVALID_INDEXNUM >= CoolTime)
			{
				float current_cool = OwnerCharacter_SkillEntityComponent->GetSkillCoolTime_V2(CurrentSkillID);
				if (0.0f < current_cool)
				{
					SetSkillIconState(ESkillIconState::VE_InCoolTime);
					CoolTime = OwnerCharacter_SkillEntityComponent->GetSkillCoolTime_V2(CurrentSkillID);
				}
			}

			if (SkillCategoryType == ESkillCategoryTypeEnum::VE_ActiveSkill03)
			{
				if (Image_SkillPoint)
				{
					//float CurrentCostValue = OwnerCharacter_SkillEntityComponent->GetSkillCostPercentage(CurrentSkillID);
					//if (CurrentCostValue != -1 && CheckCost != CurrentCostValue)
					//{
					//	CheckCost = CurrentCostValue;
					//	UpdateSkillUseCost();
					//}
					if (0.0f < MaxCoolTime)
					{
						CheckCost = 1.0f - (CoolTime / MaxCoolTime);
						UpdateSkillUseCost();
					}
				}
			}
		}

		//Check Abnormal

		
		if (IsCrewChangeIcon)
			return;

		if (IsTouch)// && CoolTime <= 0)
		{
			CheckTouchTime += InDeltaTime;

			if(CheckTouchTime > 2)
			{
				if(IsTouchUpdate == false)
					return;

				if ( IsValid(RGameInstance->EventManager))
				{
					RGameInstance->EventManager->OnIndicator.Broadcast(CurrentSkillID);
				}
			}
		}
		else
		{
			if(CheckTouchTime != 0)
			{
				CheckTouchTime = 0;
				IsTouchUpdate = false;

				if ( IsValid(RGameInstance->EventManager))
				{
					RGameInstance->EventManager->OnIndicator.Broadcast(TEXT("None"));
				}
			}
		}
	}
}

void URSkillIcon::CheckSynergy(bool InSet)
{
	if (Image_Synergy == nullptr)
		return;

	if (SkillIconState == ESkillIconState::VE_Locked)
		return;
	////return friend
	bool bIsSynergy = false;
	if (IsComputeSynergyMesh)
	{
		TArray< TWeakObjectPtr< AActor > > outEnemyList;
		// todo : get object list

		UUtilFunctionLibrary::ComputeSynergy(outEnemyList, OwnerCharacter.Get(), SkillTableID);
	}
	//TArray< TWeakObjectPtr<AActor> > OutTargets;
	//bIsSynergy = USkillManager_V2::GetSynergySkillTarget(OwnerCharacter.Get(), OutTargets, SkillTableID);

	
	auto OwnerActor_SkillEntityComponent = UCharacterHelpFunctionLibrary::GetSkillEntityComponent(OwnerCharacter.Get());
	if (IsValid(OwnerActor_SkillEntityComponent))
	{
		UISkill_V2* TempSkill = OwnerActor_SkillEntityComponent->GetSkillByCategory_V2(SkillCategoryType);
		if (IsValid(TempSkill))
		{
			FSkillTableInfo_V2* SkillInfo = TempSkill->GetSkillInfo();

			auto BattleEntityComponent = UCharacterHelpFunctionLibrary::GetBattleEntityComponent(OwnerCharacter.Get());
			if (BattleEntityComponent)
			{
				auto CurrentTarget = BattleEntityComponent->GetCurrentAttackTarget();
				bIsSynergy = OwnerActor_SkillEntityComponent->IsSkillSynergyTarget(SkillInfo, CurrentTarget);
			}
		}
		//if (bIsSynergy)
		//{
		//	bIsSynergy = OwnerActor_SkillEntityComponent->IsEnoughSkillCostToCastSkill(TempSkill->GetSkillUseCost());
		//}
	}

	if (bIsSynergy)
	{
		Image_Synergy->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
	}
	else
	{
		Image_Synergy->SetVisibility(ESlateVisibility::Collapsed);
	}
}

void URSkillIcon::Init(AActor* InOwnerCharacter, ESkillCategoryTypeEnum InSkillCategoryType)
{
	EndCooling();

	if (IsValid(InOwnerCharacter))
	{
		OwnerCharacter = InOwnerCharacter;
		SkillCategoryType = InSkillCategoryType;
		
		if (IsTouchUpdate)
		{
			IsTouchUpdate = false;

			if ( IsValid(RGameInstance->EventManager))
			{
				RGameInstance->EventManager->OnIndicator.Broadcast(TEXT("None"));
			}
		}

		auto OwnerActor_SkillEntityComponent	= UCharacterHelpFunctionLibrary::GetSkillEntityComponent(OwnerCharacter.Get());
		if (IsValid(OwnerActor_SkillEntityComponent))
		{
			IEntityBaseProperty* EntityProperty = Cast< IEntityBaseProperty >(OwnerCharacter.Get());
			if (EntityProperty)
			{
				CharacterID = EntityProperty->GetCharacterID();
			}

			if (false == OnlyDisplay && SkillCategoryType == ESkillCategoryTypeEnum::VE_ActiveSkill03)
			{
				if (Switcher_BGImage)
					Switcher_BGImage->SetActiveWidgetIndex(1);

				//CheckCost = OwnerActor_SkillEntityComponent->GetSkillCostPercentage(SkillID);
				//if (CheckCost == -1)
				//	CheckCost = 0;

				if (0.0f < MaxCoolTime)
				{
					CheckCost = 1.0f - (CoolTime / MaxCoolTime);
				}
				else
				{
					CheckCost = 1.0f;
				}

				UpdateSkillUseCost();
			}

			UISkill_V2* TempSkill = OwnerActor_SkillEntityComponent->GetSkillByCategory_V2(InSkillCategoryType);
			if (IsValid(TempSkill))
			{
				CurrentSkillID = TempSkill->GetSkillID();
				SkillTableID = TempSkill->GetSkillTableID();

				

				FSkillTableInfo_V2* SkillInfo = TempSkill->GetSkillInfo();
				if (SkillInfo)
				{
					IsPressFire = !(SkillInfo->VisibleDPAD);
					if (Image_SkillPad && SkillCategoryType != ESkillCategoryTypeEnum::VE_ActiveSkill03)
					{
						if (SkillInfo->VisibleNotifyIcon || IsCrewChangeIcon)
						{
							Image_SkillPad->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
						}
						else
						{
							Image_SkillPad->SetVisibility(ESlateVisibility::Collapsed);
						}
					}

					if (IsCrewChangeIcon)
						return;

					FSkillResourceTableInfo* SkillResourceInfo = TempSkill->GetSkillResourceInfo();
					if (SkillResourceInfo)
					{
						SetSkillIconImage(SkillResourceInfo);
					}
					else
					{
						SetSkillIconState(ESkillIconState::VE_None);
					}
				
					/*if(Image_SkillLock)
					{
						Image_SkillLock->SetVisibility(ESlateVisibility::Collapsed);
					}*/
				
					MaxCoolTime = TempSkill->GetSkillInfo()->CoolTime;
					CoolTime = OwnerActor_SkillEntityComponent->GetSkillCoolTime_V2(CurrentSkillID);
				}


				if (false == OnlyDisplay && SkillCategoryType == ESkillCategoryTypeEnum::VE_ActiveSkill03)
				{
					if ( IsValid(RGameInstance->EventManager))
					{
						RGameInstance->EventManager->OnSynergyEnter.RemoveDynamic(this, &URSkillIcon::CheckSynergy);
						RGameInstance->EventManager->OnSynergyEnter.AddDynamic(this, &URSkillIcon::CheckSynergy);
						if (IsComputeSynergyMesh)
						{
							CheckSynergy(true);
						}
					}
				}
			}
			else
			{
				//lv
				if (Text_LockLevel)
				{
					FString LockText;
// 					switch (InSkillCategoryType)
// 					{
// 						case ESkillCategoryTypeEnum::VE_ActiveSkill01:
// 						{
// 							auto UnlockTable = URenewal_BattleFunctionLibrary::GetSkillUnlocLevelTableInfo("ActiveSkill01");
// 							if (UnlockTable)
// 							{
// 								LockText = FString::FromInt(UnlockTable->HeroLevel);
// 							}
// 						}
// 						break;
// 						case ESkillCategoryTypeEnum::VE_ActiveSkill02:
// 						{
// 							auto UnlockTable = URenewal_BattleFunctionLibrary::GetSkillUnlocLevelTableInfo("ActiveSkill02");
// 							if (UnlockTable)
// 							{
// 								LockText = FString::FromInt(UnlockTable->HeroLevel);
// 							}
// 						}
// 						break;
// 						case ESkillCategoryTypeEnum::VE_ActiveSkill03:
// 						{
// 							auto UnlockTable = URenewal_BattleFunctionLibrary::GetSkillUnlocLevelTableInfo("ActiveSkill03");
// 							if (UnlockTable)
// 							{
// 								LockText = FString::FromInt(UnlockTable->HeroLevel);
// 							}
// 						}
// 						break;
// 					}
					//Text_LockLevel->SetVisibility(ESlateVisibility::SelfHitTestInvisible);

					Text_LockLevel->SetText(FText::FromString(LockText));
				}

				if (IsValid(SkillBuffIcon))
				{
					SkillBuffIcon->SetVisibility(ESlateVisibility::Collapsed);
				}

				if (Image_SkillPad)
				{
					Image_SkillPad->SetVisibility(ESlateVisibility::Collapsed);
				}

				//if (Image_SkillIcon)
				//	Image_SkillIcon->SetBrushFromTexture(nullptr);
				MaxCoolTime = 0;
				CoolTime = 0;
				SetSkillIconState(ESkillIconState::VE_Locked);
				return;
			}
		}
		
		if (0.0f < CoolTime)
		{
			SetSkillIconState(ESkillIconState::VE_InCoolTime);
		}
		else
		{
			SetSkillIconState(ESkillIconState::VE_Normal);
		}
#if LOG_SKILLDEBUG
		float debug_cool_time = OwnerActor_SkillEntityComponent->GetSkillCoolTime_V2(CurrentSkillID);
		UE_LOG(LogGamePlay, Log, TEXT("URSkillIcon::Init(%s) %s CoolTime(%.02f / %.02f)"),
			*(InOwnerCharacter->GetName()), *(CurrentSkillID.ToString()), debug_cool_time, MaxCoolTime);
#endif
	}
	else
	{
		CurrentSkillID = EXCEPTION_NONE;
	}
}

void URSkillIcon::SetSkillIconState(ESkillIconState InSkillIconState)
{
	SkillIconState = InSkillIconState;
	switch (SkillIconState)
	{
		case ESkillIconState::VE_None:
			{
				if (IsValid(Image_SkillIcon))
					Image_SkillIcon->SetVisibility(ESlateVisibility::Collapsed);

				if (IsValid(Text_CoolTime))
					Text_CoolTime->SetVisibility(ESlateVisibility::Collapsed);

				if (IsValid(Image_CoolGauge))
					Image_CoolGauge->SetVisibility(ESlateVisibility::Collapsed);

				if (IsValid(Image_TargetingIndicator))
					Image_TargetingIndicator->SetVisibility(ESlateVisibility::Collapsed);
			}
			break;
		case ESkillIconState::VE_Normal:
			{
				if (IsValid(Image_SkillIcon))
					Image_SkillIcon->SetVisibility(ESlateVisibility::SelfHitTestInvisible);

				if (IsValid(Text_CoolTime))
					Text_CoolTime->SetVisibility(ESlateVisibility::Collapsed);

				if (IsValid(Image_CoolGauge))
					Image_CoolGauge->SetVisibility(ESlateVisibility::Collapsed);

				if (IsValid(Image_TargetingIndicator))
					Image_TargetingIndicator->SetVisibility(ESlateVisibility::Collapsed);
				
				if (Image_SkillLock)
					Image_SkillLock->SetVisibility(ESlateVisibility::Collapsed);

				if (Text_LockLevel)
					Text_LockLevel->SetVisibility(ESlateVisibility::Collapsed);
			}
			break;
		case ESkillIconState::VE_Locked:
			{
				if (IsValid(Image_SkillIcon))
					Image_SkillIcon->SetVisibility(ESlateVisibility::Collapsed);

				if (IsValid(Text_CoolTime))
					Text_CoolTime->SetVisibility(ESlateVisibility::Collapsed);

				if (IsValid(Image_CoolGauge))
					Image_CoolGauge->SetVisibility(ESlateVisibility::Collapsed);

				if (IsValid(Image_TargetingIndicator))
					Image_TargetingIndicator->SetVisibility(ESlateVisibility::Collapsed);

				if (Image_SkillLock)
					Image_SkillLock->SetVisibility(ESlateVisibility::HitTestInvisible);

				if (Text_LockLevel)
					Text_LockLevel->SetVisibility(ESlateVisibility::Collapsed);
			}
			break;
		case ESkillIconState::VE_InCoolTime:
		case ESkillIconState::VE_InCrewCoolTime:
		case ESkillIconState::VE_InSpiritCoolTime:
			{
				if (IsValid(Image_SkillIcon))
					Image_SkillIcon->SetVisibility(ESlateVisibility::HitTestInvisible);

				if (IsValid(Text_CoolTime))
					Text_CoolTime->SetVisibility(ESlateVisibility::HitTestInvisible);

				if (IsValid(Image_CoolGauge))
					Image_CoolGauge->SetVisibility(ESlateVisibility::HitTestInvisible);

				if (IsValid(Image_TargetingIndicator))
					Image_TargetingIndicator->SetVisibility(ESlateVisibility::Collapsed);

				if (Image_SkillLock)
					Image_SkillLock->SetVisibility(ESlateVisibility::Collapsed);

				if (Text_LockLevel)
					Text_LockLevel->SetVisibility(ESlateVisibility::Collapsed);
			}
			break;
		case ESkillIconState::VE_WaitTarget:
		{
			if (IsValid(Image_SkillIcon))
				Image_SkillIcon->SetVisibility(ESlateVisibility::Collapsed);

			if (IsValid(Text_CoolTime))
				Text_CoolTime->SetVisibility(ESlateVisibility::Collapsed);

			if (IsValid(Image_CoolGauge))
				Image_CoolGauge->SetVisibility(ESlateVisibility::Collapsed);

			if (IsValid(Image_TargetingIndicator))
				Image_TargetingIndicator->SetVisibility(ESlateVisibility::HitTestInvisible);

			if (Image_SkillLock)
				Image_SkillLock->SetVisibility(ESlateVisibility::Collapsed);

			if (Text_LockLevel)
				Text_LockLevel->SetVisibility(ESlateVisibility::Collapsed);
		}
		break;
	}	
}

void URSkillIcon::PressIcon()
{
	PlayAni("Click_Pressed");
	if (OwnerCharacter.IsValid() == false)
		return;

	auto BattleEntityComponent = UCharacterHelpFunctionLibrary::GetBattleEntityComponent(OwnerCharacter.Get());
	if (!IsValid(BattleEntityComponent))
		return;

	if (false == BattleEntityComponent->IsAbleToUseSkill() && IsCrewChangeIcon == false)
		return;

	if (SkillIconState != ESkillIconState::VE_Normal)
		return;

	IsTouch = true;
	IsTouchUpdate = true;

#if LOG_SKILLPAD
	UE_LOG(LogBattle, Log, TEXT("URSkillIcon::PressIcon"));
#endif

	if (this->Visibility == ESlateVisibility::Collapsed
		|| this->Visibility == ESlateVisibility::Hidden)
		return;

	if (Image_Abnormal->GetVisibility() != ESlateVisibility::Collapsed)
		return;

#if LOG_SKILLPAD
	UE_LOG(LogBattle, Log, TEXT("URSkillIcon::PressIcon OnlyDisplay %d"), OnlyDisplay);
	UE_LOG(LogBattle, Log, TEXT("URSkillIcon::PressIcon CoolTime %f"), CoolTime);
#endif
	if (OnlyDisplay)
		return;
	//if (CoolTime > 0 || CheckCost != 1)
	if (CoolTime > 0)
		return;

	if ( RGameInstance->EventManager)
	{
#if LOG_SKILLPAD
		UE_LOG(LogBattle, Log, TEXT("URSkillIcon::PressIcon EInputCommandTypeEnum::VE_SkillPress"));
#endif

// 		if (!IsCrewChangeIcon)
// 		{
// 			if (SkillOriginType == ESkillOrigin::VE_Leader)
// 			{
// 				if(UUtilFunctionLibrary::GetDirectionTrackManager()->GetTopEffect() != EDirectionEffectTypeEnum::DE_Battle_CombineEffect)
// 					RGameInstance->EventManager->OnInputSkillNameDelegate.Broadcast(EInputCommandTypeEnum::VE_SkillPress, (int32)SkillCategoryType, CurrentSkillID, (int32)ESkillOrigin::VE_Leader);
// 			}
// 			if (SkillOriginType == ESkillOrigin::VE_Crew1)
// 				RGameInstance->EventManager->OnInputSkillNameDelegate.Broadcast(EInputCommandTypeEnum::VE_SkillPress, (int32)SkillCategoryType, CurrentSkillID, (int32)ESkillIconOrder::VE_Crew1Active3);
// 			if (SkillOriginType == ESkillOrigin::VE_Crew2)
// 				RGameInstance->EventManager->OnInputSkillNameDelegate.Broadcast(EInputCommandTypeEnum::VE_SkillPress, (int32)SkillCategoryType, CurrentSkillID, (int32)ESkillIconOrder::VE_Crew2Active3);
// 		}
// 		else
// 		{
// 			if (Image_SkillIcon->GetVisibility() == ESlateVisibility::Collapsed ||
// 				Image_SkillIcon->GetVisibility() == ESlateVisibility::Hidden)
// 				return;
// 
// 			if (UUtilFunctionLibrary::GetDirectionTrackManager()->GetTopEffect() != EDirectionEffectTypeEnum::DE_Battle_CombineEffect)
// 			{
// 				if (SkillOriginType == ESkillOrigin::VE_Crew1)
// 					RGameInstance->EventManager->OnInputSkillNameDelegate.Broadcast(EInputCommandTypeEnum::VE_SkillPress, (int32)SkillCategoryType, CurrentSkillID, (int32)ESkillOrigin::VE_Crew1);
// 				else
// 					RGameInstance->EventManager->OnInputSkillNameDelegate.Broadcast(EInputCommandTypeEnum::VE_SkillPress, (int32)SkillCategoryType, CurrentSkillID, (int32)ESkillOrigin::VE_Crew2);
// 			}
// 				
// 		}
	}
		
	
}

void URSkillIcon::ReleaseIcon()
{ 
	PlayAni("Click_Pressed", true);
	IsTouch = false;
	IsTouchUpdate = false;

	if (Image_Abnormal->GetVisibility() != ESlateVisibility::Collapsed)
		return;

	if (SkillIconState != ESkillIconState::VE_Normal)
		return;

	if (0 < CoolTime)
		return;

	if (OnlyDisplay)
		return;

	if(IsPressFire && !IsCrewChangeIcon)
	{
		return;
	}

	if (OwnerCharacter.IsValid() == false)
		return;
	auto EntityProperty = Cast<IEntityBaseProperty>(OwnerCharacter.Get());
	if (EntityProperty == nullptr)
		return;
	if (false == EntityProperty->GetAccountCharacter())
		return;

	auto BattleEntityComponent = UCharacterHelpFunctionLibrary::GetBattleEntityComponent(OwnerCharacter.Get());
	auto SkillEntityComponent = UCharacterHelpFunctionLibrary::GetSkillEntityComponent(OwnerCharacter.Get());
	if (!IsValid(BattleEntityComponent))
		return;
	if (!IsValid(SkillEntityComponent))
		return;

	if (false == BattleEntityComponent->IsAbleToUseSkill() && !IsCrewChangeIcon)
		return;

	
	if ( RGameInstance->EventManager)
	{
		if (!IsCrewChangeIcon)
		{
			if (SkillOriginType == ESkillOrigin::VE_Leader)
			{
				EDirectionEffectTypeEnum type = UUtilFunctionLibrary::GetDirectionTrackManager()->GetTopEffect();
				if (UUtilFunctionLibrary::GetDirectionTrackManager()->GetTopEffect() != EDirectionEffectTypeEnum::DE_Battle_CombineEffect)
					RGameInstance->EventManager->OnInputSkillNameDelegate.Broadcast(EInputCommandTypeEnum::VE_SkillRelease, (int32)SkillCategoryType, CurrentSkillID, (int32)ESkillOrigin::VE_Leader);
			}
				
// 			if (SkillOriginType == ESkillOrigin::VE_Crew1)
// 				RGameInstance->EventManager->OnInputSkillNameDelegate.Broadcast(EInputCommandTypeEnum::VE_SkillRelease, (int32)SkillCategoryType, CurrentSkillID, (int32)ESkillIconOrder::VE_Crew1Active3);
// 			if (SkillOriginType == ESkillOrigin::VE_Crew2)
// 				RGameInstance->EventManager->OnInputSkillNameDelegate.Broadcast(EInputCommandTypeEnum::VE_SkillRelease, (int32)SkillCategoryType, CurrentSkillID, (int32)ESkillIconOrder::VE_Crew2Active3);
		}
		else
		{
			if (UUtilFunctionLibrary::GetDirectionTrackManager()->GetTopEffect() != EDirectionEffectTypeEnum::DE_Battle_CombineEffect)
			{
				if (SkillOriginType == ESkillOrigin::VE_Crew1)
					RGameInstance->EventManager->OnInputSkillNameDelegate.Broadcast(EInputCommandTypeEnum::VE_SkillRelease, (int32)SkillCategoryType, CurrentSkillID, (int32)ESkillOrigin::VE_Crew1);
				else
					RGameInstance->EventManager->OnInputSkillNameDelegate.Broadcast(EInputCommandTypeEnum::VE_SkillRelease, (int32)SkillCategoryType, CurrentSkillID, (int32)ESkillOrigin::VE_Crew2);
			}
		}
	}
}

void URSkillIcon::OnDeadActor(AActor* DeadActor, AActor* FromActor)
{
	if (false == IsValid(DeadActor))
		return;
	if (false == OwnerCharacter.IsValid())
		return;
	if (OwnerCharacter.Get() != DeadActor)
		return;

	if (Text_CoolTime->GetVisibility() != ESlateVisibility::Collapsed)
		Text_CoolTime->SetVisibility(ESlateVisibility::Collapsed);

	if(Image_CoolGauge->GetVisibility() != ESlateVisibility::Collapsed)
		Image_CoolGauge->SetVisibility(ESlateVisibility::Collapsed);
}

void URSkillIcon::UpdateSkillUseCost()
{
	/*if (Image_SkillPoint == nullptr)
		return;

	auto OwnerCharacter_SkillEntityComponent = UCharacterHelpFunctionLibrary::GetSkillEntityComponent(OwnerCharacter.Get());
	if (IsValid(OwnerCharacter_SkillEntityComponent) == false)
		return;

	float CurrentCostValue = OwnerCharacter_SkillEntityComponent->GetSkillCostPercentage(SkillID);
	if (CurrentCostValue != -1 && CheckCost != CurrentCostValue)
	{
		CheckCost = CurrentCostValue;
	}
	else
	{
		return;
	}*/

	if (IsValid(Image_SkillPoint))
	{
		UMaterialInstanceDynamic* SkillCost_material = Image_SkillPoint->GetDynamicMaterial();
		if (SkillCost_material)
		{
			FName param_name = "Progress";
			SkillCost_material->SetScalarParameterValue(param_name, CheckCost);
		}
	}

	//if (Image_SkillIcon)
	//{
	//	if (CheckCost != 1)
	//	{
	//		Image_SkillIcon->SetOpacity(0.5f);
	//	}
	//	else
	//	{
	//		Image_SkillIcon->SetOpacity(1);
	//	}
	//}
}

void URSkillIcon::UpdateCooling()
{
	if (0.0f <= CoolTime)
	{
		if (Image_CoolGauge)
		{
			UMaterialInstanceDynamic* gauge_material = Image_CoolGauge->GetDynamicMaterial();
			if (gauge_material)
			{
				FName param_name = "Percent";
				float rate = CoolTime / MaxCoolTime;
				gauge_material->SetScalarParameterValue(param_name, rate);
			}
		}

		if (Text_CoolTime)
		{
			int32 cool_seconds = (int32)CoolTime + 1;
			FText cool_text = FText::Format(FText::FromString("{0}"), FText::AsNumber(cool_seconds));
			Text_CoolTime->SetText(cool_text);
		}
	}
}

void URSkillIcon::EndCooling()
{
	CoolTime = INVALID_INDEXNUM;
	MaxCoolTime = INVALID_INDEXNUM;
}

void URSkillIcon::InvalidateOwnerCharacter()
{
	TArray<FHeroSlotInfo> HeroSlot = RGAMEINSTANCE(this)->PlayStateInfo->GetSelectedCharacterArray();
	for (size_t i = 0; i < HeroSlot.Num(); ++i)
	{
		if (SkillOriginType == ESkillOrigin::VE_Crew1 && HeroSlot[i].CharacterRole == ECharacterRoleEnum::VE_Crew1 && HeroSlot[i].CharacterCrewState == ECharacterCrewStateEnum::VE_Battle)
		{
			AActor* SpawnActor = HeroSlot[i].SpawnedTargetActor;
			Init(SpawnActor, ESkillCategoryTypeEnum::VE_ActiveSkill02);
			break;
		}
		if (SkillOriginType == ESkillOrigin::VE_Crew2 && HeroSlot[i].CharacterRole == ECharacterRoleEnum::VE_Crew2 && HeroSlot[i].CharacterCrewState == ECharacterCrewStateEnum::VE_Battle)
		{
			AActor* SpawnActor = HeroSlot[i].SpawnedTargetActor;
			Init(SpawnActor, ESkillCategoryTypeEnum::VE_ActiveSkill02);
			break;
		}
	}
}

void URSkillIcon::EndCrewSkillCooling()
{
	PrevCharacter = nullptr;
	PrevSkillID = "";

	EndCooling();
}

void URSkillIcon::SetAbnormalImage(bool InIsVisible)
{
	if (Image_Abnormal == nullptr)
		return;

	TArray<FHeroSlotInfo>& HeroSlotArray = RGAMEINSTANCE(this)->PlayStateInfo->GetSelectedCharacterArray();
	if (InIsVisible == false)
	{
		for (size_t i = 0; i < HeroSlotArray.Num(); ++i)
		{
			if (SkillOriginType == ESkillOrigin::VE_Crew1 && HeroSlotArray[i].CharacterRole == ECharacterRoleEnum::VE_Crew1)
			{
				if (HeroSlotArray[i].SlotSpawnState == EHeroSpawnStateEnum::VE_Dead)
					return;
			}
			else if (SkillOriginType == ESkillOrigin::VE_Crew2 && HeroSlotArray[i].CharacterRole == ECharacterRoleEnum::VE_Crew2)
			{
				if (HeroSlotArray[i].SlotSpawnState == EHeroSpawnStateEnum::VE_Dead)
					return;
			}
		}
	}
	
	if (InIsVisible)
	{
		PortraitSwitcher->SetActiveWidgetIndex(1);
		Image_Abnormal->SetVisibility(ESlateVisibility::HitTestInvisible);
	}
	else
	{
		PortraitSwitcher->SetActiveWidgetIndex(0);
		Image_Abnormal->SetVisibility(ESlateVisibility::Collapsed);
	}
}

void URSkillIcon::SetSkillIconImage(FSkillResourceTableInfo* SkillResInfo, ESkillCategoryTypeEnum InSkillCategoryType /*= ESkillCategoryTypeEnum::VE_None*/)
{
	if (SkillResInfo)
	{
		if (IsValid(Image_SkillIcon))
		{
			Image_SkillIcon->SetBrushFromTexture(UUtilFunctionLibrary::GetTexture2DFromAssetID(SkillResInfo->SkillIconRef));
		}

		/*auto TargetImage = ((InSkillCategoryType == ESkillCategoryTypeEnum::VE_ActiveSkill03) ? Image_UltimateBG : Image_ActiveBG);
		if (IsValid(TargetImage))
		{
			switch (SkillResInfo->SkillIconColor)
			{
			case ESkillIconColorEnum::VE_Induce:
				TargetImage->SetColorAndOpacity(InduceColor);
				break;
			case ESkillIconColorEnum::VE_Damage:
				TargetImage->SetColorAndOpacity(DamageColor);
				break;
			default:
				TargetImage->SetColorAndOpacity(FLinearColor(1.0f, 1.0f, 1.0f, 1.0f));
				break;
			}
		}*/

		if (IsValid(SkillBuffIcon))
		{
			SkillBuffIcon->SetSkillIconState(SkillResInfo->SkillIconShape);
		}

		if (Image_Abnormal)
		{
			Image_Abnormal->SetVisibility(ESlateVisibility::Collapsed);
		}

// 		if (PrevCharacter.IsValid())
// 		{
// 			SetSkillIconState(ESkillIconState::VE_InCrewCoolTime);
// 		}
// 		else
// 		{
// 			SetSkillIconState(ESkillIconState::VE_Normal);
// 		}
	}
}

void URSkillIcon::SetOwnerCharacter(AActor* InActor)
{
	OwnerCharacter = InActor;
}



void URSkillIcon::StartCrewCoolTime(AActor* InPrevCharacter, FName InPrevSkillID)
{
	PrevCharacter = InPrevCharacter;
	PrevSkillID = InPrevSkillID;
	//CoolTime = InCrewCoolTime;
	SetSkillIconState(ESkillIconState::VE_InCrewCoolTime);
}

void URSkillIcon::StartSpiritCoolTime(FName InPrevSkillID)
{
	PrevSkillID = InPrevSkillID;
	FSkillTableInfo_V2* SpiritSkill = UTableManager::GetInstancePtr()->GetSkillRow_V2(InPrevSkillID);
	if (SpiritSkill)
		CoolTime = SpiritSkill->CrewCoolTime;
	SetSkillIconState(ESkillIconState::VE_InSpiritCoolTime);
}
