// TinyTavern

#pragma once

#include "CoreMinimal.h"
#include "NiagaraComponent.h"
#include "GameFramework/Actor.h"
#include "GuardianWall.generated.h"

UCLASS()
class TINYTAVERNGAME_API AGuardianWall : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AGuardianWall();
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "TT|GuardianWallParameters")
	TObjectPtr<UStaticMeshComponent>WallStaticMesh;
	
	UFUNCTION()
	void HideWallsAgain(float DeltaSeconds);
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "TT|GuardianWallParameters")
	float WallTargetHeight;

	UPROPERTY()
	FAlphaBlend GuardianWallDoorBlend;

	UPROPERTY()
	FAlphaBlend GuardianWallDoorBlendDown;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "TT|GuardianWallParameters")
	float WallBlendTimeUp = 1.5;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "TT|GuardianWallParameters")
	float WallBlendTimeDown = 3;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	virtual void Tick(float DeltaSeconds) override;

	FVector GuardianWallInitialPosition;
	FVector GuardianWallTargetPosition;
	bool isClosing = false;
};
