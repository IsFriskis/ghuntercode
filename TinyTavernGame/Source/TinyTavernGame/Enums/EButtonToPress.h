#pragma once

UENUM()
enum class EButtonToPress : uint32
{
	X UMETA(DisplayName = "X/2"),
	B UMETA(DisplayName = "B/3"),
	Y UMETA(DisplayName = "Y/1"),
	INVALID UMETA(DisplayName = "Invalid")
};
