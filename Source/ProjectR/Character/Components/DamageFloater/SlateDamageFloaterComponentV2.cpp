// Fill out your copyright notice in the Description page of Project Settings.
//#if WITH_KOKOMO

#include "ProjectR.h"
#include "PaperSprite.h"
#include "ContentGameMode/Controllers/BasePlayerController.h"

#include "UtilFunctionIntegrated.h"
#include "Algo/Reverse.h"
#include "RHI.h"
#include "SlateDamageFloaterComponentV2.h"

TSharedPtr<SDamageFloaterV2> USlateDamageFloaterComponentV2::DamageFloaterCanvas;
int32 USlateDamageFloaterComponentV2::DamageFloaterCanvasRefCount = 0;

// Sets default values for this component's properties
USlateDamageFloaterComponentV2::USlateDamageFloaterComponentV2()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;

	// ...
}

// Called when the game starts
void USlateDamageFloaterComponentV2::BeginPlay()
{
	Super::BeginPlay();

	if (DamageFloaterCanvas.IsValid() == false)
	{
		SAssignNew(DamageFloaterCanvas, SDamageFloaterV2)
			.BasePlayerController(UUtilFunctionLibrary::GetBasePlayerController(GetOwner()->GetWorld()));
		GEngine->GameViewport->AddViewportWidgetContent(DamageFloaterCanvas.ToSharedRef());
	}
	DamageFloaterCanvasRefCount++;
}

void USlateDamageFloaterComponentV2::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	DamageFloaterCanvasRefCount--;
	if (DamageFloaterCanvasRefCount <= 0)
	{
		if (DamageFloaterCanvas.IsValid())
		{
			DamageFloaterCanvas->CleanStatic();
		}
		if (GEngine != nullptr && GEngine->GameViewport != nullptr)
		{
			GEngine->GameViewport->RemoveViewportWidgetContent(DamageFloaterCanvas.ToSharedRef());
		}
		DamageFloaterCanvas = nullptr;
	}
	Super::EndPlay(EndPlayReason);
}

void USlateDamageFloaterComponentV2::AddDamageFloater(int32 damagecount, int32 addPercent, EBattleDamageResultEnum damageType, float inScale, bool bShowText)
{
	FVector HeadUpLocation;

	auto EntityRenderComponent = UCharacterHelpFunctionLibrary::GetEntityRenderComponent(GetOwner());
	if (EntityRenderComponent)
	{
		if (EntityRenderComponent->HeadUpWidget)
			HeadUpLocation = EntityRenderComponent->HeadUpWidget->GetComponentLocation();
		else
			HeadUpLocation = GetOwner()->GetActorLocation();
	}

	FLinearColor color;
	color.R = 1.0f;
	color.G = 1.0f;
	color.B = 1.0f;
	color.A = 1.0f;

	DamageFloaterCanvas->AddDamageFloater(damageType, damagecount, addPercent, GetOwner()->GetActorLocation(), color, inScale, bShowText);
}

void USlateDamageFloaterComponentV2::TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction * ThisTickFunction)
{
	if (GEngine != nullptr && GEngine->GameViewport != nullptr)
	{

	}

	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

void USlateDamageFloaterComponentV2::SetDamageFloaterVisible(bool InVisible)
{
	if (DamageFloaterCanvas.IsValid())
	{
		if (InVisible)
		{
			//auto Visible = DamageFloaterCanvas->GetVisibility();
			DamageFloaterCanvas->SetVisibility(EVisibility::SelfHitTestInvisible);
		}
		else
		{
			DamageFloaterCanvas->SetVisibility(EVisibility::Collapsed);
		}
	}
}

//#endif