// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "AbilitySystemInterface.h"
#include "CombatInterface.h"
#include "GameplayTagContainer.h"
#include "BasicCharacter.generated.h"

class UWidgetComponent;
struct FOnAttributeChangeData;
class UGameplayEffect;
class UAttributeSet;
class UGameplayAbility;
class UAnimMontage;

UCLASS()
class TINYTAVERNGAME_API ABasicCharacter : public ACharacter, public IAbilitySystemInterface, public ICombatInterface
{
	GENERATED_BODY()
	
public:
	// Sets default values for this character's properties
	ABasicCharacter();
	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	virtual void Die() override;

	//Bind with attributes
	void SetMaxSpeed(const FOnAttributeChangeData& Data);
	void SetEnemySpeed(const FOnAttributeChangeData& Data);
	void InflictPoisonDamage(const FOnAttributeChangeData& Data);

	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;

	UAttributeSet* GetAttributeSet() const { return AttributeSet; }

	virtual UAnimMontage* GetHitReactMontage_Implementation() override;
	virtual UAnimMontage* GetStunMontage_Implementation() override;
	virtual UAnimMontage* GetDieMontage_Implementation() override;
	TObjectPtr<UUserWidget> GetWidgetBar();

	// Container to add tag at actor. Use to identify recipes
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category ="TT|")
	FGameplayTagContainer TagContainer;
	//If player or enemy are reacting to attack or Die
	UPROPERTY(BlueprintReadOnly, Category ="TT|Combat")
	bool HitReactingTagCount = false;
	UPROPERTY(BlueprintReadOnly, Category ="TT|Combat")
	bool DieTagCount = false;

	

	
protected:

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;
	virtual void InitAbilityActorInfo();
	virtual void AddCharacterAbilities();
	void InitializeAttributes();
	void InitializeTags();
	void RemoveInvestigateTag( const FGameplayTag Gameplaytag, int32 Amount);

	UPROPERTY()
	TObjectPtr <UAbilitySystemComponent> AbilitySystemComponent;
	UPROPERTY(EditAnywhere)
	TObjectPtr<UAttributeSet> AttributeSet;
	UPROPERTY(VisibleAnywhere,BlueprintReadOnly)
	TObjectPtr<UWidgetComponent> WidgetBar;

private:

	UPROPERTY(EditAnywhere, Category = "TT|Abilites")
	TArray<TSubclassOf<UGameplayAbility>> StartupAbilities;

	UPROPERTY(EditAnywhere, Category = "TT|Effects")
	TSubclassOf<UGameplayEffect> DefaultAttributes;	

	UPROPERTY(EditAnywhere, Category = "TT|Effects")
	TSubclassOf<UGameplayEffect> DefaultTags;

	UPROPERTY(EditAnywhere, Category = "TT|Effects")
	TSubclassOf<UGameplayEffect> PoisonGameplayEffect;

	UPROPERTY(EditAnywhere, Category = "TT|Combat")
	TObjectPtr<UAnimMontage> HitReactMontage;

	UPROPERTY(EditAnywhere, Category = "TT|Combat")
	TObjectPtr<UAnimMontage> StunMontage;

	UPROPERTY(EditAnywhere, Category = "TT|Combat")
	TObjectPtr<UAnimMontage> DieMontage;
};
