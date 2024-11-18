// TinyTavern

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "GameplayTagContainer.h"
#include "TinyTavernGame/Components/InteractComponent.h"
#include "Resource.generated.h"

class UWidgetComponent;
class UBoxComponent;

UCLASS()
class TINYTAVERNGAME_API AResource : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AResource();
	
	//Destroy the resources and spawns the resource drop
	void SpawnDrop();
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	
	void OnState();
	void OffState();
	UFUNCTION(BlueprintCallable)
	void SetShouldDissapear(bool _ShouldDissapear);

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<USkeletalMeshComponent> ResourceSkeletalMesh;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<UBoxComponent> ResourceTrigger;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<UBoxComponent> ResourceMeshCollision;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<UWidgetComponent> ResourceWidget;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<UMotionWarpingComponent> MotionWarpingComponent;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<USceneComponent> MotionWarpingLocation;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "TT| Resource Info", meta = (Categories = "ResourceType"))
	FGameplayTag ResourceType;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "TT| Resource Info")
	TObjectPtr<UMaterial> HighlightMaterial;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "TT| Resource Info")
	FGameplayTag WeaponToObtainTag;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "TT| Resource Info")
	TSubclassOf<class AEffectActor> DropClass;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "TT| Resource Info")
	FText ResourceText;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "TT| Resource Info")
    float ResourceDissapearTime = 0.5f;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "TT| Resource Info")
	FAlphaBlend ResourceDissapearBlend;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "TT| Resource Info")
	TObjectPtr<UMaterialInstance> NoteDissolveMaterial;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool PlayerInside = false;

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UFUNCTION(BLUEPRINTCALLABLE)
	void OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION(BLUEPRINTCALLABLE)
	void OnOverlapEnd(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);
	
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

private:
	bool ShouldDissapear = false;
	TObjectPtr<UMaterialInstanceDynamic> DynamicMat;
};
