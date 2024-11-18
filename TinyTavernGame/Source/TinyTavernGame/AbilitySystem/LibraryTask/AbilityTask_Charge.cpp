// TinyTavern


#include "AbilityTask_Charge.h"

#include "AbilitySystemComponent.h"
#include "Abilities/Tasks/AbilityTask_PlayMontageAndWait.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "TinyTavernGame/TT_GameplayTags.h"
#include "TinyTavernGame/Character/Enemy/EnemyBaseClass/EnemyAAIController.h"
#include "TinyTavernGame/Player/PlayerCharacter.h"


UAbilityTask_Charge::UAbilityTask_Charge(const FObjectInitializer& ObjectInitializer)
{
	bTickingTask = true;

}

UAbilityTask_Charge* UAbilityTask_Charge::ChargeAbilityTask(UGameplayAbility* OwningAbility, FName TaskInstanceName,
                                                            FVector TargetPoint, bool ApplyHitEffects, bool MoveActors,float Radius,bool ShowDebug,float Force, TArray<TSubclassOf<AActor>> IgnoreActors, bool DestroyActors, TArray<TSubclassOf<AActor>> ActorsToDestroy)
{
	UAbilityTask_Charge* ChargeObj = NewAbilityTask<UAbilityTask_Charge>(OwningAbility, TaskInstanceName);
	ChargeObj->TargetPoint = TargetPoint;
	ChargeObj->ApplyHitEffects = ApplyHitEffects;
	ChargeObj->MoveActors = MoveActors;
	ChargeObj->Radius = Radius;
	ChargeObj->ShowDebug = ShowDebug;
	ChargeObj->Force = Force;
	ChargeObj->IgnoreActors = IgnoreActors;
	ChargeObj->DestroyActors = DestroyActors;
	ChargeObj->ActorsToDestroy = ActorsToDestroy;

	return ChargeObj;
	
}

void UAbilityTask_Charge::Activate()
{
	Super::Activate();

	UAbilitySystemComponent* ASC = AbilitySystemComponent.Get();
	AIController = Cast<AEnemyAAIController>(Cast<AEnemyBase>(ASC->GetOwnerActor())->AIController);
	Charger = ASC->GetOwnerActor();
	AIController->ReceiveMoveCompleted.AddDynamic(this ,&UAbilityTask_Charge::ChargeCompleted);
	IgnoreSelf.Add(Charger);
	Charge();
}
void UAbilityTask_Charge::TickTask(float DeltaTime)
{
	Super::TickTask(DeltaTime);
	OnSweepBegin();
}

void UAbilityTask_Charge::OnGameplayAbilityCancelled()
{
	
}

void UAbilityTask_Charge::ApplyDamageEffect(AActor* Actor, TSubclassOf<UGameplayEffect> GameplayEffect)
{
	
}
void UAbilityTask_Charge::Charge()
{
	AIController->MoveToLocation(TargetPoint);
}

void UAbilityTask_Charge::ChargeCompleted(FAIRequestID RequestID, EPathFollowingResult::Type Result)
{
	if(Result == EPathFollowingResult::Success)
	{
		AIController->ReceiveMoveCompleted.RemoveAll(this);
		OnCompleted.Broadcast(Collision);
		EndTask();
	}
}

void UAbilityTask_Charge::OnSweepBegin()
{

	TArray<FHitResult> OutHits;

	FCollisionShape ColSphere = FCollisionShape::MakeSphere(Radius);
	FVector SweepStart = Charger->GetActorLocation();
	FVector SweepEnd = SweepStart + 0.01f;
	
	FCollisionQueryParams QueryParams;

	QueryParams.AddIgnoredActors(IgnoreSelf);
	
	if(ShowDebug)
	{
		DrawDebugSphere(GetWorld(),Charger->GetActorLocation(),Radius,12,FColor::Red,false,.1f,0,0);
	}
	bool isHit = GetWorld()->SweepMultiByChannel(OutHits, SweepStart, SweepEnd, FQuat::Identity, ECollisionChannel::ECC_GameTraceChannel1, ColSphere, QueryParams);
	if(isHit)
	{
		for (auto& Hit : OutHits)
		{
			if(Hit.GetActor()->GetClass()->IsChildOf(ACharacter::StaticClass()))
			{
				if (DestroyActors)
				{
					for (int i=0 ; i < ActorsToDestroy.Num(); i++)
					{
						if(Hit.GetActor()->GetClass()->IsChildOf(ActorsToDestroy[i]))
					
						{
							AEnemyBase* DestroyActor = Cast<AEnemyBase>(Hit.GetActor());
							FGameplayEventData Payload;
							DestroyActor->GetAbilitySystemComponent()->HandleGameplayEvent(FGameplayTags::Get().States_BloodExplosion,&Payload);
						}
					}
				}
				if(MoveActors)
				{
					// TObjectPtr<ACharacter> Other =  Cast<ACharacter>(Hit.GetActor());
					// FVector Direction = (Charger->GetActorForwardVector() + Charger->GetActorRightVector()).GetSafeNormal();
					// Other->GetCharacterMovement()->AddImpulse(FVector(Force,Force,0),false);
					// Calcular la dirección del impacto

					TObjectPtr<ACharacter> Other = Cast<ACharacter>(Hit.GetActor());
					FVector ImpactDirection = (Hit.ImpactPoint - Charger->GetActorLocation()).GetSafeNormal();
					FVector ChargerForward = Charger->GetActorForwardVector();

					// Calcular el ángulo entre la dirección del impacto y el vector de avance del Charger
					float DotProduct = FVector::DotProduct(ImpactDirection, ChargerForward);
					float Angle = FMath::Acos(DotProduct);

					// Decidir la dirección del impulso basado en el ángulo
					FVector ImpulseDirection;
					if (FMath::Abs(Angle) <= FMath::DegreesToRadians(45.0f))
					{
						ImpulseDirection = ImpactDirection.RotateAngleAxis(45.0f, FVector::UpVector);
					}
					else
					{
						ImpulseDirection = ImpactDirection.RotateAngleAxis(-45.0f, FVector::UpVector);
					}

					// Aplicar el impulso en la dirección calculada
					Other->GetCharacterMovement()->AddImpulse(ImpulseDirection * Force, false);
				}
				
				if(ApplyHitEffects)
				{
					OnHitActor.Broadcast(Hit.GetActor());
				}
			}
		}
	}
}
