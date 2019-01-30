// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/RBaseWidget.h"
#include "UC_FriendHUD.generated.h"


USTRUCT(BlueprintType)
struct PROJECTR_API FVariables_FriendHUD
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY(EditAnywhere , BlueprintReadWrite , Category = "Variables")
	UImage*		Image_Hero;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Variables")
	UOverlay*	HeroDeathOverlay;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Variables")
	UProgressBar*	ProgressBar_HP;
// 	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Variables")
// 	UTextBlock*	Text_HP;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Variables")
	UWidgetAnimation* DangerAnim;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Variables")
	UCanvasPanel* CanvasPanel;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Variables")
	int32	ProtectIdx;
};

USTRUCT(BlueprintType)
struct PROJECTR_API FRenderer_FriendHUD
{
	GENERATED_USTRUCT_BODY()

	void Prepare();
	void Render(AActor* TargetActor);

public:
	FVariables_FriendHUD* variables;
	
};

UCLASS()
class PROJECTR_API UUC_FriendHUD : public URBaseWidget
{
	GENERATED_BODY()
	
public:
	void NativeConstruct();
	void NativeDestruct();
	void NativeTick(const FGeometry& MyGeometry, float InDeltaTime);

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UUC_FriendHUD | Variables")
	FVariables_FriendHUD variables;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UUC_FriendHUD | Renderer")
	FRenderer_FriendHUD Renderer;


	/*BaseStageInfo*/
	UPROPERTY()
	TWeakObjectPtr<class ALevelActor> LevelActor;
	
	
	
};
