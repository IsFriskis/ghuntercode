// TinyTavern


#include "GuncleNotes.h"

#include "CommonActionWidget.h"
#include "NoteNameButton.h"
#include "Components/Image.h"
#include "Components/ScrollBox.h"
#include "Components/TextBlock.h"
#include "Components/VerticalBox.h"
#include "Input/CommonUIInputTypes.h"
#include "TinyTavernGame/Settings/TT_GameInstance.h"
#include "TinyTavernGame/UI/Menus/RecipeBook/NoteButton.h"
#include "TinyTavernGame/UI/Menus/Tavern/NPCS/GuncleNotes_Datatable.h"


void UGuncleNotes::NativePreConstruct()
{
	Super::NativePreConstruct();
	NoteButton->SetTextStyle(TextStyleDataTable);
}

void UGuncleNotes::NativeConstruct()
{
	Super::NativeConstruct();
	
	Buttons.Append(GuncleVerticalBox.Get()->GetAllChildren());
	Buttons.Append(GuncleVerticalBox2.Get()->GetAllChildren());
	Buttons.Append(GaliasVerticalBox.Get()->GetAllChildren());
	Buttons.Append(GaliasVerticalBox2.Get()->GetAllChildren());
	Buttons.Append(OthersVerticalBox.Get()->GetAllChildren());
	for (UWidget* widget : Buttons)
	{
		if(UNoteNameButton* NoteNameButton = Cast<UNoteNameButton>(widget))
		{
			NoteNameButton->OnButtonHovered.RemoveAll(this);
			NoteNameButton->OnButtonHovered.AddDynamic(this, &UGuncleNotes::UpdateNoteInfo);
		}
	}
	BiggerNote.Get()->SetIsInteractionEnabled(false);
}

void UGuncleNotes::NativeDestruct()
{
	Super::NativeDestruct();
	for (UWidget* widget : Buttons)
	{
		if(UNoteNameButton* NoteNameButton = Cast<UNoteNameButton>(widget))
		{
			NoteNameButton->OnButtonHovered.RemoveDynamic(this, &UGuncleNotes::UpdateNoteInfo);
		}
	}
	TranscibeHandle.Unregister();
}

void UGuncleNotes::NativeOnActivated()
{
	Super::NativeOnActivated();
	BiggerNote.Get()->SetVisibility(ESlateVisibility::Hidden);
	BiggerButtonBackground.Get()->SetVisibility(ESlateVisibility::Hidden);
	BiggerText.Get()->SetVisibility(ESlateVisibility::Hidden);
	BiggerAction.Get()->SetVisibility(ESlateVisibility::Hidden);
	NoteIsBig = false;
}

UWidget* UGuncleNotes::NativeGetDesiredFocusTarget() const
{
	return FirstButton;
}

void UGuncleNotes::UpdateNoteInfo(int NoteID)
{
	UTT_GameInstance* GameInstance = Cast<UTT_GameInstance>(GetGameInstance());
	
	TArray<FName> RowNames = GameInstance->GuncleNotesDataTable.Get()->GetRowNames();
	for (FName Row : RowNames)
	{
		FGuncleNotes_Struct* NoteData = GameInstance->GuncleNotesDataTable.Get()->FindRow<FGuncleNotes_Struct>(Row,"");
		if( NoteID == NoteData->ID)
		{
			NoteButton.Get()->SetNoteInfo(NoteData->Note,NoteData->NoteText, NoteData->SignatureText);
			NoteButton.Get()->SetVisibility(ESlateVisibility::HitTestInvisible);
			BiggerNote.Get()->SetNoteInfo(NoteData->Note,NoteData->NoteText, NoteData->SignatureText);
			TranscibeHandle.Unregister();
			TranscibeHandle = RegisterUIActionBinding(FBindUIActionArgs(TranscibeInputActionData, false, FSimpleDelegate::CreateUObject(this, &UGuncleNotes::TranscibeNote)));
			BiggerButtonBackground.Get()->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
			BiggerText.Get()->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
			BiggerAction.Get()->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
			break;
		}
	}
}

void UGuncleNotes::TranscibeNote()
{
	if(!NoteIsBig)
	{
		NoteIsBig = true;
		BiggerNote.Get()->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
	}
	else
	{
		NoteIsBig = false;
		BiggerNote.Get()->SetVisibility(ESlateVisibility::Hidden);
	}
}
