#include "ProjectR.h"
#include "UC_LobbyContentsButton.h"



#include "Global/TableManager.h"
#include "Global/RGameInstance.h"
#include "Global/AttendanceManager.h"
#include "Table/ContentsUITableInfo.h"
#include "UtilFunctionIntegrated.h"
#include "Network/PacketFunctionLibrary.h"

#include "Global/ContentsUnLockManager.h"

#include "Lobby/OutdoorLobbyStateActor.h"
#include "ContentGameMode/Modes/TowerLobbyGameMode.h"

void UUC_LobbyContentsButton::NativeConstruct()
{
	Super::NativeConstruct();

	FContentsUITableInfo* UIInfo = UTableManager::GetInstancePtr()->GetContentsUITableRow(UUIFunctionLibrary::GetContentsName(UIMenuID));

	if (UIInfo == nullptr)
		return;

	// image
	Image_Icon->SetBrushFromTexture(UUtilFunctionLibrary::GetTexture2DFromTable(UIInfo->ContentsIconKey));

	// text
	FText Str = UUtilFunctionLibrary::GetLocalizedString(EStringTableType::VE_UI, UIInfo->UIStringKey);
	Text_Name->SetText(Str);

	// button
	Button_Contents->OnClicked.RemoveDynamic(this, &UUC_LobbyContentsButton::OnButtonContentsClicked);
	Button_Contents->OnClicked.AddDynamic(this, &UUC_LobbyContentsButton::OnButtonContentsClicked);
}

void UUC_LobbyContentsButton::NativeDestruct()
{
	Super::NativeDestruct();
}

void UUC_LobbyContentsButton::OnButtonContentsClicked()
{
	if (UIMenuID == EUIMenuIDEnum::VE_ShopHero)
	{
		ATowerLobbyGameMode* LobbyGameMode = Cast<ATowerLobbyGameMode>(UGameplayStatics::GetGameMode(RGAMEINSTANCE(GEngine)->GetWorld()));
		if (LobbyGameMode)
		{
			AOutdoorLobbyStateActor* OutDoorActor = Cast<AOutdoorLobbyStateActor>(LobbyGameMode->GetCurrentLobbyStateActor());
			if (OutDoorActor)
			{
				UUIFunctionLibrary::GoUIPage(EUIPageEnum::UIPage_ShopHero);
				return;
			}
		}
	}
	else if (UIMenuID == EUIMenuIDEnum::VE_Attendance)
	{
		URGameInstance* RGameInstance = RGAMEINSTANCE(this);
		RGameInstance->AttendanceManager->ShowAttendanceWidget();
		return;
	}
// 	else if (UIMenuID == EUIMenuIDEnum::VE_TimeTreasure)
// 	{
// 		URGameInstance* GameInstance = RGAMEINSTANCE(this);
// 		if (!GameInstance->ContentsUnLockManager->IsContentsUnlock(EGamePlayModeTypeEnum::VE_TimeTreasure))
// 		{
// 			FText TitleText;
// 			FText DescText;
// 
// 			GameInstance->ContentsUnLockManager->GetUnlockReasonText(EGamePlayModeTypeEnum::VE_TimeTreasure, TitleText, DescText);
// 			UUIFunctionLibrary::ShowCommonPopupSplash(TitleText, DescText);
// 			return;
// 		}
// 	}
// 	else if (UIMenuID == EUIMenuIDEnum::VE_Guild)
// 	{
// 		URGameInstance* GameInstance = RGAMEINSTANCE(this);
// 		if (!GameInstance->ContentsUnLockManager->IsContentsUnlock(EGamePlayModeTypeEnum::VE_Guild))
// 		{
// 			FText TitleText;
// 			FText DescText;
// 
// 			GameInstance->ContentsUnLockManager->GetUnlockReasonText(EGamePlayModeTypeEnum::VE_Guild, TitleText, DescText);
// 			UUIFunctionLibrary::ShowCommonPopupSplash(TitleText, DescText);
// 			return;
// 		}
// 
// 	}
	
	
	UUIFunctionLibrary::GoContents(UIMenuID);
}
