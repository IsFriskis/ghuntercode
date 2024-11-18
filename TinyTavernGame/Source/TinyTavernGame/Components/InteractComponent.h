// TinyTavern

#pragma once

#include "CoreMinimal.h"
#include "Components/SceneComponent.h"
#include "Components/SphereComponent.h"
#include "Components/WidgetComponent.h"
#include "TinyTavernGame/Player/PlayerCharacter.h"

#include "InteractComponent.generated.h"


UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent), Blueprintable)
class TINYTAVERNGAME_API UInteractComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	UInteractComponent();
	
	UPROPERTY(VisibleAnywhere, Category = "TT|Collision")
	TObjectPtr<USphereComponent> CollisionSphere;

	UPROPERTY(VisibleAnywhere, Category = "TT|UI")
	TObjectPtr<UWidgetComponent> UserWidget;


	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "TT|Drop")
	TSubclassOf<class AEffectActor> DropClass;
	
	UPROPERTY(VisibleAnywhere, Category = "TT|Drop")
	TObjectPtr<USceneComponent> PointToSpawnDrop;
	//

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

	void ShowUI();
	void HideUI();

	UPROPERTY()
	TObjectPtr<APlayerCharacter> PlayerRef;

public:
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType,
	                           FActorComponentTickFunction* ThisTickFunction) override;

	UFUNCTION()
	virtual void Interact();
	
	UFUNCTION()
	void OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
	void OnOverlapEnd(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);
};
