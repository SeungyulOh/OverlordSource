// Fill out your copyright notice in the Description page of Project Settings.

#include "ProjectR.h"
#include "GlobalIntegrated.h"
#include "UtilFunctionIntegrated.h"
#include "ContentIntergrated.h"

#include "Lobby/IndoorLobbyStateActor.h"

#include "UP_Test.h"

void UUP_Test::NativeConstruct()
{
	Super::NativeConstruct();

	TArray<FString> HeroUDList;
	RGAMEINSTANCE(this)->RInventory->GetHeroUDs(HeroUDList, EClassTypeEnum::VE_ClassTypeMax, EHeroInventoryType::VE_InventoryTypeAll);
	RGAMEINSTANCE(this)->RInventory->GetHeroUDs(HeroUDList, EClassTypeEnum::VE_ClassTypeMax, EHeroInventoryType::VE_TypeHero);
	RGAMEINSTANCE(this)->RInventory->SetHeroSort(HeroUDList, EFilterSortTypeEnum::VE_AttackPower, false);

	if (HeroUDList.IsValidIndex(0))
		MainChangeHero(HeroUDList[0]);


	if (IsValid(Button_MainChange))
	{
		Button_MainChange->OnClicked.RemoveDynamic(this, &UUP_Test::MainChangeButton);
		Button_MainChange->OnClicked.AddDynamic(this, &UUP_Test::MainChangeButton);
	}
	if (IsValid(Button_RightChange))
	{
		Button_RightChange->OnClicked.RemoveDynamic(this, &UUP_Test::RightChangeButton);
		Button_RightChange->OnClicked.AddDynamic(this, &UUP_Test::RightChangeButton);
	}
	if (IsValid(Button_LeftChange))
	{
		Button_LeftChange->OnClicked.RemoveDynamic(this, &UUP_Test::LeftChangeButton);
		Button_LeftChange->OnClicked.AddDynamic(this, &UUP_Test::LeftChangeButton);
	}
}

void UUP_Test::NativeDestruct()
{
	DestroyLeftCharacter();
	DestroyRightCharacter();
	if (IsValid(Button_MainChange))
		Button_MainChange->OnClicked.RemoveDynamic(this, &UUP_Test::MainChangeButton);
	if (IsValid(Button_RightChange))
		Button_RightChange->OnClicked.RemoveDynamic(this, &UUP_Test::RightChangeButton);
	if (IsValid(Button_LeftChange))
		Button_LeftChange->OnClicked.RemoveDynamic(this, &UUP_Test::LeftChangeButton);

	Super::NativeDestruct();
}

void UUP_Test::ChangeHero(FString InHeroUD)
{
	switch (CharacterSelectType)
	{
		case ECharacterSelectTypeEnum::VE_Main:
		{
			MainChangeHero(InHeroUD);
			break;
		}
		case ECharacterSelectTypeEnum::VE_Left:
		{
			LeftChangeHero(InHeroUD);
			break;
		}
		case ECharacterSelectTypeEnum::VE_Right:
		{
			RightChangeHero(InHeroUD);
			break;
		}
		default:
		{
			break;
		}
	}
}

void UUP_Test::MainChangeHero(FString InHeroUD)
{
	URGameInstance *RGameInstance = RGAMEINSTANCE(this);

	FHERO *Hero = RGameInstance->RInventory->GetHeroWithUD(InHeroUD);
	if (Hero)
	{
		ATowerLobbyGameMode *CurrentMode = Cast<ATowerLobbyGameMode>(UGameplayStatics::GetGameMode(GetWorld()));
		if (CurrentMode)
		{
			AIndoorLobbyStateActor *LobbyStateActor = Cast<AIndoorLobbyStateActor>(CurrentMode->GetLobbyStateActor(ETowerLobbyStateEnum::Indoor));

			if (LobbyStateActor)
			{

				FCharacterTableInfo *ChracterTable = UTableManager::GetInstancePtr()->GetCharacterRow(Hero->heroId);
				if (ChracterTable)
				{
					LobbyStateActor->ChangeToHeroName(ChracterTable->DisplayName);
					UUtilFunctionLibrary::SetTowerBossCameraLocation(ChracterTable->DisplayName);
				}
				UCameraComponent *CameraComponent = Cast<UCameraComponent>(LobbyStateActor->IndoorNormalViewCameraActor->FindComponentByClass(UCameraComponent::StaticClass()));
				if (CameraComponent)
				{
					CameraComponent->SetFieldOfView(55.f);
				}
			}
		}
	}
}

void UUP_Test::LeftChangeHero(FString InHeroUD)
{
	DestroyLeftCharacter();

	ATowerLobbyGameMode *CurrentMode = Cast<ATowerLobbyGameMode>(UGameplayStatics::GetGameMode(GetWorld()));
	if (CurrentMode)
	{
		AIndoorLobbyStateActor *LobbyStateActor = Cast<AIndoorLobbyStateActor>(CurrentMode->GetLobbyStateActor(ETowerLobbyStateEnum::Indoor));

		if (LobbyStateActor)
		{
			int32 UserKID = RGAMEINSTANCE(this)->HttpClient->GetKID();

			FHERO *HeroInvenData = RGAMEINSTANCE(this)->RInventory->GetHeroWithUD(InHeroUD);
			if (HeroInvenData == nullptr)
				return;
			FCharacterTableInfo *CharacterInfo = UTableManager::GetInstancePtr()->GetCharacterRow(HeroInvenData->heroId);
			check(CharacterInfo != nullptr);

			FLobbyCharacterTableInfo *LobbyCharacterInfo = FLobbyCharacterTableInfo::LookUp(CharacterInfo->DisplayName);

			FTransform CharacterTransform;
			FVector Location = CharacterTransform.GetLocation();
			Location.X += -100.0f;
			Location.Y += -55.0f;
			Location.Z = CharacterInfo->UnitHeight * 0.85;
			FRotator lookatRotation = FRotator::MakeFromEuler(FVector(0, 0, 65));
			CharacterTransform.SetLocation(Location);
			CharacterTransform.SetScale3D(FVector(0.85, 0.85, 0.85));
			CharacterTransform.SetRotation(lookatRotation.Quaternion());

			FActorSpawnParameters SpawnParams;
			SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;

			ABattleBaseCharacter *SpawnedActor = GetWorld()->SpawnActor<ABattleBaseCharacter>(LobbyCharacterInfo->LobbyHeroClass.LoadSynchronous(), CharacterTransform, SpawnParams);

			SpawnedActor->GetCharacterMovement()->GravityScale = 0.0f;

			LeftLobbyCharacter = SpawnedActor;

			if (!IsValid(LeftLobbyCharacter))
			{
				UE_LOG(LogLobby, Log, TEXT("AIndoorLobbyStateActor::ChangeHero: Failed to get ALobbyCharacter"));
				return;
			}

			if (!LobbyCharacterInfo->RWeaponType.IsNull())
			{
				AActor *WeaponActor = GetWorld()->SpawnActor(LobbyCharacterInfo->RWeaponType.LoadSynchronous());
				check(IsValid(WeaponActor));
				USkeletalMeshComponent *MeshComp = SpawnedActor->GetMesh();
				check(IsValid(MeshComp));
				FAttachmentTransformRules rules = FAttachmentTransformRules::KeepRelativeTransform;
				rules.bWeldSimulatedBodies = true;
				WeaponActor->AttachToComponent(MeshComp, rules, TEXT("Bone_Weapon_R"));
				LeftRWeapon = WeaponActor;
			}
			if (!LobbyCharacterInfo->LWeaponType.IsNull())
			{
				AActor *WeaponActor = GetWorld()->SpawnActor(LobbyCharacterInfo->LWeaponType.LoadSynchronous());
				check(IsValid(WeaponActor));
				USkeletalMeshComponent *MeshComp = SpawnedActor->GetMesh();
				check(IsValid(MeshComp));
				FAttachmentTransformRules rules = FAttachmentTransformRules::KeepRelativeTransform;
				rules.bWeldSimulatedBodies = true;
				WeaponActor->AttachToComponent(MeshComp, rules, TEXT("Bone_Weapon_L"));
				LeftLWeapon = WeaponActor;
			}
		}
	}
}

void UUP_Test::RightChangeHero(FString InHeroUD)
{
	DestroyRightCharacter();
	ATowerLobbyGameMode *CurrentMode = Cast<ATowerLobbyGameMode>(UGameplayStatics::GetGameMode(GetWorld()));
	if (CurrentMode)
	{
		AIndoorLobbyStateActor *LobbyStateActor = Cast<AIndoorLobbyStateActor>(CurrentMode->GetLobbyStateActor(ETowerLobbyStateEnum::Indoor));

		if (LobbyStateActor)
		{
			int32 UserKID = RGAMEINSTANCE(this)->HttpClient->GetKID();

			FHERO *HeroInvenData = RGAMEINSTANCE(this)->RInventory->GetHeroWithUD(InHeroUD);
			if (HeroInvenData == nullptr)
				return;
			FCharacterTableInfo *CharacterInfo = UTableManager::GetInstancePtr()->GetCharacterRow(HeroInvenData->heroId);
			check(CharacterInfo != nullptr);

			FLobbyCharacterTableInfo *LobbyCharacterInfo = FLobbyCharacterTableInfo::LookUp(CharacterInfo->DisplayName);

			FTransform CharacterTransform;
			FVector Location = CharacterTransform.GetLocation();
			Location.X += 100.0f;
			Location.Y += -55.0f;
			Location.Z = CharacterInfo->UnitHeight * 0.85;
			FRotator lookatRotation = FRotator::MakeFromEuler(FVector(0, 0, 125));
			CharacterTransform.SetLocation(Location);
			CharacterTransform.SetScale3D(FVector(0.85, 0.85, 0.85));
			CharacterTransform.SetRotation(lookatRotation.Quaternion());

			FActorSpawnParameters SpawnParams;
			SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;

			ABattleBaseCharacter *SpawnedActor = GetWorld()->SpawnActor<ABattleBaseCharacter>(LobbyCharacterInfo->LobbyHeroClass.LoadSynchronous(), CharacterTransform, SpawnParams);
			SpawnedActor->GetCharacterMovement()->GravityScale = 0.0f;

			RightLobbyCharacter = SpawnedActor;

			if (!IsValid(RightLobbyCharacter))
			{
				UE_LOG(LogLobby, Log, TEXT("AIndoorLobbyStateActor::ChangeHero: Failed to get ALobbyCharacter"));
				return;
			}

			if (!LobbyCharacterInfo->RWeaponType.IsNull())
			{
				AActor *WeaponActor = GetWorld()->SpawnActor(LobbyCharacterInfo->RWeaponType.LoadSynchronous());
				check(IsValid(WeaponActor));
				USkeletalMeshComponent *MeshComp = SpawnedActor->GetMesh();
				check(IsValid(MeshComp));
				FAttachmentTransformRules rules = FAttachmentTransformRules::KeepRelativeTransform;
				rules.bWeldSimulatedBodies = true;
				WeaponActor->AttachToComponent(MeshComp, rules, TEXT("Bone_Weapon_R"));
				RightRWeapon = WeaponActor;
			}
			if (!LobbyCharacterInfo->LWeaponType.IsNull())
			{
				AActor *WeaponActor = GetWorld()->SpawnActor(LobbyCharacterInfo->LWeaponType.LoadSynchronous());
				check(IsValid(WeaponActor));
				USkeletalMeshComponent *MeshComp = SpawnedActor->GetMesh();
				check(IsValid(MeshComp));
				FAttachmentTransformRules rules = FAttachmentTransformRules::KeepRelativeTransform;
				rules.bWeldSimulatedBodies = true;
				WeaponActor->AttachToComponent(MeshComp, rules, TEXT("Bone_Weapon_L"));
				RightLWeapon = WeaponActor;
			}
		}
	}
}

void UUP_Test::DestroyLeftCharacter()
{
	if (LeftLobbyCharacter && IsValid(LeftLobbyCharacter))
	{
		LeftLobbyCharacter->K2_DestroyActor();
		LeftLobbyCharacter = nullptr;
	}
	if (LeftRWeapon && IsValid(LeftRWeapon))
	{
		LeftRWeapon->K2_DestroyActor();
		LeftRWeapon = nullptr;
	}
	if (LeftLWeapon && IsValid(LeftLWeapon))
	{
		LeftLWeapon->K2_DestroyActor();
		LeftLWeapon = nullptr;
	}
}

void UUP_Test::DestroyRightCharacter()
{
	if (RightLobbyCharacter && IsValid(RightLobbyCharacter))
	{
		RightLobbyCharacter->K2_DestroyActor();
		RightLobbyCharacter = nullptr;
	}
	if (RightRWeapon && IsValid(RightRWeapon))
	{
		RightRWeapon->K2_DestroyActor();
		RightRWeapon = nullptr;
	}
	if (RightLWeapon && IsValid(RightLWeapon))
	{
		RightLWeapon->K2_DestroyActor();
		RightLWeapon = nullptr;
	}
}

void UUP_Test::MainChangeButton()
{
	CharacterSelectType = ECharacterSelectTypeEnum::VE_Main;
}

void UUP_Test::LeftChangeButton()
{
	CharacterSelectType = ECharacterSelectTypeEnum::VE_Left;
}

void UUP_Test::RightChangeButton()
{
	CharacterSelectType = ECharacterSelectTypeEnum::VE_Right;
}
