// Fill out your copyright notice in the Description page of Project Settings.

#include "ProjectR.h"
#include "UC_ImageFlow.h"

#include "GlobalIntegrated.h"


#include "UI/Common/UC_QuickToggle.h"
#include "Table/WidgetBluePrintTableInfo.h"
#include "UtilFunctionIntegrated.h"
#include "UtilFunctionIntegrated.h"



void UUC_ImageFlow::NativeConstruct()
{
	Super::NativeConstruct();

	if (IsValid(Button_Image))
	{
		Button_Image->OnClicked.RemoveDynamic(this, &UUC_ImageFlow::OnClick_ButtonImage);
		Button_Image->OnClicked.AddDynamic(this, &UUC_ImageFlow::OnClick_ButtonImage);
	}
}

void UUC_ImageFlow::Init(const TArray<FName>& InImageKeys)
{
	Clear();
	
	TotalImageNumber	= InImageKeys.Num();
	CurrentImageIndex	= 0;
	ImageKeys			= InImageKeys;

	CreateTogglesInHorizontalBox();
	SetQuickToggle();

	auto RGameInstance = RGAMEINSTANCE(this);
	if (IsValid(RGameInstance->EventManager) == false)
	{
		return;
	}

	RGameInstance->EventManager->OnClick_QuickToggle.RemoveDynamic(this, &UUC_ImageFlow::OnClick_QuickToggle);
	RGameInstance->EventManager->OnClick_QuickToggle.AddDynamic(this, &UUC_ImageFlow::OnClick_QuickToggle);

	Refresh();
}

void UUC_ImageFlow::CreateTogglesInHorizontalBox()
{
	auto RGameInstance = RGAMEINSTANCE(this);

	if (BP_HBox_Toggles == nullptr)
	{
		return;
	}
	
	auto WidgetBlueprintTableInfo = UTableManager::GetInstancePtr()->GetWidgetBluePrintRow("WBP_QuickToggle");
	if (WidgetBlueprintTableInfo == nullptr)
	{
		return;
	}

	for (int32 i = 0; i < ImageKeys.Num(); ++i)
	{
		auto QuickToggle = CreateWidget<UWidget>(RGameInstance, WidgetBlueprintTableInfo->GetWidgetClass());
		BP_HBox_Toggles->AddChild(QuickToggle);
	}
}

void UUC_ImageFlow::SetQuickToggle()
{
	QuickToggles.Empty();
	int32 ToggleNumber = BP_HBox_Toggles->GetChildrenCount();
	for (int32 i = 0; i < ToggleNumber; ++i)
	{
		auto ToggleWidget = Cast<UUC_QuickToggle>(BP_HBox_Toggles->GetChildAt(i));
		if (IsValid(ToggleWidget) == false)
		{
			continue;
		}

		ToggleWidget->CreateState(EToggleType::VE_Raid, i);

		QuickToggles.Emplace(ToggleWidget);
	}
}

void UUC_ImageFlow::OnClick_MoveImage(bool bIsLeft)
{
	if (bIsLeft)
	{
		--CurrentImageIndex;
	}
	else
	{
		++CurrentImageIndex;
	}

	if (CurrentImageIndex < 0)
	{
		CurrentImageIndex = TotalImageNumber - 1;
	}

	if (CurrentImageIndex == TotalImageNumber)
	{
		CurrentImageIndex = 0;
	}

	Refresh();
}


void UUC_ImageFlow::OnClick_ButtonImage()
{
	UUIFunctionLibrary::GoContents(EUIMenuIDEnum::VE_Mission);
}

void UUC_ImageFlow::OnClick_ButtonTipRight()
{
	OnClick_MoveImage(false);
}

void UUC_ImageFlow::OnClick_ButtonTipLeft()
{
	OnClick_MoveImage(true);
}

void UUC_ImageFlow::Refresh()
{
	if(BP_Image == nullptr)
	{
		return;
	}

	if (ImageKeys.IsValidIndex(CurrentImageIndex) == false)
	{
		return;
	}

	auto Texture = UUtilFunctionLibrary::GetTexture2DFromTable(ImageKeys[CurrentImageIndex]);
	BP_Image->SetBrushFromTexture(Texture);

	auto RGameInstance = RGAMEINSTANCE(this);
	if (IsValid(RGameInstance->EventManager) == false)
	{
		return;
	}

	RefreshToggles();
	RGameInstance->EventManager->OnClick_AnotherImage.Broadcast();
}

void UUC_ImageFlow::RefreshToggles()
{
	int32 Index = 0;
	for (const auto& ToggleElem : QuickToggles)
	{
		if (CurrentImageIndex == Index)
		{
			ToggleElem->SetActive(true);
		}
		else
		{
			ToggleElem->SetActive(false);
		}
		++Index; 
	}
}

void UUC_ImageFlow::Clear()
{
	auto RGameInstance = RGAMEINSTANCE(this);
	if (IsValid(RGameInstance->EventManager) == false)
	{
		return;
	}

	RGameInstance->EventManager->OnClick_QuickToggle.RemoveDynamic(this, &UUC_ImageFlow::OnClick_QuickToggle);
	
	QuickToggles.Empty();

	if (BP_HBox_Toggles == nullptr)
	{
		return;
	}

	BP_HBox_Toggles->ClearChildren();
}

void UUC_ImageFlow::OnClick_QuickToggle(int32 InToggleIndex)
{
	if (InToggleIndex < 0 || InToggleIndex >= TotalImageNumber || InToggleIndex == CurrentImageIndex)
	{
		return;
	}

	CurrentImageIndex = InToggleIndex;
	Refresh();
}

void UUC_ImageFlow::NativeDestruct()
{
	Clear();

	Super::NativeDestruct();
}