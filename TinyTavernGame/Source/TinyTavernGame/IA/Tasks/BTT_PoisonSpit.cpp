// TinyTavern


#include "BTT_PoisonSpit.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystemComponent.h"
#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Kismet/GameplayStatics.h"
#include "TinyTavernGame/TT_GameplayTags.h"
#include "TinyTavernGame/AbilitySystem/Abilities/Enemies/PoisonSpitAbility.h"
#include "TinyTavernGame/Character/Enemy/EnemyBaseClass/EnemyBase.h"


UBTT_PoisonSpit::UBTT_PoisonSpit(FObjectInitializer const& ObjectInitializer)
{
	NodeName = "Poison Spit Behaviour";
}

EBTNodeResult::Type UBTT_PoisonSpit::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	TObjectPtr<AEnemyBase> EnemyBase = Cast<AEnemyBase>(OwnerComp.GetAIOwner()->GetPawn());
	if(EnemyBase)
	{
		TObjectPtr<UAbilitySystemComponent> TargetAsc = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(EnemyBase.Get());
		
		FGameplayEventData Payload;
		Payload.OptionalObject = OwnerComp.GetAIOwner()->GetBlackboardComponent()->GetValueAsObject("playerReference");
		TargetAsc.Get()->HandleGameplayEvent(FGameplayTags::Get().Events_Enemy_PoisonSpit, &Payload);
		TargetAsc.Get()->OnAbilityEnded.AddUObject(this, &UBTT_PoisonSpit::FinishBTTask);
	}

	ACharacter* PlayerCharacter = UGameplayStatics::GetPlayerCharacter(GetWorld(), 0);
	if(PlayerCharacter)
	{
		FVector PlayerLocation = PlayerCharacter->GetActorLocation();
		FVector EnemyLocation = EnemyBase->GetActorLocation();
		FVector Direction = PlayerLocation - EnemyLocation;
		Direction.Normalize();
		EnemyBase->SetActorRotation(Direction.Rotation());
	}
	
	return EBTNodeResult::InProgress;
}

void UBTT_PoisonSpit::FinishBTTask(const FAbilityEndedData& AbilityEndedData)
{
	if(AbilityEndedData.AbilityThatEnded.GetClass()->IsChildOf(UPoisonSpitAbility::StaticClass()))
	{
		FinishExecute(true);
	}
}
