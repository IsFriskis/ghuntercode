// TinyTavern


#include "NoteButton.h"

#include "CommonRichTextBlock.h"
#include "Components/Image.h"
#include "Kismet/KismetMaterialLibrary.h"


void UNoteButton::SetNoteInfo(FSlateBrush NoteBrush, FText _NoteText, FText _NoteSignature)
{
	NoteText = _NoteText;
	NoteSignature = _NoteSignature;
	NoteTextBlock.Get()->SetText(BP_FormatTextForEnter());
	SignatureTextBlock.Get()->SetText(BP_FormatSignatureForEnter());
	//Dissolve material for Guncle Notes Big
	if(!InRecipeBook)
	{
		DynamicMat = UKismetMaterialLibrary::CreateDynamicMaterialInstance(GetWorld(), NoteDissolveMaterial);
		DynamicMat.Get()->SetScalarParameterValue("Dissolve", 1.0f);
		TObjectPtr<UTexture> noteTexture = Cast<UTexture>(NoteBrush.GetResourceObject());
		DynamicMat.Get()->SetTextureParameterValue("MaterialTexture", noteTexture);
		FSlateBrush noteBrush = NoteImage.Get()->GetBrush();
		noteBrush.SetResourceObject(DynamicMat);
		NoteImage.Get()->SetBrush(noteBrush);
		//Set AlphaBlend
		NoteBlend.SetBlendTime(NoteDissolveTime);
		NoteBlend.SetValueRange(1.0f, 0.0f);
		NoteBlend.Reset();
		NoteTextBlock.Get()->SetRenderOpacity(0.0f);
		SignatureTextBlock.Get()->SetRenderOpacity(0.0f);
		ShouldImageDissolve = true;
	}
	else
	{
		if(!BiggerNote)
		{
			NoteTextBlock.Get()->SetRenderOpacity(0.0f);
			SignatureTextBlock.Get()->SetRenderOpacity(0.0f);
			NoteImage.Get()->SetBrush(NoteBrush);
		}
	}
}

void UNoteButton::SetTextStyle(UDataTable* DataTable)
{
	NoteTextBlock.Get()->SetTextStyleSet(DataTable);
}

void UNoteButton::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);
	if(ShouldImageDissolve)
	{
		NoteBlend.Update(InDeltaTime);
		DynamicMat.Get()->SetScalarParameterValue("Dissolve", NoteBlend.GetBlendedValue());
		FSlateBrush noteBrush = NoteImage.Get()->GetBrush();
		noteBrush.SetResourceObject(DynamicMat);
		NoteImage.Get()->SetBrush(noteBrush);
		if(NoteBlend.IsComplete())
		{
			if(!InReverse)
			{
				NoteBlend.SetBlendTime(NoteTextAppearTime);
				NoteBlend.SetValueRange(0.0f, 1.0f);
				NoteBlend.Reset();
				ShouldImageDissolve = false;
				ShouldTextAppear = true;
			}
		}
	}
	if(ShouldTextAppear)
	{
		NoteBlend.Update(InDeltaTime);
		// NoteTextBlock.Get()->SetRenderOpacity(NoteBlend.GetBlendedValue());
		// SignatureTextBlock.Get()->SetRenderOpacity(NoteBlend.GetBlendedValue());
		if(NoteBlend.IsComplete())
		{
			ShouldTextAppear = false;
			if(InReverse)
			{
				NoteBlend.SetBlendTime(NoteDissolveTime / 2);
				NoteBlend.SetValueRange(0.0f, 1.0f);
				NoteBlend.Reset();
				ShouldImageDissolve = true;
			}
		}
	}
}

void UNoteButton::ReverseDissolve()
{
	InReverse = true;
	NoteBlend.SetBlendTime(NoteTextAppearTime / 2);
	NoteBlend.SetValueRange(1.0f, 0.0f);
	NoteBlend.Reset();
	ShouldTextAppear = true;
}
