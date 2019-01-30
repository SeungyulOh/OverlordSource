// Fill out your copyright notice in the Description page of Project Settings.

#include "ProjectR.h"
#include "UP_ItemEnchantResult.h"

#include "GlobalIntegrated.h"



#include "Runtime/UMG/Public/Components/WidgetComponent.h"

#include "UI/RWidgetManager.h"
#include "UI/Common/UC_Item_Icons.h"

#include "UI/ItemManagement/UP_ItemEnchantPopup.h"
#include "UI/UI_Common/UP_CommonPopup.h"
#include "UI/Common/UP_CommonPopupSplash.h"
#include "Object/LobbyCameraActor.h"
#include "UtilFunctionIntegrated.h"

void UUP_ItemEnchantResult::NativeConstruct()
{
	Super::NativeConstruct();
	//IsSetItmeIcon = false;
	//GachaItem = nullptr;
	//IsPlaySequence = false;
	if (IsValid(Button_OpenAll))
	{
		Button_OpenAll->OnClicked.RemoveDynamic(this, &UUP_ItemEnchantResult::OnClickOK);
		Button_OpenAll->OnClicked.AddDynamic(this, &UUP_ItemEnchantResult::OnClickOK);
		Button_OpenAll->SetVisibility(ESlateVisibility::Collapsed);
	}
		
	
	LobbyCameraActor = UUtilFunctionLibrary::GetLobbyCameraActor(this);

	URGameInstance* RGameInstance = RGAMEINSTANCE(this);
	if ( IsValid(RGameInstance->EventManager))
	{
		RGameInstance->EventManager->OnSequence_CreateWidget.Clear();
		RGameInstance->EventManager->OnSequence_CreateWidget.AddDynamic(this, &UUP_ItemEnchantResult::OnCreateWidget);
	}
}

void UUP_ItemEnchantResult::NativeDestruct()
{
	URGameInstance* RGameInstance = RGAMEINSTANCE(this);
	if ( IsValid(RGameInstance->EventManager))
	{
		RGameInstance->EventManager->OnSequence_CreateWidget.Clear();
		RGameInstance->EventManager->OnShowGachaUMG.Clear();
		RGameInstance->SequenceManager->ClearSequence();
	}

	if (IsValid(SpawnItem))
	{
		SpawnItem->Destroy();
	}

	if (IsValid(Button_OpenAll))
		Button_OpenAll->SetVisibility(ESlateVisibility::Collapsed);

	ReturnToPreviousEnvironment();
	RGameInstance->RWidgetManager->HideUserWidget(EWidgetBluePrintEnum::WBP_Enchant_Result);

	Super::NativeDestruct();
}

void UUP_ItemEnchantResult::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);
	
	
}

void UUP_ItemEnchantResult::ShowSuccessResult(FITEM_ICON_INFO ResultItemIconInfo)
{
	URGameInstance* RGameInstance = RGAMEINSTANCE(this);
	
		IsSetItmeIcon			= false;
		CurrentItemIconInfo		= ResultItemIconInfo;
		IsSuccessSequence		= true;
		StartSequence(true);
		EnchantResult = EENCHANT_RESULT::SUCCESS;

		
}

void UUP_ItemEnchantResult::ShowFailResult(FITEM_ICON_INFO ResultItemIconInfo)
{
	URGameInstance* RGameInstance = RGAMEINSTANCE(this);
	
	{		
		IsSetItmeIcon			= false;
		CurrentItemIconInfo		= ResultItemIconInfo;
		IsSuccessSequence		= false;
		//RGameInstance->SequenceManager->SpawnSequence(EnchantSequenceID, true);
		StartSequence(false);
		EnchantResult = EENCHANT_RESULT::FAIL;		

		
		//if (IsValid(RGameInstance->RWidgetManager))
		{
			//UUIFunctionLibrary::AddUIEventString(this, EUIEventEnum::VE_ItemEnchant, CurrentItemIconInfo.itemUD);
		}
	}
}

void UUP_ItemEnchantResult::ShowDestructionResult(FITEM_ICON_INFO ResultItemIconInfo, const TArray<FITEM>& FailedReturnMaterials)
{
	MaterialsAfterDestructions = FailedReturnMaterials;
	URGameInstance* RGameInstance = RGAMEINSTANCE(this);
	
	{
		IsSetItmeIcon		= false;
		CurrentItemIconInfo = ResultItemIconInfo;
		IsSuccessSequence	= false;
		//RGameInstance->SequenceManager->SpawnSequence(EnchantSequenceID, true);
		StartSequence(false);
		EnchantResult = EENCHANT_RESULT::DESTRUCTION;	

		UUIFunctionLibrary::AddUIEvent(EUIEventEnum::VE_ItemEnchant);
	}
}

//void UUP_ItemEnchantResult::AttachItem(FName ItemID)
//{
//	FItemTableInfo* ItemTableInfo = UTableManager::GetInstancePtr()->GetCharacterItemRow(ItemID);
//	if( ItemTableInfo == nullptr)
//		return;
//	UClass* weaponClass = nullptr;
//
//	if (!ItemTableInfo->Main_RMeshRef.IsNull())
//	{
//		weaponClass =  UUtilFunctionLibrary::GetWeaponBaseFromAssetID(ItemTableInfo->Main_RMeshRef);
//	}
//	else if ( !ItemTableInfo->Main_LMeshRef.IsNull())
//	{
//		weaponClass = UUtilFunctionLibrary::GetWeaponBaseFromAssetID(ItemTableInfo->Main_LMeshRef);
//	}
//
//	if (weaponClass)
//	{		
//		SpawnItem	= GetWorld()->SpawnActor(weaponClass);
//		FName tagName		= GetSequenceActorTagByItemEquipType(ItemID);
//
//		TArray<AActor*> SequenceActors;
//		if (UUtilFunctionLibrary::FindActorByTag(this->GetWorld(), tagName, SequenceActors))
//		{			
//			AWeaponBase* weaponActor = Cast<AWeaponBase>(SpawnItem);
//			if (IsValid(weaponActor))
//			{
//				AActor* SeqTargetActor = SequenceActors[0];
//				if (SeqTargetActor)
//				{
//					weaponActor->AttachToActor(SeqTargetActor, FAttachmentTransformRules::KeepRelativeTransform);
//				}
//			}
//		}		
//	}
//	
//
//
//}

void UUP_ItemEnchantResult::CallbackShowResult()
{
	OnClickOK();

}

void UUP_ItemEnchantResult::EndSquence(FName SequenceKey)
{
	//this->PlayAni(ConfirmBtnAniName);
	OnClickOK();
}

void UUP_ItemEnchantResult::VisibleSucessInfo()
{
	URGameInstance* RGameInstance = RGAMEINSTANCE(this);
// 	if (!IsValid(RGameInstance))
// 		return;

	//if (IsValid(RGameInstance->EventManager))
	//{
	//	RGameInstance->EventManager->OnClickOK.Clear();
	//	RGameInstance->EventManager->OnClickOK.AddDynamic(this, &UUP_ItemEnchantResult::ReturnEnchant);
	//}
	RGameInstance->RWidgetManager->ShowUserWidgetAutoCreate(EWidgetBluePrintEnum::WBP_Popup_ItemEnchant);

	UUP_ItemEnchantPopup* EnchantPopup = RGameInstance->RWidgetManager->GetUserWidgetT<UUP_ItemEnchantPopup>(EWidgetBluePrintEnum::WBP_Popup_ItemEnchant);
	if (IsValid(EnchantPopup))
	{
		EnchantPopup->CurItemIconInfo = CurrentItemIconInfo;		
		EnchantPopup->RefreshPopup();		
	}
}

void UUP_ItemEnchantResult::VisibleFailedInfo()
{
	URGameInstance* RGameInstance = RGAMEINSTANCE(this);
// 	if (!IsValid(RGameInstance))
// 		return;

	UUP_CommonPopupSplash* CommonPopup = UUIFunctionLibrary::ShowCommonPopupSplash(UUtilFunctionLibrary::GetLocalizedString(EStringTableType::VE_UI, TEXT("UI_Notify")),
		UUtilFunctionLibrary::GetLocalizedString(EStringTableType::VE_SystemMessage, TEXT("Confirm_Message_000000")));

	RGameInstance->GetTimerManager().SetTimer(FailHandle, this, &UUP_ItemEnchantResult::ReturnEnchant, 2.0f);

// 	UUP_CommonPopup* CommonPopup = UUIFunctionLibrary::ShowCommonPopup(
// 		UUtilFunctionLibrary::GetLocalizedString(EStringTableType::VE_UI, TEXT("USK_up_ItemManage_enchant")),
// 		UUtilFunctionLibrary::GetLocalizedString(EStringTableType::VE_SystemMessage, TEXT("Confirm_Message_000000")), ECommonPopupType::VE_OK);
// 
// 	if (IsValid(CommonPopup))
// 	{
// 		CommonPopup->OnClickOK.Clear();
// 		CommonPopup->OnClickOK.AddDynamic(this, &UUP_ItemEnchantResult::ReturnEnchant);
// 	}

	//RGameInstance->RWidgetManager->ShowUserWidgetAutoCreate(EWidgetBluePrintEnum::WBP_Popup_ItemCommon_1, true);

	//UUP_ItemCommonPopup* CommonPopup = RGameInstance->RWidgetManager->GetUserWidgetT<UUP_ItemCommonPopup>(EWidgetBluePrintEnum::WBP_Popup_ItemCommon_1);
	//if (IsValid(CommonPopup))
	//{
	//	if (IsValid(RGameInstance->EventManager))
	//	{
	//		RGameInstance->EventManager->OnClickOK.Clear();
	//		RGameInstance->EventManager->OnClickOK.AddDynamic(this, &UUP_ItemEnchantResult::ReturnEnchant);
	//	}

	//	CommonPopup->SetCommonPopup(ECommonPopupType::VE_OK,
	//		UUtilFunctionLibrary::GetLocalizedString(EStringTableType::VE_UI, TEXT("USK_up_ItemManage_enchant")),
	//		UUtilFunctionLibrary::GetLocalizedString(EStringTableType::VE_SystemMessage, TEXT("Confirm_Message_000000")));
	//}
}

void UUP_ItemEnchantResult::VisibleDestructionInfo()
{
	//URGameInstance* RGameInstance = RGAMEINSTANCE(this);
	//if (!IsValid(RGameInstance))
	//	return;

	//RGameInstance->RWidgetManager->ShowUserWidgetAutoCreate(EWidgetBluePrintEnum::WBP_Popup_ItemGrindResult, true);

	//UUP_ItemGrindResultPopup* EnchantFailPopup = RGameInstance->RWidgetManager->GetUserWidgetT<UUP_ItemGrindResultPopup>(EWidgetBluePrintEnum::WBP_Popup_ItemGrindResult);
	//if (IsValid(EnchantFailPopup))
	//{
	//	//if (IsValid(RGameInstance->EventManager))
	//	//{
	//	//	RGameInstance->EventManager->OnClickOK.Clear();
	//	//	RGameInstance->EventManager->OnClickOK.AddDynamic(this, &UUP_ItemEnchantResult::ReturnEnchant);
	//	//}

	//	TArray<FITEM_ICON_INFO> iconInfoList;
	//	iconInfoList.Reserve(MaterialsAfterDestructions.Num());
	//	for (FITEM& item : MaterialsAfterDestructions)
	//	{
	//		FITEM_ICON_INFO iconinfo;
	//		if (UUIFunctionLibrary::GetItemIconInfoWithUD(GetWorld(), iconinfo, item.itemUD))
	//		{
	//			iconInfoList.Emplace(iconinfo);
	//		}
	//	}

	//	EnchantFailPopup->SetItems(UUtilFunctionLibrary::GetLocalizedString(EStringTableType::VE_UI, TEXT("USK_up_ItemManage_enchant")),
	//		UUtilFunctionLibrary::GetLocalizedString(EStringTableType::VE_SystemMessage, TEXT("Confirm_Message_000001")),
	//		iconInfoList);
	//	
	//}
}

AActor* UUP_ItemEnchantResult::GetItemCard(FName ActorName)
{
	TArray<AActor*> OutActors;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), AActor::StaticClass(), OutActors);

	for (AActor* outActor : OutActors )
	{
		if (IsValid(outActor))
		{
			if (outActor->GetName().Contains(ActorName.ToString()))
				return outActor;
		}
	}

	return nullptr;
}

void UUP_ItemEnchantResult::ReturnToPreviousEnvironment()
{
	if (GetWorld() && GetWorld()->GetFirstPlayerController())
		GetWorld()->GetFirstPlayerController()->SetViewTarget(LobbyCameraActor.Get());
}

void UUP_ItemEnchantResult::StartSequence(bool bSuccess )
{
	URGameInstance* RGameInstance = RGAMEINSTANCE(this);
	
	{
		CurSequenceName = GetSequenceName(bSuccess);
		ABaseSequenceActor* SequenceActor =  RGameInstance->SequenceManager->SpawnSequence(CurSequenceName, true);
		if (IsValid(SequenceActor))
		{
			IsPlaySequence = true;
			SequenceActor->OnSequenceEndEvent.Clear();
			SequenceActor->OnSequenceEndEvent.AddDynamic(this, &UUP_ItemEnchantResult::EndSquence);

			CurSequenceActor = SequenceActor;
		}
	}
}

FName UUP_ItemEnchantResult::GetSequenceName(bool bSuccess)
{
	if (bSuccess)
		return EnchantSuccessSequenceID;

	return EnchantFailSequenceID;
}

void UUP_ItemEnchantResult::OnClickOK()
{
	if (IsValid(Button_OpenAll))
		Button_OpenAll->SetVisibility(ESlateVisibility::Collapsed);

	switch (EnchantResult)
	{
	case EENCHANT_RESULT::FAIL:
		VisibleFailedInfo();
	break;
	case EENCHANT_RESULT::SUCCESS:
		VisibleSucessInfo();
	break;
	case EENCHANT_RESULT::DESTRUCTION:
		VisibleDestructionInfo();
	break;
	default:
	break;
	}

}

void UUP_ItemEnchantResult::ReturnEnchant()
{
	//URGameInstance* RGameInstance = RGAMEINSTANCE(this);
	//if ( IsValid(RGameInstance->RWidgetManager))
	//{
	//	RGameInstance->RWidgetManager->ChangeUIPage(EUIPageEnum::UIPage_ItemManagement);
	//}
	UUIFunctionLibrary::GoContents(EUIMenuIDEnum::VE_DetailHeros);
}

void UUP_ItemEnchantResult::SetItemIconInUserWidget(FITEM_ICON_INFO ResultItemIconInfo)
{	

}

TWeakObjectPtr< UUC_Gacha_Item> UUP_ItemEnchantResult::FindItemCard(FITEM_ICON_INFO ResultItemIconInfo)
{
	AActor* ItemCard = GetItemCard(ItemCardName);

	if (IsValid(ItemCard))
	{
		UWidgetComponent* WidgetComp = UCharacterHelpFunctionLibrary::GetComponent<UWidgetComponent>(ItemCard);

		if (IsValid(WidgetComp))
		{
			UUserWidget* UserWidget = WidgetComp->GetUserWidgetObject();
			if (IsValid(UserWidget))
			{


			}
		}
	}
	return nullptr;
}

FName UUP_ItemEnchantResult::GetSequenceActorTagByItemEquipType(FName ItemID)
{
	FName tagName;
	// TODO: sort out sequence
	/*
	FItemTableInfo* ItemTableInfo = UTableManager::GetInstancePtr()->GetCharacterItemRow(ItemID);
	if (ItemTableInfo)
	{
		EItemEquipTypeEnum itemEquipType = ItemTableInfo->EquipType;

		switch (itemEquipType)
		{		
		case EItemEquipTypeEnum::VE_Sword:
			tagName = FName(TEXT("Sword_Target"));
		break;
		case EItemEquipTypeEnum::VE_Wand:
			tagName = FName(TEXT("Wand_Target"));
		break;
		case EItemEquipTypeEnum::VE_Staff:
			tagName = FName(TEXT("Staff_Target"));
		break;
		case EItemEquipTypeEnum::VE_Dagger:
			tagName = FName(TEXT("Sword_Target"));
		break;				
		case EItemEquipTypeEnum::VE_Bow:
			tagName = FName(TEXT("Bow_Target"));
		break;
		case EItemEquipTypeEnum::VE_Longsword:
			tagName = FName(TEXT("LongSword_Target"));
		break;
		case EItemEquipTypeEnum::VE_Hammer:
			tagName = FName(TEXT("Hammer_Target"));
		break;
		case EItemEquipTypeEnum::VE_Rod:
			tagName = FName(TEXT("Rod_Target"));
		break;
		default:
			tagName = FName(TEXT("Sword_Target"));
		break;
		}

	}
	*/
	return tagName;
}

void UUP_ItemEnchantResult::OnCreateWidget()
{
	if (CurSequenceActor.IsValid())
	{
		CurSequenceActor->Pause();
		OnClickOK();
	}
}