// TinyTavern Extra functions file

#pragma once

#include "CoreMinimal.h"

#define print(x) \
if(GEngine){GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Yellow, TEXT(x));}

#define printError(x) \
if(GEngine){GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, TEXT(x));}

#define printf(x, ...) \
if(GEngine){GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, FString::Printf(TEXT(x), __VA_ARGS__));}

#define printFString(x) \
if(GEngine){GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, x);}

#define printVector(x) \
if(GEngine){GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Green, FString::Printf(TEXT("X: %f Y: %f Z: %f"), x.X, x.Y, x.Z));}

#define printVector2(x) \
if(GEngine){GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Green, FString::Printf(TEXT("X: %f Y: %f"), x.X, x.Y));}

#define Tooltip(x) \
meta = (ToolTip = x)

#define ILoveTernarios \
true : false

#define SAVEGAME_SLOT_NAME "SlotA"
#define SAVEGAMEDEFAULT_SLOT_NAME "DefaultValues"
#define SAVEGAME_USERINDEX 0
