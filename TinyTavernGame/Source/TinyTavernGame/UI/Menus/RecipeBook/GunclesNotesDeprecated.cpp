// TinyTavern


#include "GunclesNotesDeprecated.h"

#include "GuncleNoteBig.h"
#include "NoteButton.h"
#include "Components/Image.h"
#include "TinyTavernGame/Settings/TT_GameInstance.h"

void UGunclesNotesDeprecated::NativeConstruct()
{
	Super::NativeConstruct();

	TObjectPtr<UTT_GameInstance> TTGameInstance = Cast<UTT_GameInstance>(GetGameInstance());
	//Set Notes Invisible
	Note1.Get()->SetVisibility(ESlateVisibility::Hidden);
	Note2.Get()->SetVisibility(ESlateVisibility::Hidden);
	Note3.Get()->SetVisibility(ESlateVisibility::Hidden);
	Note4.Get()->SetVisibility(ESlateVisibility::Hidden);
	Note5.Get()->SetVisibility(ESlateVisibility::Hidden);
	Note6.Get()->SetVisibility(ESlateVisibility::Hidden);
	
	GuncleNotesDataTable.Get()->GetAllRows("", GuncleNotes);
	for (FGuncleNotes_Struct* guncleNote : GuncleNotes)
	{
		if(guncleNote->ID == IDNote1 && TTGameInstance.Get()->GuncleNoteFound[IDNote1])
		{
			Note1.Get()->SetNoteInfo(guncleNote->Note, guncleNote->NoteText, guncleNote->SignatureText);
			Note1.Get()->SetVisibility(ESlateVisibility::Visible);
		}
		else if(guncleNote->ID == IDNote2 && TTGameInstance.Get()->GuncleNoteFound[IDNote2])
		{
			Note2.Get()->SetNoteInfo(guncleNote->Note, guncleNote->NoteText, guncleNote->SignatureText);
			Note2.Get()->SetVisibility(ESlateVisibility::Visible);
		}
		else if(guncleNote->ID == IDNote3 && TTGameInstance.Get()->GuncleNoteFound[IDNote3])
		{
			Note3.Get()->SetNoteInfo(guncleNote->Note, guncleNote->NoteText, guncleNote->SignatureText);
			Note3.Get()->SetVisibility(ESlateVisibility::Visible);
		}
		else if(guncleNote->ID == IDNote4 && TTGameInstance.Get()->GuncleNoteFound[IDNote4])
		{
			Note4.Get()->SetNoteInfo(guncleNote->Note, guncleNote->NoteText, guncleNote->SignatureText);
			Note4.Get()->SetVisibility(ESlateVisibility::Visible);
		}
		else if(guncleNote->ID == IDNote5 && TTGameInstance.Get()->GuncleNoteFound[IDNote5])
		{
			Note5.Get()->SetNoteInfo(guncleNote->Note, guncleNote->NoteText, guncleNote->SignatureText);
			Note5.Get()->SetVisibility(ESlateVisibility::Visible);
		}
		else if(guncleNote->ID == IDNote6 && TTGameInstance.Get()->GuncleNoteFound[IDNote6])
		{
			Note6.Get()->SetNoteInfo(guncleNote->Note, guncleNote->NoteText, guncleNote->SignatureText);
			Note6.Get()->SetVisibility(ESlateVisibility::Visible);
		}
	}
}

UWidget* UGunclesNotesDeprecated::NativeGetDesiredFocusTarget() const
{
	if(Note1.Get()->IsVisible())
	{
		return Note1;
	}
	if(Note2.Get()->IsVisible())
	{
		return Note2;
	}
	if(Note3.Get()->IsVisible())
	{
		return Note3;
	}
	if(Note4.Get()->IsVisible())
	{
		return Note4;
	}
	if(Note5.Get()->IsVisible())
	{
		return Note5;
	}
	if(Note6.Get()->IsVisible())
	{
		return Note6;
	}
	return Super::NativeGetDesiredFocusTarget();
}
