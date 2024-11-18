// TinyTavern

#pragma once

#include "CoreMinimal.h"
#include "TinyTavernGame/UI/Menus/TT_CommonActivatableWidget.h"
#include "GuncleNoteBig.generated.h"

class UNoteButton;
class UCommonTextBlock;
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FFinishedReadingNote);

class UImage;
/**
 *
 * 
 */
UCLASS()
class TINYTAVERNGAME_API UGuncleNoteBig : public UTT_CommonActivatableWidget
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintAssignable, Category = "TT | Guncle Big Note")
	FFinishedReadingNote OnFinishedReadingNote;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="TT| Guncle Big Note Sound")
	TObjectPtr<USoundBase> NoteSound;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="TT| Guncle Big Note Sound")
	TObjectPtr<USoundBase> EndNoteSound;
	
	void SetBigNoteImage(FSlateBrush NoteBrush, FText NoteText, FText SignatureText);
	
protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	TObjectPtr<UNoteButton> Note;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	TObjectPtr<UNoteButton> BiggerNote;
	UPROPERTY(Transient, meta = (BindWidgetAnim))
	UWidgetAnimation* AppearAnimation;
	
	//For Backing 
	UPROPERTY(EditDefaultsOnly)
	FDataTableRowHandle BackInputActionData;
	FUIActionBindingHandle BackHandle;
	//For Making note
	UPROPERTY(EditDefaultsOnly)
	FDataTableRowHandle BiggerNoteInputActionData;
	FUIActionBindingHandle BiggerNoteHandle;

	virtual void NativeConstruct() override;
	virtual void NativeDestruct() override;
	virtual void OnAnimationFinished_Implementation(const UWidgetAnimation* Animation) override;
	
private:
	bool InShowAnimation = true;
	bool NoteIsBig = false;
	UFUNCTION()
	void BackLogic();
	UFUNCTION()
	void MakeNoteBiggerLogic();
};
