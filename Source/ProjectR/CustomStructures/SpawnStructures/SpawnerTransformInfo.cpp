// Fill out your copyright notice in the Description page of Project Settings.

#include "ProjectR.h"
#include "SpawnerTransformInfo.h"


void FSpawnerTransformInfo::MakeTransform( int32 InID, FString strPosition, FString strRotation)
{
	ID = InID;

	TArray< FString > outStrings;
	const TCHAR* Delims[] = { TEXT(",") };
	strPosition.ParseIntoArray(outStrings, Delims, 1);

	if (outStrings.Num() != 3)
		return;

	Position.X = FCString::Atof(*(outStrings[0]));
	Position.Y = FCString::Atof(*(outStrings[1]));
	Position.Z = FCString::Atof(*(outStrings[2]));

	outStrings.Empty();
	strRotation.ParseIntoArray(outStrings, Delims, 1);

	if (outStrings.Num() != 3)
		return;

	float pitch = FCString::Atof(*(outStrings[0]));
	float yaw = FCString::Atof(*(outStrings[1]));
	float roll = FCString::Atof(*(outStrings[2]));
	Rotation = FRotator::MakeFromEuler(FVector(pitch, yaw, roll));
}

FTransform FSpawnerTransformInfo::GetTransform()
{
	FTransform resultTransform;
	resultTransform.SetLocation(Position);
	resultTransform.SetRotation(Rotation.Quaternion());

	return resultTransform;
}

void FSpawnerWallInfo::MakeTransform(int32 InID, int32 InColorID, FString strPath, FString strPosition, FString strRotation, FString strScale)
{
	ID = InID;
	ColorID = InColorID;
	Path = strPath;

	TArray< FString > outStrings;
	const TCHAR* Delims[] = { TEXT(",") };
	strPosition.ParseIntoArray(outStrings, Delims, 1);

	if (outStrings.Num() != 3)
		return;

	Position.X = FCString::Atof(*(outStrings[0]));
	Position.Y = FCString::Atof(*(outStrings[1]));
	Position.Z = FCString::Atof(*(outStrings[2]));

	outStrings.Empty();
	strRotation.ParseIntoArray(outStrings, Delims, 1);

	if (outStrings.Num() != 3)
		return;

	float pitch = FCString::Atof(*(outStrings[0]));
	float yaw = FCString::Atof(*(outStrings[1]));
	float roll = FCString::Atof(*(outStrings[2]));
	Rotation = FRotator::MakeFromEuler(FVector(pitch, yaw, roll));

	outStrings.Empty();
	strScale.ParseIntoArray(outStrings, Delims, 1);

	if (outStrings.Num() != 3)
		return;

	Scale.X = FCString::Atof(*(outStrings[0]));
	Scale.Y = FCString::Atof(*(outStrings[1]));
	Scale.Z = FCString::Atof(*(outStrings[2]));
}

void FSpawnerTriggerInfo::MakeTransform(int32 InID, FString strPath, FString strPosition, FString strRotation, FString strBoxScale)
{
	ID = InID;
	Path = strPath;

	TArray< FString > outStrings;
	const TCHAR* Delims[] = { TEXT(",") };
	strPosition.ParseIntoArray(outStrings, Delims, 1);

	if (outStrings.Num() != 3)
		return;

	Position.X = FCString::Atof(*(outStrings[0]));
	Position.Y = FCString::Atof(*(outStrings[1]));
	Position.Z = FCString::Atof(*(outStrings[2]));

	outStrings.Empty();
	strRotation.ParseIntoArray(outStrings, Delims, 1);

	if (outStrings.Num() != 3)
		return;

	float pitch = FCString::Atof(*(outStrings[0]));
	float yaw = FCString::Atof(*(outStrings[1]));
	float roll = FCString::Atof(*(outStrings[2]));
	Rotation = FRotator::MakeFromEuler(FVector(pitch, yaw, roll));

	outStrings.Empty();
	strBoxScale.ParseIntoArray(outStrings, Delims, 1);

	if (outStrings.Num() != 3)
		return;

	BoxScale.X = FCString::Atof(*(outStrings[0]));
	BoxScale.Y = FCString::Atof(*(outStrings[1]));
	BoxScale.Z = FCString::Atof(*(outStrings[2]));
}