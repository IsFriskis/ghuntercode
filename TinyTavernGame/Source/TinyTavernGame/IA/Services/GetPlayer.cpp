// TinyTavern


#include "GetPlayer.h"
#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "BehaviorTree/BTFunctionLibrary.h"
#include "GameFramework/Character.h"
#include "Kismet/GameplayStatics.h"
#include "TinyTavernGame/Character/Enemy/EnemyBaseClass/EnemyBase.h"

void UGetPlayer::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{

	APawn* OwningPawn = OwnerComp.GetAIOwner()->GetPawn();
	ACharacter* Player = UGameplayStatics::GetPlayerCharacter(GetWorld(),0);
	
	if(IsValid(Player) && IsValid(OwningPawn))
	{	
		float distanceToPlayer = OwningPawn->GetDistanceTo(Player);
		OwnerComp.GetBlackboardComponent()->SetValueAsFloat(DistanceToPlayer.SelectedKeyName,distanceToPlayer);
		OwnerComp.GetBlackboardComponent()->SetValueAsObject(PlayerReference.SelectedKeyName,Player);
	}
	
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);
}
