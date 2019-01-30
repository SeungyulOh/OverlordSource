// Fill out your copyright notice in the Description page of Project Settings.

#include "ProjectR.h"
#include "UP_HeroShop.h"

#include "CustomActors/BaseSequenceActor.h"
#include "CustomActors/RHeroGacha.h"


#include "GlobalIntegrated.h"
#include "GameInfo/RInventory.h"


#include "UI/RWidgetManager.h"

#include "CustomStructures/GachaInfos.h"

#include "Network/PacketFunctionLibrary.h"

#include "UtilFunctionIntegrated.h"

#include "Lobby/IndoorLobbyStateActor.h"
#include "Lobby/OutdoorLobbyStateActor.h"
#include "Lobby/TowerLobbyLevelScriptActor.h"

#include "Character/AnimInstance/LobbyAnimInstance.h"
#include "Character/AnimInstance/AnimInstanceDefs.h"

#include "ContentGameMode/Controllers/LobbyPlayerController.h"
#include "ContentGameMode/Modes/TowerLobbyGameMode.h"
#include "UI/UI_Lobby/UP_Lobby_Outdoor.h"
#include "UI/Common/UP_BottomTip.h"
#include "UI/UI_Common/TopBar.h"




const int32 Normal_Gacha_1_Gold					 = 1;
const int32 Normal_Gacha_1_SummonTicket			 = 2;
const int32 Normal_Gacha_10_Gold				 = 3;
const int32 Normal_Gacha_10_SummonTicket		 = 4;

const int32 Rare_Gacha_1_Gem					 = 5;
const int32 Rare_Gacha_1_SummonTicket			 = 6;
const int32 Rare_Gacha_10_Gem					 = 7;
const int32 Rare_Gacha_10_SummonTicket			 = 8;

const int32 Special_Gacha_1_Gem					 = 9;
const int32 Special_Gacha_5_Gem					 = 10;

const int32 PointGacha_Tanker					 = 11;
const int32 PointGacha_MeleeDealer				 = 12;
const int32 PointGacha_RangeDealer				 = 13;
const int32 PointGacha_MaginDealer				 = 14;
const int32 PointGacha_Supporter				 = 15;

const FName EmiliaKey = FName(TEXT("13"));


void UUP_HeroShop::NativeConstruct()
{
	Super::NativeConstruct();

	URGameInstance* RGameInstance = RGAMEINSTANCE(this);
	RGameInstance->SequenceManager->ClearSequence();
	PrevUIPage = RGameInstance->RWidgetManager->GetPrevUIPage();
	
		//AIndoorLobbyStateActor* AIndoorActor = RGameInstance->GetIndoorStateActor();
	ATowerLobbyGameMode* TowerLobbyGameMode = RGameInstance->GetTowerLobbyGameMode();
	AIndoorLobbyStateActor* AIndoorActor = nullptr;
	if (TowerLobbyGameMode)
		AIndoorActor = Cast<AIndoorLobbyStateActor>(TowerLobbyGameMode->GetLobbyStateActor(ETowerLobbyStateEnum::Indoor));
	if (IsValid(AIndoorActor))
		AIndoorActor->SetVisibilityHero(false);

	UWorld* world = RGameInstance->GetWorld();
	TArray<AActor*> OutArray;
	UGameplayStatics::GetAllActorsOfClass(world, ACameraActor::StaticClass(), OutArray);
	for (size_t i = 0; i < OutArray.Num(); ++i)
	{
		ACameraActor* CamActor = Cast<ACameraActor>(OutArray[i]);
		if (CamActor->GetName() == TEXT("HeroShop_Summon_Camera"))
		{
			GachaHeroShopCamActor = CamActor;
			break;
		}
	}

	/*if (!GachaShopDealer)
	{
		FLobbyCharacterTableInfo* EmiliaTableInfo = UTableManager::GetInstancePtr()->GetLobbyCharacterTableRow(EmiliaKey);
		if (EmiliaTableInfo)
		{
			GachaShopDealer = world->SpawnActor<ACharacter>(EmiliaTableInfo->LobbyHeroClass.LoadSynchronous(), FVector(70.2f, -469.31f, 188.f), FRotator(0.f, 100.f, 0.f));
			GachaShopDealer->GetCharacterMovement()->GravityScale = 0.f;
			UCapsuleComponent* SphereComp = GachaShopDealer->FindComponentByClass<UCapsuleComponent>();
			if (SphereComp)
				SphereComp->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Block);
		}
	}*/

	Binding_Delegates();

	PS = Cast<UParticleSystem>(StaticLoadObject(UParticleSystem::StaticClass(), nullptr, TEXT("ParticleSystem'/Game/FX/Gacha/FX_4_Gacha_BG.FX_4_Gacha_BG'")));

	Invalidate();
	UUtilFunctionLibrary::SetGachaSHopViewTarget_PCM();

	if (Line)
		Line->SetVisibility(ESlateVisibility::Visible);
	if (Image_BG_2)
		Image_BG_2->SetVisibility(ESlateVisibility::Visible);
}


void UUP_HeroShop::NativeDestruct()
{
	UUtilFunctionLibrary::SetLobbyViewTarget_PCM();
	Pause_WatchLevelSequence(false);

	URGameInstance* RGameInstance = RGAMEINSTANCE(this);

	if (PrevUIPage == EUIPageEnum::UIPage_LobbyIndoor || PrevUIPage == EUIPageEnum::UIPage_None)
	{
		ATowerLobbyGameMode* TowerLobbyGameMode = RGameInstance->GetTowerLobbyGameMode();
		AIndoorLobbyStateActor* AIndoorActor = nullptr;
		if (TowerLobbyGameMode)
			AIndoorActor = Cast<AIndoorLobbyStateActor>(TowerLobbyGameMode->GetLobbyStateActor(ETowerLobbyStateEnum::Indoor));
		if (IsValid(AIndoorActor))
			AIndoorActor->SetVisibilityHero(true);
	}
	if (PS_Component)
	{
		PS_Component->DestroyComponent();
		PS_Component = nullptr;
	}

	if (GachaShopDealer)
	{
		GachaShopDealer->Destroy();
		GachaShopDealer = nullptr;
	}

	UTopBar::OnClickPrevButton.Unbind();
	
	Super::NativeDestruct();
}


void UUP_HeroShop::Invalidate()
{
	/*
	URInventory* Inventory = RGAMEINSTANCE(this)->RInventory;
	if (!Inventory)
		return;

	FGachaInfo* GachaTable = UTableManager::GetInstancePtr()->GetShopCharacterSummonTableRow(FName(*FString::FromInt(PointGacha_Tanker)));
	if (!GachaTable)
		return;

	int32 ReqAmount = GachaTable->Cost_Amount;
	int32 CurrentMileage = Inventory->GetMileage();
	float ProgressRatio = FMath::Clamp<float>((float)CurrentMileage / (float)ReqAmount, 0.f, 1.f);
	if (Text_CurrentMileage)
		Text_CurrentMileage->SetText(FText::AsNumber(CurrentMileage));
	if (ProgressBar_Mileage)
		ProgressBar_Mileage->SetPercent(ProgressRatio);

	if (CurrentMileage < ReqAmount)
		Button_Point->SetIsEnabled(false);
	else
		Button_Point->SetIsEnabled(true);

	if (eShopType == Premium)
	{

	}
	else if (eShopType == Rare)
	{

	}
	else if (eShopType == Normal)
	{

	}
	//*/

	if (IsValid(variables.PremiumSummonOnce_Price_Text))
	{
		FGachaInfo* GachaInfo = UTableManager::GetInstancePtr()->GetShopCharacterSummonTableRow(FName(*FString::FromInt(Special_Gacha_1_Gem)));
		if (GachaInfo)
			variables.PremiumSummonOnce_Price_Text->SetText(FText::AsNumber(GachaInfo->Cost_Amount));
	}
	if (IsValid(variables.PremiumSummonPack_Price_Text))
	{
		FGachaInfo* GachaInfo = UTableManager::GetInstancePtr()->GetShopCharacterSummonTableRow(FName(*FString::FromInt(Special_Gacha_5_Gem)));
		if (GachaInfo)
			variables.PremiumSummonPack_Price_Text->SetText(FText::AsNumber(GachaInfo->Cost_Amount));
	}

	if (IsValid(variables.RareSummonOnce_Price_Text))
	{
		FGachaInfo* GachaInfo = UTableManager::GetInstancePtr()->GetShopCharacterSummonTableRow(FName(*FString::FromInt(Rare_Gacha_1_Gem)));
		if (GachaInfo)
			variables.RareSummonOnce_Price_Text->SetText(FText::AsNumber(GachaInfo->Cost_Amount));
	}
	if (IsValid(variables.RareSummonPack_Price_Text))
	{
		FGachaInfo* GachaInfo = UTableManager::GetInstancePtr()->GetShopCharacterSummonTableRow(FName(*FString::FromInt(Rare_Gacha_10_Gem)));
		if (GachaInfo)
			variables.RareSummonPack_Price_Text->SetText(FText::AsNumber(GachaInfo->Cost_Amount));
	}

	if (IsValid(variables.TicketSummonOnce_Price_Text))
	{
		FGachaInfo* GachaInfo = UTableManager::GetInstancePtr()->GetShopCharacterSummonTableRow(FName(*FString::FromInt(Rare_Gacha_1_SummonTicket)));
		if (GachaInfo)
			variables.TicketSummonOnce_Price_Text->SetText(FText::AsNumber(GachaInfo->Cost_Amount));
	}
	if (IsValid(variables.TicketSummonPack_Price_Text))
	{
		FGachaInfo* GachaInfo = UTableManager::GetInstancePtr()->GetShopCharacterSummonTableRow(FName(*FString::FromInt(Rare_Gacha_10_SummonTicket)));
		if (GachaInfo)
			variables.TicketSummonPack_Price_Text->SetText(FText::AsNumber(GachaInfo->Cost_Amount));
	}

	if (IsValid(variables.NormalSummonOnce_Price_Text))
	{
		FGachaInfo* GachaInfo = UTableManager::GetInstancePtr()->GetShopCharacterSummonTableRow(FName(*FString::FromInt(Normal_Gacha_1_Gold)));
		if (GachaInfo)
			variables.NormalSummonOnce_Price_Text->SetText(FText::AsNumber(GachaInfo->Cost_Amount));
	}
	if (IsValid(variables.NormalSummonPack_Price_Text))
	{
		FGachaInfo* GachaInfo = UTableManager::GetInstancePtr()->GetShopCharacterSummonTableRow(FName(*FString::FromInt(Normal_Gacha_10_Gold)));
		if (GachaInfo)
			variables.NormalSummonPack_Price_Text->SetText(FText::AsNumber(GachaInfo->Cost_Amount));
	}

	HeroSummonMenus->WidgetSwitcher_Gacha->SetVisibility(ESlateVisibility::Collapsed);
	HeroSummonMenus->ButtonExit->SetIsEnabled(false);
	PointSummonPopup->SetVisibility(ESlateVisibility::Collapsed);
	Summon_Popup->SetVisibility(ESlateVisibility::Collapsed);
}

void UUP_HeroShop::OnClick_PointGacha()
{
	if (IsValid(PointSummonPopup))
	{
		PointSummonPopup->SetVisibility(ESlateVisibility::Visible);
	}
}

void UUP_HeroShop::Execute_Gacha()
{
	URGameInstance* RGameInstance = RGAMEINSTANCE(this);

	FGachaInfo* GachaTable = UTableManager::GetInstancePtr()->GetShopCharacterSummonTableRow(FName(*FString::FromInt(GachaKey)));
	if (!GachaTable)
		return;

	if (GachaTable->Cost_Currency.ToString().Contains(TEXT("GOLD")))
	{
		URInventory* Inventory = RGameInstance->RInventory;
		int32 CurrentGold = Inventory->GetGold();

		if (CurrentGold < GachaTable->Cost_Amount)
		{
			if (IsValid(RGameInstance->RWidgetManager))
			{
				UUIFunctionLibrary::ShowCommonPopup(
					UUtilFunctionLibrary::GetLocalizedString(EStringTableType::VE_UI, TEXT("UI_Warning")),
					UUtilFunctionLibrary::GetLocalizedString(EStringTableType::VE_UI, TEXT("UI_Slot_Shop")), ECommonPopupType::VE_OK);
			}
			return;
		}
			
	}
	else if (GachaTable->Cost_Currency.ToString().Contains(TEXT("GEM")))
	{
		URInventory* Inventory = RGameInstance->RInventory;
		int32 CurrentGem = Inventory->GetGem();

		if (CurrentGem < GachaTable->Cost_Amount)
		{
			if (IsValid(RGameInstance->RWidgetManager))
			{
				UUIFunctionLibrary::ShowCommonPopup(
					UUtilFunctionLibrary::GetLocalizedString(EStringTableType::VE_UI, TEXT("UI_Warning")),
					UUtilFunctionLibrary::GetLocalizedString(EStringTableType::VE_UI, TEXT("UI_Slot_Shop")), ECommonPopupType::VE_OK);
			}
			return;
		}
			
	}
	else if (GachaTable->Cost_Currency.ToString().Contains(TEXT("TICKET01")))
	{
		URInventory* Inventory = RGameInstance->RInventory;
		int32 CurrentTicket01 = Inventory->GetSummonTicket01();

		if (CurrentTicket01 < GachaTable->Cost_Amount)
		{
			if (IsValid(RGameInstance->RWidgetManager))
			{
				UUIFunctionLibrary::ShowCommonPopup(
					UUtilFunctionLibrary::GetLocalizedString(EStringTableType::VE_UI, TEXT("UI_Warning")),
					UUtilFunctionLibrary::GetLocalizedString(EStringTableType::VE_UI, TEXT("UI_Slot_Shop")), ECommonPopupType::VE_OK);
			}
			return;
		}
	}
	else if (GachaTable->Cost_Currency.ToString().Contains(TEXT("TICKET02")))
	{
		URInventory* Inventory = RGameInstance->RInventory;
		int32 CurrentTicket02 = Inventory->GetSummonTicket02();

		if (CurrentTicket02 < GachaTable->Cost_Amount)
		{
			if (IsValid(RGameInstance->RWidgetManager))
			{
				UUIFunctionLibrary::ShowCommonPopup(
					UUtilFunctionLibrary::GetLocalizedString(EStringTableType::VE_UI, TEXT("UI_Warning")),
					UUtilFunctionLibrary::GetLocalizedString(EStringTableType::VE_UI, TEXT("UI_Slot_Shop")), ECommonPopupType::VE_OK);
			}
			return;
		}
	}
	else if (GachaTable->Cost_Currency.ToString().Contains(TEXT("MILEAGE")))
	{
		URInventory* Inventory = RGameInstance->RInventory;
		int32 CurrentMileage = Inventory->GetMileage();

		if (CurrentMileage < GachaTable->Cost_Amount)
		{
			if (IsValid(RGameInstance->RWidgetManager))
			{
				UUIFunctionLibrary::ShowCommonPopup(
					UUtilFunctionLibrary::GetLocalizedString(EStringTableType::VE_UI, TEXT("UI_Warning")),
					UUtilFunctionLibrary::GetLocalizedString(EStringTableType::VE_UI, TEXT("UI_Slot_Shop")), ECommonPopupType::VE_OK);
			}
			return;
		}
	}

	HeroSummonMenus->SkipButton->SetVisibility(ESlateVisibility::Visible);
	
	if (Line)
		Line->SetVisibility(ESlateVisibility::Collapsed);
	if (Image_BG_2)
		Image_BG_2->SetVisibility(ESlateVisibility::Collapsed);
	
	UPacketFunctionLibrary::SHOP_CHARACTER_SUMMON_RQ(GachaKey);
}

void UUP_HeroShop::OnClick_Premium_1Gacha()
{
	GachaKey = Special_Gacha_1_Gem;
	if (IsValid(Summon_Popup))
	{
		Summon_Popup->SetVisibility(ESlateVisibility::Visible);
		Summon_Popup->Invalidate(GachaKey);
	}
}

void UUP_HeroShop::OnClick_Premium_5Gacha()
{
	GachaKey = Special_Gacha_5_Gem;
	if (IsValid(Summon_Popup))
	{
		Summon_Popup->SetVisibility(ESlateVisibility::Visible);
		Summon_Popup->Invalidate(GachaKey);
	}

	FText txt = UUtilFunctionLibrary::GetLocalizedString(EStringTableType::VE_UI, FName(TEXT("RYORI")));
	txt = FText::Format(txt, FText::AsNumber(2));
}

void UUP_HeroShop::OnClick_Rare_1Gacha_Ticket()
{
	GachaKey = Rare_Gacha_1_SummonTicket;
	if (IsValid(Summon_Popup))
	{
		Summon_Popup->SetVisibility(ESlateVisibility::Visible);
		Summon_Popup->Invalidate(GachaKey);
	}
}

void UUP_HeroShop::OnClick_Rare_1Gacha_Gem()
{
	GachaKey = Rare_Gacha_1_Gem;
	if (IsValid(Summon_Popup))
	{
		Summon_Popup->SetVisibility(ESlateVisibility::Visible);
		Summon_Popup->Invalidate(GachaKey);
	}
}

void UUP_HeroShop::OnClick_Rare_10Gacha_Ticket()
{
	GachaKey = Rare_Gacha_10_SummonTicket;
	if (IsValid(Summon_Popup))
	{
		Summon_Popup->SetVisibility(ESlateVisibility::Visible);
		Summon_Popup->Invalidate(GachaKey);
	}
}

void UUP_HeroShop::OnClick_Rare_10Gacha_Gem()
{
	GachaKey = Rare_Gacha_10_Gem;
	if (IsValid(Summon_Popup))
	{
		Summon_Popup->SetVisibility(ESlateVisibility::Visible);
		Summon_Popup->Invalidate(GachaKey);
	}
}

void UUP_HeroShop::OnClick_Normal_1Gacha_Ticket()
{
	GachaKey = Normal_Gacha_1_SummonTicket;
	if (IsValid(Summon_Popup))
	{
		Summon_Popup->SetVisibility(ESlateVisibility::Visible);
		Summon_Popup->Invalidate(GachaKey);
	}
}

void UUP_HeroShop::OnClick_Normal_1Gacha_Gold()
{
	GachaKey = Normal_Gacha_1_Gold;
	if (IsValid(Summon_Popup))
	{
		Summon_Popup->SetVisibility(ESlateVisibility::Visible);
		Summon_Popup->Invalidate(GachaKey);
	}
}

void UUP_HeroShop::OnClick_Normal_10Gacha_Ticket()
{
	GachaKey = Normal_Gacha_10_SummonTicket;
	if (IsValid(Summon_Popup))
	{
		Summon_Popup->SetVisibility(ESlateVisibility::Visible);
		Summon_Popup->Invalidate(GachaKey);
	}
}

void UUP_HeroShop::OnClick_Normal_10Gacha_Gem()
{
	GachaKey = Normal_Gacha_10_Gold;
	if (IsValid(Summon_Popup))
	{
		Summon_Popup->SetVisibility(ESlateVisibility::Visible);
		Summon_Popup->Invalidate(GachaKey);
	}
}

void UUP_HeroShop::OnSequenceEnd()
{
	if(IsValid(variables.ShopButtonUIBox))
		variables.ShopButtonUIBox->SetVisibility(ESlateVisibility::Visible);
	if (IsValid(variables.BottomTip))
		variables.BottomTip->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
	UUtilFunctionLibrary::SetLobbyViewTarget_PCM();
	Pause_WatchLevelSequence(false);
}

void UUP_HeroShop::CallBack_SuccessSummon()
{
	URGameInstance* RGameInstance = RGAMEINSTANCE(this);

	UTopBar* TopBar = Cast<UTopBar>(RGameInstance->RWidgetManager->GetUserWidget(EWidgetBluePrintEnum::WBP_TopBar));
	if (IsValid(TopBar))
		TopBar->SetVisibility(ESlateVisibility::Collapsed);

	if (IsValid(GachaShopDealer))
		GachaShopDealer->SetActorHiddenInGame(true);

	Pause_WatchLevelSequence(true);
	SetIsDoneSelecting(false);

	RGameInstance->SequenceManager->ClearSequence();
	SequenceActor = RGameInstance->SequenceManager->SpawnSequence(SequenceKey);
	
	if (IsValid(variables.ShopButtonUIBox))
		variables.ShopButtonUIBox->SetVisibility(ESlateVisibility::Collapsed);
	if (IsValid(variables.BottomTip))
		variables.BottomTip->SetVisibility(ESlateVisibility::Collapsed);

	if (HeroSummonMenus)
		HeroSummonMenus->Btn_Skip->SetVisibility(ESlateVisibility::SelfHitTestInvisible);

	if (IsValid(SequenceActor))
	{
		if (false == SequenceActor->SequencePlayer->OnStop.IsAlreadyBound(this, &UUP_HeroShop::OnSequenceEnd))
		{
			SequenceActor->SequencePlayer->OnStop.AddDynamic(this, &UUP_HeroShop::OnSequenceEnd);
		}
	}

	ATowerLobbyLevelScriptActor* LobbyLSA = Cast<ATowerLobbyLevelScriptActor>(RGameInstance->GetWorld()->GetLevelScriptActor());
	if (LobbyLSA)
		LobbyLSA->SetDiableEvent(false);

	Invalidate();
}

void UUP_HeroShop::CallBack_Click(ETouchIndex::Type TouchIndex, FVector Location)
{
	APlayerController* PC = UGameplayStatics::GetPlayerController(GetWorld(), 0);

	FHitResult hitresult;
	PC->GetHitResultUnderCursor(ECollisionChannel::ECC_Visibility, true, hitresult);

	ACharacter* SelectedCharacter = Cast<ACharacter>(hitresult.GetActor());
	if (SelectedCharacter)
	{
		UAnimInstance* CharacterAnimInstance = UCharacterHelpFunctionLibrary::GetCharacterAnimInstance(SelectedCharacter);
		if (IsValid(CharacterAnimInstance))
		{
			ULobbyAnimInstance* LobbyAnimInstance = Cast< ULobbyAnimInstance >(CharacterAnimInstance);
			if (IsValid(LobbyAnimInstance))
			{
				LobbyAnimInstance->PlayAnimMontageHeroShopSection(ANIM_MONTAGE_LOBBY_SOCIAL);
			}
		}
	}
	
}

void UUP_HeroShop::Binding_Delegates()
{
	URGameInstance* RGameInstance = RGAMEINSTANCE(this);

	if (IsValid(variables.PremiumSummonOnce_Button))
	{
		variables.PremiumSummonOnce_Button->OnClicked.Clear();
		variables.PremiumSummonOnce_Button->OnClicked.AddDynamic(this, &UUP_HeroShop::OnClick_Premium_1Gacha);
	}

	if (IsValid(variables.PremiumSummonPack_Button))
	{
		variables.PremiumSummonPack_Button->OnClicked.Clear();
		variables.PremiumSummonPack_Button->OnClicked.AddDynamic(this, &UUP_HeroShop::OnClick_Premium_5Gacha);
	}

	if (IsValid(variables.TicketSummonOnce_Button))
	{
		variables.TicketSummonOnce_Button->OnClicked.Clear();
		variables.TicketSummonOnce_Button->OnClicked.AddDynamic(this, &UUP_HeroShop::OnClick_Rare_1Gacha_Ticket);
	}

	if (IsValid(variables.TicketSummonPack_Button))
	{
		variables.TicketSummonPack_Button->OnClicked.Clear();
		variables.TicketSummonPack_Button->OnClicked.AddDynamic(this, &UUP_HeroShop::OnClick_Rare_10Gacha_Ticket);
	}

	if (IsValid(variables.RareSummonOnce_Button))
	{
		variables.RareSummonOnce_Button->OnClicked.Clear();
		variables.RareSummonOnce_Button->OnClicked.AddDynamic(this, &UUP_HeroShop::OnClick_Rare_1Gacha_Gem);
	}

	if (IsValid(variables.RareSummonPack_Button))
	{
		variables.RareSummonPack_Button->OnClicked.Clear();
		variables.RareSummonPack_Button->OnClicked.AddDynamic(this, &UUP_HeroShop::OnClick_Rare_10Gacha_Gem);
	}

	if (IsValid(variables.NormalSummonOnce_Button))
	{
		variables.NormalSummonOnce_Button->OnClicked.Clear();
		variables.NormalSummonOnce_Button->OnClicked.AddDynamic(this, &UUP_HeroShop::OnClick_Normal_1Gacha_Gold);
	}

	if (IsValid(variables.NormalSummonPack_Button))
	{
		variables.NormalSummonPack_Button->OnClicked.Clear();
		variables.NormalSummonPack_Button->OnClicked.AddDynamic(this, &UUP_HeroShop::OnClick_Normal_10Gacha_Gem);
	}

// 	if (IsValid(Button_Point))
// 	{
// 		Button_Point->OnClicked.Clear();
// 		Button_Point->OnClicked.AddDynamic(this, &UUP_HeroShop::OnClick_PointGacha);
// 	}
	
	if (RGameInstance->EventManager)
	{
		RGameInstance->EventManager->OnSuccess_HeroSummon.Clear();
		RGameInstance->EventManager->OnSuccess_HeroSummon.AddDynamic(this, &UUP_HeroShop::CallBack_SuccessSummon);
	}

	/*UWorld* World = GetWorld();
	if (World)
	{
		APlayerController* PC = UGameplayStatics::GetPlayerController(World, 0);
		UInputComponent* InputComp = PC->InputComponent;

		InputComp->TouchBindings.Empty();
		InputComp->BindTouch(IE_Pressed, this, &UUP_HeroShop::CallBack_Click);
	}*/

	ATowerLobbyLevelScriptActor* LobbyLSA = Cast<ATowerLobbyLevelScriptActor>(RGAMEINSTANCE(this)->GetWorld()->GetLevelScriptActor());
	if (LobbyLSA)
	{
		UTopBar::OnClickPrevButton.BindUObject(LobbyLSA, &ATowerLobbyLevelScriptActor::EndProcess_ShopHero);
	}
	
	
}

void UUP_HeroShop::Pause_WatchLevelSequence(bool isPause)
{
	URGameInstance* RGameInstance = RGAMEINSTANCE(this);

	UWorld* CurrentWorld = RGameInstance->GetWorld();
	TArray<AActor*> LSActors;
	UGameplayStatics::GetAllActorsOfClass(CurrentWorld, ALevelSequenceActor::StaticClass(), LSActors);

	for (size_t i = 0; i < LSActors.Num(); ++i)
	{
		ALevelSequenceActor* LSActor = Cast<ALevelSequenceActor>(LSActors[i]);
		if (LSActor && LSActor->GetName().Contains(TEXT("Watch")))
		{
			if (isPause)
				LSActor->SequencePlayer->Pause();
			else
				LSActor->SequencePlayer->Play();
			
			break;
		}
	}
}

void UUP_HeroShop::SetVisibility_SummonMenus()
{
	if (HeroSummonMenus->Btn_Skip)
		HeroSummonMenus->Btn_Skip->SetVisibility(ESlateVisibility::Collapsed);

	HeroSummonMenus->WidgetSwitcher_Gacha->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
	HeroSummonMenus->WidgetSwitcher_Gacha->SetActiveWidgetIndex(isDoneSelecting ? 1 : 0);

	URGameInstance* RGameInstance = RGAMEINSTANCE(this);
	UTopBar* TopBar = Cast<UTopBar>(RGameInstance->RWidgetManager->GetUserWidget(EWidgetBluePrintEnum::WBP_TopBar));
	if (IsValid(TopBar))
		TopBar->SetVisibility(ESlateVisibility::SelfHitTestInvisible);

	// for tutorial condition check
	HeroSummonMenus->ButtonExit->SetIsEnabled(true);

	if (GachaKey == Normal_Gacha_1_Gold ||
		GachaKey == Normal_Gacha_1_SummonTicket ||
		GachaKey == Rare_Gacha_1_Gem ||
		GachaKey == Rare_Gacha_1_SummonTicket ||
		GachaKey == Special_Gacha_1_Gem ||
		GachaKey == Special_Gacha_5_Gem)
	{
		if (HeroSummonMenus->Button_Retry_Additional)
			HeroSummonMenus->Button_Retry_Additional->SetVisibility(ESlateVisibility::Collapsed);
		if (HeroSummonMenus->Button_OpenAll)
			HeroSummonMenus->Button_OpenAll->SetVisibility(ESlateVisibility::Collapsed);
		if (HeroSummonMenus->Button_Retry)
			HeroSummonMenus->Button_Retry->SetVisibility(ESlateVisibility::Visible);
		if(GachaKey == Special_Gacha_5_Gem)
			HeroSummonMenus->Button_OpenAll->SetVisibility(ESlateVisibility::Visible);

		//  Just One Card
		FGachaInfo* GachaInfoTable = UTableManager::GetInstancePtr()->GetShopCharacterSummonTableRow(FName(*FString::FromInt(GachaKey)));
		if (GachaInfoTable)
		{
			if (HeroSummonMenus->Text_Left)
				HeroSummonMenus->Text_Left->SetText(FText::AsNumber(GachaInfoTable->Cost_Amount));

			FWalletTableInfo* Wallet_table = UTableManager::GetInstancePtr()->GetWalletRow(GachaInfoTable->Cost_Currency);
			if (Wallet_table)
				UUIFunctionLibrary::SetImageBrush(UUIFunctionLibrary::GetIconImageFromTable(Wallet_table->ItemIconKey), HeroSummonMenus->Image_Left);
		}
	}
	else if (GachaKey >= PointGacha_Tanker)
	{
		if (HeroSummonMenus->Button_Retry_Additional)
			HeroSummonMenus->Button_Retry_Additional->SetVisibility(ESlateVisibility::Collapsed);
		if (HeroSummonMenus->Button_OpenAll)
			HeroSummonMenus->Button_OpenAll->SetVisibility(ESlateVisibility::Collapsed);
		if (HeroSummonMenus->Button_Retry)
			HeroSummonMenus->Button_Retry->SetVisibility(ESlateVisibility::Collapsed);
	}
	else
	{
		if (HeroSummonMenus->Button_Retry_Additional)
			HeroSummonMenus->Button_Retry_Additional->SetVisibility(ESlateVisibility::Visible);
		if (HeroSummonMenus->Button_OpenAll)
			HeroSummonMenus->Button_OpenAll->SetVisibility(ESlateVisibility::Visible);
		if (HeroSummonMenus->Button_Retry)
			HeroSummonMenus->Button_Retry->SetVisibility(ESlateVisibility::Visible);

		// Over than one card, five , ten , eleven
		FGachaInfo* GachaInfoTable = UTableManager::GetInstancePtr()->GetShopCharacterSummonTableRow(FName(*FString::FromInt(GachaKey)));
		if (GachaInfoTable)
		{
			if (GachaInfoTable->Cost_Currency.ToString().Contains(TEXT("Ticket")))
			{
				if (HeroSummonMenus->Text_Left)
					HeroSummonMenus->Text_Left->SetText(FText::AsNumber(GachaInfoTable->Cost_Amount));

				FWalletTableInfo* Wallet_table = UTableManager::GetInstancePtr()->GetWalletRow(GachaInfoTable->Cost_Currency);
				if (Wallet_table)
					UUIFunctionLibrary::SetImageBrush(UUIFunctionLibrary::GetIconImageFromTable(Wallet_table->ItemIconKey), HeroSummonMenus->Image_Left);


				GachaInfoTable = UTableManager::GetInstancePtr()->GetShopCharacterSummonTableRow(FName(*FString::FromInt(GachaKey-1)));
				if (HeroSummonMenus->Text_Right)
					HeroSummonMenus->Text_Right->SetText(FText::AsNumber(GachaInfoTable->Cost_Amount));

				Wallet_table = UTableManager::GetInstancePtr()->GetWalletRow(GachaInfoTable->Cost_Currency);
				if (Wallet_table)
					UUIFunctionLibrary::SetImageBrush(UUIFunctionLibrary::GetIconImageFromTable(Wallet_table->ItemIconKey), HeroSummonMenus->Image_Right);
			}
			else
			{
				if (HeroSummonMenus->Text_Right)
					HeroSummonMenus->Text_Right->SetText(FText::AsNumber(GachaInfoTable->Cost_Amount));

				FWalletTableInfo* Wallet_table = UTableManager::GetInstancePtr()->GetWalletRow(GachaInfoTable->Cost_Currency);
				if (Wallet_table)
					UUIFunctionLibrary::SetImageBrush(UUIFunctionLibrary::GetIconImageFromTable(Wallet_table->ItemIconKey), HeroSummonMenus->Image_Right);


				GachaInfoTable = UTableManager::GetInstancePtr()->GetShopCharacterSummonTableRow(FName(*FString::FromInt(GachaKey + 1)));
				if (HeroSummonMenus->Text_Left)
					HeroSummonMenus->Text_Left->SetText(FText::AsNumber(GachaInfoTable->Cost_Amount));

				Wallet_table = UTableManager::GetInstancePtr()->GetWalletRow(GachaInfoTable->Cost_Currency);
				if (Wallet_table)
					UUIFunctionLibrary::SetImageBrush(UUIFunctionLibrary::GetIconImageFromTable(Wallet_table->ItemIconKey), HeroSummonMenus->Image_Left);
			}
		}
	}
	
}

void UUC_GachaConfirmPopup::NativeConstruct()
{
	Super::NativeConstruct();

	if (Button_Cancel)
	{
		Button_Cancel->OnClicked.Clear();
		Button_Cancel->OnClicked.AddDynamic(this, &UUC_GachaConfirmPopup::OnClick_Cancel);
	}
	if (Button_OK)
	{
		Button_OK->OnClicked.Clear();
		Button_OK->OnClicked.AddDynamic(this, &UUC_GachaConfirmPopup::OnClick_OK);
	}
}

void UUC_GachaConfirmPopup::Invalidate(int32 GachaKey)
{
	FGachaInfo* GachaTable = UTableManager::GetInstancePtr()->GetShopCharacterSummonTableRow(FName(*FString::FromInt(GachaKey)));
	if (GachaTable)
	{
		FWalletTableInfo* Wallet_table = UTableManager::GetInstancePtr()->GetWalletRow(GachaTable->Cost_Currency);
			
		if(Wallet_table)
			UUIFunctionLibrary::SetImageBrush(UUIFunctionLibrary::GetIconImageFromTable(Wallet_table->ItemIconKey), CostImage);

		FString CostAmountStr = FString::FromInt(GachaTable->Cost_Amount);
		if (IsValid(CostAmount))
			CostAmount->SetText(FText::FromString(CostAmountStr));

		
		FText TopText = UUtilFunctionLibrary::GetLocalizedString(EStringTableType::VE_SystemMessage, GachaTable->String_Key01);
		if (Text_Top)
			Text_Top->SetText(TopText);

		
		int32 GachaCount = GachaTable->RewardId_BasicNum;
		if (!GachaTable->RewardId_Fix.IsNone())
			GachaCount += 1;

		FText BottomText = FText::Format(UUtilFunctionLibrary::GetLocalizedString(EStringTableType::VE_SystemMessage, GachaTable->String_Key02), GachaCount);
		if (Text_Bottom)
			Text_Bottom->SetText(BottomText);
		
	}
}

void UUC_GachaConfirmPopup::OnClick_Cancel()
{
	SetVisibility(ESlateVisibility::Collapsed);
}

void UUC_GachaConfirmPopup::OnClick_OK()
{
	SetVisibility(ESlateVisibility::Collapsed);

	URBaseWidget* BaseWidget = GetParentWidget();
	UUP_HeroShop* HeroShop = Cast<UUP_HeroShop>(BaseWidget);
	if (HeroShop)
		HeroShop->Execute_Gacha();
}

void UUC_GachaClassSelectPopup::NativeConstruct()
{
	Super::NativeConstruct();

	if (Button_Cancel)
	{
		Button_Cancel->OnClicked.Clear();
		Button_Cancel->OnClicked.AddDynamic(this, &UUC_GachaClassSelectPopup::OnClick_Cancel);
	}
	if (Button_OK)
	{
		Button_OK->OnClicked.Clear();
		Button_OK->OnClicked.AddDynamic(this, &UUC_GachaClassSelectPopup::OnClick_OK);
	}
	if (Button_Tanker)
	{
		Button_Tanker->OnClicked.Clear();
		Button_Tanker->OnClicked.AddDynamic(this, &UUC_GachaClassSelectPopup::OnClick_Tanker);
	}
	if (Button_Melee)
	{
		Button_Melee->OnClicked.Clear();
		Button_Melee->OnClicked.AddDynamic(this, &UUC_GachaClassSelectPopup::OnClick_Melee);
	}
	if (Button_Range)
	{
		Button_Range->OnClicked.Clear();
		Button_Range->OnClicked.AddDynamic(this, &UUC_GachaClassSelectPopup::OnClick_Range);
	}
	if (Button_Magic)
	{
		Button_Magic->OnClicked.Clear();
		Button_Magic->OnClicked.AddDynamic(this, &UUC_GachaClassSelectPopup::OnClick_Magic);
	}
	if (Button_Supporter)
	{
		Button_Supporter->OnClicked.Clear();
		Button_Supporter->OnClicked.AddDynamic(this, &UUC_GachaClassSelectPopup::OnClick_Support);
	}

	FindChildWidgetsWithCertainName(this, WidgetSwitcherArray, TEXT("WidgetSwitcher_Class"));
	for (size_t i = 0; i < WidgetSwitcherArray.Num(); ++i)
		WidgetSwitcherArray[i]->SetActiveWidgetIndex(1);

}

void UUC_GachaClassSelectPopup::OnClick_Cancel()
{
	SetVisibility(ESlateVisibility::Collapsed);
}

void UUC_GachaClassSelectPopup::OnClick_OK()
{
	URBaseWidget* BaseWidget = GetParentWidget();
	UUP_HeroShop* HeroShop = Cast<UUP_HeroShop>(BaseWidget);
	if (HeroShop)
	{
		if(PointGacha_Tanker <= HeroShop->GachaKey && HeroShop->GachaKey <= PointGacha_Supporter)
		{
			SetVisibility(ESlateVisibility::Collapsed);

			HeroShop->Execute_Gacha();
		}
		else
		{
			UUIFunctionLibrary::ShowCommonPopupSplash(UUtilFunctionLibrary::GetLocalizedString(EStringTableType::VE_UI, COMMONSTRING_NOTIFY),
				UUtilFunctionLibrary::GetLocalizedString(EStringTableType::VE_UI, FName(TEXT("UI_Character_PointSummon_SplashPopUp01"))));
		}
	}
}

void UUC_GachaClassSelectPopup::OnClick_Tanker()
{
	for (auto Element : WidgetSwitcherArray)
		Element->SetActiveWidgetIndex(1);

	WidgetSwitcherArray[TANKER]->SetActiveWidget(0);

	URBaseWidget* BaseWidget = GetParentWidget();
	UUP_HeroShop* HeroShop = Cast<UUP_HeroShop>(BaseWidget);
	if (HeroShop)
		HeroShop->GachaKey = PointGacha_Tanker;
}

void UUC_GachaClassSelectPopup::OnClick_Melee()
{
	for (auto Element : WidgetSwitcherArray)
		Element->SetActiveWidgetIndex(1);

	WidgetSwitcherArray[MELEE]->SetActiveWidget(0);

	URBaseWidget* BaseWidget = GetParentWidget();
	UUP_HeroShop* HeroShop = Cast<UUP_HeroShop>(BaseWidget);
	if (HeroShop)
		HeroShop->GachaKey = PointGacha_MeleeDealer;
}

void UUC_GachaClassSelectPopup::OnClick_Range()
{
	for (auto Element : WidgetSwitcherArray)
		Element->SetActiveWidgetIndex(1);

	WidgetSwitcherArray[RANGE]->SetActiveWidget(0);

	URBaseWidget* BaseWidget = GetParentWidget();
	UUP_HeroShop* HeroShop = Cast<UUP_HeroShop>(BaseWidget);
	if (HeroShop)
		HeroShop->GachaKey = PointGacha_RangeDealer;
}

void UUC_GachaClassSelectPopup::OnClick_Magic()
{
	for (auto Element : WidgetSwitcherArray)
		Element->SetActiveWidgetIndex(1);

	WidgetSwitcherArray[MAGIC]->SetActiveWidget(0);

	URBaseWidget* BaseWidget = GetParentWidget();
	UUP_HeroShop* HeroShop = Cast<UUP_HeroShop>(BaseWidget);
	if (HeroShop)
		HeroShop->GachaKey = PointGacha_MaginDealer;
}

void UUC_GachaClassSelectPopup::OnClick_Support()
{
	for (auto Element : WidgetSwitcherArray)
		Element->SetActiveWidgetIndex(1);

	WidgetSwitcherArray[SUPPORT]->SetActiveWidget(0);

	URBaseWidget* BaseWidget = GetParentWidget();
	UUP_HeroShop* HeroShop = Cast<UUP_HeroShop>(BaseWidget);
	if (HeroShop)
		HeroShop->GachaKey = PointGacha_Supporter;
}

void UUC_HeroSummonMenus::NativeConstruct()
{
	Super::NativeConstruct();

	if (Button_Skip)
	{
		Button_Skip->OnClicked.Clear();
		Button_Skip->OnClicked.AddDynamic(this, &UUC_HeroSummonMenus::OnClick_Skip);
	}
	if (SkipButton)
	{
		SkipButton->OnClicked.Clear();
		SkipButton->OnClicked.AddDynamic(this, &UUC_HeroSummonMenus::OnClick_SkipSequence);
	}
	if (Button_OpenAll)
	{
		Button_OpenAll->OnClicked.Clear();
		Button_OpenAll->OnClicked.AddDynamic(this, &UUC_HeroSummonMenus::OnClick_OpenAll_Premium);
	}
	if (Button_Retry)
	{
		Button_Retry->OnClicked.Clear();
		Button_Retry->OnClicked.AddDynamic(this, &UUC_HeroSummonMenus::OnClick_ReGacha);
	}
	if (Button_Retry_Additional)
	{
		Button_Retry_Additional->OnClicked.Clear();
		Button_Retry_Additional->OnClicked.AddDynamic(this, &UUC_HeroSummonMenus::OnClick_ReGacha_Additional);
	}

	if (ButtonExit)
	{
		ButtonExit->OnClicked.Clear();
		ButtonExit->OnClicked.AddDynamic(this, &UUC_HeroSummonMenus::OnClick_Exit);
	}

	if(HeroNameCanvas)
	{
		HeroNameCanvas->SetVisibility(ESlateVisibility::Collapsed);
	}

	FindChildWidgetsWithCertainName(this, HeroStarArray, TEXT("Hero_Star_"));

	NameEffect = Cast<UParticleSystem>(StaticLoadObject(UParticleSystem::StaticClass(), nullptr, TEXT("ParticleSystem'/Game/FX/Gacha/FX_4_Gacha_NameEffects.FX_4_Gacha_NameEffects'")));
	if (!NameEffect)
	{
		UE_LOG(LogClass, Log, TEXT("Particle Load Failed"));
	}
}

void UUC_HeroSummonMenus::OnClick_Skip()
{
	URGameInstance* RGameInstance = RGAMEINSTANCE(this);
	if (!RGameInstance)
		return;

	HeroNameCanvas->SetVisibility(ESlateVisibility::Collapsed);
	WidgetSwitcher_Gacha->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
	ButtonExit->SetIsEnabled(true);
	//AIndoorLobbyStateActor* IndoorActor = RGameInstance->GetIndoorStateActor();
	ATowerLobbyGameMode* TowerLobbyGameMode = RGameInstance->GetTowerLobbyGameMode();
	AIndoorLobbyStateActor* IndoorActor = nullptr;
	if (TowerLobbyGameMode)
		IndoorActor = Cast<AIndoorLobbyStateActor>(TowerLobbyGameMode->GetLobbyStateActor(ETowerLobbyStateEnum::Indoor));
	if (IndoorActor)
		IndoorActor->SetVisibilityHero(false);

	UWorld* world = RGameInstance->GetWorld();
	ATowerLobbyLevelScriptActor* LLSA = Cast<ATowerLobbyLevelScriptActor>(world->GetLevelScriptActor());
	if (LLSA)
		LLSA->BGParticleActivate();

	RGameInstance->SequenceManager->ClearSequence_ForGacha();

	APlayerCameraManager* PCM = UGameplayStatics::GetPlayerCameraManager(world, 0);
	if (PCM && LLSA)
	{
		FViewTargetTransitionParams BlendParams;
		BlendParams.BlendTime = 1.f;

		ARHeroGacha* HeroGachaActor = LLSA->HeroGachaActor;
		if (HeroGachaActor)
		{
			AActor* CamActor = HeroGachaActor->PrevCameraActor.Get();
			if (CamActor)
			{
				PCM->SetViewTarget(CamActor, BlendParams);

				UCameraComponent* CamComp = CamActor->FindComponentByClass<UCameraComponent>();
				if (CamComp)
				{
					CamComp->PostProcessSettings.bOverride_BloomIntensity = true;
					CamComp->PostProcessSettings.bOverride_BloomMethod = true;

					CamComp->PostProcessSettings.BloomIntensity = 0;
					CamComp->PostProcessSettings.BloomMethod = EBloomMethod::BM_SOG;
				}
			}	
		}
	}

	

}

void UUC_HeroSummonMenus::OnClick_SkipSequence()
{
	URGameInstance* RGameInstance = RGAMEINSTANCE(this);
	ABaseSequenceActor* CurSequenceActor = RGameInstance->SequenceManager->GetCurrentSequenceActor();

	if (CurSequenceActor && CurSequenceActor->IsPlaying())
	{
		float NewPosition = 4.5f;
		CurSequenceActor->SequencePlayer->JumpToPosition(NewPosition);
		SkipButton->SetVisibility(ESlateVisibility::Collapsed);
	}
		
}

void UUC_HeroSummonMenus::OnClick_OpenAll_Premium()
{
	URGameInstance* RGameInstance = RGAMEINSTANCE(this);

	ATowerLobbyLevelScriptActor* LobbyLSA = Cast<ATowerLobbyLevelScriptActor>(RGameInstance->GetWorld()->GetLevelScriptActor());
	if (LobbyLSA && LobbyLSA->HeroGachaActor)
		LobbyLSA->HeroGachaActor->OpenAllCard();
}

void UUC_HeroSummonMenus::OnClick_ReGacha()
{
	URGameInstance* RGameInstance = RGAMEINSTANCE(this);
	if (!RGameInstance)
		return;

	UUP_HeroShop* HeroShop = Cast<UUP_HeroShop>(ParentWidget);
	if (!HeroShop)
		return;

	ATowerLobbyLevelScriptActor* LobbyLSA = Cast<ATowerLobbyLevelScriptActor>(RGameInstance->GetWorld()->GetLevelScriptActor());
	if (LobbyLSA)
		LobbyLSA->EndProcess_ShopHero();

	if (HeroShop->GachaKey % 2 == 1 && HeroShop->GachaKey < Special_Gacha_1_Gem)
		HeroShop->GachaKey += 1;

	HeroShop->Execute_Gacha();
	WidgetSwitcher_Gacha->SetVisibility(ESlateVisibility::Collapsed);
	ButtonExit->SetIsEnabled(false);
}

void UUC_HeroSummonMenus::OnClick_ReGacha_Additional()
{
	URGameInstance* RGameInstance = RGAMEINSTANCE(this);
	if (!RGameInstance)
		return;

	UUP_HeroShop* HeroShop = Cast<UUP_HeroShop>(ParentWidget);
	if (!HeroShop)
		return;

	ATowerLobbyLevelScriptActor* LobbyLSA = Cast<ATowerLobbyLevelScriptActor>(RGameInstance->GetWorld()->GetLevelScriptActor());
	if (LobbyLSA)
		LobbyLSA->EndProcess_ShopHero();

	if (HeroShop->GachaKey % 2 == 0)
		HeroShop->GachaKey -= 1;

	HeroShop->Execute_Gacha();
	WidgetSwitcher_Gacha->SetVisibility(ESlateVisibility::Collapsed);
	ButtonExit->SetIsEnabled(false);
}

void UUC_HeroSummonMenus::OnClick_Exit()
{
	URGameInstance* RGameInstance = RGAMEINSTANCE(this);
	if (!RGameInstance)
		return;

	HeroNameCanvas->SetVisibility(ESlateVisibility::Collapsed);
	WidgetSwitcher_Gacha->SetVisibility(ESlateVisibility::Collapsed);
	ButtonExit->SetIsEnabled(false);

	ATowerLobbyLevelScriptActor* LobbyLSA = Cast<ATowerLobbyLevelScriptActor>(RGameInstance->GetWorld()->GetLevelScriptActor());
	if (LobbyLSA)
		LobbyLSA->EndProcess_ShopHero();

	UUP_HeroShop* HeroShop = Cast<UUP_HeroShop>(ParentWidget);
	if (!HeroShop)
		return;

	HeroShop->Binding_Delegates();
// 	HeroShop->SetDealearHidden(false);
	HeroShop->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
	HeroShop->Invalidate();

	if (HeroShop->Line)
		HeroShop->Line->SetVisibility(ESlateVisibility::Visible);
	if (HeroShop->Image_BG_2)
		HeroShop->Image_BG_2->SetVisibility(ESlateVisibility::Visible);

	if (HeroShop->PS_Component)
	{
		HeroShop->PS_Component->DestroyComponent();
		HeroShop->PS_Component = nullptr;
	}
	

	UUtilFunctionLibrary::SetGachaSHopViewTarget_PCM();

	
}

void UUC_HeroSummonMenus::Invalidate(FHERO& InHero)
{
	FCharacterTableInfo* CharacterTableInfo = UTableManager::GetInstancePtr()->GetCharacterRow(InHero.heroId);
	checkf(CharacterTableInfo, TEXT("No valid HeroID In CharacterTable"));

	int32 grade = CharacterTableInfo->Grade;
	for (UImage* ImgStar : HeroStarArray)
		ImgStar->SetVisibility(ESlateVisibility::Collapsed);


	for (int32 i = 0; i < grade; i++)
		if (HeroStarArray.IsValidIndex(i))
			HeroStarArray[i]->SetVisibility(ESlateVisibility::SelfHitTestInvisible);

	if (grade <= 5)
	{
		WidgetSwitcherStar->SetActiveWidgetIndex(0);
	}
	else if (grade < 10)
	{
		WidgetSwitcherStar->SetActiveWidgetIndex(1);
	}


	if (IsValid(Text_HeroName))
	{
		Text_HeroName->SetText(UUtilFunctionLibrary::GetLocalizedString(EStringTableType::VE_Hero, CharacterTableInfo->DisplayName));
	}

	if (IsValid(Text_HeroNick))
	{
		Text_HeroNick->SetText(UUtilFunctionLibrary::GetLocalizedString(EStringTableType::VE_Hero, CharacterTableInfo->Nick));
	}

	URGameInstance* RGameInst = RGAMEINSTANCE(this);
	if (RGameInst)
	{
		FTransform SpawnTranform;
		SpawnTranform.SetLocation(FVector(0.f, 50.f, 20.f));
		SpawnTranform.SetScale3D(FVector(0.2f, 0.2f, 0.2f));

		if(NameEffect)
			UGameplayStatics::SpawnEmitterAtLocation(RGameInst->GetWorld(), NameEffect, SpawnTranform);
		
	}
	
}
