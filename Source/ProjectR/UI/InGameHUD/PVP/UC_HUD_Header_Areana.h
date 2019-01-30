// Copyright(C) 2018 KOKOMO Games. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "UI/RBaseWidget.h"
#include "UI/UI_Battle/RBattleTimer.h"
#include "UC_HUD_Header_Areana.generated.h"

/**
 * 
 */
USTRUCT(BlueprintType)
struct PROJECTR_API FVariable_UUC_header_Arena
{
	GENERATED_USTRUCT_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = FVariable_UUC_header_Arena)
		TArray<UTextBlock*> TB_GuildNames;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = FVariable_UUC_header_Arena)
		TArray < UTextBlock*> TB_AvatarNames;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = FVariable_UUC_header_Arena)
		URBattleTimer* BattleTimer;


};

UCLASS()
class PROJECTR_API UUC_HUD_Header_Areana : public URBaseWidget
{
	GENERATED_BODY()
	
public:
	void NativeConstruct() override;
	void InitAvatarInfo(const FString& nick, const FString& guild, bool isMe);
	void SetAvatarInfo(bool isMe);
	
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UUC_HUD_Header_Areana)
	FVariable_UUC_header_Arena variables;

private:
	TArray<FString>	NickName;
	TArray<FString> GuildName;
};
