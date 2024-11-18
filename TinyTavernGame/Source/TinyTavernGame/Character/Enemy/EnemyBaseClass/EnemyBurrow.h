// TinyTavern

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "EnemyBurrow.generated.h"

class AEnemyBase;
class UStaticMeshComponent;

UCLASS()
class TINYTAVERNGAME_API AEnemyBurrow : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AEnemyBurrow();
	AEnemyBurrow(FObjectInitializer const& ObjectInitializer);
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Mesh")
	TObjectPtr<UStaticMeshComponent> MeshComponent;
	
	
};
