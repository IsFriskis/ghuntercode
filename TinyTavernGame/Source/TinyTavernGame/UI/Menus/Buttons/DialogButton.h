// TinyTavern

#pragma once

#include "CoreMinimal.h"
#include "ArticyFlowPlayer.h"
#include "ButtonBase.h"
#include "CommonTextBlock.h"
#include "DialogButton.generated.h"

class UImage;
class UNPCDialog;
class USizeBox;
class UTT_GameInstance;
class UTutorial_Dialogue_1st_DraftDialogueFragment;
/**
 * 
 */
UCLASS()
class TINYTAVERNGAME_API UDialogButton : public UButtonBase
{
	GENERATED_BODY()

public:
	void SetNPCDialog(TObjectPtr<UNPCDialog> _NPCDialog);
	void AssignText(FArticyBranch _Branch, TObjectPtr<UArticyFlowPlayer> _PlayerArticyFlowPlayer, TObjectPtr<UTutorial_Dialogue_1st_DraftDialogueFragment> _ArticyTutorialDialogueFragment);
	void BranchAlreadySelected();
	virtual void NativeOnHovered() override;
	virtual void NativeOnUnhovered() override;
	
protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (BindWidget))
	TObjectPtr<USizeBox> DialogSizeBox;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (BindWidget))
	TObjectPtr<UImage> ArrowImage;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UArticyFlowPlayer* PlayerArticyFlowPlayer;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FArticyBranch Branch;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "TT| Dialog Settings")
	FSlateColor NormalColor;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "TT| Dialog Settings")
	FSlateColor HoverColor;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "TT| Dialog Settings")
	FSlateColor AlreadyReadColor;

	virtual void NativeConstruct() override;
	virtual void NativeOnClicked() override;

private:
	TObjectPtr<UTT_GameInstance> TTGameInstance;
	TObjectPtr<UTutorial_Dialogue_1st_DraftDialogueFragment> ArticyTutorialDialogueFragment;
	TObjectPtr<UNPCDialog> NPCDialog;
	FText BranchText;
	bool AlreadyRead = false;
};
