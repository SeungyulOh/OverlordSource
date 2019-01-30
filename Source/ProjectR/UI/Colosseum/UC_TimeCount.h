// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/RBaseWidget.h"
#include "UC_TimeCount.generated.h"



USTRUCT(BlueprintType)
struct FVariable_TimeCount
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Variables")
	UImage* Image_PickTimer;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Variables")
	UTextBlock* TextBlock_PickTimer;

};

USTRUCT(BlueprintType)
struct FRenderer_TimeCount
{
	GENERATED_USTRUCT_BODY()

	void Prepare();
	void Render(float DeltaTime);

	float ElapsedTime = 0.f;

	FVariable_TimeCount* variables;

	UPROPERTY()
	UMaterialInstanceDynamic* MID;
	UPROPERTY()
	class UUC_TimeCount* ParentWidget;

};

/**
 * 
 */
UCLASS()
class PROJECTR_API UUC_TimeCount : public URBaseWidget
{
	GENERATED_BODY()
	
public:
	void NativeConstruct() override;
	void NativeDestruct() override;
	void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;

	
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UUC_TimeCount")
	FVariable_TimeCount Variables;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UUC_TimeCount")
	FRenderer_TimeCount Renderer;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UUC_TimeCount")
	float MaxTime = 3.f;
};
