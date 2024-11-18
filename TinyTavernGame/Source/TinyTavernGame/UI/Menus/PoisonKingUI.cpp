// TinyTavern


#include "PoisonKingUI.h"

#include "Buttons/ButtonBase.h"
#include "Input/CommonUIInputTypes.h"
#include "TinyTavernGame/Actor/GetPoisonForKing.h"

void UPoisonKingUI::NativeConstruct()
{
	Super::NativeConstruct();
	PlayAnimation(AppearAnimation);
}

void UPoisonKingUI::NativeDestruct()
{
	Super::NativeDestruct();
	BackHandle.Unregister();
	ObtainPoisonButton.Get()->OnClicked().RemoveAll(this);
	DenyButton.Get()->OnClicked().RemoveAll(this);
}

UWidget* UPoisonKingUI::NativeGetDesiredFocusTarget() const
{
	Super::NativeGetDesiredFocusTarget();
	return ObtainPoisonButton;
}

void UPoisonKingUI::OnAnimationFinished_Implementation(const UWidgetAnimation* Animation)
{
	Super::OnAnimationFinished_Implementation(Animation);
	//Handlers
	BackHandle = RegisterUIActionBinding(FBindUIActionArgs(BackInputActionData, false, FSimpleDelegate::CreateUObject(this, &UPoisonKingUI::BackLogic)));
	ObtainPoisonButton.Get()->OnClicked().AddUObject(this, &UPoisonKingUI::GetPoison);
	DenyButton.Get()->OnClicked().AddUObject(this, &UPoisonKingUI::BackLogic);
}

void UPoisonKingUI::SetPoisonForKingSphere(TObjectPtr<AGetPoisonForKing> _GetPoisonForKing)
{
	GetPoisonForKing = _GetPoisonForKing;
}

void UPoisonKingUI::GetPoison()
{
	GetPoisonForKing.Get()->PoisonObtained();
	BP_EndEffects();
}

void UPoisonKingUI::BackLogic()
{
	GetPoisonForKing.Get()->PoisonDenied();
}

