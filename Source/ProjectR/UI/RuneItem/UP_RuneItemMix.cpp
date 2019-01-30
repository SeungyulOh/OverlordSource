// Fill out your copyright notice in the Description page of Project Settings.

#include "ProjectR.h"

#include "GlobalIntegrated.h"



#include "GameInfo/RInventory.h"

#include "UI/RuneItem/UC_RuneScrollItem.h"

#include "UtilFunctionIntegrated.h"

#include "Network/PacketFunctionLibrary.h"

#include "UP_RuneItemMix.h"

void UUP_RuneItemMix::NativeConstruct()
{
	Super::NativeConstruct();
	Count = 0;

	DelegateAdd();

	BaseRuneScrollView->Init();

	SetInValidData();

	SetTextCount();
}
void UUP_RuneItemMix::NativeDestruct()
{
	DelegateRemove();

	Super::NativeDestruct();
}

void UUP_RuneItemMix::SetInValidData()
{
	TArray<int32>* RuneInventory = RGAMEINSTANCE(this)->RInventory->GetRune();

	if (BaseRuneScrollView)
		BaseRuneScrollView->InventoryDataReComposition(*(RuneInventory));

}

void UUP_RuneItemMix::SetTextCount()
{
	int32		MaterialCount	=	Count * 3;
	int32		RunIndex		=	CurrentRuneIndex - 1;

	if(TextBlock_Count)
		TextBlock_Count->SetText(FText::AsNumber(Count));

	if(TextBlock_CurrentCount)
		TextBlock_CurrentCount->SetText(FText::AsNumber(MaterialCount));

	if (CurrentRuneIndex > 0 && CurrentRuneIndex < 20)
	{
		TArray<int32>* idx = RGAMEINSTANCE(this)->RInventory->GetRune();
		if(TextBlock_MaxCount)
			TextBlock_MaxCount->SetText(FText::AsNumber((*idx)[RunIndex]));
	}
	else
	{
		if (TextBlock_MaxCount)
			TextBlock_MaxCount->SetText(FText::AsNumber(0));
	}
}

void UUP_RuneItemMix::DelegateAdd()
{
	if (BaseRuneScrollView)
	{
		BaseRuneScrollView->OnScrollViewSelectedItemData.RemoveDynamic(this, &UUP_RuneItemMix::OnClick_RuneIcon);
		BaseRuneScrollView->OnScrollViewSelectedItemData.AddDynamic(this, &UUP_RuneItemMix::OnClick_RuneIcon);
	}

	if (Button_Up)
	{
		Button_Up->OnClicked.RemoveDynamic(this, &UUP_RuneItemMix::OnClick_CountUp);
		Button_Up->OnClicked.AddDynamic(this, &UUP_RuneItemMix::OnClick_CountUp);
	}
	if (Button_Down)
	{
		Button_Down->OnClicked.RemoveDynamic(this, &UUP_RuneItemMix::OnClick_CountDown);
		Button_Down->OnClicked.AddDynamic(this, &UUP_RuneItemMix::OnClick_CountDown);
	}
	if (Button_Mix)
	{
		Button_Mix->OnClicked.RemoveDynamic(this, &UUP_RuneItemMix::OnClick_Mix);
		Button_Mix->OnClicked.AddDynamic(this, &UUP_RuneItemMix::OnClick_Mix);
	}

// 	RGAMEINSTANCE(this)->EventManager->OnRuneItemMixUpdate.RemoveDynamic(this, &UUP_RuneItemMix::UpdateRuneMix);
// 	RGAMEINSTANCE(this)->EventManager->OnRuneItemMixUpdate.AddDynamic(this, &UUP_RuneItemMix::UpdateRuneMix);
}

void UUP_RuneItemMix::DelegateRemove()
{
	if (BaseRuneScrollView)
		BaseRuneScrollView->OnScrollViewSelectedItemData.RemoveDynamic(this, &UUP_RuneItemMix::OnClick_RuneIcon);
	if (Button_Up)
		Button_Up->OnClicked.RemoveDynamic(this, &UUP_RuneItemMix::OnClick_CountUp);
	if (Button_Down)
		Button_Down->OnClicked.RemoveDynamic(this, &UUP_RuneItemMix::OnClick_CountDown);
	if (Button_Mix)
		Button_Mix->OnClicked.RemoveDynamic(this, &UUP_RuneItemMix::OnClick_Mix);

//	RGAMEINSTANCE(this)->EventManager->OnRuneItemMixUpdate.RemoveDynamic(this, &UUP_RuneItemMix::UpdateRuneMix);
}

void UUP_RuneItemMix::OnClick_RuneIcon(URScrollItemData* InSelectedHeroScrollItemData)
{
	if (IsValid(InSelectedHeroScrollItemData) == false)
		return;

	auto HeroScrollItemData = Cast<URuneScrollItemData>(InSelectedHeroScrollItemData);

	if (IsValid(HeroScrollItemData) == false)
		return;

	Count = 0;
	CurrentRuneIndex = HeroScrollItemData->RuneID;

	SetTextCount();

	FRuneTable*		RuneTable = UTableManager::GetInstancePtr()->GetRuneOptionRowInt(HeroScrollItemData->RuneID);
	FRuneTable*		RuneMixTable = UTableManager::GetInstancePtr()->GetRuneOptionRowInt(HeroScrollItemData->RuneID+1);

	if (RuneTable == nullptr)
		return;

	if (RuneMixTable == nullptr)
		return;

	UObject* object = UUIFunctionLibrary::GetIconImageFromTable(RuneTable->RuneInventoryImage);
	UUIFunctionLibrary::SetImageBrush(object, Image_Rune);

	object = UUIFunctionLibrary::GetIconImageFromTable(RuneMixTable->RuneInventoryImage);
	UUIFunctionLibrary::SetImageBrush(object, Image_MixRune);
}

void UUP_RuneItemMix::OnClick_CountUp()
{
	if (CurrentRuneIndex <= 0 || CurrentRuneIndex >= 20)
		return;

	int32		MaterialCount	=	(Count+1) * 3;
	int32		RunIndex		=	CurrentRuneIndex - 1;

	TArray<int32>* idx = RGAMEINSTANCE(this)->RInventory->GetRune();

	if(MaterialCount <= (*idx)[RunIndex])
		Count++;

	SetTextCount();
}

void UUP_RuneItemMix::OnClick_CountDown()
{
	if (Count > 0)
		Count--;

	SetTextCount();
}

void UUP_RuneItemMix::OnClick_Mix()
{
	if (CurrentRuneIndex <= 0 || CurrentRuneIndex >= 20)
		return;

	FName NameTemp = RGAMEINSTANCE(this)->RInventory->RuneWallet[CurrentRuneIndex];
	
	// TODO: retire this class
	//UPacketFunctionLibrary::INVENTORY_RUNE_MAKE_RQ(NameTemp, Count);
}

void UUP_RuneItemMix::UpdateRuneMix()
{
	Count = 0;
	CurrentRuneIndex = 0;
	SetInValidData();
	SetTextCount();
}
