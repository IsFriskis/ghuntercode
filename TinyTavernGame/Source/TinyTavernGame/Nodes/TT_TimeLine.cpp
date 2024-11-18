// TinyTavern


#include "TT_TimeLine.h"

#include "MaterialHLSLTree.h"

UTT_TimeLine::UTT_TimeLine()
{
	
}

UTT_TimeLine* UTT_TimeLine::TT_TimeLine(UObject* WorldContextObject,FName TaskInstanceName,float BlendTime,bool Reversed, EAlphaBlendOption BlendOption, float StartValue, float EndValue)
{
	UTT_TimeLine* MyTask = NewObject<UTT_TimeLine>();
	MyTask-> BlendTime = BlendTime;
	MyTask->Reversed = Reversed;
	MyTask-> BlendOption = BlendOption;
	MyTask-> StartValue = StartValue;
	MyTask->RegisterWithGameInstance(WorldContextObject);
	MyTask-> EndValue = EndValue;
	MyTask->WorldContextObject = WorldContextObject;

	return MyTask;
}

void UTT_TimeLine::Activate()
{
	Super::Activate();
	
}

void UTT_TimeLine::Update()
{

}


void UTT_TimeLine::Finish()
{
	
}

