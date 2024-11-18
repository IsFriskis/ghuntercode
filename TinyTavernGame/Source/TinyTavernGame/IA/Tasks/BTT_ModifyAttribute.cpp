// TinyTavern


#include "BTT_ModifyAttribute.h"

#include "AIController.h"
#include "TinyTavernGame/Character/Enemy/EnemyBaseClass/EnemyBase.h"

UBTT_ModifyAttribute::UBTT_ModifyAttribute(FObjectInitializer const& ObjectInitializer)
{
	bNotifyTick = false;
}

EBTNodeResult::Type UBTT_ModifyAttribute::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	AEnemyBase* Enemy = Cast<AEnemyBase>(OwnerComp.GetAIOwner()->GetPawn());
	UAbilitySystemComponent* ABS =  Enemy->GetAbilitySystemComponent();
	ABS->SetNumericAttributeBase(Attribute,Value);
	ABS->AddLooseGameplayTag(AddTag);
	ABS->RemoveLooseGameplayTag(RemoveTag);

	return EBTNodeResult::Succeeded;
}
