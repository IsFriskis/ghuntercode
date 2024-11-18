// TinyTavern

#pragma once

#include "CoreMinimal.h"
#include "ArticyBaseTypes.h"
#include "ButtonBase.h"
#include "NPCQuestionButton.generated.h"

class UNPCDialog;
class UArticyFlowPlayer;
class UImage;
/**
 * 
 */
UCLASS()
class TINYTAVERNGAME_API UNPCQuestionButton : public UButtonBase
{
	GENERATED_BODY()

public:
	void SetNPCDialog(TObjectPtr<UNPCDialog> _NPCDialog);
	void SetDialogID(FArticyId _DialogID){DialogID = _DialogID;}
	void SetFlowPlayer(TObjectPtr<UArticyFlowPlayer> _PlayerArticyFlowPlayer){PlayerArticyFlowPlayer = _PlayerArticyFlowPlayer;}
	void SetNPCDialogParent(TObjectPtr<UNPCDialog> _NPCParent){NPCParent = _NPCParent;}
	
protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (BindWidget))
	TObjectPtr<UImage> ArrowImage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "TT| Dialog Settings")
	FSlateColor NormalColor;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "TT| Dialog Settings")
	FSlateColor HoverColor;
	
	virtual void NativeOnClicked() override;
	virtual void NativeOnHovered() override;
	virtual void NativeOnUnhovered() override;
	
private:
	FArticyId DialogID;
	TObjectPtr<UArticyFlowPlayer> PlayerArticyFlowPlayer;
	TObjectPtr<UNPCDialog> NPCParent;
	FText QuestionText;
	TObjectPtr<UNPCDialog> NPCDialog;
};
