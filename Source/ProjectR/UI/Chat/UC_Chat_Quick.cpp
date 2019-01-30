// Fill out your copyright notice in the Description page of Project Settings.

#include "ProjectR.h"
#include "UC_Chat_Quick.h"
#include "UtilFunctionIntegrated.h"
#include "GlobalIntegrated.h"



void UUC_Chat_Quick::NativeConstruct()
{
	Super::NativeConstruct();

	LoadConfig();

	FText HintText = UUtilFunctionLibrary::GetLocalizedString(EStringTableType::VE_UI, TEXT("UI_Chatting_000024"));

	for (int i = 0; i < TextBox_Inputs.Num(); i++)
	{
		if (Messages.Num() <= i)
		{
			Messages.AddDefaulted();
		}

		TextBox_Inputs[i]->SetText(FText::FromString(Messages[i]));

		TextBox_Inputs[i]->OnTextCommitted.AddDynamic(this, &UUC_Chat_Quick::OnTextCommitted);

		TextBox_Inputs[i]->HintText = HintText;
		TextBox_Inputs[i]->SynchronizeProperties();
	}

	RefreshButtons();
}

void UUC_Chat_Quick::NativeDestruct()
{
	Save();

	for (int i = 0; i < TextBox_Inputs.Num(); i++)
	{
		TextBox_Inputs[i]->OnTextCommitted.Clear();
	}

	Super::NativeDestruct();
}

void UUC_Chat_Quick::OnTextCommitted(const FText& Text, ETextCommit::Type CommitMethod)
{
	RefreshButtons();

	Save();
}

void UUC_Chat_Quick::RefreshButtons()
{
	for (int i = 0; i < FMath::Min(TextBox_Inputs.Num(), Buttons.Num()); i++)
	{
		Buttons[i]->SetIsEnabled(!TextBox_Inputs[i]->GetText().IsEmpty());
	}
}

void UUC_Chat_Quick::Save()
{
	for (int i = 0; i < TextBox_Inputs.Num(); i++)
	{
		if (Messages.Num() <= i)
		{
			Messages.AddDefaulted();
		}

		Messages[i] = TextBox_Inputs[i]->GetText().ToString();
	}
	SaveConfig();
}
