// Fill out your copyright notice in the Description page of Project Settings.

#include "ProjectR.h"
#include "CharacterUserWidget.h"




void UCharacterUserWidget::SetOwner(AActor* TargetActor)
{
	OwnerCharacter = TargetActor;

	OnSetOwner();
}

void UCharacterUserWidget::RefreshCharacterUserWidget()
{

}

