// TinyTavern


#include "ErraticDodgeAbility.h"
#include "NavigationSystem.h"
#include "Abilities/Tasks/AbilityTask_PlayMontageAndWait.h"
#include "TinyTavernGame/AbilitySystem/LibraryTask/TaskGamePlayAbilityEnded.h"
#include "TinyTavernGame/Character/Enemy/EnemyBaseClass/EnemyBase.h"

void UErraticDodgeAbility::ActivateAbility(const FGameplayAbilitySpecHandle Handle,
                                           const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo,
                                           const FGameplayEventData* TriggerEventData)
{
	float Result =  FMath::RandRange(1,3);
	//Right
	if(Result == 1)
	{
		float angle = 45;
		//float angle = PI/4;
		// float cosAngle = FMath::Cos(angle);
		// float sinAngle = FMath::Sin(angle);
		// TargetDirection = FVector(ForwardDirection.X * cosAngle - ForwardDirection.Y * sinAngle,
		// 	ForwardDirection.X * sinAngle + ForwardDirection.Y * cosAngle,
		// 	ForwardDirection.Z);
		Character = Cast<ACharacter>( ActorInfo->OwnerActor.Get());
		FRotator NewRotation = Character->GetActorRotation();
		NewRotation.Yaw += angle;
		Character->SetActorRotation(NewRotation);
		UAbilityTask_PlayMontageAndWait* Montage = UAbilityTask_PlayMontageAndWait::CreatePlayMontageAndWaitProxy(this,"ErraticMontage",ErraticDodgeMontage);
		Montage->OnCompleted.AddDynamic(this,&UErraticDodgeAbility::OnCompletedAnim);
		Montage->ReadyForActivation();
	}
	//Left
	else if(Result == 2)
	{
		float angle = -45;
		//float angle = -PI/4;
		// float cosAngle = FMath::Cos(angle);
		// float sinAngle = FMath::Sin(angle);
		// TargetDirection = FVector(ForwardDirection.X * cosAngle - ForwardDirection.Y * sinAngle,
		// 	ForwardDirection.X * sinAngle + ForwardDirection.Y * cosAngle,
		// 	ForwardDirection.Z);
		//
		Character = Cast<ACharacter>( ActorInfo->OwnerActor.Get());
		FRotator NewRotation = Character->GetActorRotation();
		NewRotation.Yaw += angle;
		Character->SetActorRotation(NewRotation);
		UAbilityTask_PlayMontageAndWait* Montage = UAbilityTask_PlayMontageAndWait::CreatePlayMontageAndWaitProxy(this,"ErraticMontage",ErraticDodgeMontage);
		Montage->OnCompleted.AddDynamic(this,&UErraticDodgeAbility::OnCompletedAnim);
		Montage->ReadyForActivation();
	}
	//Forward
	else
	{
		Character = Cast<ACharacter>( ActorInfo->OwnerActor.Get());
		UAbilityTask_PlayMontageAndWait* Montage = UAbilityTask_PlayMontageAndWait::CreatePlayMontageAndWaitProxy(this,"ErraticMontage",ErraticDodgeMontage);
		Montage->OnCompleted.AddDynamic(this,&UErraticDodgeAbility::OnCompletedAnim);
		Montage->ReadyForActivation();
		//TargetDirection = ForwardDirection;
	}
	
// 	FVector ProjectedLocation = EnemyLocation + (TargetDirection * Distance);
// 	
// 	FNavLocation NavLocation;
// 	FVector Extent = FVector(0.f, 0.0f, 0.0f);
// 	bool isInNavMesh = NavSystem->ProjectPointToNavigation(ProjectedLocation,NavLocation,Extent);
// 	if(isInNavMesh)
// 	{
// #if WITH_EDITOR
// 		DrawDebugBox(
// 			GetWorld(),
// 			ProjectedLocation,
// 			FVector(10, 10, 10),
// 			ActorInfo->OwnerActor->GetActorQuat(),
// 			FColor::Red,
// 			true,
// 			15.0f);
//
// 		DrawDebugLine(GetWorld(),
// 			EnemyLocation,
// 			ProjectedLocation,
// 			FColor::Red,
// 			true,
// 			15.0f,
// 			0,
// 			5);
// #endif
		
		/*ActorInfo->OwnerActor->SetActorLocation(ProjectedLocation);
	}
	
	EnemyLocation = ActorInfo->OwnerActor->GetActorLocation();
	ForwardDirection = ActorInfo->OwnerActor->GetActorForwardVector();
	Result =  FMath::RandRange(1,3);
	//Right
	if(Result == 1)
	{
		float angle = PI/4;
		// float cosAngle = FMath::Cos(angle);
		// float sinAngle = FMath::Sin(angle);
		// TargetDirection = FVector(ForwardDirection.X * cosAngle - ForwardDirection.Y * sinAngle,
		// 	ForwardDirection.X * sinAngle + ForwardDirection.Y * cosAngle,
		// 	ForwardDirection.Z);
		ACharacter* Character = Cast<ACharacter>( ActorInfo->OwnerActor.Get());
		FRotator NewRotation = Character->GetActorRotation();
		NewRotation.Yaw += angle;
		Character->SetActorRotation(NewRotation);
		Character->PlayAnimMontage(ErraticDodgeMontage);
		
	}
	//Left
	else if(Result == 2)
	{
		float angle = -PI/4;
		// float cosAngle = FMath::Cos(angle);
		// float sinAngle = FMath::Sin(angle);
		// TargetDirection = FVector(ForwardDirection.X * cosAngle - ForwardDirection.Y * sinAngle,
		// 	ForwardDirection.X * sinAngle + ForwardDirection.Y * cosAngle,
		// 	ForwardDirection.Z);
		ACharacter* Character = Cast<ACharacter>( ActorInfo->OwnerActor.Get());
		FRotator NewRotation = Character->GetActorRotation();
		NewRotation.Yaw += angle;
		Character->SetActorRotation(NewRotation);
		Character->PlayAnimMontage(ErraticDodgeMontage);
		
	}
	//Forward
	else
	{
		ACharacter* Character = Cast<ACharacter>( ActorInfo->OwnerActor.Get());
		Character->PlayAnimMontage(ErraticDodgeMontage);
		//TargetDirection = ForwardDirection;
	}
	
	ProjectedLocation = EnemyLocation + (ForwardDirection * Distance);
	isInNavMesh = NavSystem->ProjectPointToNavigation(ProjectedLocation,NavLocation,Extent);

	if(isInNavMesh)
	{
#if WITH_EDITOR
		DrawDebugBox(
			GetWorld(),
			ProjectedLocation,
			FVector(10, 10, 10),
			ActorInfo->OwnerActor->GetActorQuat(),
			FColor::Red,
			true,
			15.0f);

		DrawDebugLine(GetWorld(),
			EnemyLocation,
			ProjectedLocation,
			FColor::Red,
			true,
			15.0f,
			0,
			5);
#endif
		ActorInfo->OwnerActor->SetActorLocation(ProjectedLocation);*/
	//}
}

void UErraticDodgeAbility::OnCompletedAnim()
{

	FVector EnemyLocation = Character->GetActorLocation();
	FVector ForwardDirection = Character->GetActorForwardVector();
	float Result =  FMath::RandRange(1,3);
	//Right
	if(Result == 1)
	{
		float angle = 45;
		//float angle = PI/4;
		// float cosAngle = FMath::Cos(angle);
		// float sinAngle = FMath::Sin(angle);
		// TargetDirection = FVector(ForwardDirection.X * cosAngle - ForwardDirection.Y * sinAngle,
		// 	ForwardDirection.X * sinAngle + ForwardDirection.Y * cosAngle,
		// 	ForwardDirection.Z);
		
		FRotator NewRotation = Character->GetActorRotation();
		NewRotation.Yaw += angle;
		Character->SetActorRotation(NewRotation);
		UAbilityTask_PlayMontageAndWait* Montage = UAbilityTask_PlayMontageAndWait::CreatePlayMontageAndWaitProxy(this,"ErraticMontage",ErraticDodgeMontage);
		Montage->OnCompleted.AddDynamic(this,&UErraticDodgeAbility::OnEndTask);
		Montage->ReadyForActivation();
	}
	//Left
	else if(Result == 2)
	{
		float angle = -45;
		//float angle = -PI/4;
		// float cosAngle = FMath::Cos(angle);
		// float sinAngle = FMath::Sin(angle);
		// TargetDirection = FVector(ForwardDirection.X * cosAngle - ForwardDirection.Y * sinAngle,
		// 	ForwardDirection.X * sinAngle + ForwardDirection.Y * cosAngle,
		// 	ForwardDirection.Z);
		//
		FRotator NewRotation = Character->GetActorRotation();
		NewRotation.Yaw += angle;
		Character->SetActorRotation(NewRotation);
		UAbilityTask_PlayMontageAndWait* Montage = UAbilityTask_PlayMontageAndWait::CreatePlayMontageAndWaitProxy(this,"ErraticMontage",ErraticDodgeMontage);
		Montage->OnCompleted.AddDynamic(this,&UErraticDodgeAbility::OnEndTask);
		Montage->ReadyForActivation();
	}
}

void UErraticDodgeAbility::OnEndTask()
{
	EndAbility(CurrentSpecHandle,CurrentActorInfo,CurrentActivationInfo,true,true);
}

