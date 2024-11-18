// TinyTavern

#pragma once

#include "CoreMinimal.h"
#include "ButtonBase.h"
#include "NPCOptionButton.generated.h"

class UNPCDialog;
class UImage;
/**
 * 
 */
UCLASS()
class TINYTAVERNGAME_API UNPCOptionButton : public UButtonBase
{
	GENERATED_BODY()

public:
	void SetNPCDialog(TObjectPtr<UNPCDialog> _NPCDialog);
	void ChangeNormalColor();
	void SetNormalColor();
	
protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (BindWidget))
	TObjectPtr<UImage> ArrowImage;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "TT| Dialog Settings")
	FSlateColor NormalColor;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "TT| Dialog Settings")
	FSlateColor HoverColor;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "TT| Dialog Settings")
	FSlateColor UpgradeRelicNeededTextColor;

	virtual void NativeConstruct() override;
	virtual void NativeOnHovered() override;
	virtual void NativeOnUnhovered() override;

private:
	FSlateColor NormalColorCopy;
	TObjectPtr<UNPCDialog> NPCDialog;
};
