// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "UI/RBaseWidget.h"
#include "UI/UIDefs.h"
// #include "UI/UI_Battle/RHeroIcon_ForInGame.h"
#include "UC_HUD_Icon_Hero_Arena.generated.h"


class AActor;
class URBuffIcon;
class URSkillIcon;
class UISkill_V2;
class UTexture2D;
class URHeroUIforPvp;
/**
 * 
 */
UCLASS()
class PROJECTR_API UUC_HUD_Icon_Hero_Arena : public URBaseWidget
{
	GENERATED_BODY()
	
public:

	void NativeConstruct() override;
	void NativeDestruct() override;
	void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;

	void Init(AActor* hero_actor, bool my_hero);

	UFUNCTION()
	void InitHero();

	UFUNCTION(BlueprintCallable, Category = UUC_HUD_Icon_Hero_Arena)
	bool IsMyHeroIcon() { return MyHero; }

	UFUNCTION(BlueprintCallable, Category = UUC_HUD_Icon_Hero_Arena)
	void SetImageHeroPortrait(UImage* img_wdg);

	UFUNCTION(BlueprintCallable, Category = UUC_HUD_Icon_Hero_Arena)
	void SetImageHeroClass(UImage* img_wdg);

	UFUNCTION(BlueprintCallable, Category = UUC_HUD_Icon_Hero_Arena)
	void SetTextkHeroName(UTextBlock* textblock_wdg);

	UFUNCTION(BlueprintCallable, Category = UUC_HUD_Icon_Hero_Arena)
	void SetAbnormalIcon(TArray<URBuffIcon*> icon_array);

	UFUNCTION(BlueprintCallable, Category = UUC_HUD_Icon_Hero_Arena)
	void SetProgressHP(UProgressBar* hp_prog);

	UFUNCTION(BlueprintCallable, Category = UUC_HUD_Icon_Hero_Arena)
	void SetSkillIcon(URSkillIcon* skill_icon);

	UFUNCTION(BlueprintCallable, Category = UUC_HUD_Icon_Hero_Arena)
	void SetLevelText(UTextBlock* text_wdg);

	UFUNCTION()
	void SetTeamColor();

	UFUNCTION()
	void SetCameraFocusOnSelectHero();


	void UpdateHeroHP();

// 	UFUNCTION()
// 	void OnDeadActor(AActor* DeadActor, AActor* FromActor);

	UFUNCTION(BlueprintCallable, Category = UUC_HUD_Icon_Hero_Arena)
	void SetCurrentSkillIcon(AActor* InCaster, UISkill_V2* UseSkill);

	UFUNCTION(BlueprintCallable, Category = UUC_HUD_Icon_Hero_Arena)
	void CheckAbnormal(EAbnormalSlotTypeEnum InSlotType, bool InSet);

	void HideSkillIcon();

	void PlayChildIconAni(FString name, bool bReverse  = false );
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UUC_HUD_Icon_Hero_Arena)
	URHeroUIforPvp*			HeroUIforPvp;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UUC_HUD_Icon_Hero_Arena)
	UImage*					Icon_Class;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UUC_HUD_Icon_Hero_Arena)
	UImage*					Image_Death;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UUC_HUD_Icon_Hero_Arena)
	UTextBlock*				Text_UserName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UUC_HUD_Icon_Hero_Arena)
	TArray<URBuffIcon*>		BuffIconArray;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UUC_HUD_Icon_Hero_Arena)
	URSkillIcon*			SkillIcon;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UUC_HUD_Icon_Hero_Arena)
	UProgressBar*			ProgressBar_HP;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UUC_HUD_Icon_Hero_Arena)
	UButton*				PortraitButton;


private:
	UPROPERTY()
	TWeakObjectPtr<AActor>	HeroActor;

	UPROPERTY()
	FName					HeroID;

	bool					MyHero = false;

	UPROPERTY()
	UObject*				HeroPortrait = nullptr;

	UPROPERTY()
	TArray<URBuffIcon*>		AbnormalIconList;

	UPROPERTY()
	URSkillIcon*			UseSkillIcon;

	UPROPERTY()
	TArray<TWeakObjectPtr<UISkill_V2>> OwnSkillList;

	UPROPERTY()
	FTimerHandle			SkillIconTimerHandler;

	int32					HeroLevel = 1;
	int32					HeroMaxHP = 100;
	int32					HeroCurrentHP = 0;
};
