// TinyTavern

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Sound/AmbientSound.h"
#include "CamStreamingVolumes.generated.h"

UCLASS()
class TINYTAVERNGAME_API ACamStreamingVolumes : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	ACamStreamingVolumes();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;
	
	UPROPERTY(EditAnywhere, Category = "TT|Comps")
	// ReSharper disable once UnrealHeaderToolError
	TSubclassOf<AAmbientSound> AmbientSound;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "TT|Comps")
	bool bToCombatCam = true;
	
	UPROPERTY(EditAnywhere, Category = "TT|Comps")
	class UBoxComponent* BoxComponent;

	UFUNCTION()
	void OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	
public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;
};
