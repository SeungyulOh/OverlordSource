// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "UI/RBaseWidget.h"
#include "SharedConstants/GlobalConstants.h"
#include "UP_EditorPlayTestInfo.generated.h"

class UISkill_V2;
/**
 * 
 */

UCLASS()
class PROJECTR_API UUP_EditorPlayTestInfo : public URBaseWidget
{
	GENERATED_BODY()
	
public:
	void NativeConstruct() override;
	void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;

	void SetActor(AActor* InActor);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UUP_EditorPlayTestInfo)
	UButton*				Skill1Btn = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UUP_EditorPlayTestInfo)
	UButton*				Skill2Btn = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UUP_EditorPlayTestInfo)
	UButton*				Skill3Btn = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UUP_EditorPlayTestInfo)
	UButton*				Skill4Btn = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UUP_EditorPlayTestInfo)
	UButton*				Skill5Btn = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UUP_EditorPlayTestInfo)
	UTextBlock*				Skill1CoolTimeText = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UUP_EditorPlayTestInfo)
	UTextBlock*				Skill2CoolTimeText = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UUP_EditorPlayTestInfo)
	UTextBlock*				Skill3CoolTimeText = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UUP_EditorPlayTestInfo)
	UTextBlock*				Skill4CoolTimeText = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UUP_EditorPlayTestInfo)
	UTextBlock*				Skill5CoolTimeText = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UUP_EditorPlayTestInfo)
	UCheckBox*				AICheckBox = nullptr;

	UFUNCTION(BlueprintCallable, Category = UUP_EditorPlayTestInfo)
	void					Skill1Exe();

	UFUNCTION(BlueprintCallable, Category = UUP_EditorPlayTestInfo)
	void					Skill2Exe();

	UFUNCTION(BlueprintCallable, Category = UUP_EditorPlayTestInfo)
	void					Skill3Exe();

	UFUNCTION(BlueprintCallable, Category = UUP_EditorPlayTestInfo)
	void					Skill4Exe();

	UFUNCTION(BlueprintCallable, Category = UUP_EditorPlayTestInfo)
	void					Skill5Exe();

	UFUNCTION(BlueprintCallable, Category = UUP_EditorPlayTestInfo)
	void					AICheckBoxExe(bool bActive);

private:
	TWeakObjectPtr<AActor>	OwnerActor = nullptr;

	UISkill_V2*				ActiveSkill1 = nullptr;
	UISkill_V2*				ActiveSkill2 = nullptr;
	UISkill_V2*				ActiveSkill3 = nullptr;
	UISkill_V2*				PassiveSkill1 = nullptr;
	UISkill_V2*				PassiveSkill2 = nullptr;
};
