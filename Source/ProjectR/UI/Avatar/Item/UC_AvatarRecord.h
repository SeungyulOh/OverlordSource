// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "UI/RBaseWidget.h"
#include "UI/UIDefs.h"
#include "UC_AvatarRecord.generated.h"


/**
 * 
 */
UCLASS()
class PROJECTR_API UUC_AvatarRecord : public URBaseWidget
{
	GENERATED_BODY()
	
public:
	void NativeConstruct() override;
	void NativeDestruct() override;

	void SetValue(int32 InTotalCount, int32 InWinCount);

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UC_AvatarRecord|Ref")
	UTextBlock*				Text_Title = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UC_AvatarRecord|Ref")
	UTextBlock*				Text_Total = nullptr;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UC_AvatarRecord|Ref")
	UTextBlock*				Text_Win = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UC_AvatarRecord)
	FName					TextKey;

private:
	int32					TotalCount = 0;
	int32					WinCount = 0;

};
