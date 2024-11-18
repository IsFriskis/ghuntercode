// Fill out your copyright notice in the Description page of Project Settings.


#include "BasicCharacter.h"
#include "Components/WidgetComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "TinyTavernGame/AbilitySystem/AbilitySystemComponent/TT_ASC.h"
#include "TinyTavernGame/AbilitySystem/AttributeSet/TT_AttributeSet.h"
#include "Blueprint/UserWidget.h"
#include "TinyTavernGame/TT_GameplayTags.h"
#include "TinyTavernGame/Settings/TT_GameInstance.h"

// Sets default values
ABasicCharacter::ABasicCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	AbilitySystemComponent = CreateDefaultSubobject<UTT_ASC>("AbilitySystemComponent");
	AttributeSet = CreateDefaultSubobject<UTT_AttributeSet>("AttributeSet");
	WidgetBar = CreateDefaultSubobject<UWidgetComponent>("WidgetBar");
	WidgetBar->SetupAttachment(GetRootComponent());
}

// Called when the game starts or when spawned
void ABasicCharacter::BeginPlay()
{
	Super::BeginPlay();
	AbilitySystemComponent->RegisterGameplayTagEvent(FGameplayTags::Get().States_Alert,EGameplayTagEventType::NewOrRemoved).AddUObject(this,&ABasicCharacter::RemoveInvestigateTag);
	
}

void ABasicCharacter::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);
	AbilitySystemComponent->RegisterGameplayTagEvent(FGameplayTags::Get().States_Alert,EGameplayTagEventType::NewOrRemoved).RemoveAll(this);
}

// Called every frame
void ABasicCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
}

UAbilitySystemComponent* ABasicCharacter::GetAbilitySystemComponent() const
{
	return AbilitySystemComponent;
}

//TODO: Die Ability
void ABasicCharacter::Die()
{
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Blue, TEXT("HAS MUERTOOOO"));
}

void ABasicCharacter::InitAbilityActorInfo()
{
	
}

void ABasicCharacter::AddCharacterAbilities()
{
	UTT_ASC* TT_ASC = Cast<UTT_ASC>(AbilitySystemComponent);
	TT_ASC->AddCharacterAbilities(StartupAbilities);
}

void ABasicCharacter::InitializeAttributes()
{
	check(AbilitySystemComponent);
	check(DefaultAttributes);
	FGameplayEffectContextHandle Handle = AbilitySystemComponent->MakeEffectContext();
	FGameplayEffectSpecHandle Spec =AbilitySystemComponent->MakeOutgoingSpec(DefaultAttributes,1, Handle);
	AbilitySystemComponent->ApplyGameplayEffectSpecToSelf(*Spec.Data.Get());
}

void ABasicCharacter::InitializeTags()
{
	check(AbilitySystemComponent);
	check(DefaultTags);
	FGameplayEffectContextHandle Handle = AbilitySystemComponent->MakeEffectContext();
	FGameplayEffectSpecHandle Spec = AbilitySystemComponent->MakeOutgoingSpec(DefaultTags, 1, Handle);
	AbilitySystemComponent->ApplyGameplayEffectSpecToSelf(*Spec.Data.Get());
}





void ABasicCharacter::SetMaxSpeed(const FOnAttributeChangeData& Data)
{
	
	GetCharacterMovement()->MaxWalkSpeed = Data.NewValue * Cast<UTT_AttributeSet>(GetAttributeSet())->GetMaxSpeed();
}

void ABasicCharacter::SetEnemySpeed(const FOnAttributeChangeData& Data)
{
	
	GetCharacterMovement()->MaxWalkSpeed = Data.NewValue;
}

void ABasicCharacter::InflictPoisonDamage(const FOnAttributeChangeData& Data)
{
	if(Cast<UTT_AttributeSet>(GetAttributeSet())->GetMaxPoisonResistance() == Data.NewValue)
	{
		FGameplayEffectContextHandle Handle = AbilitySystemComponent->MakeEffectContext();
		FGameplayEffectSpecHandle Spec = AbilitySystemComponent->MakeOutgoingSpec(PoisonGameplayEffect, 1, Handle);
		AbilitySystemComponent->ApplyGameplayEffectSpecToSelf(*Spec.Data.Get());
		Cast<UTT_AttributeSet>(GetAttributeSet())->SetPoisonResistance(0);
	}
}

UAnimMontage* ABasicCharacter::GetHitReactMontage_Implementation()
{
	return HitReactMontage;
}

UAnimMontage* ABasicCharacter::GetStunMontage_Implementation()
{
	return StunMontage;
}

UAnimMontage* ABasicCharacter::GetDieMontage_Implementation()
{
	return DieMontage;
}

TObjectPtr<class UUserWidget> ABasicCharacter::GetWidgetBar()
{
	return WidgetBar.Get()->GetWidget();
}

void ABasicCharacter::RemoveInvestigateTag(const FGameplayTag Gameplaytag, int32 Amount)
{
	AbilitySystemComponent->RemoveLooseGameplayTag(FGameplayTags::Get().States_IA_Investigating);
}