// Fill out your copyright notice in the Description page of Project Settings.

#include "ProjectR.h"
#include "RBossBar.h"

#include "GlobalIntegrated.h"

//#include "Global/BattleFieldManager.h"

#include "ContentGameMode/Controllers/BasePlayerController.h"
#include "Character/SharedBattle/BattleOperator.h"
#include "Character/Components/FunctionalComponents/BattleEntityComponent.h"
#include "Character/Components/FunctionalComponents/SkillEntityComponent.h"
#include "UtilFunctionIntegrated.h"
#include "UI/Component/ProgressBar_Phase.h"
#include "UI/Component/LocalizingTextBlock.h"
#include "UI/UI_Battle/RBattleTimer.h"
#include "Skill_V2/SharedSkill/ISkill_V2.h"
#include "Network/HttpRequestClient.h"
#include "Network/RTSManager.h"
#include "UI/UI_Battle/Component/RBuffIcon.h"


void URBossBar::NativeConstruct()
{
	Super::NativeConstruct();

	URGameInstance* RGameInstance = RGAMEINSTANCE(this);
	if (RGameInstance == nullptr)
		return;

	BossGaugeDown_Ref = GetWidgetAnimFromName(TEXT("BossGaugeDown"));
	BossGaugeUp_Ref = GetWidgetAnimFromName(TEXT("BossGaugeUp"));

	auto EventManager = RGameInstance->EventManager;
	if (EventManager == nullptr)
		return;

// 	if (UProudNetManager::GetInstancePtr()->IsBattleFieldConnected() ||
// 		UProudNetManager::GetInstancePtr()->IsBattleGroundConnected() ||
// 		UProudNetManager::GetInstancePtr()->IsGuildRaidConnected())
	if (URTSManager::GetInstancePtr()->IsConnectedMode(EPVP::BF) ||
		URTSManager::GetInstancePtr()->IsConnectedMode(EPVP::BG) ||
		URTSManager::GetInstancePtr()->IsConnectedMode(EPVP::MR) ||
		URTSManager::GetInstancePtr()->IsConnectedMode(EPVP::SR) ||
		URTSManager::GetInstancePtr()->IsConnectedMode(EPVP::GR))
	{
		EventManager->OnTargetChange.RemoveDynamic(this, &URBossBar::OnTargetChange);
		EventManager->OnTargetChange.AddDynamic(this, &URBossBar::OnTargetChange);
// 		EventManager->OnDisappear_RTS_Player.RemoveDynamic(this, &URBossBar::OnDisappear_RTS_Player);
// 		EventManager->OnDisappear_RTS_Player.AddDynamic(this, &URBossBar::OnDisappear_RTS_Player);
	}
	else
	{
		EventManager->OnBossSpawn.RemoveDynamic(this, &URBossBar::BossSpawnEvent);
		EventManager->OnBossSpawn.AddDynamic(this, &URBossBar::BossSpawnEvent);

#if WITH_EDITOR
		if (RGameInstance->GetCurrentStateType() == EGameStateEnum::VE_PlayTest)
		{
			EventManager->OnTargetChange.RemoveDynamic(this, &URBossBar::OnTargetChange);
			EventManager->OnTargetChange.AddDynamic(this, &URBossBar::OnTargetChange);
		}
#endif
	}

	EventManager->OnBossGuageHide.RemoveDynamic(this, &URBossBar::BossHideEvent);
	EventManager->OnBossGuageHide.AddDynamic(this, &URBossBar::BossHideEvent);

	EventManager->OnBossShieldNotify.RemoveDynamic(this, &URBossBar::CallbackShieldNotify);
	EventManager->OnBossShieldNotify.AddDynamic(this, &URBossBar::CallbackShieldNotify);

	BossGaugeDown_Ref = GetAni("BossGaugeDown");
	BossGaugeUp_Ref = GetAni("BossGaugeUp");

	if (BossGaugeUp_Ref && TargetCharacter == nullptr)
	{
		this->SetVisibility(ESlateVisibility::Collapsed);
		PlayAnimation(BossGaugeUp_Ref);
	}

	CallbackShieldNotify(0, false);
}

void URBossBar::NativeDestruct()
{
	URGameInstance* RGameInstance = RGAMEINSTANCE(this);
	if (RGameInstance)
	{
		auto EventManager = RGameInstance->EventManager;
		if (EventManager)
		{
			EventManager->OnBossSpawn.RemoveDynamic(this, &URBossBar::BossSpawnEvent);
			EventManager->OnBossGuageHide.RemoveDynamic(this, &URBossBar::BossHideEvent);
			EventManager->OnBossShieldNotify.RemoveDynamic(this, &URBossBar::CallbackShieldNotify);
			EventManager->OnTargetChange.RemoveDynamic(this, &URBossBar::OnTargetChange);
//			EventManager->OnDisappear_RTS_Player.RemoveDynamic(this, &URBossBar::OnDisappear_RTS_Player);
		}
	}

	USkillEntityComponent* SkillEntityComponent = UCharacterHelpFunctionLibrary::GetSkillEntityComponent(TargetCharacter);
	if (SkillEntityComponent)
	{
		SkillEntityComponent->OnNotifySkillCasting.RemoveDynamic(this, &URBossBar::SetCurrentSkillIcon);
	}

	UBattleEntityComponent* BattleEntityComponent = UCharacterHelpFunctionLibrary::GetBattleEntityComponent(TargetCharacter);
	if (IsValid(BattleEntityComponent))
	{
		BattleEntityComponent->OnReceiveDamageDelegate.RemoveDynamic(this, &URBossBar::ReceiveDamageEvent);
	}

	ClearAbnormalIcon();
	/*
	TargetCharacter = nullptr;

	LocalText_BossName = nullptr;
	ProgressBar_Base = nullptr;
	ProgressBar_Deco = nullptr;
	Text_BossHPCount = nullptr;
	Text_BossHPSign = nullptr;
	BattleTimer = nullptr;

	//Ref
	GlowLoop_Ref = nullptr;
	GaugeDamageAni_Ref = nullptr;
	BossGaugeUp_Ref = nullptr;
	BossGaugeDown_Ref = nullptr;
	*/
	Super::NativeDestruct();
}

void URBossBar::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);


	if (IsValid(TargetCharacter))
	{
		if (ProgressBar_Base)
		{
			int32 PhaseValue = ProgressBar_Base->GetCurrentPhase();
			if (PhaseValue != CheckPhaseValue)
			{
				CheckPhaseValue = PhaseValue;
				if (Text_BossHPCount)
				{
					Text_BossHPCount->SetText(FText::AsNumber(PhaseValue));
				}
			}

			if (CheckPhaseValue == 0 && ProgressBar_Base->GetCurrentHP() <= 0)
			{
				CheckPhaseValue = -1;
//				this->SetVisibility(ESlateVisibility::Collapsed);
				BossHideEvent();
			}
		}

		SkillCastingProgressBarTick(InDeltaTime);
		TargetNameCheckTick(InDeltaTime);
	}
	else
	{
		if (eTargetCategory != ECharacterCategoryTypeEnum::VE_None)
		{
			BossHideEvent();
		}
	}
}

void URBossBar::SetTargetNameText(FText TargetName)
{
	if (CurrentDisplayName.EqualTo(TargetName) == false)
	{
		CurrentDisplayName = TargetName;
		if (IsValid(Text_TargetName))
		{
			Text_TargetName->SetText(CurrentDisplayName);
			Text_TargetName->SetVisibility(ESlateVisibility::HitTestInvisible);
		}
		if (IsValid(TargetImage))
		{
			TargetImage->SetVisibility(ESlateVisibility::HitTestInvisible);
		}
	}
}

void URBossBar::HideTargetNameText()
{
	CurrentDisplayName = FText::GetEmpty();
	if (IsValid(Text_TargetName))
	{
		Text_TargetName->SetVisibility(ESlateVisibility::Collapsed);
	}
	if (IsValid(TargetImage))
	{
		TargetImage->SetVisibility(ESlateVisibility::Collapsed);
	}
}

void URBossBar::TargetNameCheckTick(float InDeltaTime)
{
	if (Text_TargetName)
	{
		if (bBossTypeTarget)
		{
			auto EntityAIComponent = UCharacterHelpFunctionLibrary::GetEntityAIComponent(TargetCharacter);
			//UBattleEntityComponent* BattleEntityComponent = UCharacterHelpFunctionLibrary::GetBattleEntityComponent(TargetCharacter);
			if (IsValid(EntityAIComponent))
			{
				auto CurrentAttackTarget = EntityAIComponent->GetBlackboardActorValue(ERAIBalckboardValueEnum::VE_CurrentTargetActor);
				if (CurrentAttackTarget)
				{
					FText AttackTargetName = UUIFunctionLibrary::GetCharacterName(CurrentAttackTarget);
					SetTargetNameText(AttackTargetName);
				}
				else
				{
					HideTargetNameText();
				}
			}
		}
		else
		{
			UBattleEntityComponent* BattleEntityComponent = UCharacterHelpFunctionLibrary::GetBattleEntityComponent(TargetCharacter);
			if (BattleEntityComponent)
			{
				auto CurrentAttackTarget = BattleEntityComponent->GetCurrentAttackTarget();
				FText AttackTargetName = FText();
				IEntityBaseProperty* Target_BaseProperty = Cast<IEntityBaseProperty>(CurrentAttackTarget);
					
				if (Target_BaseProperty)
				{
					auto RGameInstance = RGAMEINSTANCE(this);
					if(RGameInstance)
					{
						int32 PlayerUK = Target_BaseProperty->GetPlayerUK();
						FRTS_ACTOR* BattleFieldUserInfo = RGameInstance->RealTimeModManager->Get_RTS_Actor(PlayerUK);
						if (BattleFieldUserInfo)
							AttackTargetName = FText::FromString(BattleFieldUserInfo->nick);
// 						auto BattleFieldUserInfo = RGameInstance->BattleFieldManager->GetBattleFieldUserInfo(PlayerUK);
// 						if (BattleFieldUserInfo)
// 						{
// 							AttackTargetName = FText::FromString(BattleFieldUserInfo->NickName);
// 						}
					}
				}

				SetTargetNameText(AttackTargetName);
			}
			else
			{
				HideTargetNameText();
			}
		}
	}
}

void URBossBar::SkillCastingProgressBarTick(float InDeltaTime)
{
	if (bCastingTrigger)
	{
		UBattleEntityComponent* BattleEntityComponent = UCharacterHelpFunctionLibrary::GetBattleEntityComponent(TargetCharacter);
		if (IsValid(BattleEntityComponent))
		{
			if (BattleEntityComponent->GetIsPauseState())
				return;
		}

		USkillEntityComponent* SkillEntityComponent = UCharacterHelpFunctionLibrary::GetSkillEntityComponent(TargetCharacter);
		if (IsValid(SkillEntityComponent) && SkillEntityComponent->IsSkillCasting() == false)
		{
			if (Skill_Box)
				Skill_Box->SetVisibility(ESlateVisibility::Collapsed);
			if (Skill_Overlay)
				Skill_Overlay->SetVisibility(ESlateVisibility::Collapsed);

			ProgressSkillCastingDataReset();
		}
		else
		{
			if (ProgressBarSkillCasting)
				ProgressBarSkillCasting->SetPercent(fCurrentPercent);
			if(UGameplayStatics::IsGamePaused(this) == false)
				fCurrentTime += InDeltaTime;
			fCurrentPercent = FMath::Lerp(0.0f, 1.0f, fCurrentTime / ((fCastingTime >= 0.0f) ? fCastingTime : 0.01f));

			if (fCurrentPercent >= 1.0f)
			{
				if (Skill_Box)
					Skill_Box->SetVisibility(ESlateVisibility::Collapsed);
				if (Skill_Overlay)
					Skill_Overlay->SetVisibility(ESlateVisibility::Collapsed);
				ProgressSkillCastingDataReset();
			}

// 			UE_LOG(LogBattle, Log, TEXT("SM : fCurrentPercent : %f"), fCurrentPercent);
// 			FString log = FString::Printf(TEXT("SM : fCurrentPercent : %f, castingtime : %f, fCurrentTime : %f"), fCurrentPercent, fCastingTime, fCurrentTime);
// 			GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Yellow, log);
		}
	}
}

bool URBossBar::IsValidData()
{
	if (ProgressBar_Base == nullptr)
		return false;

	if (ProgressBar_Deco == nullptr)
		return false;

	if (Text_BossHPCount == nullptr)
		return false;

	if (LocalText_BossName == nullptr)
		return false;

	if (ProgressBarSkillCasting == nullptr)
		return false;

	return true;
}

void URBossBar::BossHideEvent()
{
	if (eTargetCategory != ECharacterCategoryTypeEnum::VE_None
//		&& this->GetVisibility() == ESlateVisibility::HitTestInvisible
		)
	{
		if (IsValid(BossGaugeDown_Ref) && IsValid(BossGaugeUp_Ref))
		{
			if (IsAnimationPlaying(BossGaugeDown_Ref))
			{
				StopAnimation(BossGaugeDown_Ref);
			}
			PlayAnimation(BossGaugeUp_Ref);
		}
	}
	HideTargetNameText();
	if (IsValid(Skill_Box))
	{
		Skill_Box->SetVisibility(ESlateVisibility::Collapsed);
	}
	eTargetCategory = ECharacterCategoryTypeEnum::VE_None;

//	this->SetVisibility(ESlateVisibility::Collapsed);
}

void URBossBar::BossSpawnEvent(AActor* InCharacter)
{
	//if (IsValidData() == false)
	//	return;

	if (InCharacter == nullptr)
		return;

	bBossTypeTarget = true;
	eTargetCategory = UCharacterHelpFunctionLibrary::GetCharacterCategory(InCharacter);

	TargetCharacter = InCharacter;

	SetAbnormalIcon();

	//option
	//if(IsProgressBarLayer)
	if (ProgressBar_Base )
	{
		//ProgressBar_Base->SetCustomPhaseColor();
		ProgressBar_Base->SetTargetActor(TargetCharacter, EProgressTypeEnum::VE_Layer1);
		ProgressBar_Base->GetPhasePercent();
		ProgressBar_Base->SynchronizeProperties();

		int32 PhaseValue = ProgressBar_Base->GetCurrentPhase();
		if (Text_BossHPCount)
		{
			Text_BossHPCount->SetText(FText::AsNumber(PhaseValue));
		}
	}
	if (ProgressBar_Deco)
	{
		//ProgressBar_Deco->SetCustomPhaseColor();
		ProgressBar_Deco->SetTargetActor(TargetCharacter, EProgressTypeEnum::VE_Layer2);
		ProgressBar_Deco->SetNeedle(Slider_Needle);
		ProgressBar_Deco->GetPhasePercent();
		ProgressBar_Deco->SynchronizeProperties();
	}

	//Set BossTableData
	UBattleEntityComponent* BattleEntityComponent = UCharacterHelpFunctionLibrary::GetBattleEntityComponent(TargetCharacter);
	if (IsValid(BattleEntityComponent))
	{
		BattleEntityComponent->OnReceiveDamageDelegate.RemoveDynamic(this, &URBossBar::ReceiveDamageEvent);
		BattleEntityComponent->OnReceiveDamageDelegate.AddDynamic(this, &URBossBar::ReceiveDamageEvent);


		if (IsValid(TextBlock_BossLevel))
		{
			if (BattleEntityComponent->GetBattleInfo())
			{
				int32 lv =  BattleEntityComponent->GetBattleInfo()->GetLevel();
				TextBlock_BossLevel->SetText( FText::AsNumber(lv) );
			}
		}

		if(BattleEntityComponent->bBoss)
		{
			this->SetVisibility(ESlateVisibility::HitTestInvisible);

			if (BossGaugeDown_Ref)
			{
				if (IsAnimationPlaying(BossGaugeUp_Ref))
				{
					StopAnimation(BossGaugeUp_Ref);
				}
				PlayAnimation(BossGaugeDown_Ref);
			}
		}
	}

	/*if (Text_BossHPCount)
	{
		Text_BossHPCount->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
	}
	if (Text_BossHPSign)
	{
		Text_BossHPSign->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
	}*/

	SynchronizeBossData();
}

void URBossBar::ReceiveDamageEvent(AActor* InCharacter, float Damage, EBattleDamageResultEnum DamageResultType, AActor* FromActor)
{
	if (GaugeDamageAni_Ref)
		PlayAnimation(GaugeDamageAni_Ref);
}

void URBossBar::CallbackShieldNotify(float InShieldPercent, bool InIsTimeOut)
{
	if (IsValid(ProgressBar_Shield_HP))
	{
		if (InShieldPercent > 0.0f)
		{
			if (ProgressBar_Shield_HP->GetVisibility() != ESlateVisibility::SelfHitTestInvisible)
				ProgressBar_Shield_HP->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
		}
		else
		{
			if (ProgressBar_Shield_HP->GetVisibility() != ESlateVisibility::Collapsed)
				ProgressBar_Shield_HP->SetVisibility(ESlateVisibility::Collapsed);
		}

		ProgressBar_Shield_HP->SetPercent(InShieldPercent);
	}
}

void URBossBar::SynchronizeBossData()
{
	if (TargetCharacter == nullptr)
		return;

	FName						TargetID = UCharacterHelpFunctionLibrary::GetCharacterID(TargetCharacter);
	ECharacterCategoryTypeEnum	TargetChategory = UCharacterHelpFunctionLibrary::GetCharacterCategory(TargetCharacter);

	auto CharacterInfo = URenewal_BattleFunctionLibrary::GetCharacterTableInfo(TargetChategory, TargetID);
	if (CharacterInfo)
	{
		EStringTableType		TableType;
		switch (TargetChategory)
		{
		case ECharacterCategoryTypeEnum::VE_Hero:
			TableType = EStringTableType::VE_Hero;
			break;
		default:
			TableType = EStringTableType::VE_Npc;
			break;
		}

		FText LocalText = UUtilFunctionLibrary::GetLocalizedString(TableType, CharacterInfo->DisplayName);
		if(LocalText_BossName)
			LocalText_BossName->SetText(LocalText);
	}

	//USkillSlot->SkillApplierList
	if(Skill_Box)
	{
		USkillEntityComponent* SkillEntityComponent = UCharacterHelpFunctionLibrary::GetSkillEntityComponent(TargetCharacter);
		if (SkillEntityComponent)
		{
			SkillEntityComponent->OnNotifySkillCasting.RemoveDynamic(this, &URBossBar::SetCurrentSkillIcon);
			SkillEntityComponent->OnNotifySkillCasting.AddDynamic(this, &URBossBar::SetCurrentSkillIcon);
		}
	}
//*/	
}

void URBossBar::SetCurrentSkillIcon(AActor* InCaster, UISkill_V2* UseSkill)
{
// 	if (SkillBox)
// 	{
// 		SkillBox->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
// 	}
#if LOG_UI
	auto name = InCaster->GetName();
	UE_LOG(LogBattle, Log, TEXT("SM : Caster Name : %s"), *(name));
#endif
	if (IsValid(InCaster) && TargetCharacter == InCaster)
	{
		USkillEntityComponent* SkillEntityComponent = UCharacterHelpFunctionLibrary::GetSkillEntityComponent(TargetCharacter);
		if (IsValid(SkillEntityComponent))
		{
			if (IsValid(UseSkill))
			{
				auto skillinfo = UseSkill->GetSkillInfo();
				if(skillinfo != nullptr)
				{
					if (SkillEntityComponent->IsSkillCasting())
					{
						bCastingTrigger = true;
						if (Skill_Box)
							Skill_Box->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
						
						fCastingTime = skillinfo->CastingTime;
						
						if(IsValid(SkillName))
							SkillName->SetText(UUtilFunctionLibrary::GetLocalizedString(EStringTableType::VE_Skill, skillinfo->SkillName));

						FSkillResourceTableInfo* SkillResourceInfo = UseSkill->GetSkillResourceInfo();
						if(SkillResourceInfo)
						{
							UTexture2D* pTexture = UUtilFunctionLibrary::GetTexture2DFromAssetID(SkillResourceInfo->SkillIconRef);
							if (IsValid(Skill_Overlay) && pTexture != nullptr)
							{
								Skill_Overlay->SetVisibility(ESlateVisibility::SelfHitTestInvisible);

								if (SkillResourceInfo)
								{
									if (IsValid(ImageSkillIcon))
									{
										ImageSkillIcon->SetBrushFromTexture(pTexture);
									}
								}
							}
						}

#ifdef WITH_EDITOR
#if LOG_UI
						UE_LOG(LogBattle, Log, TEXT("SM : CastingTime : %f"), fCastingTime);
#endif
// 						FString log = FString::Printf(TEXT("boss castingtime : %f"), fCastingTime);
// 						GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Turquoise, log);
#endif // WITH_EDITOR

// 						UE_LOG(LogBattle, Log, TEXT("SM : CastingStart"));
					}
					else
					{ 
						ProgressSkillCastingDataReset();

						if (Skill_Box)
							Skill_Box->SetVisibility(ESlateVisibility::Collapsed);

// 						UE_LOG(LogBattle, Log, TEXT("SM : CastingEnd"));
					}
				}
			}
		}
	}
}

void URBossBar::ProgressSkillCastingDataReset()
{
	bCastingTrigger = false;
	fCastingTime = 0.0f;
	fCurrentTime = 0.0f;
	fCurrentPercent = 0.0f;
}

void URBossBar::OnTargetChange(AActor* AttackTarget)
{
	bBossTypeTarget = false;
	ShowHPBar(AttackTarget);
}

void URBossBar::OnDisappear_RTS_Player(int32 UserKID)
{
	URGameInstance* RGameInstance = RGAMEINSTANCE(this);
	if (RGameInstance == nullptr)
		return;
	if (!RGameInstance->HttpClient->IsValidKID(UserKID))
		return;

	BossHideEvent();
	TargetCharacter = nullptr;
}

void URBossBar::ShowHPBar(AActor* InCharacter)
{
	//if (IsValidData() == false)
	//	return;

	if (InCharacter == nullptr)
	{
		BossHideEvent();
		TargetCharacter = nullptr;
		return;
	}

	FName TargetID = UCharacterHelpFunctionLibrary::GetCharacterID(InCharacter);
	eTargetCategory = UCharacterHelpFunctionLibrary::GetCharacterCategory(InCharacter);

	if (eTargetCategory != ECharacterCategoryTypeEnum::VE_Hero && eTargetCategory != ECharacterCategoryTypeEnum::VE_Npc && eTargetCategory != ECharacterCategoryTypeEnum::VE_BattleObject)
	{
		if (TargetCharacter != nullptr)
			BossHideEvent();

		TargetCharacter = nullptr;
		return;
	}

	bool bCheckNpc = (eTargetCategory == ECharacterCategoryTypeEnum::VE_Npc) || (eTargetCategory == ECharacterCategoryTypeEnum::VE_BattleObject);
#if WITH_EDITOR
	auto RGameInstance = RGAMEINSTANCE(this);
	if (RGameInstance->GetCurrentStateType() == EGameStateEnum::VE_PlayTest)
	{
		bCheckNpc = false;
	}
#endif

	//if (bCheckNpc)
	//{
	//	auto CharacterInfo = URenewal_BattleFunctionLibrary::GetCharacterTableInfo(eTargetCategory, TargetID);

	//	if (CharacterInfo->Grade != EGradeEnum::VE_Boss && CharacterInfo->Grade != EGradeEnum::VE_Epic)
	//	{
	//		if (TargetCharacter != nullptr)
	//			BossHideEvent();
	//		TargetCharacter = nullptr;
	//		return;
	//	}
	//}

	if (TargetCharacter != InCharacter)
	{
		ClearAbnormalIcon();

		TargetCharacter = InCharacter;

		SetAbnormalIcon();
		CheckAbnormalFromChangeTarget();

		this->SetVisibility(ESlateVisibility::HitTestInvisible);

		if (BossGaugeDown_Ref)
		{
			if (IsAnimationPlaying(BossGaugeUp_Ref))
			{
				StopAnimation(BossGaugeUp_Ref);
			}
			PlayAnimation(BossGaugeDown_Ref);
		}

		//option
		//if(IsProgressBarLayer)
		if (ProgressBar_Base)
		{
			//ProgressBar_Base->SetCustomPhaseColor();
			ProgressBar_Base->SetTargetActor(TargetCharacter, EProgressTypeEnum::VE_Layer1);
			ProgressBar_Base->GetPhasePercent();
			ProgressBar_Base->SynchronizeProperties();

			int32 PhaseValue = ProgressBar_Base->GetCurrentPhase();
			if (Text_BossHPCount)
			{
				Text_BossHPCount->SetText(FText::AsNumber(PhaseValue));
			}
		}
		if (ProgressBar_Deco)
		{
			//ProgressBar_Deco->SetCustomPhaseColor();
			ProgressBar_Deco->SetTargetActor(TargetCharacter, EProgressTypeEnum::VE_Layer2);
			ProgressBar_Deco->SetNeedle(Slider_Needle);
			ProgressBar_Deco->GetPhasePercent();
			ProgressBar_Deco->SynchronizeProperties();
		}

		//Set BossTableData
		UBattleEntityComponent* BattleEntityComponent = UCharacterHelpFunctionLibrary::GetBattleEntityComponent(TargetCharacter);
		if (IsValid(BattleEntityComponent))
		{
			BattleEntityComponent->OnReceiveDamageDelegate.RemoveDynamic(this, &URBossBar::ReceiveDamageEvent);
			BattleEntityComponent->OnReceiveDamageDelegate.AddDynamic(this, &URBossBar::ReceiveDamageEvent);


			if (IsValid(TextBlock_BossLevel))
			{
				if (BattleEntityComponent->GetBattleInfo())
				{
					int32 lv = BattleEntityComponent->GetBattleInfo()->GetLevel();
					TextBlock_BossLevel->SetText(FText::AsNumber(lv));
				}
			}
		}
	}

	/*if (Text_BossHPCount)
	{
	Text_BossHPCount->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
	}
	if (Text_BossHPSign)
	{
	Text_BossHPSign->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
	}*/

	SynchronizeTargetData();
}

void URBossBar::SynchronizeTargetData()
{
	if (TargetCharacter == nullptr)
		return;

	FName TargetID = UCharacterHelpFunctionLibrary::GetCharacterID(TargetCharacter);
//	ECharacterCategoryTypeEnum	TargetCategory = UCharacterHelpFunctionLibrary::GetCharacterCategory(TargetCharacter);

	auto CharacterInfo = URenewal_BattleFunctionLibrary::GetCharacterTableInfo(eTargetCategory, TargetID);
	auto RGameInstance = RGAMEINSTANCE(this);
	FText LocalText = FText();
	if (CharacterInfo)
	{
		EStringTableType TableType;
		switch (eTargetCategory)
		{
		case ECharacterCategoryTypeEnum::VE_Hero:
		{
// 			TableType = EStringTableType::VE_Hero;

			IEntityBaseProperty* Target_BaseProperty = Cast<IEntityBaseProperty>(TargetCharacter);
			if (Target_BaseProperty)
			{
				FRTS_ACTOR* BattleFieldUserInfo = RGameInstance->RealTimeModManager->Get_RTS_Actor(Target_BaseProperty->GetPlayerUK());
				if (BattleFieldUserInfo)
					LocalText = FText::FromString(BattleFieldUserInfo->nick);
//				int32 PlayerUK = Target_BaseProperty->GetPlayerUK();
// 				auto BattleFieldUserInfo = RGameInstance->BattleFieldManager->GetBattleFieldUserInfo(PlayerUK);
// 				if (BattleFieldUserInfo)
// 				{
// 					LocalText = FText::FromString(BattleFieldUserInfo->NickName);
// 				}
			}
		}break;
		default:
		{
			TableType = EStringTableType::VE_Npc;
			LocalText = UUtilFunctionLibrary::GetLocalizedString(TableType, CharacterInfo->DisplayName);
		}break;
		}

//		FText LocalText = UUtilFunctionLibrary::GetLocalizedString(TableType, CharacterInfo->DisplayName);
		if (LocalText_BossName)
			LocalText_BossName->SetText(LocalText);
	}

	//USkillSlot->SkillApplierList
	if (Skill_Box)
	{
		USkillEntityComponent* SkillEntityComponent = UCharacterHelpFunctionLibrary::GetSkillEntityComponent(TargetCharacter);
		if (SkillEntityComponent)
		{
			SkillEntityComponent->OnNotifySkillCasting.RemoveDynamic(this, &URBossBar::SetCurrentSkillIcon);
			SkillEntityComponent->OnNotifySkillCasting.AddDynamic(this, &URBossBar::SetCurrentSkillIcon);
		}
	}
}

void URBossBar::ClearAbnormalIcon()
{
	for (auto Element : AbnormalIconArray)
	{
		if (Element == nullptr)
			continue;

		Element->SetStatUpDownState(EAbnormalSlotTypeEnum::VE_None);
	}

	USkillEntityComponent* SkillEntityComponent = UCharacterHelpFunctionLibrary::GetSkillEntityComponent(TargetCharacter);
	if (IsValid(SkillEntityComponent))
	{
		SkillEntityComponent->OnSetAbnormalIcon.RemoveDynamic(this, &URBossBar::CheckAbnormal);
	}
}

void URBossBar::CheckAbnormalFromChangeTarget()
{
	USkillEntityComponent* SkillEntityComponent = UCharacterHelpFunctionLibrary::GetSkillEntityComponent(TargetCharacter);
	if (IsValid(SkillEntityComponent))
	{
		int32 Count = 0;
		for (auto Element : CheckAbnormalStatArray)
		{
			if (SkillEntityComponent->IsInSlot(Element))
			{
				CheckAbnormal(Element, true);
				++Count;
				if (Count >= 5)
				{
					break;;
				}
			}
		}
	}
	
}

void URBossBar::SetAbnormalIcon()
{
	AbnormalIconArray.Reset(5);
	AbnormalIconArray.Emplace(BuffIcon1);
	AbnormalIconArray.Emplace(BuffIcon2);
	AbnormalIconArray.Emplace(BuffIcon3);
	AbnormalIconArray.Emplace(BuffIcon4);
	AbnormalIconArray.Emplace(BuffIcon5);

	USkillEntityComponent* SkillEntityComponent = UCharacterHelpFunctionLibrary::GetSkillEntityComponent(TargetCharacter);
	if (IsValid(SkillEntityComponent))
	{
		SkillEntityComponent->OnSetAbnormalIcon.RemoveDynamic(this, &URBossBar::CheckAbnormal);
		SkillEntityComponent->OnSetAbnormalIcon.AddDynamic(this, &URBossBar::CheckAbnormal);
	}
}

void URBossBar::CheckAbnormal(EAbnormalSlotTypeEnum InSlotType, bool InSet)
{
	for (auto Element : AbnormalIconArray)
	{
		if (Element == nullptr)
			continue;

		if (InSet)
		{
			if (Element->IsActive())
			{
				if (Element->IsTypeCheck(InSlotType))
				{
					break;
				}
				continue;
			}

			Element->SetStatUpDownState(InSlotType);
			break;
		}
		else
		{
			if (Element->IsTypeCheck(InSlotType))
			{
				Element->SetStatUpDownState(EAbnormalSlotTypeEnum::VE_None);
				break;
			}
		}
	}
}

void URBossBar::CreateCheckAbnormalArray()
{
	if (CheckAbnormalStatArray.Num() != 0)
		return;

	int32 Max = 32;
	CheckAbnormalStatArray.Reserve(Max);

	CheckAbnormalStatArray.Emplace(EAbnormalSlotTypeEnum::VE_Slot_StatUpPhysicalAttack);
	CheckAbnormalStatArray.Emplace(EAbnormalSlotTypeEnum::VE_Slot_StatUpMagicalAttack);
	CheckAbnormalStatArray.Emplace(EAbnormalSlotTypeEnum::VE_Slot_StatUpPhysicalDefence);
	CheckAbnormalStatArray.Emplace(EAbnormalSlotTypeEnum::VE_Slot_StatUpMagicalDefence);
	CheckAbnormalStatArray.Emplace(EAbnormalSlotTypeEnum::VE_Slot_StatUpAccuracy);//
	CheckAbnormalStatArray.Emplace(EAbnormalSlotTypeEnum::VE_Slot_StatUpAvoid);
	CheckAbnormalStatArray.Emplace(EAbnormalSlotTypeEnum::VE_Slot_StatUpAttackSpeed);
	CheckAbnormalStatArray.Emplace(EAbnormalSlotTypeEnum::VE_Slot_StatUpAttackRange);
	CheckAbnormalStatArray.Emplace(EAbnormalSlotTypeEnum::VE_Slot_StatUpMoveSpeed);
	CheckAbnormalStatArray.Emplace(EAbnormalSlotTypeEnum::VE_Slot_StatUpCriticalRate);//
	CheckAbnormalStatArray.Emplace(EAbnormalSlotTypeEnum::VE_Slot_StatUpCriticalDamage);
	CheckAbnormalStatArray.Emplace(EAbnormalSlotTypeEnum::VE_Slot_StatUpPierceValue);
// 	CheckAbnormalStatArray.Emplace(EAbnormalSlotTypeEnum::VE_Slot_StatUpParryValue);
// 	CheckAbnormalStatArray.Emplace(EAbnormalSlotTypeEnum::VE_Slot_StatUpCriticalResistValue);
// 	CheckAbnormalStatArray.Emplace(EAbnormalSlotTypeEnum::VE_Slot_StatUpCriticalResistDamage);
// 	CheckAbnormalStatArray.Emplace(EAbnormalSlotTypeEnum::VE_Slot_StatUpMagicCriticalRate);
// 	CheckAbnormalStatArray.Emplace(EAbnormalSlotTypeEnum::VE_Slot_ReflectDamage);
	
	CheckAbnormalStatArray.Emplace(EAbnormalSlotTypeEnum::VE_Slot_StatDownPhysicalAttack);
	CheckAbnormalStatArray.Emplace(EAbnormalSlotTypeEnum::VE_Slot_StatDownMagicalAttack);
	CheckAbnormalStatArray.Emplace(EAbnormalSlotTypeEnum::VE_Slot_StatDownPhysicalDefence);
	CheckAbnormalStatArray.Emplace(EAbnormalSlotTypeEnum::VE_Slot_StatDownMagicalDefence);//
	CheckAbnormalStatArray.Emplace(EAbnormalSlotTypeEnum::VE_Slot_StatDownAccuracy);
	CheckAbnormalStatArray.Emplace(EAbnormalSlotTypeEnum::VE_Slot_StatDownAvoid);
	CheckAbnormalStatArray.Emplace(EAbnormalSlotTypeEnum::VE_Slot_StatDownAttackSpeed);
	CheckAbnormalStatArray.Emplace(EAbnormalSlotTypeEnum::VE_Slot_StatDownAttackRange);
	CheckAbnormalStatArray.Emplace(EAbnormalSlotTypeEnum::VE_Slot_StatDownMoveSpeed);//
	CheckAbnormalStatArray.Emplace(EAbnormalSlotTypeEnum::VE_Slot_StatDownCriticalRate);
	CheckAbnormalStatArray.Emplace(EAbnormalSlotTypeEnum::VE_Slot_StatDownCriticalDamage);
	CheckAbnormalStatArray.Emplace(EAbnormalSlotTypeEnum::VE_Slot_StatDownPierceValue);
// 	CheckAbnormalStatArray.Emplace(EAbnormalSlotTypeEnum::VE_Slot_StatDownParryValue);
// 	CheckAbnormalStatArray.Emplace(EAbnormalSlotTypeEnum::VE_Slot_StatDownCriticalResistValue);
// 	CheckAbnormalStatArray.Emplace(EAbnormalSlotTypeEnum::VE_Slot_StatDownCriticalResistDamage);
// 	CheckAbnormalStatArray.Emplace(EAbnormalSlotTypeEnum::VE_Slot_StatDownMagicCriticalRate);
}
