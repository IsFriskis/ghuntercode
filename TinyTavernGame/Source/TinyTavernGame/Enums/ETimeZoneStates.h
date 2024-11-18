#pragma once

UENUM()
enum class ETimeZoneStates : int
{
	NotPressed,
	Success,
	FailedOnce
};

//USTRUCT()
struct FTimeZone
{
	//GENERATED_BODY()

	float Position;
	ETimeZoneStates State;
};