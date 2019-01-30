// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

//#include "Object.h"
#include "SharedConstants/GlobalConstants.h"

#include "ScopedTimeElapsedLog.generated.h"

USTRUCT(BlueprintType)
struct PROJECTR_API FScopedTimeElapsedLog
{
	GENERATED_USTRUCT_BODY()

public:

	FScopedTimeElapsedLog() {};
	FScopedTimeElapsedLog( const FString& InTimeLogName );
	~FScopedTimeElapsedLog();
	

private:

	double	StartTime = 0.0f;
	double	EndTime = 0.0f;

	FString	TimeLogName;
};
