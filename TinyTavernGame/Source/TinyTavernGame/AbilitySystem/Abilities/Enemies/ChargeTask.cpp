// TinyTavern


#include "ChargeTask.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "TinyTavernGame/AbilitySystem/AttributeSet/TT_AttributeSet.h"
#include "TinyTavernGame/Character/Enemy/EnemyBaseClass/EnemyBase.h"
#include "TinyTavernGame/TT_GameplayTags.h"
#include "ChargeAbility.h"
#include "TinyTavernGame/Character/Enemy/Gamusino/EnemyGamusino.h"
#include "TinyTavernGame/Player/PlayerCharacter.h"

UChargeTask::UChargeTask(const FObjectInitializer& ObjectInitializer)
{
	bTickingTask = true;

}

void UChargeTask::TickTask(float DeltaTime)
{
	Super::TickTask(DeltaTime);

	//Init vars that are required to be accesible multiple times
	FVector GnomePos = TargetActor->GetActorLocation();
	FVector EnemyPos = GetOwnerActor()->GetActorLocation();
	FVector Direction = GnomePos - EnemyPos;
	Direction.Normalize();

		#if WITH_EDITOR
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
		#endif

	//Charge time calculation (Just rotates the character towards player)
	if (AimTime > 0.0f)
	{
		GetOwnerActor()->SetActorRotation(Direction.Rotation());
		AimTime -= DeltaTime;

		if (AimTime <= 0.0f)
		{
			SetChargeAim(Direction);
			//OnAimCompleted.Broadcast();
			Charge();
		}
	}

	// TODO: Why are we checking collision twice? We can just use the visibility channel to check for both player and wall at once.

	//Charge time finished so whole charge logic
	else
	{
//		//Raycast to check if we reached the target or hitted a wall
//		FHitResult HitResult;
//		FCollisionQueryParams CollisionParams;
//		CollisionParams.AddIgnoredActor(GetOwnerActor());
//
//		// Perform the raycast
//		bool bHit = GetWorld()->LineTraceSingleByChannel(HitResult, EnemyPos, EnemyPos +  GetOwnerActor()->GetActorForwardVector() * 50.0f, ECC_Visibility, CollisionParams);
//
//		if (bHit)
//		{
//			ApplyStunEffect(StunTime, GetOwnerActor(), StunEffect);
//			OnReachedTarget.Broadcast();
//			EndTask();
//		}
//#if WITH_EDITOR
//		DrawDebugLine(
//			GetWorld(),
//		EnemyPos,
//		EnemyPos +  GetOwnerActor()->GetActorForwardVector() * 50.0f,
//			FColor::Red,  // Red color
//			false,  // Don't persist over frames
//			-1.0f,  // Infinite lifetime
//			0,  // Depth priority
//			1.0f  // Thickness
//			);
//#endif
// 
		//Percentage calculation to know where we are in the charge
		float TotalDist = FVector::Dist(StartPos, EndPos);
		float CurrentDist = FVector::Dist(GetOwnerActor()->GetActorLocation(), EndPos);
		float Percentage = 0.0f;
		
		Percentage = CurrentDist / TotalDist * 100.0f;


		//SpeedBoost calculation
		
		float CurrentSpeed = EnemyAttributeSet->GetSpeed();
		
		//Accel
		if(Percentage >= AccelerationPercentage)
		{

			float Result = FMath::InterpEaseInOut(Percentage , AccelerationPercentage , 1, 3) / 100.0f;
			Result = 1 - Result;
			CurrentSpeed += Result * SpeedBoost;
		}
		//Linear
		else if(Percentage >= DecelerationPercentage / 100)
		{
			CurrentSpeed = EnemyAttributeSet->GetSpeed() + SpeedBoost / 1.5;
		}
		//Deaccel
		else if(Percentage <= DecelerationPercentage)
		{
			float Result = FMath::InterpEaseInOut(DecelerationPercentage , Percentage, 1, 3) / 100.0f;
			Result = 1 - Result;
			CurrentSpeed -= Result * SpeedBoost;
		}

		//Clamp speed so it doesnt go below 0 or higher than max
		CurrentSpeed = FMath::Clamp(CurrentSpeed, 0.0f, EnemyAttributeSet->GetMaxSpeed() * 4.0f);
		EnemyAttributeSet->SetSpeed(CurrentSpeed);
		
		
		//FVector PosToGo = EnemyPos + GetOwnerActor()->GetActorForwardVector() * CurrentSpeed * DeltaTime;


		//Check if hitted player to apply stun
		TArray<FHitResult> Hits;
		FCollisionShape CollisionShape;
		CollisionShape.SetBox(FVector3f(HitboxRangeY, HitboxRangeY, HitboxRangeY)); // TODO: All are hitboxrangeY?
		FCollisionQueryParams QueryParams;
		QueryParams.AddIgnoredActor(GetOwnerActor());
		
		#if WITH_EDITOR
		DrawDebugBox(
			GetWorld(),
			GetOwnerActor()->GetActorLocation(),
			FVector(HitboxRangeX, HitboxRangeX, HitboxRangeY)
			, GetOwnerActor()->GetActorQuat(),
			FColor::Cyan,
			false,
			-1.0f);
		#endif
		
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
					
					if(AEnemyGamusino* Gamusino = Cast<AEnemyGamusino>(GetOwnerActor()))
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
						if(Gamusino->DoesDamage && !PlayerTags.HasTagExact(FGameplayTags::Get().States_Damaged))
						{
							ApplyDamageEffect(Hits[i].GetActor(), DamageEffect);
							ApplyInCombatEffect(Hits[i].GetActor(), InCombatEffect);
							EnemyController.Get()->GetPathFollowingComponent()->AbortMove(*this, FPathFollowingResultFlags::UserAbort);
						}
						//If gamusino is passive, just stun
						else
						{
							ApplyInCombatEffect(GetOwnerActor(), InCombatEffect);
							ApplyStunEffect(StunTime, Hits[i].GetActor(), StunEffect);
							ApplyStunEffect(StunTime, GetOwnerActor(), StunEffect);
							//OnReachedTarget.Broadcast();
							EnemyController.Get()->GetPathFollowingComponent()->AbortMove(*this, FPathFollowingResultFlags::UserAbort);
						}
					}
					//I didn't do this code, no fking idea what is this for
					if(i==Hits.Num() && PlayerTags.HasTagExact(FGameplayTags::Get().States_Damaged))
					{
						PlayerTags.RemoveTag(FGameplayTags::Get().States_Damaged);
					}
				}
				// Enemy hit a wall
				else
				{
					ApplyInCombatEffect(GetOwnerActor(), InCombatEffect);
					ApplyStunEffect(StunTime, GetOwnerActor(), StunEffect);
					EndTask();
					EnemyController.Get()->StopMovement(); // Does not call receive move completed
				}

			}
		}

		////If reached target, stun and end task
		//float FinalDis =  FVector::Dist(GetOwnerActor()->GetActorLocation(), EndPos);
		//if(FinalDis <= 60.0f)
		//{
		//	OnReachedTarget.Broadcast();
		//	EndTask();
		//}
	}
}

// TObjectPtr<UChargeTask> UChargeTask::ExecuteCharge(const FExecuteChargeParams& Params)
// {
// 	TObjectPtr<UChargeTask> ChargeTask = NewAbilityTask<UChargeTask>(Params.OwningAbility);
// 	ChargeTask->TargetActor = Params.TargetActor;
// 	ChargeTask->AimTime = Params.AimTime;
// 	ChargeTask->RecoveryTime = Params.RecoveryTime;
// 	ChargeTask->EnemyController = Params.EnemyController;
// 	ChargeTask->AccelerationPercentage = 100 - Params.AccelerationPercentage;
// 	ChargeTask->DecelerationPercentage = 100 - Params.DecelerationPercentage;
// 	ChargeTask->HitboxRangeX = Params.HitboxRangeX;
// 	ChargeTask->HitboxRangeY = Params.HitboxRangeY;
// 	ChargeTask->StunTime = Params.StunTime;
// 	ChargeTask->CollisionChannel = Params.CollisionChannel;
// 	ChargeTask->StunEffect = Params.StunEffect;
// 	ChargeTask->DamageEffect = Params.DamageEffect;
// 	ChargeTask->InCombatEffect = Params.InCombatEffect;
// 	
//
// 	AEnemyBase* enemyBase = Cast<AEnemyBase>(Params.OwningAbility->GetCurrentActorInfo()->OwnerActor.Get());
// 		
// 	if(enemyBase)
// 	{
// 		UTT_AttributeSet* EnemyAS = CastChecked<UTT_AttributeSet>(enemyBase->GetAttributeSet());
// 		if(EnemyAS)
// 		{
// 			ChargeTask->EnemyAttributeSet = EnemyAS;
// 			ChargeTask->SpeedBoost = EnemyAS->GetSpeed() * Params.MaxSpeed / 100.0f;
// 		}
// 	}
// 	
// 	return ChargeTask;
// }

void UChargeTask::ApplyStunEffect(float _StunTime, AActor* Actor, TSubclassOf<UGameplayEffect> _GameplayEffect)
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

void UChargeTask::ApplyDamageEffect(AActor* Actor, TSubclassOf<UGameplayEffect> _GameplayEffect)
{
	TObjectPtr<UAbilitySystemComponent> PlayerAsc = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(Actor);
	//Grab the cooldown effect passed by reference and apply it setting the duration
	FGameplayEffectContextHandle EffectContextHandle = PlayerAsc->MakeEffectContext();
	EffectContextHandle.AddSourceObject(this);
	FGameplayEffectSpecHandle DamageSpecHandle = PlayerAsc.Get()->MakeOutgoingSpec(_GameplayEffect, 1, EffectContextHandle);

	//The duration is equal to the recovery time based on the charge amount of the weapon

	PlayerAsc.Get()->ApplyGameplayEffectSpecToSelf(*DamageSpecHandle.Data.Get());
}

void UChargeTask::ApplyInCombatEffect(AActor* Actor, TSubclassOf<UGameplayEffect> _GameplayEffect)
{
	TObjectPtr<UAbilitySystemComponent> PlayerAsc = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(Actor);
	//Grab the cooldown effect passed by reference and apply it setting the duration
	FGameplayEffectContextHandle EffectContextHandle = PlayerAsc->MakeEffectContext();
	EffectContextHandle.AddSourceObject(this);
	FGameplayEffectSpecHandle InCombatSpecHandle = PlayerAsc.Get()->MakeOutgoingSpec(_GameplayEffect, 1, EffectContextHandle);

	//The duration is equal to the recovery time based on the charge amount of the weapon

	PlayerAsc.Get()->ApplyGameplayEffectSpecToSelf(*InCombatSpecHandle.Data.Get());
}

void UChargeTask::SetChargeAim(FVector Direction)
{
	StartPos = GetOwnerActor()->GetActorLocation();
	EndPos = TargetActor->GetActorLocation() + Direction * 150.0f;
}

void UChargeTask::Charge()
{
	EnemyController->MoveToLocation(EndPos, 60.0f, false, true, false);
	//EnemyController->ReceiveMoveCompleted.AddDynamic(this, );
}




