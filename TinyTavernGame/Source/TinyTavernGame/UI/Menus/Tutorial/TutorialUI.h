// TinyTavern

#pragma once

#include "CoreMinimal.h"
#include "TinyTavernGame/UI/TTUserWidget.h"
#include "TutorialUI.generated.h"

class UHorizontalBox;
class UCommonActionWidget;
class UCommonTextBlock;
class UWrapBox;
/**
 * 
 */
UCLASS()
class TINYTAVERNGAME_API UTutorialUI : public UTTUserWidget
{
	GENERATED_BODY()

public:
	void EstablishTutorialText(FText Text1, FText Text2, FText Text3, FText Text4, FText Text5);
	void EstablishInputActions(FDataTableRowHandle Input1, FDataTableRowHandle Input2, FDataTableRowHandle Input3, FDataTableRowHandle Input4);
	void SetInteractVisibility(ESlateVisibility NewVisibility);
protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (BindWidget))
	TObjectPtr<UWrapBox> TutorialWrapBox;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (BindWidget))
	TObjectPtr<UCommonTextBlock> TutorialTextBlock1;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (BindWidget))
	TObjectPtr<UCommonTextBlock> TutorialTextBlock2;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (BindWidget))
	TObjectPtr<UCommonTextBlock> TutorialTextBlock3;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (BindWidget))
	TObjectPtr<UCommonTextBlock> TutorialTextBlock4;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (BindWidget))
	TObjectPtr<UCommonTextBlock> TutorialTextBlock5;
	//Action Widgets to show Inputs
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (BindWidget))
	TObjectPtr<UCommonActionWidget> ActionWidget1;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (BindWidget))
	TObjectPtr<UCommonActionWidget> ActionWidget2;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (BindWidget))
	TObjectPtr<UCommonActionWidget> ActionWidget3;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (BindWidget))
	TObjectPtr<UCommonActionWidget> ActionWidget4;

	//Interact Button Image
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (BindWidget))
	TObjectPtr<UHorizontalBox> InteractBox;
	
};
