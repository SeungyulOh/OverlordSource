// Fill out your copyright notice in the Description page of Project Settings.

#include "ProjectR.h"
#include "ScopedTimeElapsedLog.h"


FScopedTimeElapsedLog::FScopedTimeElapsedLog(const FString& InTimeLogName)
{
#if LOG_TIME
	StartTime = FPlatformTime::Seconds();
	EndTime = StartTime;

	TimeLogName = InTimeLogName;
#endif
}

FScopedTimeElapsedLog::~FScopedTimeElapsedLog()
{
#if LOG_TIME
	EndTime = FPlatformTime::Seconds();
	double ElapsedTime = EndTime - StartTime;

	UE_LOG(LogGamePlay, Log, TEXT(" ******* ElapsedTime(%f) - %s *******"), ElapsedTime, *(TimeLogName));
#endif
}