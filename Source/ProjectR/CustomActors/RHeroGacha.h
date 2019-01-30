// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Network/PacketDataStructures.h"
#include "RHeroGacha.generated.h"

USTRUCT()
struct FPostProcessTimer
{
	GENERATED_USTRUCT_BODY()

	enum PostProcessType { BLOOM, FADE, END };

	PostProcessType PPType;
	UCurveFloat* Curve;
	float LifeTime;
	float ElapsedTime;
	float InitialValue;
};


UENUM(BlueprintType)
enum class EGradeType : uint8
{
	VE_NORMAL,
	VE_SPECIAL_HERO,
	VE_SPECIAL_SPIRIT,
	VE_Max,
};

UCLASS()
class PROJECTR_API ARHeroGachaCard : public AActor
{
	GENERATED_BODY()
public:
	ARHeroGachaCard();

protected:
	virtual void BeginPlay() override;

public:
	virtual void Tick(float DeltaTime) override;

public:
	void Init();

	UFUNCTION()
	void CallBack_AfterSpecialAction();
	void Binding_SpecialSequence();

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SkeletalComponent")
	USkeletalMeshComponent* SkeletalMeshComponent;

	UPROPERTY(EditAnywhere , BlueprintReadWrite, Category = "BoxComponent")
	UBoxComponent*			BoxComponent;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "BoxComponent")
	USceneComponent*		RootComp;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ParticleSystemComponent")
	UParticleSystemComponent*	PS_LeftTop;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ParticleSystemComponent")
	UParticleSystemComponent*	PS_RightTop;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ParticleSystemComponent")
	UParticleSystemComponent*	PS_LeftBottom;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ParticleSystemComponent")
	UParticleSystemComponent*	PS_RightBottom;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ParticleSystemComponent")
	UParticleSystemComponent*	PS_Body;

	UPROPERTY()
	UMaterialInstanceConstant* Mat_Special;

	UPROPERTY()
	TArray<UAnimSequence*>	AnimSeqArray;
	UPROPERTY()
	TArray<UAnimSequence*>	AnimIdleArray;

	UPROPERTY()
	TArray<UParticleSystem*> PSArray_SpecialHero_Idle;
	UPROPERTY()
	TArray<UParticleSystem*> PSArray_SpecialSpirit_Idle;
	UPROPERTY()
	TArray<UParticleSystem*> PSArray_NormalHero_Idle;
	UPROPERTY()
	TArray<UParticleSystem*> PSArray_NormalSpirit_Idle;
	
	UPROPERTY()
	TArray<UParticleSystem*> PSArray_SpecialSpirit_Run;
	UPROPERTY()
	TArray<UParticleSystem*> PSArray_SpecialHero_Run;
	
	UPROPERTY()
	TArray<UParticleSystem*> PSArray_Body;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Widget")
	class URWidgetComponent*		WidgetComp = nullptr;

	

	UPROPERTY()
	TArray<FPostProcessTimer> PPTimerArray;

	UPROPERTY()
	FHERO					CurrentHero;

	UPROPERTY()
	EGradeType				eGradeType = EGradeType::VE_Max;

	bool					isSelected = false;

	
};

UCLASS()
class PROJECTR_API ARHeroGacha : public AActor
{
	GENERATED_BODY()
public:
	enum GACHATYPE {FIVE, TEN, ELEVEN , ONE , MAX};
public:	
	// Sets default values for this actor's properties
	ARHeroGacha();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION()
	void CallBack_OnTouch(ETouchIndex::Type TouchIndex, FVector Location);

	
	void Init();
	
	void SetGachaType(GACHATYPE InType);
	void PlayNextAnimation();
	void OpenAllCard();
	bool CheckAllSelected();



public:
	UPROPERTY()
	USkeletalMeshComponent* SkeletalMeshComponent;
	
	UPROPERTY()
	TArray<USkeletalMesh*> SkeletalMeshArray;

	UPROPERTY()
	TArray<UAnimSequence*> AnimSequenceArray;

	UPROPERTY()
	TArray<ARHeroGachaCard*>	HeroCardArray;

	UPROPERTY()
	TArray<FHERO>	GachaHeroList;

	UPROPERTY()
	TWeakObjectPtr<AActor>	PrevCameraActor;
	

	GACHATYPE eGachaType;

	bool isReady_Select = false;

	bool isPlayingSpecialCard = false;

	
	
};
