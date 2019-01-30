// Fill out your copyright notice in the Description page of Project Settings.

#include "ProjectR.h"
#include "PlayerCharacter.h"
#include "SharedConstants/GlobalConstants.h"
#include "Character/Components/FunctionalComponents/EntityRenderComponent.h"
#include "Character/Components/FunctionalComponents/BattleEntityComponent.h"

APlayerCharacter::APlayerCharacter(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
}

void APlayerCharacter::BeginPlay()
{
	Super::BeginPlay();
}

void APlayerCharacter::Destroyed()
{
#ifndef RULE_TESTDELETE
	if (BattleEntityComponent)
	{
		BattleEntityComponent->OnReceiveDamageDelegate.Clear();
		BattleEntityComponent->OnHitDelegate.Clear();
		BattleEntityComponent->OnNotifyDamageDelegate.Clear();
		BattleEntityComponent->OnReceiveHealDelegate.Clear();
	}
#endif

	Super::Destroyed();
}

void APlayerCharacter::Tick(float DeltaSecond)
{
	Super::Tick(DeltaSecond);

#if WITH_EDITOR
	if (ShowBoundingCircle)
	{
		DrawDebugBoundingCircle();
	}
	if (ShowDebugingPanel)
	{
		if (EntityRenderComponent)
		{
			EntityRenderComponent->AttachDebugWidgetComponent(true);
		}
	}
#endif

}

void APlayerCharacter::DrawDebugBoundingCircle()
{
	if (BattleEntityComponent)
	{
		//bool PersistentLines, float LifeTime, uint8 DepthPriority, float Thickness, FVector YAxis, FVector ZAxis, bool bDrawAxis
		//DrawDebugCircle(GetWorld(), GetActorLocation(), BattleEntityComponent->GetBoundingRadius(), (int32)32, FColor::Red, false, -1.0f, 0, 10.0f, FVector::UpVector, FVector::RightVector, true );
		FMatrix TM;
		TM.SetOrigin(GetActorLocation());		
		TM.SetAxis(1, FVector(1, 0, 0));
		TM.SetAxis(2, FVector(0, 1, 0));
		TM.SetAxis(0, FVector(0, 0, 1));
		DrawDebugCircle(GetWorld(), TM, BattleEntityComponent->GetBoundingRadius(), (int32)32, FColor::Red );
	}
	//DrawDebugPoint(GetWorld(), GetActorLocation(), 100.0f, FColor::Red);
}




//----------------------------------------------------------------------------------------
// Battle base characet interface
//----------------------------------------------------------------------------------------
void APlayerCharacter::AttachUI()
{
	Super::AttachUI();
	
//#ifndef RULE_TESTDELETE && !RULE_SUBMARINE
	if (EntityRenderComponent)
	{
		EntityRenderComponent->AttachHPComponent(GetCharacterCategory());
		EntityRenderComponent->AttachRenderComponents();
#if WITH_EDITOR
		EntityRenderComponent->AttachDebugWidgetComponent();
#endif
	}
//#endif
}