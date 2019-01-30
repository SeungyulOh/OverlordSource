// Fill out your copyright notice in the Description page of Project Settings.

#include "ProjectR.h"
#include "CameraBlockSocket.h"
#include "GlobalIntegrated.h"


#include "Character/Interfaces/EntityBaseProperty.h"
#include "Character/Components/FunctionalComponents/BattleEntityComponent.h"
#include "Character/Components/FunctionalComponents/SkillEntityComponent.h"
#include "UtilFunctionIntegrated.h"

ACameraBlockSocket::ACameraBlockSocket(const FObjectInitializer& ObjectInitializer) :
	Super(ObjectInitializer) { }
