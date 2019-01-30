// Fill out your copyright notice in the Description page of Project Settings.

#include "ProjectR.h"
#include "UC_TimeCount.h"

#include "Global/RGameInstance.h"
#include "UI/RWidgetManager.h"




void UUC_TimeCount::NativeConstruct()
{
	Super::NativeConstruct();

	Renderer.variables = &Variables;
	Renderer.ParentWidget = this;
	Renderer.Prepare();
}

void UUC_TimeCount::NativeDestruct()
{
	Super::NativeDestruct();
}

void UUC_TimeCount::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);

	Renderer.Render(InDeltaTime);
}

void FRenderer_TimeCount::Prepare()
{
	variables->Image_PickTimer->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
	variables->TextBlock_PickTimer->SetVisibility(ESlateVisibility::SelfHitTestInvisible);

	if (!MID->IsValidLowLevel())
	{
		MID = variables->Image_PickTimer->GetDynamicMaterial();
		MID->SetScalarParameterValue(TEXT("Progress"), 0.f);
		variables->Image_PickTimer->SetBrushFromMaterial(MID);
	}

	ElapsedTime = 0.f;
}

void FRenderer_TimeCount::Render(float DeltaTime)
{
	if (ElapsedTime < ParentWidget->MaxTime)
	{
		ElapsedTime += DeltaTime;
		float Progress = ElapsedTime / ParentWidget->MaxTime;

		if(variables->TextBlock_PickTimer->IsValidLowLevel())
			variables->TextBlock_PickTimer->SetText(FText::AsNumber(int32(ParentWidget->MaxTime - ElapsedTime + 1)));
		
		if(MID->IsValidLowLevel())
			MID->SetScalarParameterValue(TEXT("Progress"), Progress);
	}
	else
	{
		RGAMEINSTANCE(ParentWidget)->RWidgetManager->HideUserWidget(EWidgetBluePrintEnum::WBP_TimeCount);
	}
	
}
