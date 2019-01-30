// Fill out your copyright notice in the Description page of Project Settings.

#include "ProjectR.h"
#include "RBaseDeckWidget.h"
#include "UP_GenericScrollview.h"
#include "Utils/UIFunctionLibrary.h"
#include "UI/Common/ScrollView/UP_HeroScrollView_Bottom.h"



void URBaseDeckWidget::OnClick_Deck()
{
	UUP_GenericScrollview* GenericScrollview = UUIFunctionLibrary::GetGenericScrollview();
	if (GenericScrollview)
	{
		if (GenericScrollview->Blackboard.TargetWidgetArray.Find(this) == INDEX_NONE)
			return;

		GenericScrollview->Blackboard.SetData_OnBlackBoardData(this);
	}
		
}
