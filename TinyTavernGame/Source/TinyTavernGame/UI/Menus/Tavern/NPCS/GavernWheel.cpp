// TinyTavern


#include "GavernWheel.h"

#include "NPCDialog.h"
#include "Components/AudioComponent.h"
#include "Components/Image.h"
#include "Input/CommonUIInputTypes.h"
#include "Kismet/GameplayStatics.h"
#include "TinyTavernGame/Settings/TT_GameInstance.h"

void UGavernWheel::NativeConstruct()
{
	Super::NativeConstruct();
	//First set a slow rate to show the upgradse in the wheel
	LoopBlend.SetBlendTime(FirstLoopTime);
	LoopBlend.SetValueRange(0, -360);
	//Divide the image into Number Of Sections and assigned them a GavernUpgrade
	TArray<EGavernUpgrades> gavernUpgrades = {SHIELD, PUFFBALLRESISTANCE, EXTRAREPUTATION};
	float oneSectionMaxAngle = 360/NumberOfSections;
	for (int i = 0; i < NumberOfSections; i++)
	{
		float beginValue = i * oneSectionMaxAngle * -1;
		float endvalue = beginValue + oneSectionMaxAngle * -1;
		WheelSections.Add(FSectionOfWheel(beginValue, endvalue, gavernUpgrades[i]));
	}
	PlaySound(InitialSound);
	ActivateWidget();
}

void UGavernWheel::NativeDestruct()
{
	Super::NativeDestruct();
	StopWheelHandle.Unregister();
}

void UGavernWheel::SetGavern(UNPCDialog* _Gavern)
{
	Gavern = _Gavern;
}

void UGavernWheel::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);

	if(!WheelStopped)
	{
		//While player did not stop the wheel, it should keep looping
		if(!PlayerStoppedWheel)
		{
			LoopBlend.Update(InDeltaTime);
			WheelImage.Get()->SetRenderTransformAngle(LoopBlend.GetBlendedValue());
			if(LoopBlend.IsComplete())
			{
				WheelImage.Get()->SetRenderTransformAngle(0.0f);
				if(!FirstLoopDone)
				{
					//After first loop, player can stop the wheel
					LoopSoundComponent = UGameplayStatics::SpawnSound2D(GetWorld(), LoopSound);
					StopWheelHandle = RegisterUIActionBinding(FBindUIActionArgs(StopWheelInputActionData, true, FSimpleDelegate::CreateUObject(this, &UGavernWheel::StopWheel)));
					FirstLoopDone = true;
				}
				LoopBlend.SetBlendTime(LoopTime);
				LoopBlend.Reset();
			}
		}
		else
		{
			//Player stopped the wheel, execute the stopping alpha blend
			StopWheelBlend.Update(InDeltaTime);
			WheelImage.Get()->SetRenderTransformAngle(StopWheelBlend.GetBlendedValue());
			if(StopWheelBlend.IsComplete())
			{
				WheelStopped = true;
				CheckWheelSectionItLanded();
			}
		}
	}
}

void UGavernWheel::StopWheel()
{
	if(LoopSoundComponent)
	{
		LoopSoundComponent.Get()->Stop();
	}
	StopWheelHandle.Unregister();
	StopWheelBlend.SetBlendTime(StoppingTime);
	float beginValue = WheelImage.Get()->GetRenderTransformAngle();
	float endValue = WheelImage.Get()->GetRenderTransformAngle() - 180.0f;
	StopWheelBlend.SetValueRange(beginValue, endValue);
	PlayerStoppedWheel = true;
	PlaySound(EndSound);
}

void UGavernWheel::CheckWheelSectionItLanded()
{
	float angleItStopped = WheelImage.Get()->GetRenderTransformAngle();
	if(angleItStopped < -360.0f)
	{
		angleItStopped += 360.0f;
	}
	for (int i = 0; i < WheelSections.Num(); i++)
	{
		float negativeBeginValue = WheelSections[i].BeginningValue;
		float negativeEndValue = WheelSections[i].EndValue;
		if(angleItStopped <= negativeBeginValue && angleItStopped >= negativeEndValue)
		{
			SectionItLanded = WheelSections[i];
			break;
		}
	}
	//Gavern.Get()->BP_ShowUpgrade(SectionItLanded.GavernUpgrade);
	Cast<UTT_GameInstance>(GetGameInstance())->GavernUpgrade = SectionItLanded.GavernUpgrade;
	Gavern.Get()->ShowUpgradeText();
	DeactivateWidget();
	RemoveFromParent();
}
