// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "SharedConstants/GlobalConstants.h"
#include "StagePlayerController.h"
#include "TestPlayerController.generated.h"
/**
 * 
 */
UCLASS()
class PROJECTR_API ATestPlayerController : public AStagePlayerController
{
	GENERATED_BODY()
public:
	ATestPlayerController(const FObjectInitializer& ObjectInitializer);


	virtual void				BeginPlay() override;
	virtual void				TickActor(float DeltaTime, enum ELevelTick TickType, FActorTickFunction& ThisTickFunction) override;
	virtual void				EndPlay(const EEndPlayReason::Type EndPlayReason) override;

	UFUNCTION(BlueprintCallable, Category = TestPlayerController)
	void								ActorTest();
	UFUNCTION()
	void								EditorTestKey(FKey InKey);
	UFUNCTION()
	void								EditorUseZoomInOut(float InOutValue);
	UFUNCTION()
	void								ShowDebugPanel();
	UFUNCTION()
	void								EditorActiveSkill1();
	UFUNCTION()
	void								EditorActiveSkill2();
	UFUNCTION()
	void								EditorActiveSkill3();
	UFUNCTION()
	void								EditorNormalAttack();
	UFUNCTION()
	void								TempDelayTestPlay();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = TestPlayerController)
	TArray<FName>						CommandTestInfo;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = TestPlayerController)
	TArray<ECharacterCategoryTypeEnum>	CommandTestSlotCategory;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = TestPlayerController)
	TArray<int32>						CommandTestSlotLevel;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = TestPlayerController)
	TArray<int32>						CommandTestSlotGrade;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = TestPlayerController)
	int32								DebugDrawLineColorIndex = 0;
};

