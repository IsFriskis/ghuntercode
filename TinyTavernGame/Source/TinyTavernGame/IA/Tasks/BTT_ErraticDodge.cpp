// TinyTavern


#include "BTT_ErraticDodge.h"


#include "AbilitySystemBlueprintLibrary.h"
#include "AIController.h"
#include "Abilities/GameplayAbilityTypes.h"
#include "TinyTavernGame/TT_GameplayTags.h"
#include "TinyTavernGame/AbilitySystem/Abilities/Enemies/ErraticDodgeAbility.h"
#include "TinyTavernGame/AbilitySystem/LibraryTask/TaskGamePlayAbilityEnded.h"
#include "TinyTavernGame/Character/Enemy/EnemyBaseClass/EnemyBase.h"

UBTT_ErraticDodge::UBTT_ErraticDodge(FObjectInitializer const& ObjectInitializer)
{
	NodeName = "Erratic Dodge";
}

EBTNodeResult::Type UBTT_ErraticDodge::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{

	FGameplayEventData Payload;
	TObjectPtr<AEnemyBase> Enemy = Cast<AEnemyBase>(OwnerComp.GetAIOwner()->GetPawn());
	ASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(Enemy.Get());
	
	ASC->HandleGameplayEvent(FGameplayTags::Get().Events_Enemy_ErraticDodge,&Payload);
	ASC->OnAbilityEnded.AddUObject(this,&UBTT_ErraticDodge::FinishBTTask);
	return EBTNodeResult::InProgress;
}

void UBTT_ErraticDodge::FinishBTTask(const FAbilityEndedData& AbilityEndedData)
{
    if(AbilityEndedData.AbilityThatEnded.GetClass()->IsChildOf(UErraticDodgeAbility::StaticClass()))
    {
    	ASC->RemoveLooseGameplayTag(FGameplayTags::Get().Events_Enemy_ErraticDodge);
	    FinishExecute(true);
    	
    }
}
