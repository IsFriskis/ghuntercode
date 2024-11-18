// TinyTavern


#include "MeleeDashTask.h"

#include "MeleeDashAbility.h"
#include "TinyTavernGame/TinyTavernUtils.h"
#include "TinyTavernGame/TT_GameplayTags.h"
#include "TinyTavernGame/Actor/Destroyable.h"
#include "TinyTavernGame/Character/Enemy/EnemyBaseClass/EnemyBase.h"
#include "TinyTavernGame/Player/TTPlayerController.h"

UMeleeDashTask::UMeleeDashTask(const FObjectInitializer& ObjectInitializer)
{
	bTickingTask = true;
}


TObjectPtr<UMeleeDashTask> UMeleeDashTask::ExecuteDashTask(UGameplayAbility* OwningAbility)
{
	TObjectPtr<UMeleeDashTask> MeleeDashTask = NewAbilityTask<UMeleeDashTask>(OwningAbility);

	MeleeDashTask.Get()->MeleeDashAbility = Cast<UMeleeDashAbility>(OwningAbility);

	return MeleeDashTask;
}

void UMeleeDashTask::TickTask(float DeltaTime)
{
	Super::TickTask(DeltaTime);
	
	
	FVector MeleeCollisionPosition = MeleeDashAbility->Player.Get()->GetActorLocation() + MeleeDashAbility->Player.Get()->GetActorForwardVector() * MeleeDashAbility.Get()->HitboxLength;
	
	TArray<FHitResult> Hits;
	FCollisionShape CollisionShape;
	CollisionShape.SetBox(FVector3f(MeleeDashAbility.Get()->HitboxLength, MeleeDashAbility.Get()->HitboxWidth , MeleeDashAbility.Get()->HitboxHeight));
	FCollisionQueryParams QueryParams;
	QueryParams.AddIgnoredActor(MeleeDashAbility->Player.Get());
	
#if WITH_EDITOR
	DrawDebugBox(
		GetWorld(),
		MeleeCollisionPosition,
		CollisionShape.GetExtent(),
		MeleeDashAbility->Player.Get()->GetActorQuat(),
		FColor::Red,
		false,
		-1,
		0,
		1);
#endif
	
	bool isHit = GetWorld()->SweepMultiByChannel(Hits, MeleeCollisionPosition, MeleeCollisionPosition, MeleeDashAbility.Get()->Player.Get()->GetActorQuat(), MeleeDashAbility.Get()->CollisionChannel, CollisionShape, QueryParams);
  	if (isHit)
	{
		for (int i = 0; i < Hits.Num(); i++)
		{
			//If collided with enemy, send type of cut, and amount of stun
			if (AEnemyBase* EnemyBase = Cast<AEnemyBase>(Hits[i].GetActor()))
			{
				TObjectPtr<UAbilitySystemComponent> TargetAsc = EnemyBase->GetAbilitySystemComponent();
				if (!TargetAsc)
				{
					return;	
				}

				if (TargetAsc->HasMatchingGameplayTag(FGameplayTag::RequestGameplayTag(FName("States.Vulnerable"))))
				{
					float DistanceToEnemy = FVector::Distance(MeleeDashAbility.Get()->Player->GetActorLocation(), EnemyBase->GetActorLocation());
					if (NearestEnemy == nullptr || DistanceToEnemy <= NearestEnemyDistance)
					{
						NearestEnemy = EnemyBase;
						NearestEnemyDistance = DistanceToEnemy;
					}
				}
				
				//MeleeDashAbility.Get()->MeleeDashTaskAnim.Get()->OnCompleted.Broadcast();
				EndTask();
			}

			if(ADestroyable* Destroyable = Cast<ADestroyable>(Hits[i].GetActor()))
			{
				Destroyable->Destroy();
			}
		}
		TObjectPtr<UAbilitySystemComponent> PlayerAsc = MeleeDashAbility->Player->GetAbilitySystemComponent();
		if (NearestEnemy != nullptr)
		{
			FGameplayEventData GameplayEvent;
			GameplayEvent.Instigator = MeleeDashAbility.Get()->Player.Get();

			FFinisherData* FinisherPayload = new FFinisherData();
			FinisherPayload->EnemyToKill = NearestEnemy;
			FinisherPayload->PlayerActor = MeleeDashAbility.Get()->Player.Get();
			FinisherPayload->MeleeWeaponLevel = static_cast<int>(MeleeDashAbility.Get()->EventMagnitude);


			FGameplayAbilityTargetDataHandle Handle;
			Handle.Data.Add(TSharedPtr<FGameplayAbilityTargetData>(FinisherPayload));
			GameplayEvent.TargetData = Handle;

			PlayerAsc.Get()->HandleGameplayEvent(FGameplayTags::Get().Events_Player_Finisher, &GameplayEvent);
			//MeleeDashAbility.Get()->MeleeDashTaskAnim.Get()->OnCompleted.Broadcast();
			EndTask();
		}
		
	}
}

