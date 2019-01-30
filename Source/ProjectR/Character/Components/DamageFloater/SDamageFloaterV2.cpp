// Copyright 1998-2016 Epic Games, Inc. All Rights Reserved.
//#if WITH_KOKOMO

#include "ProjectR.h"
#include "SDamageFloaterV2.h"
#include "PaperSprite.h"
#include "SBoxPanel.h"
#include "Algo/Reverse.h"
#include "ContentGameMode/Controllers/BasePlayerController.h"
#include "UtilFunctionIntegrated.h"
#include "RHI.h"

#include "Blueprint/WidgetLayoutLibrary.h"
#include "Runtime/Engine/Classes/Engine/UserInterfaceSettings.h"

static bool					InitDamageCurve = true;
static UCurveFloat*			DamageFadeCurve = nullptr;
static UCurveFloat*			DamageScaleCurve = nullptr;
static UCurveVector*		DamageOffsetCurve = nullptr;
static UCurveFloat*			DamageCriticalFadeCurve = nullptr;
static UCurveFloat*			DamageCriticalScaleCurve = nullptr;
static UCurveVector*		DamageCriticalOffsetCurve = nullptr;
static UCurveFloat*			DamageHealFadeCurve = nullptr;
static UCurveFloat*			DamageHealScaleCurve = nullptr;
static UCurveVector*		DamageHealOffsetCurve = nullptr;
/**
* Construct this widget
*
* @param	InArgs	The declaration data for this widget
*/

SDamageFloaterV2::FDamageFloater::FDamageFloater(EBattleDamageResultEnum damageType, int32 damage, int32 addpercent, FVector basepos, float padding, float life, float speed, float inScale, bool inShowText, FLinearColor color, SDamageFloaterV2 & canvas)
{
	RefCanvas = &canvas;
	DamageType = damageType;
	
	Damage = FMath::Abs(damage);
	Percent = addpercent;

	Positive = damage >= 0.0f;
	Color = color;
	BasePos = basepos;
	CurrentPos = BasePos;
	LifeSeconds = life;
	CurrentLife = 0.0f;
	FloatingSpeed = speed;
	Padding = padding;
	Scale = inScale;
	ShowText = inShowText;
	MakeNumericArray();
	MakeNumericPanel(canvas);
	//SetVisibility(EVisibility::HitTestInvisible);
	if (InitDamageCurve)
	{
		DamageFadeCurve = UUtilFunctionLibrary::GetCurveFloat("DamageFade");
		DamageScaleCurve = UUtilFunctionLibrary::GetCurveFloat("DamageScale");
		DamageOffsetCurve = UUtilFunctionLibrary::GetCurveVector("DamageOffset");
		DamageCriticalFadeCurve = UUtilFunctionLibrary::GetCurveFloat("DamageCriticalFade");
		DamageCriticalScaleCurve = UUtilFunctionLibrary::GetCurveFloat("DamageCriticalScale");
		DamageCriticalOffsetCurve = UUtilFunctionLibrary::GetCurveVector("DamageCriticalOffset");
		DamageHealFadeCurve = UUtilFunctionLibrary::GetCurveFloat("DamageHealFade");
		DamageHealScaleCurve = UUtilFunctionLibrary::GetCurveFloat("DamageHealScale");
		DamageHealOffsetCurve = UUtilFunctionLibrary::GetCurveVector("DamageHealOffset");

		DamageFadeCurve->AddToRoot();
		DamageScaleCurve->AddToRoot();
		DamageOffsetCurve->AddToRoot();
		DamageCriticalFadeCurve->AddToRoot();
		DamageCriticalScaleCurve->AddToRoot();
		DamageCriticalOffsetCurve->AddToRoot();
		DamageHealFadeCurve->AddToRoot();
		DamageHealScaleCurve->AddToRoot();
		DamageHealOffsetCurve->AddToRoot();
		InitDamageCurve = false;
	}
}

SDamageFloaterV2::FDamageFloater::~FDamageFloater()
{
	DamageDigits.Empty();
	PercentDigits.Empty();
	DigitSizes.Empty();
	DigitEtcSizes.Empty();
	DamageImages.Empty();
	DamageImageEtcs.Empty();
	Slots.Empty();
	SlotEtcs.Empty();
	SlotPercent.Empty();
	RefCanvas = nullptr;
	
}

void SDamageFloaterV2::CleanStatic()
{
	if (DamageFadeCurve == nullptr)
		return;

	DamageFadeCurve->RemoveFromRoot();
	DamageScaleCurve->RemoveFromRoot();
	DamageOffsetCurve->RemoveFromRoot();
	DamageCriticalFadeCurve->RemoveFromRoot();
	DamageCriticalScaleCurve->RemoveFromRoot();
	DamageCriticalOffsetCurve->RemoveFromRoot();
	DamageHealFadeCurve->RemoveFromRoot();
	DamageHealScaleCurve->RemoveFromRoot();
	DamageHealOffsetCurve->RemoveFromRoot();

	DamageFadeCurve = nullptr;
	DamageScaleCurve = nullptr;
	DamageOffsetCurve = nullptr;
	DamageCriticalFadeCurve = nullptr;
	DamageCriticalScaleCurve = nullptr;
	DamageCriticalOffsetCurve = nullptr;
	DamageHealFadeCurve = nullptr;
	DamageHealScaleCurve = nullptr;
	DamageHealOffsetCurve = nullptr;
	InitDamageCurve = true;
}

float SDamageFloaterV2::FDamageFloater::GetWidth()
{
	float result = 0.0f;
	int32 NumDigitSizes = DigitSizes.Num();
	for (int32 i = 0; i < NumDigitSizes; ++i)
	{
		result += DigitSizes[i].X;
	}
	return result;
}

float SDamageFloaterV2::FDamageFloater::GetWidth(float scale)
{
	float result = 0.0f;
	float fComputeScale = scale * ((RefCanvas != nullptr) ? RefCanvas->SpacingScale : 1.0f);
	int32 NumDigitSizes = DigitSizes.Num();
	for (int32 i = 0; i < NumDigitSizes; ++i)
	{
		result += (DigitSizes[i].X + Padding) * fComputeScale;
	}
	return result;
}

float SDamageFloaterV2::FDamageFloater::GetRemainedLifeSeconds()
{
	return (LifeSeconds - CurrentLife > 0.0f) ? (LifeSeconds - CurrentLife) : 0.0f;
}

void SDamageFloaterV2::FDamageFloater::Update(float delta, float InScale, ABasePlayerController* playerController)
{
	CurrentLife += delta;
	//CurrentPos.Y -= FloatingSpeed * scale * delta;
	if (RefCanvas)
	{
		float fTimeRate = CurrentLife / LifeSeconds;
		if (fTimeRate > 1.0f)
			fTimeRate = 1.0f;

		FVector offset = FVector::ZeroVector;
		float fFade = 1.0f;
		float fScale = 1.0f;
		
		//if (playerController)
		{
			switch (DamageType)
			{
			case EBattleDamageResultEnum::VE_Critical:
			case EBattleDamageResultEnum::VE_ReceiveCritical:
			case EBattleDamageResultEnum::VE_JustInput:
// 			case EBattleDamageResultEnum::VE_MagicCritical:
// 			case EBattleDamageResultEnum::VE_ReceiveMagicCritical:
			{
				if (DamageCriticalOffsetCurve)
				{
					offset = DamageCriticalOffsetCurve->GetVectorValue(fTimeRate);
				}

				if (DamageCriticalFadeCurve)
				{
					fFade = DamageCriticalFadeCurve->GetFloatValue(fTimeRate);
					Color.A = fFade;
				}

				if (DamageCriticalScaleCurve)
				{
					fScale = DamageCriticalScaleCurve->GetFloatValue(fTimeRate);
				}
			}
			break;
			case EBattleDamageResultEnum::VE_Heal:
			{
				if (DamageHealOffsetCurve)
				{
					offset = DamageHealOffsetCurve->GetVectorValue(fTimeRate);
				}

				if (DamageHealFadeCurve)
				{
					fFade = DamageHealFadeCurve->GetFloatValue(fTimeRate);
					Color.A = fFade;
				}

				if (DamageHealScaleCurve)
				{
					fScale = DamageHealScaleCurve->GetFloatValue(fTimeRate);
				}
			}
			break;
			default:
			{
				if (DamageOffsetCurve)
				{
					offset = DamageOffsetCurve->GetVectorValue(fTimeRate);
				}

				if (DamageFadeCurve)
				{
					fFade = DamageFadeCurve->GetFloatValue(fTimeRate);
					Color.A = fFade;
				}

				if (DamageScaleCurve)
				{
					fScale = DamageScaleCurve->GetFloatValue(fTimeRate);
				}
			}
			break;
			}
		}

		float fPreScale = fScale * Scale;
		FVector2D newpos = RefCanvas->GetScreenPos(CurrentPos);
		float fWidth = GetWidth(fPreScale * InScale);
		newpos.X = newpos.X - (fWidth * 0.5f) + offset.X;
		newpos.Y -= offset.Y * Scale;

		FVector2D newpos1 = newpos;
		
		float fComputeScale = fPreScale * InScale * ((RefCanvas != nullptr) ? RefCanvas->SpacingScale : 1.0f);
		float fComputePercentScale = fComputeScale * 0.5f;
		if (DigitSizes.Num() > 0)
		{
			float fHeight = DigitSizes[0].Y * fPreScale;

			int32 NumSlots = Slots.Num();
			if (DigitSizes.Num() == NumSlots)
			{
				for (int32 i = 0; i < NumSlots; ++i)
				{
					Slots[i]->PositionAttr.Set(newpos1);

					newpos1.X += (DigitSizes[i].X + Padding) * fComputeScale;

					Slots[i]->SizeAttr.Set(DigitSizes[i] * fPreScale);

					if (DamageImages.IsValidIndex(i))
					{
						DamageImages[i]->SetColorAndOpacity(Color);
					}
				}
			}

			if (PercentSizes.Num() > 0)
			{
				int32 NumSlotPercent = SlotPercent.Num();
				if (PercentSizes.Num() == NumSlotPercent)
				{
					FVector2D startPos = newpos1;

					float fPercentScale = fPreScale * 0.5f;
					float fDiffHeight = fHeight - PercentSizes[0].Y * fPercentScale;
					if (fDiffHeight > 0.0f)
					{
						startPos.Y += fDiffHeight;
					}
					
					for (int32 i = 0; i < NumSlotPercent; ++i)
					{
						SlotPercent[i]->PositionAttr.Set(startPos);

						startPos.X += (PercentSizes[i].X + Padding) * fComputePercentScale;

						SlotPercent[i]->SizeAttr.Set(PercentSizes[i] * fPercentScale);

						if (PercentImages.IsValidIndex(i))
						{
							PercentImages[i]->SetColorAndOpacity(Color);
						}
					}
				}
			}
		}


		float fTotal = (newpos1.X - newpos.X);
		float fComputeTotal = newpos.X + (fTotal * 0.5f);
		int32 NumSlotEtcs = SlotEtcs.Num();
		for (int32 j = 0; j < NumSlotEtcs; ++j)
		{
			FVector2D newPos2 = newpos;
			newPos2.X = fComputeTotal - (DigitEtcSizes[j].X * fComputePercentScale);

			switch (DamageType)
			{
				case EBattleDamageResultEnum::VE_Miss:
				case EBattleDamageResultEnum::VE_Dodge:
				case EBattleDamageResultEnum::VE_ReceiveDodge:
					break;
				case EBattleDamageResultEnum::VE_Heal:
					newPos2.X = newpos.X - (DigitEtcSizes[j].X + Padding) * fComputeScale;
					break;
				default:
					newPos2.Y -= 50.0f;
					break;
			}
			
			SlotEtcs[j]->PositionAttr.Set(newPos2);

			SlotEtcs[j]->SizeAttr.Set(DigitEtcSizes[j] * fPreScale);

			if (DamageImageEtcs.IsValidIndex(j))
			{
				DamageImageEtcs[j]->SetColorAndOpacity(Color);
			}
		}
	}
}

void SDamageFloaterV2::FDamageFloater::MakeNumericArray()
{
	int32 digit = Damage;
	while (true)
	{
		if (digit < 10)
		{
			DamageDigits.Emplace(digit);
			break;
		}
		DamageDigits.Emplace(digit % 10);
		//digit /= 10;
		digit *= 0.1f;
	}
	Algo::Reverse(DamageDigits);

	PercentDigits.Reset();
	if (Percent > 0)
	{
		int32 percent = Percent;
		while (true)
		{
			if (percent < 10)
			{
				PercentDigits.Emplace(percent);
				break;
			}
			PercentDigits.Emplace(percent % 10);
			//percent /= 10;
			percent *= 0.1f;
		}
		Algo::Reverse(PercentDigits);
	}
}

float SDamageFloaterV2::FDamageFloater::MakeNumericPanel(SDamageFloaterV2 & canvas)
{
	bool bDamageShow = true;
	int32 FontColor = SDamageFloaterV2::eFontColor::eWhiteColor;

	switch (DamageType)
	{
	case EBattleDamageResultEnum::VE_Critical:
		FontColor = SDamageFloaterV2::eFontColor::eYellowColor;
		Padding = 0.0f;
		break;
// 	case EBattleDamageResultEnum::VE_MagicCritical:
// 		FontColor = SDamageFloaterV2::eFontColor::ePinkColor;
// 		Padding = 0.0f;
// 		break;
	case EBattleDamageResultEnum::VE_JustInput:
		FontColor = SDamageFloaterV2::eFontColor::ePinkColor;
		Padding = 5.0f;
		break;
	case EBattleDamageResultEnum::VE_ReceiveHit:
	case EBattleDamageResultEnum::VE_ReceiveCritical:
// 	case EBattleDamageResultEnum::VE_ReceiveMagicCritical:
	case EBattleDamageResultEnum::VE_ReceiveImmune:
	case EBattleDamageResultEnum::VE_ReceiveCounter:
	case EBattleDamageResultEnum::VE_ReceiveDead:
// 	case EBattleDamageResultEnum::VE_ReceiveParry:
	case EBattleDamageResultEnum::VE_ReceivePierce:
	case EBattleDamageResultEnum::VE_ReceiveResistantDamage:
		FontColor = SDamageFloaterV2::eFontColor::eRedColor;
		Padding = 0.0f;
		break;
	case EBattleDamageResultEnum::VE_Heal:
		FontColor = SDamageFloaterV2::eFontColor::eGreenColor;
		Padding = 5.0f;
		break;
	case EBattleDamageResultEnum::VE_Miss:
	case EBattleDamageResultEnum::VE_Dodge:
	case EBattleDamageResultEnum::VE_ReceiveDodge:
		bDamageShow = false;
		break;
	case EBattleDamageResultEnum::VE_ResistantDamage:
		FontColor = SDamageFloaterV2::eFontColor::eGrayColor;
		Padding = 0.0f;
		break;
	default:
		FontColor = SDamageFloaterV2::eFontColor::eWhiteColor;
		Padding = 0.0f;
		break;
	}

	float result = 0.0f;
	float percentresult = 0.0f;
	int32 NumDamageDigits = DamageDigits.Num();
	DigitSizes.Reset(NumDamageDigits);
	DigitEtcSizes.Reset();
	PercentSizes.Reset(PercentDigits.Num() + 4);

	FVector2D slotpos = canvas.GetScreenPos(CurrentPos);
	float firstPos = slotpos.X;
	if (bDamageShow)
	{
		float fComputeScale = canvas.ScreenScale * SpacingScale * ((RefCanvas != nullptr) ? RefCanvas->SpacingScale : 1.0f);
		//float totaldigitwidth = GetWidth() * canvas.ScreenScale;
		//CurrentPos.X -= totaldigitwidth / 2.0f;		
		for (int32 i = 0; i < NumDamageDigits; ++i)
		{
			TSharedRef<SImage> image = SNew(SImage)
				.ColorAndOpacity(FSlateColor::UseForeground())
				.Image(canvas.BrushSetMap[FontColor][DamageDigits[i]].Get());

			SCanvas::FSlot& slot = canvas.AddSlot()
				.Size(canvas.NumericSpritesMap[FontColor][DamageDigits[i]]->GetSpriteSize())
				.HAlign(HAlign_Left)
				.VAlign(VAlign_Top)
				.Position(slotpos)[image];

			DamageImages.Emplace(image);
			Slots.Emplace(&slot);
			float digitwidth = canvas.NumericSpritesMap[FontColor][DamageDigits[i]]->GetSpriteSize().X;
			DigitSizes.Emplace(canvas.NumericSpritesMap[FontColor][DamageDigits[i]]->GetSpriteSize());
			slotpos.X += (digitwidth + Padding) * fComputeScale;
			result += digitwidth;
		}

		int32 NumPercentDigits = PercentDigits.Num();
		if (NumPercentDigits > 0)
		{
			TSharedRef<SImage> imageLeft = SNew(SImage)
				.ColorAndOpacity(FSlateColor::UseForeground())
				.Image(canvas.EtcBrushSetMap[(int32)eEtc::eWhiteBracketLeft].Get());

			SCanvas::FSlot& slotLeft = canvas.AddSlot()
				.Size(canvas.EtcSpritesMap[(int32)eEtc::eWhiteBracketLeft]->GetSpriteSize())
				.HAlign(HAlign_Left)
				.VAlign(VAlign_Top)
				.Position(slotpos)[imageLeft];

			PercentImages.Emplace(imageLeft);
			SlotPercent.Emplace(&slotLeft);
			float percentwidth = canvas.EtcSpritesMap[(int32)eEtc::eWhiteBracketLeft]->GetSpriteSize().X;
			PercentSizes.Emplace(canvas.EtcSpritesMap[(int32)eEtc::eWhiteBracketLeft]->GetSpriteSize());
			slotpos.X += (percentwidth + Padding) * fComputeScale;

			TSharedRef<SImage> imagePlus = SNew(SImage)
				.ColorAndOpacity(FSlateColor::UseForeground())
				.Image(canvas.EtcBrushSetMap[(int32)eEtc::eWhitePlus].Get());

			SCanvas::FSlot& slotPlus = canvas.AddSlot()
				.Size(canvas.EtcSpritesMap[(int32)eEtc::eWhitePlus]->GetSpriteSize())
				.HAlign(HAlign_Left)
				.VAlign(VAlign_Top)
				.Position(slotpos)[imagePlus];

			PercentImages.Emplace(imagePlus);
			SlotPercent.Emplace(&slotPlus);
			percentwidth = canvas.EtcSpritesMap[(int32)eEtc::eWhitePlus]->GetSpriteSize().X;
			PercentSizes.Emplace(canvas.EtcSpritesMap[(int32)eEtc::eWhitePlus]->GetSpriteSize());
			slotpos.X += (percentwidth + Padding) * fComputeScale;

			for (int32 j = 0; j < NumPercentDigits; ++j)
			{
				TSharedRef<SImage> image1 = SNew(SImage)
					.ColorAndOpacity(FSlateColor::UseForeground())
					.Image(canvas.BrushSetMap[SDamageFloaterV2::eFontColor::eWhiteColor][PercentDigits[j]].Get());

				SCanvas::FSlot& slot1 = canvas.AddSlot()
					.Size(canvas.NumericSpritesMap[SDamageFloaterV2::eFontColor::eWhiteColor][PercentDigits[j]]->GetSpriteSize())
					.HAlign(HAlign_Left)
					.VAlign(VAlign_Top)
					.Position(slotpos)[image1];

				PercentImages.Emplace(image1);
				SlotPercent.Emplace(&slot1);
				percentwidth = canvas.NumericSpritesMap[SDamageFloaterV2::eFontColor::eWhiteColor][PercentDigits[j]]->GetSpriteSize().X;
				PercentSizes.Emplace(canvas.NumericSpritesMap[SDamageFloaterV2::eFontColor::eWhiteColor][PercentDigits[j]]->GetSpriteSize());
				slotpos.X += (percentwidth + Padding) * fComputeScale;
				percentresult += percentwidth;
			}

			TSharedRef<SImage> imagePercent = SNew(SImage)
				.ColorAndOpacity(FSlateColor::UseForeground())
				.Image(canvas.EtcBrushSetMap[(int32)eEtc::eWhitePercent].Get());

			SCanvas::FSlot& slotPercent = canvas.AddSlot()
				.Size(canvas.EtcSpritesMap[(int32)eEtc::eWhitePercent]->GetSpriteSize())
				.HAlign(HAlign_Left)
				.VAlign(VAlign_Top)
				.Position(slotpos)[imagePercent];

			PercentImages.Emplace(imagePercent);
			SlotPercent.Emplace(&slotPercent);
			percentwidth = canvas.EtcSpritesMap[(int32)eEtc::eWhitePercent]->GetSpriteSize().X;
			PercentSizes.Emplace(canvas.EtcSpritesMap[(int32)eEtc::eWhitePercent]->GetSpriteSize());
			slotpos.X += (percentwidth + Padding) * fComputeScale;

			TSharedRef<SImage> imageRight = SNew(SImage)
				.ColorAndOpacity(FSlateColor::UseForeground())
				.Image(canvas.EtcBrushSetMap[(int32)eEtc::eWhiteBracketRight].Get());

			SCanvas::FSlot& slotRight = canvas.AddSlot()
				.Size(canvas.EtcSpritesMap[(int32)eEtc::eWhiteBracketRight]->GetSpriteSize())
				.HAlign(HAlign_Left)
				.VAlign(VAlign_Top)
				.Position(slotpos)[imageRight];

			PercentImages.Emplace(imageRight);
			SlotPercent.Emplace(&slotRight);
			percentwidth = canvas.EtcSpritesMap[(int32)eEtc::eWhiteBracketRight]->GetSpriteSize().X;
			PercentSizes.Emplace(canvas.EtcSpritesMap[(int32)eEtc::eWhiteBracketRight]->GetSpriteSize());
			slotpos.X += (percentwidth + Padding) * fComputeScale;
		}
	}


	if (ShowText)
	{
		float fTotal = slotpos.X - firstPos;

		switch (DamageType)
		{
		case EBattleDamageResultEnum::VE_Critical:
		{			
			TSharedRef<SImage> criticalimage = SNew(SImage)
				.ColorAndOpacity(FSlateColor::UseForeground())
				.Image(canvas.EtcBrushSetMap[(int32)eEtc::eCritical].Get());
			
			FVector2D slotCriticalPos;
			slotCriticalPos.X = firstPos + fTotal * 0.5f - canvas.EtcSpritesMap[(int32)eEtc::eCritical]->GetSpriteSize().X * canvas.ScreenScale;
			slotCriticalPos.Y = slotpos.Y + 50.f;

			SCanvas::FSlot& criticalslot = canvas.AddSlot()
				.Size(canvas.EtcSpritesMap[(int32)eEtc::eCritical]->GetSpriteSize())
				.HAlign(HAlign_Left)
				.VAlign(VAlign_Top)
				.Position(slotCriticalPos)[criticalimage];

			DamageImageEtcs.Emplace(criticalimage);
			SlotEtcs.Emplace(&criticalslot);
			DigitEtcSizes.Emplace(canvas.EtcSpritesMap[(int32)eEtc::eCritical]->GetSpriteSize());
		}
		break;
		case EBattleDamageResultEnum::VE_JustInput:
// 		case EBattleDamageResultEnum::VE_MagicCritical:
		{
			TSharedRef<SImage> criticalimage = SNew(SImage)
				.ColorAndOpacity(FSlateColor::UseForeground())
				.Image(canvas.EtcBrushSetMap[(int32)eEtc::eMagicCritical].Get());

			FVector2D slotCriticalPos;
			slotCriticalPos.X = firstPos + fTotal * 0.5f - canvas.EtcSpritesMap[(int32)eEtc::eMagicCritical]->GetSpriteSize().X * canvas.ScreenScale;
			slotCriticalPos.Y = slotpos.Y + 50.f;

			SCanvas::FSlot& criticalslot = canvas.AddSlot()
				.Size(canvas.EtcSpritesMap[(int32)eEtc::eMagicCritical]->GetSpriteSize())
				.HAlign(HAlign_Left)
				.VAlign(VAlign_Top)
				.Position(slotCriticalPos)[criticalimage];

			DamageImageEtcs.Emplace(criticalimage);
			SlotEtcs.Emplace(&criticalslot);
			DigitEtcSizes.Emplace(canvas.EtcSpritesMap[(int32)eEtc::eMagicCritical]->GetSpriteSize());
		}
		break;
		case EBattleDamageResultEnum::VE_Miss:
		case EBattleDamageResultEnum::VE_Dodge:
		case EBattleDamageResultEnum::VE_ReceiveDodge:
		{
			TSharedRef<SImage> missimage = SNew(SImage)
				.ColorAndOpacity(FSlateColor::UseForeground())
				.Image(canvas.EtcBrushSetMap[(int32)eEtc::eMiss].Get());

			FVector2D slotMissPos;
			slotMissPos.X = firstPos + fTotal * 0.5f - canvas.EtcSpritesMap[(int32)eEtc::eMiss]->GetSpriteSize().X * canvas.ScreenScale;
			slotMissPos.Y = slotpos.Y;

			SCanvas::FSlot& missslot = canvas.AddSlot()
				.Size(canvas.EtcSpritesMap[(int32)eEtc::eMiss]->GetSpriteSize())
				.HAlign(HAlign_Left)
				.VAlign(VAlign_Top)
				.Position(slotMissPos)[missimage];

			DamageImageEtcs.Emplace(missimage);
			SlotEtcs.Emplace(&missslot);
			DigitEtcSizes.Emplace(canvas.EtcSpritesMap[(int32)eEtc::eMiss]->GetSpriteSize());
		}
		break;
		case EBattleDamageResultEnum::VE_Pierce:
		case EBattleDamageResultEnum::VE_ReceivePierce:
		{
			TSharedRef<SImage> pierceimage = SNew(SImage)
				.ColorAndOpacity(FSlateColor::UseForeground())
				.Image(canvas.EtcBrushSetMap[(int32)eEtc::ePierce].Get());

			FVector2D slotPiercePos;
			slotPiercePos.X = firstPos + fTotal * 0.5f - canvas.EtcSpritesMap[(int32)eEtc::ePierce]->GetSpriteSize().X * canvas.ScreenScale;
			slotPiercePos.Y = slotpos.Y + 50.f;

			SCanvas::FSlot& pierceslot = canvas.AddSlot()
				.Size(canvas.EtcSpritesMap[(int32)eEtc::ePierce]->GetSpriteSize())
				.HAlign(HAlign_Left)
				.VAlign(VAlign_Top)
				.Position(slotPiercePos)[pierceimage];

			DamageImageEtcs.Emplace(pierceimage);
			SlotEtcs.Emplace(&pierceslot);
			DigitEtcSizes.Emplace(canvas.EtcSpritesMap[(int32)eEtc::ePierce]->GetSpriteSize());
		}
		break;
		case EBattleDamageResultEnum::VE_Heal:
		{
			TSharedRef<SImage> plusimage = SNew(SImage)
				.ColorAndOpacity(FSlateColor::UseForeground())
				.Image(canvas.EtcBrushSetMap[(int32)eEtc::ePlus].Get());

			FVector2D slotPlusPos;
			slotPlusPos.X = firstPos - (canvas.EtcSpritesMap[(int32)eEtc::ePlus]->GetSpriteSize().X + Padding) * canvas.ScreenScale;
			slotPlusPos.Y = slotpos.Y + 50.f;

			SCanvas::FSlot& plusslot = canvas.AddSlot()
				.Size(canvas.EtcSpritesMap[(int32)eEtc::ePlus]->GetSpriteSize())
				.HAlign(HAlign_Left)
				.VAlign(VAlign_Top)
				.Position(slotPlusPos)[plusimage];

			DamageImageEtcs.Emplace(plusimage);
			SlotEtcs.Emplace(&plusslot);
			DigitEtcSizes.Emplace(canvas.EtcSpritesMap[(int32)eEtc::ePlus]->GetSpriteSize());
		}
		break;
		}
	}

	return (result + (Padding * DigitSizes.Num()));
}

void SDamageFloaterV2::Construct(const FArguments& InArgs)
{
	BasePlayerController = InArgs._BasePlayerController.Get();
	EnableOverlapSort = false;
	bCanTick = true;
	ScreenScale = 1.0f;
	SpacingScale = 1.0f;
	MakeNumericSprites();

	//get device profile
	UDeviceProfile* DeviceProfile = UDeviceProfileManager::Get().GetActiveProfile();
	FString ActiveDeviceProfileName = DeviceProfile->GetName();

	if (ActiveDeviceProfileName == TEXT("iPadAir2"))
	{
		SpacingScale = 0.8f;
	}
	
	SetVisibility(EVisibility::HitTestInvisible);
}

FVector2D SDamageFloaterV2::GetScreenPos(FVector position)
{
	FVector2D result;
	
	if (BasePlayerController != nullptr)
	{
		BasePlayerController->ProjectWorldLocationToScreen(position, result, true);
		result /= ScreenScale;
	}
	return result;
}



SDamageFloaterV2::~SDamageFloaterV2()
{
	BrushSetMap.Empty();
	for (int32 i = 0; i < eFontColor::eColorMax; ++i)
	{
		for (int32 j = 0; j < NumericSpritesMap[i].Num(); ++j)
		{
			NumericSpritesMap[i][j]->RemoveFromRoot();
		}
	}
	NumericSpritesMap.Empty();

	EtcBrushSetMap.Empty();

	TArray<TWeakObjectPtr<class UPaperSprite>> arrayList;
	EtcSpritesMap.GenerateValueArray(arrayList);

	for (TWeakObjectPtr<class UPaperSprite>& sprite : arrayList)
	{
		if(sprite.IsValid())
			sprite->RemoveFromRoot();
	}

	EtcSpritesMap.Empty();

	for (int32 i = 0; i < DamageFloaters.Num(); ++i)
	{
		delete DamageFloaters[i];
	}

	DamageFloaters.Empty();
}

void SDamageFloaterV2::MakeNumericSprites()
{
	NumericSpritesMap.Empty();
	BrushSetMap.Empty();

	TArray<TWeakObjectPtr<class UPaperSprite>> SpriteList;
	TArray<TSharedPtr<FSlateBrush>> BrushList;

	static int32 MaxColor = eFontColor::eColorMax;
	NumericSpritesMap.Reserve(MaxColor);
	BrushSetMap.Reserve(MaxColor);
	static int32 MaxColorArray = 10;
	for (int32 i = 0; i < MaxColor; ++i)
	{
		NumericSpritesMap.Emplace(i, SpriteList);
		BrushSetMap.Emplace(i, BrushList);

		NumericSpritesMap[i].Reserve(MaxColorArray);
		BrushSetMap[i].Reserve(MaxColorArray);
	}

	TArray<FString> colorStringList;
	static int32 MaxColorString = 6;
	colorStringList.Reserve(MaxColorString);
	colorStringList.Emplace(TEXT("HUD_Plotter_Green_"));
	colorStringList.Emplace(TEXT("HUD_Plotter_Red_"));
	colorStringList.Emplace(TEXT("HUD_Plotter_White_"));
	colorStringList.Emplace(TEXT("HUD_Plotter_Yellow_"));
	colorStringList.Emplace(TEXT("HUD_Plotter_Gray_"));
	colorStringList.Emplace(TEXT("HUD_Plotter_Pink_"));

	FString DefaultPathName = TEXT("/Game/UMG/Image/Atlas/HUD/Plotter/");
	for (int32 i = 0; i < 10; ++i)
	{
		for (int32 j = 0; j < MaxColorString; ++j)
		{
			FString defaultnumericspritename = colorStringList[j] + FString::FormatAsNumber(i) + TEXT("_Sprite");
			FStringAssetReference stringassetref(DefaultPathName + defaultnumericspritename + FString(TEXT(".")) + defaultnumericspritename);
			UPaperSprite* papersprite = LoadObject<UPaperSprite>(NULL, *stringassetref.ToString());
			checkf(papersprite != nullptr, TEXT("UPaperSprite is Empty[%s]"), *stringassetref.ToString());
			papersprite->AddToRoot();
			NumericSpritesMap[j].Emplace(papersprite);

			TSharedPtr<FSlateBrush> brush = MakeShareable(new FSlateBrush());
			brush->SetResourceObject(papersprite);

			BrushSetMap[j].Emplace(brush);
		}
	}

	//enum eEtc
	//{
	//	ePlus,
	//	eBlock,
	//	eCritical,
	//	eMagicCritical,
	//	eMiss,
	//	ePierce,
	//	eGrayBracketLeft,
	//	eGrayBracketRight,
	//	eGrayPercent,
	//	eGrayPlus,
	//	eWhiteBracketLeft,
	//	eWhiteBracketRight,
	//	eWhitePercent,
	//	eWhitePlus,
	//	eEtcMax
	//};

	TArray<FString> etcStringList;
	static int32 MaxEtcString = 14;
	etcStringList.Reserve(MaxEtcString);
	etcStringList.Emplace(TEXT("HUD_Plotter_Green_Plus_Sprite"));
	etcStringList.Emplace(TEXT("HUD_Plotter_Block_Sprite"));
	etcStringList.Emplace(TEXT("HUD_Plotter_Critical_Sprite"));
	etcStringList.Emplace(TEXT("HUD_Plotter_MagicCritical_Sprite"));
	etcStringList.Emplace(TEXT("HUD_Plotter_Miss_Sprite"));
	etcStringList.Emplace(TEXT("HUD_Plotter_Pierce_Sprite"));

	etcStringList.Emplace(TEXT("HUD_Plotter_Gray_Bracket_Left_Sprite"));
	etcStringList.Emplace(TEXT("HUD_Plotter_Gray_Bracket_Right_Sprite"));
	etcStringList.Emplace(TEXT("HUD_Plotter_Gray_Percent_Sprite"));
	etcStringList.Emplace(TEXT("HUD_Plotter_Gray_Plus_Sprite"));

	etcStringList.Emplace(TEXT("HUD_Plotter_White_Bracket_Left_Sprite"));
	etcStringList.Emplace(TEXT("HUD_Plotter_White_Bracket_Right_Sprite"));
	etcStringList.Emplace(TEXT("HUD_Plotter_White_Percent_Sprite"));
	etcStringList.Emplace(TEXT("HUD_Plotter_White_Plus_Sprite"));

	EtcSpritesMap.Empty();
	EtcBrushSetMap.Empty();
	EtcSpritesMap.Reserve(MaxEtcString);
	EtcBrushSetMap.Reserve(MaxEtcString);
	for (int32 i = 0; i < MaxEtcString; ++i)
	{
		FStringAssetReference stringassetref(DefaultPathName + etcStringList[i] + FString(TEXT(".")) + etcStringList[i]);
		UPaperSprite* papersprite = LoadObject<UPaperSprite>(NULL, *stringassetref.ToString());
		papersprite->AddToRoot();

		EtcSpritesMap.Emplace(i, papersprite);

		TSharedPtr<FSlateBrush> brush = MakeShareable(new FSlateBrush());
		brush->SetResourceObject(papersprite);

		EtcBrushSetMap.Emplace(i, brush);
	}
}

void SDamageFloaterV2::AddDamageFloater(EBattleDamageResultEnum damagetype, int32 damage, int32 addpercent, FVector basepos, FLinearColor color, float inScale, bool bShowText)
{
	if (GEngine != nullptr && GEngine->GameViewport != nullptr)
	{
		FDamageFloater* damagefloater = new FDamageFloater(damagetype, damage, addpercent, basepos, -5.0f, 1.0f, 100.0f, inScale, bShowText, color, *this);
		DamageFloaters.Emplace(damagefloater);
	}
}

int32 SDamageFloaterV2::OnPaint(const FPaintArgs& Args, const FGeometry& AllottedGeometry, const FSlateRect& MyClippingRect, FSlateWindowElementList& OutDrawElements, int32 LayerId, const FWidgetStyle& InWidgetStyle, bool bParentEnabled) const
{
	LayerId = SCanvas::OnPaint(Args, AllottedGeometry, MyClippingRect, OutDrawElements, LayerId, InWidgetStyle, bParentEnabled);
	return LayerId;
}

void SDamageFloaterV2::Tick(const FGeometry& AllottedGeometry, const double InCurrentTime, const float InDeltaTime)
{	
	if (BasePlayerController)
	{
		float scale = UWidgetLayoutLibrary::GetViewportScale(BasePlayerController);

		ScreenScale = scale;
		for (int32 i = DamageFloaters.Num() - 1; i >= 0; --i)
		{
			DamageFloaters[i]->Update(InDeltaTime, ScreenScale, BasePlayerController);
			if (DamageFloaters[i]->GetRemainedLifeSeconds() <= 0.0f)
			{
				int32 NumDamageImages = DamageFloaters[i]->DamageImages.Num();
				for (int32 j = 0; j < NumDamageImages; ++j)
				{
					this->RemoveSlot(DamageFloaters[i]->DamageImages[j]);
				}
				int32 NumPercentImages = DamageFloaters[i]->PercentImages.Num();
				for (int32 j = 0; j < NumPercentImages; ++j)
				{
					this->RemoveSlot(DamageFloaters[i]->PercentImages[j]);
				}
				int32 NumDamageImageEtcs = DamageFloaters[i]->DamageImageEtcs.Num();
				for (int32 j = 0; j < NumDamageImageEtcs; ++j)
				{
					this->RemoveSlot(DamageFloaters[i]->DamageImageEtcs[j]);
				}
				delete DamageFloaters[i];
				DamageFloaters.RemoveAt(i);
			}
		}
	}
}

//#endif