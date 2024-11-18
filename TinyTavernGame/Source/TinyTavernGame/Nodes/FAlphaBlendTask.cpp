// TinyTavern


#include "FAlphaBlendTask.h"

#include "BehaviorTree/BTAuxiliaryNode.h"
#include "Tests/AutomationCommon.h"


UFAlphaBlendTask::UFAlphaBlendTask(const FObjectInitializer& ObjectInitializer)

	:Super(ObjectInitializer)
	{}


void UFAlphaBlendTask::OnGameplayAbilityCancelled()
{
	
}

UFAlphaBlendTask* UFAlphaBlendTask::FAlphaBlendTask(FName TaskInstanceName,	float BlendTime,bool Reversed, EAlphaBlendOption BlendOption, float StartValue, float EndValue)
{
	
	UFAlphaBlendTask* AlphaBlendObj = NewObject<UFAlphaBlendTask>();
	AlphaBlendObj-> BlendTime = BlendTime;
	AlphaBlendObj->Reversed = Reversed;
	AlphaBlendObj-> BlendOption = BlendOption;
	AlphaBlendObj-> StartValue = StartValue;
	AlphaBlendObj-> EndValue = EndValue;
	AlphaBlendObj->bTickingTask = 1;
	return AlphaBlendObj;
}

void UFAlphaBlendTask::Activate()
{
	Super::Activate();
}

void UFAlphaBlendTask::TickTask(float DeltaTime)
{
	Super::TickTask(DeltaTime);
	if(Reversed)
	{
		float Aux = StartValue;
		StartValue = EndValue;
		EndValue = Aux;
	}
	AlphaBlend.SetBlendTime(BlendTime);
	AlphaBlend.SetBlendOption(BlendOption);
	//AlphaBlend.SetValueRange(StartValue,EndValue);
	AlphaBlend.Update(DeltaTime);
	float CurrentValue = FMath::Lerp(StartValue, EndValue, AlphaBlend.GetAlpha());
	Values.Broadcast(CurrentValue);
	
	Timer += DeltaTime;

	if(Timer > BlendTime)
		{
		OnCompleted.Broadcast(0);
		EndTask();
	}
}
