// TinyTavern


#include "ChestUI.h"

#include "CommonTextBlock.h"
#include "Components/Image.h"
#include "Input/CommonUIInputTypes.h"

void UChestUI::NativeConstruct()
{
	Super::NativeConstruct();
	
}

void UChestUI::NativeDestruct()
{
	Super::NativeDestruct();
	BackHandle.Unregister();
}

void UChestUI::OnAnimationFinished_Implementation(const UWidgetAnimation* Animation)
{
	Super::OnAnimationFinished_Implementation(Animation);
	if(!InShowAnimation)
	{
		Chest.Get()->ChestFinished();
	}
	else
	{
		//Back Handler
		BackHandle = RegisterUIActionBinding(FBindUIActionArgs(BackInputActionData, false, FSimpleDelegate::CreateUObject(this, &UChestUI::BackLogic)));
	}
	InShowAnimation = !InShowAnimation;
}

void UChestUI::BackLogic()
{
	BackHandle.Unregister();
	PlayAnimationReverse(RelicAppearAnimation, 2);
}

void UChestUI::SetChestRelic(TObjectPtr<AChest> _Chest, FRelicInfoStruct RelicInfo)
{
	Chest = _Chest;

	switch (RelicInfo.RelicType) {
		case SACREDSPORES:
			RelicImage.Get()->SetBrush(DamselsSporesBrush);
			StampImage.Get()->SetBrush(GoctorStamp);
			RelicName.Get()->SetText(FText::FromString("Pouch of The Damsel's Spores"));
			break;
		case FUNGAlSILK:
			RelicImage.Get()->SetBrush(ZerroleanSilkBrush);
			StampImage.Get()->SetBrush(GailorStamp);
			RelicName.Get()->SetText(FText::FromString("Zerrolean Silk"));
			break;
		case GMITHRELIC:
			RelicImage.Get()->SetBrush(GungstenBarBrush);
			StampImage.Get()->SetBrush(GmithStamp);
			RelicName.Get()->SetText(FText::FromString("Gungsten Bar"));
			break;
		default: ;
	}
	PlayAnimation(RelicAppearAnimation, GetAnimationCurrentTime(RelicAppearAnimation));
}

