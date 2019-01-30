// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Engine/StaticMeshActor.h"
#include "WallActor.generated.h"



UCLASS()
class PROJECTR_API AWallActor : public AStaticMeshActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AWallActor(const FObjectInitializer& ObjectInitializer);
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = AWallActor)
	int32							ActorID = 0;

	// 0 = Blue, 1 = Green, 2 = Purple
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = AWallActor)
	int32							ColorTypeID = 0;

	UPROPERTY(Category = AWallActor, VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	UBoxComponent*					BoxComponent = nullptr;

	UFUNCTION(BlueprintImplementableEvent, Category = AWallActor)
	void BP_SetWallColor(int32 ColorID);
};
