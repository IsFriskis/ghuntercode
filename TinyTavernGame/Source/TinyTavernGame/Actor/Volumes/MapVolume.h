// TinyTavern

#pragma once

#include "CoreMinimal.h"
#include "Engine/TriggerVolume.h"
#include "MapVolume.generated.h"

class APlayerCharacter;
class AEnemyBase;

UCLASS()
class TINYTAVERNGAME_API AMapVolume : public ATriggerVolume
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AMapVolume();

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(EditAnywhere,Category = "TT|MapSettings")
	int ID;

	UPROPERTY(EditAnywhere,Category = "TT|MapSettings")
	int MapAreaID;
	
	UFUNCTION()
	void OnOverlapBegin(AActor* OverlappedActor, AActor* OtherActor);
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

private:
	TObjectPtr<APlayerCharacter> Player;
};
