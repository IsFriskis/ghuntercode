// TinyTavern


#include "RemoveTagToEndNode.h"

#include "AIController.h"
#include "BehaviorTree/BTTaskNode.h"
#include "TinyTavernGame/Character/Enemy/EnemyBaseClass/EnemyBase.h"

void URemoveTagToEndNode::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);

	Enemy = Cast<AEnemyBase>(OwnerComp.GetAIOwner()->GetPawn());
	const UBTNode* ActiveNode = OwnerComp.GetActiveNode();
	if (!Enemy->GetAbilitySystemComponent()->HasMatchingGameplayTag(Tag))
		
	{
		const UBTTaskNode* TaskNode = Cast<UBTTaskNode>(ActiveNode);
		TaskNode->FinishLatentTask(OwnerComp,EBTNodeResult::Succeeded);
		
	}

}
