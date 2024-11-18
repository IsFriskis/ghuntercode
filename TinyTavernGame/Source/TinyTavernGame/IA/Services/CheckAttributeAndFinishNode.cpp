// TinyTavern


#include "CheckAttributeAndFinishNode.h"

#include "AIController.h"
#include "BehaviorTree/BTTaskNode.h"
#include "TinyTavernGame/AbilitySystem/AttributeSet/TT_AttributeSet.h"
#include "TinyTavernGame/Character/Enemy/EnemyBaseClass/EnemyBase.h"

void UCheckAttributeAndFinishNode::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);

	TArray<FGameplayAttribute> Attributes;
	Enemy = Cast<AEnemyBase>(OwnerComp.GetAIOwner()->GetPawn());
	Enemy->GetAbilitySystemComponent()->GetAllAttributes(Attributes);
	UTT_AttributeSet* EnemyATS = CastChecked<UTT_AttributeSet>(Enemy->GetAttributeSet());
	float CurrentValue = 0;
	float TargetValue = 0;
	for (FGameplayAttribute attribute : Attributes)
	{
		if (attribute == Attribute)
		{
			CurrentValue  = attribute.GetNumericValue(EnemyATS);
			TargetValue =  (MaxAttribute.GetGameplayAttributeData(EnemyATS)->GetBaseValue() * Percentage)/100;
			const UBTNode* ActiveNode = OwnerComp.GetActiveNode();	
			if(TargetValue < CurrentValue)
			{
				//ActiveNode = OwnerComp.GetActiveNode();				
				const UBTTaskNode* TaskNode = Cast<UBTTaskNode>(ActiveNode);
				TaskNode->FinishLatentTask(OwnerComp,EBTNodeResult::Succeeded);
			}
		}
	}


	

}
