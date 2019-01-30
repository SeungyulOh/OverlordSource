

#pragma once

#include "UI/RBaseWidget.h"
#include "Network/PacketDataStructures.h"
#include "UP_GuildRuneDonation.generated.h"

class UHeroBaseScrollItem;
class UUC_GuildRuneRequestPopup;
class URScrollView;
/**
*
*/
UCLASS()
class PROJECTR_API UUP_GuildRuneDonation : public URBaseWidget
{
	GENERATED_BODY()

public:
	void NativeConstruct() override;
	void NativeDestruct() override;
	void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;

public:
	UFUNCTION()
	void OnGuildRuneStatusRpDelegate();
	
	UFUNCTION()
	void OnClickRequestButton();

	UFUNCTION()
	void OnClickReceiveButton();
// 	UFUNCTION(BlueprintCallable, Category = RuneDonation)
public:
	void Init();
	void SetMyAvatarInfo(FGUILD_RUNE_REQUESTER& MyRuneRequest);
	void SetHeroRuneInfo(FGUILD_RUNE_REQUESTER& MyRuneRequest);
	void SetTimeInfo(int32 timeLeft);
	void SetCoolTime(int32 timeLeft);


public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = RuneDonation)
	URScrollView*				RuneRequestScrollView = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = RuneDonation)
	UTextBlock*					RequestCount = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = RuneDonation)
	UImage*						AvatarImage = nullptr;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = RuneDonation)
	ULocalizingTextBlock*		AvatarNick = nullptr;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = RuneDonation)
	ULocalizingTextBlock*		AvatarGuildGrade = nullptr;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = RuneDonation)
	UHeroBaseScrollItem*		RequestHeroIcon = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = RuneDonation)
	UTextBlock*					RuneCount = nullptr;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = RuneDonation)
	UTextBlock*					CurrentCount = nullptr;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = RuneDonation)
	UTextBlock*					MaxCount = nullptr;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = RuneDonation)
	UProgressBar*				CurrentRuneProgressBar = nullptr;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = RuneDonation)
	UTextBlock*					RemainHour = nullptr;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = RuneDonation)
	UTextBlock*					RemainMinute = nullptr;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = RuneDonation)
	UTextBlock*					RemainSeconds = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = RuneDonation)
	UWidgetSwitcher*			RequestStateSwitcher = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = RuneDonation)
	UWidgetSwitcher*			MyStateSwitcher = nullptr;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = RuneDonation)
	UWidgetSwitcher*			RequestAbleSwitcher = nullptr;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = RuneDonation)
	UButton*					ReceiveButton;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = RuneDonation)
	UButton*					RequestButton;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = RuneDonation)
	UUC_GuildRuneRequestPopup*	RequestRunePopup = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = RuneDonation)
	URRichTextBlock*			RequestDesc = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = RuneDonation)
	URRichTextBlock*			CoolTimeText = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = RuneDonation)
	URRichTextBlock*			MyRequestText1 = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = RuneDonation)
	URRichTextBlock*			MyRequestText2 = nullptr;

private:
	bool						bCoolTimeCheck = false;
	bool						bTimeTickCheck = false;
	int64						nCurrentTime = 0;
	int32						nRemainTime = 0;
};