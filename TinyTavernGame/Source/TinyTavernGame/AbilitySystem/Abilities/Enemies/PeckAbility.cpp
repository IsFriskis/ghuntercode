// TinyTavern


#include "PeckAbility.h"

#include "GameFramework/Character.h"
#include "Kismet/GameplayStatics.h"
#include "TinyTavernGame/Character/TTAnimNotify.h"
#include "TinyTavernGame/Character/Enemy/EnemyBaseClass/EnemyBase.h"
#include "TinyTavernGame/Player/PlayerCharacter.h"

void UPeckAbility::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
                                   const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	//PeckAnimNotify = Cast<UTTAnimNotify>(AnimMontage->AnimNotifyTracks[0].Notifies[0]->Notify.Get());
	PeckAnimNotify = Cast<UTTAnimNotify>(AnimMontage->Notifies[0].Notify.Get());
	if(PeckAnimNotify)
	{
		PeckAnimNotify.Get()->OnNotified.AddDynamic(this, &UPeckAbility::ExecutePeck);
	}
}

void UPeckAbility::EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
	const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled)
{
	PeckAnimNotify.Get()->OnNotified.RemoveAll(this);
	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
}

void UPeckAbility::CancelAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
	const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateCancelAbility)
{
	PeckAnimNotify.Get()->OnNotified.RemoveAll(this);
	Super::CancelAbility(Handle, ActorInfo, ActivationInfo, bReplicateCancelAbility);
}
//Hold the logic for the ability
void UPeckAbility::ExecutePeck()
{
	FVector PeckSpawnLocation = Enemy->GetMesh()->GetSocketLocation(FName("Mouth"));
	TArray<FHitResult> OutHits;

	FCollisionShape ColSphere = FCollisionShape::MakeSphere(DamageRadius);
	FVector SweepStart = PeckSpawnLocation;
	FVector SweepEnd = SweepStart;
	
	FCollisionQueryParams QueryParams;
	QueryParams.AddIgnoredActor(Enemy);

	bool isHit = GetWorld()->SweepMultiByChannel(OutHits, SweepStart, SweepEnd, FQuat::Identity, ECC_WorldStatic, ColSphere, QueryParams);

#if WITH_EDITOR
	// draw collision sphere
	DrawDebugSphere(GetWorld(), SweepStart, ColSphere.GetSphereRadius(), 6, FColor::Purple, true);

	if (isHit)
	{
		// loop through TArray
		for (auto& Hit : OutHits)
		{
			if (GEngine) 
			{
				// screen log information on what was hit
				GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, FString::Printf(TEXT("Hit Result: %s"), *Hit.GetActor()->GetName()));
				// uncommnet to see more info on sweeped actor
				// GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("All Hit Information: %s"), *Hit.ToString()));
			}						
		}
	}
#endif
	for (auto& Hit : OutHits)
	{
		if(Hit.GetActor() == Player)
		{
			UAbilitySystemComponent* ASC =  Player->GetAbilitySystemComponent();
			FGameplayEffectContextHandle Handle = ASC->MakeEffectContext();
			FGameplayEffectSpecHandle Spec = ASC->MakeOutgoingSpec(GE_Damage, 1, Handle);
			const FActiveGameplayEffectHandle ActiveEffectHandle = ASC->ApplyGameplayEffectSpecToTarget(*Spec.Data.Get(),ASC);
		}
	}
}
