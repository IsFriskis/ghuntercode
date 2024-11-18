// TinyTavern


#include "TurnToAshesTask.h"

#include "BurnEnemy.h"
#include "TurnToAshesAbility.h"
#include "TinyTavernGame/Character/Enemy/Basilisco/EnemyBasilisco.h"
#include "TinyTavernGame/Character/Enemy/EnemyBaseClass/EnemyBase.h"

UTurnToAshesTask::UTurnToAshesTask(const FObjectInitializer& ObjectInitializer)
{
	bTickingTask = true;
	
}

TObjectPtr<UTurnToAshesTask> UTurnToAshesTask::ExecuteTurnToAshesTask(UGameplayAbility* OwningAbility)
{
	TObjectPtr<UTurnToAshesTask> TurnToAshesTask =  NewAbilityTask<UTurnToAshesTask>(OwningAbility);

	TurnToAshesTask->TurnToAshesAbility = Cast<UBurnEnemy>(OwningAbility);
	return TurnToAshesTask;
}

void UTurnToAshesTask::TickTask(float DeltaTime)
{
	Super::TickTask(DeltaTime);
	// Step 1: Initialize blend value and material parameters if not already done
	if (!bIsInitialized)
	{
		DynamicMaterial = UMaterialInstanceDynamic::Create(TurnToAshesAbility->BurnMaterial, this);
		if(Cast<AEnemyBasilisco>(TurnToAshesAbility->Enemy.Get()))
		{	
			TurnToAshesAbility->Enemy.Get()->GetMesh()->SetMaterial(1, DynamicMaterial);
		}
		else
		{
			TurnToAshesAbility->Enemy.Get()->GetMesh()->SetMaterial(0, DynamicMaterial);
		}
		bIsInitialized = true;
	}

	// Step 2: Update the blend value based on some logic (e.g., time or gameplay events)
	BlendValue += DeltaTime * 0.9f; // Example logic to update blend value
	BlendValue = FMath::Clamp(BlendValue, 0.0f, 1.0f);
	
	DynamicMaterial->SetScalarParameterValue(FName("BurnDisappearance"), BlendValue);
	
	if (BlendValue >= 1.0f)
	{
		OnFullMaterialBurnChange.Broadcast();
		EndTask();
	}
	
}
