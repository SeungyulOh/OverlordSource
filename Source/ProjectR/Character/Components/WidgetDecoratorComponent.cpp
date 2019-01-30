// Fill out your copyright notice in the Description page of Project Settings.

#include "ProjectR.h"
#include "WidgetDecoratorComponent.h"

#include "UI/UIDefs.h"
#include "UI/CharacterUI/CharacterUserWidget.h"

#include "UtilFunctionIntegrated.h"
#include "ContentGameMode/Controllers/BasePlayerController.h"
#include "Runtime/UMG/Public/Blueprint/WidgetLayoutLibrary.h"


// Sets default values for this component's properties
UWidgetDecoratorComponent::UWidgetDecoratorComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	
	bWantsInitializeComponent = true;
	
	PrimaryComponentTick.bCanEverTick = true;
	PrimaryComponentTick.bTickEvenWhenPaused = true;

	bUseAttachParentBound = true;
}

void UWidgetDecoratorComponent::InitializeComponent()
{
	Super::InitializeComponent();

	OwnerActor = GetOwner();
	if (OwnerActor.IsValid())
	{
		ActiveWidgetDecorator = true;
		GameViewportSize = UWidgetLayoutLibrary::GetViewportSize(OwnerActor->GetWorld());
		GameResolution = UUIFunctionLibrary::GetGameResolution();
		PlayerController = UUtilFunctionLibrary::GetBasePlayerController(OwnerActor->GetWorld());

		WidgetVisibleSizeMin.X = -200.f;
		WidgetVisibleSizeMin.Y = -200.f;
		WidgetVisibleSizeMax.X = GameViewportSize.X + 200.0f;
		WidgetVisibleSizeMax.Y = GameViewportSize.X + 200.0f;

		CreateWidgetDecorator();
	}
}

void UWidgetDecoratorComponent::UninitializeComponent()
{
	DeleteWidgetDecorator();
	
	Super::UninitializeComponent();
}

void UWidgetDecoratorComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (ActiveWidgetDecorator)
	{
		RefreshWidgetPosition();
	}
}

void UWidgetDecoratorComponent::CreateWidgetDecorator()
{
	if (!WidgetDecoratorID.IsNone())
	{
		WidgetDecorator = UUtilFunctionLibrary::CreateUserWidget(this->GetWorld(), WidgetDecoratorID);
		if (WidgetDecorator.IsValid())
		{
			UCharacterUserWidget* CharacterWidget = Cast< UCharacterUserWidget >(WidgetDecorator.Get());
			if (CharacterWidget)
			{
				CharacterWidget->SetOwner(OwnerActor.Get());
			}
			RefreshWidgetZOrder(true);
			WidgetDecorator->SetVisibility(ESlateVisibility::Collapsed);
		}
	}
}

void UWidgetDecoratorComponent::DeleteWidgetDecorator()
{
	if (WidgetDecorator.IsValid())
	{
		WidgetDecorator->SetVisibility(ESlateVisibility::Collapsed);
		WidgetDecorator->RemoveFromViewport();
		WidgetDecorator = nullptr;
	}
}

void UWidgetDecoratorComponent::SetActiveWidgetDecorator(bool InActive)
{
	ActiveWidgetDecorator = InActive;

	RefreshWidgetPosition();
}

void UWidgetDecoratorComponent::RefreshWidgetZOrder(bool bForce)
{
#ifdef RULE_TESTWIDGET
	if (Init)
		return;
#endif //RULE_TESTWIDGET

	if (!WidgetDecorator.IsValid())
		return;
	if (!OwnerActor.IsValid())
		return;
	if (!bForce && CharacterCategory == ECharacterCategoryTypeEnum::VE_None)
		return;

	int32 CheckOrder = CHARACTERUI_ORDER;

	if (CharacterCategory == ECharacterCategoryTypeEnum::VE_Hero)
	{
		CheckOrder = CHARACTERUI_ORDER + 1;
		if (IsValid(PlayerController))
		{
			if (PlayerController->GetSelectedCharacter() == OwnerActor.Get())
			{
				CheckOrder = CHARACTERUI_ORDER + 2;
			}
		}
	}

	if (WidgetZOrder != CheckOrder)
	{
		WidgetZOrder = CheckOrder;

		if (WidgetDecorator->IsInViewport())
		{
			WidgetDecorator->RemoveFromViewport();
		}
		WidgetDecorator->AddToViewport(WidgetZOrder);
		Init = true;
	}
}

void UWidgetDecoratorComponent::RefreshWidgetPosition()
{
	if (!WidgetDecorator.IsValid())
		return;

	ESlateVisibility ChangeVisible = ESlateVisibility::Collapsed;

	if (ActiveWidgetDecorator)
	{
		if(OwnerActor.IsValid() && IsValid(PlayerController))
		{
			FVector DecoratorComponentLocation = this->GetComponentLocation();
			FVector CameraLocation = FVector::ZeroVector;
			if (IsValid(PlayerController->PlayerCameraManager))
			{
				CameraLocation = PlayerController->PlayerCameraManager->GetCameraLocation();
			}
			float fDistance = FVector::Dist(CameraLocation, DecoratorComponentLocation);
			if (fDistance < 5000.0f)
			{
				FVector2D ScreenLocation;
				if (PlayerController->ProjectWorldLocationToScreen(DecoratorComponentLocation, ScreenLocation))
				{
					if (WidgetVisibleSizeMin.X < ScreenLocation.X && WidgetVisibleSizeMin.Y < ScreenLocation.Y)
					{
						if (ScreenLocation.X < WidgetVisibleSizeMax.X && ScreenLocation.Y < WidgetVisibleSizeMax.Y)
						{
							WidgetDecorator->SetPositionInViewport(ScreenLocation);
							WidgetDecorator->SetAlignmentInViewport(FVector2D(0.5f, 0.5f));
							ChangeVisible = ESlateVisibility::SelfHitTestInvisible;
						}
					}
				}
			}
		}
	}

	if (false == UUtilFunctionLibrary::IsCheatVisibleInGameHPBar())
	{
		WidgetDecorator->SetVisibility(ESlateVisibility::Collapsed);
		return;
	}

	WidgetDecorator->SetVisibility(ChangeVisible);
}

void UWidgetDecoratorComponent::RefreshComponent()
{
	if (WidgetDecorator.IsValid())
	{
		UCharacterUserWidget* CharacterWidget = Cast< UCharacterUserWidget >(WidgetDecorator.Get());
		if (IsValid(CharacterWidget))
		{
			CharacterWidget->RefreshCharacterUserWidget();
		}
	}
}

