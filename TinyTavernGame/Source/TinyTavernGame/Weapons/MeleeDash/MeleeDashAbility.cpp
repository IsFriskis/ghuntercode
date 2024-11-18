// TinyTavern


#include "MeleeDashAbility.h"

#include "AbilitySystemComponent.h"
#include "MeleeDashTask.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "TinyTavernGame/TT_GameplayTags.h"
#include "TinyTavernGame/Character/TTAnimNotify.h"
#include "TinyTavernGame/Components/WeaponComponent.h"
#include "TinyTavernGame/Player/PlayerCharacter.h"


void UMeleeDashAbility::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);
	
	Player = Cast<APlayerCharacter>(ActorInfo->AvatarActor);
	
	EventMagnitude = TriggerEventData->EventMagnitude;
	

	FVector ForwardVector = Player.Get()->GetActorForwardVector();
	FVector VelocityVector = Player.Get()->GetVelocity();

	// Normalize the vectors
	ForwardVector.Normalize();
	VelocityVector.Normalize();

	// Calculate the angle between ForwardVector and VelocityVector
	float Angle = FMath::RadiansToDegrees(acosf(FVector::DotProduct(ForwardVector, VelocityVector)));

	// Check if the angle differs more than 80 degrees
	if (Angle < 100.0f)
	{
		//MeleeDashRotate = MeleeDashRotate.Get()->ExecuteDashRotateTask(this);
		//MeleeDashRotate.Get()->ReadyForActivation();	
		//MeleeDashRotate.Get()->OnTaskEnded.AddUObject(this, &UMeleeDashAbility::OnRotationEnd);
		Player.Get()->GetWeaponComponent()->ChangeWeaponForDash();
		if(VelocityVector.Size() > 0)
		{
			FVector NearEnemy = MakeAutoAimSweep(VelocityVector);
			if(NearEnemy == FVector::ZeroVector)
			{
				Player->SetActorRotation(VelocityVector.Rotation());
			}
			else
			{
				Player->SetActorRotation(NearEnemy.Rotation());
			}
		}
		else
		{
			FVector NearEnemy = MakeAutoAimSweep(ForwardVector);
			if(NearEnemy == FVector::ZeroVector)
			{
				Player->SetActorRotation(ForwardVector.Rotation());
			}
			else
			{
				Player->SetActorRotation(NearEnemy.Rotation());
			}
		}
		MeleeDashTask = MeleeDashTask.Get()->ExecuteDashTask(this);
		MeleeDashTaskAnim = MeleeDashTaskAnim->CreatePlayMontageAndWaitProxy(this, "DashMontage", DashMontage.Get());
		MeleeDashTask.Get()->ReadyForActivation();
		MeleeDashTaskAnim.Get()->ReadyForActivation();
		for (int i = 0; i < DashMontage->Notifies.Num(); i++)
		{
			UTTAnimNotify* Notify = Cast<UTTAnimNotify>(DashMontage->Notifies[i].Notify.Get());
			if (Notify)
			{
				Notify->OnNotified.RemoveAll(this);
				Notify->OnNotified.AddDynamic(this, &UMeleeDashAbility::EndHitbox);
			}
		}
		MeleeDashTaskAnim.Get()->OnCompleted.AddDynamic(this, &UMeleeDashAbility::OnAnimationEnd);
		MeleeDashTaskAnim.Get()->OnCancelled.AddDynamic(this, &UMeleeDashAbility::OnAnimationEnd);
		MeleeDashTaskAnim.Get()->OnInterrupted.AddDynamic(this, &UMeleeDashAbility::OnAnimationEnd);
	}
	else
	{
		MeleeDashBackwardsTaskAnim = MeleeDashBackwardsTaskAnim->CreatePlayMontageAndWaitProxy(this, "DashMontage", DashBackwardsMontage.Get());
		
		MeleeDashBackwardsTaskAnim.Get()->ReadyForActivation();
		FVector OppositeDirection = -VelocityVector.GetSafeNormal();

		// Calculate the rotation to face the opposite direction
		FRotator NewRotation = OppositeDirection.Rotation();

		// Set the player's rotation
		Player->SetActorRotation(NewRotation);

		for (int i = 0; i < DashBackwardsMontage->Notifies.Num(); i++)
		{
			UTTAnimNotify* Notify = Cast<UTTAnimNotify>(DashBackwardsMontage->Notifies[i].Notify.Get());
			if (Notify)
			{
				Notify->OnNotified.RemoveAll(this);
				Notify->OnNotified.AddDynamic(this, &UMeleeDashAbility::RemoveDelegates);
				Notify->OnNotified.AddDynamic(this, &UMeleeDashAbility::RemoveTags);
			}
		}
		
		MeleeDashBackwardsTaskAnim.Get()->OnCompleted.AddDynamic(this, &UMeleeDashAbility::OnAnimationEnd);
		MeleeDashBackwardsTaskAnim.Get()->OnCancelled.AddDynamic(this, &UMeleeDashAbility::OnAnimationEnd);
		MeleeDashBackwardsTaskAnim.Get()->OnInterrupted.AddDynamic(this, &UMeleeDashAbility::OnAnimationEnd);
	}
	if (TObjectPtr<UAbilitySystemComponent> PlayerASC = Player.Get()->GetAbilitySystemComponent())
	{
		PlayerASC->RemoveLooseGameplayTag(FGameplayTags::Get().States_PlayerSingleFire);
		PlayerASC->AddLooseGameplayTag(FGameplayTags::Get().States_WeaponsBlocked);
		PlayerASC->AddLooseGameplayTag(FGameplayTags::Get().States_AimBlocked);
		PlayerASC->AddLooseGameplayTag(FGameplayTags::Get().States_OnDashAttack);
	}
}

void UMeleeDashAbility::EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled)
{
	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);

	Player->GetAbilitySystemComponent()->RemoveLooseGameplayTag(FGameplayTags::Get().States_WeaponsBlocked, 3);
	Player->GetAbilitySystemComponent()->RemoveLooseGameplayTag(FGameplayTags::Get().States_AimBlocked,3);
	Player->GetAbilitySystemComponent()->RemoveLooseGameplayTag(FGameplayTags::Get().States_OnDashAttack, 3);
	RemoveDelegates();
}

void UMeleeDashAbility::CancelAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateCancelAbility)
{
	Super::CancelAbility(Handle, ActorInfo, ActivationInfo, bReplicateCancelAbility);

	
	OnAnimationEnd();
//	Player->GetAbilitySystemComponent()->RemoveLooseGameplayTag(FGameplayTags::Get().States_WeaponsBlocked);
	//Player->GetAbilitySystemComponent()->RemoveLooseGameplayTag(FGameplayTags::Get().States_AimBlocked);
	//Player->GetCharacterMovement()->StopMovementImmediately();
	
}

void UMeleeDashAbility::OnAnimationEnd()
{
	EndAbility(GetCurrentAbilitySpecHandle(), GetCurrentActorInfo(), GetCurrentActivationInfo(), false, false);
}

void UMeleeDashAbility::OnRotationEnd()
{

	MeleeDashRotate.Get()->OnTaskEnded.RemoveAll(this);
	
	MeleeDashTask = MeleeDashTask.Get()->ExecuteDashTask(this);
	MeleeDashTaskAnim = MeleeDashTaskAnim->CreatePlayMontageAndWaitProxy(this, "DashMontage", DashMontage.Get());
	MeleeDashTask.Get()->ReadyForActivation();
	MeleeDashTaskAnim.Get()->ReadyForActivation();
	for (int i = 0; i < DashMontage->Notifies.Num(); i++)
	{
		UTTAnimNotify* Notify = Cast<UTTAnimNotify>(DashMontage->Notifies[i].Notify.Get());
		if (Notify)
		{
			Notify->OnNotified.RemoveAll(this);
			Notify->OnNotified.AddDynamic(this, &UMeleeDashAbility::EndHitbox);
		}
	}
	MeleeDashTaskAnim.Get()->OnCompleted.AddDynamic(this, &UMeleeDashAbility::OnAnimationEnd);
	MeleeDashTaskAnim.Get()->OnCancelled.AddDynamic(this, &UMeleeDashAbility::OnAnimationEnd);
	MeleeDashTaskAnim.Get()->OnInterrupted.AddDynamic(this, &UMeleeDashAbility::OnAnimationEnd);
}

void UMeleeDashAbility::EndHitbox()
{
	for (int i = 0; i < DashMontage->Notifies.Num(); i++)
	{
		UTTAnimNotify* Notify = Cast<UTTAnimNotify>(DashMontage->Notifies[i].Notify.Get());
		if (Notify)
		{
			Notify->OnNotified.RemoveAll(this);
		}
	}
	if(MeleeDashTask)
	{
		MeleeDashTask.Get()->EndTask();
	}
}

void UMeleeDashAbility::RemoveTags()
{
	Player->GetAbilitySystemComponent()->RemoveLooseGameplayTag(FGameplayTags::Get().States_WeaponsBlocked);
	Player->GetAbilitySystemComponent()->RemoveLooseGameplayTag(FGameplayTags::Get().States_AimBlocked);
}

void UMeleeDashAbility::RemoveDelegates()
{
	if(MeleeDashTaskAnim)
	{
		MeleeDashTaskAnim.Get()->OnCompleted.RemoveAll(this);
		MeleeDashTaskAnim.Get()->OnCancelled.RemoveAll(this);
		MeleeDashTaskAnim.Get()->OnInterrupted.RemoveAll(this);
	}
}

FVector UMeleeDashAbility::MakeAutoAimSweep(FVector _Direction)
{
	FVector ToReturn = FVector::ZeroVector;

	FVector PlayerLocation = Player->GetActorLocation();
	FVector ForwardVector = Player->GetActorForwardVector();
	TArray<float> Angles = {-AngleDeviation,
		-AngleDeviation/1.25f,
		-AngleDeviation/1.5f,
		-AngleDeviation/2,
		-AngleDeviation/2.50f,
		-AngleDeviation/3.25f,
		-AngleDeviation/4,
		0,
		AngleDeviation/4,
		AngleDeviation/3.25f,
		AngleDeviation/2.50f,
		AngleDeviation/2,
		AngleDeviation/1.5f,
		AngleDeviation/1.25f,
		AngleDeviation};
	FVector HitLocation = FVector::ZeroVector;
	TMap<AEnemyBase*, float> EnemyDistances;

	for (float Angle : Angles)
	{
		// Calculate the direction vector for the given angle
		float RadAngle = FMath::DegreesToRadians(Angle);
		float XComp = (cos(RadAngle) * _Direction.X) - (sin(RadAngle) * _Direction.Y);
		float YComp = (sin(RadAngle) * _Direction.X) + (cos(RadAngle) * _Direction.Y);
		FVector Direction = FVector(XComp, YComp, 0).GetSafeNormal();

		// Perform the line trace
		FHitResult OutHit;
		FCollisionQueryParams CollisionParams;
		CollisionParams.AddIgnoredActor(Player);

		bool IsHit = GetWorld()->LineTraceSingleByChannel(
			OutHit,
			PlayerLocation,
			PlayerLocation + Direction * DistanceToCheck,
			ECC_GameTraceChannel1,
			CollisionParams
		);
		// Draw debug line
		FColor LineColor = IsHit ? FColor::Red : FColor::Green;

		/*
		DrawDebugLine(
			GetWorld(),
			PlayerLocation,
			PlayerLocation + Direction * DistanceToCheck,
			LineColor,
			false,
			1.0f,
			0,
			1.0f
		);
		*/
		// Check if the hit actor is an enemy
		if (IsHit && OutHit.GetActor()->IsA(AEnemyBase::StaticClass()))
		{
			EnemyDistances.Add(Cast<AEnemyBase>(OutHit.GetActor()), OutHit.Distance);
		}
	}
	float FinalDistance = DistanceToCheck;
	for(TTuple<AEnemyBase*, float> TupleToCheck : EnemyDistances)
	{
		if(TupleToCheck.Value < FinalDistance && TupleToCheck.Key->GetAbilitySystemComponent()->HasMatchingGameplayTag(FGameplayTags::Get().States_Cooked))
		{
			FinalDistance = TupleToCheck.Value;
			ToReturn = TupleToCheck.Key->GetActorLocation() - PlayerLocation;
		}
	}
	
	return ToReturn;
}


