// TinyTavern


#include "PetrificationAbility.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystemComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "TinyTavernGame/TT_GameplayTags.h"
#include "TinyTavernGame/AbilitySystem/AttributeSet/TT_AttributeSet.h"
#include "TinyTavernGame/Character/Enemy/EnemyBaseClass/EnemyBase.h"


void UPetrificationAbility::ActivateAbility(const FGameplayAbilitySpecHandle Handle,
                                            const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo,
                                            const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	ApplyCooldown(Handle, ActorInfo, ActivationInfo);

	GetAbilitySystemComponentFromActorInfo()->RegisterGameplayTagEvent(FGameplayTags::Get().States_InProcessOfPetrification, EGameplayTagEventType::NewOrRemoved).AddUObject(this, &UPetrificationAbility::ApplyPetrification);
	Enemy = Cast<AEnemyBase>(ActorInfo->OwnerActor);
	PetrificationDuration = TriggerEventData->EventMagnitude;
}

void UPetrificationAbility::EndAbility(const FGameplayAbilitySpecHandle Handle,
	const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo,
	bool bReplicateEndAbility, bool bWasCancelled)
{
	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);

	//Set Speed Back
	TObjectPtr<UTT_AttributeSet> attributeSet = Cast<UTT_AttributeSet>(Enemy.Get()->GetAttributeSet());
	Enemy.Get()->GetCharacterMovement()->MaxWalkSpeed = attributeSet->MaxSpeed.GetCurrentValue();
	
	Enemy.Get()->GetCharacterMovement()->SetDefaultMovementMode();
	
	//Remove Material
	if(Enemy.Get()->GetMesh()->GetOverlayMaterial() == PetrifiedMaterial)
	{
		Enemy.Get()->GetMesh()->SetOverlayMaterial(nullptr);
	}

	//Remove Bindings
	GetAbilitySystemComponentFromActorInfo()->RegisterGameplayTagEvent(FGameplayTags::Get().States_InProcessOfPetrification, EGameplayTagEventType::NewOrRemoved).RemoveAll(this);
	GetAbilitySystemComponentFromActorInfo()->RegisterGameplayTagEvent(FGameplayTags::Get().States_InPetrification, EGameplayTagEventType::NewOrRemoved).RemoveAll(this);
}

void UPetrificationAbility::CancelAbility(const FGameplayAbilitySpecHandle Handle,
	const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo,
	bool bReplicateCancelAbility)
{
	Super::CancelAbility(Handle, ActorInfo, ActivationInfo, bReplicateCancelAbility);

	//Set Speed Back
	TObjectPtr<UTT_AttributeSet> attributeSet = Cast<UTT_AttributeSet>(Enemy.Get()->GetAttributeSet());
	Enemy.Get()->GetCharacterMovement()->MaxWalkSpeed = attributeSet->MaxSpeed.GetCurrentValue();
	
	Enemy.Get()->GetCharacterMovement()->SetDefaultMovementMode();
	
	//Remove Material
	if(Enemy.Get()->GetMesh()->GetOverlayMaterial() == PetrifiedMaterial)
	{
		Enemy.Get()->GetMesh()->SetOverlayMaterial(nullptr);
	}

	//Remove Bindings
	GetAbilitySystemComponentFromActorInfo()->RegisterGameplayTagEvent(FGameplayTags::Get().States_InProcessOfPetrification, EGameplayTagEventType::NewOrRemoved).RemoveAll(this);
	GetAbilitySystemComponentFromActorInfo()->RegisterGameplayTagEvent(FGameplayTags::Get().States_InPetrification, EGameplayTagEventType::NewOrRemoved).RemoveAll(this);
}

void UPetrificationAbility::ApplyPetrification(const FGameplayTag Tag, int32 Value)
{
	//Add Jam Tag
	GetAbilitySystemComponentFromActorInfo()->AddLooseGameplayTag(FGameplayTags::Get().States_Ingredient_Jam);
	FGameplayEventData Payload;
	GetAbilitySystemComponentFromActorInfo()->	HandleGameplayEvent(FGameplayTags::Get().States_HitReact, &Payload);

	//Petrification Effect, with duration in Payload as EventMagnitude
	FGameplayEffectContextHandle effectContextHandle = GetAbilitySystemComponentFromActorInfo()->MakeEffectContext();
	effectContextHandle.AddSourceObject(this);
	FGameplayEffectSpecHandle petrificationSpecHandle = GetAbilitySystemComponentFromActorInfo()->MakeOutgoingSpec(PetrificationEffectClass, 1, effectContextHandle);
	UAbilitySystemBlueprintLibrary::SetDuration(petrificationSpecHandle, PetrificationDuration);

	GetAbilitySystemComponentFromActorInfo()->ApplyGameplayEffectSpecToSelf(*petrificationSpecHandle.Data.Get());
	
	//Bind to Tag State.Petrification, so that when effect finishes, this ability ends
	GetAbilitySystemComponentFromActorInfo()->RegisterGameplayTagEvent(FGameplayTags::Get().States_InPetrification, EGameplayTagEventType::NewOrRemoved).AddUObject(this, &UPetrificationAbility::FinishPassive);

	//Set Walk Speed To 0.0f
	Enemy.Get()->GetCharacterMovement()->MaxWalkSpeed = 0.0f;
	//Disable Movement
	Enemy.Get()->GetCharacterMovement()->DisableMovement();
	
	//Add Overlay Material
	//TODO in GameplayCue
	Enemy.Get()->GetMesh()->SetOverlayMaterial(PetrifiedMaterial);
}

void UPetrificationAbility::FinishPassive(const FGameplayTag Tag, int32 Value)
{
	//Add Alert Tag
	GetAbilitySystemComponentFromActorInfo()->AddLooseGameplayTag(FGameplayTags::Get().States_Alert);
	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, false);
}
