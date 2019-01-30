// Fill out your copyright notice in the Description page of Project Settings.
#pragma once

#include "UI/ItemManagement/UC_GenericPopupContent.h"
#include "SharedConstants/GlobalConstants.h"
#include "Utils/UtilFunctionLibrary.h"
#include "Components/TextBlock.h"
#include "UC_Popup_SpiritInfo.generated.h"
class URenewal_BaseHeroIcon;
UCLASS()
class PROJECTR_API UUC_Popup_SpiritInfo : public UUC_GenericPopupContent
{
	GENERATED_BODY()
public:
	void NativeConstruct() override;
	void NativeDestruct() override;

	//// Interface
	void InvalidateData() override;
	UFUNCTION(BlueprintCallable, Category = UUC_Popup_SpiritInfo)
	void	OnClick_Close();
	//// BP Widgets
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UUC_Popup_SpiritInfo)
	UButton*						Button_Close = nullptr;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UUC_Popup_SpiritInfo)
	TArray<ULocalizingTextBlock*>	Descriptions;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UUC_Popup_SpiritInfo)
	URenewal_BaseHeroIcon*			UC_Icon_Hero;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UUC_Popup_SpiritInfo)
	ULocalizingTextBlock*			LocalTextBlock_SkillName;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UUC_Popup_SpiritInfo)
	ULocalizingTextBlock*			LocalTextBlock_SpiritName;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UUC_Popup_SpiritInfo)
	UImage*							SkillIcon;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UUC_Popup_SpiritInfo)
	ULocalizingTextBlock*			LocalTextBlock_Time;
	
	FString							CurSpiritUD;
};
