// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "SharedConstants/GlobalConstants.h"
#include "LoginGameMode.generated.h"

class UAuthStateMachine;
class UMediaPlayer;
class UOnlineIdentityLoginUI;
class UUP_Popup;
/**
 */
UCLASS()
class PROJECTR_API ALoginGameMode : public AGameModeBase
{
	GENERATED_BODY()
	
public:
	void BeginPlay() override;
	void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

	// Called by level BP when config is finished
	UFUNCTION(BlueprintCallable, Category = ALoginGameMode)
	void OnSetupComplete();

	UUP_Popup*	ShowExceptionLoginPopup(FText InCaption, FText InContents, 
		ECommonPopupType InPopupType = ECommonPopupType::VE_OK);

	UOnlineIdentityLoginUI* GetLoginWidget();


	
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=Classes, meta = (DisplayName = "MainWidget Class"))
	TSubclassOf<UOnlineIdentityLoginUI> MainWidgetClass;

	// for stopping movie playback when patch UI is up
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = ALoginGameMode)
	UMediaPlayer*	LoginBackground_Movie = nullptr;

private:
	UFUNCTION()
	void LoginMediaOpen(FString InUrl);

private:
	UPROPERTY()
	UAuthStateMachine* AuthStateMachine = nullptr;
};
