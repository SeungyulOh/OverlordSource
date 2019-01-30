// Fill out your copyright notice in the Description page of Project Settings.

#include "ProjectR.h"
#include "GlobalIntegrated.h"

#include "UtilFunctionIntegrated.h"
#include "UC_Tooltip.h"

#include "GameInfo/UserInfo.h"

#include "Table/ItemTableInfo.h"
#include "Table/CharacterTableInfo.h"

#include "ToolTipList/ToolTipBaseEvent.h"
#include "Runtime/UMG/Public/Blueprint/WidgetLayoutLibrary.h"

#include "UI/Common/UC_Item_Icons.h"
#include "UI/Common/Renewal_BaseHeroIcon.h"
#include "UI/Common/BaseMonsterIcon.h"
#include "UI/UI_Battle/Component/RBuffIcon.h"

void UUC_Tooltip::NativeConstruct()
{
	Super::NativeConstruct();

	FindChildWidgetsWithCertainName<URBuffIcon>(this, WeakIconList, TEXT("WeakBuffIcon_"));
	FindChildWidgetsWithCertainName<URBuffIcon>(this, ImmuneIconList, TEXT("ImmuneIcon_"));

	this->SetVisibility(ESlateVisibility::HitTestInvisible);
}


void UUC_Tooltip::SetTooltipEvent(UTooltipBaseEvent* tooltip, FVector2D InPos)
{
// 	TooltipBaseEvent = tooltip;
// 	LastPos = InPos;
// 
// 	if (IsValid(TooltipBaseEvent))
// 	{
// 		if (IsValid(NameText))
// 		{
// 			NameText->SetText(TooltipBaseEvent->GetNameText());
// 		}
// 
// 		if (IsValid(DescriptionText))
// 		{
// 			DescriptionText->SetText(TooltipBaseEvent->GetDescriptionText());
// 		}
// 
// 		if (IsValid(ItemText))
// 		{
// 			//ItemText->SetVisibility(ESlateVisibility::Collapsed);
// 			ItemText->SetText(TooltipBaseEvent->GetToolTipTypeText());
// 		}
// 
// 		if (IsValid(NumPanelWidget))
// 		{
// 			NumPanelWidget->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
// 		}
// 
// 		if (IsValid(InfoWidgetSwitcher))
// 		{
// 			InfoWidgetSwitcher->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
// 		}
// 
// 		FName id = TooltipBaseEvent->GetEventID();
// 
// 		EToolTipEventTypeEnum curType = TooltipBaseEvent->GetTooltipEventType();
// 
// 		switch (curType)
// 		{
// 		case EToolTipEventTypeEnum::VE_Item:
// 		case EToolTipEventTypeEnum::VE_ItemDetail:		
// 		{
// 			if (IsValid(IconTypeSwitcher))
// 			{
// 				IconTypeSwitcher->SetActiveWidgetIndex(0);
// 			}
// 			
// 			if (IsValid(InfoWidgetSwitcher))
// 			{
// 				InfoWidgetSwitcher->SetActiveWidgetIndex(1);
// 			}
// 
// 			FItemTableInfo* ItemTableInfo = UTableManager::GetInstancePtr()->GetCharacterItemRow(id);
// 			if (ItemTableInfo)
// 			{
// 				if (IsValid(ItemIcon))
// 				{
// 					FITEM_ICON_INFO info;
// 					if (UUIFunctionLibrary::GetItemIconInfoWithID(this, info, id))
// 					{
// 						//info.count = UUtilFunctionLibrary::GetCurrentHaveItemCount(GetWorld(), id);
// 						info.count = 0;
// 						ItemIcon->SetItemIconStructure(info);
// 					}
// 				}
// 
// 				if (IsValid(NumText))
// 				{
// 					int32 count = UUtilFunctionLibrary::GetCurrentHaveItemCount(GetWorld(), id);
// 					NumText->SetText(FText::AsNumber(count));
// 				}
// 			}
// 		}
// 		break;
// 		case EToolTipEventTypeEnum::VE_PreviewItem:
// 		{
// 			if (IsValid(IconTypeSwitcher))
// 			{
// 				IconTypeSwitcher->SetActiveWidgetIndex(0);
// 			}
// 
// 			if (IsValid(InfoWidgetSwitcher))
// 			{
// 				InfoWidgetSwitcher->SetVisibility(ESlateVisibility::Collapsed);
// 			}
// 
// 			if (IsValid(ItemIcon))
// 			{
// 				FITEM_ICON_INFO info;
// 				if (UUIFunctionLibrary::GetItemPreviewIconInfoWithID(info, id))
// 				{
// 					info.count = 0;
// 					ItemIcon->SetItemIconStructure(info);
// 				}
// 			}
// 		}
// 		break;
// 		case EToolTipEventTypeEnum::VE_Goods:
// 		{
// 			if (IsValid(IconTypeSwitcher))
// 			{
// 				IconTypeSwitcher->SetActiveWidgetIndex(0);
// 			}
// 
// 			if (IsValid(InfoWidgetSwitcher))
// 			{
// 				InfoWidgetSwitcher->SetActiveWidgetIndex(1);
// 			}
// 
// 			FITEM_ICON_INFO info;
// 			info.enchant = 0;
// 			info.enchantValue = 0;
// 			info.tuneCnt = 0;
// 			info.count = 0;
// 			info.bEquiped = false;			
// 			info.bCompare = false;
// 			info.bCompareUp = true;
// 
// 			info.itemGrade = EItemGradeEnum::VE_None;
// 			info.ItemEquipType = EItemEquipTypeEnum::VE_EveryOne;
// 			info.ItemEquipPosition = EItemEquipPositionEnum::VE_EquipPositionMax;
// 
// 			info.ToolTipEventType = EToolTipEventTypeEnum::VE_None;
// 
// 			int32 count = 0;
// 			FCurrencyPropertyTableInfo* CurrencyPropertyTableInfo = UTableManager::GetInstancePtr()->GetCurrencyPropertyRow(id);
// 			if (CurrencyPropertyTableInfo)
// 			{				
// 				if (!CurrencyPropertyTableInfo->ItemIconKey.IsNone())
// 				{
// 					info.ItemIconRef = UUIFunctionLibrary::GetIconImageFromTable(CurrencyPropertyTableInfo->ItemIconKey);
// 				}				
// 
// 				URGameInstance* RGameInstance = RGAMEINSTANCE(this);
// 				if ( IsValid(RGameInstance->UserInfo))
// 				{
// 					switch (CurrencyPropertyTableInfo->CurrencyType)
// 					{
// 					case EREWARD_TYPE::GOLD:
// 					{
// 						count = RGameInstance->RInventory->GetGold();
// 					}
// 					break;
// 					case EREWARD_TYPE::GEM:
// 					{
// 						count = RGameInstance->RInventory->GetGem();
// 					}
// 					break;
// 					case EREWARD_TYPE::STAMINA:
// 					{
// 						count = RGameInstance->RInventory->GetStamina();
// 					}
// 					break;
// // 					case EREWARD_TYPE::SWEEPER:
// // 					{
// // 						count = RGameInstance->UserInfo->Avatar.sweeper;
// // 					}
// // 					break;
// // 					case EREWARD_TYPE::REDSTONE:
// // 					{
// // 						count = RGameInstance->UserInfo->Avatar.redstone;
// // 					}
// // 					break;
// // 					case EREWARD_TYPE::PVPTOKEN:
// // 					{
// // 						count = RGameInstance->UserInfo->Avatar.pvptoken;
// // 					}
// // 					break;
// // 					case EREWARD_TYPE::EP1:
// // 					{
// // 						count = RGameInstance->UserInfo->Avatar.ep1;
// // 					}
// // 					break;
// // 					case EREWARD_TYPE::EP2:
// // 					{
// // 						count = RGameInstance->UserInfo->Avatar.ep2;
// // 					}
// // 					break;
// // 					case EREWARD_TYPE::EP3:
// // 					{
// // 						count = RGameInstance->UserInfo->Avatar.ep3;
// // 					}
// // 					break;
// 
// 					case EREWARD_TYPE::RUNE:
// 					{
// 						FString strID = id.ToString();
// 						FString strFID;
// 						for (int32 i = 0; i < strID.Len(); ++i)
// 						{
// 							if (strID[i] >= TEXT('0') && strID[i] <= TEXT('9'))
// 							{
// 								strFID += strID[i];
// 							}
// 						}
// 
// 						int32 iId = FCString::Atoi(*strFID);
// 
// 						info.heroId = FName(*FString::FromInt(iId));
// 						count = UUtilFunctionLibrary::GetCurrentHaveRuneCount(GetWorld(), info.heroId);
// 					}
// 					break;
// 					case EREWARD_TYPE::STAMINA_PVP:
// 					{
// 						count = RGameInstance->RInventory->GetStaminaPVP();
// 					}
// 					break;
// // 					case EREWARD_TYPE::BGT1:
// // 					{
// // 						count = RGameInstance->UserInfo->Avatar.bgt1;
// // 					}
// // 					break;
// // 					case EREWARD_TYPE::BGT2:
// // 					{
// // 						count = RGameInstance->UserInfo->Avatar.bgt2;
// // 					}
// // 					break;
// // 					case EREWARD_TYPE::BGT3:
// // 					{
// // 						count = RGameInstance->UserInfo->Avatar.bgt3;
// // 					}
// // 					break;
// 					}
// 				}
// 			}
// 
// 			if (IsValid(NumText))
// 			{
// 				NumText->SetText(FText::AsNumber(count));
// 			}
// 
// 			if (IsValid(ItemIcon))
// 			{
// 				ItemIcon->SetItemIconStructure(info);
// 			}
// 		}
// 		break;
// 		case EToolTipEventTypeEnum::VE_Npc:
// 		{
// 			if (IsValid(IconTypeSwitcher))
// 			{
// 				IconTypeSwitcher->SetActiveWidgetIndex(2);
// 			}
// 
// 			if (IsValid(InfoWidgetSwitcher))
// 			{
// 				InfoWidgetSwitcher->SetActiveWidgetIndex(0);
// 			}
// 			FCharacterTableInfo* TableInfo = UTableManager::GetInstancePtr()->GetCharacterNpcRow(id);
// 
// 			if(TableInfo != nullptr)
// 			{
// 				int nIndex = 0;
// 				for (URBuffIcon* icon : WeakIconList)
// 				{
// 					if (TableInfo->Tooltip_Weak.IsValidIndex(nIndex))
// 					{
// 						icon->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
// 						icon->SetIconImage(TableInfo->Tooltip_Weak[nIndex++]);
// 					}
// 					else
// 					{
// 						icon->SetVisibility(ESlateVisibility::Collapsed);
// 					}
// 				}
// 				nIndex = 0;
// 				for (URBuffIcon* icon : ImmuneIconList)
// 				{
// 					if(TableInfo->Tooltip_Immune.IsValidIndex(nIndex))
// 					{
// 						icon->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
// 						icon->SetIconImage(TableInfo->Tooltip_Immune[nIndex++]);
// 					}
// 					else
// 					{
// 						icon->SetVisibility(ESlateVisibility::Collapsed);
// 					}
// 				}
// 			}
// 			if (IsValid(NumPanelWidget))
// 			{
// 				NumPanelWidget->SetVisibility(ESlateVisibility::Collapsed);
// 			}
// 
// 			if (IsValid(MonsterIcon))
// 			{
// 				FHERO_ICON_INFO NPCIconInfo;
// 				UUIFunctionLibrary::GetHeroIconInfoForNpc(GetWorld(), NPCIconInfo, id);
// 				NPCIconInfo.level = 0;
// 
// 				MonsterIcon->SetMonsterIconByIconInfo(NPCIconInfo, false);
// 			}
// 		}
// 		break;
// 		case EToolTipEventTypeEnum::VE_Hero:
// 		{
// 			if (IsValid(IconTypeSwitcher))
// 			{
// 				IconTypeSwitcher->SetActiveWidgetIndex(1);
// 			}
// 
// 			if (IsValid(NumPanelWidget))
// 			{
// 				NumPanelWidget->SetVisibility(ESlateVisibility::Collapsed);
// 			}
// 
// 			if (IsValid(HeroIcon))
// 			{
// 				FHERO_ICON_INFO HeroIconInfo;
// 				UUIFunctionLibrary::GetHeroIconInfoWithID(GetWorld(), HeroIconInfo, id, EHeroOwnershipType::VE_Me);
// 				HeroIconInfo.level = 0;
// 
// 				HeroIcon->SetHeroIconInfo(HeroIconInfo);
// 			}
// 		}
// 		break;
// 		}
// 	}
// 
// 	bShow = true;
// 
// 	PrevPos = FVector2D::ZeroVector;
// 
// 	SetPosition(FGeometry());
// 
// 	//this->SetVisibility(ESlateVisibility::Hidden);
// 	//UKismetSystemLibrary::Delay(tooltip->GetWorld(), 5.0f, FLatentActionInfo());
// // 	this->GetWorld()->GetTimerManager().ClearTimer(TooltipHandler);
// // 	this->GetWorld()->GetTimerManager().SetTimer(TooltipHandler, this, &UUC_Tooltip::Show, 1.0f, false);
//
}

void UUC_Tooltip::Show()
{
	this->SetVisibility(ESlateVisibility::Visible);
}

void UUC_Tooltip::SetPosition(const FGeometry& MyGeometry)
{
	UpdateWidgetSize(MyGeometry, true);

	FVector2D size = UWidgetLayoutLibrary::GetViewportSize(this);
	float scale = UWidgetLayoutLibrary::GetViewportScale(this);

	float width = (WidgetSize.Right - WidgetSize.Left) * scale;
	float height = (WidgetSize.Bottom - WidgetSize.Top) * scale;

	if (LastPos.Y + OffsetPos.Y < 0.0f)
	{
		OffsetPos.Y = 65.0f;
	}

	if ((LastPos.X + width) > size.X)
	{
		SetPositionInViewport(FVector2D(LastPos.X - width, LastPos.Y) + OffsetPos);

		if (IsValid(BG_Image))
		{
			BG_Image->SetRenderScale(FVector2D(-1.0f, 1.0f));
		}
	}
	else
	{
		SetPositionInViewport(LastPos + OffsetPos);

		if (IsValid(BG_Image))
		{
			BG_Image->SetRenderScale(FVector2D(1.0f, 1.0f));
		}
	}
}
void UUC_Tooltip::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);

	if (bShow)
	{
		SetPosition(MyGeometry);
		//APlayerController* PlayerController = UUtilFunctionLibrary::GetPlayerController(GetWorld());
		//if (IsValid(PlayerController))
		//{
		//	//float X;
		//	//float Y;
		//	//if (PlayerController->GetMousePosition(X, Y))
		//	//{
		//	//	FVector2D Pos(X, Y);
		//	//	if (!Pos.Equals(PrevPos, 1.0f))
		//	//	{
		//	//		float width = (WidgetSize.Right - WidgetSize.Left) * scale;
		//	//		if ((X + width) > size.X)
		//	//		{
		//	//			SetPositionInViewport(FVector2D(X - width, Y) + OffsetPos);

		//	//			if (IsValid(BGImage))
		//	//			{
		//	//				BGImage->SetRenderScale(FVector2D(-1.0f, 1.0f));
		//	//			}
		//	//		}
		//	//		else
		//	//		{
		//	//			SetPositionInViewport(FVector2D(X, Y) + OffsetPos);

		//	//			if (IsValid(BGImage))
		//	//			{
		//	//				BGImage->SetRenderScale(FVector2D(1.0f, 1.0f));
		//	//			}
		//	//		}

		//	//		//SetPositionInViewport(FVector2D(X, Y) + OffsetPos);

		//	//		PrevPos = Pos;
		//	//	}
		//	//}

		//}
	}
}

void UUC_Tooltip::HideTooltip()
{
	bShow = false;
}


bool UUC_Tooltip::GetShow()
{
	return bShow;
}