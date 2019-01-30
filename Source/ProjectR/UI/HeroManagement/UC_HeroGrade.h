// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "UI/RBaseWidget.h"
#include "UC_HeroGrade.generated.h"

class UImage;

/**
 * Stars
 * �����ε� UI ����.pdf ���� - �⺻����
 */
UCLASS()
class PROJECTR_API UUC_HeroGrade : public URBaseWidget
{
	GENERATED_BODY()
	
public:

	//// Interface

	void Refresh(int32 Grade);

	//// BP Widgets

	// stars
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UUC_HeroGrade)
	UImage* Image_0;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UUC_HeroGrade)
	UImage* Image_1;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UUC_HeroGrade)
	UImage* Image_2;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UUC_HeroGrade)
	UImage* Image_3;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UUC_HeroGrade)
	UImage* Image_4;
	
};
