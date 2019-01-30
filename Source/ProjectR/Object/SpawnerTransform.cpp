// Fill out your copyright notice in the Description page of Project Settings.

#include "ProjectR.h"

#include "UtilFunctionIntegrated.h"

#include "SpawnerTransform.h"



// Sets default values
ASpawnerTransform::ASpawnerTransform()
{ 	

}

void ASpawnerTransform::BeginPlay()
{
	Super::BeginPlay();
}

void ASpawnerTransform::InitTrigger()
{
	OnActorBeginOverlap.Clear();
	OnActorBeginOverlap.AddDynamic(this, &ASpawnerTransform::OnOverlapBegin);
}

void ASpawnerTransform::OnOverlapBegin(AActor* OverlappedActor, AActor* OtherActor)
{
	/*FString str = TEXT("Waypoint : ") + OverlappedActor->GetName() + TEXT("   Actor : ") + OtherActor->GetName();
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Cyan, str);*/

	if (OtherActor)
	{
		if (UUtilFunctionLibrary::IsLeaderCharacter(OtherActor))
		{
			UBaseStageInfo*			BaseStageInfo		=	UGamePlayBluePrintFunction::GetBaseStageInfo();
			ALevelActor*			LevelActor			=	BaseStageInfo->GetLevelActor();

			if (LevelActor == nullptr)
				return;

			FSpawnerTransformInfo	NextWayPointTransform = RGAMEINSTANCE(GEngine)->AutoPlayManager->GetNextWayPointTransform();
			if (NextWayPointTransform.ID != OverlappedActor->GetUniqueID())
				return;

			LevelActor->ConditionCheck(EPhaseClearEnumType::VE_Move);
			LevelActor->ConditionCheck(EPhaseClearEnumType::VE_CharacterAllSurvival);
			OnActorBeginOverlap.Clear();
		}
	}
}
