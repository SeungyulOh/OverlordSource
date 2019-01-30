// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/RBaseWidget.h"
#include "RSpiritIcon.generated.h"


USTRUCT(BlueprintType)
struct PROJECTR_API FVariables_SpiritIcon
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Variables")
	UWidgetSwitcher* WidgetSwitcher_State;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Variables")
	UImage*		Image_SkillIcon;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Variables")
	UImage*		Image_Portrait;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Variables")
	UOverlay*	PortraitOverlay;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Variables")
	UButton*	Button_Click;
	

	ECharacterRoleEnum OwnerRole;
};


USTRUCT(BlueprintType)
struct PROJECTR_API FRenderer_SpiritIcon
{
	GENERATED_USTRUCT_BODY()

	FVariables_SpiritIcon* variables;
	class URSpiritIcon* ParentWidget;

	void Prepare(struct FHeroSlotInfo slotinfo);
	void Render();
};

/**
 * 
 */
UCLASS()
class PROJECTR_API URSpiritIcon : public URBaseWidget
{
	GENERATED_BODY()

public:
	void NativeConstruct();
	void NativeDestruct();

public:
	UFUNCTION()
	void OnClickIcon();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "USkillIcon_New")
	FVariables_SpiritIcon	Variables;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "USkillIcon_New")
	FRenderer_SpiritIcon	Renderer;



		
	
	
};
