// TinyTavern


#include "BTT_Charge.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "TinyTavernGame/TT_GameplayTags.h"
#include "TinyTavernGame/Character/Enemy/EnemyBaseClass/EnemyBase.h"

UBTT_Charge::UBTT_Charge(FObjectInitializer const& ObjectInitializer)
{
	bNotifyTick = true;
}

EBTNodeResult::Type UBTT_Charge::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	Enemy = Cast<AEnemyBase>(OwnerComp.GetAIOwner()->GetPawn());
	if(Enemy)
	{
		TObjectPtr<UAbilitySystemComponent> EnemyASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(Enemy);
		
		FGameplayEventData Payload;
		Payload.Instigator = Enemy;
		Payload.OptionalObject = OwnerComp.GetAIOwner()->GetBlackboardComponent()->GetValueAsObject("playerReference");
		EnemyASC.Get()->HandleGameplayEvent(FGameplayTags::Get().Events_Enemy_Charge, &Payload);
		EnemyASC->OnAbilityEnded.AddUObject(this, &UBTT_Charge::EndTask);
	}
	return EBTNodeResult::InProgress;
}

void UBTT_Charge::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickTask(OwnerComp, NodeMemory, DeltaSeconds);

	AEnemyBase* EnemyBase = Cast<AEnemyBase>(OwnerComp.GetAIOwner()->GetPawn());
	if(EnemyBase)
	{
		FGameplayTagContainer EnemyTags;
		EnemyBase->GetAbilitySystemComponent()->GetOwnedGameplayTags(EnemyTags);

		if(!EnemyTags.HasTagExact(FGameplayTags::Get().Events_Enemy_Charge))
		{
			
			EnemyBase->GetAbilitySystemComponent()->RemoveLooseGameplayTag(FGameplayTags::Get().States_Charge);
			FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
		}
	}
	
}

void UBTT_Charge::EndTask(const FAbilityEndedData& Data)
{
	Cast<AEnemyBase>(Enemy)->GetAbilitySystemComponent()->RemoveLooseGameplayTag(FGameplayTags::Get().States_Charge);
}
