// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "UI/RBaseWidget.h"
#include "UI/UIDefs.h"

#include "UC_Popup_HeroInfo_LevelUp.generated.h"


UCLASS()
class PROJECTR_API UUC_Popup_HeroInfo_LevelUp : public URBaseWidget
{
	GENERATED_BODY()
//	�ɹ� �Լ�
public:
	void NativeConstruct() override;
	void NativeDestruct() override;
	void Init();
protected:
private:

//	�ɹ� ����
public:
	UPROPERTY()
	UButton*			Button_OK		=	nullptr;
	UPROPERTY()
	UButton*			Button_Cancel	=	nullptr;
protected:
private:
};