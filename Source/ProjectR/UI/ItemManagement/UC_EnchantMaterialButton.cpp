// Fill out your copyright notice in the Description page of Project Settings.

#include "ProjectR.h"
#include "UC_EnchantMaterialButton.h"


#include "Table/CurrencyPropertyTableInfo.h"
#include "UtilFunctionIntegrated.h"
#include "UI/Common/UC_ItemIcon.h"


void UUC_EnchantMaterialButton::NativeConstruct()
{
	Super::NativeConstruct();

	Button_Material->OnClicked.AddDynamic(this, &UUC_EnchantMaterialButton::OnButtonMaterialClicked);

	if(IsValid(Text_NeededAmount))
		SavedMaterialColor = Text_NeededAmount->ColorAndOpacity;
}

void UUC_EnchantMaterialButton::NativeDestruct()
{
	if (IsValid(Button_Material))
		Button_Material->OnClicked.RemoveAll(this);

	Super::NativeDestruct();
}

void UUC_EnchantMaterialButton::InvalidateData()
{
	Super::InvalidateData();

	bool IsEnabled = MaterialHaveAmount >= MaterialNeedAmount;
	IsEnabled &= !IsDisabled;

	if(IsValid(Text_OwnedAmount))
		Text_OwnedAmount->SetText(FText::AsNumber(MaterialHaveAmount));
	if (IsValid(Text_NeededAmount))
	{
		Text_NeededAmount->SetText(FText::AsNumber(MaterialNeedAmount));
		Text_NeededAmount->SetColorAndOpacity(IsEnabled ? SavedMaterialColor : Color_InsufficientMaterial);
	}
		

	
	if(IsValid(Button_Material))
		Button_Material->SetIsEnabled(IsEnabled);

	if(IsValid(Image_Disabled))
		Image_Disabled->SetVisibility(IsEnabled ? ESlateVisibility::Collapsed : ESlateVisibility::SelfHitTestInvisible);

	SetSelect(false);
}

void UUC_EnchantMaterialButton::SetSelect(bool IsOn)
{
	IsSelected = IsOn;

	MaterialIcon->Refresh(MaterialIconKey, MaterialHaveAmount, IsOn, 0);
}

bool UUC_EnchantMaterialButton::GetIsSelected()
{
	return IsSelected;
}

bool UUC_EnchantMaterialButton::GetIsEnabled()
{
	return Button_Material->GetIsEnabled();
}

void UUC_EnchantMaterialButton::OnButtonMaterialClicked()
{
	// toggle
	SetSelect(!IsSelected);

	OnButtonToggled.Broadcast(IsSelected);
}
