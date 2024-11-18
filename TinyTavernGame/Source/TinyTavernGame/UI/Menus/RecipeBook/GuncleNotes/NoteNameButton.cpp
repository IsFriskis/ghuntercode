// TinyTavern


#include "NoteNameButton.h"

#include "CommonTextBlock.h"
#include "Components/Border.h"
#include "Components/TextBlock.h"
#include "TinyTavernGame/Settings/TT_GameInstance.h"
#include "TinyTavernGame/UI/Menus/Tavern/NPCS/GuncleNotes_Datatable.h"

class UTT_GameInstance;


void UNoteNameButton::NativePreConstruct()
{
	Super::NativePreConstruct();
#if WITH_EDITOR
	if(PreviewAsFound)
	{
		SetUnlocked();
	}
#endif
}

void UNoteNameButton::NativeConstruct()
{
	Super::NativeConstruct();
	UTT_GameInstance* GameInstance = Cast<UTT_GameInstance>(GetGameInstance());
	Unlocked = GameInstance->GuncleNoteFound[NoteID];

	if(Unlocked)
	{
		SetUnlocked();
	}
	else
	{
		this->SetVisibility(ESlateVisibility::HitTestInvisible);
		ShadowBorder.Get()->SetVisibility(ESlateVisibility::HitTestInvisible);
		ShadowText.Get()->SetVisibility(ESlateVisibility::HitTestInvisible);
		ButtonText.Get()->SetVisibility(ESlateVisibility::Hidden);
	}
#if WITH_EDITOR
	if(PreviewAsFound)
	{
		SetUnlocked();
	}
#endif
	TArray<FName> RowNames = GameInstance->GuncleNotesDataTable.Get()->GetRowNames();
	for (auto Row : RowNames)
	{
		FGuncleNotes_Struct* NoteData = GameInstance->GuncleNotesDataTable.Get()->FindRow<FGuncleNotes_Struct>(Row,"");
		if( NoteID == NoteData->ID)
		{
			ButtonText.Get()->SetText(NoteData->NoteTitle);
		}
	}
}

void UNoteNameButton::SetUnlocked()
{
	this->SetVisibility(ESlateVisibility::Visible);
	ShadowBorder.Get()->SetVisibility(ESlateVisibility::Hidden);
	ShadowText.Get()->SetVisibility(ESlateVisibility::Hidden);
	ButtonText.Get()->SetVisibility(ESlateVisibility::Visible);
}

void UNoteNameButton::NativeOnHovered()
{
	Super::NativeOnHovered();
	OnButtonHovered.Broadcast(NoteID);
}


