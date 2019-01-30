// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include "SharedConstants/GlobalConstants.h"
#include "EntityRenderComponent.generated.h"

class UCustomChangeMaterialComponent;
class USlateDamageFloaterComponentV2;
class USelectionIndicatorMeshComponent;
class UWidgetDecoratorComponent;
class USkillAreaIndicatorMeshComponent;
class USkillCastingEffectComponent;

class UEffectComponent;

#define STATUS_COLOR_PARAM TEXT("StatusColor")

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class PROJECTR_API UEntityRenderComponent : public UActorComponent

{
	GENERATED_BODY()

public:
	// Sets default values for this component's properties	
	UEntityRenderComponent(const FObjectInitializer& ObjectInitializer);

	// Called when the game starts
	virtual void InitializeComponent() override;

	virtual void UninitializeComponent() override;
	
	// Called every frameb
	virtual void TickComponent( float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction ) override;

	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

	
	void					RefreshComponent();
	void					AttachHPComponent(ECharacterCategoryTypeEnum CharacterCategory, EBattleObjectTypeEnum BattleObjectType = EBattleObjectTypeEnum::VE_None);
	void					AttachDebugWidgetComponent(bool bForce = false);
	void					AttachRenderComponents();
	void					AttachRenderComponentsAfterAttachSkill();

	virtual void			SetVisible(bool bVisible);
	void					DeadClear();

	/*
	Effect
	*/
	UFUNCTION(BlueprintCallable, Category = EntityRenderComponent)
	virtual UParticleSystemComponent*	AttachEffect(FName id, UParticleSystem* EffectTemplate, FName AttachPointName = NAME_None);
	UFUNCTION(BlueprintCallable, Category = EntityRenderComponent)
	virtual void					RemoveEffect(FName id);
	UFUNCTION(BlueprintCallable, Category = EntityRenderComponent)
	bool							IsAttachEffect(FName id);

	UFUNCTION(BlueprintCallable, Category = EntityRenderComponent)
	UParticleSystemComponent*	AttachDieDestroyEffect(UParticleSystem* EffectTemplate, FName AttachPointName = NAME_None, FVector InLocation = FVector::ZeroVector, FRotator Rotation = FRotator::ZeroRotator, EAttachLocation::Type LocationType = EAttachLocation::KeepRelativeOffset, bool bAutoDestroy = true);
	UFUNCTION(BlueprintCallable, Category = EntityRenderComponent)
	void						RemoveDieDestroyEffect();

	UFUNCTION(BlueprintCallable, Category = EntityRenderComponent)
	virtual void					SetDecoEffect(UParticleSystem* TargetEffect = nullptr);
	UFUNCTION(BlueprintCallable, Category = EntityRender)
	virtual void					SetDecoEffectSocket(UParticleSystem* TargetEffect, FName SocketName);
	UFUNCTION(BlueprintCallable, Category = EntityRenderComponent)
	virtual void					SetDecoBodyParts(USkeletalMeshComponent* TargeMesh);

	void SetPartsView();
	virtual void					PrepareDecoParts(bool InViewer = false);
	virtual void					OutLine(bool bShow);

	virtual void					CharacterChangeOpacity(float Opacity);

	virtual void					SetMaterialColor(FColor InColor);

	virtual void					StartTrail(bool bRight);
	virtual void					EndTrail(bool bRight);


	virtual void					PrepareMeshMaterial();
	virtual void					ClearDecoParticles();

	virtual void					ShowDeadDisapearEvent();

	//Component
	FORCEINLINE UCustomChangeMaterialComponent*				GetCustomMaterialComponent() { return CustomMaterialComponent; }
	FORCEINLINE USlateDamageFloaterComponentV2 *			GetSlateDamageFloaterComponent() { return SlateDamageFloaterComponent; }

	FORCEINLINE USelectionIndicatorMeshComponent*			GetSelectionIndicatorMeshComponent() { return SelectionIndicatorMeshComponent; }
	FORCEINLINE USelectionIndicatorMeshComponent*			GetSynergyMeshComponent() { return SynergyMeshComponent; }
	FORCEINLINE USelectionIndicatorMeshComponent*			GetTargetingMeshComponent() { return TargetingMeshComponent; }
	FORCEINLINE UWidgetDecoratorComponent*					GetHpBarWidgetComponent() { return HpBarWidgetComponent; }
	//FORCEINLINE USkillCastingDecalComponent*				GetSkillCastingDecalComponent() { return SkillCastingDecalComponent; }
	FORCEINLINE USkillCastingEffectComponent*				GetSkillCastingEffectComponent() { return SkillCastingEffectComponent; }
	FORCEINLINE TArray<USkillAreaIndicatorMeshComponent*>&	GetSkillAreaArray() { return SkillAreaArray; }

	virtual FTransform				GetHeadUpTransform();

	void							ClearAllEffectForced();

	void							ShowSkillCastingDecal(ESearchShapeTypeEnum ShapeType, TArray<float>& ShapeScope, int32 CastingRange, float CastingTime, FVector& CastingLocation);

	void SetActiveWidgetComp(bool InVisible);
	void HpBarWidgetDestroy();
	void SetDamageFloaterVisible(bool InVisible);

protected:

	TWeakObjectPtr< AActor >	OwnerActor;

	UPROPERTY()
	UCustomChangeMaterialComponent*	CustomMaterialComponent = nullptr;
	UPROPERTY()
	USlateDamageFloaterComponentV2 * SlateDamageFloaterComponent = nullptr;

	UPROPERTY()
	USelectionIndicatorMeshComponent* SelectionIndicatorMeshComponent = nullptr;
	UPROPERTY()
	USelectionIndicatorMeshComponent* SynergyMeshComponent = nullptr;
	UPROPERTY()
	USelectionIndicatorMeshComponent* TargetingMeshComponent = nullptr;
	
// 	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Cha)
// 	class UHpBarWidgetComponent*		HpBarWidgetComponent2 = nullptr;

	UPROPERTY()
	UWidgetDecoratorComponent* HpBarWidgetComponent = nullptr;
	UPROPERTY()
	UWidgetDecoratorComponent* DebugWidgetComponent = nullptr;

	UPROPERTY()
	TArray<USkillAreaIndicatorMeshComponent*> SkillAreaArray;
	UPROPERTY()
	USkeletalMeshComponent*	DecoMeshComponent = nullptr;
	
	UPROPERTY()
	USkillCastingEffectComponent* SkillCastingEffectComponent = nullptr;

	/*
	Effect
	*/
	UPROPERTY()
	TArray< TWeakObjectPtr<UParticleSystemComponent> >				AttachEffectList;
	UPROPERTY()
	TMap<FName, TWeakObjectPtr<UParticleSystemComponent> >			AttachEffectMap;

	UPROPERTY()
	TArray< TWeakObjectPtr<UParticleSystemComponent> >				DieDestroyEffectList;

	UPROPERTY()
	FTimerHandle			SpawnTimerHandler;

public:

	UPROPERTY()
	TArray<UParticleSystemComponent*>		DecoParticles;

	UPROPERTY()
	TArray<UMaterialInstanceDynamic* >		OriginalMaterialList;
	UPROPERTY()
	TArray<UMaterialInstanceDynamic* >		SubMeshOriginalMaterialList;

	UPROPERTY()
	UParticleSystemComponent*				SightRangeEffect = nullptr;

	/*
	Test Mesh
	*/
	TWeakObjectPtr< USkeletalMeshComponent >		SubDecoMesh;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Battle)
	UArrowComponent*			HeadUpWidget = nullptr;

	

private:
	void PrepareDecoParts_Mesh(TAssetPtr< USkeletalMesh > DecoMesh);
	void PrepareDecoParts_Particle(TAssetPtr< UParticleSystem > InDecoParticle, FName InDecoParticleSocket);
	void PrepareMeshMaterial_Material(TAssetPtr< UMaterialInstance > Material1, TAssetPtr< UMaterialInstance > Material2);
};
