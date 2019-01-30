// Fill out your copyright notice in the Description page of Project Settings.

#include "ProjectR.h"

#include "GlobalIntegrated.h"


#include "UtilFunctionIntegrated.h"

#include "CustomStructures/HeroSlotInfo.h"

#include "UI/Dialogue/UP_Dialogue.h"

#include "Object/BaseCameraActor.h"
#include "Object/EmptyBasicActor.h"
#include "3DDialogueActor.h"
#include "RWidgetComponent.h"

#define PATH_CHARACTER			FString("/Game/Character/")
#define PATH_NPC				FString("/Game/Character/Unit/")
#define PATH_WEAPON				FString("/Game/Character/Characters_Weapons/")
#define PATH_ANIMATION			FString("Animations/")
#define PATH_SKELMESH			FString("_SkeletalMesh")

#define PATH_IDLE 				FString("_Ground_Move_Idle")
#define PATH_LOBBYIDLE 			FString("_Ground_LobbyIdle_01")
#define PATH_LOBBYSOCIAL 		FString("_Ground_LoobbySocial")
#define PATH_LOBBYSPECIAL		FString("_Lobby_SpecialAni")
#define PATH_BATTLEREADY01 		FString("_BattleReady_Action_01")
#define PATH_BATTLEWIN01 		FString("_BattleEnd_Action_01")
#define PATH_BATTLEEND 			FString("_BattleEnd_Idle_01")

#define STATUS_COLOR_PARAM2 TEXT("StatusColor")

A3DDialogueActor::A3DDialogueActor(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	SyncCount = 0;
	MyRootComponent = CreateDefaultSubobject<USceneComponent>(FName(TEXT("Root")));
	MyRootComponent->SetupAttachment(RootComponent);
		
	WidgetComp = CreateDefaultSubobject<URWidgetComponent>(FName(TEXT("WidgetComponent")));
	WidgetComp->SetupAttachment(MyRootComponent);

}

void A3DDialogueActor::BeginPlay()
{
	Super::BeginPlay();

	InitialPositions.Emplace(LeftSkelComp->GetRelativeTransform().GetTranslation());
	InitialPositions.Emplace(RightSkelComp->GetRelativeTransform().GetTranslation());
	InitialPositions.Emplace(CenterSkelComp->GetRelativeTransform().GetTranslation());

	InitialRotations.Emplace(LeftSkelComp->GetRelativeTransform().GetRotation());
	InitialRotations.Emplace(RightSkelComp->GetRelativeTransform().GetRotation());
	InitialRotations.Emplace(CenterSkelComp->GetRelativeTransform().GetRotation());

}

void A3DDialogueActor::ShowActor(FDialogueInfo& InInfo)
{
	

	URGameInstance* GameInstance =  RGAMEINSTANCE(this);
	UPlayStateInfo* PlayStateInfo = GameInstance->PlayStateInfo;
	UTableManager* TablePtr = UTableManager::GetInstancePtr();

	checkf(GameInstance != nullptr, TEXT("GameInstance is Empty"));
	checkf(PlayStateInfo != nullptr, TEXT("PlayStateInfo is Empty"));
	checkf(TablePtr != nullptr, TEXT("TablePtr is Empty"));

	PrevCharacterPos = CurrentCharacterPos;
	CurrentDialogue = InInfo;

	if(InInfo.SpeakerPos.ToString().Contains(TEXT("LEFT")))
		CurrentCharacterPos = EDialogueCharPosition::VE_Left;
	else if (InInfo.SpeakerPos.ToString().Contains(TEXT("RIGHT")))
		CurrentCharacterPos = EDialogueCharPosition::VE_Right;
	else if (InInfo.SpeakerPos.ToString().Contains(TEXT("CENTER")))
		CurrentCharacterPos = EDialogueCharPosition::VE_Center;
	else
		CurrentCharacterPos = EDialogueCharPosition::VE_None;

	CurrentCharacterName = InInfo.SpeakerName;
	CurrentSpeakerResourceID = InInfo.SpeakerRes;

	if (ComputeCurrentCharacter() == false)
	{
		UE_LOG(LogGamePlay, Log, TEXT("Error - A3DDialogueActor::ComputeCurrentCharacter"));
		return;
	}

	SetCharacterAnimation();
	SetCharacterOffset();
	
	//SetLightChange();
	
	if (IsCompleteInit == false)
	{
		SyncPos();
		IsCompleteInit = true;
	}
}

void A3DDialogueActor::HideActor(bool InIsReset /*= true*/)
{
	//SetActorHiddenInGame(true);
	SyncCount = 0;

	IsCompleteInit = false;
	if (InIsReset)
	{
		if (LeftSkelComp)
			LeftSkelComp->SetSkeletalMesh(nullptr);
		if (RightSkelComp)
			RightSkelComp->SetSkeletalMesh(nullptr);
		if (CenterSkelComp)
			CenterSkelComp->SetSkeletalMesh(nullptr);
		
		FDetachmentTransformRules Rules(EDetachmentRule::KeepRelative, true);
		if (LeftWeaponL)
		{
			LeftWeaponL->DetachFromComponent(Rules);
			LeftWeaponL->SetStaticMesh(nullptr);
		}
		if (LeftWeaponR)
		{
			LeftWeaponR->DetachFromComponent(Rules);
			LeftWeaponR->SetStaticMesh(nullptr);
		}
		if (RightWeaponL)
		{
			RightWeaponL->DetachFromComponent(Rules);
			RightWeaponL->SetStaticMesh(nullptr);
		}
		if (RightWeaponR)
		{
			RightWeaponR->DetachFromComponent(Rules);
			RightWeaponR->SetStaticMesh(nullptr);
		}
		if (CenterWeaponL)
		{
			CenterWeaponL->DetachFromComponent(Rules);
			CenterWeaponL->SetStaticMesh(nullptr);
		}
		if (CenterWeaponR)
		{
			CenterWeaponR->DetachFromComponent(Rules);
			CenterWeaponR->SetStaticMesh(nullptr);
		}

		CurrentCharacterMesh = nullptr;
		CurrentWeaponMesh = nullptr;
		CurrentTargetCharacter = nullptr;
	}
}

bool A3DDialogueActor::ComputeCurrentCharacter()
{
	ResetCharacter();

	if (CharacterMap.Contains(CurrentCharacterName) == false)
	{
		CreateCharacter();
	}

	auto Test = CharacterMap.Find(CurrentCharacterName);
	if (Test == nullptr)
		return false;

	CurrentCharacterMesh = *(Test);
	if (CurrentCharacterMesh == nullptr)
		return false;
	
	auto TestDeco = CharacterDecoMap.Find(CurrentCharacterName);
	if (TestDeco)
	{
		CurrentCharacterDecoMesh = *(TestDeco);
	}
	else
	{
		CurrentCharacterDecoMesh = nullptr;
	}

	switch (CurrentCharacterPos)
	{
	case EDialogueCharPosition::VE_Left:
	{
		CurrentTargetCharacter = LeftSkelComp;
		CurrentTargetDeco = LeftDecoComp;
	}
	break;
	case EDialogueCharPosition::VE_Right:
	{
		CurrentTargetCharacter = RightSkelComp;
		CurrentTargetDeco = RightDecoComp;
	}
	break;
	case EDialogueCharPosition::VE_Center:
	{
		CurrentTargetCharacter = CenterSkelComp;
		CurrentTargetDeco = nullptr;
	}
	break;
	}

	if (CurrentTargetCharacter == nullptr)
		return false;

	if (CurrentTargetCharacter->SkeletalMesh)
	{
		IsReverseAni = true;
	}



	CurrentTargetCharacter->SetSkeletalMesh(CurrentCharacterMesh);
	/*material settings*/
	UTableManager* TablePtr = UTableManager::GetInstancePtr();
	checkf(TablePtr != nullptr, TEXT("TablePtr is Empty"));
	FDialogueResource* Table = TablePtr->GetDialogueResourceTableRow(CurrentSpeakerResourceID);
	checkf(Table != nullptr, TEXT("Table is Empty"));
	int32 MatNum = CurrentTargetCharacter->GetNumMaterials();
	UMaterialInstance* MI = nullptr;
	for (size_t i = 0; i < MatNum; ++i)
	{
		if (i == 0)
			MI = UUtilFunctionLibrary::GetMaterialInstanceFromAssetID(Table->Material0);
		else if(i == 1)
			MI = UUtilFunctionLibrary::GetMaterialInstanceFromAssetID(Table->Material1);
		else if (i == 2)
			MI = UUtilFunctionLibrary::GetMaterialInstanceFromAssetID(Table->Material2);
		else if (i == 3)
			MI = UUtilFunctionLibrary::GetMaterialInstanceFromAssetID(Table->Material3);

		if (MI->IsValidLowLevel())
			CurrentTargetCharacter->SetMaterial(i, MI);
	}
	/**/
	
	if (CurrentTargetDeco)
	{
		CurrentTargetDeco->SetSkeletalMesh(CurrentCharacterDecoMesh);
		if (CurrentCharacterDecoMesh)
		{
			CurrentTargetDeco->AttachToComponent(CurrentTargetCharacter, FAttachmentTransformRules::KeepRelativeTransform);
		}
	}

	ComputeWeapon();

	return true;
}


void A3DDialogueActor::CreateCharacter()
{
	UTableManager* TablePtr = UTableManager::GetInstancePtr();
	checkf(TablePtr != nullptr, TEXT("TablePtr is Empty"));

	FDialogueResource* Table = TablePtr->GetDialogueResourceTableRow(CurrentSpeakerResourceID);
	checkf(Table != nullptr, TEXT("Table is Empty"));

	USkeletalMesh* SkelMesh = UUtilFunctionLibrary::GetSkeletalMeshFromAssetID(Table->SkeletalMesh);
	
	//UMaterial* mat0 = UUtilFunctionLibrary::GetMaterialFromAssetID(Table->Material0);
	if (SkelMesh)
	{
		CharacterMap.Emplace(CurrentCharacterName, SkelMesh);
	}
	else
	{
		UE_LOG(LogGamePlay, Log, TEXT("Error - A3DDialogueActor::CreateCharacter"));
	}
}


void A3DDialogueActor::PrevCharacterEffect(bool InDeafult /*= true*/)
{
	auto Pos = ((InDeafult) ? CurrentCharacterPos : PrevCharacterPos);
	if (Pos == EDialogueCharPosition::VE_None)
		return;

	auto TargetLight = ((Pos == EDialogueCharPosition::VE_Left) ? LeftLight : RightLight);
	auto TargetPointLight = ((Pos == EDialogueCharPosition::VE_Left) ? LeftPointLight : RightPointLight);
	

	if (InDeafult)
	{
		if (TargetLight)
		{
			TargetLight->Intensity = 5.0f;
		}
		if (TargetPointLight)
		{
			TargetPointLight->Intensity = 1500;
		}
	}
	else
	{
		if (TargetLight)
		{
			TargetLight->Intensity = 1.0f;
		}
		if (TargetPointLight)
		{
			TargetPointLight->Intensity = 300;
		}
	}
}

void A3DDialogueActor::SetMaterial()
{
	UTableManager* TablePtr = UTableManager::GetInstancePtr();
	checkf(TablePtr != nullptr, TEXT("TablePtr is Empty"));

	FDialogueAnimationTableInfo* Table = TablePtr->GetDialogueAnimationTableRow(CurrentCharacterName);
	checkf(Table != nullptr, TEXT("Table is Empty"));

	if (Table->MeshMaterial.IsNull() == false)
	{
		auto MaterialInstance1 = UUtilFunctionLibrary::GetMaterialInstanceFromAssetID(Table->MeshMaterial);
		CurrentTargetCharacter->SetMaterial(0, MaterialInstance1);
	}
	else
	{
		PrevCharacterEffect();
	}
}

void A3DDialogueActor::ComputeWeapon()
{
	UTableManager* TablePtr = UTableManager::GetInstancePtr();
	checkf(TablePtr != nullptr, TEXT("TablePtr is Empty"));

	FDialogueResource* Table = TablePtr->GetDialogueResourceTableRow(CurrentSpeakerResourceID);
	checkf(Table != nullptr, TEXT("Table is Empty"));

	
	UStaticMesh* LWeapon = nullptr;
	UStaticMesh* RWeapon = nullptr;
	EDialogueCharPosition WeaponPos = EDialogueCharPosition::VE_None;
	if (Table->LWeaponType.IsNull() == false)
	{
		LWeapon = UUtilFunctionLibrary::GetStaticMeshFromAssetID(Table->LWeaponType);
		WeaponPos = EDialogueCharPosition::VE_Left;
	}
	if (Table->RWeaponType.IsNull() == false)
	{
		RWeapon = UUtilFunctionLibrary::GetStaticMeshFromAssetID(Table->RWeaponType);
		if(WeaponPos == EDialogueCharPosition::VE_None)
			WeaponPos = EDialogueCharPosition::VE_Right;
		else if(WeaponPos == EDialogueCharPosition::VE_Left)
			WeaponPos = EDialogueCharPosition::VE_Both;
	}

	TArray<UStaticMeshComponent*> TempArray;
	TempArray.Reserve(2);
	switch (CurrentCharacterPos)
	{
	case EDialogueCharPosition::VE_Left:
	{
		TempArray.Emplace(LeftWeaponL);
		TempArray.Emplace(LeftWeaponR);
	}
	break;
	case EDialogueCharPosition::VE_Right:
	{
		TempArray.Emplace(RightWeaponL);
		TempArray.Emplace(RightWeaponR);
	}
	break;
	case EDialogueCharPosition::VE_Center:
	{
		TempArray.Emplace(CenterWeaponL);
		TempArray.Emplace(CenterWeaponR);
	}
	break;
	}

	ResetWeapon();
	
	FAttachmentTransformRules rules = FAttachmentTransformRules::KeepRelativeTransform;
	rules.bWeldSimulatedBodies = true;
	rules.RotationRule = EAttachmentRule::SnapToTarget;
	switch (WeaponPos)
	{
		case EDialogueCharPosition::VE_Left:
		{
			CurrentWeaponMesh = LWeapon;
			TempArray[0]->SetStaticMesh(CurrentWeaponMesh);
			TempArray[0]->AttachToComponent(CurrentTargetCharacter, rules, SOCKET_LWEAPON);
		}
		break;
		case EDialogueCharPosition::VE_Right:
		{
			CurrentWeaponMesh = RWeapon;
			TempArray[1]->SetStaticMesh(CurrentWeaponMesh);
			TempArray[1]->AttachToComponent(CurrentTargetCharacter, rules, SOCKET_RWEAPON);
		}
		break;
		case EDialogueCharPosition::VE_Both:
		{
			CurrentWeaponMesh = LWeapon;
			TempArray[0]->SetStaticMesh(CurrentWeaponMesh);
			TempArray[0]->AttachToComponent(CurrentTargetCharacter, rules, SOCKET_LWEAPON);

			CurrentWeaponMesh = RWeapon;
			TempArray[1]->SetStaticMesh(CurrentWeaponMesh);
			TempArray[1]->AttachToComponent(CurrentTargetCharacter, rules, SOCKET_RWEAPON);
		}
		break;
	}
}

void A3DDialogueActor::ResetCharacter()
{
	switch (CurrentCharacterPos)
	{
	case EDialogueCharPosition::VE_Left:
	{
		LeftSkelComp->SetMaterial(0, nullptr);
		LeftSkelComp->SetSkeletalMesh(nullptr);
	}
	break;
	case EDialogueCharPosition::VE_Right:
	{
		RightSkelComp->SetMaterial(0, nullptr);
		RightSkelComp->SetSkeletalMesh(nullptr);
	}
	break;
	case EDialogueCharPosition::VE_Center:
	{
		CenterSkelComp->SetMaterial(0, nullptr);
		CenterSkelComp->SetSkeletalMesh(nullptr);
	}
	break;
	}

	LeftSkelComp->SetRelativeLocation(InitialPositions[(int32)EDialogueCharPosition::VE_Left]);
	LeftSkelComp->SetRelativeRotation(InitialRotations[(int32)EDialogueCharPosition::VE_Left]);

	RightSkelComp->SetRelativeLocation(InitialPositions[(int32)EDialogueCharPosition::VE_Right]);
	RightSkelComp->SetRelativeRotation(InitialRotations[(int32)EDialogueCharPosition::VE_Right]);

	CenterSkelComp->SetRelativeLocation(InitialPositions[(int32)EDialogueCharPosition::VE_Center]);
	CenterSkelComp->SetRelativeRotation(InitialRotations[(int32)EDialogueCharPosition::VE_Center]);
}

void A3DDialogueActor::ResetWeapon()
{
	FDetachmentTransformRules Rules(EDetachmentRule::KeepRelative, true);

	switch (CurrentCharacterPos)
	{
		case EDialogueCharPosition::VE_Left:
		{
			LeftWeaponL->SetStaticMesh(nullptr);
			LeftWeaponR->SetStaticMesh(nullptr);
		}
		break;
		case EDialogueCharPosition::VE_Right:
		{
			RightWeaponL->SetStaticMesh(nullptr);
			RightWeaponR->SetStaticMesh(nullptr);
		}
		break;
		case EDialogueCharPosition::VE_Center:
		{
			CenterWeaponL->SetStaticMesh(nullptr);
			CenterWeaponR->SetStaticMesh(nullptr);
		}
		break;
	}
}

void A3DDialogueActor::SetCharacterOffset()
{
	if (CurrentTargetCharacter == nullptr)
		return;

	for (auto& Element : UP_Dialogue->OverrideMap)
	{
		FDialogueInfo* info = UTableManager::GetInstancePtr()->GetDialogueTableRow(FName(*FString::FromInt(Element.Key)));
		if (info && CurrentTargetCharacter)
		{
			if (UP_Dialogue.Get()->GetCurrentDialogueID() != Element.Key)
				continue;

			CurrentTargetCharacter->AddRelativeLocation(Element.Value.AdditionalLocation);
			CurrentTargetCharacter->AddRelativeRotation(Element.Value.AdditionalRotation);

			if(Element.Value.OverridedAnim)
				CurrentTargetCharacter->PlayAnimation(Element.Value.OverridedAnim, true);

			break;
		}
	}
	
}

void A3DDialogueActor::SetCharacterAnimation()
{
	UAnimationAsset* NewAnimToPlay = GetAnimation();
	if (NewAnimToPlay == nullptr)
		return;

	bool InAniLooping = true;
	CurrentTargetCharacter->PlayAnimation(NewAnimToPlay, InAniLooping);
	if (CurrentCharacterDecoMesh)
	{
		CurrentTargetDeco->PlayAnimation(NewAnimToPlay, InAniLooping);
	}
}

void A3DDialogueActor::Set2DImageBack(FDialogueInfo& InInfo)
{
	CurrentDialogue = InInfo;
	UTexture2D* texture = CurrentDialogue.BackgroundImg.Get();

	if (texture == nullptr)
	{
		WidgetComp->SetVisibility(false);
	}
	else
	{
		WidgetComp->SetVisibility(true);

		SyncPos();
		IsCompleteInit = true;
		
		UUserWidget* userwidget = WidgetComp->GetWidget();
		UImage* Image = Cast<UImage>(userwidget->WidgetTree->RootWidget);
		texture = UUtilFunctionLibrary::GetTexture2DFromAssetID(CurrentDialogue.BackgroundImg);
		Image->SetBrushFromTexture(texture);

		
	}

}

UAnimationAsset* A3DDialogueActor::GetAnimation()
{
	UTableManager* TablePtr = UTableManager::GetInstancePtr();
	checkf(TablePtr != nullptr, TEXT("TablePtr is Empty"));

	FDialogueResource* Table = TablePtr->GetDialogueResourceTableRow(CurrentSpeakerResourceID);
	checkf(Table != nullptr, TEXT("Table is Empty"));

	UAnimationAsset* NewAnimToPlay = UUtilFunctionLibrary::GetAnimationAssetFromAssetID(Table->Animation);
	
	return NewAnimToPlay;
}

bool A3DDialogueActor::SyncViewCameraPos()
{
	URGameInstance* GameInstance = RGAMEINSTANCE(this);
	checkf(GameInstance != nullptr, TEXT("GameInstance is Empty"));

	ABaseCameraActor* BaseCameraActor = GameInstance->CameraActor;

	if (BaseCameraActor)
	{
		APlayerController* PlayerController = GetWorld()->GetFirstPlayerController();
		if (IsValid(PlayerController))
		{
			auto CheckViewTarget = PlayerController->GetViewTarget();
			if (CheckViewTarget != BaseCameraActor)
			{
				BaseCameraActor->SetOwnerCharacter(nullptr);
				FVector OutCamLoc;
				FRotator OutCamRot;
				PlayerController->PlayerCameraManager->GetCameraViewPoint(OutCamLoc, OutCamRot);

				auto ViewTargetTransform = CheckViewTarget->GetActorTransform();
				auto SpringArm = BaseCameraActor->GetSpringArmComponent();
				if (SpringArm)
				{
					SpringArm->TargetArmLength = 0;
					SpringArm->SetRelativeRotation(FRotator::ZeroRotator);
				}
				FVector CameOwnerLoc(0, 0, 0);
				FRotator CamOwnerRot(0, 0, 0);
				FTransform Transform;
				for (TActorIterator<AEmptyBasicActor> ActorItr(GetWorld()); ActorItr; ++ActorItr)
				{
					AEmptyBasicActor* TargetActor = *ActorItr;
					if (TargetActor == nullptr)
						continue;

					//TArray<AActor*> ChildActors;
					TArray<USceneComponent*> ChildComp;
					if (TargetActor->Scene)
					{
						ChildComp = TargetActor->Scene->GetAttachChildren();
					}

					if (ChildComp.Num() == 0)
						continue;

					for (auto Element : ChildComp)
					{
						auto Child = Element->GetAttachChildren();
						if (Child.Num() > 0)
						{
							UCameraComponent* CamTarget = Cast<UCameraComponent>(Child[0]);
							if (CamTarget)
							{
								Transform = CamTarget->GetRelativeTransform();
								CameOwnerLoc = CamTarget->GetComponentLocation();
								CamOwnerRot = CamTarget->GetComponentRotation();
									
								break;
							}
						}
					}
					break;
				}
				
				BaseCameraActor->SetActorLocation(CameOwnerLoc, true);
				BaseCameraActor->SetActorRotation(CamOwnerRot);

				auto CamComp = BaseCameraActor->GetCameraComponent();
				if (CamComp)
				{
					CamComp->SetRelativeTransform(Transform, true);
				}
				this->GetWorldTimerManager().SetTimer(SyncHandler, this, &A3DDialogueActor::ProcessPos, 0.9f);

				return true;
			}
		}
	}
	return false;
}

void A3DDialogueActor::ProcessPos()
{
	this->SetActorHiddenInGame(false);
	if (SyncHandler.IsValid())
	{
		this->GetWorldTimerManager().ClearTimer(SyncHandler);
	}
}

void A3DDialogueActor::SyncPos()
{
	URGameInstance* GameInstance = RGAMEINSTANCE(this);
	checkf(GameInstance != nullptr, TEXT("GameInstance is Empty"));

	ABaseCameraActor* BaseCameraActor = GameInstance->CameraActor;

	if (BaseCameraActor == nullptr)
		return;

	bool IsSyncTarget = SyncViewCameraPos();

	FAttachmentTransformRules rules = FAttachmentTransformRules::KeepRelativeTransform;
	rules.bWeldSimulatedBodies = false;
	rules.RotationRule = EAttachmentRule::SnapToTarget;

	auto Comp = BaseCameraActor->GetDialoguePosComponent();
	this->AttachToComponent(Comp, rules);
	
	if (IsSyncTarget)
		return;

	if (IsSyncTarget == false
		|| IsCompleteInit)
	{
		this->SetActorHiddenInGame(false);
	}

	return;



	auto DialoguePosComp = BaseCameraActor->GetDialoguePosComponent();
	if (DialoguePosComp)
	{
		FVector Location = DialoguePosComp->GetComponentLocation();
		if (IsSyncTarget)
		{
			auto CamComp = BaseCameraActor->GetCameraComponent();
			if (CamComp)
			{
				Location.Z -= 70;
			}
		}
	}

	auto SpringArm = BaseCameraActor->GetSpringArmComponent();
	if (SpringArm)
	{
		FRotator Rot = BaseCameraActor->GetActorRotation();
			
		if (IsSyncTarget)
		{
			Rot.Yaw += 90;
			auto temp = Rot.Roll;
			Rot.Roll = Rot.Pitch;
			Rot.Pitch = temp;
		}
		else
		{
			auto SpringRot = SpringArm->GetComponentRotation();
			Rot.Roll = SpringRot.Pitch;

			if (Rot.Yaw >= 0 || Rot.Yaw < -45)
			{
				Rot.Yaw += 90;
			}
			else if (Rot.Yaw >= 89 && Rot.Yaw <= 179)
			{
				Rot.Yaw += 90;
				Rot.Yaw *= -1;
			}
			else
			{
				Rot.Yaw *= -1;
			}
		}
			
		this->SetActorRotation(Rot);
	}

	this->SetActorHiddenInGame(false);
}
