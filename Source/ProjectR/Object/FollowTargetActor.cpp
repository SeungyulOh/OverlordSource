// Fill out your copyright notice in the Description page of Project Settings.

#include "ProjectR.h"
#include "FollowTargetActor.h"


// Sets default values
AFollowTargetActor::AFollowTargetActor() : Super()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AFollowTargetActor::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AFollowTargetActor::Tick( float DeltaTime )
{
	Super::Tick( DeltaTime );

	if (FollowTarget.IsValid())
	{
		this->SetActorTransform(FollowTarget->GetTransform());
	}
}

