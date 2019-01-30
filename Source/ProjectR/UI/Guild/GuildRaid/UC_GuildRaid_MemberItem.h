#pragma once


#include "UI/Control/RScrollItem.h"
#include "UC_GuildRaid_MemberItem.generated.h"

/**
*
*/
UCLASS()
class PROJECTR_API UUC_GuildRaid_MemberItem : public URScrollItem
{
	GENERATED_BODY()

public:
	void SetScrollItemData(int32 Index, URScrollItemData* item, URScrollView* ScrollView) override;

	void Refresh();
private:
	int32 GetDays(int32 InTick);
	int32 GetHours(int32 InTick);
	int32 GetMinutes(int32 InTick);
	int32 GetSeconds(int32 InTick);

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UUC_GuildRaid_MemberItem)
	ULocalizingTextBlock*	MemberNameText = nullptr;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UUC_GuildRaid_MemberItem)
	UTextBlock*				ChallengeCount = nullptr;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UUC_GuildRaid_MemberItem)
	UTextBlock*				CreateCount = nullptr;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UUC_GuildRaid_MemberItem)
	ULocalizingTextBlock*	LocTimeText1 = nullptr;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UUC_GuildRaid_MemberItem)
	ULocalizingTextBlock*	LocTimeText2 = nullptr;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UUC_GuildRaid_MemberItem)
	UTextBlock*				TimeNum1 = nullptr;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UUC_GuildRaid_MemberItem)
	UTextBlock*				TimeNum2 = nullptr;

};