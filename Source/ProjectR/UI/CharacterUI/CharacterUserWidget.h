// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

//#include "Blueprint/UserWidget.h"
#include "UI/RBaseWidget.h"
#include "SharedConstants/GlobalConstants.h"
#include "CharacterUserWidget.generated.h"

class AActor;
/**
 * 
 */
UCLASS()
class PROJECTR_API UCharacterUserWidget : public URBaseWidget
{
	GENERATED_BODY()
	
public:

	virtual void SetOwner(AActor* TargetActor);
	virtual void RefreshCharacterUserWidget();
	
	UFUNCTION(BlueprintImplementableEvent, Category = CharacterUserWidget)
	void	OnSetOwner();

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = CharacterUserWidget)
	TWeakObjectPtr< AActor >		OwnerCharacter;
	
	
};
