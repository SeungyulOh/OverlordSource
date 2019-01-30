
#include "ProjectR.h"
#include "UC_Guild_LevelUp.h"
#include "Global/TableManager.h"
#include "UI/Common/UC_CurrencyDisplay.h"
#include "UtilFunctionIntegrated.h"
#include "UI/ItemManagement/UP_ItemShortagePopup.h"
#include "Network/PacketFunctionLibrary.h"
#include "GameInfo/GuildInfo.h"
#include "UI/RWidgetManager.h"

void UUC_Guild_LevelUp::NativeConstruct()
{
	Super::NativeConstruct();
}

void UUC_Guild_LevelUp::NativeDestruct()
{
	Super::NativeDestruct();
}

void UUC_Guild_LevelUp::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);
}

void UUC_Guild_LevelUp::OnClickLevelUp()
{
	URGameInstance* RGameInstance = RGAMEINSTANCE(this);
// 	if (IsValid(RGameInstance) == false)
// 		return;
	int32 NextLevel = RGameInstance->GuildInfo->MyGuildInfo.level + 1;
	UPacketFunctionLibrary::GUILD_LEVEL_UP_RQ(NextLevel);
}

void UUC_Guild_LevelUp::OnClickClose()
{
	URGameInstance* RGameInstance = RGAMEINSTANCE(this);
// 	if (IsValid(RGameInstance) == false)
// 		return;
}

void UUC_Guild_LevelUp::SetGuildLevelUpInfo(ULocalizingTextBlock* CurrentLevel, ULocalizingTextBlock* NextLevel,
	ULocalizingTextBlock* CurrentPoint, ULocalizingTextBlock* MaxPoint, 
	ULocalizingTextBlock* CurrentGold, ULocalizingTextBlock* MaxGold, UProgressBar* ExpProgressBar, UTextBlock* ExpText)
{
	URGameInstance* RGameInstance = RGAMEINSTANCE(this);
// 	if (IsValid(RGameInstance) == false)
// 		return;
	int32 CurrentGuldLevel = RGameInstance->GuildInfo->MyGuildInfo.level;
	int32 NextGuldLevel = CurrentGuldLevel + 1;
	CurrentLevel->SetText(FText::AsNumber(CurrentGuldLevel));
	NextLevel->SetText(FText::AsNumber(NextGuldLevel));
	int32 NeedLevelUpPoint = 0;
	int32 NeedLevelUpGold = 0;
	FName GuildLevelUpKey = FName(*FString::FromInt(NextGuldLevel));
	FGuildLevelUpTableInfo* GuildTableInfo = UTableManager::GetInstancePtr()->GetGuildLevelUpRow(GuildLevelUpKey);
	if (GuildTableInfo)
	{
		NeedLevelUpPoint = GuildTableInfo->point;
		NeedLevelUpGold = GuildTableInfo->gold;
	}
	CurrentPoint->SetText(FText::AsNumber(RGameInstance->GuildInfo->MyGuildInfo.point));
	CurrentGold->SetText(FText::AsNumber(RGameInstance->GuildInfo->MyGuildInfo.gold));
	MaxPoint->SetText(FText::AsNumber(NeedLevelUpPoint));
	MaxGold->SetText(FText::AsNumber(NeedLevelUpGold));
	float percent = (float)(RGameInstance->GuildInfo->MyGuildInfo.point) / (float)NeedLevelUpPoint;
	ExpProgressBar->SetPercent(percent);
	ExpText->SetText(FText::AsPercent((double)percent));
}
