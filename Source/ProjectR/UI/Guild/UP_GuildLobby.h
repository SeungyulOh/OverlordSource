// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/RBaseWidget.h"
#include "UP_GuildLobby.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTR_API UUP_GuildLobby : public URBaseWidget
{
	GENERATED_BODY()
	
public:
	void NativeConstruct() override;
	void NativeDestruct() override;
	
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UUP_GuildLobby")
	UWidgetSwitcher*	Switcher;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UUP_GuildLobby")
		class UUP_Guild_Join* Guild_Join;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UUP_GuildLobby")
		class UUP_Guild_Management* Guild_management;
	
};
