// TinyTavern


#include "MeleeDashRotate.h"

#include "MeleeDashAbility.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "TinyTavernGame/Player/PlayerCharacter.h"

UMeleeDashRotate::UMeleeDashRotate()
{
	bTickingTask = true;
}

TObjectPtr<UMeleeDashRotate> UMeleeDashRotate::ExecuteDashRotateTask(UGameplayAbility* OwningAbility)
{
	TObjectPtr<UMeleeDashRotate> MeleeDashRotate = NewAbilityTask<UMeleeDashRotate>(OwningAbility);
	MeleeDashRotate->MeleeDashAbility = Cast<UMeleeDashAbility>(OwningAbility);
	MeleeDashRotate->TimeToRotate = MeleeDashRotate->MeleeDashAbility->TimeToTurn;
	MeleeDashRotate->Player = Cast<APlayerCharacter>(MeleeDashRotate->MeleeDashAbility->Player.Get());

	
	
	if (MeleeDashRotate->Player->GetCharacterMovement()->Velocity == FVector::ZeroVector)
    {
		MeleeDashRotate->TargetVector = MeleeDashRotate->Player->GetActorForwardVector();
    }
	else
    {
		MeleeDashRotate->TargetVector = MeleeDashRotate->Player->GetCharacterMovement()->Velocity;
		MeleeDashRotate->TargetVector.Normalize();
    }
	FVector ToIf = MeleeDashRotate->TargetVector;
	ToIf.Normalize();
	FVector ToIf2 = MeleeDashRotate->Player->GetActorForwardVector();
	ToIf2.Normalize();

	if(ToIf.Equals(ToIf2, 1))
	{
		MeleeDashRotate->TargetVector = MeleeDashRotate->Player->GetActorForwardVector();
	}

	if (MeleeDashRotate->Player && MeleeDashRotate->Player->GetCharacterMovement())
	{
		MeleeDashRotate->Player->GetCharacterMovement()->DisableMovement();
	}
	
	return  MeleeDashRotate;
}

void UMeleeDashRotate::TickTask(float DeltaTime)
{
	Super::TickTask(DeltaTime);

	if(TargetVector == Player->GetActorForwardVector())
	{
		FinishTask();
	}
	if (Player)
	{
		if (!Blend.IsComplete())
		{
			Blend.Update(DeltaTime/MeleeDashAbility->TimeToTurn);
			
			FRotator CurrentRotation = Player->GetActorRotation();
			CurrentRotation.Pitch = 0;
			CurrentRotation.Roll = 0;

			FQuat CurrentQuat = FQuat(CurrentRotation);
			FQuat TargetQuat = FQuat(TargetVector.Rotation());

			// Slerp interpolates between CurrentQuat and TargetQuat based on Alpha (0.0 to 1.0)
			FQuat InterpolatedQuat = FQuat::Slerp(CurrentQuat, TargetQuat, Blend.GetAlpha());

			// Convert back to FRotator for use
			FRotator NewRotation = InterpolatedQuat.Rotator();

			NewRotation.Pitch = 0;
			NewRotation.Roll = 0;

			Player->SetActorRotation(NewRotation);
		}
		else
		{
			FinishTask();
		}
	}
}

void UMeleeDashRotate::FinishTask()
{
	if (Player->GetCharacterMovement())
	{
		Player->GetCharacterMovement()->SetMovementMode(MOVE_Walking);
	}
	OnTaskEnded.Broadcast();
	EndTask();
}
