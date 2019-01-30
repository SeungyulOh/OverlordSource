

#pragma once

#include "UI/RBaseWidget.h"
#include "Network/PacketDataStructures.h"
#include "UP_Guild_Visit.generated.h"

class UHeroBaseScrollItem;
class UUC_GuildRuneRequestPopup;
class URScrollView;
/**
*
*/
UCLASS()
class PROJECTR_API UUP_GuildVisit : public URBaseWidget
{
	GENERATED_BODY()

public:
	void NativeConstruct() override;
	void NativeDestruct() override;
	void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;
public:
	void Init();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UUP_GuildVisit)
	ULocalizingTextBlock*		GuestGuildNameText;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UUP_GuildVisit)
	ULocalizingTextBlock*		GuestGuildMasterNameText;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UUP_GuildVisit)
	UTextBlock*					GuestGuildRankingText;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UUP_GuildVisit)
	UTextBlock*					GuestGuildPointText;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UUP_GuildVisit)
	UTextBlock*					GuestGuildGoldText;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UUP_GuildVisit)
	ULocalizingTextBlock*		GuestGuildMemberCntText;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UUP_GuildVisit)
	ULocalizingTextBlock*		GuestGuildMemberMaxText;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UUP_GuildVisit)
	ULocalizingTextBlock*		GuestGuildMemberMaxNotice;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UUP_GuildVisit)
	URScrollView*				GuestGuildMemberScrollView;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UUP_GuildVisit)
	UImage*						GuestGuildLevelIconImage;

// private:
// 	bool						bCoolTimeCheck = false;
// 	bool						bTimeTickCheck = false;
// 	int64						nCurrentTime = 0;
// 	int32						nRemainTime = 0;
	UPROPERTY()
	TArray<class UGuildManagerListScrollItemData*> cachedData;
};