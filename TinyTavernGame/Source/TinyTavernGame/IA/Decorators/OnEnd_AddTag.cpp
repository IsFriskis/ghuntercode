// TinyTavern


#include "OnEnd_AddTag.h"

#include "AIController.h"
#include "TinyTavernGame/Character/Enemy/EnemyBaseClass/EnemyBase.h"

UOnEnd_AddTag::UOnEnd_AddTag()
{
	bNotifyProcessed = true;
	bNotifyDeactivation = true;
	NodeName = "On end add tag";
}

void UOnEnd_AddTag::OnNodeProcessed(FBehaviorTreeSearchData& SearchData, EBTNodeResult::Type& NodeResult)
{
	Super::OnNodeProcessed(SearchData, NodeResult);
	AEnemyBase* Enemy = Cast<AEnemyBase>(SearchData.OwnerComp.GetAIOwner()->GetPawn());
	UAbilitySystemComponent* ASC = Enemy->GetAbilitySystemComponent();

	ASC->AddLooseGameplayTag(Tag_To_Add);
}


bool UOnEnd_AddTag::CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const
{
	return true;
}
