// Fill out your copyright notice in the Description page of Project Settings.

#include "ProjectR.h"
#include "UC_Chat_BanList_ScrollData.h"
#include "UC_Chat_BanList_ScrollItem.h"
#include "GlobalIntegrated.h"
#include "UtilFunctionIntegrated.h"


void UUC_Chat_BanList_ScrollItem::NativeConstruct()
{
	Super::NativeConstruct();

	URGameInstance* RGameInstance = RGAMEINSTANCE(this);
	ChatManager = RGameInstance->ChatManager;
}

void UUC_Chat_BanList_ScrollItem::Unblock()
{
	auto Data = Cast<UUC_Chat_BanList_ScrollData>(ItemData);

	ChatManager->UnblockUser(Data->kId);
}


void UUC_Chat_BanList_ScrollItem::InvalidateData()
{
	auto Data = Cast<UUC_Chat_BanList_ScrollData>(ItemData);

	if (IsValid(Text_Name))
	{
		Text_Name->SetText(FText::FromString(Data->who));
	}
}
