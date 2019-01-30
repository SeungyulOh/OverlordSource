// Fill out your copyright notice in the Description page of Project Settings.

#include "ProjectR.h"
#include "GlobalIntegrated.h"
#include "UtilFunctionIntegrated.h"

#include "ContentGameMode/Controllers/BasePlayerController.h"
#include "UI/UI_Tutorial/RSystemNoti.h"
#include "Animation/SkeletalMeshActor.h"
#include "classes/Components/SkeletalMeshComponent.h"
#include "Blueprint/UserWidget.h"

#include "Character/BattleBaseCharacter.h"
#include "GameInfo/PlayStateInfo.h"

#include "SFTrigger.h"

ASFTrigger::ASFTrigger(const FObjectInitializer& ObjectInitializer) :
	Super(ObjectInitializer)
{
	PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void ASFTrigger::BeginPlay()
{
	Super::BeginPlay();
	
	SetActorTickEnabled(IsBeginTick);
}

// Called every frame
void ASFTrigger::Tick( float DeltaTime )
{
	Super::Tick( DeltaTime );
	
	if (IsBlockIn || IsBlockX || IsBlockY)
	{
		CheckBlock();
	}
	else if (SetTargetActor())
	{
		AActor* SelectCharacter = BasePlayerController->GetSelectedCharacter();
		if (SelectCharacter == nullptr)
			return;
		
		FVector NewPosition = SelectCharacter->GetActorLocation();//UUtilFunctionLibrary::GetNavAgentLocation(SelectCharacter);
		if (ComputeTriggerLocation(NewPosition))
		{
			if (SystemNotiWidget->IsInViewport() == false)
			{
				SystemNotiWidget->AddToViewport(500);
			}

			USkeletalMeshComponent* Comp = nullptr;
			if (TargetActor != nullptr)
			{
				Comp = TargetActor->GetSkeletalMeshComponent();
			}
			else if (TempSaveActor != nullptr)
			{
				Comp = TempSaveActor;
			}

			if (Comp == nullptr)
				return;

			FVector NotiLocation = Comp->GetSocketLocation(SOCKET_STATE);
			SystemNotiWidget->SetLocation(NotiLocation);
		}
		else
		{
			if(SystemNotiWidget->IsInViewport())
			{
				SystemNotiWidget->RemoveFromViewport();
			}			
		}
	}
}

void ASFTrigger::Destroyed()
{
	if(SystemNotiWidget)
	{
		SystemNotiWidget = nullptr;
	}
	
	TargetActor = nullptr;

	Super::Destroyed();
}

bool ASFTrigger::ComputeTriggerLocation(FVector& InOutLocation)
{
	FVector ThisLocation = this->GetActorLocation();
	FVector TrackingLocation = InOutLocation - ThisLocation;
	
	float SizeX = FMath::Sqrt(TrackingLocation.X * TrackingLocation.X);
	float SizeY = FMath::Sqrt(TrackingLocation.Y * TrackingLocation.Y);

	float CheckSizeX = FMath::Sqrt(TargetLocation.X * TargetLocation.X);
	float CheckSizeY = FMath::Sqrt(TargetLocation.Y * TargetLocation.Y);

	bool bResultSum = false;
	if (SizeX <= CheckSizeX && SizeY <= CheckSizeY)
	{
		bResultSum = true;
	}

	return bResultSum;
}

void ASFTrigger::SetNotiDialogue()
{
	if (SystemNotiWidget != nullptr)
		return;

	FName Path = TEXT("/Game/UMG/NewTutorial/UC_HUD_NotiDialogue.UC_HUD_NotiDialogue_C");
	UClass* Result = LoadClass<UUserWidget>(NULL, *Path.ToString(), NULL, LOAD_None, NULL);;
	
	NotiDialogue = CreateWidget<UUserWidget>(BasePlayerController->GetWorld(), Result);
	if (NotiDialogue)
	{
		SystemNotiWidget = Cast<URSystemNoti>(NotiDialogue);
		
		SystemNotiWidget->SetDialogueString(DialogueID);
		
		FVector Location = this->GetActorLocation();
		SystemNotiWidget->SetLocation(Location);
	}
}

bool ASFTrigger::SetTargetActor()
{
	if (TargetActorID.IsNone())
		return false;

	if (PlayerTargetIndex != -1)
	{
		AActor* Target = RGAMEINSTANCE(this)->PlayStateInfo->GetOwnerCharacter(PlayerTargetIndex);
		if (Target)
		{
			auto BattleCharacter = Cast<ABattleBaseCharacter>(Target);
			TempSaveActor = BattleCharacter->GetMesh();

			SetNotiDialogue();
			return true;
		}
	}

	bool bResult = false;
	for (TActorIterator<ASkeletalMeshActor> ActorItr(GetWorld()); ActorItr; ++ActorItr)
	{
		TArray<UActorComponent*> Check = ActorItr->GetComponentsByTag(USkeletalMeshComponent::StaticClass(), TargetActorID);
		if(Check.Num() != 0)
		{
			TargetActor = *ActorItr;
			bResult = true;

			SetNotiDialogue();
			break;
		}
	}

	return bResult;
}

void ASFTrigger::CheckBlock()
{
	AActor* SelectCharacter = BasePlayerController->GetSelectedCharacter();
	if (SelectCharacter == nullptr)
		return;
	FVector NewPosition = SelectCharacter->GetActorLocation();

	FVector ThisLocation = this->GetActorLocation();
	FVector TrackingLocation = NewPosition - ThisLocation;

	if (IsBlockIn)
	{
		float SizeX = FMath::Sqrt(TrackingLocation.X * TrackingLocation.X);
		float CheckSizeX = FMath::Sqrt(TargetLocation.X * TargetLocation.X);

		float SizeY = FMath::Sqrt(TrackingLocation.Y * TrackingLocation.Y);
		float CheckSizeY = FMath::Sqrt(TargetLocation.Y * TargetLocation.Y);

		if (SizeX <= CheckSizeX && SizeY <= CheckSizeY)
		{
			if (IsBlockX)
			{
				if (NewPosition.X > ThisLocation.X)
				{
					NewPosition.X = ThisLocation.X + TargetLocation.X;
				}
				else
				{
					NewPosition.X = ThisLocation.X - TargetLocation.X;
				}
			}
			if (IsBlockY)
			{
				if (NewPosition.Y > ThisLocation.Y)
				{
					NewPosition.Y = ThisLocation.Y + TargetLocation.Y;
				}
				else
				{
					NewPosition.Y = ThisLocation.Y - TargetLocation.Y;
				}
			}

			SelectCharacter->SetActorLocation(NewPosition);
		}
		return;
	}

	if (IsBlockX)
	{
		float SizeX = FMath::Sqrt(TrackingLocation.X * TrackingLocation.X);
		float CheckSizeX = FMath::Sqrt(TargetLocation.X * TargetLocation.X);
		
		if (SizeX <= CheckSizeX)
		{
			if (NewPosition.X > ThisLocation.X)
			{
				NewPosition.X = ThisLocation.X + TargetLocation.X;
			}
			else
			{
				NewPosition.X = ThisLocation.X - TargetLocation.X;
			}
			SelectCharacter->SetActorLocation(NewPosition);
		}
	}
	if (IsBlockY)
	{
		float SizeY = FMath::Sqrt(TrackingLocation.Y * TrackingLocation.Y);
		float CheckSizeY = FMath::Sqrt(TargetLocation.Y * TargetLocation.Y);

		if (SizeY <= CheckSizeY)
		{
			if (NewPosition.Y > ThisLocation.Y)
			{
				NewPosition.Y = ThisLocation.Y + TargetLocation.Y;
			}
			else
			{
				NewPosition.Y = ThisLocation.Y - TargetLocation.Y;
			}
			SelectCharacter->SetActorLocation(NewPosition);
		}
	}
}

void ASFTrigger::CheckTick()
{
	bool bIsTick = !(TargetActorID.IsNone());
	bool test = (IsBlockIn || IsBlockX || IsBlockY || bIsTick);
	bIsTick = test;
	if (bIsTick)
	{
		if (BasePlayerController == nullptr)
		{
			BasePlayerController = UUtilFunctionLibrary::GetBasePlayerController(this->GetWorld());
		}
	}

	SetActorTickEnabled(bIsTick);
}

int32 ASFTrigger::GetGroupIndex()
{
	return GroupIndex;
}
