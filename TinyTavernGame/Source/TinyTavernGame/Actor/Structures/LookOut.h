// TinyTavern

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Input/UIActionBindingHandle.h"
#include "TinyTavernGame/Character/TTAnimNotify.h"
#include "LookOut.generated.h"


class ATTPlayerController;
class ULookOutUI;

UCLASS()
class TINYTAVERNGAME_API ALookOut : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	ALookOut();
	
	UFUNCTION()
	void OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
	void OnOverlapEnd(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);
	
	UFUNCTION()
	void Interact();
	UFUNCTION()
	void ExitLogic();

	UFUNCTION()
	void SetCanExit();
	
	UFUNCTION()
	void BlockMovement();
	void ShowOrHideCanInteract(bool bCanInteract);

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(VisibleAnywhere, Category = "TT|Components")
	class UStaticMeshComponent* StaticMeshComponent;

	UPROPERTY(VisibleAnywhere, Category = "TT|Components")
	class UBoxComponent* BoxComponent;

	UPROPERTY(VisibleAnywhere, Category = "TT|Components")
	class UCameraComponent* CameraComponent;

	UPROPERTY(VisibleAnywhere, Category = "TT|Components")
	class UWidgetComponent* WidgetComponent;

	UPROPERTY(EditAnywhere, Category = "TT|Animation")
	class UAnimMontage* ApproachToLookOutMontage;
	
	UPROPERTY(EditAnywhere, Category = "TT|OverlayMaterial")
	class UMaterialInterface* OverlayMaterial;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	class USceneComponent* MotionWarpingLocation;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "TT|Sounds")
	class USoundBase* InSound;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "TT|Sounds")
	class USoundBase* BackSound;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "TT|Sounds")
	TSubclassOf<ULookOutUI> LookOutWidgetClass;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int LookoutID;

	// Called every frame
	virtual void Tick(float DeltaTime) override;

private:
	class APlayerCharacter* PlayerRef;

	bool bCanExit = false;

	UTTAnimNotify* NotifyToSetExit;
	UTTAnimNotify* NotifyToBlockMovement;
	TObjectPtr<ULookOutUI> LookOutWidget;
	bool AlreadyUsed = false;
	bool bIsInside = false;
	TObjectPtr<ATTPlayerController> PlayerController; 
};
