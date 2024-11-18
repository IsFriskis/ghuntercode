// TinyTavern


#include "AimingRangeAbilityTask.h"

#include "TinyTavernGame/TT_GameplayTags.h"
#include "TinyTavernGame/Player/PlayerCharacter.h"
#include "TinyTavernGame/Components/WeaponComponent.h"

UAimingRangeAbilityTask::UAimingRangeAbilityTask(const FObjectInitializer& ObjectInitializer)
{
	bTickingTask = true;
}

TObjectPtr<UAimingRangeAbilityTask> UAimingRangeAbilityTask::ExecuteAimRangeTask(UGameplayAbility* OwningAbility,
	TObjectPtr<APlayerCharacter> _Player, TObjectPtr<UStaticMesh> _GrillIndicatorMesh,
	TObjectPtr<UStaticMesh> _BoilIndicatorMesh, TObjectPtr<UStaticMesh> _FryIndicatorMesh,
	TObjectPtr<UStaticMesh> _FryExplosionIndicatorMesh, FIndicatorsInfo _IndicatorsInfo)
{
	TObjectPtr<UAimingRangeAbilityTask> aimRangeTask = NewAbilityTask<UAimingRangeAbilityTask>(OwningAbility);
	aimRangeTask.Get()->Player = _Player;
	aimRangeTask.Get()->GrillIndicatorMesh = _GrillIndicatorMesh;
	aimRangeTask.Get()->BoilIndicatorMesh = _BoilIndicatorMesh;
	aimRangeTask.Get()->FryIndicatorMesh = _FryIndicatorMesh;
	aimRangeTask.Get()->FryExplosionIndicatorMesh = _FryExplosionIndicatorMesh;
	aimRangeTask.Get()->IndicatorsInfo = _IndicatorsInfo;

	aimRangeTask.Get()->Player.Get()->WeaponIndicatorStaticMesh.Get()->SetVisibility(true);
	
	return aimRangeTask;
}

void UAimingRangeAbilityTask::TickTask(float DeltaTime)
{
	Super::TickTask(DeltaTime);

	FRotator weaponRotation = Player.Get()->GetActorRotation();
	weaponRotation.Yaw = 90;
	//Grill
	if(Player.Get()->GetWeaponComponent()->GetCurrentRangeWeaponTag() == FGameplayTags::Get().Weapon_Range_Grill)
	{
		Player.Get()->WeaponIndicatorStaticMesh2.Get()->SetVisibility(false);
		Player.Get()->WeaponIndicatorStaticMesh.Get()->SetStaticMesh(GrillIndicatorMesh);
		FVector indicatorPosition = Player.Get()->GetActorLocation();
		indicatorPosition = IndicatorsInfo.PositionOfGrill * Player.Get()->GetActorForwardVector() + Player.Get()->GetActorLocation();
		indicatorPosition.Z = Player.Get()->GetActorLocation().Z;
		Player.Get()->WeaponIndicatorStaticMesh.Get()->SetRelativeRotation(weaponRotation);
		Player.Get()->WeaponIndicatorStaticMesh.Get()->SetWorldLocation(indicatorPosition);
		Player.Get()->WeaponIndicatorStaticMesh.Get()->SetRelativeScale3D(FVector(IndicatorsInfo.ScaleOfGrill, IndicatorsInfo.ScaleOfGrill, 1));
	}
	else if(Player.Get()->GetWeaponComponent()->GetCurrentRangeWeaponTag() == FGameplayTags::Get().Weapon_Range_Boil)
	{
		Player.Get()->WeaponIndicatorStaticMesh2.Get()->SetVisibility(false);
		Player.Get()->WeaponIndicatorStaticMesh.Get()->SetStaticMesh(BoilIndicatorMesh);
		FVector indicatorPosition = Player.Get()->GetActorLocation();
		indicatorPosition = IndicatorsInfo.PositionOfBoil * Player.Get()->GetActorForwardVector() + Player.Get()->GetActorLocation();
		indicatorPosition.Z = Player.Get()->GetActorLocation().Z;
		Player.Get()->WeaponIndicatorStaticMesh.Get()->SetRelativeRotation(weaponRotation);
		Player.Get()->WeaponIndicatorStaticMesh.Get()->SetWorldLocation(indicatorPosition);
		Player.Get()->WeaponIndicatorStaticMesh.Get()->SetRelativeScale3D(FVector(IndicatorsInfo.ScaleOfBoil, IndicatorsInfo.ScaleOfBoil, 1));
	}
	else if(Player.Get()->GetWeaponComponent()->GetCurrentRangeWeaponTag() == FGameplayTags::Get().Weapon_Range_Fry)
	{
		Player.Get()->WeaponIndicatorStaticMesh2.Get()->SetVisibility(true);
		Player.Get()->WeaponIndicatorStaticMesh.Get()->SetStaticMesh(FryIndicatorMesh);
		Player.Get()->WeaponIndicatorStaticMesh2.Get()->SetStaticMesh(FryExplosionIndicatorMesh);
		FVector indicatorPosition = Player.Get()->GetActorLocation();
		indicatorPosition = IndicatorsInfo.PositionOfFry * Player.Get()->GetActorForwardVector() + Player.Get()->GetActorLocation();
		indicatorPosition.Z = Player.Get()->GetActorLocation().Z;
		Player.Get()->WeaponIndicatorStaticMesh.Get()->SetRelativeRotation(weaponRotation);
		Player.Get()->WeaponIndicatorStaticMesh.Get()->SetWorldLocation(indicatorPosition);
		Player.Get()->WeaponIndicatorStaticMesh.Get()->SetRelativeScale3D(FVector(1.0f, IndicatorsInfo.ScaleOfFry, 1));

		FVector explosionPosition = Player.Get()->GetActorLocation();
		explosionPosition = IndicatorsInfo.PositionOfFryExplosion * Player.Get()->GetActorForwardVector() + Player.Get()->GetActorLocation();
		explosionPosition.Z = Player.Get()->GetActorLocation().Z;
		Player.Get()->WeaponIndicatorStaticMesh2.Get()->SetRelativeRotation(weaponRotation);
		Player.Get()->WeaponIndicatorStaticMesh2.Get()->SetWorldLocation(explosionPosition);
		Player.Get()->WeaponIndicatorStaticMesh2.Get()->SetRelativeScale3D(FVector(IndicatorsInfo.ScaleOfFryExplosion, IndicatorsInfo.ScaleOfFryExplosion, 1));
	}
}
