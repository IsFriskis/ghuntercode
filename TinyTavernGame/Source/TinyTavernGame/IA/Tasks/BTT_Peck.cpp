// TinyTavern


#include "BTT_Peck.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "AIController.h"
#include "TinyTavernGame/TT_GameplayTags.h"
#include "TinyTavernGame/AbilitySystem/Abilities/Enemies/PeckAbility.h"
#include "TinyTavernGame/Character/Enemy/EnemyBaseClass/EnemyBase.h"


UBTT_Peck::UBTT_Peck(FObjectInitializer const& ObjectInitializer)
{
	NodeName = TEXT("Peck");
}

EBTNodeResult::Type UBTT_Peck::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	TObjectPtr<AEnemyBase> EnemyBase = Cast<AEnemyBase>(OwnerComp.GetAIOwner()->GetPawn());
	if(EnemyBase)
	{
		TObjectPtr<UAbilitySystemComponent> TargetAsc = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(EnemyBase.Get());
		FGameplayEventData Payload;
		TargetAsc.Get()->HandleGameplayEvent(FGameplayTags::Get().Events_Enemy_Peck, &Payload);
		TargetAsc.Get()->OnAbilityEnded.AddUObject(this, &UBTT_Peck::FinishBTTask);
	}
	return EBTNodeResult::InProgress;
}

void UBTT_Peck::FinishBTTask(const FAbilityEndedData& AbilityEndedData)
{
	if(AbilityEndedData.AbilityThatEnded.GetClass()->IsChildOf(UPeckAbility::StaticClass()))
	{
		FinishExecute(true);
	}
}
