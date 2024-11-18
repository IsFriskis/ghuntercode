// TinyTavern


#include "WineAimTask.h"

#include "TinyTavernGame/Player/PlayerCharacter.h"

UWineAimTask::UWineAimTask(const FObjectInitializer& ObjectInitializer)
{
	bTickingTask = true;
}

TObjectPtr<UWineAimTask> UWineAimTask::ExecuteWineAimTask(UGameplayAbility* OwningAbility,
	TObjectPtr<APlayerCharacter> _Player, float _WineRadius, TObjectPtr<UAnimMontage> HoldAnim,
	TObjectPtr<UStaticMesh> _WineIndicatorMesh)
{
	TObjectPtr<UWineAimTask> wineAimTask = NewAbilityTask<UWineAimTask>(OwningAbility);
	wineAimTask.Get()->Player = _Player;
	wineAimTask.Get()->WineRadius = _WineRadius;
	wineAimTask.Get()->Player.Get()->PlayAnimMontage(HoldAnim.Get());
	wineAimTask.Get()->Player.Get()->WeaponIndicatorStaticMesh.Get()->SetStaticMesh(_WineIndicatorMesh);
	wineAimTask.Get()->Player.Get()->WeaponIndicatorStaticMesh.Get()->SetVisibility(true);
	return wineAimTask;
}

void UWineAimTask::TickTask(float DeltaTime)
{
	Super::TickTask(DeltaTime);
	
#if WITH_EDITOR
	DrawDebugSphere(GetWorld(), Player.Get()->GetActorLocation(), WineRadius, 8, FColor::Purple, false, -1.0f);
#endif
	Player.Get()->WeaponIndicatorStaticMesh.Get()->SetVisibility(true);
	FVector indicatorPosition = Player.Get()->GetActorLocation();
	indicatorPosition.Z = Player.Get()->GetActorLocation().Z;
	Player.Get()->WeaponIndicatorStaticMesh.Get()->SetWorldRotation(FRotator::ZeroRotator);
	Player.Get()->WeaponIndicatorStaticMesh.Get()->SetWorldLocation(indicatorPosition);
	Player.Get()->WeaponIndicatorStaticMesh.Get()->SetRelativeScale3D(FVector(WineRadius / 100, WineRadius / 100, 1));
}
