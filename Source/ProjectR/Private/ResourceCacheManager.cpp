// Fill out your copyright notice in the Description page of Project Settings.

#include "ProjectR.h"
#include "ResourceCacheManager.h"
#include "Global/TableManager.h"
#include "Global/AsyncLoader.h"
#include "UIFunctionLibrary.h"
#include "UI/UP_LoadingScene.h"



void UResourceCacheManager::Clear()
{
	CachedClassArray.Empty();
	CachedObjectArray.Empty();
	AssetsQueue.Empty();
}

void UResourceCacheManager::cache_HeroShop(FStreamableDelegate Callback)
{
	TArray<FName> cacheNames = UTableManager::GetInstancePtr()->GetResourceCacheRowNames();
	for (size_t i = 0; i < cacheNames.Num(); ++i)
	{
		FResourceCacheTable* tableinfo = UTableManager::GetInstancePtr()->GetResourceCacheRow(cacheNames[i]);
		if (tableinfo)
		{
			if (!tableinfo->cacheclass.ToString().IsEmpty())
			{
				AssetsQueue.AddUnique(tableinfo->cacheclass.ToSoftObjectPath());
			}

			if (!tableinfo->cacheobj.ToString().IsEmpty())
			{
				AssetsQueue.AddUnique(tableinfo->cacheobj.ToSoftObjectPath());
			}
		}
	}

	if (AssetsQueue.Num())
	{
		OnAsyncLoadedCompleted.Unbind();
		OnAsyncLoadedCompleted.BindUObject(this, &UResourceCacheManager::Callback_AsyncLoaded);

		UUIFunctionLibrary::ShowWaitingPopup(true);
		URAsyncLoader::GetInstancePtr()->LoadAsyncResource(AssetsQueue, Callback);
	}
		
}

void UResourceCacheManager::cache_TutorialMap(FStreamableDelegate Callback)
{
	/*Todo : Need to add resources in AssetsQueue*/
	AssetsQueue.Empty();
	SetAssetResource_FromTables(AssetsQueue);
	FName Agathon = FName(TEXT("1002"));
	FName Flame_NPC = FName(TEXT("122510"));
	FName Sophia_NPC = FName(TEXT("122511"));
	FName Heal_NPC = FName(TEXT("122512"));
	SetAssetResource(AssetsQueue, Agathon, false);
	SetAssetResource(AssetsQueue, Flame_NPC, true);
	SetAssetResource(AssetsQueue, Sophia_NPC, true);
	SetAssetResource(AssetsQueue, Heal_NPC, true);
	/**/

	UUP_LoadingScene* LoadingScene = Cast<UUP_LoadingScene>(RGAMEINSTANCE(this)->LoadingSceneManager->Widget);
	if (IsValid(LoadingScene))
	{
		int32 iTemp = AssetsQueue.Num() / iPartition;
		LoadingScene->UnitPercent = 100.f / iTemp;
	}
	
	TArray<FSoftObjectPath> TargetAssets;
	for (size_t i = 0; i < iPartition; ++i)
	{
		if (AssetsQueue.IsValidIndex(0))
		{
			FSoftObjectPath target = AssetsQueue.Pop();
			TargetAssets.AddUnique(target);
		}
	}
	if (TargetAssets.Num())
		URAsyncLoader::GetInstancePtr()->LoadAsyncResource(TargetAssets, FStreamableDelegate::CreateUObject(this, &UResourceCacheManager::RecursiveCacheResource , Callback));
	
}

void UResourceCacheManager::SetAssetResource(TArray<FSoftObjectPath>& InAssets, FName& InHeroID, bool InNpc /*= false*/)
{
	FCharacterTableInfo* CharacterInfo;
	if (InNpc)
		CharacterInfo = UTableManager::GetInstancePtr()->GetCharacterNpcRow(InHeroID);
	else
		CharacterInfo = UTableManager::GetInstancePtr()->GetCharacterRow(InHeroID);

	if (CharacterInfo == nullptr)
		return;

	if (!CharacterInfo->HeroClass.IsNull())
		InAssets.AddUnique(CharacterInfo->HeroClass.ToSoftObjectPath());
	if (!CharacterInfo->MeshMaterial.IsNull())
		InAssets.AddUnique(CharacterInfo->MeshMaterial.ToSoftObjectPath());
	if (!CharacterInfo->MeshMaterial2.IsNull())
		InAssets.AddUnique(CharacterInfo->MeshMaterial2.ToSoftObjectPath());
	if (!CharacterInfo->DecoParticle.IsNull())
		InAssets.AddUnique(CharacterInfo->DecoParticle.ToSoftObjectPath());
	if (!CharacterInfo->DecoMesh.IsNull())
		InAssets.AddUnique(CharacterInfo->DecoMesh.ToSoftObjectPath());
	if (!CharacterInfo->RWeaponType.IsNull())
		InAssets.AddUnique(CharacterInfo->RWeaponType.ToSoftObjectPath());
	if (!CharacterInfo->LWeaponType.IsNull())
		InAssets.AddUnique(CharacterInfo->LWeaponType.ToSoftObjectPath());

	//Spawn Sequence
	if (!CharacterInfo->SpawnSequence.IsNone())
	{
		FResourceSequenceTableInfo* SequenceTableInfo = UTableManager::GetInstancePtr()->GetResourceSequenceRow(CharacterInfo->SpawnSequence);
		if (SequenceTableInfo)
		{
			InAssets.AddUnique(SequenceTableInfo->SequenceClass.ToSoftObjectPath());

			//if (SequenceTableInfo->SequenceClass.ToString().Contains(TEXT("_C")))
			//{
			//	FString path = SequenceTableInfo->SequenceClass.ToString();
			//	path = path.LeftChop(2);

			//	FSoftObjectPath SoftObjectPath;
			//	SoftObjectPath.SetPath(path);

			//	InAssets.AddUnique(SoftObjectPath);
			//}

		}

	}

	//HitEffect
	FResourceFXTableInfo* HitFXTable = UTableManager::GetInstancePtr()->GetResourceFXRow(CharacterInfo->HitEffect);
	if (HitFXTable)
	{
		if (HitFXTable != nullptr && !HitFXTable->Particle.IsNull())
			InAssets.AddUnique(HitFXTable->Particle.ToSoftObjectPath());
		if (HitFXTable != nullptr && !HitFXTable->Sound.IsNull())
			InAssets.AddUnique(HitFXTable->Sound.ToSoftObjectPath());
	}


	//	Bullet 관련
	FBulletTableInfo* BulletTable = UTableManager::GetInstancePtr()->GetBulletRow(CharacterInfo->BulletID);
	if (BulletTable != nullptr)
	{
		if (!BulletTable->BulletClass.IsNull())
			InAssets.AddUnique(BulletTable->BulletClass.ToSoftObjectPath());

		FResourceFXTableInfo* BulletFXTable = UTableManager::GetInstancePtr()->GetResourceFXRow(BulletTable->BulletHitEffect);
		if (BulletFXTable != nullptr && !BulletFXTable->Particle.IsNull())
			InAssets.AddUnique(BulletFXTable->Particle.ToSoftObjectPath());
		if (BulletFXTable != nullptr && !BulletFXTable->Sound.IsNull())
			InAssets.AddUnique(BulletFXTable->Sound.ToSoftObjectPath());
	}

	//	Skill 관련
	FHERO* CurHeroInfo = RGAMEINSTANCE(GEngine)->RInventory->GetHeroWithID(InHeroID);
	TArray<int32>		skills = CurHeroInfo == nullptr ? TArray<int32>() : CurHeroInfo->skills;

	TArray<FName>	SkillID;
	if (!CharacterInfo->ActiveSkill01.IsNone())
	{
		FString skillstr = CharacterInfo->ActiveSkill01.ToString();
		if (!InNpc)
			skillstr += TEXT("_") + FString::FromInt(skills.IsValidIndex((int32)ESkillCategoryTypeEnum::VE_ActiveSkill01) ? skills[(int32)ESkillCategoryTypeEnum::VE_ActiveSkill01] + 1 : 1);
		SkillID.Emplace(*skillstr);
	}
	if (!CharacterInfo->ActiveSkill02.IsNone())
	{
		FString skillstr = CharacterInfo->ActiveSkill02.ToString();
		if (!InNpc)
			skillstr += TEXT("_") + FString::FromInt(skills.IsValidIndex((int32)ESkillCategoryTypeEnum::VE_ActiveSkill02) ? skills[(int32)ESkillCategoryTypeEnum::VE_ActiveSkill02] + 1 : 1);
		SkillID.Emplace(*skillstr);
	}
	if (!CharacterInfo->ActiveSkill03.IsNone())
	{
		FString skillstr = CharacterInfo->ActiveSkill03.ToString();
		if (!InNpc)
			skillstr += TEXT("_") + FString::FromInt(skills.IsValidIndex((int32)ESkillCategoryTypeEnum::VE_ActiveSkill03) ? skills[(int32)ESkillCategoryTypeEnum::VE_ActiveSkill03] + 1 : 1);
		SkillID.Emplace(*skillstr);
	}
	if (!CharacterInfo->PassiveSkill01.IsNone())
	{
		FString skillstr = CharacterInfo->PassiveSkill01.ToString();
		if (!InNpc)
			skillstr += TEXT("_") + FString::FromInt(skills.IsValidIndex((int32)ESkillCategoryTypeEnum::VE_PassiveSkill01) ? skills[(int32)ESkillCategoryTypeEnum::VE_PassiveSkill01] + 1 : 1);
		SkillID.Emplace(*skillstr);
	}

	for (int32 i = 0; i < SkillID.Num(); i++)
	{
		if (SkillID.IsValidIndex(i) == false)
			continue;
		if (SkillID[i].IsNone() || SkillID[i].IsValid() == false)
			continue;

		FSkillTableInfo_V2* SkillInfo_V2 = nullptr;
		SkillInfo_V2 = UTableManager::GetInstancePtr()->GetSkillRow_V2(SkillID[i]);

		if (SkillInfo_V2 == nullptr)
			continue;

		auto Recursive_IfSummon = [=, &InAssets](ESkillApplierTypeEnum InType, FName SummonID) {
			if (((uint8)InType >= (uint8)ESkillApplierTypeEnum::VE_i_summon_1Max1) &&
				((uint8)InType <= (uint8)ESkillApplierTypeEnum::VE_i_summon_Once5))
			{
				SetAssetResource(InAssets, SummonID, true);
			}
		};

		Recursive_IfSummon(SkillInfo_V2->TargetEffect1, FName(*FString::FromInt(SkillInfo_V2->TargetValue1)));
		Recursive_IfSummon(SkillInfo_V2->TargetEffect2, FName(*FString::FromInt(SkillInfo_V2->TargetValue2)));
		Recursive_IfSummon(SkillInfo_V2->TargetEffect3, FName(*FString::FromInt(SkillInfo_V2->TargetValue3)));
		Recursive_IfSummon(SkillInfo_V2->TargetEffect4, FName(*FString::FromInt(SkillInfo_V2->TargetValue4)));
		Recursive_IfSummon(SkillInfo_V2->TargetEffect5, FName(*FString::FromInt(SkillInfo_V2->TargetValue5)));
		Recursive_IfSummon(SkillInfo_V2->TargetEffect6, FName(*FString::FromInt(SkillInfo_V2->TargetValue6)));


		FSkillResourceTableInfo* SkillResourceInfo = UTableManager::GetInstancePtr()->GetSkillResourceRow(SkillInfo_V2->SkillResourceID);
		if (SkillResourceInfo == nullptr)
			continue;

		if (!SkillResourceInfo->CastingEffect.IsNull())
			InAssets.AddUnique(SkillResourceInfo->CastingEffect.ToSoftObjectPath());
		if (!SkillResourceInfo->CastingBeamEffect.IsNull())
			InAssets.AddUnique(SkillResourceInfo->CastingBeamEffect.ToSoftObjectPath());
		if (!SkillResourceInfo->ActionEffect.IsNull())
			InAssets.AddUnique(SkillResourceInfo->ActionEffect.ToSoftObjectPath());
		if (!SkillResourceInfo->AreaEffect.IsNull())
			InAssets.AddUnique(SkillResourceInfo->AreaEffect.ToSoftObjectPath());
		if (!SkillResourceInfo->ScreenEffect.IsNull())
			InAssets.AddUnique(SkillResourceInfo->ScreenEffect.ToSoftObjectPath());

		FBulletTableInfo* BulletTableInfo = UTableManager::GetInstancePtr()->GetBulletRow(SkillResourceInfo->BulletID);
		if (BulletTableInfo != nullptr)
		{
			if (!BulletTableInfo->BulletClass.IsNull())
				InAssets.AddUnique(BulletTableInfo->BulletClass.ToSoftObjectPath());
		}


		FResourceFXTableInfo* SkillResource = UTableManager::GetInstancePtr()->GetResourceFXRow(SkillResourceInfo->HitEffect);
		if (SkillResource == nullptr)
			continue;

		if (!SkillResource->Particle.IsNull())
			InAssets.AddUnique(SkillResource->Particle.ToSoftObjectPath());
		if (!SkillResource->Sound.IsNull())
			InAssets.AddUnique(SkillResource->Sound.ToSoftObjectPath());

	}
	//	Skill 관련

	// Animation Particle
	TSubclassOf<ABattleBaseCharacter> HeroBPClass = CharacterInfo->GetHeroBlueprintClass();
	AActor* Character_CDO = HeroBPClass->GetDefaultObject<AActor>();
	if (Character_CDO)
	{
		USkeletalMeshComponent* SkelComp = Character_CDO->FindComponentByClass<USkeletalMeshComponent>();
		if (SkelComp && SkelComp->AnimClass)
		{
			TSubclassOf<UAnimInstance> AnimInstanceClass = SkelComp->AnimClass;
			UBattleBaseAnimInstance* BattleAnimInst_CDO = AnimInstanceClass->GetDefaultObject<UBattleBaseAnimInstance>();

			if (BattleAnimInst_CDO)
			{
				if (!BattleAnimInst_CDO->AnimMontageIdle.IsNull())
					InAssets.AddUnique(BattleAnimInst_CDO->AnimMontageIdle.ToSoftObjectPath());
				if (!BattleAnimInst_CDO->AnimMontageRun.IsNull())
					InAssets.AddUnique(BattleAnimInst_CDO->AnimMontageRun.ToSoftObjectPath());
				if (!BattleAnimInst_CDO->AnimMontageAttack.IsNull())
					InAssets.AddUnique(BattleAnimInst_CDO->AnimMontageAttack.ToSoftObjectPath());
				if (!BattleAnimInst_CDO->AnimMontageAbnormal.IsNull())
					InAssets.AddUnique(BattleAnimInst_CDO->AnimMontageAbnormal.ToSoftObjectPath());
				if (!BattleAnimInst_CDO->AnimMontageEvent.IsNull())
					InAssets.AddUnique(BattleAnimInst_CDO->AnimMontageEvent.ToSoftObjectPath());
				if (!BattleAnimInst_CDO->AnimMontageDamageResult.IsNull())
					InAssets.AddUnique(BattleAnimInst_CDO->AnimMontageDamageResult.ToSoftObjectPath());

				auto AddAssets = [&InAssets](TAssetPtr<UAnimMontage> InAsset) {
					UAnimMontage* AttackAnim = UUtilFunctionLibrary::GetAnimMontageFromAssetID(InAsset);
					if (AttackAnim)
					{
						for (auto Element : AttackAnim->Notifies)
						{
							UAttachResourceFXNotify* FXNoti = Cast<UAttachResourceFXNotify>(Element.Notify);
							if (FXNoti)
							{
								FResourceFXTableInfo* info = UTableManager::GetInstancePtr()->GetResourceFXRow(FXNoti->FXKey);
								if (info)
								{
									if (!info->Particle.IsNull())
										InAssets.AddUnique(info->Particle.ToSoftObjectPath());
									if (!info->Sound.IsNull())
										InAssets.AddUnique(info->Sound.ToSoftObjectPath());
								}
							}
						}
					}
				};

				AddAssets(BattleAnimInst_CDO->AnimMontageAttack);

				for (auto Element : BattleAnimInst_CDO->AnimMontageSkillMap)
					AddAssets(Element.Value);

			}


		}
	}
}

void UResourceCacheManager::SetAssetResource_FromTables(TArray<FSoftObjectPath>& InAssets)
{
	UTableManager* TableManager = UTableManager::GetInstancePtr();
	if (TableManager)
	{
		TArray<FName> AbnormalRows = TableManager->GetAbnormalTableInfoRowNames();
		for (size_t i = 0; i < AbnormalRows.Num(); ++i)
		{
			FAbnormalStateGroupTableInfo_V2* tableinfo = TableManager->GetAbnormalStateGroupRow_V2(AbnormalRows[i]);
			if (tableinfo)
			{
				if (!tableinfo->Effect.IsNull())
					InAssets.AddUnique(tableinfo->Effect.ToSoftObjectPath());
				if (!tableinfo->EffectMaterial.IsNull())
					InAssets.AddUnique(tableinfo->EffectMaterial.ToSoftObjectPath());
			}
		}

		TArray<FName> CastingRows = TableManager->GetSkillCastingTableInfoRowNames();
		for (size_t i = 0; i < CastingRows.Num(); ++i)
		{
			FSkillCastingEffectTableInfo* tableinfo = TableManager->GetSkillCastingEffectRow(CastingRows[i]);
			if (tableinfo)
			{
				if (!tableinfo->SkillCastingDecal.IsNull())
					InAssets.AddUnique(tableinfo->SkillCastingDecal.ToSoftObjectPath());
				if (!tableinfo->ParticleSystem.IsNull())
					InAssets.AddUnique(tableinfo->ParticleSystem.ToSoftObjectPath());
			}
		}

		TArray<FName> MaterialRows = TableManager->GetResourceMaterialRowNames();
		for (size_t i = 0; i < MaterialRows.Num(); ++i)
		{
			FResourceMaterialTableInfo* tableinfo = TableManager->GetResourceMaterialRow(MaterialRows[i]);
			if (tableinfo)
			{
				if (!tableinfo->Asset.IsNull())
					InAssets.AddUnique(tableinfo->Asset.ToSoftObjectPath());
			}
		}

		TArray<FName> ParticleRows = TableManager->GetResourceParticleRowNames();
		for (size_t i = 0; i < ParticleRows.Num(); ++i)
		{
			FResourceParticleTableInfo* tableinfo = TableManager->GetResourceParticleRow(ParticleRows[i]);
			if (tableinfo)
			{
				if (!tableinfo->Asset.IsNull())
					InAssets.AddUnique(tableinfo->Asset.ToSoftObjectPath());
			}
		}

		TArray<FName> DamageFloaterTexRows = TableManager->GetResourceDamageFloater2DRowNames();
		for (size_t i = 0; i < DamageFloaterTexRows.Num(); ++i)
		{
			FResourceTexture2DTableInfo* tableinfo = TableManager->GetResourceDamageFloaterTex2DRow(DamageFloaterTexRows[i]);
			if (tableinfo)
			{
				if (!tableinfo->Asset.IsNull())
					InAssets.AddUnique(tableinfo->Asset.ToSoftObjectPath());
			}
		}

		TArray<FName> StaticMeshRows = TableManager->GetResourceStaticMeshRowNames();
		for (size_t i = 0; i < StaticMeshRows.Num(); ++i)
		{
			FResourceStaticMeshTableInfo* tableinfo = TableManager->GetResourceStaticMeshRow(StaticMeshRows[i]);
			if (tableinfo)
			{
				if (!tableinfo->Asset.IsNull())
					InAssets.AddUnique(tableinfo->Asset.ToSoftObjectPath());
			}
		}
	}
}

void UResourceCacheManager::RecursiveCacheResource(FStreamableDelegate Callback)
{
	TArray<FSoftObjectPath> TargetAssets;
	for (size_t i = 0; i < iPartition; ++i)
	{
		if (AssetsQueue.IsValidIndex(0))
		{
			FSoftObjectPath target = AssetsQueue.Pop();
			TargetAssets.AddUnique(target);
		}
	}
	if (TargetAssets.Num())
	{
		UUP_LoadingScene* LoadingScene = Cast<UUP_LoadingScene>(RGAMEINSTANCE(GEngine)->LoadingSceneManager->Widget);
		if (LoadingScene->IsValidLowLevel())
		{
			LoadingScene->CurrentPercent += LoadingScene->UnitPercent;
			FMath::Clamp<int32>(LoadingScene->CurrentPercent, 0, 100);

			float percent = LoadingScene->CurrentPercent / 100.f;
			LoadingScene->SetProgressBarPercent(percent);
		}
		URAsyncLoader::GetInstancePtr()->LoadAsyncResource(TargetAssets, FStreamableDelegate::CreateUObject(this, &UResourceCacheManager::RecursiveCacheResource , Callback));
	}
	else
	{
		OnAsyncLoadedCompleted.Unbind();
		OnAsyncLoadedCompleted.BindUObject(this, &UResourceCacheManager::Callback_AsyncLoaded);

		URAsyncLoader::GetInstancePtr()->LoadAsyncResource(AssetsQueue, Callback);
	}
}

void UResourceCacheManager::Callback_AsyncLoaded()
{
	for (size_t i = 0; i < AssetsQueue.Num(); ++i)
	{
		if (AssetsQueue[i].IsValid())
		{
			if (Cast<UClass>(AssetsQueue[i].ResolveObject()))
				CachedClassArray.AddUnique(Cast<UClass>(AssetsQueue[i].ResolveObject()));
			else if (AssetsQueue[i].ResolveObject())
				CachedObjectArray.AddUnique(AssetsQueue[i].ResolveObject());
			else
			{
				FString str = TEXT("Fail to load asset : ") + AssetsQueue[i].GetAssetName();
				UE_LOG(LogClass, Log, TEXT("%s"), *str);
			}
		}
	}

	UUIFunctionLibrary::ShowWaitingPopup(false);
	AssetsQueue.Empty();
}
