// TinyTavern

#pragma once

#include "CoreMinimal.h"
#include "Engine/LevelScriptActor.h"
#include "CreditsLevel.generated.h"

UCLASS()
class TINYTAVERNGAME_API ACreditsLevel : public ALevelScriptActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	ACreditsLevel();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
};
