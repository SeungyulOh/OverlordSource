// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Kismet/BlueprintFunctionLibrary.h"
#include "SharedConstants/GlobalConstants.h"
#include "Global/RGameInstance.h"
#include "Classes/Navigation/PathFollowingComponent.h"
#include "Network/PacketDataEnum.h"
#include "Network/PacketDataStructures.h"
#include "Table/CharacterTableInfo.h"
#include "UtilFunctionLibrary.generated.h"


class AProjectRGameMode;
class URInGameHUD;
class ARecastNavMesh;
class URGameInstance;
class UBehaviorTree;
class ABlockingVolume;
class UPartyManager;
class UParty;
class ADPadController;
class USpeechBubbleComponent;
class ALevelSequenceActor;

/**
 * 
 */
UCLASS()
class PROJECTR_API UUtilFunctionLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()
	
	
public:	 
	UFUNCTION(BlueprintCallable, Category = UtilFunction )
	static void ShowIndicator(FText DisplayText, float InDuration = 0.0f, bool bNoAni = false, bool bBottomMsg = false);
	UFUNCTION(BlueprintCallable, Category = UtilFunction)
	static void HideIndicator();
	UFUNCTION(BlueprintCallable, Category = UtilFunction)
	static bool IsShowIndicator();
	UFUNCTION(BlueprintCallable, Category = UtilFunction)
	static UUserWidget* GetIndicatorWidget();
	UFUNCTION(BlueprintCallable, Category = UtilFunction)
	static void SetIndicatorOffset(float InOffsetX, float InOffsetY);

//	static FSkillTableInfo_V2* GetSkillTableFromHeroUD(const FString& InHeroUD);
	static FSkillTableInfo_V2* GetSkillTableFromHeroID(const FName& InHeroID);

// 	UFUNCTION(BlueprintCallable, Category = UtilFunction)
// 	static bool IsRealTimeGameMode(UWorld* World);

	UFUNCTION(BlueprintCallable, Category = UtilFunction)
	static EGamePlayModeTypeEnum GetGamePlayModeEnum();

//	static AProjectRGameMode* GetGameModeByRGameInstance(URGameInstance* RGameInstance);
	static AProjectRGameMode* GetGamePlayMode();
	static	void	CheckGameModeState();
	static	void	ChangeGameModeState(EGameModeStateEnum  InModeState, bool InIsForce = false);

	static bool	IsPvPContents(UWorld* World);
	/*
	Global object ref..
	*/
	//UFUNCTION(BlueprintCallable, Category = UtilFunction)
	//static URGameInstance* GetRGameInstance(UWorld* World );

	//UFUNCTION(BlueprintCallable, Category = UtilFunction)
	//static URGameInstance* GetRGameInstanceFromActor(AActor* Actor);
	UFUNCTION(BlueprintCallable, Category = "GameInstance")
	static URGameInstance*	GetRGameInstanceWithBluePrint();
	// object manager
// 	UFUNCTION(BlueprintCallable, Category = UtilFunction)
// 		static UGameObjectManager* GetGameObjectManagerFromWorld(UWorld* World);

// 	UFUNCTION(BlueprintCallable, Category = UtilFunction)
// 		static UGameObjectManager* GetGameObjectManagerFromActor(AActor* Actor);

	UFUNCTION(BlueprintCallable, Category = UtilFunction)
	static void DestroyUObject(UObject* TargetObject);

	static bool IsInUnActableState(AActor* TargetActor);
	static class UDirectionTrackManager* GetDirectionTrackManager();


	// Particle
	static UParticleSystemComponent* SpawnEmitterAttachedWithTableID(FName& InTableID, AActor* Target, FName AttachPointName = NAME_None, FVector InLocation = FVector::ZeroVector, FRotator Rotation = FRotator::ZeroRotator, EAttachLocation::Type LocationType = EAttachLocation::KeepRelativeOffset, bool bAutoDestroy = true);

	UFUNCTION(BlueprintCallable, Category = UtilFunction)
	static UParticleSystemComponent* SpawnEmitterAttachedWithAssetID( TAssetPtr< UParticleSystem > InAssetID, AActor* Target, FName AttachPointName = NAME_None, FVector InLocation = FVector::ZeroVector, FRotator Rotation = FRotator::ZeroRotator, EAttachLocation::Type LocationType = EAttachLocation::KeepRelativeOffset, bool bAutoDestroy = true);

	UFUNCTION(BlueprintCallable, Category = UtilFunction)
	static UParticleSystemComponent* SpawnEmitterAttached(UParticleSystem* EmitterTemplate, AActor* Target, FName AttachPointName = NAME_None, FVector InLocation = FVector::ZeroVector, FRotator Rotation = FRotator::ZeroRotator, EAttachLocation::Type LocationType = EAttachLocation::KeepRelativeOffset, bool bAutoDestroy = true);

	UFUNCTION(BlueprintCallable, Category = UtilFunction)
	static bool						ComputeEffectSpeed(AActor* InTargetActor, UParticleSystemComponent* InTargetParticle);

	UFUNCTION(BlueprintCallable, Category = UtilFunction)
	static UParticleSystemComponent* CustomSpawnEmitterAttached(UParticleSystem* EmitterTemplate, USceneComponent* AttachToComponent, FName AttachPointName, FVector Location, FRotator Rotation, EAttachLocation::Type LocationType = EAttachLocation::KeepRelativeOffset, bool bAutoDestroy = true);

	UFUNCTION(BlueprintCallable, Category = UtilFunction)
	static UParticleSystemComponent* CustomSpawnEmitterAtLocation(UObject* WorldContextObject, class UParticleSystem* EmitterTemplate, FVector Location, FRotator Rotation = FRotator::ZeroRotator, bool bAutoDestroy = true);

//	UFUNCTION(BlueprintCallable, Category = UtilFunction)
	static void SpawnFX(TWeakObjectPtr<UParticleSystemComponent> &Result, FName FXKey, AActor* TargetActor, FVector SpawnLocation, FRotator SpawnRotator, FName SocketName);

	UFUNCTION(BlueprintCallable, Category = UtilFunction)
	static void NextTargetEffect(AActor* InSourceActor, AActor* InDestActor, float InLifeTime, bool InCheckPrevHide = true);


	/*
	sound
	*/
	UFUNCTION(BlueprintCallable, Category = UtilFunction)
	static void PlaySound2D(UWorld* InWorld, FName SoundKey, float VolumeMultiplier = 1.f, float PitchMultiplier = 1.f);

	UFUNCTION(BlueprintCallable, Category = UtilFunction)
	static void PlaySoundSceneComponent(FName SoundKey, USceneComponent* Comp, FName SocketName = NAME_None, float VolumeMultiplier = 1.f, float PitchMultiplier = 1.f);

	UFUNCTION(BlueprintCallable, Category = UtilFunction)
	static void PlaySoundLocation(UWorld* InWorld, FName SoundKey, FVector Location, float VolumeMultiplier = 1.f, float PitchMultiplier = 1.f);

	UFUNCTION(BlueprintCallable, Category = UtilFunction)
	static void PauseBGSound();

	UFUNCTION(BlueprintCallable, Category = UtilFunction)
	static void ResumeBGSound();

	UFUNCTION(BlueprintCallable, Category = UtilFunction)
	static void FadeInBGM();

	UFUNCTION(BlueprintCallable, Category = UtilFunction)
	static void FadeOutBGM();


	//Math
	UFUNCTION(BlueprintCallable, Category = UtilFunction)
	static float GetBetweenDegree(FVector SourceDir, FVector DestDir);

	UFUNCTION(BlueprintCallable, Category = UtilFunction)
	static ECharacterDirection4REnum GetBetweenDirFrontBack(FVector SourceDir, FVector DestDir);

	UFUNCTION(BlueprintCallable, Category = UtilFunction)
	static ECharacterDirection4REnum GetBetweenDirLeftRight(FVector SourceDir, FVector DestDir);

	UFUNCTION(BlueprintCallable, Category = UtilFunction)
	static ECharacterDirection4Enum GetBetweenDir4(FVector SourceDir, FVector DestDir);

	UFUNCTION(BlueprintCallable, Category = UtilFunction)
	static ECharacterDirection4REnum GetBetweenDir4R(FVector SourceDir, FVector DestDir);

	UFUNCTION(BlueprintCallable, Category = UtilFunction)
	static FVector GetCameraPos(FVector SourcePos, FVector CamPos, FVector CamDir);


// 	UFUNCTION(BlueprintCallable, Category = UtilFunction)
// 	static AActor* GetCommandSlotOwnerCharacter(AActor* Actor, int32 SlotIndex);

// 	UFUNCTION(BlueprintCallable, Category = UtilFunction)
// 	static EHeroSpawnStateEnum GetSpawnState(AActor* Actor, int32 SlotIndex);

	UFUNCTION(BlueprintCallable, Category = UtilFunction)
	static APlayerController* GetPlayerController(UWorld* World);

	UFUNCTION(BlueprintCallable, Category = UtilFunction)
	static ABasePlayerController* GetBasePlayerController(UWorld* World);

	UFUNCTION(BlueprintCallable, Category = UtilFunction)
	static ALobbyPlayerController* GetLobbyPlayerController(UWorld* World);

	UFUNCTION(BlueprintCallable, Category = UtilFunction)
	static ADPadController* GetDPadController(UWorld* World);

	static bool IsLeaderCharacter(AActor* actor);
	static bool IsMyPlayCharacter(AActor* actor);

	UFUNCTION(BlueprintCallable, Category = UtilFunction)
	static bool AIComponentAttach(AActor* actor, FName ainame);
	
// 	UFUNCTION(BlueprintCallable, Category = UtilFunction)
// 	static bool RandPercentage(int32 ClearPercent);

	UFUNCTION(BlueprintPure, meta = (HidePin = "WorldContextObject", DefaultToSelf = "WorldContextObject", DisplayName = "CreateObjectFromClass", CompactNodeTitle = "Create", Keywords = "new create uobject"), Category = CustomNode)
	static UObject* CreateObjectFromClass(UObject* WorldContextObject, TSubclassOf<UObject> USourceClass);

	
// 	UFUNCTION(BlueprintCallable, Category = UtilFunction)
// 	static void ScreenLog(UObject* WorldContextObject, FString message);

// 	UFUNCTION(BlueprintCallable, meta = (HidePin = "WorldContextObject", DefaultToSelf = "WorldContextObject"), Category = UtilFunction)
// 	static void GetGameEnvironmentVar(UObject* WorldContextObject, FGameEnvironmentVar& OutGameVars);
		
// 	UFUNCTION(BlueprintCallable, Category = UtilFunction)
// 	static void SetChangeLocation(AActor* baseCharacter, FVector pos);


	/*
	String Related..
	*/
	static FString GetEnumToString(const TCHAR* Name, int32 Value);
	static FText GetEnumToDisplayName(const TCHAR* Name, int32 Value);
	static FName GetEnumToName(const TCHAR* Name, int32 Value);
	// -1/INDEX_NONE for Str not being one of the enum values
	static int32 StringToEnum(const TCHAR* EnumTypeName, FString ValueString);

	static FName GetResultSkillID(ECharacterCategoryTypeEnum CharacterCategory, const FName InSkillID, int32 InLevel);

	
	/*
	String UtilFunction
	*/
	UFUNCTION(BlueprintCallable, Category = UtilFunction)
	static FText GetLocalizedString(EStringTableType TableType, FName StringID);


	/*
	String Decode
	*/
// 	UFUNCTION(BlueprintCallable, Category = UtilFunction)
// 	static bool Base64Decode(FString& Source, FString& dest);
// 
// 	UFUNCTION(BlueprintCallable, Category = UtilFunction)
// 	static bool Base64DecodeTArray(const FString& Source, TArray<uint8>& Dest);
// 
// 	UFUNCTION(BlueprintCallable, Category = UtilFunction)
// 	static FString Base64Encode(FString& Source);
// 
// 	UFUNCTION(BlueprintCallable, Category = UtilFunction)
// 	static FString Base64EncodeTArray(const TArray<uint8>& Source);

	//template<typename T>
	//static void MakeFormatNamedArguments(const T* TargetStruct, FFormatNamedArguments& InTargetInfoFormatNamedArguments, UCharacterBattleInfo* InCharacterBattleInfo = nullptr);										


	/*
	Get
	*/


	/*
	Item
	*/
	static FName GetItemGradeText(FName itemid);
	static UTexture2D* GetItemGradeIconImg(FName itemid);
	static FSlateColor GetItemGradeColor(FName itemid);

	static bool CanHeroEquipItem(FString HeroUD, FString ItemUD);

	static int32 GetCurrentHaveItemCount(UWorld* World, FName itemName);
	static int32 GetItemManufactureEnableCount(UWorld* World, FName manufactueid);
	static bool IsSameType(EItemEquipArmorTypeEnum	InArmorType, EItemEquipTypeEnum InItemEquipType);
	static bool CanCharacterEquipItem(FName InCharacterID, FName InItemID);

	static EREWARD_TYPE GetRewardTypeFName(FName InType);
	static bool IsCurrencyHave(UWorld* InWorld, FName InCurrencyType, int32 InQuantity);
	static int32 GetCurrencyHave(UWorld* InWorld, FName InCurrencyType);

	static FText ItemBasicOptionStatValueToText(EItemStatOption ItemStatOption, int32 Value);
	static FText ItemAdditionalOptionStatValueToText(EItemStatOption ItemStatOption, int32 Value);

	/*
	Hero
	*/
	static void MakeEquipedHeroData(FEQUIPED_HERO& EquipedHero, FString& InHeroHashKey, URGameInstance* RGameInstance);
	static int32 GetCurrentHaveRuneCount(UWorld* World, FName InCharacterName);
	static bool CheckSelectCharacterAttackAnim(AActor* InActor);
	static void SetDamageFloaterVisible(UWorld* InWorld, bool InVisible);
	static FString GetCurrentIndoorLobbyHero();
	/*
	InGame Spawn
	*/
	static void FirstSpawnPos(UWorld* World, FVector location, FRotator Rotator = FRotator::ZeroRotator);

	/*
	Actor Finder
	*/
	template<typename T>
	static T* FindActor(UObject* WorldContextObject, TSubclassOf<T> ActorClass, FName name)
	{
		UWorld* InWorld = GEngine->GetWorldFromContextObject(WorldContextObject, EGetWorldErrorMode::Assert);
		if (InWorld == nullptr)
			return nullptr;
		for (TActorIterator<T> ActorItr(InWorld); ActorItr; ++ActorItr)
		{
			T* Actor = *ActorItr;
			if (IsValid(Actor) && Actor->GetFName() == name)
			{
				return Actor;
			}
		}
		return nullptr;
	}
	template<typename T>
	static bool FindActorByTag(UObject* WorldContextObject, FName tag, TArray<T*>& OutActors)
	{
		UWorld* InWorld = GEngine->GetWorldFromContextObject(WorldContextObject, EGetWorldErrorMode::Assert);
		if (InWorld == nullptr)
			return false;

		bool IsExistActor = false;
		for (TActorIterator<T> ActorItr(InWorld); ActorItr; ++ActorItr)
		{
			T* Actor = Cast<T>(*ActorItr);
			if (IsValid(Actor) && Actor->Tags.Contains(tag)) 
			{
				OutActors.Emplace(Actor);
				IsExistActor = true;
			}
		}
	
		if(!IsExistActor)
			return false;

		return true;
	};

	static AActor* FindActor(UWorld* world, FName ActorName);

	static AActor* FindSpawnBox(FName name);
	static AActor* FindWaypoint(FName name);
	static class AWallActor* FindWall(FName name);


	/*
	related matenee
	*/
	static bool PlayMatineeActor(UWorld* InWorld, FString MatineeActorName, FString MatineeCameraName = "", AActor* TargetActor = nullptr);
	static bool PlayNoneSkipMatineeActor(UWorld* InWorld, FString MatineeActorName, FString MatineeCameraName = "", AActor* TargetActor = nullptr);
	static bool StopMatineeActor(UWorld* InWorld, FString MatineeActorName);
	static AMatineeActor* GetPlayingMatineeActor(UObject* WorldContextObject);
	static FString GetPlayingMatineeActorName(UObject* WorldContextObject);
	static bool SkipMatineeActor(UObject* WorldContextObject, FString MatineeActorName);
	static AMatineeActor* GetMatineeActor(UWorld* InWorld, FString MatineeActorName);
	static ACameraActor* GetMatineeCameraActor(UWorld* InWorld, FString MatineeCameraName);

	UFUNCTION(BlueprintCallable, Category = UtilFunction)
	static void DestroyMatineeActorByName(UWorld* InWorld, FString MatineeActorName);
	UFUNCTION(BlueprintCallable, Category = UtilFunction)
	static void DestroyMatineeActorByActor(AMatineeActor* MatineeActor);
	
	static AActor* SetActorVictoryPosition(UObject* WorldContextObject);

	static void ShowColosseumTeamAnimMontageEvent(URGameInstance* InRGameInstance, const FName SectionName, bool PositionRight);
	static void PlayAnimMontageEventFromTeam(TWeakObjectPtr<UParty> TeamData, const FName SectionName);


	/*
	Sequence
	*/
// 	static ALevelSequenceActor* GetSequenceActor(UWorld* InWorld, FString SequenceActorName);
// 	static ALevelSequenceActor* GetFirstFindSequenceActor(UWorld* InWorld);

	static void PauseSequence(URGameInstance* InRGameInstance);


	/*
	Sequence Contents
	*/
	static void ShowVictorySequence(URGameInstance* InRGameInstance);
	static void ShowVictoryAnim(URGameInstance* InRGameInstance);

/*	static AStageBase* GetStageSpawner(URGameInstance* InRGameInstance);*/

	static void BossName(AActor* InBossActor);
	static void RaidBossName(URGameInstance* InRGameInstance, bool Show);

	static void StageClearSequenceEnd(URGameInstance* InRGameInstance);
	

	/*
	Component Helper
	*/
	static void SettingStaticMeshComponent(AActor* InOwner, UStaticMeshComponent* InTarget, FName InStaticMeshTableName = FName(), float InOffsetZ = 0);
	static void OptimizeStaticMeshComponent(UStaticMeshComponent* InTarget);

	/*
	related matenee event
	*/
// 	UFUNCTION(BlueprintCallable, meta = (HidePin = "WorldContextObject", DefaultToSelf = "WorldContextObject"), Category = UtilFunction)
// 	static void CallEventStartStoryMatineeEnd(UObject* WorldContextObject);
// 	UFUNCTION(BlueprintCallable, meta = (HidePin = "WorldContextObject", DefaultToSelf = "WorldContextObject"), Category = UtilFunction)
// 	static void CallEventEndStoryMatineeEnd(UObject* WorldContextObject);
// 	UFUNCTION(BlueprintCallable, meta = (HidePin = "WorldContextObject", DefaultToSelf = "WorldContextObject"), Category = UtilFunction)
// 	static void CallEventMatineeEnd(UObject* WorldContextObject, FName BossSpawnMatinee = FName("") );
// 	UFUNCTION(BlueprintCallable, meta = (HidePin = "WorldContextObject", DefaultToSelf = "WorldContextObject"), Category = UtilFunction)
// 	static void CallSetAllObjectHidden(UObject* WorldContextObject, bool IsHidden = true, AActor* IgnoreActor = nullptr);


	/*
	related Asset
	*/	
	UFUNCTION(BlueprintCallable, Category = UtilFunction)
	static UClass*					GetCharacterClassFromAssetID( TAssetSubclassOf< ABattleBaseCharacter > AssetID);

// 	UFUNCTION(BlueprintCallable, Category = UtilFunction)
// 	static UClass*					GetLobbyActorClassFromAssetID(TAssetSubclassOf< ALobbyActor > AssetID)

	UFUNCTION(BlueprintCallable, Category = UtilFunction)
	static UClass*					GetWeaponBaseFromAssetID( TAssetSubclassOf< AWeaponBase > AssetID);

	UFUNCTION(BlueprintCallable, Category = UtilFunction)
	static UClass*					GetBulletBaseFromAssetID( TAssetSubclassOf< ABulletBase > AssetID);

	UFUNCTION(BlueprintCallable, Category = UtilFunction)
	static UClass*					GetBaseWidgetFromAssetID(TAssetSubclassOf< URBaseWidget > AssetID);	

	UFUNCTION(BlueprintCallable, Category = UtilFunction)
	static UStaticMesh*				GetStaticMeshFromAssetID( TAssetPtr< UStaticMesh > AssetID);

	UFUNCTION(BlueprintCallable, Category = UtilFunction)
	static USkeletalMesh*			GetSkeletalMeshFromAssetID( TAssetPtr< USkeletalMesh > AssetID);

	UFUNCTION(BlueprintCallable, Category = UtilFunction)
	static UMaterial*				GetMaterialFromAssetID(TAssetPtr< UMaterial > AssetID);

	UFUNCTION(BlueprintCallable, Category = UtilFunction)
	static UMaterialInstance*		GetMaterialInstanceFromAssetID(TAssetPtr< UMaterialInstance > AssetID );

	UFUNCTION(BlueprintCallable, Category = UtilFunction)
	static UParticleSystem*			GetParticleSystemFromAssetID(TAssetPtr< UParticleSystem > AssetID);
		
	UFUNCTION(BlueprintCallable, Category = UtilFunction)
	static UTexture2D*				GetTexture2DFromAssetID(TAssetPtr< UTexture2D > AssetID);

	UFUNCTION(BlueprintCallable, Category = UtilFunction)
	static UPaperSprite*			GetSpriteFromAssetID(TAssetPtr< UPaperSprite > AssetID);

	UFUNCTION(BlueprintCallable, Category = UtilFunction)
	static USoundCue*				GetSoundCueFromAssetID(TAssetPtr< USoundCue > AssetID);

	UFUNCTION(BlueprintCallable, Category = UtilFunction)
	static UAnimationAsset*			GetAnimationAssetFromAssetID(TAssetPtr< UAnimationAsset > AssetID);

	UFUNCTION(BlueprintCallable, Category = UtilFunction)
	static UAnimMontage*			GetAnimMontageFromAssetID(TAssetPtr< UAnimMontage > AssetID);

	static UCurveFloat* GetCurveFloat(FName AssetName);
	static UCurveVector* GetCurveVector(FName AssetName);
	static UCurveBase* GetCurveBaseAssetID(TAssetPtr< UCurveBase > AssetID);

	UFUNCTION(BlueprintCallable, Category = UtilFunction)
	static UStaticMesh*				GetStaticMeshFromTable( FName AssetName);

	UFUNCTION(BlueprintCallable, Category = UtilFunction)
	static UMaterialInstance*		GetMaterialInstanceFromTable( FName AssetName);

	UFUNCTION(BlueprintCallable, Category = UtilFunction)
	static UParticleSystem*			GetParticleSystemFromTable( FName AssetName);

	UFUNCTION(BlueprintCallable, Category = UtilFunction)
	static UTexture2D*				GetTexture2DFromTable( FName AssetName);

	UFUNCTION(BlueprintCallable, Category = UtilFunction)
	static UPaperSprite*			GetSpriteFromTable(FName AssetName);

//	UFUNCTION(BlueprintCallable, Category = UtilFunction)
	static FSlateBrush*	GetSpriteToBrush(UPaperSprite* Sprite);

// 	UFUNCTION(BlueprintCallable, Category = UtilFunction)
 	static FSlateBrush*	GetTexture2DToBrush(UTexture2D* Texture2D);

	
	/*
	related Userwidget
	*/
	UFUNCTION(BlueprintCallable, Category = UtilFunction)
	static UUserWidget*						CreateUserWidget(UWorld* InWorld, FName WidgetID);

//	static UHpBarWidgetComponent*			AttachHpBarComp(AActor* InCharacter, FName InWidgetID);

	UFUNCTION(BlueprintCallable, Category = UtilFunction)
	static UWidgetDecoratorComponent*		AttachCharacterWidget(AActor* Character, FName WidgetID, FTransform Pivot, ECharacterCategoryTypeEnum InCharacterCategory);

	UFUNCTION(BlueprintCallable, Category = UtilFunction)
	static USpeechBubbleComponent*			AttachCharacterSpeechBubbleWidget(AActor* Character, FName WidgetID, FTransform Pivot);

// 	UFUNCTION(BlueprintCallable, Category = UtilFunction)
// 	static UCustomWidgetComponent*			AttachWidgetComponent(AActor* Character, FName WidgetID, FTransform Pivot);

	UFUNCTION(BlueprintCallable, Category = UtilFunction)
	static void								ChangeZOrder(AActor* InCharacter);


	/*
	Camera
	*/
	UFUNCTION(BlueprintCallable, meta = (HidePin = "WorldContextObject", DefaultToSelf = "WorldContextObject"), Category = UtilFunction)
	static ACharacterSetCameraActor* 	GetCharacterSetCamera(UObject* WorldContextObject);
	UFUNCTION(BlueprintCallable, meta = (HidePin = "WorldContextObject", DefaultToSelf = "WorldContextObject"), Category = UtilFunction)
	static ALobbyCameraActor* 			GetLobbyCameraActor(UObject* WorldContextObject);
	UFUNCTION(BlueprintCallable, meta = (HidePin = "WorldContextObject", DefaultToSelf = "WorldContextObject"), Category = UtilFunction)
	static bool							ViewTargetBaseCamera(UObject* WorldContextObject);

	//Getting Current active camera component
	static UCameraComponent* GetActiveCameraComponent(UWorld* InWorld, int32 InPlayerIdx = 0);
	static void SetLobbyViewTarget_PCM();
	static void SetIndoorViewTarget_PCM();
	static void SetTowerBossLobby();
	//temp
	static void SetTowerBossCameraLocation(FName BossName);
	static void SetGachaSHopViewTarget_PCM(float InBlendTime = 0.f);


	/*
	Hud
	*/

// 	UFUNCTION(BlueprintCallable, meta = (HidePin = "WorldContextObject", DefaultToSelf = "WorldContextObject"), Category = UtilFunction)
// 	static bool	IsMyTeamNumberFirst(UObject* WorldContextObject);


	/*
	Cheat
	*/
	UFUNCTION(BlueprintCallable, Category = UtilFunction)
	static bool					IsCheatCoolTimeZero(UWorld* InWorld);

// 	UFUNCTION(BlueprintCallable, Category = UtilFunction)
// 	static bool					IsCheatSkillCostZero(UWorld* InWorld);

	UFUNCTION(BlueprintCallable, meta = (HidePin = "WorldContextObject", DefaultToSelf = "WorldContextObject"), Category = UtilFunction)
	static void					CheatGlobalTimeDilation(UObject* WorldContextObject, float GlobalTime);

	UFUNCTION(BlueprintCallable, Category = UtilFunction)
	static bool					IsCheatVisibleInGameHPBar();


	/*
	*/
	static FString GetBlueprintPath(FString path);
	static AActor* SpawnPath(UWorld* InWorld, FTransform transform, FString path);
	static FName GetActorFileName(AActor* InActor);

	static bool IsRareItem(EItemGradeEnum eItemGradeEnum);
	/*
	Tutorial
	*/
	static bool IsCustomGame(UWorld* InWorld);
	static bool isTutorialGameMode();
	static class ATutorialLevelScriptActor* GetTutorialLevelScriptActor();
//	static int32 GetObjReferenceCount(UObject* Obj, TArray<UObject*>* OutReferredToObjects = nullptr, bool InIsLog = false);

	/*
	Touch
	*/
	UFUNCTION(BlueprintCallable, meta = (HidePin = "WorldContextObject", DefaultToSelf = "WorldContextObject"), Category = UtilFunction)
	static int32 	GetTouchCount(UObject* WorldContextObject);

// 	UFUNCTION(BlueprintCallable, meta = (HidePin = "WorldContextObject", DefaultToSelf = "WorldContextObject"), Category = UtilFunction)
// 	static void 	ShowViewportScale(UObject* WorldContextObject);

	static bool ComputeSynergy(TArray< TWeakObjectPtr< AActor > >& outEnemyList, AActor* InActor, FName InSkillTableID);
	static bool CheckCampaignStorySkip(URGameInstance* InRGameInstance);

	//static EItemGenerateOptionType GetItemOptionType(FString InValue);

	static FText	GetBossName(AActor* InBossActor);

	static bool IsNightingWithRealTime();
public:

	static void UpdateGoodsInfo(URGameInstance* InRGameInstance,FUPDATE_PACKET_SLIM& InUpdate, bool bCheckHeroUpdate = true);
//	static void UpdateGoodsInfo(URGameInstance* InRGameInstance, FUPDATE_PACKET& InUpdate);


// 	template<class T>
// 	static T* LoadObjectPath(const FString path)
// 	{
// 		FStringAssetReference strRef = path;
// 
// 		UObject* obj = strRef.ResolveObject();
// 		if (!IsValid(obj))
// 		{
// 			obj = strRef.TryLoad();
// 		}
// 
// 		return Cast<T>(obj);
// 	}

	template<typename T>
	static FName ConvertNumToFName(T& Number)
	{
		FString NumString;

		if (ARE_TYPES_EQUAL(T, int32))
		{
			NumString = FString::FromInt(Number);
		}
		else if (ARE_TYPES_EQUAL(T, float))
		{
			NumString = FString::SanitizeFloat(Number);
		}

		return FName(*NumString);
	}

	template<typename T>
	static FName ConvertNumToFName(T&& Number)
	{
		FString NumString;

		if (ARE_TYPES_EQUAL(T, int32))
		{
			NumString = FString::FromInt(Number);
		}
		else if (ARE_TYPES_EQUAL(T, float))
		{
			NumString = FString::SanitizeFloat(Number);
		}

		return FName(*NumString);
	}

	template<typename T>
	static T ConvertFNameToNum(FName& Name)
	{
		T retValue;
		FString NumString = Name.ToString();

		if (ARE_TYPES_EQUAL(T, int32))
		{
			retValue = FCString::Atoi(*NumString);
		}
		else if (ARE_TYPES_EQUAL(T, float))
		{
			retValue = FCString::Atof(*NumString);
		}

		return retValue;
	}
	
	template <typename EnumType>
	static FORCEINLINE EnumType GetEnumValueFromString(const FString& EnumName, const FString& String)
	{
		UEnum* Enum = FindObject<UEnum>(ANY_PACKAGE, *EnumName, true);
		if (!Enum)
		{
			return EnumType(0);
		}
		return (EnumType) Enum->GetIndexByName(FName(*String), EGetByNameFlags::None);
	}
		

};
