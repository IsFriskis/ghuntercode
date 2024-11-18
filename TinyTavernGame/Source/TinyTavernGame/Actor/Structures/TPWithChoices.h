// TinyTavern

#pragma once

#include "CoreMinimal.h"
#include "Components/BoxComponent.h"
#include "GameFramework/Actor.h"
#include "TPWithChoices.generated.h"	

class UTTAnimNotify;
class UNiagaraComponent;
class UBookMap;
class APlayerCharacter;
class UTT_GameInstance;

UCLASS()
class TINYTAVERNGAME_API ATPWithChoices : public AActor
{
	GENERATED_BODY()
		
public:
	// Sets default values for this actor's properties
	ATPWithChoices();
	void EndPlay(const EEndPlayReason::Type EndPlayReason) override;
	UFUNCTION()
	void UnlockedTP();
	FVector GetTPLocation();
	FRotator GetTPRotator();

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	TObjectPtr<UStaticMeshComponent> StaticMesh;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	TObjectPtr<UBoxComponent> ColliderForEntering;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	TObjectPtr<class UWidgetComponent> WidgetComponent;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	TObjectPtr<class USphereComponent> InteractSphere;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	TObjectPtr<USceneComponent> TPWarpingLocation;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	TObjectPtr<USceneComponent> TPActivationWarpingLocation;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	TObjectPtr<UNiagaraComponent> VFX;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	TObjectPtr<UAudioComponent> Audio;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	TObjectPtr<UBoxComponent> ColliderForUnlocking;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "TT|BaseParams")
	int ID = 0;
	UPROPERTY(EditAnywhere, BlueprintReadWrite,Category = "TT|BaseParams")
	bool Unlocked = false;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "TT|BaseParams")
	bool ShouldTPToTavern = false;
	
protected:
	UPROPERTY(EditAnywhere, Category = "TT|Animations")
	TObjectPtr<UAnimMontage> TPAnimMontage;
	UPROPERTY(EditAnywhere, Category = "TT|Animations")
	TObjectPtr<UAnimMontage> TPActivationAnimMontage;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(MakeEditWidget),Category = "TT|BaseParams")
	FTransform TpLocation;
	UPROPERTY(EditAnywhere, BlueprintReadWrite,Category = "TT|BaseParams")
	TSubclassOf<UUserWidget> MapWidgetClass;
	UPROPERTY(EditAnywhere, BlueprintReadWrite,Category = "TT|BaseParams")
	TSubclassOf<UUserWidget> FadeOutWidget;
	
	
	UFUNCTION()
	void Interact();
	UFUNCTION()
	void PlayTPMontage();
	UFUNCTION()
	void TPToTavern();
	UFUNCTION()
	void OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
	void OnOverlapUnlockBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	UFUNCTION()
	void OnOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

private:
	TObjectPtr<UUserWidget> InteractWidget = nullptr;
	TObjectPtr<APlayerCharacter> Player;
	TObjectPtr<UTT_GameInstance> GameInstance = nullptr;
	bool OnActivationAnim = false;
};
