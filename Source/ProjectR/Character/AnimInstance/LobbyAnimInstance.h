// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Animation/AnimInstance.h"
#include "SharedConstants/GlobalConstants.h"
#include "LobbyAnimInstance.generated.h"


/**
 * 
 */
UCLASS()
class PROJECTR_API ULobbyAnimInstance : public UAnimInstance
{
	GENERATED_UCLASS_BODY()
	
public:

	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = ULobbyAnimInstance)
	TAssetPtr< UAnimMontage >		AnimMontageLobby;

	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = ULobbyAnimInstance)
	TAssetPtr< UAnimMontage >		AnimMontageEvent;

	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = ULobbyAnimInstance)
	TAssetPtr< UAnimMontage >		AnimMontageHeroShop;

public:
	/*
	Override functions
	*/
	void PostLoad() override;
	void NativeInitializeAnimation() override;
	void NativeUpdateAnimation(float DeltaTimeX) override;
	void NativeUninitializeAnimation() override;

	bool PlayAnimMontageLobbySection(const FName& SectionName, bool ForcePlay = false);
	bool PlayAnimMontageEventSection(const FName& SectionName, bool ForcePlay = false);
	bool PlayAnimMontageHeroShopSection(const FName& SectionName, bool ForcePlay = false);
};
