// TinyTavern

#pragma once

#include "CoreMinimal.h"
#include "ArticyBaseTypes.h"
#include "GameFramework/Actor.h"
#include "TinyTavernGame/Settings/TT_GameInstance.h"
#include "NarrativeUnlockable.generated.h"

class UTutorial_Dialogue_1st_DraftGlobalVariables;
class UGuncleNoteBig;
class UWidgetComponent;
class ATTPlayerController;
class APlayerCharacter;
class UBoxComponent;

USTRUCT(BlueprintType)
struct FNewNPCDialog
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (Categories = "NPC."))
	FGameplayTag NPCName;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FArticyId DialogIDUnlocked;
};

UENUM(BlueprintType)
enum ETypeOfNarrative
{
	GUNCLENOTE,
	NPCENTRY,
	BOTH
};

UCLASS()
class TINYTAVERNGAME_API ANarrativeUnlockable : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	ANarrativeUnlockable();
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<UBoxComponent> NarrativeTrigger;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<UAudioComponent> NarrativeAudio;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<UWidgetComponent> NarrativeWidget;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "TT| Narrative Info")
	TEnumAsByte<ETypeOfNarrative> TypeOfNarrative = ETypeOfNarrative::NPCENTRY;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "TT| Narrative Info")
	TArray<FNewNPCDialog> NewNPCDialogs;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "TT| Narrative Info")
	int GuncleNoteID = 0;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "TT| Narrative Info")
	bool ShouldUpdateArticyGlobalVariable = false;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "TT| Narrative Info")
	FName GlobalVariable;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "TT| Narrative Info")
	TObjectPtr<USoundBase> NPCEntrySound;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "TT| Narrative Info")
	TObjectPtr<USoundBase> GuncleNoteSound;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "TT| Narrative Info")
	TSubclassOf<UGuncleNoteBig> GuncleNoteBigWidget;
	
protected:
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
	TObjectPtr<UGuncleNoteBig> BigNote;
	bool PlayerInsideTrigger = false;
	TObjectPtr<UTutorial_Dialogue_1st_DraftGlobalVariables> ArticyGlobalVariables;
	
	UFUNCTION(BlueprintCallable)
	void PlayerInteracted();
	UFUNCTION(BlueprintCallable)
	void ObtainNote();
	UFUNCTION(BlueprintCallable)
	void ObtainNewNPCEntry();

	bool CheckIfShouldBeEnabled();
	UFUNCTION()
	void GuncleNoteFinishedReading();
	void GuncleNoteUnlocked();
	
};
