// TinyTavern

#pragma once

#include "CoreMinimal.h"
#include "GameplayCueNotify_Looping.h"
#include "BoilGameplayCueLooping.generated.h"

class UNiagaraComponent;

/**
 * 
 */
UCLASS()
class TINYTAVERNGAME_API ABoilGameplayCueLooping : public AGameplayCueNotify_Looping
{
	GENERATED_BODY()

public:
	ABoilGameplayCueLooping();

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Effects")
	TObjectPtr<UNiagaraComponent> NiagaraComponent;
	
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	// Called every frame
	virtual void Tick(float DeltaTime) override;
};
