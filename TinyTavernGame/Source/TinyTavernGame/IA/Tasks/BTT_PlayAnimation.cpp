// TinyTavern


#include "BTT_PlayAnimation.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "AIController.h"
#include "Kismet/GameplayStatics.h"
#include "TinyTavernGame/TT_GameplayTags.h"
#include "TinyTavernGame/Character/Enemy/EnemyBaseClass/EnemyBase.h"
#include "TinyTavernGame/Player/PlayerCharacter.h"
#include "TinyTavernGame/AbilitySystem/Abilities/Enemies/PlayAnimationAbility.h"

class AEnemyBase;

UBTT_PlayAnimationAbility::UBTT_PlayAnimationAbility(FObjectInitializer const& ObjectInitializer)
{
	NodeName = "Play Animation behaviour";
}

EBTNodeResult::Type UBTT_PlayAnimationAbility::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	AEnemyBase* EnemyBase = OwnerComp.GetAIOwner()->GetPawn<AEnemyBase>();
	if(EnemyBase)
	{
		TObjectPtr<UAbilitySystemComponent> TargetAsc = EnemyBase->GetAbilitySystemComponent();
		FGameplayEventData Payload;
		TargetAsc.Get()->HandleGameplayEvent(AbilityTag, &Payload);
		TargetAsc.Get()->OnAbilityEnded.AddUObject(this, &UBTT_PlayAnimationAbility::FinishBTTask);
	}

	if(ShouldAlwaysFacePlayer)
	{
		//Do the animation facing the player character
		ACharacter* PlayerCharacter = UGameplayStatics::GetPlayerCharacter(GetWorld(), 0);
		if(PlayerCharacter)
		{
			FVector PlayerLocation = PlayerCharacter->GetActorLocation();
			FVector EnemyLocation = EnemyBase->GetActorLocation();
			FVector Direction = PlayerLocation - EnemyLocation;
			Direction.Normalize();
			EnemyBase->SetActorRotation(Direction.Rotation());
		}
	}
	return EBTNodeResult::InProgress;
}

void UBTT_PlayAnimationAbility::FinishBTTask(const FAbilityEndedData& AbilityEndedData)
{
	if(AbilityEndedData.AbilityThatEnded.GetClass()->IsChildOf(UPlayAnimationAbility::StaticClass()))
	{
		FinishExecute(true);
	}
}
