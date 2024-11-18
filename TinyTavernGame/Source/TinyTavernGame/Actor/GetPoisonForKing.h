// TinyTavern

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "GetPoisonForKing.generated.h"

class UPoisonKingUI;
class UWidgetComponent;
class UTT_GameInstance;
class ATTPlayerController;
class APlayerCharacter;
class USphereComponent;

UCLASS()
class TINYTAVERNGAME_API AGetPoisonForKing : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AGetPoisonForKing();

	void PoisonObtained();
	void PoisonDenied();

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<USphereComponent> CircleCollision;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<UWidgetComponent> PoisonKingPopUpWidget;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "TT| GetPoisonForKin Settings")
	TSubclassOf<UPoisonKingUI> PoisonKingUIClass;
	
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

	UFUNCTION(BLUEPRINTCALLABLE)
	void OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	UFUNCTION(BLUEPRINTCALLABLE)
	void OnEndOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

private:
	TObjectPtr<APlayerCharacter> Player;
	TObjectPtr<ATTPlayerController> PlayerController;
	TObjectPtr<UTT_GameInstance> TTGameInstance;
	bool PlayerInsideTrigger = false;
	TObjectPtr<UPoisonKingUI> PoisonKingUI;
	bool PlayerHasInteracted = false;
	
	UFUNCTION(BlueprintCallable)
	void PlayerInteracted();
};
