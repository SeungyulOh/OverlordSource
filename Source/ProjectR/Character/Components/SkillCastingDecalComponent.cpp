// Fill out your copyright notice in the Description page of Project Settings.

#include "ProjectR.h"
#include "SkillCastingDecalComponent.h"

#include "GlobalIntegrated.h"
#include "UtilFunctionIntegrated.h"

// Sets default values
USkillCastingDecalComponent::USkillCastingDecalComponent() : Super()
{
	PrimaryComponentTick.bCanEverTick = true;
	
	ScalarParmName = TEXT("SkillRangeProgress");

	UUtilFunctionLibrary::OptimizeStaticMeshComponent(this);
}

// Called when the game starts or when spawned
void USkillCastingDecalComponent::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void USkillCastingDecalComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (false == IsActiveComponent)
		return;

	if (false == OwnerActor.IsValid())
		return;

	if (1.0f == ScalarParm)
	{
		EndSkillCasting();
		return;
	}

	CheckTime += DeltaTime;
	ScalarParm = CheckTime / CastingMaxTime;
	if (1.0f < ScalarParm)
		ScalarParm = 1.0f;

	for (auto MaterialInst : StaticMeshMaterialList)
	{
		if (IsValid(MaterialInst))
		{
			MaterialInst->SetScalarParameterValue(ScalarParmName, ScalarParm);
		}
	}
}

void USkillCastingDecalComponent::SetOwner(AActor* InOwner)
{
	OwnerActor = InOwner;

	if (false == OwnerActor.IsValid())
		return;

	ACharacter* OwnerCharacter = Cast< ACharacter >(OwnerActor.Get());
	if (false == IsValid(OwnerCharacter))
		return;

	ZPos_SkillArea = RGAMEINSTANCE(this)->GameEnvironmentVar.ZPos_SkillArea;
	
	float DefaultHalfHeight = OwnerCharacter->GetDefaultHalfHeight();
	float OffsetZ = ZPos_SkillArea - DefaultHalfHeight;

	CapsuleHeightOffset = FVector(0, 0, OffsetZ);
	SetRelativeLocation(CapsuleHeightOffset);
}

void USkillCastingDecalComponent::StartSkillCasting()
{
	SetVisibility(true);

	IsActiveComponent = true;

	if (Particle_Deco)
	{
		Particle_Deco->SetVisibility(true);
	}
}

void USkillCastingDecalComponent::EndSkillCasting()
{
	SetVisibility(false);

	IsActiveComponent = false;

	if (Particle_Deco)
	{
		Particle_Deco->DestroyComponent();
		Particle_Deco = nullptr;		
	}
}

void USkillCastingDecalComponent::ResetData(float CastingTime)
{
	ScalarParm = 0.0f;

	CheckTime = 0.0f;
	CastingMaxTime = CastingTime;
	IsActiveComponent = false;
}

void USkillCastingDecalComponent::SetShapeType(ESearchShapeTypeEnum InType, float InShapeParam)
{
	if (SkillShapeType == InType && SkillShapeParam == InShapeParam)
		return;

	SkillShapeType = InType;
	SkillShapeParam = InShapeParam;

	FName AssetName;
	
	switch (DecalColorType)
	{
	case ESkillDecalColorType::VE_HeroOwner:
	{
		switch (SkillShapeType)
		{
		case ESearchShapeTypeEnum::VE_Circle:
		{
			AssetName = TEXT("PC_Circle");
			EffectName = AssetName;
		}
		break;
		case ESearchShapeTypeEnum::VE_Box:
		{
			AssetName = TEXT("PC_Ractangle");
			EffectName = EXCEPTION_NONE;
		}
		break;
		case ESearchShapeTypeEnum::VE_Sector:
		{
			if (60.0f == SkillShapeParam) {
				AssetName = TEXT("PC_Sector60");
			}
			else if (90.0f == SkillShapeParam) {
				AssetName = TEXT("PC_Sector90");
			}
			else if (120.0f == SkillShapeParam) {
				AssetName = TEXT("PC_Sector120");
			}
			else if (180.0f == SkillShapeParam) {
				AssetName = TEXT("PC_Sector180");
			}
			EffectName = AssetName;
		}
		break;
		}
	}
	break;
	case ESkillDecalColorType::VE_Npc_Enemy:
	{
		bool bEffect = IsHero;
		switch (SkillShapeType)
		{
		case ESearchShapeTypeEnum::VE_Circle:
		{
			AssetName = TEXT("NPC_Circle");
		}
		break;
		case ESearchShapeTypeEnum::VE_Box:
		{
			AssetName = TEXT("NPC_Ractangle");
			bEffect = false;
		}
		break;
		case ESearchShapeTypeEnum::VE_Sector:
		{
			if (60.0f == SkillShapeParam) {
				AssetName = TEXT("NPC_Sector60");
			}
			else if (90.0f == SkillShapeParam) {
				AssetName = TEXT("NPC_Sector90");
			}
			else if (120.0f == SkillShapeParam) {
				AssetName = TEXT("NPC_Sector120");
			}
			else if (180.0f == SkillShapeParam) {
				AssetName = TEXT("NPC_Sector180");
			}
		}
		break;
		}

		if (bEffect)
		{
			EffectName = AssetName;
		}
	}
	break;
	case ESkillDecalColorType::VE_Npc_Friend:
	{
		bool bEffect = IsHero;
		switch (SkillShapeType)
		{
		case ESearchShapeTypeEnum::VE_Circle:
		{
			AssetName = TEXT("NPC_Friend_Circle");
		}
		break;
		case ESearchShapeTypeEnum::VE_Box:
		{
			AssetName = TEXT("NPC_Friend_Ractangle");
			bEffect = false;
		}
		break;
		case ESearchShapeTypeEnum::VE_Sector:
		{
			if (60.0f == SkillShapeParam) {
				AssetName = TEXT("NPC_Friend_Sector60");
			}
			else if (90.0f == SkillShapeParam) {
				AssetName = TEXT("NPC_Friend_Sector90");
			}
			else if (120.0f == SkillShapeParam) {
				AssetName = TEXT("NPC_Friend_Sector120");
			}
			else if (180.0f == SkillShapeParam) {
				AssetName = TEXT("NPC_Friend_Sector180");
			}
		}
		break;
		}

		if (bEffect)
		{
			EffectName = AssetName;
		}
	}
	break;
	case ESkillDecalColorType::VE_Npc_Akhan:
	{
		switch (SkillShapeType)
		{
		case ESearchShapeTypeEnum::VE_Circle:
		{
			AssetName = TEXT("Raid_Akhan_Circle");
		}
		break;
		}
	}
	break;
	case ESkillDecalColorType::VE_Npc_Gigant:
	{
		switch (SkillShapeType)
		{
		case ESearchShapeTypeEnum::VE_Circle:
		{
			AssetName = TEXT("Raid_Gigant_Circle");
		}
		break;
		}
	}
	break;
	default: 
		break;
	}


	StaticMeshMaterialList.Empty();
	
	UStaticMesh* DecalMesh = UUtilFunctionLibrary::GetStaticMeshFromTable(AssetName);
	if (IsValid(DecalMesh))
	{
		SetStaticMesh(DecalMesh);

		int32 CheckIndex = 0;
		TArray<UMaterialInterface*> DecalMaterials = this->GetMaterials();
		for (auto Material : DecalMaterials)
		{
			if (IsValid(Material))
			{
				UMaterialInstanceDynamic* dynamicInstance = this->CreateDynamicMaterialInstance(CheckIndex, Material);
				if (IsValid(dynamicInstance))
				{
					StaticMeshMaterialList.Emplace(dynamicInstance);
				}
			}

			++CheckIndex;
		}
	}
	else
	{
		SetStaticMesh(nullptr);
	}
}

void USkillCastingDecalComponent::ShowSkillArea(float InRangeX, float InRangeY)
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

	SetRelativeScale3D(TempRange);
}

void USkillCastingDecalComponent::SetCastingLocation(FVector& CastingLocation)
{
	if (CastingLocation.IsNearlyZero())
	{
		SetRelativeLocation(CapsuleHeightOffset + AddAirOffset);
	}
	else
	{
		//TODO: Location different.
		SetWorldLocation(CastingLocation);
	}

	if (Particle_Deco)
	{
		Particle_Deco->DestroyComponent();
		Particle_Deco = nullptr;
	}

	if (EffectName.IsNone() == false)
	{
		UParticleSystem* EmitterTemplate = UUtilFunctionLibrary::GetParticleSystemFromTable(EffectName);
		Particle_Deco = UGameplayStatics::SpawnEmitterAttached(EmitterTemplate, this);		
	}
}