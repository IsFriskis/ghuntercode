// TinyTavern

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Chest.generated.h"

class UChestUI;

UENUM()
enum ERelicType
{
	SACREDSPORES = 0,
	FUNGAlSILK = 1,
	GMITHRELIC = 2
};

USTRUCT(BlueprintType)
struct FRelicInfoStruct
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TEnumAsByte<ERelicType> RelicType = SACREDSPORES;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(ClampMin=1), meta=(ClampMax=4), meta=(UIMin=1), meta=(UIMax=4))
	int NumberOfRelic = 1;
};

class ATTPlayerController;
class APlayerCharacter;
class UTT_GameInstance;
class UChestUId;
class UWidgetComponent;
class UBoxComponent;

UCLASS()
class TINYTAVERNGAME_API AChest : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AChest();
	void ChestFinished();
	UFUNCTION(BlueprintImplementableEvent)
	void BP_ChestFinished();
	UFUNCTION(BlueprintImplementableEvent)
	void BP_ChestAlreadyOpened();
	
protected:
	//Widgets
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<UBoxComponent> ChestTrigger;
	//TODO PROBABLY CHANGE TO SKELETAL MESH
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<UStaticMeshComponent> ChestMesh;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<UAudioComponent> ChestAudio;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<UWidgetComponent> InteractWidget;

	//Chest Info
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "TT| Chest Info")
	TSubclassOf<UChestUI> ChestUIWidget;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "TT| Chest Info")
	FRelicInfoStruct RelicObtained;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "TT| Chest Info")
	TObjectPtr<UAnimMontage> ChestMontage;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "TT| Chest Info")
	TObjectPtr<UStaticMesh> ChestOpenMesh;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "TT| Chest Info")
	TObjectPtr<UStaticMesh> PouchofTheDamselsSporesMesh;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "TT| Chest Info")
	TObjectPtr<UStaticMesh> ZerroleanSilkMesh;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "TT| Chest Info")
	TObjectPtr<UStaticMesh> GungstenBarMesh;
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

	UFUNCTION(BLUEPRINTCALLABLE)
	void OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	UFUNCTION(BLUEPRINTCALLABLE)
	void OnEndOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);
	UFUNCTION(BlueprintCallable)
	void ShowChestUI();
	//Sets the relic of the chest in the players hand
	UFUNCTION(BlueprintCallable)
	void BP_ShowRelic();
	
private:
	TObjectPtr<UTT_GameInstance> TTGameInstance;
	TObjectPtr<APlayerCharacter> Player;
	TObjectPtr<ATTPlayerController> PlayerController;
	TObjectPtr<UChestUI> ChestUI;
	bool PlayerHasInteracted = false;

	UFUNCTION(BlueprintCallable)
	void PlayerInteracted();
	bool CheckIfShouldBeEnabled();
	//Returns the mininum number value of the npc relic of this chest
	//so that in can be access/safed correctly.
	//It returns 0 for Goctor, 3 for Gailor, and 7 for Gmith.
	//These values make sense with the definition of ChestOpened
	//in TTGameInstance.
	int ChestOpenedNPCValue();
};
