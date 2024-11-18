// TinyTavern


#include "BoilGameplayCueLooping.h"
#include "NiagaraComponent.h"

ABoilGameplayCueLooping::ABoilGameplayCueLooping()
{
	// Create the Niagara component and attach it to the root
	NiagaraComponent = CreateDefaultSubobject<UNiagaraComponent>(TEXT("NiagaraComponent"));
	NiagaraComponent->SetupAttachment(RootComponent);
}

void ABoilGameplayCueLooping::BeginPlay()
{
	Super::BeginPlay();
}

void ABoilGameplayCueLooping::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}
