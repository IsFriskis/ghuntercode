// TinyTavern


#include "BurnEnemy.h"

#include "TurnToAshesTask.h"
#include "TinyTavernGame/Character/TTAnimNotify.h"
#include "TinyTavernGame/Character/Enemy/EnemyBaseClass/EnemyBase.h"
#include "TinyTavernGame/Settings/TT_GameInstance.h"


void UBurnEnemy::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
                                 const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	Enemy = Cast<AEnemyBase>(ActorInfo->OwnerActor);

	Enemy.Get()->SetActorEnableCollision(false);
	if(Enemy.Get()->BurnMaterial)
	{
		BurnMaterial = Enemy.Get()->BurnMaterial;
	}
	if(Enemy.Get()->AnimMontage)
	{
		AnimMontage = Enemy.Get()->AnimMontage;
	}
	if(Enemy.Get()->ExplosionEffect)
	{
		ExplosionEffect = Enemy.Get()->ExplosionEffect;
	}
	MontageTask = MontageTask->CreatePlayMontageAndWaitProxy(this, "BurnMontage", AnimMontage);
	if (AnimMontage != NULL)
	{
		UTTAnimNotify* Notify = Cast<UTTAnimNotify>(AnimMontage->Notifies[0].Notify.Get());
		if(Notify)
		{
			Notify->OnNotified.AddDynamic(this, &UBurnEnemy::ExecuteTurnToAshes);
		}
		MontageTask.Get()->ReadyForActivation();
	}	

	TObjectPtr<UTT_GameInstance> ttGameInstance = Cast<UTT_GameInstance>(GetWorld()->GetGameInstance());
	if (ttGameInstance)
	{
		ttGameInstance.Get()->OvercookedEnemy();
	}
}

void UBurnEnemy::EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
	const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled)
{
	if (AnimMontage != NULL)
	{
		UTTAnimNotify* Notify = Cast<UTTAnimNotify>(AnimMontage->Notifies[0].Notify.Get());
		if(Notify)
		{
			Notify->OnNotified.RemoveAll(this);
		}
	}
	if(TurnToAshesTask)
	{
		TurnToAshesTask->OnFullMaterialBurnChange.RemoveAll(this);
	}
	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
}

void UBurnEnemy::CancelAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
	const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateCancelAbility)
{
	if (AnimMontage != NULL) 
	{

		UTTAnimNotify* Notify = Cast<UTTAnimNotify>(AnimMontage->Notifies[0].Notify.Get());
		if(Notify)
		{
			Notify->OnNotified.RemoveAll(this);
		}
		
		if(TurnToAshesTask)
		{
			TurnToAshesTask->OnFullMaterialBurnChange.RemoveAll(this);
		}

	}
	Super::CancelAbility(Handle, ActorInfo, ActivationInfo, bReplicateCancelAbility);
}

void UBurnEnemy::ExecuteTurnToAshes()
{
	UTTAnimNotify* Notify = Cast<UTTAnimNotify>(AnimMontage->Notifies[0].Notify.Get());
	if(Notify)
	{
		Notify->OnNotified.RemoveAll(this);
	}
	TurnToAshesTask = TurnToAshesTask->ExecuteTurnToAshesTask(this);
	TurnToAshesTask->ReadyForActivation();
	TurnToAshesTask->OnFullMaterialBurnChange.AddUObject(this, &UBurnEnemy::EndLogic);
}

void UBurnEnemy::EndLogic()
{
	TurnToAshesTask->OnFullMaterialBurnChange.RemoveAll(this);
	GetActorInfo().AvatarActor->Destroy();
	EndAbility(GetCurrentAbilitySpecHandle(), GetCurrentActorInfo(), GetCurrentActivationInfo(), false, false);
}
