// TinyTavern

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "BackDoor.generated.h"

class ALever;
class ATTPlayerController;
class APlayerCharacter;
class UTT_GameInstance;
class UWidgetComponent;
class UBoxComponent;

UCLASS()
class TINYTAVERNGAME_API ABackDoor : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	ABackDoor();
	
	void BackDoorCompleted();
	void BackDoorExit();
	UFUNCTION(BlueprintImplementableEvent)
	void BP_BackDoorCompleted();
	
protected:
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite,Category = "TT| BackDoor Settings")
	TObjectPtr<UStaticMeshComponent> BackDoorMesh;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "TT| BackDoor Settings")
	TObjectPtr<ALever> Lever;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "TT| BackDoor Settings")
	int DoorID = 0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "TT| BackDoor Anim Settings")
	float AnimationDoorTime = 3;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "TT| BackDoor Anim Settings")
	float BackDoorHeight;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "TT| BackDoor Anim Settings")
	EAlphaBlendOption BlendType;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "TT| BackDoor Anim Settings")
	TObjectPtr<UAudioComponent> BackDoorAudio = nullptr;
	UPROPERTY()
	FAlphaBlend BackDoorBlend;

	
	TObjectPtr<UTT_GameInstance> TTGameInstance;
	TObjectPtr<APlayerCharacter> Player;
	TObjectPtr<ATTPlayerController> PlayerController;
	bool PlayerHasInteracted = false;
	FVector BackDoorInitialPosition;

	UPROPERTY()
	bool Open = false;
	UPROPERTY()
	FVector TargetPosition;
	bool Sound;
	bool Ended;
	
	UFUNCTION(BlueprintImplementableEvent)
	void BP_DoorActivate();

	UFUNCTION(BlueprintImplementableEvent)
	void BP_DoorEnd();

	UFUNCTION()
	void OpenDoor(int ID);

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	/*UFUNCTION()
	
	virtual void PlayerInteracted();
	bool CheckIfShouldBeEnabled();
	*/


	
	

	
};
