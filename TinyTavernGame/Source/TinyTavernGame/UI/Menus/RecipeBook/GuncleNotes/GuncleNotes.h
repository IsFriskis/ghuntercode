// TinyTavern

#pragma once

#include "CoreMinimal.h"
#include "TinyTavernGame/UI/Menus/TT_CommonActivatableWidget.h"
#include "GuncleNotes.generated.h"

class UCommonActionWidget;
class UTextBlock;
class UImage;
class UNoteNameButton;
class UNoteButton;
class UVerticalBox;

/**
 * 
 */
UCLASS()
class TINYTAVERNGAME_API UGuncleNotes : public UTT_CommonActivatableWidget
{
	GENERATED_BODY()
	
public:
	virtual void NativePreConstruct() override;
	virtual void NativeConstruct() override;
	virtual void NativeDestruct() override;
	virtual void NativeOnActivated() override;
	virtual UWidget* NativeGetDesiredFocusTarget() const override;
	UFUNCTION()
	void UpdateNoteInfo(int NoteID);

protected:
	UPROPERTY(VisibleAnywhere,BlueprintReadWrite, meta = (BindWidget))
	TObjectPtr<UVerticalBox> GuncleVerticalBox;
	UPROPERTY(VisibleAnywhere,BlueprintReadWrite, meta = (BindWidget))
	TObjectPtr<UVerticalBox> GuncleVerticalBox2;
	UPROPERTY(VisibleAnywhere,BlueprintReadWrite, meta = (BindWidget))
	TObjectPtr<UVerticalBox> GaliasVerticalBox;
	UPROPERTY(VisibleAnywhere,BlueprintReadWrite, meta = (BindWidget))
	TObjectPtr<UVerticalBox> GaliasVerticalBox2;
	UPROPERTY(VisibleAnywhere,BlueprintReadWrite, meta = (BindWidget))
	TObjectPtr<UVerticalBox> OthersVerticalBox;
	UPROPERTY(VisibleAnywhere,BlueprintReadWrite, meta = (BindWidget))
	TObjectPtr<UNoteButton> NoteButton;
	UPROPERTY(VisibleAnywhere,BlueprintReadWrite, meta = (BindWidget))
	TObjectPtr<UNoteNameButton> FirstButton;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	TObjectPtr<UNoteButton> BiggerNote;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	TObjectPtr<UImage> BiggerButtonBackground;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	TObjectPtr<UTextBlock> BiggerText;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	TObjectPtr<UCommonActionWidget> BiggerAction;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="TT| Note")
	TObjectPtr<UDataTable> TextStyleDataTable;

	//For Backing with Gamepad
	UPROPERTY(EditDefaultsOnly, Category = "TT")
	FDataTableRowHandle TranscibeInputActionData;
	FUIActionBindingHandle TranscibeHandle;
	
private:
	UPROPERTY()
	TArray<UWidget*> Buttons;
	bool NoteIsBig = false;
	UFUNCTION()
	void TranscibeNote();
};
