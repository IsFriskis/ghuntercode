// TinyTavern


#include "CreditsLevel.h"

#include "TinyTavernGame/Settings/TT_GameInstance.h"
#include "TinyTavernGame/Settings/TT_SaveGameSystem.h"


// Sets default values
ACreditsLevel::ACreditsLevel()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void ACreditsLevel::BeginPlay()
{
	Super::BeginPlay();
	TObjectPtr<UTT_GameInstance> ttGameInstance = Cast<UTT_GameInstance>(GetGameInstance());

	ttGameInstance.Get()->CompletedTheGame = true;
	
	(new FAsyncTask<FCreateDefaultGameAsyncTask>(ttGameInstance.Get()->GetSubsystem<UTT_SaveGameSystem>(), ttGameInstance.Get()))->StartSynchronousTask();
	FAsyncTask<FSaveGameAsyncTask>* saveGameTask = new FAsyncTask<FSaveGameAsyncTask>(ttGameInstance.Get()->GetSubsystem<UTT_SaveGameSystem>(), ttGameInstance.Get());
	saveGameTask->StartSynchronousTask();
}

