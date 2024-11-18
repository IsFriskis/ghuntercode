// TinyTavern


#include "CheckAttributes.h"

#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "TinyTavernGame/TT_GameplayTags.h"
#include "TinyTavernGame/AbilitySystem/AttributeSet/TT_AttributeSet.h"
#include "TinyTavernGame/Character/Enemy/EnemyBaseClass/EnemyBase.h"

void UCheckAttributes::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);
	TArray<FGameplayAttribute> Attributes;
	Enemy = Cast<AEnemyBase>(OwnerComp.GetAIOwner()->GetPawn());
	Enemy->GetAbilitySystemComponent()->GetAllAttributes(Attributes);
	UTT_AttributeSet* EnemyATS = CastChecked<UTT_AttributeSet>(Enemy->GetAttributeSet());
	float CurrentValue = 0;
	for (FGameplayAttribute _Attribute : Attributes)
	{
		if (_Attribute == Attribute)
		{
			CurrentValue  = _Attribute.GetNumericValue(EnemyATS);
		}
	}

	OwnerComp.GetBlackboardComponent()->SetValueAsFloat(AttributeKey.SelectedKeyName,CurrentValue);
	

	
	// for	(FTagsToValue tag :  TagsAndValues)
	// {
	// 	if (CurrentValue > tag.Value)
	// 	{
	// 		AbilitySystemComponent->AddLooseGameplayTag(tag.Tag);
	// 	}
	// }
	
	
}
