// Fill out your copyright notice in the Description page of Project Settings.

#include "ProjectR.h"
#include "CustomWidgetComponent.h"
#include "UI/UI_Battle/RSkillIcon.h"
#include "UI/CharacterUI/CharacterUserWidget.h"
#include "UtilFunctionIntegrated.h"
#include "ContentGameMode/Controllers/BasePlayerController.h"
#include "Camera/CameraActor.h"



// Sets default values for this component's properties
UCustomWidgetComponent::UCustomWidgetComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;
	PrimaryComponentTick.bTickEvenWhenPaused = true;

	// ...
}


// Called when the game starts
void UCustomWidgetComponent::BeginPlay()
{
	Super::BeginPlay();
	
}

void UCustomWidgetComponent::SetWidgetSpace(bool bIsWorld)
{
	if (bIsWorld)
	{
		Space = EWidgetSpace::World;
	}
	else
	{
		Space = EWidgetSpace::Screen;
	}
}

void UCustomWidgetComponent::SetBlendMode(EWidgetBlendMode InBlendMode)
{
	BlendMode = InBlendMode;
	if (BlendMode == EWidgetBlendMode::Transparent)
	{
		//TranslucencySortPriority = 255;
		TranslucentMaterial->GetMaterial()->bDisableDepthTest = true;
		TranslucentMaterial_OneSided->GetMaterial()->bDisableDepthTest = true;		
	}
	else
	{
		TranslucentMaterial->GetMaterial()->bDisableDepthTest = false;
		TranslucentMaterial_OneSided->GetMaterial()->bDisableDepthTest = false;
		MaskedMaterial->GetMaterial()->bDisableDepthTest = false;
		MaskedMaterial_OneSided->GetMaterial()->bDisableDepthTest = false;
	}
	
	UpdateMaterialInstanceParameters(); //by danzang FOR 4.13
	MarkRenderStateDirty();
}

// Called every frame
void UCustomWidgetComponent::TickComponent( float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction )
{
	Super::TickComponent( DeltaTime, TickType, ThisTickFunction );
	SetWidgetAngleFromCameraView();
}

void UCustomWidgetComponent::OnRegister()
{
	Super::OnRegister();

	
}


void UCustomWidgetComponent::CustomUpdateMaterialInstance()
{
	UMaterialInterface* Parent = nullptr;
	switch (BlendMode)
	{
	case EWidgetBlendMode::Opaque:
		Parent = bIsTwoSided ? OpaqueMaterial : OpaqueMaterial_OneSided;
		break;
	case EWidgetBlendMode::Masked:
		Parent = bIsTwoSided ? MaskedMaterial : MaskedMaterial_OneSided;
		break;
	case EWidgetBlendMode::Transparent:
		Parent = bIsTwoSided ? TranslucentMaterial : (bHasTranslucentDepthTest ? TranslucentMaterial_OneSided : TranslucentMaterial_OneSided_NoDepth);
		break;
	}

	if (MaterialInstance)
	{
		MaterialInstance->MarkPendingKill();
		MaterialInstance = nullptr;
	}

	MaterialInstance = UMaterialInstanceDynamic::Create(Parent, this);

	if (MaterialInstance)
	{
		MaterialInstance->SetTextureParameterValue("SlateUI", RenderTarget);
	}

}


void UCustomWidgetComponent::SetWidgetPos()
{
	if (WidgetDecorator.IsValid() && OwnerActor.IsValid() )
	{
		APlayerController* PlayerController = OwnerActor->GetWorld()->GetFirstPlayerController();
		if (PlayerController)
		{
			FVector Location = this->GetComponentLocation();
			FVector CameraDistance = FVector::ZeroVector;
			if (PlayerController->PlayerCameraManager)
			{
				CameraDistance = PlayerController->PlayerCameraManager->GetCameraLocation();
			}
			float fDistance = FVector::Dist(CameraDistance, Location);

			if (fDistance < 10000.0f)
			{
				FVector2D ScreenLocation;
				PlayerController->ProjectWorldLocationToScreen(Location, ScreenLocation);
				WidgetDecorator->SetPositionInViewport(ScreenLocation);
				WidgetDecorator->SetAlignmentInViewport(FVector2D(0.5f, 0.5f));
				
				if (!WidgetDecorator->IsVisible())
				{
					WidgetDecorator->SetVisibility(ESlateVisibility::Visible);
				}
			}
			else
			{
				if (WidgetDecorator->IsVisible())
				{
					WidgetDecorator->SetVisibility(ESlateVisibility::Hidden);
				}
			}
		}		
	}
}

void UCustomWidgetComponent::OnComponentDestroyed(bool bDestroyingHierarchy)
{
	if (WidgetDecorator.IsValid())
	{
		WidgetDecorator->RemoveFromViewport();
		WidgetDecorator = nullptr;
	}
	Super::DestroyComponent(bDestroyingHierarchy);
}

void UCustomWidgetComponent::SetWidgetAngleFromCameraView()
{
	ABasePlayerController* BasePlayerController = UUtilFunctionLibrary::GetBasePlayerController(this->GetWorld());
	if(BasePlayerController)
	{
		ABaseCameraActor* Camera = BasePlayerController->GetCameraActor();
		if (Camera)
		{
			FRotator lookatRotation = Camera->GetActorRotation();
			//Pitch MasicNumber 90(Character Rot)
			if (UpdateCheck == lookatRotation.Yaw)
				return;

			UpdateCheck = lookatRotation.Yaw;
			FRotator lookatRotation2 = FRotator(90, 0, -lookatRotation.Yaw + 180.0f);
			this->SetRelativeRotation(lookatRotation2);// > AddLocalRotation(lookatRotation2);
		}
	}
}