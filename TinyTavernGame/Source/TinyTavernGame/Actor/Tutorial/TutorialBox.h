// TinyTavern

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "TutorialBox.generated.h"

class UTutorialPopUps;
class UBoxComponent;

UCLASS()
class TINYTAVERNGAME_API ATutorialBox : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	ATutorialBox();
	
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	void ShowTutorialWidget();
	void RemoveWidget();
	
	UFUNCTION()
	void OnBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp,
	int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult);
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<USceneComponent> DefaultSceneComponent;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<UBoxComponent> BoxCollider;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "TT")
	bool DisplayOnBeginPlay = false;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "TT")
	bool PauseGame = false;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "TT")
	FDataTableRowHandle TutorialRow;
	
	TObjectPtr<UTutorialPopUps> TutorialWidget;

private:
	bool AlreadyActivated = false;
	FTimerHandle TutorialWidgetTimer;
};
