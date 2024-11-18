// TinyTavern


#include "BTT_RollDice.h"

#include "AIController.h"
#include "TinyTavernGame/Character/Enemy/EnemyBaseClass/EnemyBase.h"


UBTT_RollDice::UBTT_RollDice(FObjectInitializer const& ObjectInitializer)
{
	bNotifyTick = false;
}

EBTNodeResult::Type UBTT_RollDice::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	AEnemyBase* Enemy = Cast<AEnemyBase>(OwnerComp.GetAIOwner()->GetPawn());
	float Diceresult = FMath::RandRange(0,99);
	if (Diceresult < TruePercentage)
	{
		Enemy->GetAbilitySystemComponent()->AddLooseGameplayTag(TrueTag);
	}
	else
	{
		Enemy->GetAbilitySystemComponent()->AddLooseGameplayTag(FalseTag);
	}
	return EBTNodeResult::Succeeded;
}
