// TinyTavern

#pragma once

#include "CoreMinimal.h"
#include "Abilities/Tasks/AbilityTask.h"
#include "TinyTavernGame/Character/Enemy/EnemyBaseClass/EnemyBase.h"
#include "MeleeDashTask.generated.h"


class UMeleeDashAbility;
/**
 * 
*/
USTRUCT()
struct FFinisherData : public FGameplayAbilityTargetData
{
	GENERATED_BODY()

public:
	UPROPERTY()
	APlayerCharacter* PlayerActor;

	UPROPERTY()
	AEnemyBase* EnemyToKill;

	UPROPERTY()
	int MeleeWeaponLevel = 0;

	// This is required for all child structs of FGameplayAbilityTargetData
	virtual UScriptStruct* GetScriptStruct() const override
	{
		return FFinisherData::StaticStruct();
	}
};

UCLASS()
class TINYTAVERNGAME_API UMeleeDashTask : public UAbilityTask
{
public:
	UMeleeDashTask(const FObjectInitializer& ObjectInitializer);


private:
	GENERATED_BODY()

public:

	TObjectPtr<UMeleeDashTask> ExecuteDashTask(UGameplayAbility* OwningAbility);
	virtual void TickTask(float DeltaTime) override;

protected:
	
	UPROPERTY()
	TObjectPtr<UMeleeDashAbility> MeleeDashAbility;

private:
	
	UPROPERTY()
	AEnemyBase* NearestEnemy;
	float NearestEnemyDistance;
	
};
