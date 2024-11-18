// TinyTavern


#include "BTT_WaveAttack.h"

#include "AIController.h"
#include "Abilities/GameplayAbilityTypes.h"
#include "TinyTavernGame/TT_GameplayTags.h"
#include "TinyTavernGame/Character/Enemy/EnemyBaseClass/EnemyAAIController.h"
#include "TinyTavernGame/Character/Enemy/EnemyBaseClass/EnemyBase.h"

UBTT_WaveAttack::UBTT_WaveAttack(FObjectInitializer const& ObjectInitializer)
{
	bNotifyTick = true;
	NodeName = "WaveAttack";
}

EBTNodeResult::Type UBTT_WaveAttack::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	Enemy = OwnerComp.GetAIOwner()->GetPawn<AEnemyBase>();
	if(JumpTimer>0)
	{
		GetWorld()->GetTimerManager().SetTimer(
		TimerHandle, // handle to cancel timer at a later time
		this, // the owning object
		&UBTT_WaveAttack::ActivateAbility, // function to call on elapsed
		JumpTimer, // float delay until elapsed
		true,-1);
	}
	return EBTNodeResult::InProgress;
}

void UBTT_WaveAttack::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickTask(OwnerComp, NodeMemory, DeltaSeconds);


	if(CanWave)
	{
		FGameplayEventData Payload;
		Enemy.Get()->GetAbilitySystemComponent()->HandleGameplayEvent(FGameplayTags::Get().States_Jump, &Payload);
		CanWave = false;
	}
}

void UBTT_WaveAttack::EndTask(const FAbilityEndedData& Data)
{
}

void UBTT_WaveAttack::ActivateAbility()
{
	
	CanWave = true;
}
