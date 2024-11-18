// TinyTavern

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/BoxComponent.h"
#include "Destroyable.generated.h"

UCLASS()
class TINYTAVERNGAME_API ADestroyable : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	ADestroyable();

protected:
	
	virtual void BeginPlay() override;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<UStaticMeshComponent> ResourceMesh;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<UBoxComponent> ResourceMeshCollision;
	// VFX to spawn upon destruction
	UPROPERTY(EditAnywhere, Category = "VFX")
	TSubclassOf<AActor> DestructionVFX;
public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	virtual void BeginDestroy() override;
};
