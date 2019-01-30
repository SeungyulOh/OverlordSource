#pragma once
#include "UI/RBaseWidget.h"
#include "UI/Control/RScrollItem.h"
#include "GuildRuneDonationScrollItem.generated.h"

class UHeroBaseScrollItem;

/**
*
*/
UCLASS()
class PROJECTR_API UUC_GuildRuneDonationScrollItem : public URScrollItem
{
	GENERATED_BODY()

public:
	void NativeConstruct() override;
	void NativeDestruct() override;
	void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;
	void SetScrollItemData(int32 Index, URScrollItemData* item, URScrollView* ScrollView) override;

public:
	UFUNCTION()
	void OnClickRuneDonationButton();

public:
	void SetTimeText(int32 hour, int32 min, int32 sec);

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = GuildRuneDonationScrollItem)
	UImage*						AvatarImage = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = GuildRuneDonationScrollItem)
	ULocalizingTextBlock*		AvatarNick = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = GuildRuneDonationScrollItem)
	ULocalizingTextBlock*		AvatarGuildGrade = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = GuildRuneDonationScrollItem)
	UHeroBaseScrollItem*		RequestHeroIcon = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = GuildRuneDonationScrollItem)
	UTextBlock*					RuneCount = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = GuildRuneDonationScrollItem)
	UTextBlock*					CurrentCount = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = GuildRuneDonationScrollItem)
	UTextBlock*					MaxCount = nullptr;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = GuildRuneDonationScrollItem)
	UProgressBar*				CurrentRuneProgressBar = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = GuildRuneDonationScrollItem)
	UTextBlock*					RemainHour = nullptr;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = GuildRuneDonationScrollItem)
	UTextBlock*					RemainMinute = nullptr;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = GuildRuneDonationScrollItem)
	UTextBlock*					RemainSeconds = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = GuildRuneDonationScrollItem)
	UWidgetSwitcher*			DonationStateSwitcher = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = GuildRuneDonationScrollItem)
	UButton*					Button_SupportRune = nullptr;

private:
	bool				bTimeTickCheck = false;
	int64				nCurrentTime = 1.0;
	int32				nRemainTime = 0;
};