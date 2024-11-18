// TinyTavern

#pragma once

#include "CoreMinimal.h"
#include "InteractComponent.h"
#include "InteractResourceComponent.generated.h"


UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class TINYTAVERNGAME_API UInteractResourceComponent : public UInteractComponent
{
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	UInteractResourceComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType,
	                           FActorComponentTickFunction* ThisTickFunction) override;
	virtual void Interact() override;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, meta=(Category="TT|Animation"))
	TObjectPtr<UAnimMontage> AnimationToRecollect;

	
};
