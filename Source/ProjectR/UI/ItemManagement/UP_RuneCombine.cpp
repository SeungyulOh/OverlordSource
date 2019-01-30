// Fill out your copyright notice in the Description page of Project Settings.

#include "ProjectR.h"
#include "UP_RuneCombine.h"

#include "Table/ItemTableInfo.h"

#include "UtilFunctionIntegrated.h"
#include "Network/HttpRequestClient.h"
#include "Network/PacketDataStructures.h"
#include "Network/PacketFunctionLibrary.h"

#include "GlobalIntegrated.h"
#include "UI/RWidgetManager.h"


#include "GameInfo/RInventory.h"

#include "UC_GenericPopup.h"
#include "UC_HeroItemInfo.h"
#include "UI/Common/UC_ItemIcon.h"
#include "UC_StatChange.h"
#include "UC_EnchantMaterialButton.h"
#include "UP_ItemEnchantResult_NewDesign.h"
#include "UP_GenericSplash.h"
#include "UC_ItemLevelUpResult.h"
#include "UC_ExpBar.h"
#include "UC_ItemEnchant.h"
#include "SharedConstants/WalletConstants.h"
#include "UC_WalletItemInfo.h"
#include "UC_RewardsSplash.h"
#include "UC_AmountSelectSlider.h"


void UUP_RuneCombine::NativeConstruct()
{
	Super::NativeConstruct();

	AmountSelectSlider->OnCountChangedDelegate.BindUObject(this, &UUP_RuneCombine::OnSliderCountChanged);

	Button_Combine->OnClicked.AddDynamic(this, &UUP_RuneCombine::OnButtonCombineClicked);
}

void UUP_RuneCombine::NativeDestruct()
{
	if (IsValid(Button_Combine))
		Button_Combine->OnClicked.RemoveAll(this);
	if (IsValid(AmountSelectSlider))
		AmountSelectSlider->OnCountChangedDelegate.Unbind();

	Super::NativeDestruct();
}

void UUP_RuneCombine::InvalidateData()
{
	Super::InvalidateData();

	if (WalletKey.IsNone())
		return;

	FWalletTableInfo* InputWalletInfo = UTableManager::GetInstancePtr()->GetWalletRow(WalletKey);
	check(InputWalletInfo != nullptr)
	int32 InputRuneGrade = FRuneTable::WalletKeyToRuneGrade(WalletKey);
	check(InputRuneGrade != INDEX_NONE);
	check(InputRuneGrade != UUC_WalletItemInfo::LastRuneGrade);
	FRuneTable* InputRuneInfo = FRuneTable::LookUp(InputRuneGrade);
	check(InputRuneInfo != nullptr);
	int32 InputRuneOwnedAmount = RGAMEINSTANCE(this)->RInventory->GetRuneWithGrade(InputRuneGrade);

	int32 OutputRuneGrade = InputRuneGrade + 1;
	FName OutputWalletKey = RGAMEINSTANCE(this)->RInventory->RuneGradeToWalletKey(OutputRuneGrade);
	check(OutputWalletKey != NAME_None);
	FWalletTableInfo* OutputWalletInfo = UTableManager::GetInstancePtr()->GetWalletRow(OutputWalletKey);
	check(OutputWalletInfo != nullptr);
	FRuneTable* OutputRuneInfo = FRuneTable::LookUp(OutputRuneGrade);
	check(OutputRuneInfo != nullptr);
	int32 OutputRuneOwnedAmount = RGAMEINSTANCE(this)->RInventory->GetRuneWithGrade(OutputRuneGrade);

	GoldCostRate = OutputRuneInfo->RuneMakeGoldCost;

	// (design) OwnedAmount 3 -> 1 to indicate that you get 1 output rune for 3 input runes
	InputRuneIcon->Refresh(InputRuneInfo->RuneInventoryImage, 3, false, 0);
	Text_InputRuneName->SetText(UUtilFunctionLibrary::GetLocalizedString(EStringTableType::VE_Common, InputWalletInfo->CommonStringKey));

	OutputRuneIcon->Refresh(OutputRuneInfo->RuneInventoryImage, 1, false, 0);
	Text_OutputRuneName->SetText(UUtilFunctionLibrary::GetLocalizedString(EStringTableType::VE_Common, OutputWalletInfo->CommonStringKey));

	int32 GoldOwnedAmount = RGAMEINSTANCE(this)->RInventory->GetCurrencyWithKey(WALLET_GOLD);

	// Determine MaxCount
	int32 MaxCount = InputRuneOwnedAmount / OneCombineAmount;
	MaxCount = GoldOwnedAmount / GoldCostRate < MaxCount ? GoldOwnedAmount / GoldCostRate : MaxCount;

	Text_MaxCountCombine->SetText(FText::AsNumber(MaxCount));

	CurrentCount = 0;

	AmountSelectSlider->CurrentCount = CurrentCount;
	AmountSelectSlider->MaxCount = MaxCount;
	AmountSelectSlider->InvalidateData();

	InvalidateOnCurrentCountChanged();
}

void UUP_RuneCombine::OnInventoryWalletItemSelected(FName SelectedWalletKey)
{
	int32 SelectedRuneGrade = FRuneTable::WalletKeyToRuneGrade(SelectedWalletKey);

	// Not letting select the last rune for Rune Combine
	if (SelectedRuneGrade == UUC_WalletItemInfo::LastRuneGrade)
		return;

	WalletKey = SelectedWalletKey;

	CurrentCount = 0;
	InvalidateData();

	check(IsValid(ItemInventoryPage));

	ItemInventoryPage->InvalidateData();
}

void UUP_RuneCombine::UpdateScrollItemData_Material(UInventoryScrollDataBase* ScrollItemData)
{
	ScrollItemData->IsSelected = ScrollItemData->WalletKey == WalletKey;
}

void UUP_RuneCombine::OnButtonCombineClicked()
{
	check(CurrentCount != 0);

	// These are passed for sanity check in OnInventoryRuneMakeRp
	int32 OutputRuneGrade = FRuneTable::WalletKeyToRuneGrade(WalletKey) + 1;
	FName OutputWalletKey = RGAMEINSTANCE(this)->RInventory->RuneGradeToWalletKey(OutputRuneGrade);
	int32 PrevOutputRuneCount = RGAMEINSTANCE(this)->RInventory->GetRuneWithGrade(OutputRuneGrade);

	RGAMEINSTANCE(this)->HttpClient->OnInventoryRuneMake.AddUObject(this, &UUP_RuneCombine::OnInventoryRuneMakeRp, CurrentCount, OutputRuneGrade, PrevOutputRuneCount);

	UPacketFunctionLibrary::INVENTORY_RUNE_MAKE_RQ(OutputWalletKey.ToString().ToUpper(), CurrentCount);
}

void UUP_RuneCombine::OnInventoryRuneMakeRp(bool bSuccess, int32 Count, int32 OutputRuneGrade, int32 PrevOutputRuneCount)
{
	RGAMEINSTANCE(this)->HttpClient->OnInventoryRuneMake.RemoveAll(this);

	UUP_GenericSplash* Splash = UUIFunctionLibrary::ShowGenericSplashContent(EWidgetBluePrintEnum::WBP_Splash_RewardsSplash);
	check(Splash);
	UUC_RewardsSplash* RewardsSplash = Cast<UUC_RewardsSplash>(Splash->GetContentWidget());
	check(RewardsSplash);

	int32 NewOutputRuneAmount = RGAMEINSTANCE(this)->RInventory->GetRuneWithGrade(OutputRuneGrade);

	ensure(NewOutputRuneAmount - PrevOutputRuneCount == Count);

	FREWARD RuneMade;
	RuneMade.amt = Count;
	RuneMade.prob = 0; // ?
	RuneMade.rewardId = RGAMEINSTANCE(this)->RInventory->RuneGradeToWalletKey(OutputRuneGrade);
	RuneMade.type = EREWARD_TYPE::WALLET;

	RewardsSplash->RefreshRewards({ RuneMade }, EItemInventoryRewardOrigin::RuneCombine);

	InvalidateData();

	check(IsValid(ItemInventoryPage));
	ItemInventoryPage->bInventoryUpdated = true;
	ItemInventoryPage->InvalidateData();
}

void UUP_RuneCombine::OnSliderCountChanged(int32 NewCount)
{
	CurrentCount = NewCount;

	InvalidateOnCurrentCountChanged();
}

void UUP_RuneCombine::InvalidateOnCurrentCountChanged()
{
	Text_CurrentCountCombine->SetText(FText::AsNumber(CurrentCount));

	Button_Combine->SetGoldAmount(GoldCostRate * CurrentCount);
	Button_Combine->SetDisabled(CurrentCount == 0);
}

void UUP_RuneCombine::SetWalletKey(FName InWalletKey)
{
	WalletKey = InWalletKey;
	InvalidateData();
}
