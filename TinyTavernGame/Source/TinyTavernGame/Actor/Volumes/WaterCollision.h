// TinyTavern

#pragma once

#include "CoreMinimal.h"
#include "Engine/TriggerVolume.h"
#include "WaterCollision.generated.h"

class AEnemyBase;

UCLASS()
class TINYTAVERNGAME_API AWaterCollision : public ATriggerVolume
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AWaterCollision();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION()
	void OnOverlapBegin(AActor* OverlappedActor, AActor* OtherActor);


	UFUNCTION()
	void DiveAnimEnd(FName NotifyName, const FBranchingPointNotifyPayload& BranchingPointPayload);

	TObjectPtr<AEnemyBase> Enemy;
	
	//TObjectPtr<APlayerCharacter> Player;
};
