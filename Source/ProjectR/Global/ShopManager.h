// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Object.h"
#include "Network/PacketDataStructures.h"
#include "ShopManager.generated.h"


//class URGameInstance;
struct FShopDisplayGroupTableInfo;

UENUM(BlueprintType)		//"BlueprintType" is essential to include
enum class EShopDisplayGroupEnum : uint8
{
	VE_None				UMETA(DisplayName = "None"),
	VE_Group1			UMETA(DisplayName = "1.Group1"),
	VE_Group2			UMETA(DisplayName = "2.Group2"),
	VE_Group3			UMETA(DisplayName = "3.Group3"),
	VE_Group4			UMETA(DisplayName = "4.Group4"),
	VE_Group5			UMETA(DisplayName = "5.Group5"),
	VE_Group6			UMETA(DisplayName = "6.Group6"),
};

USTRUCT(BlueprintType)
struct PROJECTR_API FShopProductInfo
{
	GENERATED_USTRUCT_BODY()

public:

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = ShopProductInfo)
	FName		ProductID;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = ShopProductInfo)
	ESHOP_PRODUCT_TYPE		ProductType;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = ShopProductInfo)
	FName		ProductKey;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = ShopProductInfo)
	FName		ProductName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = ShopProductInfo)
	FName		ProductDesc;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = ShopProductInfo)
	int32		SellCount;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = ShopProductInfo)
	int32		MaxPurchaseCount;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = ShopProductInfo)
	int32		CurrentPurchaseCount;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = ShopProductTableInfo)
	FName		CurrencyType;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = ShopProductInfo)
	int32		Price;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = ShopProductInfo)
	int32				buyType;

};

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnChangeShopData);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnClickBuyBtn, FSHOP_FLOORSHOP_SLOT, data);
/**
 * 
 */
UCLASS()
class PROJECTR_API UShopManager : public UObject
{
	GENERATED_BODY()

public:

	void								Initialize(/*URGameInstance* RGameInstance*/);

	void								PrepareSellingProducts();
	void								PrepareShopDisplayGroup();

	void								SetShopUserData(FSHOP_USER_DATA& InShopUserData);

	FSHOP_PRODUCT_STATUS				GetShopProductStatus(FName InProductID);
	int32								GetShopProductBuyCount(FName InProductID);

	FShopProductInfo*					GetShopProductInfoPtr(FName InProductID);
	FShopDisplayGroupTableInfo*			GetShopDisplayGroupInfo(int32 InDisplayGroupIdx);
	TArray< FName >*					GetShopDisplayList(int32 InDisplayGroupIdx);
	void								GetShopDisplayList(int32 InIndex, TArray< FName >& InOutArray);

	void								SetHeroShopPanelList(TArray<FHERO_SHOP_PANEL>& InList);
	FORCEINLINE TArray<FHERO_SHOP_PANEL>&			GetHeroShopPanelList() { return HeroShopPanelList; }
	void								UpdateHeroShopPanel(FHERO_SHOP_PANEL& InPanel);
	void								UpdateHeroProduct(FHERO_SHOP_PRODUCT& InProduct);
	FHERO_SHOP_PANEL*					GetHeroShopPanel(FString InPanelID);
	FHERO_SHOP_PANEL*					GetHeroShopPanelByProductID(FString InProductID);
	FDateTime							GetShopPanelTime(FString InPanelID);
	void SetFloorShopSlotArrData(TArray<FSHOP_FLOORSHOP_SLOT> dataList);
	TArray<FSHOP_FLOORSHOP_SLOT> GetFloorShopSlotArrData();
	FORCEINLINE void SetShopID(int32 id)	{ shopId = id; }
	FORCEINLINE int32 GetShopID()			{ return shopId; }

	UFUNCTION()
	void								UpdateResetRemainTime(int32 remainSec);


	FOnChangeShopData							OnShopDataChange;
	FOnClickBuyBtn BuyItemDelegate;

private:
// 	UPROPERTY()
// 	URGameInstance*								GameInstance = nullptr;						// Gamestate class can't access GetGameInstance.. because GetWorld is invalid..
	
	FTimerHandle								ResetRemainTimeHandler;	

	//TMap< FName, FShopProductInfo >				ShopProductMap;
	TArray< FShopProductInfo >					ShopProductList;
	TMap< int32, TArray< FName > >				SellingProducts;

	TMap< int32, FShopDisplayGroupTableInfo* >	ShopDisplayGroups;

	FSHOP_USER_DATA								ShopUserData;

	TArray<FHERO_SHOP_PANEL>					HeroShopPanelList;
	TMap<FString, FDateTime>					HeroShopPanelTimeMap;
	TArray<FSHOP_FLOORSHOP_SLOT>				FloorShopSlotArr;
	int32 shopId = -1;
	int32 remainTime = 0;

//	Ãþ »óÁ¡
public:
	void	RequestFloorStoreStatus(uint32	InFloor, bool InIsUseTicket);
	void	RequestFloorShopItemBuy(int32 slotId);
	void	RequestFloorShopSlotOpen(int32 slotId);
	void	RequestFloorStoreReset(uint32	InFloor, uint32 InStoreType, bool InIsUseGem);
// 	void	ResponFloorStoreStatus(FSHOP_FLOORSTORE_STATUS InCurFloorStoreStatus);
// 	void	ResponFloorStoreItemBuy(FSHOP_FLOORSTORE_STATUS InCurFloorStoreStatus, TArray<FREWARD>& InReward);
// 	void	ResponFloorStoreReset(FSHOP_FLOORSTORE_STATUS InCurFloorStoreStatus);
	void	UpdateFloorStoreStatus();
	void	UpdateFloorStoreItemBuy();
	void	UpdateFloorStoreReset();

	FORCEINLINE void					SetCurFloor(uint32 InCurFloor) 
	{
		CurFloor = InCurFloor; 
		CurFloor = FMath::Min(uint32(24), CurFloor);
		CurFloor = FMath::Max(uint32(1), CurFloor);
	}
// 	FORCEINLINE void					SetCurStore(ESHOP_FLOORSTORE_TYPE InCurStore) { CurStore = InCurStore; }
	FORCEINLINE uint32					GetCurFloor() 
	{ 
		CurFloor = FMath::Min(uint32(24), CurFloor);
		CurFloor = FMath::Max(uint32(1), CurFloor);
		return CurFloor; 
	}
// 	FORCEINLINE ESHOP_FLOORSTORE_TYPE	GetCurStore() { return CurStore; }

public:
	int32 CurrShopID = -1;
// 	FSHOP_FLOORSTORE_STATUS		CurFloorStoreStatus;
	TArray<FREWARD>				Reward;
	uint32						CurFloor = 1;
// 	ESHOP_FLOORSTORE_TYPE		CurStore = ESHOP_FLOORSTORE_TYPE::COMMON;
//	Ãþ »óÁ¡
};
