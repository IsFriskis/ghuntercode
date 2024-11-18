// TinyTavern

#pragma once

#include "CoreMinimal.h"
#include "TinyTavernGame/UI/Menus/Buttons/ButtonBase.h"
#include "NoteNameButton.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnButtonHoveredSiganture ,int, NoteID);

class UTextBlock;
class UBorder;
/**
 * 
 */
UCLASS()
class TINYTAVERNGAME_API UNoteNameButton : public UButtonBase
{
	GENERATED_BODY()
public:
	virtual void NativePreConstruct() override;
	virtual void NativeConstruct() override;
	void SetUnlocked();
	virtual void NativeOnHovered() override;

	UPROPERTY(BlueprintAssignable)
	FOnButtonHoveredSiganture OnButtonHovered;
	
protected:
	UPROPERTY(VisibleAnywhere,BlueprintReadWrite, meta = (BindWidget))
	TObjectPtr<UBorder> ShadowBorder;
	UPROPERTY(VisibleAnywhere,BlueprintReadWrite, meta = (BindWidget))
	TObjectPtr<UTextBlock> ShadowText;
	//UPROPERTY(VisibleAnywhere,BlueprintReadWrite, meta = (BindWidget))
	//TObjectPtr<UTextBlock> NoteNameText;
	
	UPROPERTY(EditAnywhere, Category = "TT")
	int NoteID = 0;
	UPROPERTY(EditAnywhere, Category = "TT")
	bool PreviewAsFound = false;

	bool Unlocked = false;
};
