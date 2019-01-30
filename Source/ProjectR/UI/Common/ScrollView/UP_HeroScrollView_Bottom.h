// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/RBaseWidget.h"
#include "RBaseDeckWidget.h"
#include "UI/Control/RScrollItemData.h"
#include "UI/Control/RScrollItem.h"
#include "UI/Control/RScrollView.h"
#include "Delegates/DelegateCombinations.h"
#include <list>
#include "UP_HeroScrollView_Bottom.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FBlackboardStateChanged);

const int32 MAX_DECKCOUNT = 5;

UCLASS()
class PROJECTR_API UHeroScrollItemData_New : public URScrollItemData
{
	GENERATED_BODY()

public:

	UPROPERTY()
	FString HeroUD;

	enum SelectType
	{
		IDLE,
		SELECTING,
		SELECTED,
		MAX
	};

	SelectType eSelectType = MAX;
};

UCLASS()
class PROJECTR_API UHeroScrollItem_New : public URScrollItem
{
	GENERATED_BODY()
public:
	void InvalidateData() override;
	void Selected() override;


public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UHeroScrollItem_New)
	class URenewal_BaseHeroIcon* HeroIcon = nullptr;
};

UCLASS()
class PROJECTR_API UUC_BaseHeroScrollView_New : public URScrollView
{
	GENERATED_BODY()
	
		/*overrided funtions*/
public:
	virtual void			ClearAllSelected() override;
	virtual void			ScrollItemList_ChangeItemDataList(int32 dataIndex) override;
	
	void Update_ChangeItemDataList();
	
		/*inline functions*/
public:
	FORCEINLINE TArray<URScrollItemData*>& GetSelectedScrollItemDataList() { return SelectedScrollItemDataList; }

public:
	EUIPageEnum CurrentUIPage;
};

UENUM(BlueprintType)
enum class EUserBehaviorState : uint8
{
	IDLE,					// if user click nothing
	SELECTING_START,		// if user click heroicon in scrollview
	SELECTING_END,			// if user click available herodeck
	CHANGING_START,			// if user click herodeck
	CHANGING_END,			// if user click herodeck
	DESELECT_SCROLLITEM,
	DESELECT_DECK,
	DESELECT_ALL,
	AUTOSETTING_ALL,
	LOADDECKSETTING,
	Max
};


/*Store meaningful information*/
/*UserState,SelectedHeroUDs are only changed by StateController*/
USTRUCT(BlueprintType)
struct PROJECTR_API FDeckManager_Blackboard
{
	GENERATED_USTRUCT_BODY()

		/*Current Selected HeroUDs*/
		TArray<FString> SelectedHeroUDs;

	std::list<UUserWidget*> WaitingList;

	/*When blackboard data chagned, execute event*/
	FBlackboardStateChanged OnBBStateChanged;

	TArray<class URBaseDeckWidget*> TargetWidgetArray;

	void SetData_OnBlackBoardData(UUserWidget* InWidget);
};

USTRUCT(BlueprintType)
struct PROJECTR_API FUI_StateController
{
	GENERATED_USTRUCT_BODY()

public:
	void ChangeState();
	void Process();



	/*RTS Related*/
	void SendSelectPacket(int32 RoleIdx);
	/**/

	bool isSameHeroInDeck(FString InheroUD, FString TargetUD);


public:
	FORCEINLINE void SetState(EUserBehaviorState InState) { UserState = InState; }
public:
	/*User Input State*/
	/*State transition is controlled in this class*/
	/*if State is changed , OnBBStateChanged event will invoke*/
	EUserBehaviorState UserState = EUserBehaviorState::IDLE;

	FDeckManager_Blackboard * blackboard;
};


/**
 * 
 */
UCLASS()
class PROJECTR_API UUP_HeroScrollView_Bottom : public URBaseWidget
{
	GENERATED_BODY()
public:
	void NativeConstruct() override;
	void NativeDestruct() override;

	UFUNCTION()
	void Update_StateController();


	UFUNCTION()
	void OnClick_ButtonSortingShow();
	UFUNCTION()
	void OnClick_ButtonSortingHide();
	UFUNCTION()
	void OnClick_ButtonFilterShow();

	void Recompose_Itemdatalist(TArray<FString> HeroUDs);
	
public:
	/*ScrollView*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UUP_HeroScrollView_Bottom|Ref")
	UUC_BaseHeroScrollView_New*			ScrollView = nullptr;
	
	/*Buttons*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UUP_HeroScrollView_Bottom|Ref")
	UButton*				Button_Sorting_Show;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UUP_HeroScrollView_Bottom|Ref")
	UButton*				Button_Sorting_Hide;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UUP_HeroScrollView_Bottom|Ref")
	UButton*				Button_Filter_Show;

	/*Switchers*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UUP_HeroScrollView_Bottom|Ref")
	UWidgetSwitcher*		Switcher_Sorting;

	/*UC*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UUP_HeroScrollView_Bottom|Ref")
	class UUC_Hero_SortingFilter*			UC_Sorting;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UUP_HeroScrollView_Bottom|Ref")
	class UUC_Hero_MultiFilter*				UC_Filter;
	
	

	/*Memory Optimization*/
	UPROPERTY()
	TArray<UHeroScrollItemData_New*> cachedItemDataList;

	/*State*/
	UPROPERTY()
	FDeckManager_Blackboard Blackboard;
	UPROPERTY()
	FUI_StateController StateController;
	
};
