// TinyTavern

#pragma once

#include "CoreMinimal.h"
#include "TinyTavernGame/UI/Menus/TT_CommonActivatableWidget.h"
#include "LookOutUI.generated.h"

class UButtonBase;
class UCanvasPanelSlot;
class UCanvasPanel;
/**
 * 
 */
UCLASS()
class TINYTAVERNGAME_API ULookOutUI : public UTT_CommonActivatableWidget
{
	GENERATED_BODY()

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (BindWidget))
	TObjectPtr<UCanvasPanel> MovableLookOutPanel;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (BindWidget))
	TObjectPtr<UCanvasPanel> MovePanel;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "TT| LookOut Settings")
	float MoveAmount = 100.0f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "TT| LookOut Settings")
	FVector2D MinPosition = FVector2d(-1920.0f, -1080.0f);
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "TT| LookOut Settings")
	FVector2D MaxPosition = FVector2d(0,0);

	virtual void NativeConstruct() override;
	virtual void NativeDestruct() override;
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	TObjectPtr<UButtonBase> MoveLeftButton;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	TObjectPtr<UButtonBase> MoveRightButton;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	TObjectPtr<UButtonBase> MoveUpButton;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	TObjectPtr<UButtonBase> MoveDownButton;
	
private:
	TObjectPtr<UCanvasPanelSlot> MovableLookOutPanelSlot;
	FVector2d LookOutPanelPosition = FVector2d(-960.0f,-540.0f);
	bool IsHoldingLeft = false;
	bool IsHoldingRight = false;
	bool IsHoldingUp = false;
	bool IsHoldingDown = false;
	void MoveLeft();
	void MoveRight();
	void MoveUp();
	void MoveDown();
	
	UFUNCTION()
	void HoldLeft();
	UFUNCTION()
	void ReleaseLeft();
	UFUNCTION()
	void HoldRight();
	UFUNCTION()
	void ReleaseRight();
	UFUNCTION()
	void HoldUp();
	UFUNCTION()
	void ReleaseUp();
	UFUNCTION()
	void HoldDown();
	UFUNCTION()
	void ReleaseDown();
};
