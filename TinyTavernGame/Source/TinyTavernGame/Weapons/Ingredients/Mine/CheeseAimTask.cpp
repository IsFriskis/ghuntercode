// TinyTavern


#include "CheeseAimTask.h"

#include "TinyTavernGame/Player/PlayerCharacter.h"

UCheeseAimTask::UCheeseAimTask(const FObjectInitializer& ObjectInitializer)
{
	bTickingTask = true;
}

TObjectPtr<UCheeseAimTask> UCheeseAimTask::ExecuteCheeseAimTask(UGameplayAbility* OwningAbility,
	TObjectPtr<APlayerCharacter> _Player, float _CheeseMineInnerRadius, float _CheeseMineOuterRadius,
	TObjectPtr<UAnimMontage> HoldAnim, float _MinePlacementDistanceFromPlayer,
	TObjectPtr<UStaticMesh> _CheeseIndicatorMesh)
{
	TObjectPtr<UCheeseAimTask> cheeseAimTask = NewAbilityTask<UCheeseAimTask>(OwningAbility);
	cheeseAimTask.Get()->Player = _Player;
	cheeseAimTask.Get()->CheeseMineInnerRadius = _CheeseMineInnerRadius;
	cheeseAimTask.Get()->CheeseMineOuterRadius = _CheeseMineOuterRadius;
	cheeseAimTask.Get()->Player.Get()->PlayAnimMontage(HoldAnim);
	cheeseAimTask.Get()->MinePlacementDistanceFromPlayer = _MinePlacementDistanceFromPlayer;
	cheeseAimTask.Get()->Player.Get()->WeaponIndicatorStaticMesh.Get()->SetStaticMesh(_CheeseIndicatorMesh);
	
	return cheeseAimTask;
}


void UCheeseAimTask::TickTask(float DeltaTime)
{
	Super::TickTask(DeltaTime);

	FVector MinePosition = Player.Get()->GetActorForwardVector() * FVector(MinePlacementDistanceFromPlayer,MinePlacementDistanceFromPlayer,0);
#if WITH_EDITOR
	DrawDebugSphere(GetWorld(), Player.Get()->GetActorLocation() + MinePosition, CheeseMineInnerRadius, 8, FColor::Yellow, false, -1.0f);
	DrawDebugSphere(GetWorld(), Player.Get()->GetActorLocation() + MinePosition, CheeseMineOuterRadius, 8, FColor::Red, false, -1.0f);
#endif
	Player.Get()->WeaponIndicatorStaticMesh.Get()->SetVisibility(true);
	FVector indicatorPosition = Player.Get()->GetActorLocation() + MinePosition;
	indicatorPosition.Z = Player.Get()->GetActorLocation().Z;
	Player.Get()->WeaponIndicatorStaticMesh.Get()->SetWorldRotation(FRotator::ZeroRotator);
	Player.Get()->WeaponIndicatorStaticMesh.Get()->SetWorldLocation(indicatorPosition);
	Player.Get()->WeaponIndicatorStaticMesh.Get()->SetRelativeScale3D(FVector(CheeseMineInnerRadius / 100, CheeseMineInnerRadius / 100, 1));
}
