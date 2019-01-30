// Fill out your copyright notice in the Description page of Project Settings.

#include "ProjectR.h"
#include "WallActor.h"



// Sets default values
AWallActor::AWallActor(const FObjectInitializer& ObjectInitializer) :
	Super(ObjectInitializer)
{ 	
	BoxComponent = ObjectInitializer.CreateDefaultSubobject<UBoxComponent>(this, FName(TEXT("Collision")));
	BoxComponent->SetupAttachment(RootComponent);
}
