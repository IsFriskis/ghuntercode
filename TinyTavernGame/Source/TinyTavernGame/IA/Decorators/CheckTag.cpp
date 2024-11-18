// TinyTavern


#include "CheckTag.h"

#include "AIController.h"
#include "TinyTavernGame/Character/Enemy/EnemyBaseClass/EnemyBase.h"

bool UCheckTag::CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const
{
	AEnemyBase* Enemy = Cast<AEnemyBase>(OwnerComp.GetAIOwner()->GetPawn());
	UAbilitySystemComponent* ASC = Enemy->GetAbilitySystemComponent();
	bool hasTag = false;
	if(ASC)
	{
		for(FGameplayTag tag: TagsToCheck)
		{
			
			hasTag = ASC->HasMatchingGameplayTag(tag);
			if(hasTag)
			{
				return hasTag;
			}
		}
	}
	return hasTag;
}
