// Fill out your copyright notice in the Description page of Project Settings.

#include "ProjectR.h"
#include "BaseEditor.h"


// Sets default values
ABaseEditor::ABaseEditor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ABaseEditor::BeginPlay()
{
	Super::BeginPlay();

	APlayerController* Controller = UGameplayStatics::GetPlayerController(this, 0);
	if (IsValid(Controller))
	{
		EnableInput(Controller);
		Controller->bShowMouseCursor = true;
	}	
}

// Called every frame
void ABaseEditor::Tick( float DeltaTime )
{
	Super::Tick( DeltaTime );

}

