// Fill out your copyright notice in the Description page of Project Settings.

#include "ProjectR.h"
#include "EmptyBasicActor.h"


// Sets default values
AEmptyBasicActor::AEmptyBasicActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Scene = CreateDefaultSubobject<USceneComponent>(FName(TEXT("Scene")));
	Scene->SetMobility(EComponentMobility::Movable);
	SetRootComponent(Scene);
}

// Called when the game starts or when spawned
void AEmptyBasicActor::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AEmptyBasicActor::Tick( float DeltaTime )
{
	Super::Tick( DeltaTime );

}

