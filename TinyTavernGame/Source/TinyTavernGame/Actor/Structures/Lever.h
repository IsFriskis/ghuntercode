// TinyTavern

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Lever.generated.h"

class UTTAnimNotify;
class ATTPlayerController;
class APlayerCharacter;
class UTT_GameInstance;
class UWidgetComponent;
class UBoxComponent;
class UMaterialInterface;


DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FLeverUsed, int, ID);

UCLASS()
class TINYTAVERNGAME_API ALever : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	ALever();

	UPROPERTY(BlueprintAssignable)
	FLeverUsed LeverUsedDelegate;

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	TObjectPtr<UBoxComponent> LeverTrigger;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	TObjectPtr<USkeletalMeshComponent> LeverMesh;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite,Category = "TT| Lever Settings")
	TObjectPtr<UAudioComponent> LeverAudio;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	TObjectPtr<UWidgetComponent> InteractWidget;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	TObjectPtr<USceneComponent> MotionWarpingLocation;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "TT| Lever Settings")
	TObjectPtr<UStaticMeshComponent> UsedLeverMesh;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "TT| Lever Settings")
	int LeverID = 0;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "TT| Lever Settings")
	TObjectPtr<UAnimMontage> ActorInteractionAnimation;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "TT| Lever Settings")
	FName WarpTargerName;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "TT| Lever Settings")
	TObjectPtr<UAnimationAsset> LeverAnimation;
	UPROPERTY(EditAnywhere, Category = "TT| Lever Settings")
	TObjectPtr<UMaterialInterface> OverlayMaterial;

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	UFUNCTION(BLUEPRINTCALLABLE)
	void OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	UFUNCTION(BLUEPRINTCALLABLE)
	void OnEndOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);
	UFUNCTION(BLUEPRINTCALLABLE)
	void PlayerInteracted();

private:

	bool LeverUsed;
	TObjectPtr<UTT_GameInstance> TTGameInstance;
	TObjectPtr<APlayerCharacter> Player;
	TObjectPtr<ATTPlayerController> PlayerController;
	bool Sound;
	UTTAnimNotify* Notify;
	UTTAnimNotify* Notify2;
	UAnimMontage* AnimMontage;

	UFUNCTION()
	void EndAnimation();
	UFUNCTION()
	void ShowWeapon();

};
