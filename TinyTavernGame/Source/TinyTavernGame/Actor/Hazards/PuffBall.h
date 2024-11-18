// TinyTavern

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "AbilitySystemInterface.h"
#include "Components/BoxComponent.h"
#include "Components/SphereComponent.h"
#include "TinyTavernGame/Player/PlayerCharacter.h"
#include "PuffBall.generated.h"

class APlayerCharacter;
class UGameplayAbility;

UCLASS()
class TINYTAVERNGAME_API APuffBall : public APawn , public IAbilitySystemInterface
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	APuffBall();
	
	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

	void InitAbilityActorInfo();
	
	virtual void AddCharacterAbilities();

	UPROPERTY(EditAnywhere)
	TObjectPtr <UAbilitySystemComponent> AbilitySystemComponent;
	
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	USkeletalMeshComponent* Mesh;
	
	UPROPERTY(EditAnywhere)
	USphereComponent* SphereTriggerShape;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	bool spawningSpores;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	bool StartingSpores;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	float animRate;

	
	//Reference to the player to use his delegate OnInteract to use the prompt
	TObjectPtr<APlayerCharacter> m_Player;

private:	
	UPROPERTY(EditAnywhere, Category = "Abilites")
	TArray<TSubclassOf<UGameplayAbility>> StartupAbilities;
};
