// Fill out your copyright notice in the Description page of Project Settings.

#include "ProjectR.h"
#include "RHudStar.h"




void URHudStar::NativeConstruct()
{
	Super::NativeConstruct();

	/*if (IsValid(Image_1))
		Image_1->SetVisibility(ESlateVisibility::Collapsed);

	if (IsValid(Image_2))
		Image_2->SetVisibility(ESlateVisibility::Collapsed);

	if (IsValid(Image_3))
		Image_3->SetVisibility(ESlateVisibility::Collapsed);*/
}

void URHudStar::NativeDestruct()
{
	Super::NativeDestruct();


}

void URHudStar::UpdateImage(EPlayStageStarEnum InStarCnt)
{
	if (StarCnt == InStarCnt)
		return;

	StarCnt = InStarCnt;

	switch (InStarCnt)
	{
	case EPlayStageStarEnum::VE_Star1:
		if (IsValid(Image_1))
			Image_1->SetVisibility(ESlateVisibility::Visible);

		if (IsValid(Image_2))
			Image_2->SetVisibility(ESlateVisibility::Collapsed);

		if (IsValid(Image_3))
			Image_3->SetVisibility(ESlateVisibility::Collapsed);
		break;
	case EPlayStageStarEnum::VE_Star2:
		if (IsValid(Image_1))
			Image_1->SetVisibility(ESlateVisibility::Visible);

		if (IsValid(Image_2))
			Image_2->SetVisibility(ESlateVisibility::Visible);

		if (IsValid(Image_3))
			Image_3->SetVisibility(ESlateVisibility::Collapsed);
		break;
	case EPlayStageStarEnum::VE_Star3:
		if (IsValid(Image_1))
			Image_1->SetVisibility(ESlateVisibility::Visible);

		if (IsValid(Image_2))
			Image_2->SetVisibility(ESlateVisibility::Visible);

		if (IsValid(Image_3))
			Image_3->SetVisibility(ESlateVisibility::Visible);
		break;
	default:
		if (IsValid(Image_1))
			Image_1->SetVisibility(ESlateVisibility::Collapsed);

		if (IsValid(Image_2))
			Image_2->SetVisibility(ESlateVisibility::Collapsed);

		if (IsValid(Image_3))
			Image_3->SetVisibility(ESlateVisibility::Collapsed);
		break;

	}
}

void URHudStar::UpdateStar()
{
	switch(StarNum)
	{
		case 1:
			Image_2->SetVisibility(ESlateVisibility::Collapsed);
		break;
		case 2:
			Image_3->SetVisibility(ESlateVisibility::Collapsed);
		break;
	}

	--StarNum;
}
