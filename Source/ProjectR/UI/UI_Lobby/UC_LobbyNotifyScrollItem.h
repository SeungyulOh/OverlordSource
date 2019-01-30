// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "UI/Control/RScrollItem.h"
#include "NotifyList/NotifyDefs.h"
#include "UC_LobbyNotifyScrollItem.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTR_API UUC_LobbyNotifyScrollItem : public URScrollItem
{
	GENERATED_BODY()
	
public:
	void NativeConstruct() override;
	void SetScrollItemData(int32 Index, URScrollItemData* item, URScrollView* ScrollView) override;

	UFUNCTION(BlueprintCallable, Category = UUC_LobbyNotifyScrollItem)
	void Go();


private:
	//FNotifyQueueTableInfo*  GetNotifyQueueTableInfo(ENotifyEventTypeEnum eventtype);

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UUC_LobbyNotifyScrollItem)
	ULocalizingTextBlock*		TitleTextBlock = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UUC_LobbyNotifyScrollItem)
	UButton*					BP_Btn_Move = nullptr;

	//UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UUC_PDDefenceScrollItem)
	//TArray<UHeroIconWithButton*>			HeroIconList;

};
