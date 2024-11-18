// TinyTavern


#include "GuncleNoteBig.h"

#include "CommonTextBlock.h"
#include "NoteButton.h"
#include "Animation/WidgetAnimation.h"
#include "Components/Image.h"
#include "Input/CommonUIInputTypes.h"

void UGuncleNoteBig::NativeConstruct()
{
	Super::NativeConstruct();
	PlaySound(NoteSound);
	Note.Get()->SetIsInteractionEnabled(false);
	BiggerNote.Get()->SetIsInteractionEnabled(false);
	BiggerNote.Get()->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
	BiggerNote.Get()->SetRenderOpacity(0.0f);
}


void UGuncleNoteBig::NativeDestruct()
{
	Super::NativeDestruct();
	BackHandle.Unregister();
	BiggerNoteHandle.Unregister();
}

void UGuncleNoteBig::OnAnimationFinished_Implementation(const UWidgetAnimation* Animation)
{
	Super::OnAnimationFinished_Implementation(Animation);
	
	if(!InShowAnimation)
	{
		OnFinishedReadingNote.Broadcast();
	}
	else
	{
		//Back Handler
		BackHandle = RegisterUIActionBinding(FBindUIActionArgs(BackInputActionData, false, FSimpleDelegate::CreateUObject(this, &UGuncleNoteBig::BackLogic)));
		BiggerNoteHandle = RegisterUIActionBinding(FBindUIActionArgs(BiggerNoteInputActionData, false, FSimpleDelegate::CreateUObject(this, &UGuncleNoteBig::MakeNoteBiggerLogic)));
	}
	InShowAnimation = !InShowAnimation;
}

void UGuncleNoteBig::BackLogic()
{
	if(NoteIsBig)
	{
		//Disappear
		BiggerNote.Get()->SetVisibility(ESlateVisibility::Hidden);	
	}
	BackHandle.Unregister();
	BiggerNoteHandle.Unregister();
	PlaySound(EndNoteSound);
	Note.Get()->ReverseDissolve();
	PlayAnimationReverse(AppearAnimation, 2);
}

void UGuncleNoteBig::SetBigNoteImage(FSlateBrush NoteBrush, FText NoteText, FText SignatureText)
{
	Note.Get()->SetNoteInfo(NoteBrush, NoteText, SignatureText);
	BiggerNote.Get()->SetNoteInfo(NoteBrush, NoteText, SignatureText);
	PlayAnimation(AppearAnimation, GetAnimationCurrentTime(AppearAnimation));
}

void UGuncleNoteBig::MakeNoteBiggerLogic()
{
	NoteIsBig = !NoteIsBig;
	if(NoteIsBig)
	{
		//Show the note bigger
		BiggerNote.Get()->SetRenderOpacity(1.0f);
	}
	else
	{
		//Disappear
		BiggerNote.Get()->SetRenderOpacity(0.0f);
	}
}
