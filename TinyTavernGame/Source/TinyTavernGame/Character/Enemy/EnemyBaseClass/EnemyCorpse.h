// TinyTavern

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "AbilitySystemInterface.h"
#include "Components/BoxComponent.h"
#include "EnemyCorpse.generated.h"

class UAttributeSet;
class UGameplayEffect;
class APlayerCharacter;
class UWidgetComponent;
class UGameplayAbility;

UCLASS()
class TINYTAVERNGAME_API AEnemyCorpse : public AActor, public IAbilitySystemInterface
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AEnemyCorpse();

	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;
	UAttributeSet* GetAttributeSet() const { return AttributeSet; };

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	virtual void Tick(float DeltaTime) override;

	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

	void InitAbilityActorInfo();

	void InitializeAttributes();

	virtual void AddCharacterAbilities();

	UPROPERTY()
	TObjectPtr <UAbilitySystemComponent> AbilitySystemComponent;
	UPROPERTY()
	TObjectPtr <UAttributeSet> AttributeSet;

	UPROPERTY(EditAnywhere, Category = "Attributes")
	TSubclassOf<UGameplayEffect> DefaultAttributes;

	UPROPERTY(EditAnywhere)
	UStaticMeshComponent* Mesh;
	UPROPERTY(EditAnywhere)
	UBoxComponent* TriggerBox;

	UPROPERTY(EditAnywhere, Category = "UI")
	TObjectPtr<class UWidgetComponent> WidgetBar;

	//Reference to the player to use his delegate OnInteract to use the prompt
	TObjectPtr<APlayerCharacter> m_Player;
	bool canInteract = false;
	bool hasTookDish = false;
private:	
	UFUNCTION()
	void OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	UFUNCTION()
	void OnOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);
	UFUNCTION()
	//Function to interact with the corpse and retrieve the enemyTagContainer
	void InteractWithPrompt();

	UPROPERTY(EditAnywhere, Category = "Abilites")
	TArray<TSubclassOf<UGameplayAbility>> StartupAbilities;
};
