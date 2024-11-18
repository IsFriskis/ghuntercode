// TinyTavern


#include "ChargeAbility.h"

#include "ChargeTask.h"
#include "BehaviorTree/BehaviorTreeTypes.h"
#include "AIController.h"
#include "GameFramework/Actor.h"
#include "ChargeRecoveryAbilityTask.h"
#include "ChargeAimAbilityTask.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "ChargeAbilityTask.h"
#include "Abilities/Tasks/AbilityTask_PlayMontageAndWait.h"

void UChargeAbility::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);
	//Set params for the charge task with Struct
	FExecuteChargeParams Params;
	Params.OwningAbility = this;
	Params.TargetActor = Cast<AActor>(TriggerEventData->OptionalObject);
	Params.EnemyController = Cast<AEnemyAAIController>(Cast<AEnemyBase>(ActorInfo->OwnerActor)->AIController);
	Params.AimTime = AimTime;
	Params.Distance = Distance;
	Params.RecoveryTime = RecoveryTime;
	Params.MaxSpeed = MaxSpeed;
	Params.AccelerationPercentage = AccelerationPercentage;
	Params.DecelerationPercentage = DecelerationPercentage;
	Params.HitboxRangeX = HitboxRangeX;
	Params.HitboxRangeY = HitboxRangeY;
	Params.StunTime = StunTime;
	Params.CollisionChannel = CollisionChannel;
	Params.StunEffect = StunEffect;
	Params.DamageEffect = DamageEffect;
	Params.InCombatEffect = InCombatEffect;

	ActorInfo->AbilitySystemComponent->AddLooseGameplayTag(FGameplayTag::RequestGameplayTag("Events.Enemy.Charge"));
	
	ACharacter* Character = Cast<ACharacter>(ActorInfo->OwnerActor.Get());
	if (Character)
	{
		UCharacterMovementComponent* MovementComponent = Character->GetCharacterMovement();
		if (MovementComponent)
		{
			// Store movement speed
			OriginalSpeed = MovementComponent->MaxWalkSpeed;
			Params.OriginalSpeed = OriginalSpeed;
		}
	}

	if (ChargeMontage)
	{
		float PlayRate = 1.0f / AimTime;
		SetMontagePlayRate(StartSectionName, PlayRate);

		UAbilityTask_PlayMontageAndWait* AnimationTask = UAbilityTask_PlayMontageAndWait::CreatePlayMontageAndWaitProxy(this, NAME_None, ChargeMontage, 1.0f, StartSectionName, false);
		AnimationTask->ReadyForActivation();
	}

	TObjectPtr<UChargeAimAbilityTask> AimTask = UChargeAimAbilityTask::ExecuteAim(Params);
	AimTask->OnCompleted.AddDynamic(this, &UChargeAbility::OnAimCompleted);
	AimTask->ReadyForActivation();

}

void UChargeAbility::EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
	const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled)
{
	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);

	ActorInfo->AbilitySystemComponent->AddLooseGameplayTag(FGameplayTag::RequestGameplayTag("States.Damaged"));
	ActorInfo->AbilitySystemComponent->RemoveLooseGameplayTag(FGameplayTag::RequestGameplayTag("Events.Enemy.Charge"));
}

void UChargeAbility::CancelAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
	const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateCancelAbility)
{
	Super::CancelAbility(Handle, ActorInfo, ActivationInfo, bReplicateCancelAbility);
	ActorInfo->AbilitySystemComponent->RemoveLooseGameplayTag(FGameplayTag::RequestGameplayTag("Events.Enemy.Charge"));
}

void UChargeAbility::OnAimCompleted(FExecuteChargeParams& ChargeParams)
{
	//Init vars that are required to be accesible multiple times
	FVector GnomePos = ChargeParams.TargetActor->GetActorLocation();
	GEngine->AddOnScreenDebugMessage(-1,5,FColor::Red,FString("AimCompleted"));
	TObjectPtr<UChargeAbilityTask> ChargeTask = UChargeAbilityTask::ExecuteCharge(ChargeParams);
	ChargeTask->OnHitWall.AddDynamic(this, &UChargeAbility::OnRecoveryCompleted);
	ChargeTask->OnCompleted.AddDynamic(this, &UChargeAbility::OnChargeCompleted);
	ChargeTask->ReadyForActivation();

	ACharacter* Character = Cast<ACharacter>(GetAvatarActorFromActorInfo());
	if (Character && ChargeMontage)
	{
		// Reset play rate for the loop section if needed
		SetMontagePlayRate(LoopSectionName, 1.0f);

		UAnimInstance* AnimInstance = Character->GetMesh()->GetAnimInstance();
		if (AnimInstance)
		{
			AnimInstance->Montage_JumpToSection(LoopSectionName, ChargeMontage);
		}
	}

}

void UChargeAbility::OnChargeCompleted(FExecuteChargeParams& Params)
{
	GEngine->AddOnScreenDebugMessage(-1,5,FColor::Red,FString("ChargeCompleted"));
	TObjectPtr<UChargeRecoveryAbilityTask> RecoveryTask = UChargeRecoveryAbilityTask::ExecuteRecovery(Params);
	RecoveryTask->OnCompleted.AddDynamic(this, &UChargeAbility::OnRecoveryCompleted);
	RecoveryTask->ReadyForActivation();
	ACharacter* Character = Cast<ACharacter>(GetAvatarActorFromActorInfo());
	if (Character)
	{
		if (ChargeMontage)
		{
			// Set play rate for the end section
			//SetMontagePlayRate(EndSectionName, 0.5f); // Slow down by 50%

			UAnimInstance* AnimInstance = Character->GetMesh()->GetAnimInstance();
			if (AnimInstance)
			{
				AnimInstance->Montage_JumpToSection(EndSectionName, ChargeMontage);
			}
		}
		
		// Reset the movement speed here
		Character->GetCharacterMovement()->MaxWalkSpeed = OriginalSpeed;
	}
}

// Stun the enemy, end ability
void UChargeAbility::OnRecoveryCompleted()
{
	GEngine->AddOnScreenDebugMessage(-1,5,FColor::Red,FString("RecoveryCompleted"));
	ACharacter* Character = Cast<ACharacter>(GetAvatarActorFromActorInfo());
	if (Character)
	{
		// Reset the movement speed here
		Character->GetCharacterMovement()->MaxWalkSpeed = OriginalSpeed;
	}
	EndAbility(FGameplayAbilitySpecHandle(), GetCurrentActorInfo(), FGameplayAbilityActivationInfo(), false, false);
}

void UChargeAbility::SetMontagePlayRate(FName SectionName, float PlayRate)
{
	ACharacter* Character = Cast<ACharacter>(GetAvatarActorFromActorInfo());
	if (Character && ChargeMontage)
	{
		UAnimInstance* AnimInstance = Character->GetMesh()->GetAnimInstance();

		int32 SectionIndex = ChargeMontage->GetSectionIndex(SectionName);
		if (SectionIndex != INDEX_NONE && AnimInstance)
		{
			float SectionLength = ChargeMontage->GetSectionLength(SectionIndex);
			AnimInstance->Montage_SetPlayRate(ChargeMontage, SectionLength / (SectionLength / PlayRate));
		}
	}
}

