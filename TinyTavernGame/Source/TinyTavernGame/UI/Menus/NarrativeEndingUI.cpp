// TinyTavern


#include "NarrativeEndingUI.h"

#include "Components/Image.h"
#include "Kismet/KismetMaterialLibrary.h"

void UNarrativeEndingUI::NativeConstruct()
{
	Super::NativeConstruct();
	DynamicMat = UKismetMaterialLibrary::CreateDynamicMaterialInstance(GetWorld(), DissolveMaterial);
	DynamicMat.Get()->SetScalarParameterValue("Dissolve", 1.0f);
	FSlateBrush noteBrush = NarrativeVideoImage.Get()->GetBrush();
	noteBrush.SetResourceObject(DynamicMat);
	NarrativeVideoImage.Get()->SetBrush(noteBrush);
	//Set AlphaBlend
	EndingBlend.SetBlendTime(DissolveTime);
	EndingBlend.SetValueRange(1.0f, 0.0f);
	EndingBlend.Reset();
	ShouldImageDissolve = true;
}

void UNarrativeEndingUI::NativeDestruct()
{
	Super::NativeDestruct();
}

void UNarrativeEndingUI::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);
	if(ShouldImageDissolve)
	{
		EndingBlend.Update(InDeltaTime);
		DynamicMat.Get()->SetScalarParameterValue("Dissolve", EndingBlend.GetBlendedValue());
		FSlateBrush noteBrush = NarrativeVideoImage.Get()->GetBrush();
		noteBrush.SetResourceObject(DynamicMat);
		NarrativeVideoImage.Get()->SetBrush(noteBrush);
		if(EndingBlend.IsComplete())
		{
			ShouldImageDissolve = false;
			BP_FinishedDissolve();
		}
	}
}
