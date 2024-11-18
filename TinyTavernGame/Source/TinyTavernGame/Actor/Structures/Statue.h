// TinyTavern

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "GameFramework/Actor.h"
#include "Statue.generated.h"

struct FGameplayTag;
class UStatueUI;
class ATTPlayerController;
class APlayerCharacter;
class UTT_GameInstance;
class UWidgetComponent;
class UBoxComponent;

UCLASS()
class TINYTAVERNGAME_API AStatue : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AStatue();
	
	void StatueCompleted();
	void StatueExit();
	UFUNCTION(BlueprintImplementableEvent)
	void BP_StatueCompleted();
	UFUNCTION(BlueprintImplementableEvent)
	void BP_StatueAlreadyOpened();
	
protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<UBoxComponent> StatueTrigger;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<UStaticMeshComponent> StatueMesh;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<UStaticMeshComponent> StatueDoorMesh;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<UAudioComponent> StatueAudio;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<UWidgetComponent> InteractWidget;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "TT| Statue Info")
	TSubclassOf<UStatueUI> StatueUIWidget;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "TT| Statue Info")
	int StatueID = 1;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "TT| Statue Info")
	float StatueDoorInitialSize;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "TT| Statue Info")
	float StatueDoorFinalSize;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "TT| Statue Info")
	FText StatueRiddle;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "TT| Statue Info")
	TObjectPtr<USoundBase> StatueOpeningSound;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "TT| Statue Info")
	float AnimationDoorTime = 3;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "TT| Statue Info")
	FAlphaBlend StatueBlend;
	
	TObjectPtr<UTT_GameInstance> TTGameInstance;
	TObjectPtr<APlayerCharacter> Player;
	TObjectPtr<ATTPlayerController> PlayerController;
	TObjectPtr<UStatueUI> StatueUI;
	bool PlayerHasInteracted = false;
	bool StatueHasBeenCompleted = false;
	FVector StatueDoorInitialPosition;

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

	UFUNCTION(BLUEPRINTCALLABLE)
	void OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	UFUNCTION(BLUEPRINTCALLABLE)
	void OnEndOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION()
	virtual void PlayerInteracted();
	bool CheckIfShouldBeEnabled();
};
