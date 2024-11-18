// TinyTavern


#include "../Actor/EffectActor.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystemComponent.h"
#include "../Player/PlayerCharacter.h"
#include "../Character/Enemy/EnemyBaseClass/EnemyBase.h"

AEffectActor::AEffectActor()
{
 
	PrimaryActorTick.bCanEverTick = false;



}

void AEffectActor::BeginPlay()
{
	Super::BeginPlay();
}

void AEffectActor::ApplyEffectToTarget(AActor* TargetActor, TSubclassOf<UGameplayEffect> GameplayEffectClass)
{
	TObjectPtr<UAbilitySystemComponent> TargetASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(TargetActor);

	if(TargetASC == nullptr)
	{
		return;
	}

	//check(GameplayEffectClass);
	FGameplayEffectContextHandle EffectContextHandle = TargetASC->MakeEffectContext();
	EffectContextHandle.AddSourceObject(this);
	const FGameplayEffectSpecHandle EffectSpecHandle = TargetASC->MakeOutgoingSpec(GameplayEffectClass,ActorLevel,EffectContextHandle);
	const FActiveGameplayEffectHandle ActiveEffectHandle = TargetASC->ApplyGameplayEffectSpecToSelf(*EffectSpecHandle.Data.Get());

	const bool isInfinite = EffectSpecHandle.Data.Get()->Def.Get()->DurationPolicy == EGameplayEffectDurationType::Infinite;
	if(isInfinite && InfiniteEffectRemovalPolicy == EEffectRemovalPolicy::RemoveOnEndOverlap)
	{
		ActiveEffectHandles.Add(ActiveEffectHandle, TargetASC);
	}
}

void AEffectActor::OnOverlap(AActor* TargetActor)
{
	
	if(InstantEffectApplicationPolicy == EEffectApplicationPolicy::ApplyOnOverlap)
	{
		if(InstantGameplayEffectClass) {
			ApplyEffectToTarget(TargetActor,InstantGameplayEffectClass);
			

		}
	}
	if(DurationEffectApplicationPolicy == EEffectApplicationPolicy::ApplyOnOverlap)
	{
		if(DurationGameplayEffectClass)
		{
			ApplyEffectToTarget(TargetActor, DurationGameplayEffectClass);
		}
	}
	if(InfiniteEffectApplicationPolicy == EEffectApplicationPolicy::ApplyOnOverlap)
	{
		if(InfiniteGameplayEffectClass)
		{
			ApplyEffectToTarget(TargetActor, InfiniteGameplayEffectClass);
		}
	}
}

void AEffectActor::EndOverlap(AActor* TargetActor)
{
	if(InstantEffectApplicationPolicy == EEffectApplicationPolicy::ApplyOnEndOverlap)
	{
		ApplyEffectToTarget(TargetActor,InstantGameplayEffectClass);
	}
	if(DurationEffectApplicationPolicy == EEffectApplicationPolicy::ApplyOnEndOverlap)
	{
		ApplyEffectToTarget(TargetActor, DurationGameplayEffectClass);
	}
	if(InfiniteEffectApplicationPolicy == EEffectApplicationPolicy::ApplyOnEndOverlap)
	{
		ApplyEffectToTarget(TargetActor, InfiniteGameplayEffectClass);
	}
	if(InfiniteEffectRemovalPolicy == EEffectRemovalPolicy::RemoveOnEndOverlap)
	{
		UAbilitySystemComponent* TargerASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(TargetActor);
		if(!IsValid(TargerASC))
		{
			return;
		}

		TArray<FActiveGameplayEffectHandle> HandlessToRemove;
		
		for (TTuple<FActiveGameplayEffectHandle, TObjectPtr<UAbilitySystemComponent> > HandlePair : ActiveEffectHandles)
		{
			if (TargerASC == HandlePair.Value)
			{
				TargerASC->RemoveActiveGameplayEffect(HandlePair.Key,1);
				HandlessToRemove.Add(HandlePair.Key);
			}
		}
		for (FActiveGameplayEffectHandle& Handle : HandlessToRemove)
		{
			ActiveEffectHandles.FindAndRemoveChecked(Handle);
		}
	}
}

void AEffectActor::PlayerOnOverlap(AActor* TargetActor)
{
	TObjectPtr<APlayerCharacter> player = Cast<APlayerCharacter>(TargetActor);
	if(player)
	{
		OnOverlap(player);
	}
	
}

void AEffectActor::EnemyOnOverlap( AActor* TargetActor)
{
	TObjectPtr<AEnemyBase> enemy = Cast<AEnemyBase>(TargetActor);
	if (enemy)
	{
		OnOverlap(enemy);
	}
}

void AEffectActor::PlayerEndOverlap(AActor* TargetActor)
{
	TObjectPtr<APlayerCharacter> player = Cast<APlayerCharacter>(TargetActor);
	if (player)
	{
		EndOverlap(player);
	}
}

void AEffectActor::EnemyEndOverlap(AActor* TargetActor)
{
	TObjectPtr<AEnemyBase> enemy = Cast<AEnemyBase>(TargetActor);
	if (enemy)
	{
		EndOverlap(enemy);
	}
}



