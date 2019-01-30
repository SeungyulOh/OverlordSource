// Fill out your copyright notice in the Description page of Project Settings.

#include "ProjectR.h"

#include "Character/Interfaces/EntityBaseProperty.h"

#include "UtilFunctionIntegrated.h"

#include "Skill_V2/SharedSkill/ISkill_V2.h"
#include "ContentGameMode/Controllers/StagePlayerController.h"
#include "Controllers/DPadController.h"
#include "LeaderControlHelper.h"

#include "RSkillSet_New.h"

#include "SkillAreaIndicatorMeshComponent.h"

void USkillAreaIndicatorMeshComponent::SetOwner(AActor* TargetActor)
{
	if (IsValid(TargetActor) == false)
		return;

	OwnerActor = TargetActor;
	
}

void USkillAreaIndicatorMeshComponent::SetSkillAreaMeshType(ESkillAreaMeshTypeEnum InSkillAreaMeshType)
{
	SkillAreaMeshType = InSkillAreaMeshType;
}

void USkillAreaIndicatorMeshComponent::ShowSkillArea(float InRangeX, float InRangeY)
{
	float TempValueX = 1.0f;
	float TempValueY = 1.0f;
	if (InRangeX > 0)
	{
		TempValueX = InRangeX * 0.01f;// / 100;
	}
	if (InRangeY > 0)
	{
		TempValueY = InRangeY * 0.01f;// / 100;
	}
	FVector TempRange(TempValueX, TempValueY, 1);

	this->SetRelativeScale3D(TempRange);
	
}

void USkillAreaIndicatorMeshComponent::SkillAreaMeshUpdate(FVector& InWorldPos)
{
	if (SkillShapeType == ESearchShapeTypeEnum::VE_Sector || 
		SkillShapeType == ESearchShapeTypeEnum::VE_Box)
	{
		FVector Location = OwnerActor->GetActorLocation();
		SetWorldLocation(FVector(Location.X , Location.Y , InWorldPos.Z));
	}
	else
		SetWorldLocation(InWorldPos);



	if (SkillAreaMeshType == ESkillAreaMeshTypeEnum::VE_SkillPickArea)
	{
		//todo othertype
		if (SkillShapeType == ESearchShapeTypeEnum::VE_Circle
			|| SkillShapeType == ESearchShapeTypeEnum::VE_Line
			|| SkillShapeType == ESearchShapeTypeEnum::VE_Box
			|| SkillShapeType == ESearchShapeTypeEnum::VE_Sector)
		{
			ShowAreaSkillTarget(InWorldPos);
		}
	}
	else
	{
		ShowAreaSkillTarget(InWorldPos);
	}

}
void USkillAreaIndicatorMeshComponent::SetLocationOriginal()
{
	if (OwnerActor.IsValid())
	{
		SetWorldLocation(OwnerActor->GetRootComponent()->GetComponentLocation());
		SetWorldRotation(FRotator(0, 0, 0));
		ACharacter* OwnerCharacter = Cast< ACharacter >(OwnerActor.Get());
		if (OwnerCharacter)
		{
#ifdef RULE_TESTDELETE
			SetRelativeLocation(FVector(0.0f, 0.0f, -(OwnerCharacter->GetDefaultHalfHeight() * 0.5f)));
#else
			SetRelativeLocation(FVector(0.0f, 0.0f, -(OwnerCharacter->GetDefaultHalfHeight() / 2.0f)));
#endif
		}
		SetTintColor(FLinearColor(0.247f, 1.0f, 0.463f, 1.0f));
	}
}

void USkillAreaIndicatorMeshComponent::SetTintColor(FLinearColor InColor)
{
	if(SelectionMaterialInstance)
		SelectionMaterialInstance->SetVectorParameterValue(FName(TEXT("TintColor")), InColor);
}

void USkillAreaIndicatorMeshComponent::ShowAreaSkillTarget(const FVector& InTargetLocation)
{
	if (OwnerActor.IsValid() == false)
		return;

	auto OwnerActor_SkillEntityComponent = UCharacterHelpFunctionLibrary::GetSkillEntityComponent(OwnerActor.Get());
	if (OwnerActor_SkillEntityComponent == nullptr)
		return;

	ClearAreaSkillTarget();
	
	AStagePlayerController* BasePlayerController = Cast<AStagePlayerController>(UUtilFunctionLibrary::GetBasePlayerController(GetWorld()));
	if (BasePlayerController == nullptr)
		return;

	URSkillSet_New* skillset = UUIFunctionLibrary::GetSkillSet(OwnerActor->GetWorld());
	UISkill_V2* Skill = skillset->GetCurrentSkill();
	if (IsValid(Skill) == false)
		return;
	
	auto SkillComp = UCharacterHelpFunctionLibrary::GetSkillEntityComponent(OwnerActor.Get());
	if (!IsValid(SkillComp))
		return;

	Skill->SetTargetLocation(InTargetLocation);
	SkillComp->GetSkillTarget(Prev_TargetActors, Skill);

	static FColor ChangeColor(80, 180, 255, 0);
	for (auto TargetActorElem : Prev_TargetActors)
	{
		if (TargetActorElem.IsValid() == false)
			continue;

		auto TargetActor_EntityRenderComponent = UCharacterHelpFunctionLibrary::GetEntityRenderComponent(TargetActorElem.Get());
		if (TargetActor_EntityRenderComponent == nullptr)
			continue;
		
		TargetActor_EntityRenderComponent->SetMaterialColor(ChangeColor);

	}

	if (Prev_TargetActors.Num() == 0)
		return;

	TArray< USkillAreaIndicatorMeshComponent* >	TargetComponentArray = UCharacterHelpFunctionLibrary::GetSkillAreaArray(OwnerActor.Get());
	for (auto& Element : TargetComponentArray)
	{
		bool IsPickArea = (Element && Element->GetSkillAreaMeshType() == ESkillAreaMeshTypeEnum::VE_SkillPickArea);
		if (IsPickArea == false)
			continue;

		if (Element->GetSkillShapeType() == ESearchShapeTypeEnum::VE_Line)
			Element->SetVisibility(true);
	}
}

void USkillAreaIndicatorMeshComponent::ClearAreaSkillTarget()
{
	static FColor DefaultColor(0, 0, 0, 0);
	for (auto TargetActorElem : Prev_TargetActors)
	{
		if (TargetActorElem.IsValid() == false)
			continue;
		
		auto TargetActor_EntityRenderComponent = UCharacterHelpFunctionLibrary::GetEntityRenderComponent(TargetActorElem.Get());
		if (TargetActor_EntityRenderComponent == nullptr)
			continue;
		
		TargetActor_EntityRenderComponent->SetMaterialColor(DefaultColor);
	}
	
	TArray< USkillAreaIndicatorMeshComponent* >	TargetComponentArray = UCharacterHelpFunctionLibrary::GetSkillAreaArray(OwnerActor.Get());
	for (auto& Element : TargetComponentArray)
	{
		bool IsPickArea = (Element && Element->GetSkillAreaMeshType() == ESkillAreaMeshTypeEnum::VE_SkillPickArea);
		if (IsPickArea == false)
			continue;

		if (Element->GetSkillShapeType() == ESearchShapeTypeEnum::VE_Line)
			Element->SetVisibility(false);
	}
		
	Prev_TargetActors.Reset();

	
}

void USkillAreaIndicatorMeshComponent::SetShapeType(ESearchShapeTypeEnum InType, float InShapeParam)
{
	if (SkillShapeType == InType
		&& SkillShapeParam == InShapeParam)
		return;

	SkillShapeType = InType;
	SkillShapeParam = InShapeParam;

	FName AssetName;
	switch (InType)
	{
		case ESearchShapeTypeEnum::VE_Cylinder:
		case ESearchShapeTypeEnum::VE_Circle:
		{
			AssetName = STATICM_CIRCLE;
			SetVisibility(true);
		}
		break;
		case ESearchShapeTypeEnum::VE_Sector2D:
		case ESearchShapeTypeEnum::VE_Sector:
		{
			if(InShapeParam == 60.0f)
				AssetName = TEXT("Sector60");
			else if (InShapeParam == 90.0f)
				AssetName = TEXT("Sector90");
			else if (InShapeParam == 120.0f)
				AssetName = TEXT("Sector120");
			else if (InShapeParam == 180.0f)
				AssetName = TEXT("Sector180");
			else
				AssetName = STATICM_CIRCLE;
			SetVisibility(true);
		}
		break;
		case ESearchShapeTypeEnum::VE_Box:
		case ESearchShapeTypeEnum::VE_Box2D:
		{
			AssetName = TEXT("Box");
			SetVisibility(true);
		}
		break;
		case ESearchShapeTypeEnum::VE_Line:
		{
			AssetName = TEXT("Line");
			SetVisibility(true);
		}
		break;
		default:
		{
			SetVisibility(false);
		}
		break;
	}
	if(AssetName.IsNone())
	{
		this->SetStaticMesh(nullptr);
	}
	else
	{
		UStaticMesh* SkillPickAreaMesh = UUtilFunctionLibrary::GetStaticMeshFromTable(AssetName);
		if (SkillPickAreaMesh)
		{
			this->SetStaticMesh(SkillPickAreaMesh);
		}
		else
		{
			this->SetStaticMesh(nullptr);
		}
	}
}

