// TinyTavern


#include "OnEnd_RemoveTag.h"

#include "AIController.h"
#include "TinyTavernGame/Character/Enemy/EnemyBaseClass/EnemyBase.h"


UOnEnd_RemoveTag::UOnEnd_RemoveTag()
{
	bNotifyProcessed = true;
	bNotifyDeactivation = true;
	NodeName = "On end Remove tag";
}

void UOnEnd_RemoveTag::OnNodeProcessed(FBehaviorTreeSearchData& SearchData, EBTNodeResult::Type& NodeResult)
{
	Super::OnNodeProcessed(SearchData, NodeResult);
	AEnemyBase* Enemy = Cast<AEnemyBase>(SearchData.OwnerComp.GetAIOwner()->GetPawn());
	UAbilitySystemComponent* ASC = Enemy->GetAbilitySystemComponent();

	ASC->RemoveLooseGameplayTag(Tag_To_Add);
}


bool UOnEnd_RemoveTag::CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const
{
	return true;
}