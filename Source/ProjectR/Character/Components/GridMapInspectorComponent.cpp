// Fill out your copyright notice in the Description page of Project Settings.

#include "ProjectR.h"
#include "GridMapInspectorComponent.h"
#include "GlobalIntegrated.h"

#include "UtilFunctionIntegrated.h"


// Sets default values for this component's properties
UGridMapInspectorComponent::UGridMapInspectorComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	bWantsInitializeComponent = true;
	PrimaryComponentTick.bCanEverTick = true;

	// ...

}


// Called when the game starts
void UGridMapInspectorComponent::InitializeComponent()
{
	Super::InitializeComponent();

	OwnerActor = GetOwner();
	CurrentGridIndics.XIdx = 0;
	CurrentGridIndics.YIdx = 0;
}

void UGridMapInspectorComponent::UninitializeComponent()
{
	URGameInstance* RGameInstance = RGAMEINSTANCE(this);
	if (RGameInstance && RGameInstance->GridMapManager)
	{
		FMapCellData MapCellData;
		MapCellData.CellInfo = EMapCellStateEnum::VE_Movable;
		RGameInstance->GridMapManager->SetMapInfo(CurrentGridIndics.XIdx, CurrentGridIndics.YIdx, MapCellData);
	}
}


// Called every frame
void UGridMapInspectorComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent( DeltaTime, TickType, ThisTickFunction );
	
	UpdateGridMap();
}

void UGridMapInspectorComponent::UpdateGridMap()
{
	if (OwnerActor.IsValid() == false)
		return;

	FVector CurrentLocation = OwnerActor->GetActorLocation();

	URGameInstance* RGameInstance = RGAMEINSTANCE(this);
	if (RGameInstance && RGameInstance->GridMapManager)
	{
		FCellIndex NewGridIndics;
		bool bOccupyNew = RGameInstance->GridMapManager->UpdateCellInfo(CurrentGridIndics, CurrentLocation, NewGridIndics);
		if( bOccupyNew )
			CurrentGridIndics = NewGridIndics;
	}
		
}

FCellIndex& UGridMapInspectorComponent::GetCurrentGridIndex()
{
	return CurrentGridIndics;
}
