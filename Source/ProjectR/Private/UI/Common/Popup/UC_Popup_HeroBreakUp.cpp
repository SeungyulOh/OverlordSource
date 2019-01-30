// Fill out your copyright notice in the Description page of Project Settings.

#include "ProjectR.h"
#include "UC_Popup_HeroBreakUp.h"
#include "Network/PacketFunctionLibrary.h"




void UUC_Popup_HeroBreakUp::NativeConstruct()
{
	Super::NativeConstruct();

	if (IsValid(Button_Cancel))
		Button_Cancel->OnClicked.AddDynamic(this, &UUC_Popup_HeroBreakUp::OnClickCancel);

	if (IsValid(Button_Ok))
		Button_Ok->OnClicked.AddDynamic(this, &UUC_Popup_HeroBreakUp::OnClickOK);

}

void UUC_Popup_HeroBreakUp::NativeDestruct()
{
	if (IsValid(Button_Cancel))
		Button_Cancel->OnClicked.RemoveDynamic(this, &UUC_Popup_HeroBreakUp::OnClickCancel);

	if (IsValid(Button_Ok))
		Button_Ok->OnClicked.RemoveDynamic(this, &UUC_Popup_HeroBreakUp::OnClickOK);

	Super::NativeDestruct();
}

void UUC_Popup_HeroBreakUp::SetBreakUpList(TArray<FString> list)
{
	if (list.Num() > 0)
		SelectedHeros = list;

	if(Text_SelectedCount)
		Text_SelectedCount->SetText(FText::AsNumber(SelectedHeros.Num()));
}

void UUC_Popup_HeroBreakUp::OnClickCancel()
{
	this->SetVisibility(ESlateVisibility::Collapsed);
}

void UUC_Popup_HeroBreakUp::OnClickOK()
{
	 UPacketFunctionLibrary::INVENTORY_HERO_GRIND_RQ(SelectedHeros);
	 this->SetVisibility(ESlateVisibility::Collapsed);
}
