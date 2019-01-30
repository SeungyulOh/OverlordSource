// Fill out your copyright notice in the Description page of Project Settings.

#include "ProjectR.h"
#include "UCW_HPBar.h"

#include "GlobalIntegrated.h"









#include "UtilFunctionIntegrated.h"
#include "ContentGameMode/Controllers/BasePlayerController.h"
#include "Character/Interfaces/EntityBaseProperty.h"
#include "Character/Components/SelectionIndicatorMeshComponent.h"
#include "Character/Components/FunctionalComponents/BattleEntityComponent.h"
#include "Character/Components/FunctionalComponents/EntityRenderComponent.h"
#include "GameInfo/UserInfo.h"
#include "GameInfo/GuildInfo.h"
#include "GameInfo/PlayStateInfo.h"
#include "CustomStructures/HeroSlotInfo.h"
#include "Skill_V2/SharedSkill/ISkill_V2.h"
#include "UI/Component/LocalizingTextBlock.h"
#include "UI/UI_Battle/Component/RBuffIcon.h"
#include "UI/CharacterUI/UC_PileUpUI.h"

#include "Network/RTSManager.h"

void UUCW_HPBar::NativeConstruct()
{
	Super::NativeConstruct();

	BasePlayerController = UUtilFunctionLibrary::GetBasePlayerController(GetWorld());

	auto RGameInstance = RGAMEINSTANCE(this);
	if (IsValid(RGameInstance->EventManager) == false)
		return;

	RGameInstance->EventManager->OnSpawnActorFromCharacterSpawner.RemoveDynamic(this, &UUCW_HPBar::CheckProtectActorSpawn);
	RGameInstance->EventManager->OnSpawnActorFromCharacterSpawner.AddDynamic(this, &UUCW_HPBar::CheckProtectActorSpawn);

	RGameInstance->EventManager->OnUpdateAttackCount.RemoveDynamic(this, &UUCW_HPBar::OnUpdateAttackCount);
	RGameInstance->EventManager->OnUpdateAttackCount.AddDynamic(this, &UUCW_HPBar::OnUpdateAttackCount);

	RGameInstance->EventManager->OnHpBarBoxShow.RemoveDynamic(this, &UUCW_HPBar::OnHpBarBoxShow);
	RGameInstance->EventManager->OnHpBarBoxShow.AddDynamic(this, &UUCW_HPBar::OnHpBarBoxShow);

	if (Image_CoolGauge)
	{
		UMaterialInterface* MI = Cast<UMaterialInterface>(Image_CoolGauge->Brush.GetResourceObject());
		if (MI)
		{
			MID = UMaterialInstanceDynamic::Create(MI, this);
			Image_CoolGauge->SetBrushFromMaterial(MID);
		}

	}
}

void UUCW_HPBar::NativeDestruct()
{
	auto RGameInstance = RGAMEINSTANCE(this);
	if ( IsValid(RGameInstance->EventManager))
	{
		RGameInstance->EventManager->OnSpawnActorFromCharacterSpawner.RemoveDynamic(this, &UUCW_HPBar::CheckProtectActorSpawn);
		RGameInstance->EventManager->OnUpdateAttackCount.RemoveDynamic(this, &UUCW_HPBar::OnUpdateAttackCount);
		RGameInstance->EventManager->OnHpBarBoxShow.RemoveDynamic(this, &UUCW_HPBar::OnHpBarBoxShow);
	}

	USkillEntityComponent* SkillEntityComponent = UCharacterHelpFunctionLibrary::GetSkillEntityComponent(OwnerCharacter.Get());
	if (SkillEntityComponent)
	{
		SkillEntityComponent->OnNotifySkillCasting.RemoveDynamic(this, &UUCW_HPBar::SetCurrentSkillIcon);
		SkillEntityComponent->OnSetAbnormalIcon.RemoveDynamic(this, &UUCW_HPBar::CheckAbnormal);
	}

	if (BattleEntityComponent)
	{
		if (BattleEntityComponent->OnReceiveDamageDelegate.IsBound())
		{
			BattleEntityComponent->OnReceiveDamageDelegate.RemoveDynamic(this, &UUCW_HPBar::OnReceiveDamage);
		}
		BattleEntityComponent->OnSetBlockAbnormalMsg.RemoveDynamic(this, &UUCW_HPBar::SetBlockAbnormalMsg);
	}
	Super::NativeDestruct();
}

void UUCW_HPBar::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);
	UpdateHP();
	UpdateCustomScale();
	UpdateTaticsMove(InDeltaTime);
}

void UUCW_HPBar::SetOwner(AActor* TargetActor)
{
	Super::SetOwner(TargetActor);

	if (Box_AbnormalIcons)
	{
		Box_AbnormalIcons->SetVisibility(ESlateVisibility::HitTestInvisible);
	}

	if (Image_ProtectionMark)
	{
		Image_ProtectionMark->SetVisibility(ESlateVisibility::Collapsed);
	}

	if (URTSManager::GetInstancePtr()->IsConnected())
	{
		auto RGameInstance = RGAMEINSTANCE(this);
		
		{
			if (URTSManager::GetInstancePtr()->IsConnectedMode(EPVP::BG))
			{
				int32 RealTimeParam = 0;
				if (OwnerCharacter.IsValid())
				{
					IEntityBaseProperty* Owner_BaseProperty = Cast<IEntityBaseProperty>(OwnerCharacter.Get());
					if (Owner_BaseProperty)
					{
						RealTimeParam = Owner_BaseProperty->GetRealTimeParam();
					}
				}

				OnRefreshRealTimeParam(RealTimeParam);
			}
		}
	}
	
	OwnerCharacter->GetWorldTimerManager().SetTimerForNextTick(this, &UUCW_HPBar::SettingHPBar);

	UEntityAIComponent* AIComponent = UCharacterHelpFunctionLibrary::GetEntityAIComponent(OwnerCharacter.Get());
	if (AIComponent)
	{
		AIComponent->OnStart_TacticsMove.Clear();
		AIComponent->OnStart_TacticsMove.AddDynamic(this, &UUCW_HPBar::OnStart_TacticMoves);
	}
}

void UUCW_HPBar::RefreshCharacterUserWidget()
{
	SetCharacterColor();
}


void UUCW_HPBar::PlayAniBuffText(bool IsBuff)
{
	if (IsBuff)
		PlayAni("increase_text");
	else
		PlayAni("decrease_text");
}

void UUCW_HPBar::PlayAniImmuneText()
{
	PlayAni("immune_text");
}

void UUCW_HPBar::OnStart_TacticMoves(float Duration)
{
	TM_RemainingTimeToReuse = Duration;
	TM_Duration = Duration;
}

void UUCW_HPBar::OnUpdateAttackCount(AActor* InActor)
{
	int32 nCount = 0;
	int32 nMaxCount = 0;
	if(OwnerCharacter.Get() == InActor)
	{
		UBattleEntityComponent* battlecomp = UCharacterHelpFunctionLibrary::GetBattleEntityComponent(InActor);
		if (IsValid(battlecomp) && IsValid(battlecomp->GetBattleInfoBlackboard()))
		{
			nCount = battlecomp->GetBattleInfoBlackboard()->GetCurrentAttackCount();
			nMaxCount = battlecomp->GetBattleInfoBlackboard()->GetMaxAttackCount();
		}
	}

	if (IsValid(PileUpProgressUI))
	{
		PileUpProgressUI->SetVisibility((0 < nCount ) ? ESlateVisibility::SelfHitTestInvisible : ESlateVisibility::Collapsed);

		if (0 < nCount )
		{
			PileUpProgressUI->SetStackProgress(nCount, nMaxCount - 1);
		}
			
	}
}

void UUCW_HPBar::UpdateHP()
{
	if (IsValid(ProgressBar_HP) == false)
		return;
	
	if (IsValid(BattleEntityComponent) == false)
		return;

	float HPPercent = BattleEntityComponent->GetCurrentHPPercent();
	if (ProgressBar_HP->Percent != HPPercent)
	{
		ProgressBar_HP->SetPercent(HPPercent);
	}

	if (HPPercent <= 0.0f)
	{
		if (ProgressBar_HP)
		{
			ProgressBar_HP->SetVisibility(ESlateVisibility::Collapsed);
		}

		this->SetVisibility(ESlateVisibility::Collapsed);
	}
}

void UUCW_HPBar::UpdateCustomScale()
{
	switch (CharacterCategory)
	{
		case ECharacterCategoryTypeEnum::VE_Hero:
		{
			if (IsControlCharacter)
			{
				if (BasePlayerController == nullptr)
					return;

				bool IsSelectedCharacter = BasePlayerController->IsSelectedCharacter(OwnerCharacter.Get());
				if (IsSelectedCharacter)
				{
					if (IsSelectHpBar == false)
					{
						IsSelectHpBar = true;
						this->SetRenderScale(SelectSize);
					}
				}
				else
				{
					if (IsSelectHpBar)
					{
						IsSelectHpBar = false;
						this->SetRenderScale(DefaultSize);
					}
				}
			}
			else if (IsEnemyHero)
			{
				auto EntityRenderComponent = UCharacterHelpFunctionLibrary::GetEntityRenderComponent(OwnerCharacter.Get());
				if (EntityRenderComponent)
				{
					auto TargetingMesh = EntityRenderComponent->GetTargetingMeshComponent();
					if (TargetingMesh == nullptr)
						return;

					bool debug = TargetingMesh->IsVisible();
					if (debug)
					{
						if (IsSelectHpBar == false)
						{
							IsSelectHpBar = true;
							this->SetRenderScale(SelectSize);
						}
					}
					else
					{
						if (IsSelectHpBar)
						{
							IsSelectHpBar = false;
							this->SetRenderScale(DefaultSize);
						}
					}
				}
			}
		}
		break;
		case ECharacterCategoryTypeEnum::VE_Npc:
		case ECharacterCategoryTypeEnum::VE_BattleObject:
		{
			if (IsBossCharacter)
				return;

			auto EntityRenderComponent = UCharacterHelpFunctionLibrary::GetEntityRenderComponent(OwnerCharacter.Get());
			if (EntityRenderComponent)
			{
				auto TargetingMesh = EntityRenderComponent->GetTargetingMeshComponent();
				if (TargetingMesh == nullptr)
					return;

				bool debug = TargetingMesh->IsVisible();
				if (debug)
				{
					if (IsSelectHpBar == false)
					{
						if (CheckTimerHandler.IsValid())
						{
							OwnerCharacter->GetWorldTimerManager().ClearTimer(CheckTimerHandler);
						}

						IsSelectHpBar = true;
						if (ProgressBar_HP)
						{
							ProgressBar_HP->SetVisibility(ESlateVisibility::HitTestInvisible);
							ProgressBar_HP->SetRenderScale(SelectSize);
						}
					}
				}
				else
				{
					if (IsSelectHpBar)
					{
						if (CheckTimerHandler.IsValid() == false)
						{
							if (ProgressBar_HP)
							{
								ProgressBar_HP->SetVisibility(ESlateVisibility::Collapsed);
							}
						}

						IsSelectHpBar = false;
						if (ProgressBar_HP)
						{
							ProgressBar_HP->SetRenderScale(DefaultSize);
						}
					}
				}
			}
		}
		break;
	}

	
}

void UUCW_HPBar::UpdateTaticsMove(float TimeDelta)
{
	if (!Meet_CoolTime)
		return;

	if (TM_RemainingTimeToReuse > 0)
	{
		TM_RemainingTimeToReuse -= TimeDelta;

		if (Meet_CoolTime->GetVisibility() != ESlateVisibility::SelfHitTestInvisible)
			Meet_CoolTime->SetVisibility(ESlateVisibility::SelfHitTestInvisible);

		if (Text_CoolTime)
			Text_CoolTime->SetText(FText::AsNumber((int32)(TM_RemainingTimeToReuse + 1.f)));

		if (MID)
		{
			MID->SetScalarParameterValue(FName(TEXT("Percent")), TM_RemainingTimeToReuse * 0.1f);
		}
	}
	else
	{
		if (Meet_CoolTime->GetVisibility() == ESlateVisibility::Collapsed)
			return;

		Meet_CoolTime->SetVisibility(ESlateVisibility::Collapsed);

		TM_RemainingTimeToReuse = 0.f;
	}
}

void UUCW_HPBar::UpdateSkillCastingProgress(float InDeltaTime)
{
	if (bCastingTrigger)
	{
		if (IsValid(BattleEntityComponent))
		{
			if (BattleEntityComponent->GetIsPauseState())
				return;
		}

		USkillEntityComponent* SkillEntityComponent = UCharacterHelpFunctionLibrary::GetSkillEntityComponent(OwnerCharacter.Get());
		if (IsValid(SkillEntityComponent) && SkillEntityComponent->IsSkillCasting() == false)
		{
			if (ProgressBar_SkillCasting)
				ProgressBar_SkillCasting->SetVisibility(ESlateVisibility::Collapsed);

			ProgressSkillCastingDataReset();
		}
		else
		{
			if (ProgressBar_SkillCasting)
				ProgressBar_SkillCasting->SetPercent(fCurrentPercent);
			if (UGameplayStatics::IsGamePaused(this) == false)
				fCurrentTime += InDeltaTime;
			fCurrentPercent = FMath::Lerp(0.0f, 1.0f, fCurrentTime / ((fCastingTime >= 0.0f) ? fCastingTime : 0.01f));

			if (fCurrentPercent >= 1.0f)
			{
				if (ProgressBar_SkillCasting)
					ProgressBar_SkillCasting->SetVisibility(ESlateVisibility::Collapsed);

				ProgressSkillCastingDataReset();
			}

		}
	}
}



void UUCW_HPBar::SettingHPBar()
{
	auto RGameInstance = RGAMEINSTANCE(this);
	
	{
		AlwaysShowHPBar = RGameInstance->GameEnvironmentVar.AlwaysShowHPBar;
	}
	
	BattleEntityComponent = UCharacterHelpFunctionLibrary::GetBattleEntityComponent(OwnerCharacter.Get());
	CharacterCategory = UCharacterHelpFunctionLibrary::GetCharacterCategory(OwnerCharacter.Get());

	SetScale();
	SetCharacterName();
	SetCharacterColor();
	SetDamageDelegate();
	SetRealTimeParamDelegate();
	SetAbnormalIcon();
	if(IsValid(BattleEntityComponent))
	{
		if (false == BattleEntityComponent->OnSetBlockAbnormalMsg.IsAlreadyBound(this, &UUCW_HPBar::SetBlockAbnormalMsg))
		{
			BattleEntityComponent->OnSetBlockAbnormalMsg.AddDynamic(this, &UUCW_HPBar::SetBlockAbnormalMsg);
		}
	}
}



void UUCW_HPBar::SetScale()
{
	EGamePlayModeTypeEnum modeType = UUtilFunctionLibrary::GetGamePlayModeEnum();
	switch (CharacterCategory)
	{
		case ECharacterCategoryTypeEnum::VE_Hero:
		{
			IsControlCharacter = URenewal_BattleFunctionLibrary::IsControlCharacter(OwnerCharacter.Get());
			if (IsControlCharacter == false)
			{
				IsCustomScale = false;
			}
			else
			{
				if (BasePlayerController == nullptr)
					return;

				AActor* SelectHero = BasePlayerController->GetSelectedCharacter();
				if (SelectHero)
				{
					if (modeType != EGamePlayModeTypeEnum::VE_BattleGround 
						&& modeType != EGamePlayModeTypeEnum::VE_GuildRaid
						&& modeType != EGamePlayModeTypeEnum::VE_RaidKillBoss)
					{
						if (SelectHero == OwnerCharacter.Get())
							return;
					}

					IsPartyCharacter = URenewal_BattleFunctionLibrary::IsPartyActor(SelectHero, OwnerCharacter.Get());

					if (IsPartyCharacter == false)
					{
						IsNonPortrait = !(URenewal_BattleFunctionLibrary::IsEnemyActor(SelectHero, OwnerCharacter.Get()));
//						if(URTSManager::GetInstancePtr()->IsConnectedMode(EPVP::MR))
//						IsNonPortrait =	URenewal_BattleFunctionLibrary::IsAllyActor(SelectHero, OwnerCharacter.Get());
						IsEnemyHero = !(IsNonPortrait);
					}
				}
			}
		}
		break;
		case ECharacterCategoryTypeEnum::VE_Npc:
		case ECharacterCategoryTypeEnum::VE_BattleObject:
		{
			IsBossCharacter = BattleEntityComponent->IsBoss();
			if (IsBossCharacter)
			{
				this->SetRenderScale(SelectSize);
				if (ProgressBar_HP)
				{
					ProgressBar_HP->SetVisibility(ESlateVisibility::HitTestInvisible);
					AlwaysShowHPBar = true;
				}
			}
			else
			{
				auto OwnerCategory = UCharacterHelpFunctionLibrary::GetCharacterCategory(BattleEntityComponent->GetSummonOwner());
				IsEnemySummon = (OwnerCategory == ECharacterCategoryTypeEnum::VE_Npc) || (OwnerCategory == ECharacterCategoryTypeEnum::VE_BattleObject);
			}

			IsNonPortrait = true;
		}
		break;
	}
}

void UUCW_HPBar::SetCharacterName()
{
	FString UserNick = "";
	FString GuildName = "";

	if (IsValid(Image_BG))
		Image_BG->SetVisibility(ESlateVisibility::Collapsed);

	if (CharacterCategory == ECharacterCategoryTypeEnum::VE_Hero)
	{
		auto RGameInstance = RGAMEINSTANCE(this);
		if ( OwnerCharacter.IsValid())
		{
			IEntityBaseProperty* Owner_BaseProperty = Cast<IEntityBaseProperty>(OwnerCharacter.Get());
			if (Owner_BaseProperty)
			{
				if (Image_Hero && IsNonPortrait == false)
				{
					FName CharacterID = Owner_BaseProperty->GetCharacterID();
					auto CharacterTableInfo = UTableManager::GetInstancePtr()->GetCharacterRow(CharacterID);
					if(CharacterTableInfo)
					{
						UUIFunctionLibrary::SetImageBrush(UUIFunctionLibrary::GetCharacterPortraitSprite(ECharacterCategoryTypeEnum::VE_Hero, CharacterID), Image_Hero);
						Image_Hero->SetVisibility(ESlateVisibility::HitTestInvisible);
					}
				}
			}
		}

		if (IsValid(Image_BG))
		{
			IEntityBaseProperty* Owner_BaseProperty = Cast<IEntityBaseProperty>(OwnerCharacter.Get());
			if (Owner_BaseProperty)
			{
				FName CharacterID = Owner_BaseProperty->GetCharacterID();
				FCharacterTableInfo* CharacterTableInfo = URenewal_BattleFunctionLibrary::GetCharacterTableInfo(ECharacterCategoryTypeEnum::VE_Hero, CharacterID);
				if(CharacterTableInfo != nullptr)
				{
					TArray<UPaperSprite*> BGArray = URAsyncLoader::GetInstancePtr()->PortraitBGArray;

					FLinearColor color;
					if (CharacterTableInfo->Grade >= 1 && CharacterTableInfo->Grade < 4)
					{
						color = FLinearColor::Black;
						Image_BG->Brush.SetResourceObject(BGArray[0]);
					}
					else if (CharacterTableInfo->Grade >= 4 && CharacterTableInfo->Grade < 6)
					{
						color = FColor(67, 186, 35);
						Image_BG->Brush.SetResourceObject(BGArray[1]);
					}
					else if (CharacterTableInfo->Grade >= 6 && CharacterTableInfo->Grade < 8)
					{
						color = FColor(38, 174, 238);
						Image_BG->Brush.SetResourceObject(BGArray[2]);
					}
					else if (CharacterTableInfo->Grade >= 8 && CharacterTableInfo->Grade < 10)
					{
						color = FColor(216, 158, 67);
						Image_BG->Brush.SetResourceObject(BGArray[3]);
					}
					else
						Image_BG->Brush.SetResourceObject(BGArray[4]);

					if (Image_BG && CharacterTableInfo->Grade < 10)
						Image_BG->Brush.TintColor = FSlateColor(color);
					
					Image_BG->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
				}
			}
		}
	}

	if (IsValid(AvartarInfo))
	{
		if (UserNick.IsEmpty())
		{
			AvartarInfo->SetVisibility(ESlateVisibility::Collapsed);
		}
		else
		{
			AvartarInfo->SetVisibility(ESlateVisibility::HitTestInvisible);
		}
	}

	if (IsValid(TextUserName))
	{
		if (UserNick.IsEmpty())
		{
			TextUserName->SetVisibility(ESlateVisibility::Collapsed);
		}
		else
		{
			TextUserName->SetVisibility(ESlateVisibility::HitTestInvisible);
		}

		TextUserName->SetText(FText::FromString(UserNick));
	}

	if (IsValid(TextGuildName))
	{
		if (GuildName.IsEmpty())
		{
			TextGuildName->SetVisibility(ESlateVisibility::Collapsed);
		}
		else
		{
			TextGuildName->SetVisibility(ESlateVisibility::HitTestInvisible);
		}
		TextGuildName->SetText(FText::FromString(GuildName));
	}
}

void UUCW_HPBar::SetCharacterColor()
{
	if (OwnerCharacter.IsValid())
	{
		FLinearColor ProgressBar_HPColor;
		FSlateColor SlateColor;

		bEnemyAgainstCurrentPlayer = URenewal_BattleFunctionLibrary::IsEnemyAgainstCurrentPlayer(OwnerCharacter.Get());
		if (bEnemyAgainstCurrentPlayer)
		{
			ProgressBar_HPColor = Color_EnemyBar;
			SlateColor = Color_EnemySlate;
		}
		else
		{
			ProgressBar_HPColor = Color_FriendBar;

			EGamePlayModeTypeEnum modeType = UUtilFunctionLibrary::GetGamePlayModeEnum();
			if (modeType == EGamePlayModeTypeEnum::VE_BattleGround
				|| modeType == EGamePlayModeTypeEnum::VE_GuildRaid 
				|| modeType == EGamePlayModeTypeEnum::VE_RaidKillBoss )
			{
				auto RGameInstance = RGAMEINSTANCE(this);
				
				{
					IEntityBaseProperty* Owner_BaseProperty = Cast<IEntityBaseProperty>(OwnerCharacter.Get());
					if (Owner_BaseProperty)
					{
// 						IsPartyCharacter = (RGameInstance->GameObjectMgr->GetMyPartyID() == Owner_BaseProperty->GetPartyID());
						IsPartyCharacter = (UObjectFunctionLibrary::GetMyPartyID() == Owner_BaseProperty->GetPartyID());
					}
				}
			}

			if (IsPartyCharacter)
			{
				SlateColor = Color_PartySlate;
			}
			else
			{
				SlateColor = Color_FriendSlate;
			}
		}

		if (IsValid(ProgressBar_HP))
		{
			ProgressBar_HP->SetFillColorAndOpacity(ProgressBar_HPColor);
		}

		if (IsValid(TextUserName))
		{
			TextUserName->SetColorAndOpacity(SlateColor);
		}
	}
}

void UUCW_HPBar::SetCharacterJob()
{
	if (IsValid(ImageJobInfo))
	{
		EClassTypeEnum ClassType = UCharacterHelpFunctionLibrary::GetCharacterClassType(OwnerCharacter.Get());
		UTexture2D* ClassTypeTexture = UResourceFunctionLibrary::GetHeroClassTypeTexture(ClassType);
		if (IsValid(ClassTypeTexture))
		{
			UUIFunctionLibrary::SetImageBrush(ClassTypeTexture, ImageJobInfo);
		}
	}
}

void UUCW_HPBar::SetDamageDelegate()
{
	if (!OwnerCharacter.IsValid())
		return;

	if (!IsValid(BattleEntityComponent))
		return;

	if (bHpBar_Small)
	{
		bool UseHPBar = false;
		bHideDelay = true;

		switch (CharacterCategory)
		{
		case ECharacterCategoryTypeEnum::VE_Minion:
		{
			UseHPBar = true;
			
			if (false == BattleEntityComponent->OnReceiveDamageDelegate.IsAlreadyBound(this, &UUCW_HPBar::OnReceiveDamage))
			{
				BattleEntityComponent->OnReceiveDamageDelegate.AddDynamic(this, &UUCW_HPBar::OnReceiveDamage);
			}
		}break;
		case ECharacterCategoryTypeEnum::VE_Npc:
		case ECharacterCategoryTypeEnum::VE_BattleObject:
		{
			UseHPBar = true;
			if (false == bEnemyAgainstCurrentPlayer)
			{
				bHideDelay = false;
			}
			if (false == BattleEntityComponent->OnReceiveDamageDelegate.IsAlreadyBound(this, &UUCW_HPBar::OnReceiveDamage))
			{
				BattleEntityComponent->OnReceiveDamageDelegate.AddDynamic(this, &UUCW_HPBar::OnReceiveDamage);
			}
		}break;
		case ECharacterCategoryTypeEnum::VE_Object:
		{
			UseHPBar = true;
			if (false == BattleEntityComponent->OnReceiveDamageDelegate.IsAlreadyBound(this, &UUCW_HPBar::OnReceiveDamage))
			{
				BattleEntityComponent->OnReceiveDamageDelegate.AddDynamic(this, &UUCW_HPBar::OnReceiveDamage);
			}
		}break;
		}

		if (UseHPBar)
		{
			if (IsValid(ProgressBar_HP))
			{
				if (bHideDelay)
				{
					ProgressBar_HP->SetVisibility(ESlateVisibility::Collapsed);
				}
				else
				{
					ProgressBar_HP->SetVisibility(ESlateVisibility::HitTestInvisible);
				}
			}
		}
	}
	else
	{
		bHideDelay = false;

		switch (CharacterCategory)
		{
		case ECharacterCategoryTypeEnum::VE_Hero:
		{
			if (false == BattleEntityComponent->OnReceiveDamageDelegate.IsAlreadyBound(this, &UUCW_HPBar::OnReceiveDamage))
			{
				BattleEntityComponent->OnReceiveDamageDelegate.AddDynamic(this, &UUCW_HPBar::OnReceiveDamage);
			}
		}break;
		case ECharacterCategoryTypeEnum::VE_Minion:
		{
			bHideDelay = true;
		}break;
		case ECharacterCategoryTypeEnum::VE_Npc:
		case ECharacterCategoryTypeEnum::VE_BattleObject:
		{
			if (false == BattleEntityComponent->IsBoss())
			{
				bHideDelay = true;
				if (false == BattleEntityComponent->OnReceiveDamageDelegate.IsAlreadyBound(this, &UUCW_HPBar::OnReceiveDamage))
				{
					BattleEntityComponent->OnReceiveDamageDelegate.AddDynamic(this, &UUCW_HPBar::OnReceiveDamage);
				}
			}
		}break;
		}

		if (bHideDelay)
		{
			if (IsValid(ProgressBar_HP))
			{
				ProgressBar_HP->SetVisibility(ESlateVisibility::Collapsed);
			}
		}
	}
}

void UUCW_HPBar::SetSkillCastingProgress()
{
	if (bHpBar_Small)
	{
		USkillEntityComponent* SkillEntityComponent = UCharacterHelpFunctionLibrary::GetSkillEntityComponent(OwnerCharacter.Get());
		if (SkillEntityComponent)
		{
			SkillEntityComponent->OnNotifySkillCasting.RemoveDynamic(this, &UUCW_HPBar::SetCurrentSkillIcon);
			SkillEntityComponent->OnNotifySkillCasting.AddDynamic(this, &UUCW_HPBar::SetCurrentSkillIcon);
		}
	}
}

void UUCW_HPBar::OnReceiveDamage(AActor* Invoker, float Damage, EBattleDamageResultEnum DamageResultType, AActor* FromActor)
{
	if (UUtilFunctionLibrary::IsCheatVisibleInGameHPBar())
	{
		if (IsValid(ProgressBar_HP) && ProgressBar_HP->IsVisible() == false)
		{
			if (ProgressBar_HP->Percent > 0)
			{
				ProgressBar_HP->SetVisibility(ESlateVisibility::HitTestInvisible);
			}
		}

		PlayAni("HpBar_Damage");

		bool bIsHideEvent = (bHpBar_Small && !(bHideDelay && !AlwaysShowHPBar)) 
			|| (bHpBar_Small == false && bHideDelay && !AlwaysShowHPBar); //???

		if (bIsHideEvent)
		{
			if (CheckTimerHandler.IsValid())
			{
				OwnerCharacter->GetWorldTimerManager().ClearTimer(CheckTimerHandler);
			}

			OwnerCharacter->GetWorldTimerManager().SetTimer(CheckTimerHandler, this, &UUCW_HPBar::HideEvent, 3.0f);
		}

		//ReceiveDamageAnimation();
	}
	//OwnerCharacter->GetWorldTimerManager().SetTimerForNextTick(this, &UUCW_HPBar::UpdateHP);
}

void UUCW_HPBar::HideEvent()
{
	if (IsSelectHpBar)
		return;

	if (ProgressBar_HP)
	{
		ProgressBar_HP->SetVisibility(ESlateVisibility::Collapsed);
	}
}

void UUCW_HPBar::SetRealTimeParamDelegate()
{
	if (!OwnerCharacter.IsValid())
		return;

	if (bHpBar_Small)
		return;

	if (ECharacterCategoryTypeEnum::VE_Hero != CharacterCategory)
		return;

	if (!URTSManager::GetInstancePtr()->IsConnected())
		return;

	auto RGameInstance = RGAMEINSTANCE(this);
// 	if (!IsValid(RGameInstance))
// 		return;

	if (!URTSManager::GetInstancePtr()->IsConnectedMode(EPVP::BG))
		return;

	if (!IsValid(BattleEntityComponent))
		return;

	if (false == BattleEntityComponent->OnRefreshRealTimeParam.IsAlreadyBound(this, &UUCW_HPBar::OnRefreshRealTimeParam))
	{
		BattleEntityComponent->OnRefreshRealTimeParam.AddDynamic(this, &UUCW_HPBar::OnRefreshRealTimeParam);
	}
}

void UUCW_HPBar::OnRefreshRealTimeParam(int32 InRealTimeParam)
{
	if (!URTSManager::GetInstancePtr()->IsConnected())
		return;

	auto RGameInstance = RGAMEINSTANCE(this);
// 	if (!IsValid(RGameInstance))
// 		return;

	if (!URTSManager::GetInstancePtr()->IsConnectedMode(EPVP::BG))
		return;

	if (IsValid(PanelMedal) && IsValid(TextMedalCount))
	{
		ESlateVisibility PanelVisibility = ESlateVisibility::Collapsed;

		if (0 < InRealTimeParam)
		{
			PanelVisibility = ESlateVisibility::SelfHitTestInvisible;
			TextMedalCount->SetText(FText::AsNumber(InRealTimeParam));

			// Text Notify
			if (5 <= InRealTimeParam)
			{
				IEntityBaseProperty* Owner_BaseProperty = Cast<IEntityBaseProperty>(OwnerCharacter.Get());
				if (Owner_BaseProperty)
				{
// 					if (RGameInstance->GameObjectMgr->GetMyGroupKey() != Owner_BaseProperty->GetGroupKey())
					if (UObjectFunctionLibrary::GetMyTeamKey() != Owner_BaseProperty->GetGroupKey())
					{
						FRTS_ACTOR* ActorInfo = RGameInstance->RealTimeModManager->Get_RTS_Actor(Owner_BaseProperty->GetPlayerUK());
						if (ActorInfo)
						{
							FFormatOrderedArguments Args;
							Args.Emplace(FText::FromString(*(ActorInfo->nick)));
							Args.Emplace(FText::AsNumber(InRealTimeParam));
							FText LocalizedText = UUtilFunctionLibrary::GetLocalizedString(EStringTableType::VE_UI, TEXT("UI_BattleGroundMsg2"));
							LocalizedText = FText::Format(LocalizedText, Args);
//							RGameInstance->EventManager->OnRealTimeModNotify.Broadcast(LocalizedText);

							RefreshCharacterUserWidget();
						}
					}
				}
			}
		}

		PanelMedal->SetVisibility(PanelVisibility);		
	}
}

void UUCW_HPBar::SetCurrentSkillIcon(AActor* InCaster, UISkill_V2* UseSkill)
{
	if (IsValid(InCaster) && OwnerCharacter == InCaster)
	{
		USkillEntityComponent* SkillEntityComponent = UCharacterHelpFunctionLibrary::GetSkillEntityComponent(OwnerCharacter.Get());
		if (IsValid(SkillEntityComponent) && IsValid(UseSkill))
		{
			auto skillinfo = UseSkill->GetSkillInfo();
			if (skillinfo != nullptr)
			{
				if (SkillEntityComponent->IsSkillCasting())
				{
					bCastingTrigger = true;
					if (ProgressBar_SkillCasting)
						ProgressBar_SkillCasting->SetVisibility(ESlateVisibility::SelfHitTestInvisible);

					fCastingTime = skillinfo->CastingTime;
#ifdef WITH_EDITOR
					FString log = FString::Printf(TEXT("npc castingtime : %f"), fCastingTime);
					GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Turquoise, log);
#endif // WITH_EDITOR
				}
				else
				{
					ProgressSkillCastingDataReset();

					if (ProgressBar_SkillCasting)
						ProgressBar_SkillCasting->SetVisibility(ESlateVisibility::Collapsed);
				}
			}
		}
	}
}

void UUCW_HPBar::ProgressSkillCastingDataReset()
{
	bCastingTrigger = false;
	fCastingTime = 0.0f;
	fCurrentTime = 0.0f;
	fCurrentPercent = 0.0f;
}



void UUCW_HPBar::SetAbnormalIcon()
{
	if (IsEnemySummon)
		return;

	AbnormalIconArray.Empty(3);
	AbnormalIconArray.Emplace(AbnormalIcon1);
	AbnormalIconArray.Emplace(AbnormalIcon2);
	AbnormalIconArray.Emplace(AbnormalIcon3);

	USkillEntityComponent* SkillEntityComponent = UCharacterHelpFunctionLibrary::GetSkillEntityComponent(OwnerCharacter.Get());
	if (IsValid(SkillEntityComponent))
	{
		SkillEntityComponent->OnSetAbnormalIcon.RemoveDynamic(this, &UUCW_HPBar::CheckAbnormal);
		SkillEntityComponent->OnSetAbnormalIcon.AddDynamic(this, &UUCW_HPBar::CheckAbnormal);
	}
}

void UUCW_HPBar::CheckAbnormal(EAbnormalSlotTypeEnum InSlotType, bool InSet)
{
	CheckStatAbnormal(InSlotType, InSet);
	
	int32 Index = 0;
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

				if (Index == 2)
				{
					if (AbnormalStack.Contains(InSlotType) == false)
					{
						AbnormalStack.Emplace(InSlotType);
					}
				}
				++Index;
				continue;
			}

			if (AbnormalStack.Contains(InSlotType))
			{
				AbnormalStack.Remove(InSlotType);
			}

			Element->SetAbnormalState(InSlotType);
			break;
		}
		else
		{
			if (Element->IsTypeCheck(InSlotType))
			{
				EAbnormalSlotTypeEnum Target = EAbnormalSlotTypeEnum::VE_None;
				if (AbnormalStack.Num() > 0)
				{
					Target = InSlotType;
				}

				Element->SetAbnormalState(Target);
				break;
			}
			
			if (Index == 2)
			{
				if (AbnormalStack.Contains(InSlotType))
				{
					AbnormalStack.Remove(InSlotType);
				}
			}
			++Index;
		}
	}
}

void UUCW_HPBar::CheckStatAbnormal(EAbnormalSlotTypeEnum InSlotType, bool InSet)
{
	if (InSet)
	{
		FName Key;
		bool bIsBuff = false;
		FName SlotID = UUtilFunctionLibrary::GetEnumToName(TEXT("EAbnormalSlotTypeEnum"), (int32)InSlotType);
		FAbnormalStateGroupTableInfo_V2* info = UTableManager::GetInstancePtr()->GetAbnormalStateGroupRow_V2(SlotID);
		if (info != nullptr)
		{
			bIsBuff = info->BuffType == EAbnormalApplyTypeEnum::VE_Buff;
			Key = info->DescKey;
		}

		if (Key.IsNone() == false && IsValid(LocalText_Stat))
		{
// 			LocalText_Stat->LocalizingKey = Key;
// 			LocalText_Stat->ApplyLocalizedText();
			LocalText_Stat->SetText(UUtilFunctionLibrary::GetLocalizedString(EStringTableType::VE_Common, Key));
			PlayAniBuffText(bIsBuff);
		}
	}
}

void UUCW_HPBar::SetBlockAbnormalMsg(ESkillApplierTypeEnum InBlockApplierType)
{
	FName Key = UUIFunctionLibrary::GetBlockAbnormalMsg(InBlockApplierType);
	if (Key.IsNone())
		return;

	if (LocalizingTextBlock_1)
	{
// 		LocalizingTextBlock_1->LocalizingKey = Key;
// 		LocalizingTextBlock_1->ApplyLocalizedText();
		LocalizingTextBlock_1->SetText(UUtilFunctionLibrary::GetLocalizedString(EStringTableType::VE_Common, Key));
		PlayAniImmuneText();
	}
}

void UUCW_HPBar::CheckProtectActorSpawn(AActor* SpawnActor)
{
// 	if (SpawnActor && SpawnActor == OwnerCharacter.Get())
// 	{
// 		UBattleEntityComponent* BattleEntityComp = UCharacterHelpFunctionLibrary::GetBattleEntityComponent(SpawnActor);
// 		if (IsValid(BattleEntityComp))
// 		{
// // 			if (BattleEntityComp->CharacterSpawner.IsValid())
// // 			{
// // 				ESpawnerTypeEnum SpawnType = BattleEntityComp->CharacterSpawner->GetSpawnerType();
// // 				if (ESpawnerTypeEnum::VE_ProtectTarget == SpawnType ||
// // 					ESpawnerTypeEnum::VE_ProtectionTargetObject == SpawnType)
// // 				{
// 					if (Box_AbnormalIcons)
// 					{
// 						Box_AbnormalIcons->SetVisibility(ESlateVisibility::Collapsed);
// 					}
// 
// 					if (Image_ProtectionMark)
// 					{
// 						Image_ProtectionMark->SetVisibility(ESlateVisibility::HitTestInvisible);
// 					}
// // 				}
// // 			}
// 		}
// 	}
}



void UUCW_HPBar::OnHpBarBoxShow(bool bShow)
{
	UWidget* hpBox = nullptr;

	hpBox = bHpBar_Small ? (UWidget*)HpBar_Box_Small : (UWidget*)HpBar_Box;
	
	if (IsValid(hpBox))
	{
		hpBox->SetVisibility(bShow ? ESlateVisibility::SelfHitTestInvisible : ESlateVisibility::Collapsed);
	}
}