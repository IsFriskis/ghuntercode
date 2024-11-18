// TinyTavern


#include "CheeseMineTask.h"
#include "TinyTavernGame/UI/HUD/3DHuds/Hud3D.h"

UCheeseMineTask::UCheeseMineTask(const FObjectInitializer& ObjectInitializer)
{
	bTickingTask = true;
}

void UCheeseMineTask::TickTask(float DeltaTime)
{
	Super::TickTask(DeltaTime);
	
	CurrentTime += DeltaTime;
	MinePlantBlend.Update(DeltaTime);
	if (CurrentTime >= MinePlantTime)
	{
		MinePlantBlend.Reset();
		OnCheeseMinePlanted.Broadcast();
	}
}

TObjectPtr<UCheeseMineTask> UCheeseMineTask::ExecuteMineTask(UGameplayAbility* OwningAbility,
				float _MinePlantTime, TObjectPtr<UHud3D> _PlayerHud3D)
{
	TObjectPtr<UCheeseMineTask> cheeseMineTask = NewAbilityTask<UCheeseMineTask>(OwningAbility);
	cheeseMineTask.Get()->MinePlantTime = _MinePlantTime;
	cheeseMineTask.Get()->PlayerHud3D = _PlayerHud3D;
	cheeseMineTask.Get()->MinePlantBlend.SetBlendTime(_MinePlantTime);
	cheeseMineTask.Get()->MinePlantBlend.SetValueRange(0, 1);

	return cheeseMineTask;
}

void UCheeseMineTask::CancelMine()
{
	PlayerHud3D.Get()->ResetProgressBar();
	MinePlantBlend.Reset();
}

