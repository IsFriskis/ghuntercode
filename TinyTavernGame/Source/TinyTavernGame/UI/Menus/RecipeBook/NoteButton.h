// TinyTavern

#pragma once

#include "CoreMinimal.h"
#include "TinyTavernGame/UI/Menus/Buttons/ButtonBase.h"
#include "NoteButton.generated.h"

class UCommonRichTextBlock;
class UImage;
/**
 * 
 */
UCLASS()
class TINYTAVERNGAME_API UNoteButton : public UButtonBase
{
	GENERATED_BODY()

public:
	void SetNoteInfo(FSlateBrush NoteBrush, FText _NoteText, FText _NoteSignature);
	UFUNCTION(BlueprintImplementableEvent)
	FText BP_FormatTextForEnter();
	UFUNCTION(BlueprintImplementableEvent)
	FText BP_FormatSignatureForEnter();
	void SetTextStyle(UDataTable* DataTable);
	void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;
	void ReverseDissolve();
	
protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (BindWidget))
	TObjectPtr<UImage> NoteImage;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (BindWidget))
	TObjectPtr<UCommonRichTextBlock> NoteTextBlock;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (BindWidget))
	TObjectPtr<UCommonRichTextBlock> SignatureTextBlock;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FText NoteText;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FText NoteSignature;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "TT| Note Button Settings")
	bool InRecipeBook = true;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "TT| Note Button Settings")
	bool BiggerNote = false;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "TT| Note Button Settings")
	TObjectPtr<UMaterialInstance> NoteDissolveMaterial;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "TT| Note Button Settings")
	float NoteDissolveTime = 1.0f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "TT| Note Button Settings")
	float NoteTextAppearTime = 1.0f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "TT| Note Button Settings")
	FAlphaBlend NoteBlend;

private:
	TObjectPtr<UMaterialInstanceDynamic> DynamicMat;
	bool ShouldImageDissolve = false;
	bool ShouldTextAppear = false;
	bool InReverse = false;
};
