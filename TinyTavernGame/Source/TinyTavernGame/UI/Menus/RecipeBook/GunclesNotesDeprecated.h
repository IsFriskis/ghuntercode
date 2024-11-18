// TinyTavern

#pragma once

#include "CoreMinimal.h"
#include "TinyTavernGame/UI/Menus/TT_CommonActivatableWidget.h"
#include "TinyTavernGame/UI/Menus/Tavern/NPCS/GuncleNotes_Datatable.h"
#include "GunclesNotesDeprecated.generated.h"

class UNoteButton;
class UGuncleNoteBig;
class UImage;
/**
 * 
 */
UCLASS()
class TINYTAVERNGAME_API UGunclesNotesDeprecated : public UTT_CommonActivatableWidget
{
	GENERATED_BODY()

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (BindWidget))
	TObjectPtr<UNoteButton> Note1;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (BindWidget))
	TObjectPtr<UNoteButton> Note2;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (BindWidget))
	TObjectPtr<UNoteButton> Note3;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (BindWidget))
	TObjectPtr<UNoteButton> Note4;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (BindWidget))
	TObjectPtr<UNoteButton> Note5;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (BindWidget))
	TObjectPtr<UNoteButton> Note6;

	//Guncle Info
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "TT| Guncle Notes")
	TObjectPtr<UDataTable> GuncleNotesDataTable;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "TT| Guncle Notes")
	int IDNote1 = 1;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "TT| Guncle Notes")
	int IDNote2 = 2;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "TT| Guncle Notes")
	int IDNote3 = 3;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "TT| Guncle Notes")
	int IDNote4 = 4;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "TT| Guncle Notes")
	int IDNote5 = 5;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "TT| Guncle Notes")
	int IDNote6 = 6;

	virtual void NativeConstruct() override;
	virtual UWidget* NativeGetDesiredFocusTarget() const override;
	
private:
	TArray<FGuncleNotes_Struct*> GuncleNotes;
};
