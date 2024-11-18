// TinyTavern


#include "ChargeAbilityTask.h"

#include "TinyTavernGame/Character/Enemy/EnemyBaseClass/EnemyBase.h"
#include "TinyTavernGame/Character/Enemy/Gamusino/EnemyGamusino.h"
#include "TinyTavernGame/AbilitySystem/AttributeSet/TT_AttributeSet.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "TinyTavernGame/Player/PlayerCharacter.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "TinyTavernGame/TT_GameplayTags.h"

UChargeAbilityTask::UChargeAbilityTask(const FObjectInitializer& ObjectInitializer)
{
	bTickingTask = true;
}

TObjectPtr<UChargeAbilityTask> UChargeAbilityTask::ExecuteCharge(const FExecuteChargeParams& Params)
{
	TObjectPtr<UChargeAbilityTask> ChargeTask = NewAbilityTask<UChargeAbilityTask>(Params.OwningAbility);
	ChargeTask->Params = Params;
	AEnemyBase* enemyBase = Cast<AEnemyBase>(Params.OwningAbility->GetCurrentActorInfo()->OwnerActor.Get());
	ChargeTask->ChargeTimer = 0;
	ChargeTask->StartPos = enemyBase->GetActorLocation();
	ChargeTask->EndPos = Params.TargetLocation;
	ChargeTask->Params.EnemyController->ReceiveMoveCompleted.AddDynamic(ChargeTask.Get(), &UChargeAbilityTask::OnMoveCompleted);
	ChargeTask->Params.EnemyController->MoveToLocation(Params.TargetLocation, 20.0f, false, true, false);

	if (enemyBase)
	{
		UTT_AttributeSet* EnemyAS = CastChecked<UTT_AttributeSet>(enemyBase->GetAttributeSet());
		if (EnemyAS)
		{
			ChargeTask->EnemyAttributeSet = EnemyAS;
		}
	}
	return ChargeTask;
}

void UChargeAbilityTask::ApplyStunEffect(float _StunTime, AActor* Actor, TSubclassOf<UGameplayEffect> _GameplayEffect)
{
	TObjectPtr<UAbilitySystemComponent> playerASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(Actor);
	//Grab the cooldown effect passed by reference and apply it setting the duration
	FGameplayEffectContextHandle effectContextHandle = playerASC->MakeEffectContext();
	effectContextHandle.AddSourceObject(this);
	FGameplayEffectSpecHandle stunSpecHandle = playerASC.Get()->MakeOutgoingSpec(_GameplayEffect, 1, effectContextHandle);

	//The duration is equal to the recovery time based on the charge amount of the weapon
	UAbilitySystemBlueprintLibrary::SetDuration(stunSpecHandle, _StunTime);

	playerASC.Get()->ApplyGameplayEffectSpecToSelf(*stunSpecHandle.Data.Get());
}

void UChargeAbilityTask::ApplyDamageEffect(AActor* Actor, TSubclassOf<UGameplayEffect> _GameplayEffect)
{
	TObjectPtr<UAbilitySystemComponent> PlayerAsc = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(Actor);
	//Grab the cooldown effect passed by reference and apply it setting the duration
	FGameplayEffectContextHandle EffectContextHandle = PlayerAsc->MakeEffectContext();
	EffectContextHandle.AddSourceObject(this);
	FGameplayEffectSpecHandle DamageSpecHandle = PlayerAsc.Get()->MakeOutgoingSpec(_GameplayEffect, 1, EffectContextHandle);

	//The duration is equal to the recovery time based on the charge amount of the weapon

	PlayerAsc.Get()->ApplyGameplayEffectSpecToSelf(*DamageSpecHandle.Data.Get());
}

void UChargeAbilityTask::ApplyInCombatEffect(AActor* Actor, TSubclassOf<UGameplayEffect> _GameplayEffect)
{
	TObjectPtr<UAbilitySystemComponent> PlayerAsc = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(Actor);
	//Grab the cooldown effect passed by reference and apply it setting the duration
	FGameplayEffectContextHandle EffectContextHandle = PlayerAsc->MakeEffectContext();
	EffectContextHandle.AddSourceObject(this);
	FGameplayEffectSpecHandle InCombatSpecHandle = PlayerAsc.Get()->MakeOutgoingSpec(_GameplayEffect, 1, EffectContextHandle);

	//The duration is equal to the recovery time based on the charge amount of the weapon

	PlayerAsc.Get()->ApplyGameplayEffectSpecToSelf(*InCombatSpecHandle.Data.Get());
}

void UChargeAbilityTask::TickTask(float DeltaTime)
{
	//Init vars that are required to be accesible multiple times
	FVector GnomePos = Params.TargetActor->GetActorLocation();
	FVector EnemyPos = GetOwnerActor()->GetActorLocation();
	FVector Direction = GnomePos - EnemyPos;
	Direction.Normalize();

#if WITH_EDITOR
	DrawHitbox();
#endif

	//Percentage calculation to know where we are in the charge
	float TotalDist = FVector::Dist(StartPos, EndPos);
	float CurrentDist = FVector::Dist(GetOwnerActor()->GetActorLocation(), EndPos);
	float ChargePercentage = 1 - CurrentDist / TotalDist;
	
	//Speed calculation
	float CurrentSpeed = EnemyAttributeSet->GetSpeed();
	float InterpolationAlpha;

	//Accel
	if (ChargePercentage <= Params.AccelerationPercentage)
	{
		InterpolationAlpha = ChargePercentage / Params.AccelerationPercentage;
		float NewSpeed = FMath::InterpEaseInOut(Params.OriginalSpeed, Params.MaxSpeed, InterpolationAlpha, 3);
		CurrentSpeed = NewSpeed;
	}
	//Linear
	else if (ChargePercentage <= Params.DecelerationPercentage)
	{
		CurrentSpeed = Params.MaxSpeed;

		
	}
	//Deaccel
	else if (ChargePercentage > Params.DecelerationPercentage)
	{
		InterpolationAlpha = (ChargePercentage - Params.DecelerationPercentage) / (1 - Params.DecelerationPercentage);
		float NewSpeed = FMath::InterpEaseInOut(Params.MaxSpeed, Params.OriginalSpeed, InterpolationAlpha, 3);
		CurrentSpeed = NewSpeed;
	}

	//Clamp speed so it doesnt go below 0 or higher than max
	// TODO: this clamp is not necessary
	CurrentSpeed = FMath::Clamp(CurrentSpeed, 0.0f, EnemyAttributeSet->GetMaxSpeed() * 4.0f);
	EnemyAttributeSet->SetSpeed(CurrentSpeed);

	ACharacter* Self = Cast<ACharacter>(GetOwnerActor());
	Self->GetCharacterMovement()->MaxWalkSpeed = CurrentSpeed;

	if (CurrentSpeed >= Params.MaxSpeed)
	{
		//Check if hitted player to apply stun
		TArray<FHitResult> Hits;
		FCollisionShape CollisionShape;
		CollisionShape.SetBox(FVector3f(Params.HitboxRangeY, Params.HitboxRangeY, Params.HitboxRangeY)); // TODO: All are hitboxrangeY?
		FCollisionQueryParams QueryParams;
		QueryParams.AddIgnoredActor(GetOwnerActor());

		bool isHit = GetWorld()->SweepMultiByChannel(Hits, GetOwnerActor()->GetActorLocation(), GetOwnerActor()->GetActorLocation(), GetOwnerActor()->GetActorQuat(), ECC_Visibility, CollisionShape, QueryParams);
		if (isHit)
		{
			for (int i = 0; i < Hits.Num(); i++)
			{
				// If I hit the player
				if (APlayerCharacter* Character = Cast<APlayerCharacter>(Hits[i].GetActor()))
				{
					TObjectPtr<UAbilitySystemComponent> TargetAsc = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(Hits[i].GetActor());
					/*FGameplayEventData Payload;
					Payload.TargetTags = FGameplayTagContainer(FGameplayTags::Get().Events_Common_StunAbility);
					Payload.TargetTags.AddTag(FGameplayTags::Get().Events_Common_StunAbility);
					Payload.EventMagnitude = StunTime;*/

					FGameplayTagContainer PlayerTags;
					TargetAsc->GetOwnedGameplayTags(PlayerTags);

					if (AEnemyGamusino* Gamusino = Cast<AEnemyGamusino>(GetOwnerActor()))
					{
						//If hits a gmausino aswell, stun himself and gamusino, canceling abilities
						// TArray<FGameplayAbilitySpec> ActivableAbilities = TargetAsc.Get()->GetActivatableAbilities();
						// FGameplayTagContainer StunContainer;
						// StunContainer.AddTag(FGameplayTags::Get().Events_Common_StunAbility);
						// for (FGameplayAbilitySpec _Ability : ActivableAbilities)
						// {
						// 	if (_Ability.Ability.Get()->AbilityTags != StunContainer && _Ability.IsActive())
						// 	{
						// 		TargetAsc.Get()->CancelAbilityHandle(_Ability.Handle);
						// 	}
						// }
						//If gamusino is aggresive, apply damage
						if (Gamusino->DoesDamage && !PlayerTags.HasTagExact(FGameplayTags::Get().States_Damaged))
						{
							ApplyDamageEffect(Hits[i].GetActor(), Params.DamageEffect);
							ApplyStunEffect(Params.StunTime, Hits[i].GetActor(), Params.StunEffect);
							ApplyInCombatEffect(Hits[i].GetActor(), Params.InCombatEffect);
							Params.EnemyController->GetPathFollowingComponent()->AbortMove(*this, FPathFollowingResultFlags::UserAbort);
						}
						//If gamusino is passive, just stun
						else
						{
							ApplyInCombatEffect(GetOwnerActor(), Params.InCombatEffect);
							ApplyStunEffect(Params.StunTime, Hits[i].GetActor(), Params.StunEffect);
							Params.EnemyController->GetPathFollowingComponent()->AbortMove(*this, FPathFollowingResultFlags::UserAbort);
						}
					}
					//I didn't do this code, no fking idea what is this for
					if (i == Hits.Num() && PlayerTags.HasTagExact(FGameplayTags::Get().States_Damaged))
					{
						PlayerTags.RemoveTag(FGameplayTags::Get().States_Damaged);
					}
				}
				// TODO: Fix enemy collision with other enemies and walls
				// Enemy hit another enemy
				else if (AEnemyBase* OtherEnemy = Cast<AEnemyBase>(Hits[i].GetActor()))
				{
					ApplyInCombatEffect(GetOwnerActor(), Params.InCombatEffect);
					Params.EnemyController->StopMovement(); // Does not call receive move completed
					TObjectPtr<UAbilitySystemComponent> EnemyASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(GetOwnerActor());
					EnemyASC->RemoveLooseGameplayTag(FGameplayTags::Get().Events_Enemy_Charge);
					ApplyStunEffect(Params.StunTime, OtherEnemy, Params.StunEffect);
					ApplyStunEffect(Params.StunTime, GetOwnerActor(), Params.StunEffect);
					EnemyASC->RemoveLooseGameplayTag(FGameplayTags::Get().States_Cackle); 
				}
				// Enemy hit a wall
				else
				{
					ApplyInCombatEffect(GetOwnerActor(), Params.InCombatEffect);
					Params.EnemyController->StopMovement(); // Does not call receive move completed
					TObjectPtr<UAbilitySystemComponent> EnemyASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(GetOwnerActor());
					EnemyASC->RemoveLooseGameplayTag(FGameplayTags::Get().Events_Enemy_Charge);
					ApplyStunEffect(Params.StunTime, GetOwnerActor(), Params.StunEffect);
					EnemyASC->RemoveLooseGameplayTag(FGameplayTags::Get().States_Cackle); 
				}
			}
		}
	}

	

}

void UChargeAbilityTask::DrawHitbox()
{
	DrawDebugSphere(
		GetWorld(),
		EndPos,
		10.f, // radius of the sphere
		4, // number of segments, more segments mean a smoother sphere
		FColor::Blue,
		false,
		-1.f, // duration the sphere should stay
		0,
		1.0f
	);

}

void UChargeAbilityTask::OnMoveCompleted(FAIRequestID RequestID, EPathFollowingResult::Type Result)
{
	OnCompleted.Broadcast(Params);
	EndTask();
}
