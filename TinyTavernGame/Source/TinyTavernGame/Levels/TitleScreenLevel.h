// TinyTavern

#pragma once

#include "CoreMinimal.h"
#include "Engine/LevelScriptActor.h"
#include "TitleScreenLevel.generated.h"

UCLASS()
class TINYTAVERNGAME_API ATitleScreenLevel : public ALevelScriptActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	ATitleScreenLevel();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;
};
