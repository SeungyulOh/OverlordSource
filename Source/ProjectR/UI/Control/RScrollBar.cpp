// Fill out your copyright notice in the Description page of Project Settings.

#include "ProjectR.h"
#include "Blueprint/UserWidget.h"
#include "RScrollBar.h"

URScrollBar::URScrollBar(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{

}

void URScrollBar::SynchronizeProperties()
{
	Super::SynchronizeProperties();
}

void URScrollBar::ReleaseSlateResources(bool bReleaseChildren)
{
	Super::ReleaseSlateResources(bReleaseChildren);
}

void URScrollBar::PostLoad()
{
	Super::PostLoad();
}

/*
// Make a scroll bar 
ScrollBar = ConstructScrollBar();
ScrollBar->SetThickness(InArgs._ScrollBarThickness);
ScrollBar->SetUserVisibility(InArgs._ScrollBarVisibility);
ScrollBar->SetScrollBarAlwaysVisible(InArgs._ScrollBarAlwaysVisible);

bScrollBarIsExternal = false;
*/