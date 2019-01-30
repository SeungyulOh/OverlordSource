// Fill out your copyright notice in the Description page of Project Settings.

#include "ProjectR.h"
#include "RHeroGacha.h"

#include "GlobalIntegrated.h"

#include "CustomActors/BaseSequenceActor.h"

#include "UtilFunctionIntegrated.h"

#include "UI/DeckManagement/UC_HeroDeck.h"
#include "UI/RWidgetManager.h"
#include "UI/Shop/UP_HeroShop.h"

#include "Lobby/IndoorLobbyStateActor.h"
#include "Lobby/TowerLobbyLevelScriptActor.h"

#include "RWidgetComponent.h"

const FString CardSocketName = TEXT("Card");

// Sets default values
ARHeroGacha::ARHeroGacha()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	SkeletalMeshComponent = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("SkeletalMesh"));
	SkeletalMeshComponent->SetupAttachment(RootComponent);

}

// Called when the game starts or when spawned
void ARHeroGacha::BeginPlay()
{
	Super::BeginPlay();

	switch (eGachaType)
	{
	case ONE:
	case FIVE:
		SkeletalMeshComponent->PlayAnimation(AnimSequenceArray[FIVE * 2], false);
		break;

	case TEN:
		SkeletalMeshComponent->PlayAnimation(AnimSequenceArray[TEN * 2], false);
		break;

	case ELEVEN:
		SkeletalMeshComponent->PlayAnimation(AnimSequenceArray[ELEVEN * 2], false);
		break;
	}

	for (size_t i = 0; i < GachaHeroList.Num() ; ++i)
	{
		FTransform transform;
		ARHeroGachaCard* GachaCard = GetWorld()->SpawnActorDeferred<ARHeroGachaCard>(ARHeroGachaCard::StaticClass(), transform);
		GachaCard->Init();
		GachaCard->CurrentHero = GachaHeroList[i];
		GachaCard->FinishSpawning(transform);

		FAttachmentTransformRules Rules(EAttachmentRule::SnapToTarget, false);
		FString SocketNameStr = CardSocketName + FString::FromInt(eGachaType == FIVE ? i + 1 : i);
		GachaCard->AttachToComponent(SkeletalMeshComponent, Rules, FName(*SocketNameStr));


		HeroCardArray.Emplace(GachaCard);
	}


	UWorld* World = GetWorld();
	if (World)
	{
		APlayerController* PC = UGameplayStatics::GetPlayerController(World, 0);
		UInputComponent* InputComp = PC->InputComponent;

		InputComp->TouchBindings.Empty();
		InputComp->BindTouch(IE_Pressed, this, &ARHeroGacha::CallBack_OnTouch);
	}
	
}


// Called every frame
void ARHeroGacha::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (IsPendingKill())
		return;

	if (SkeletalMeshComponent->IsPlaying())
		return;

	if (!isReady_Select)
	{
		PlayNextAnimation();
		isReady_Select = true;
	}
}

void ARHeroGacha::CallBack_OnTouch(ETouchIndex::Type TouchIndex, FVector Location)
{
	if (isPlayingSpecialCard)
		return;
	
	APlayerController* PC = UGameplayStatics::GetPlayerController(GetWorld(), 0);

	FHitResult hitresult;
	PC->GetHitResultUnderCursor(ECollisionChannel::ECC_Visibility, true, hitresult);


	ARHeroGachaCard* SelectedCard = Cast<ARHeroGachaCard>(hitresult.GetActor());
	if (SelectedCard && !SelectedCard->isSelected)
	{
		USkeletalMeshComponent* SkelComp = SelectedCard->SkeletalMeshComponent;
		if (SkelComp)
		{
			int32 AnimIdx = FMath::Clamp<int32>((int32)SelectedCard->eGradeType, 0 , 1);
			SkelComp->PlayAnimation(SelectedCard->AnimSeqArray[AnimIdx], false);
			
			if (SelectedCard->eGradeType == EGradeType::VE_SPECIAL_HERO)
			{
				TArray<UParticleSystem*> PSArray = SelectedCard->PSArray_SpecialHero_Run;
				SelectedCard->PS_LeftTop->SetTemplate(PSArray[0]);
				SelectedCard->PS_LeftBottom->SetTemplate(PSArray[1]);
				SelectedCard->PS_RightTop->SetTemplate(PSArray[2]);
				SelectedCard->PS_RightBottom->SetTemplate(PSArray[3]);
				isPlayingSpecialCard = true;
			}
			else if (SelectedCard->eGradeType == EGradeType::VE_SPECIAL_SPIRIT)
			{
				TArray<UParticleSystem*> PSArray = SelectedCard->PSArray_SpecialSpirit_Run;
				SelectedCard->PS_LeftTop->SetTemplate(PSArray[0]);
				SelectedCard->PS_LeftBottom->SetTemplate(PSArray[1]);
				SelectedCard->PS_RightTop->SetTemplate(PSArray[2]);
				SelectedCard->PS_RightBottom->SetTemplate(PSArray[3]);
				/*isPlayingSpecialCard = true;*/
			}
			
			SelectedCard->isSelected = true;
			
				
		}

		if (CheckAllSelected())
		{
			URWidgetManager* WidgetManager = RGAMEINSTANCE(this)->RWidgetManager;
			if (WidgetManager)
			{
				UUP_HeroShop* HeroShop = WidgetManager->GetUserWidgetT<UUP_HeroShop>(EWidgetBluePrintEnum::WBP_ShopHero);
				if (HeroShop)
				{
					HeroShop->SetIsDoneSelecting(true);
					HeroShop->SetVisibility_SummonMenus();
				}

			}
		}
	}
}

void ARHeroGacha::Init()
{
	SkeletalMeshArray.Reserve(MAX - 1);
	FSoftObjectPath path;
	path.SetPath(TEXT("SkeletalMesh'/Game/Items/HeroSummon_Card/Hero_Gacha_5_SkeletalMesh.Hero_Gacha_5_SkeletalMesh'"));
	if (path.IsValid())
		SkeletalMeshArray.Emplace(Cast<USkeletalMesh>(path.ResolveObject()));

	path.SetPath(TEXT("SkeletalMesh'/Game/Items/HeroSummon_Card/Hero_Gacha_10_SkeletalMesh.Hero_Gacha_10_SkeletalMesh'"));
	if(path.IsValid())
		SkeletalMeshArray.Emplace(Cast<USkeletalMesh>(path.ResolveObject()));

	path.SetPath(TEXT("SkeletalMesh'/Game/Items/HeroSummon_Card/Hero_Gacha_11_Skeletal.Hero_Gacha_11_Skeletal'"));
	if (path.IsValid())
		SkeletalMeshArray.Emplace(Cast<USkeletalMesh>(path.ResolveObject()));

	AnimSequenceArray.Reserve((MAX - 1) * 2);


	path.SetPath(TEXT("AnimSequence'/Game/Items/HeroSummon_Card/Animations/HeroCard_5_Gacha_Pre_Idle.HeroCard_5_Gacha_Pre_Idle'"));
	if (path.IsValid())
		AnimSequenceArray.Emplace(Cast<UAnimSequence>(path.ResolveObject()));


	path.SetPath(TEXT("AnimSequence'/Game/Items/HeroSummon_Card/Animations/HeroCard_5_Gacha_Idle.HeroCard_5_Gacha_Idle'"));
	if (path.IsValid())
		AnimSequenceArray.Emplace(Cast<UAnimSequence>(path.ResolveObject()));

	path.SetPath(TEXT("AnimSequence'/Game/Items/HeroSummon_Card/Animations/HeroCard_10_Gacha_Pre_Idle.HeroCard_10_Gacha_Pre_Idle'"));
	if (path.IsValid())
		AnimSequenceArray.Emplace(Cast<UAnimSequence>(path.ResolveObject()));

	path.SetPath(TEXT("AnimSequence'/Game/Items/HeroSummon_Card/Animations/HeroCard_10_Gacha_Idle.HeroCard_10_Gacha_Idle'"));
	if (path.IsValid())
		AnimSequenceArray.Emplace(Cast<UAnimSequence>(path.ResolveObject()));

	path.SetPath(TEXT("AnimSequence'/Game/Items/HeroSummon_Card/Animations/HeroCard_11_Gacha_Pre_Idle.HeroCard_11_Gacha_Pre_Idle'"));
	if (path.IsValid())
		AnimSequenceArray.Emplace(Cast<UAnimSequence>(path.ResolveObject()));

	path.SetPath(TEXT("AnimSequence'/Game/Items/HeroSummon_Card/Animations/HeroCard_11_Gacha_Idle.HeroCard_11_Gacha_Idle'"));
	if (path.IsValid())
		AnimSequenceArray.Emplace(Cast<UAnimSequence>(path.ResolveObject()));
}

void ARHeroGacha::SetGachaType(GACHATYPE InType)
{
	eGachaType = InType;

	switch(eGachaType)
	{
		case ONE:
		case FIVE:
			SkeletalMeshComponent->SetSkeletalMesh(SkeletalMeshArray[FIVE]);
			break;

		case TEN:
			SkeletalMeshComponent->SetSkeletalMesh(SkeletalMeshArray[TEN]);
			break;

		case ELEVEN:
			SkeletalMeshComponent->SetSkeletalMesh(SkeletalMeshArray[ELEVEN]);
			break;
	}
}

void ARHeroGacha::PlayNextAnimation()
{
	switch (eGachaType)
	{
	case ONE:
	case FIVE:
		SkeletalMeshComponent->PlayAnimation(AnimSequenceArray[FIVE * 2 + 1], true);
		break;

	case TEN:
		SkeletalMeshComponent->PlayAnimation(AnimSequenceArray[TEN * 2 + 1], true);
		break;

	case ELEVEN:
		SkeletalMeshComponent->PlayAnimation(AnimSequenceArray[ELEVEN * 2 + 1], true);
	}
}

void ARHeroGacha::OpenAllCard()
{
	for (size_t i = 0; i < HeroCardArray.Num(); ++i)
	{
		if (HeroCardArray[i]->eGradeType != EGradeType::VE_NORMAL)
			continue;
		if (HeroCardArray[i]->isSelected)
			continue;

		USkeletalMeshComponent* SkelComp = HeroCardArray[i]->SkeletalMeshComponent;
		SkelComp->PlayAnimation(HeroCardArray[i]->AnimSeqArray[(int32)EGradeType::VE_NORMAL], false);
		HeroCardArray[i]->isSelected = true;
	}

	if (CheckAllSelected())
	{
		URWidgetManager* WidgetManager = RGAMEINSTANCE(this)->RWidgetManager;
		if (WidgetManager)
		{
			UUP_HeroShop* HeroShop = WidgetManager->GetUserWidgetT<UUP_HeroShop>(EWidgetBluePrintEnum::WBP_ShopHero);
			if (HeroShop)
			{
				HeroShop->SetIsDoneSelecting(true);
				HeroShop->SetVisibility_SummonMenus();
			}
				
		}
	}
}

bool ARHeroGacha::CheckAllSelected()
{
	for (size_t i = 0; i < HeroCardArray.Num(); ++i)
	{
		if (HeroCardArray[i]->isSelected == false)
			return false;
	}

	return true;
}

ARHeroGachaCard::ARHeroGachaCard()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	RootComp = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
		
	SkeletalMeshComponent = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("SkeletalMeshComponent"));
	SkeletalMeshComponent->SetupAttachment(RootComp);

	BoxComponent = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxComponent"));
	BoxComponent->SetBoxExtent(FVector(40.f, 32.f, 60.f));
	BoxComponent->SetupAttachment(RootComp);
	BoxComponent->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Block);

	WidgetComp = CreateDefaultSubobject<URWidgetComponent>(FName(TEXT("WidgetComponent")));
	PS_LeftTop = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("PS_LeftTop"));
	PS_RightTop = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("PS_RightTop"));
	PS_LeftBottom = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("PS_LeftBottom"));
	PS_RightBottom = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("PS_RightBottom"));
	PS_Body = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("PS_Body"));
}

void ARHeroGachaCard::BeginPlay()
{
	Super::BeginPlay();

	FAttachmentTransformRules Rules(EAttachmentRule::KeepRelative, false);
	UClass* FoundClass = nullptr;

	FSoftObjectPath path;
	path.SetPath(TEXT("WidgetBlueprint'/Game/UMG/DeckManager/UC_DeckManager_Hero.UC_DeckManager_Hero_C'"));
	if (path.IsValid())
		FoundClass = Cast<UClass>(path.ResolveObject());

	if (FoundClass)
	{
		WidgetComp->AttachToComponent(SkeletalMeshComponent, Rules, FName(TEXT("Card_Body")));
		WidgetComp->SetWidgetClass(FoundClass);
		WidgetComp->SetDrawSize(FVector2D(190, 300));
		WidgetComp->SetRelativeLocation(FVector(0.f, 0.f, -5.f));
		WidgetComp->SetRelativeRotation(FRotator(-90.f, -90.f, 0.f));
		WidgetComp->SetRelativeScale3D(FVector(0.45f, 0.45f, 0.45f));
	}
	
	FCharacterTableInfo* CharacterTable = UTableManager::GetInstancePtr()->GetCharacterRow(CurrentHero.heroId);
	if (CharacterTable)
	{
		if (CharacterTable->Grade >= 5)
		{
			if(Mat_Special)
				SkeletalMeshComponent->SetMaterial(0, Mat_Special);

			if (SkeletalMeshComponent)
				SkeletalMeshComponent->PlayAnimation(AnimIdleArray[1], true);

			if (CharacterTable->IsHero)
			{
				eGradeType = EGradeType::VE_SPECIAL_HERO;
				if(PSArray_Body.IsValidIndex((int32)eGradeType - 1))
					PS_Body->SetTemplate(PSArray_Body[(int32)eGradeType - 1]);

				PS_LeftTop->SetTemplate(PSArray_SpecialHero_Idle[0]);
				PS_RightTop->SetTemplate(PSArray_SpecialHero_Idle[1]);
				PS_LeftBottom->SetTemplate(PSArray_SpecialHero_Idle[2]);
				PS_RightBottom->SetTemplate(PSArray_SpecialHero_Idle[3]);
			}
			else
			{
				eGradeType = EGradeType::VE_SPECIAL_SPIRIT;
				if (PSArray_Body.IsValidIndex((int32)eGradeType - 1))
					PS_Body->SetTemplate(PSArray_Body[(int32)eGradeType - 1]);

				PS_LeftTop->SetTemplate(PSArray_SpecialSpirit_Idle[0]);
				PS_RightTop->SetTemplate(PSArray_SpecialSpirit_Idle[1]);
				PS_LeftBottom->SetTemplate(PSArray_SpecialSpirit_Idle[2]);
				PS_RightBottom->SetTemplate(PSArray_SpecialSpirit_Idle[3]);
			}

			PS_Body->ActivateSystem();
		}
		else
		{
			if (SkeletalMeshComponent)
				SkeletalMeshComponent->PlayAnimation(AnimIdleArray[0], true);

			eGradeType = EGradeType::VE_NORMAL;
			if (CharacterTable->IsHero)
			{
				PS_LeftTop->SetTemplate(PSArray_NormalHero_Idle[0]);
				PS_RightTop->SetTemplate(PSArray_NormalHero_Idle[1]);
				PS_LeftBottom->SetTemplate(PSArray_NormalHero_Idle[2]);
				PS_RightBottom->SetTemplate(PSArray_NormalHero_Idle[3]);
			}
			else
			{
				PS_LeftTop->SetTemplate(PSArray_NormalSpirit_Idle[0]);
				PS_RightTop->SetTemplate(PSArray_NormalSpirit_Idle[1]);
				PS_LeftBottom->SetTemplate(PSArray_NormalSpirit_Idle[2]);
				PS_RightBottom->SetTemplate(PSArray_NormalSpirit_Idle[3]);
			}
		}			
	}
	
	UUC_HeroDeck* HeroDeck = Cast<UUC_HeroDeck>(WidgetComp->GetWidget());
	if (HeroDeck)
		HeroDeck->SetHeroWithID(CurrentHero.heroId.ToString());
}

void ARHeroGachaCard::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (PPTimerArray.Num() == 0)
		return;

	for (size_t i = 0; i < PPTimerArray.Num(); ++i)
	{
		PPTimerArray[i].ElapsedTime += DeltaTime;

		if (PPTimerArray[i].ElapsedTime < PPTimerArray[i].LifeTime)
		{
			if (PPTimerArray[i].PPType == FPostProcessTimer::BLOOM)
			{
				float CurrentIntensity = PPTimerArray[i].Curve->GetFloatValue(PPTimerArray[i].ElapsedTime / PPTimerArray[i].LifeTime);
				URenderFunctionLibrary::SetBloomEffectWithViewTarget(true, PPTimerArray[i].InitialValue + CurrentIntensity);
			}
		}
		else
		{
			if (PPTimerArray[i].PPType == FPostProcessTimer::BLOOM)
			{
				URenderFunctionLibrary::SetBloomEffectWithViewTarget(true, 0);
			}
			PPTimerArray.RemoveAt(i);
		}
	}
}

void ARHeroGachaCard::Init()
{
	FAttachmentTransformRules Rules(EAttachmentRule::KeepRelative, false);

	FSoftObjectPath path;
	path.SetPath(TEXT("SkeletalMesh'/Game/Items/HeroSummon_Card/HeroSummon_Card.HeroSummon_Card'"));
	if (path.IsValid())
	{
		USkeletalMesh* mesh = Cast<USkeletalMesh>(path.ResolveObject());
		SkeletalMeshComponent->SetSkeletalMesh(mesh);
	}

	path.SetPath(TEXT("AnimSequence'/Game/Items/HeroSummon_Card/Animations/HeroSummon_Card_TouchAction.HeroSummon_Card_TouchAction'"));
	if (path.IsValid())
	{
		UAnimSequence* anim = Cast<UAnimSequence>(path.ResolveObject());
		AnimSeqArray.Emplace(anim);
	}

	path.SetPath(TEXT("AnimSequence'/Game/Items/HeroSummon_Card/Animations/HeroSummon_Card_SP_TouchAction.HeroSummon_Card_SP_TouchAction'"));
	if (path.IsValid())
	{
		UAnimSequence* anim = Cast<UAnimSequence>(path.ResolveObject());
		AnimSeqArray.Emplace(anim);
	}

	path.SetPath(TEXT("AnimSequence'/Game/Items/HeroSummon_Card/Animations/HeroSummon_Card_Idle.HeroSummon_Card_Idle'"));
	if (path.IsValid())
	{
		UAnimSequence* anim = Cast<UAnimSequence>(path.ResolveObject());
		AnimIdleArray.Emplace(anim);
	}

	path.SetPath(TEXT("AnimSequence'/Game/Items/HeroSummon_Card/Animations/HeroSummon_Card_SP_Idle.HeroSummon_Card_SP_Idle'"));
	if (path.IsValid())
	{
		UAnimSequence* anim = Cast<UAnimSequence>(path.ResolveObject());
		AnimIdleArray.Emplace(anim);
	}

	path.SetPath(TEXT("MaterialInstanceConstant'/Game/Items/HeroSummon_Card/Material/HeroSummon_Card_SP_Mat.HeroSummon_Card_SP_Mat'"));
	if (path.IsValid())
	{
		UMaterialInstanceConstant* MI = Cast<UMaterialInstanceConstant>(path.ResolveObject());
		Mat_Special = MI;
	}
		

	PSArray_SpecialHero_Idle.Empty();
	for (int32 i = 1; i < 5; ++i)
	{
		TArray<FStringFormatArg> Args;
		Args.Empty();
		Args.Push(FString::FromInt(i));
		FString Path1 = FString::Format(TEXT("ParticleSystem'/Game/FX/Gacha/FX_4_Gacha_CardMagic0{0}_Idle.FX_4_Gacha_CardMagic0{0}_Idle'"), Args);

		path.SetPath(Path1);
		if (path.IsValid())
		{
			UParticleSystem* PS = Cast<UParticleSystem>(path.ResolveObject());
			PSArray_SpecialHero_Idle.Emplace(PS);
		}
	}

	PSArray_SpecialSpirit_Idle.Empty();
	for (int32 i = 1; i < 5; ++i)
	{
		TArray<FStringFormatArg> Args;
		Args.Empty();
		Args.Push(FString::FromInt(i));
		FString Path1 = FString::Format(TEXT("ParticleSystem'/Game/FX/Gacha/FX_4_Gacha_CardSoul0{0}_Idle.FX_4_Gacha_CardSoul0{0}_Idle'"), Args);

		path.SetPath(Path1);
		if (path.IsValid())
		{
			UParticleSystem* PS = Cast<UParticleSystem>(path.ResolveObject());
			PSArray_SpecialSpirit_Idle.Emplace(PS);
		}

		

	}

	PSArray_SpecialSpirit_Run.Empty();
	for (int32 i = 1; i < 5; ++i)
	{
		TArray<FStringFormatArg> Args;
		Args.Empty();
		Args.Push(FString::FromInt(i));
		FString Path1 = FString::Format(TEXT("ParticleSystem'/Game/FX/Gacha/FX_4_Gacha_CardSoul0{0}.FX_4_Gacha_CardSoul0{0}'"), Args);

		path.SetPath(Path1);
		if (path.IsValid())
		{
			UParticleSystem* PS = Cast<UParticleSystem>(path.ResolveObject());
			PSArray_SpecialSpirit_Run.Emplace(PS);
		}

	}

	PSArray_NormalHero_Idle.Empty();
	for (int32 i = 1; i < 5; ++i)
	{
		TArray<FStringFormatArg> Args;
		Args.Empty();
		Args.Push(FString::FromInt(i));
		FString Path1 = FString::Format(TEXT("ParticleSystem'/Game/FX/Gacha/FX_4_Gacha_B_CardMagic0{0}_Idle.FX_4_Gacha_B_CardMagic0{0}_Idle'"), Args);

		path.SetPath(Path1);
		if (path.IsValid())
		{
			UParticleSystem* PS = Cast<UParticleSystem>(path.ResolveObject());
			PSArray_NormalHero_Idle.Emplace(PS);
		}
		
	}

	PSArray_NormalSpirit_Idle.Empty();
	for (int32 i = 1; i < 5; ++i)
	{
		TArray<FStringFormatArg> Args;
		Args.Empty();
		Args.Push(FString::FromInt(i));
		FString Path1 = FString::Format(TEXT("ParticleSystem'/Game/FX/Gacha/FX_4_Gacha_NCardSoul0{0}_Idle.FX_4_Gacha_NCardSoul0{0}_Idle'"), Args);

		path.SetPath(Path1);
		if (path.IsValid())
		{
			UParticleSystem* PS = Cast<UParticleSystem>(path.ResolveObject());
			PSArray_NormalSpirit_Idle.Emplace(PS);
		}
	}

	PSArray_SpecialHero_Run.Empty();
	for (int32 i = 1; i < 5; ++i)
	{
		TArray<FStringFormatArg> Args;
		Args.Empty();
		Args.Push(FString::FromInt(i));
		FString Path1 = FString::Format(TEXT("ParticleSystem'/Game/FX/Gacha/FX_4_Gacha_CardMagic0{0}.FX_4_Gacha_CardMagic0{0}'"), Args);

		path.SetPath(Path1);
		if (path.IsValid())
		{
			UParticleSystem* PS = Cast<UParticleSystem>(path.ResolveObject());
			PSArray_SpecialHero_Run.Emplace(PS);
		}
	}

	PS_LeftTop->AttachToComponent(SkeletalMeshComponent, Rules, FName(TEXT("FX_4_Gacha_CardMagic01")));
	PS_RightTop->AttachToComponent(SkeletalMeshComponent, Rules, FName(TEXT("FX_4_Gacha_CardMagic02")));
	PS_LeftBottom->AttachToComponent(SkeletalMeshComponent, Rules, FName(TEXT("FX_4_Gacha_CardMagic03")));
	PS_RightBottom->AttachToComponent(SkeletalMeshComponent, Rules, FName(TEXT("FX_4_Gacha_CardMagic04")));


	path.SetPath(TEXT("ParticleSystem'/Game/FX/Gacha/FX_4_Gacha_CardLevel.FX_4_Gacha_CardLevel'"));
	if (path.IsValid())
	{
		UParticleSystem* PS = Cast<UParticleSystem>(path.ResolveObject());
		PSArray_Body.Emplace(PS);
	}


	path.SetPath(TEXT("ParticleSystem'/Game/FX/Gacha/FX_4_Gacha_CardSoul_Level.FX_4_Gacha_CardSoul_Level'"));
	if (path.IsValid())
	{
		UParticleSystem* PS = Cast<UParticleSystem>(path.ResolveObject());
		PSArray_Body.Emplace(PS);
	}

	PS_Body->AttachToComponent(SkeletalMeshComponent, Rules, FName(TEXT("FX_4_Gacha_CardLevel")));
	PS_Body->SetRelativeRotation(FRotator(0.f, 0.f, -90.f));
	PS_Body->bAutoActivate = false;
}

void ARHeroGachaCard::CallBack_AfterSpecialAction()
{
	UWorld* world = RGAMEINSTANCE(this)->GetWorld();
	ATowerLobbyLevelScriptActor* LLSA = Cast<ATowerLobbyLevelScriptActor>(world->GetLevelScriptActor());
	if (LLSA->HeroGachaActor)
		LLSA->HeroGachaActor->isPlayingSpecialCard = false;

	APlayerCameraManager* PCM = UGameplayStatics::GetPlayerCameraManager(world, 0);
	if (PCM)
		PCM->StopCameraFade();

	UUP_HeroShop* HeroShop = RGAMEINSTANCE(this)->RWidgetManager->GetUserWidgetT<UUP_HeroShop>(EWidgetBluePrintEnum::WBP_ShopHero);
	if (HeroShop)
	{
		UUC_HeroSummonMenus* SummonMenus = HeroShop->HeroSummonMenus;
		if (SummonMenus)
		{
			SummonMenus->Invalidate(CurrentHero);
			SummonMenus->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
			SummonMenus->HeroNameCanvas->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
		}
		HeroShop->Pause_WatchLevelSequence(false);
	}
	
}

void ARHeroGachaCard::Binding_SpecialSequence()
{
	ABaseSequenceActor* BaseSequenceActor = RGAMEINSTANCE(this)->SequenceManager->GetCurrentSequenceActor();
	if (BaseSequenceActor)
	{
		BaseSequenceActor->SequencePlayer->OnStop.AddDynamic(this, &ARHeroGachaCard::CallBack_AfterSpecialAction);
	}
}

