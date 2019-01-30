// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include "Network/PacketDataEnum.h"
#include "CurrencyPropertyTableInfo.generated.h"

/**
 * 
 */
USTRUCT()
struct PROJECTR_API FCurrencyPropertyTableInfo : public FTableRowBase
{
	GENERATED_USTRUCT_BODY()

public:	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = FCurrencyPropertyTableInfo)
	FName ItemIconKey;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = FCurrencyPropertyTableInfo)
	FName CommonStringKey;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = FCurrencyPropertyTableInfo)
	FName TooltipType;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = FCurrencyPropertyTableInfo)
	FName Tooltip;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = FCurrencyPropertyTableInfo)
	int32 Value;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = FCurrencyPropertyTableInfo)
	EREWARD_TYPE	CurrencyType = EREWARD_TYPE::NONE;

};

/**
 * for FWalletTableInfo.PropertyType
 */
UENUM(BlueprintType)
enum class EWalletPropertyType : uint8
{
	None, // ?
	Currency,
	Energy,
	Material,
	Rune,
	HeroPiece,
	Consumable,

	Max,
};

USTRUCT()
struct PROJECTR_API FWalletTableInfo : public FTableRowBase
{
	GENERATED_USTRUCT_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = FWalletTableInfo)
	FName			RewardId;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = FWalletTableInfo)
	int32			RefillAmount;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = FWalletTableInfo)
	int32			RefillPeriod;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = FWalletTableInfo)
	int32			RefillMax;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = FWalletTableInfo)
	FName			Enum;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = FWalletTableInfo)
	int32			SellAvailable = 0;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = FWalletTableInfo)
	int32			SellPrice = 0;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = FWalletTableInfo)
	EWalletPropertyType PropertyType = EWalletPropertyType::None;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = FWalletTableInfo)
	FName			CommonStringKey;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = FWalletTableInfo)
	int32			ArrGroup;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = FWalletTableInfo)
	int32			ArrIndex;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = FWalletTableInfo)
	int32			Grade;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = FWalletTableInfo)
	FName			ItemIconKey;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = FWalletTableInfo)
	int32			Sorting_Order;

	// item tooltip function is no more/legacy.
	// These strings are used in item info popup. See UC_WalletItemInfo.
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = FWalletTableInfo)
	FName			TooltipType;
	/** for UC_WalletItemInfo upper description textblock */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = FWalletTableInfo)
	FName			Tooltip;
	/** for UC_WalletItemInfo lower description textblock */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = FWalletTableInfo)
	FName			Tooltip2;
};
