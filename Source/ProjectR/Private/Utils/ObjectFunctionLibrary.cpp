// Fill out your copyright notice in the Description page of Project Settings.

#include "ProjectR.h"
#include "ProjectRGameMode.h"

#include "ObjectFunctionLibrary.h"
#include "GlobalIntegrated.h"


#include "GameInfo/UserInfo.h"
#include "SharedUtils/Renewal_BattleFunctionLibrary.h"
#include "Network/HttpRequestClient.h"


#include "UtilFunctionIntegrated.h"

#include "GameInfo/RInventory.h"

// #include "Character/Interfaces/EntityBaseProperty.h"
#include "Character/BattleObject/BattleBaseObject.h"
#include "Character/Components/VibrationComponent.h"
#include "ContentGameMode/Controllers/StagePlayerController.h"
#ifdef WITH_EDITOR
	#include "Editor/SpawnerEditor.h"
#endif

int32	UObjectFunctionLibrary::MyGroupKey = GroupKey_SINGLE_NONE;
int32	UObjectFunctionLibrary::MyPartyID = PartyID_NONE;

#define	IMMUNE_MODIFY_NAME_THAN_ACTIVE2		FName(TEXT("90015"))
#define	IMMUNE_MODIFY_NAME_BONNIE_ACTIVE1	FName(TEXT("90007"))
#define	IMMUNE_MODIFY_NAME_YOSEPH_ACTIVE1	FName(TEXT("90001"))
#define	IMMUNE_MODIFY_NAME_YOSEPH_ACTIVE3	FName(TEXT("90002"))
#define	IMMUNE_MODIFY_NAME_MOMENT_ACTIVE3	FName(TEXT("90019"))
#define	IMMUNE_MODIFY_NAME_SOUL_STONE		FName(TEXT("120350"))


AActor* UObjectFunctionLibrary::SpawnCharacter(FTransform SpawnTransform, FName CharacterID, ECharacterCategoryTypeEnum Category, int32 UserKID, FString HeroUD, int32 InGroupKey, int32 InPartyID, int32 InPlayerUK, int32 Level /*= 1*/, int32 Grade /*= 1*/, bool IsAttachSkill /*= true*/, int32 InCommandSlotIndex /*= INVALID_INDEXNUM*/, bool Remote /*= false*/, bool bSpawnEffectFromCrewChange /*= false*/)
{
	URGameInstance* RGameInstance = RGAMEINSTANCE(GEngine);
	UWorld* World = GEngine->GetWorldFromContextObject(RGameInstance, EGetWorldErrorMode::Assert);


	if (!IsValid(RGameInstance->HttpClient) || !IsValid(RGameInstance->EventManager))
		return nullptr;
	bool bAddObject = false;

	AProjectRGameMode* pGameMode = Cast<AProjectRGameMode>(UGameplayStatics::GetGameMode(RGAMEINSTANCE(GEngine)));
	if (pGameMode)
	{
		bAddObject = true;
	}

	if (ECharacterCategoryTypeEnum::VE_Object == Category)
	{
		//checkf(false, TEXT("spawn ECharacterCategoryTypeEnum::VE_Object"));
		return SpawnObject(SpawnTransform, CharacterID, Category, UserKID, HeroUD, InGroupKey, InPartyID, InPlayerUK, Level, Grade, IsAttachSkill, InCommandSlotIndex, Remote);
	}
	
	FCharacterTableInfo* CharacterInfo = URenewal_BattleFunctionLibrary::GetCharacterTableInfo(Category, CharacterID);
	if (CharacterInfo == nullptr)
		return nullptr;

	FActorSpawnParameters SpawnParams;
	SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;

	UClass* SpawnedHeroClass = nullptr;
	if (RGameInstance->GameEnvironmentVar.LobbyHeroToManagement)
	{
		if (ECharacterCategoryTypeEnum::VE_UICharacter == Category)
		{
			SpawnedHeroClass = CharacterInfo->GetLobbyHeroBlueprintClass();
		}
	}
	if (!IsValid(SpawnedHeroClass))
	{
		SpawnedHeroClass = CharacterInfo->GetHeroBlueprintClass();
	}
	
	ABattleBaseCharacter* SpawnedCharacter = World->SpawnActor<ABattleBaseCharacter>(SpawnedHeroClass, SpawnTransform, SpawnParams);
	if (!IsValid(SpawnedCharacter))
		return nullptr;

	SpawnedCharacter->bSpawnedFromCrewChange = bSpawnEffectFromCrewChange;
	
	IEntityBaseProperty* BaseProperty = Cast<IEntityBaseProperty>(SpawnedCharacter);
	if (BaseProperty == nullptr)
		return nullptr;

	BaseProperty->SetCharacterID(CharacterID);
	BaseProperty->SetCharacterCategory(Category);
	BaseProperty->SetUserKID(UserKID);
	BaseProperty->SetHeroUD(HeroUD);
	BaseProperty->SetGroupKey(InGroupKey);
	BaseProperty->SetPartyID(InPartyID);
	BaseProperty->SetPlayerUK(InPlayerUK);
	BaseProperty->SetLevel(Level);
	BaseProperty->SetGrade(Grade);
	BaseProperty->SetJobClass(CharacterInfo->Class);
	BaseProperty->SetEntityType((Remote ? EEntityTypeEnum::VE_RemoteEntity : EEntityTypeEnum::VE_Normal));
	BaseProperty->SetAttachPropertyType(CharacterInfo->AttackPropertyType);
	BaseProperty->SetAccountCharacter(RGameInstance->HttpClient->IsValidKID(UserKID));

	USkillEntityComponent* SkillComp = nullptr;
	UBattleEntityComponent* BattleComp = nullptr;
	UEntityAIComponent* EntityAIComp = nullptr;
	UEntityRenderComponent* RenderComp = nullptr;

	switch (Category)
	{
	case ECharacterCategoryTypeEnum::VE_Hero:
	case ECharacterCategoryTypeEnum::VE_Minion:
	case ECharacterCategoryTypeEnum::VE_Npc:
	case ECharacterCategoryTypeEnum::VE_NeutralNpc:
	case ECharacterCategoryTypeEnum::VE_BattleObject:
	{
		if (bAddObject)
			pGameMode->ObjectHelper.AddObject(SpawnedCharacter, InPlayerUK);

		auto gigantComp = UCharacterHelpFunctionLibrary::GetGigantComponent(SpawnedCharacter);
		if (false == IsValid(gigantComp))
		{
			UCapsuleComponent* CapsuleComp = UCharacterHelpFunctionLibrary::GetCapsuleComponent(SpawnedCharacter);
			if (IsValid(CapsuleComp))
			{
				auto ScaledHeight = CapsuleComp->GetUnscaledCapsuleHalfHeight();
				if (CharacterInfo->ModelScale > 0.1f)
				{
					ScaledHeight *= CharacterInfo->ModelScale;
					SpawnTransform.SetScale3D(FVector(CharacterInfo->ModelScale, CharacterInfo->ModelScale, CharacterInfo->ModelScale));
				}

				FVector ResultPos;
				FVector CurVec = SpawnedCharacter->GetActorLocation();
				CurVec = SpawnTransform.GetLocation();
 				if (UMapFunctionLibrary::GetProjectionToFloorHitBlockingVolume(RGameInstance, SpawnTransform.GetLocation(), ResultPos, SpawnedCharacter) == false)
 					return nullptr;

				ResultPos.Z = ResultPos.Z + ScaledHeight;
				SpawnTransform.SetLocation(ResultPos);
			}
		}
		SpawnedCharacter->SetActorTransform(SpawnTransform);
		BaseProperty->InitHeroAfterCreation();

		SkillComp = UCharacterHelpFunctionLibrary::GetSkillEntityComponent(SpawnedCharacter);
		BattleComp = UCharacterHelpFunctionLibrary::GetBattleEntityComponent(SpawnedCharacter);
		EntityAIComp = UCharacterHelpFunctionLibrary::GetEntityAIComponent(SpawnedCharacter);
		RenderComp = UCharacterHelpFunctionLibrary::GetEntityRenderComponent(SpawnedCharacter);
		if (!IsValid(SkillComp) || !IsValid(BattleComp) || !IsValid(EntityAIComp) || !IsValid(RenderComp))
			break;

		SkillComp->SetAttachSkill(IsAttachSkill);
		BattleComp->SetCommandSlotIndex(InCommandSlotIndex);
		EntityAIComp->InitAIFSM();

		if (ECharacterCategoryTypeEnum::VE_Hero == Category)
			EntityAIComp->SetPause(true);

		RenderComp->SetVisible(false);
		BaseProperty->AttachUI();

		if (ECharacterCategoryTypeEnum::VE_Hero == Category)
		{
			EGamePlayModeTypeEnum ContentType = RGameInstance->CurrentGameMode;

			if (EGamePlayModeTypeEnum::VE_Campaign == ContentType ||
				EGamePlayModeTypeEnum::VE_Colosseum == ContentType ||
				EGamePlayModeTypeEnum::VE_MirrorDungeon == ContentType ||
				EGamePlayModeTypeEnum::VE_RaidKillBoss == ContentType ||
				EGamePlayModeTypeEnum::VE_HeroEpisode == ContentType ||
				EGamePlayModeTypeEnum::VE_WeekDungeon == ContentType)
			{
				if (RGameInstance->UserInfo)
					RGameInstance->UserInfo->ApplyHeroConfig(ContentType, HeroUD, BattleComp, SkillComp);
			}
		}

		SkillComp->TryToAttachSkills_V2();

		EquipItemToObject(SpawnedCharacter);

		// SetReady
		BaseProperty->SetSpawnEvent();
		bool bAllObjectVisible = false;
	
// 		if(pGamePlayMode)
// 			bAllObjectVisible = pGamePlayMode->GetAllObjectVisible();
		if (pGameMode)
			bAllObjectVisible = pGameMode->ObjectHelper.GetAllObjectVisible();
		if (bAllObjectVisible)
		{
			RenderComp->SetVisible(true);
		}

		BaseProperty->InitStartProperty();
		SetSoundAndVibration(SpawnedCharacter, Category);
		

	}break;
	case ECharacterCategoryTypeEnum::VE_UICharacter:
	{
		// Add Object
// 		if (bAddObject)
// 			pGamePlayMode->AddObject(SpawnedCharacter, InPlayerUK);
		if(bAddObject)
			pGameMode->ObjectHelper.AddObject(SpawnedCharacter, InPlayerUK);
// 		AddObject(SpawnedCharacter, InPlayerUK);

		// Set Transform
		FVector ResultPos = SpawnTransform.GetLocation();
		UCapsuleComponent* CapsuleComp = UCharacterHelpFunctionLibrary::GetCapsuleComponent(SpawnedCharacter);
		if (IsValid(CapsuleComp))
		{
			ResultPos = URenewal_BattleFunctionLibrary::GetFloorPosition(World, SpawnTransform.GetLocation(), CapsuleComp->GetScaledCapsuleHalfHeight(), SpawnedCharacter);
			ResultPos.Z = ResultPos.Z + CapsuleComp->GetScaledCapsuleHalfHeight();
			SpawnTransform.SetLocation(ResultPos);
		}
		
		SpawnedCharacter->SetActorTransform(SpawnTransform);

		// Add Component
		BaseProperty->InitHeroAfterCreation(); //del

		EquipItemToObject(SpawnedCharacter); //
		
		/*if (CharacterInfo->ModelScale > 0.1f)
		{
			SpawnedCharacter->SetActorScale3D(FVector(CharacterInfo->ModelScale, CharacterInfo->ModelScale, CharacterInfo->ModelScale));
		}*/

		UCharacterMovementComponent* MovementComp = UCharacterHelpFunctionLibrary::GetCharacterMovementComponent(SpawnedCharacter);
		if (IsValid(MovementComp))
		{
			MovementComp->DisableMovement();
		}

		BaseProperty->InitStartProperty();

		// SetReady
		BaseProperty->SetReady(true);

	}break;
	}

	if (Category == ECharacterCategoryTypeEnum::VE_BattleObject)
	{
		UCharacterMovementComponent* MovementComp = UCharacterHelpFunctionLibrary::GetCharacterMovementComponent(SpawnedCharacter);
		if (IsValid(MovementComp))
			MovementComp->bOrientRotationToMovement = false;
	}
	
// 	RGameInstance->EventManager->OnSpawnActor.Broadcast(SpawnedCharacter);

	return SpawnedCharacter;
}

AActor* UObjectFunctionLibrary::SpawnCharacterForArena(FTransform SpawnTransform, FName CharacterID, ECharacterCategoryTypeEnum Category, int32 UserKID, FString HeroUD, int32 InGroupKey, int32 InPartyID, int32 InPlayerUK, int32 Level, int32 Grade, int32 InCommandSlotIndex)
{
	URGameInstance* RGameInstance = RGAMEINSTANCE(GEngine);
// 	if (!IsValid(RGameInstance))
// 		return nullptr;
// 	if (!IsValid(RGameInstance->GetWorld()))
// 		return nullptr;
	if (!IsValid(RGameInstance->HttpClient) || !IsValid(RGameInstance->EventManager))
		return nullptr;
	bool bAddObject = false;
	AProjectRGameMode* pGameMode = Cast<AProjectRGameMode>(UGameplayStatics::GetGameMode(RGAMEINSTANCE(GEngine)));
	if (pGameMode)
		bAddObject = true;

	FCharacterTableInfo* CharacterInfo = URenewal_BattleFunctionLibrary::GetCharacterTableInfo(Category, CharacterID);
	if (CharacterInfo == nullptr)
		return nullptr;

	FActorSpawnParameters SpawnParams;
	SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

	ABattleBaseCharacter* SpawnedCharacter = RGameInstance->GetWorld()->SpawnActor<ABattleBaseCharacter>(CharacterInfo->GetHeroBlueprintClass(), SpawnTransform, SpawnParams);
	if (!IsValid(SpawnedCharacter))
		return nullptr;

	IEntityBaseProperty* BaseProperty = Cast<IEntityBaseProperty>(SpawnedCharacter);
	if (BaseProperty == nullptr)
		return nullptr;
	
	USkeletalMeshComponent* SkeletalMeshComponent = SpawnedCharacter->GetMesh();
	if (CharacterInfo->Material0.IsValid())
		SkeletalMeshComponent->SetMaterial(0, CharacterInfo->Material0.Get());

	if (CharacterInfo->Material1.IsValid())
		SkeletalMeshComponent->SetMaterial(1, CharacterInfo->Material1.Get());

	if (CharacterInfo->Material2.IsValid())
		SkeletalMeshComponent->SetMaterial(2, CharacterInfo->Material2.Get());

	if (CharacterInfo->Material3.IsValid())
		SkeletalMeshComponent->SetMaterial(3, CharacterInfo->Material3.Get());

	// IEntityBaseProperty
	BaseProperty->SetCharacterID(CharacterID);
	BaseProperty->SetCharacterCategory(Category);
	BaseProperty->SetUserKID(UserKID);
	BaseProperty->SetHeroUD(HeroUD);
	BaseProperty->SetGroupKey(InGroupKey);
	BaseProperty->SetPartyID(InPartyID);
	BaseProperty->SetPlayerUK(InPlayerUK);
	BaseProperty->SetLevel(Level);
	BaseProperty->SetGrade(Grade);
	BaseProperty->SetJobClass(CharacterInfo->Class);
	BaseProperty->SetEntityType(EEntityTypeEnum::VE_Normal);
	BaseProperty->SetAttachPropertyType(CharacterInfo->AttackPropertyType);

	BaseProperty->SetAccountCharacter(RGameInstance->HttpClient->IsValidKID(UserKID));

	if(bAddObject)
		pGameMode->ObjectHelper.AddObject(SpawnedCharacter, InPlayerUK);

	// Set Transform
	auto gigantComp = UCharacterHelpFunctionLibrary::GetGigantComponent(SpawnedCharacter);
	if (false == IsValid(gigantComp))
	{
		UCapsuleComponent* CapsuleComp = UCharacterHelpFunctionLibrary::GetCapsuleComponent(SpawnedCharacter);
		if (IsValid(CapsuleComp))
		{
			auto ScaledHeight = CapsuleComp->GetUnscaledCapsuleHalfHeight();
			if (CharacterInfo->ModelScale > 0.1f)
			{
				ScaledHeight *= CharacterInfo->ModelScale;
				SpawnTransform.SetScale3D(FVector(CharacterInfo->ModelScale, CharacterInfo->ModelScale, CharacterInfo->ModelScale));
			}

			FVector ResultPos;
			if (UMapFunctionLibrary::GetProjectionToFloorHitBlockingVolume(RGameInstance, SpawnTransform.GetLocation(), ResultPos) == false)
				return nullptr;

			ResultPos.Z = ResultPos.Z + ScaledHeight;

			SpawnTransform.SetLocation(ResultPos);
		}
	}
	SpawnedCharacter->SetActorTransform(SpawnTransform);
	// Add Component
	BaseProperty->InitHeroAfterCreation();
	USkillEntityComponent* SkillComp = UCharacterHelpFunctionLibrary::GetSkillEntityComponent(SpawnedCharacter);
	UBattleEntityComponent* BattleComp = UCharacterHelpFunctionLibrary::GetBattleEntityComponent(SpawnedCharacter);
	UEntityAIComponent* EntityAIComp = UCharacterHelpFunctionLibrary::GetEntityAIComponent(SpawnedCharacter);
	if (!IsValid(SkillComp) || !IsValid(BattleComp) || !IsValid(EntityAIComp))
		return nullptr;
	SkillComp->SetAttachSkill(true);
	BattleComp->SetCommandSlotIndex(InCommandSlotIndex);
	EntityAIComp->InitAIFSM();
	BaseProperty->AttachUI();
	if (ECharacterCategoryTypeEnum::VE_Hero == Category)
	{
		EGamePlayModeTypeEnum ContentType = RGameInstance->CurrentGameMode;

		if (EGamePlayModeTypeEnum::VE_MirrorDungeon == ContentType)
		{
			if (RGameInstance->UserInfo)
			{
				RGameInstance->UserInfo->ApplyHeroConfig(ContentType, HeroUD, BattleComp, SkillComp);
			}
		}
		else if (EGamePlayModeTypeEnum::VE_Arena == ContentType)
		{
			TArray<bool> SkillAutoOn;
			SkillAutoOn.Init(true, 3);
			SkillComp->SetSkillAutoModeOnOff(SkillAutoOn);
		}
	}

	SkillComp->TryToAttachSkills_V2();

	EquipItemToObject(SpawnedCharacter);

	UBattleBaseAnimInstance* BattleBaseAnimInstance = UCharacterHelpFunctionLibrary::GetCharacterBattleBaseAnimInstance(SpawnedCharacter);
	if (IsValid(BattleBaseAnimInstance))
	{
		BattleBaseAnimInstance->SetBasicIdleAnimSequence();
	}
	BaseProperty->InitStartProperty();

	UUtilFunctionLibrary::PlaySoundSceneComponent(TEXT("HeroSpawn"), SpawnedCharacter->GetRootComponent());

	if (Category == ECharacterCategoryTypeEnum::VE_BattleObject)
	{
		UCharacterMovementComponent* MovementComp = UCharacterHelpFunctionLibrary::GetCharacterMovementComponent(SpawnedCharacter);
		if (IsValid(MovementComp))
			MovementComp->bOrientRotationToMovement = false;
	}

	return SpawnedCharacter;
}

AActor* UObjectFunctionLibrary::SpawnCharacterWithRTS(FTransform SpawnTransform, FName CharacterID, ECharacterCategoryTypeEnum Category, int32 UserKID, FString HeroUD, int32 InGroupKey, int32 InPartyID, int32 InPlayerUK, int32 Level /*= 1*/, int32 Grade /*= 1*/, bool IsAttachSkill /*= true*/, int32 InCommandSlotIndex /*= INVALID_INDEXNUM*/, bool Remote /*= false*/, bool bSpawnEffectFromCrewChange /*= false*/)
{
	URGameInstance* RGameInstance = RGAMEINSTANCE(GEngine);
	UWorld* World = GEngine->GetWorldFromContextObject(RGameInstance, EGetWorldErrorMode::Assert);
	AProjectRGameMode* pGameMode = Cast<AProjectRGameMode>(UGameplayStatics::GetGameMode(RGAMEINSTANCE(GEngine)));
	if (!IsValid(pGameMode))
		return nullptr;
// 	if (!IsValid(RGameInstance))
// 		return nullptr;
// 	if (!IsValid(World))
// 		return nullptr;
	if (!IsValid(RGameInstance->HttpClient))
		return nullptr;
	if (ECharacterCategoryTypeEnum::VE_Object == Category)
	{
		return SpawnObject(SpawnTransform, CharacterID, Category, UserKID, HeroUD, InGroupKey, InPartyID, InPlayerUK, Level, Grade, IsAttachSkill, InCommandSlotIndex, Remote);
	}

	FCharacterTableInfo* CharacterInfo = URenewal_BattleFunctionLibrary::GetCharacterTableInfo(Category, CharacterID);
	if (CharacterInfo == nullptr)
		return nullptr;
	
	UClass* SpawnedHeroClass = CharacterInfo->GetHeroBlueprintClass();
	if (!IsValid(SpawnedHeroClass))
		return nullptr;
	
	FActorSpawnParameters SpawnParams;
	SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;

	ABattleBaseCharacter* SpawnedCharacter = World->SpawnActor<ABattleBaseCharacter>(SpawnedHeroClass, SpawnTransform, SpawnParams);
	if (!IsValid(SpawnedCharacter))
		return nullptr;

	SpawnedCharacter->bSpawnedFromCrewChange = bSpawnEffectFromCrewChange;

	IEntityBaseProperty* BaseProperty = Cast<IEntityBaseProperty>(SpawnedCharacter);
	if (BaseProperty == nullptr)
		return nullptr;

	BaseProperty->SetCharacterID(CharacterID);
	BaseProperty->SetCharacterCategory(Category);
	BaseProperty->SetUserKID(UserKID);
	BaseProperty->SetHeroUD(HeroUD);
	BaseProperty->SetGroupKey(InGroupKey);
	BaseProperty->SetPartyID(InPartyID);
	BaseProperty->SetPlayerUK(InPlayerUK);
	BaseProperty->SetLevel(Level);
	BaseProperty->SetGrade(Grade);
	BaseProperty->SetJobClass(CharacterInfo->Class);
	if (Remote)
		BaseProperty->SetEntityType(EEntityTypeEnum::VE_RemoteEntity );
	else
		BaseProperty->SetEntityType(EEntityTypeEnum::VE_Normal);
	
	BaseProperty->SetAttachPropertyType(CharacterInfo->AttackPropertyType);
	BaseProperty->SetAccountCharacter(RGameInstance->HttpClient->IsValidKID(UserKID));

	USkillEntityComponent* SkillComp = nullptr;
	UBattleEntityComponent* BattleComp = nullptr;
	UEntityAIComponent* EntityAIComp = nullptr;
	UEntityRenderComponent* RenderComp = nullptr;

	switch (Category)
	{
		case ECharacterCategoryTypeEnum::VE_Hero:
		case ECharacterCategoryTypeEnum::VE_Minion:
		case ECharacterCategoryTypeEnum::VE_Npc:
		case ECharacterCategoryTypeEnum::VE_NeutralNpc:
		case ECharacterCategoryTypeEnum::VE_BattleObject:
		{
			pGameMode->ObjectHelper.AddObject(SpawnedCharacter, InPlayerUK);

			auto gigantComp = UCharacterHelpFunctionLibrary::GetGigantComponent(SpawnedCharacter);
			if (!IsValid(gigantComp))
			{
				UCapsuleComponent* CapsuleComp = UCharacterHelpFunctionLibrary::GetCapsuleComponent(SpawnedCharacter);
				if (IsValid(CapsuleComp))
				{
					auto ScaledHeight = CapsuleComp->GetUnscaledCapsuleHalfHeight();
					if (CharacterInfo->ModelScale > 0.1f)
					{
						ScaledHeight *= CharacterInfo->ModelScale;
						SpawnTransform.SetScale3D(FVector(CharacterInfo->ModelScale, CharacterInfo->ModelScale, CharacterInfo->ModelScale));
					}

					FVector ResultPos;
					FVector CurVec = SpawnedCharacter->GetActorLocation();
					CurVec = SpawnTransform.GetLocation();
					if (UMapFunctionLibrary::GetProjectionToFloorHitBlockingVolume(RGameInstance, SpawnTransform.GetLocation(), ResultPos, SpawnedCharacter) == false)
						return nullptr;

					ResultPos.Z = ResultPos.Z + ScaledHeight;
					SpawnTransform.SetLocation(ResultPos);
				}
			}

			SpawnedCharacter->SetActorTransform(SpawnTransform);
			BaseProperty->InitHeroAfterCreation();
			SkillComp = UCharacterHelpFunctionLibrary::GetSkillEntityComponent(SpawnedCharacter);
			BattleComp = UCharacterHelpFunctionLibrary::GetBattleEntityComponent(SpawnedCharacter);
			EntityAIComp = UCharacterHelpFunctionLibrary::GetEntityAIComponent(SpawnedCharacter);
			RenderComp = UCharacterHelpFunctionLibrary::GetEntityRenderComponent(SpawnedCharacter);
			if (!IsValid(SkillComp) || !IsValid(BattleComp) || !IsValid(EntityAIComp) || !IsValid(RenderComp))
				break;
			SkillComp->SetAttachSkill(IsAttachSkill);
			BattleComp->SetCommandSlotIndex(InCommandSlotIndex);
			EntityAIComp->InitAIFSM();
			RenderComp->SetVisible(false);
			BaseProperty->AttachUI();
			SkillComp->TryToAttachSkills_V2();

			if (ECharacterCategoryTypeEnum::VE_Hero == Category)
			{
				EntityAIComp->AIStop();
				EGamePlayModeTypeEnum ContentType = RGameInstance->CurrentGameMode;
				if (RGameInstance->UserInfo)
					RGameInstance->UserInfo->ApplyHeroConfig(ContentType, HeroUD, BattleComp, SkillComp);
			}
			

			EquipItemToObject(SpawnedCharacter);

			// SetReady
			BaseProperty->SetSpawnEvent();
			bool bAllObjectVisible = false;

			bAllObjectVisible = pGameMode->ObjectHelper.GetAllObjectVisible();

			RenderComp->SetVisible(bAllObjectVisible);
			
			BaseProperty->InitStartProperty();
			SetSoundAndVibration(SpawnedCharacter, Category);
			break;
		}
		case ECharacterCategoryTypeEnum::VE_UICharacter:
		{
			pGameMode->ObjectHelper.AddObject(SpawnedCharacter, InPlayerUK);
	
			FVector ResultPos = SpawnTransform.GetLocation();
			UCapsuleComponent* CapsuleComp = UCharacterHelpFunctionLibrary::GetCapsuleComponent(SpawnedCharacter);
			if (IsValid(CapsuleComp))
			{
				ResultPos = URenewal_BattleFunctionLibrary::GetFloorPosition(World, SpawnTransform.GetLocation(), CapsuleComp->GetScaledCapsuleHalfHeight(), SpawnedCharacter);
				ResultPos.Z = ResultPos.Z + CapsuleComp->GetScaledCapsuleHalfHeight();
				SpawnTransform.SetLocation(ResultPos);
			}
			SpawnedCharacter->SetActorTransform(SpawnTransform);
			// Add Component
			BaseProperty->InitHeroAfterCreation(); 
			EquipItemToObject(SpawnedCharacter);
			UCharacterMovementComponent* MovementComp = UCharacterHelpFunctionLibrary::GetCharacterMovementComponent(SpawnedCharacter);
			if (IsValid(MovementComp))
				MovementComp->DisableMovement();
			BaseProperty->InitStartProperty();
			// SetReady
			BaseProperty->SetReady(true);
			break;
		}
	}

	if (Category == ECharacterCategoryTypeEnum::VE_BattleObject)
	{
		UCharacterMovementComponent* MovementComp = UCharacterHelpFunctionLibrary::GetCharacterMovementComponent(SpawnedCharacter);
		if (IsValid(MovementComp))
			MovementComp->bOrientRotationToMovement = false;
	}

	return SpawnedCharacter;
}

void UObjectFunctionLibrary::SetSoundAndVibration(AActor* InSpawnedCharacter, ECharacterCategoryTypeEnum InType)
{
	switch (InType)
	{
	case ECharacterCategoryTypeEnum::VE_Hero:
	{
		UUtilFunctionLibrary::PlaySoundSceneComponent(TEXT("HeroSpawn"), InSpawnedCharacter->GetRootComponent());
		UVibrationComponent* Vibration = UCharacterHelpFunctionLibrary::GetVibrationComponent(InSpawnedCharacter);
		if (Vibration)
			Vibration->IsActivateComponent = false;

	}
	break;

	case ECharacterCategoryTypeEnum::VE_Npc:
	case ECharacterCategoryTypeEnum::VE_BattleObject:
	{

		/*
		Test mass spawn friend npc, 11-6
		Disable RVO.
		*/

		// If friend npc is spawned, disable RVO.
		if (URenewal_BattleFunctionLibrary::IsEnemyAgainstCurrentPlayer(InSpawnedCharacter) == false)
		{
			UCharacterMovementComponent* MovementComp = UCharacterHelpFunctionLibrary::GetCharacterMovementComponent(InSpawnedCharacter);
			if (IsValid(MovementComp))
			{
				MovementComp->bUseRVOAvoidance = false;
			}
		}

	}
	// !!!Skip break; because above code is for npc only. but the below code is needed for both npc & minion.
	case ECharacterCategoryTypeEnum::VE_Minion:
	{
		UUtilFunctionLibrary::PlaySoundSceneComponent(TEXT("MinionSpawn"), InSpawnedCharacter->GetRootComponent());
		UVibrationComponent* Vibration = UCharacterHelpFunctionLibrary::GetVibrationComponent(InSpawnedCharacter);
		if (Vibration)
			Vibration->IsActivateComponent = true;
	}
	break;
	case ECharacterCategoryTypeEnum::VE_NeutralNpc:
	{
		UCharacterMovementComponent* MovementComp = UCharacterHelpFunctionLibrary::GetCharacterMovementComponent(InSpawnedCharacter);
		if (IsValid(MovementComp))
		{
			MovementComp->DisableMovement();
			MovementComp->bUseRVOAvoidance = false;
		}
	}
	break;
	}
}

AActor* UObjectFunctionLibrary::SpawnObject(FTransform SpawnTransform, FName CharacterID, ECharacterCategoryTypeEnum Category, int32 UserKID, FString HeroUD, int32 InGroupKey, int32 InPartyID, int32 InPlayerUK, int32 Level /*= 1*/, int32 Grade /*= 1*/, bool IsAttachSkill /*= true*/, int32 InCommandSlotIndex /*= INVALID_INDEXNUM*/, bool Remote /*= false*/)
{
	URGameInstance* RGameInstance = RGAMEINSTANCE(GEngine);
// 	if (!IsValid(RGameInstance))
// 		return nullptr;

	if (!IsValid(RGameInstance->GetWorld()))
		return nullptr;

	if (!IsValid(RGameInstance->HttpClient) || !IsValid(RGameInstance->EventManager))
		return nullptr;

	if (ECharacterCategoryTypeEnum::VE_Object != Category)
		return nullptr;

	FBattleObjectTableInfo* BattleObjectInfo = URenewal_BattleFunctionLibrary::GetBattleObjectTableInfo(CharacterID);
	if (BattleObjectInfo == nullptr)
		return nullptr;

	FActorSpawnParameters SpawnParams;
	SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;

	AActor* SpawnedObject = RGameInstance->GetWorld()->SpawnActor<ABattleBaseObject>(BattleObjectInfo->GetModelClass(), SpawnParams);
	if (!IsValid(SpawnedObject))
		return nullptr;

	IEntityBaseProperty* BaseProperty = Cast<IEntityBaseProperty>(SpawnedObject);
	if (BaseProperty == nullptr)
		return nullptr;

	bool bAddObject = false;
// 	UIGamePlayMode*		pGamePlayMode = UUtilFunctionLibrary::GetGamePlayMode();
// 	if (IsValid(pGamePlayMode))
// 		bAddObject = true;
	AProjectRGameMode* pGameMode = Cast<AProjectRGameMode>(UGameplayStatics::GetGameMode(RGAMEINSTANCE(GEngine)));
	if (pGameMode)
	{
		bAddObject = true;
	}
	
	// Set Transform
	FVector ResultPos;
	if (UMapFunctionLibrary::GetProjectionToFloorHitBlockingVolume(RGameInstance,SpawnTransform.GetLocation(), ResultPos) == false)
		return nullptr;

	UCapsuleComponent* CapsuleComp = UCharacterHelpFunctionLibrary::GetCapsuleComponent(SpawnedObject);
	if (IsValid(CapsuleComp))
	{
		ResultPos.Z = ResultPos.Z + CapsuleComp->GetScaledCapsuleHalfHeight();
	}
	SpawnTransform.SetLocation(ResultPos);
	SpawnedObject->SetActorTransform(SpawnTransform);

	// IEntityBaseProperty
	BaseProperty->SetCharacterID(CharacterID);
	BaseProperty->SetCharacterCategory(Category);
	BaseProperty->SetUserKID(UserKID);
	BaseProperty->SetHeroUD(HeroUD);
	BaseProperty->SetGroupKey(InGroupKey);
	BaseProperty->SetPartyID(InPartyID);
	BaseProperty->SetPlayerUK(InPlayerUK);
	BaseProperty->SetLevel(Level);
	BaseProperty->SetGrade(Grade);
	BaseProperty->SetEntityType((Remote ? EEntityTypeEnum::VE_RemoteEntity : EEntityTypeEnum::VE_Normal));
	BaseProperty->SetAttachPropertyType(ECharacterPropertyTypeEnum::VE_PhysicalAttackPower);

	BaseProperty->SetAccountCharacter(RGameInstance->HttpClient->IsValidKID(UserKID));
// 	if (BaseProperty->GetAccountCharacter())
// 	{
// 		SetMyGroupKey(InGroupKey);
// 		SetMyPartyID(InPartyID);
// 	}

	// Add Object
// 	if(bAddObject)
// 		pGamePlayMode->AddObject(SpawnedObject, InPlayerUK);
	if(bAddObject)
		pGameMode->ObjectHelper.AddObject(SpawnedObject, InPlayerUK);
// 	AddObject(SpawnedObject, InPlayerUK);

	// Add Component
	BaseProperty->InitHeroAfterCreation();

	USkillEntityComponent* SkillComp = UCharacterHelpFunctionLibrary::GetSkillEntityComponent(SpawnedObject);
	if (IsValid(SkillComp))
	{
		SkillComp->SetAttachSkill(IsAttachSkill);
	}
	UEntityAIComponent* EntityAIComp = UCharacterHelpFunctionLibrary::GetEntityAIComponent(SpawnedObject);
	if (IsValid(EntityAIComp))
	{
		EntityAIComp->InitAIFSM();
	}
		
	BaseProperty->AttachUI();

	if (IsValid(SkillComp))
	{
		SkillComp->TryToAttachSkills_V2();
	}

	UCharacterMovementComponent* MovementComp = UCharacterHelpFunctionLibrary::GetCharacterMovementComponent(SpawnedObject);
	if (IsValid(MovementComp))
	{
		MovementComp->DisableMovement();
	}

	BaseProperty->InitStartProperty();

	// SetReady
	BaseProperty->SetReady(true);

// 	RGameInstance->EventManager->OnSpawnActor.Broadcast(SpawnedObject);

	return SpawnedObject;
}

void UObjectFunctionLibrary::EquipItemToObject(AActor* Obj)
{
	if (!IsValid(Obj))
	{
		return;
	}
	URGameInstance* RGameInstance = RGAMEINSTANCE(GEngine);
// 	if (!IsValid(RGameInstance))
// 	{
// 		return;
// 	}

	IEntityBaseProperty* BaseProperty = Cast<IEntityBaseProperty>(Obj);
	if (!BaseProperty)
		return;

	if (BaseProperty->GetCharacterCategory() != ECharacterCategoryTypeEnum::VE_Hero && BaseProperty->GetCharacterCategory() != ECharacterCategoryTypeEnum::VE_UICharacter)
		return;

	if (BaseProperty->GetAccountCharacter())
	{
		FHERO* HeroItemData = RGameInstance->RInventory->GetHeroWithUD(BaseProperty->GetHeroUD());
		if (HeroItemData != nullptr)
		{
			UBattleEntityComponent* BattleComp = UCharacterHelpFunctionLibrary::GetBattleEntityComponent(Obj);
			if (IsValid(BattleComp))
			{
				for (int32 i = (int32)EItemEquipPositionEnum::VE_Weapon; i < (int32)EItemEquipPositionEnum::VE_Max; ++i)
				{
					if (HeroItemData->equipedItemUDs.IsValidIndex(i))
						BattleComp->EquipItem((EItemEquipPositionEnum)i, HeroItemData->equipedItemUDs[i]);
				}
			}
		}
		else
		{
			HeroItemData = RGameInstance->RInventory->GetHeroWithID(BaseProperty->GetCharacterID());
			if (HeroItemData != nullptr)
			{
				UBattleEntityComponent* BattleComp = UCharacterHelpFunctionLibrary::GetBattleEntityComponent(Obj);
				if (IsValid(BattleComp))
				{
					for (int32 i = (int32)EItemEquipPositionEnum::VE_Weapon; i < (int32)EItemEquipPositionEnum::VE_Max; ++i)
					{
						if (HeroItemData->equipedItemUDs.IsValidIndex(i))
							BattleComp->EquipItem((EItemEquipPositionEnum)i, HeroItemData->equipedItemUDs[i]);
					}
				}
			}
		}
	}
	else
	{
		FEQUIPED_HERO OtherHeroData;
		if (RGameInstance->OtherHeroManager->GetOtherHeroData(BaseProperty->GetUserKID(), BaseProperty->GetHeroUD(), OtherHeroData))
		{
			UBattleEntityComponent* BattleComp = UCharacterHelpFunctionLibrary::GetBattleEntityComponent(Obj);
			if (IsValid(BattleComp))
			{
				for (int32 i = (int32)EItemEquipPositionEnum::VE_Weapon; i < (int32)EItemEquipPositionEnum::VE_Max; ++i)
				{
					if (OtherHeroData.equipedItems.IsValidIndex(i))
						BattleComp->EquipItemFITEM((EItemEquipPositionEnum)i, OtherHeroData.equipedItems[i]);
				}
			}
		}
	}

#if WITH_EDITOR
	if (IsValid(RGameInstance->EditorManager))
	{
		if (IsValid(RGameInstance->EditorManager->GetEditorActor()))
		{
			ASpawnerEditor* Editor = Cast<ASpawnerEditor>(RGameInstance->EditorManager->GetEditorActor());
			if (IsValid(Editor))
			{
				Editor->HeroEquipItemToObject(Obj);
			}
		}
	}
#endif
}

void UObjectFunctionLibrary::SetMyTeamKey(int32 InGrouopKey)
{
	MyGroupKey = InGrouopKey;
}

int32 UObjectFunctionLibrary::GetMyTeamKey()
{
	return MyGroupKey;
}

void UObjectFunctionLibrary::SetMyPartyID(int32 InPartyID)
{
	MyPartyID = InPartyID;
}

int32 UObjectFunctionLibrary::GetMyPartyID()
{
	return MyPartyID;
}

ESpawnResultTypeEnum UObjectFunctionLibrary::CanSpawnHero(int32 SlotIndex)
{
	if (SlotIndex == ROLE_CREW1_BATTLE_INDEX)
		return ESpawnResultTypeEnum::VE_Success;
	if (SlotIndex == ROLE_CREW1_REST_INDEX)
		return ESpawnResultTypeEnum::VE_Fail;
	if (SlotIndex == ROLE_CREW2_BATTLE_INDEX)
		return ESpawnResultTypeEnum::VE_Success;
	if (SlotIndex == ROLE_CREW2_REST_INDEX)
		return ESpawnResultTypeEnum::VE_Fail;
	return ESpawnResultTypeEnum::VE_Fail;
}

bool UObjectFunctionLibrary::IsAbleModifyActor(AActor* InActor)
{
	if (!InActor)
		return false;

	UBattleEntityComponent* pBattleComp = UCharacterHelpFunctionLibrary::GetBattleEntityComponent(InActor);
	if (pBattleComp && pBattleComp->IsBoss())
	{
		// 보스변경시키면 스테이지가 꼬이는 현상 때문에 modification 면역으로 처리
		return false;
	}

	IEntityBaseProperty* EntityProperty = Cast< IEntityBaseProperty >(InActor);
	if (!EntityProperty)
		return false;

	FName id = EntityProperty->GetCharacterID();
	// modify 면역되어야 하는 npc id 하드코딩
	if (id == IMMUNE_MODIFY_NAME_THAN_ACTIVE2
		|| id == IMMUNE_MODIFY_NAME_BONNIE_ACTIVE1
		|| id == IMMUNE_MODIFY_NAME_YOSEPH_ACTIVE1
		|| id == IMMUNE_MODIFY_NAME_YOSEPH_ACTIVE3
		|| id == IMMUNE_MODIFY_NAME_MOMENT_ACTIVE3
		|| id == IMMUNE_MODIFY_NAME_SOUL_STONE
		)
	{
		return false;
	}

	return true;
}

void UObjectFunctionLibrary::ModifyActor(AActor* From, bool bApply)
{
	AActor* Owner = From;
	if (!Owner)
		return;

	if (!IsAbleModifyActor(Owner))
		return;

	UBattleEntityComponent* pBattleComp = UCharacterHelpFunctionLibrary::GetBattleEntityComponent(Owner);
	AActor* SpawnedCharacter = nullptr;
	FTransform SpawnTransform = Owner->GetActorTransform();

	ECharacterCategoryTypeEnum eCategoryType;// = EntityProperty->GetCharacterCategory();
	int32 UserKID;//= EntityProperty->GetUserKID();
	FString HeroUD;//= EntityProperty->GetHeroUD();
	int32 GroupKey;//= EntityProperty->GetGroupKey();
	int32 PartyID;//= EntityProperty->GetPartyID();
	int32 PlayerUK;//= EntityProperty->GetPlayerUK();
	int32 Level;//= EntityProperty->GetLevel();
	int32 Grade;//= EntityProperty->GetGrade();
	float Hp;
	float MaxHp;
	//int32 MaxHp;
	FName CharacterID;// = FName("KokoA");
	int32 Armor;
	int32 MagicResistance;

	IEntityBaseProperty* EntityProperty = Cast< IEntityBaseProperty >(Owner);
	UCharacterBattleInfo* pBattleInfo = UCharacterHelpFunctionLibrary::GetBattleInfo(Owner);
	USkillEntityComponent*	pSkillComp = UCharacterHelpFunctionLibrary::GetSkillEntityComponent(Owner);
	if (EntityProperty && pBattleComp && pBattleInfo /*&& pSkillComp*/)
	{
		eCategoryType = EntityProperty->GetCharacterCategory();
		UserKID = EntityProperty->GetUserKID();
		HeroUD = EntityProperty->GetHeroUD();
		GroupKey = EntityProperty->GetGroupKey();
		PartyID = EntityProperty->GetPartyID();
		PlayerUK = EntityProperty->GetPlayerUK();
		Level = EntityProperty->GetLevel();
		Grade = EntityProperty->GetGrade();
		MaxHp = pBattleInfo->CharacterProperty.GetCharacterProperty<float>(ECharacterPropertyTypeEnum::VE_MaxHP);
		Hp = pBattleInfo->CharacterProperty.GetCharacterProperty<float>(ECharacterPropertyTypeEnum::VE_HP);

		if (bApply)// 변이
		{
			CharacterID = FName("185");
			Armor = pBattleInfo->CharacterProperty.Armor;
			MagicResistance = pBattleInfo->CharacterProperty.MagicResistance;

			//pBattleInfo->SetBeforeModificationInfo(From, EntityProperty->GetCharacterID(), Armor, MagicResistance);


		}
		else// 복구
		{
			pBattleInfo->GetBeforeModificationInfo(CharacterID, Armor, MagicResistance);
		}
	}
	else
	{
		return;
	}

	// 	UGameObjectManager* objectMgr = UUtilFunctionLibrary::GetGameObjectManagerFromActor(Owner);
	// 
	// 	if (objectMgr)

	// 	SpawnedCharacter = objectMgr->SpawnCharacter(SpawnTransform, CharacterID, eCategoryType, UserKID, HeroUD, GroupKey, PartyID, PlayerUK, Level, Grade);
	SpawnedCharacter = UObjectFunctionLibrary::SpawnCharacter(SpawnTransform, CharacterID, eCategoryType, UserKID, HeroUD, GroupKey, PartyID, PlayerUK, Level, Grade);
	if (SpawnedCharacter)
	{
		pBattleComp = UCharacterHelpFunctionLibrary::GetBattleEntityComponent(SpawnedCharacter);
		pBattleInfo = UCharacterHelpFunctionLibrary::GetBattleInfo(SpawnedCharacter);

		if (EntityProperty && pBattleComp && pBattleInfo /*&& pSkillComp*/)
		{
			pBattleInfo->CharacterProperty.SetCharacterProperty<float>(ECharacterPropertyTypeEnum::VE_HP, Hp);
			pBattleInfo->CharacterProperty.SetCharacterProperty<float>(ECharacterPropertyTypeEnum::VE_MaxHP, MaxHp);
			pBattleInfo->CharacterProperty.SetCharacterProperty<int32>(ECharacterPropertyTypeEnum::VE_Armor, Armor);
			pBattleInfo->CharacterProperty.SetCharacterProperty<int32>(ECharacterPropertyTypeEnum::VE_MagicResistance, MagicResistance);

			if (bApply)
			{
				pBattleInfo->SetBeforeModificationInfo(SpawnedCharacter, EntityProperty->GetCharacterID(), Armor, MagicResistance);
				pBattleInfo->ModifyTimerActive();
			}
		}
#ifdef WITH_EDITOR
		EntityProperty = Cast< IEntityBaseProperty >(SpawnedCharacter);
		if (EntityProperty)
		{
			EntityProperty->InitStartProperty();// for test
		}
#endif // WITH_EDITOR
	}

	Owner->Destroy();
}
