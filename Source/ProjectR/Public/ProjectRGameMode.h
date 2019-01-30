// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/GameMode.h"
#include "CustomStructures/InfiniteSpawnerInfo.h"
#include "GameState/Stage/Spawner/BaseStageInfo.h"
#include "ProjectRGameMode.generated.h"

class URGameInstance;
class ABaseSequenceActor;
class ABulletBase;

UENUM(BlueprintType)		//"BlueprintType" is essential to include
enum class EStageContextEnum : uint8
{
	VE_None				UMETA(DisplayName = "None"),
	VE_Progress			UMETA(DisplayName = "Progress"),
	VE_Success			UMETA(DisplayName = "Success"),
	VE_Fail				UMETA(DisplayName = "Fail"),
};
UENUM(BlueprintType)		//"BlueprintType" is essential to include
enum class EPlayStageStarEnum : uint8
{
	VE_None				UMETA(DisplayName = "None"),
	VE_Star1			UMETA(DisplayName = "Star1"),
	VE_Star2			UMETA(DisplayName = "Star2"),
	VE_Star3			UMETA(DisplayName = "Star3"),
};


USTRUCT()
struct PROJECTR_API FObjectHelper
{
	GENERATED_USTRUCT_BODY()
public:
	void GetObjectList(TArray< TWeakObjectPtr< AActor > >& outObjectList);
	void GetObjectList(TArray<int32> UOKeyList, TArray< TWeakObjectPtr< AActor > >& outObjectList);
	void GetFriendList(TArray< TWeakObjectPtr< AActor > >& outFriendList, AActor* Source);
	void GetPartyList(TArray< TWeakObjectPtr< AActor > >& outFriendList, AActor* Source);
	void GetEnemyList(TArray< TWeakObjectPtr< AActor > >& outEnemyList, AActor* Source);
	void GetMySummonList(TArray< TWeakObjectPtr< AActor > >& outEnemyList, AActor* Source);
	void GetFriendSummonList(TArray< TWeakObjectPtr< AActor > >& outFriendSummonList);

	void AddObject(AActor* obj, int32 InPlayerUK = 0);
	void RemoveObject(AActor * obj, bool bDestory = false);
	void RemoveObject(int32 InPlayerUK, bool bDestory = false);
	void RemoveObjectAll(ECharacterCategoryTypeEnum CharacterCategory);
	void RemoveObjectAll(TArray< TWeakObjectPtr< AActor >> ignoreList);
	void RemoveSelectObjectAll(TArray< TWeakObjectPtr< AActor >> DeleteList);
	void RemoveObjectAll();

	void FindTarget(TArray< TWeakObjectPtr< AActor > >& outArray, AActor* Source, FVector targetLocation, EFriendshipTypeEnum friendshipType, ESearchShapeTypeEnum applyType, FVector value0, float rotate, bool bDrawDebug = false);
	void FindTargetFriendType(TArray< TWeakObjectPtr< AActor > >& outArray, AActor* Source, EFriendshipTypeEnum friendshipType);
	AActor* GetOverlapTargets(TArray< TWeakObjectPtr< AActor > >& outArray, AActor* Source, EFriendshipTypeEnum friendshipType, float overlapScale = 1.0f);
	AActor* GetOverlapTargetFromPosition(TArray< TWeakObjectPtr< AActor > >& outArray, AActor* Source, FVector SourceLocation, float overlapRange, EFriendshipTypeEnum friendshipType);

	TWeakObjectPtr< AActor > GetObjectByPlayerUK(int32 InPlayerUK);
	void RegisterPlayerUK(AActor* obj, int32 InPlayerUK);
	void RemoveObjectByKey(int32 InPlayerUK);

	int32 GeneratePlayerUK();


	FORCEINLINE bool GetAllObjectVisible() { return AllObjectVisible; }


	// Object Pause
	void SetAllObjectVisible(bool IsVisible, TArray< AActor* > IgnoreObject = TArray< AActor* >());
	void SetAllObjectPause(float SlowTime, float SlowRate, AActor* IgnoreObject = nullptr);
	void SetAllObjectBreak(bool InIsNotBreak = false, bool InPauseDamage = false);
	void SetAllAIStop();
	void CheckIgnoreForcePauseTimeEnd(AActor* obj);


	// Object BlackOut
	void SetAllObjectBlackOut(AActor* IgnoreObject = nullptr);
	void CheckIgnoreBlackOutTimeEnd(AActor* obj);
	void SetBlackOut(bool bEnable);

	void SetAllObjectHPBarVisible(bool IsVisible);
	void HpBarWidgetDestroy();

	void		SetAssetResource(TArray<FSoftObjectPath>& InAssets, FName& InHeroID, bool InNpc = false);
	void		SetAssetResource_FromTables(TArray<FSoftObjectPath>& InAssets);


	UPROPERTY()
	TArray<FSoftObjectPath> AssetsQueue;
	UPROPERTY()
	TArray<UObject*> CachedResourceArray;
	UPROPERTY()
	TArray<UClass*> CachedUClassArray;

	TArray< TWeakObjectPtr< AActor > >			ObjectList;

	TArray< TWeakObjectPtr< ABulletBase > >		BulletList;
	TArray< TWeakObjectPtr< AActor > >			IgnorePauseList;
	TArray< TWeakObjectPtr< AActor > >			IgnoreBlackOutList;

	TMap< int32, TWeakObjectPtr<AActor> >		PlayerUKMap;

	int32										PlayerUKSerial = 0;

	UPROPERTY()
		TWeakObjectPtr<ABaseSequenceActor>			VictorySequenceActor;
	UPROPERTY()
		TWeakObjectPtr<	AActor >					MainCameraActor;

public:
	bool										AllObjectVisible = true;
};

/**
 * 
 */
UCLASS()
class PROJECTR_API AProjectRGameMode : public AGameModeBase//: public AGameMode
{
	GENERATED_BODY()

public:
	AProjectRGameMode(const FObjectInitializer& ObjectInitializer);
	
	virtual void InitGameMode();		// GameMode Initialize
	virtual void PrepareGameMode();		// Object Create, GamePlayMode Init(Time, Enemy..)
	virtual void ReadyGameMode();		// Start Matinee, Effect
	virtual void StartGameMode();		// Play Game
	virtual void EndGameMode();			// End Matinee, Effect
	virtual void ResultGameMode();		// Result UI
	virtual void StopGameMode();
	virtual void GamePlayModeEnd();
	virtual void ResultUI();

	virtual void DestroyResource();

	virtual EStageContextEnum GetStageContext() { return EStageContextEnum::VE_None; }

	void EndPlay(const EEndPlayReason::Type EndPlayReason) override;
	void BeginPlay() override;
public:
	UFUNCTION()
	virtual void OnCompleteResourcePreLoading();

	UFUNCTION(BlueprintCallable, Category = GamePlayMode)
	void ChangeGameModeState(EGameModeStateEnum state, bool inForceExecute = false);

	void CheckGameModeState(bool inForceExecute = false);

	/*
	Override functions..
	*/
	void Tick(float DeltaTime) override;
	
	// Console Exec Function
	bool ProcessConsoleExec(const TCHAR* Cmd, FOutputDevice& Ar, UObject* Executor) override;



public:
	UPROPERTY()
	FName				GameModeSequenceKey;
	UPROPERTY()
	FTimerHandle		DelayedGameMode_TimeHandler;
	UPROPERTY()
	EGameModeStateEnum	DelayedGameModeState = EGameModeStateEnum::VE_None;

	UPROPERTY()
	class UDirectionTrackManager* DirectionTrackManager;

	UFUNCTION()
	void		DelayNextStage(EGameModeStateEnum InDelayedNextModeState, float DelayTime);
	void		OnChangeDelayedNextStage();
	void		ClearDelayedNextStage();
	void		StageClearSuccess(EPlayStageStarEnum InGrade);
	void		StageClearFail();
	void		InitCameraTransform(FVector CameraPosition);

	UPROPERTY()
	FObjectHelper		ObjectHelper;

	void GamePlayModeEnd(bool IsSuccess);

	UPROPERTY()
	URGameInstance*		GameInstance = nullptr;

	UPROPERTY()
	class UDamageFloaterManager* DamageFloaterMgr;

	UPROPERTY()
	class AActor*	VictoryPositionActor;


	EStageContextEnum	GameContextState = EStageContextEnum::VE_None;
	EGameModeStateEnum	GameModeState = EGameModeStateEnum::VE_None;
	EPlayStageStarEnum	StarGrade = EPlayStageStarEnum::VE_None;
	EPlayStageStarEnum	SaveStarGrade = EPlayStageStarEnum::VE_None;
	EGameModeStateEnum	NextGameModeState = EGameModeStateEnum::VE_None;
	
	double				LoadingStartTime;
	float				StageTime;
	bool				bIsTimeOut = false;

	UPROPERTY()
	UMaterialInstanceConstant* MIC_Android;

	UPROPERTY()
	class ARSceneCapturer* SceneCapturer;

	UPROPERTY()
	UMaterialInstanceDynamic* MID;




	// for performance
	int32									iTryCntToLoadSync = 0;

	FORCEINLINE int32 GetTryCntLoadSync() { return iTryCntToLoadSync; }
	void AddTryCntLoadSync(FString id);

protected:
	int32 iPartition = 5;

	


	

};