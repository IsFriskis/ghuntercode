// TinyTavern

#pragma once

#include "CoreMinimal.h"
#include "TinyTavernGame/Settings/TT_GameInstance.h"
#include "TinyTavernGame/UI/Menus/TT_CommonActivatableWidget.h"
#include "GavernWheel.generated.h"

class UNPCDialog;
enum EGavernUpgrades : int;

USTRUCT()
struct FSectionOfWheel
{
	GENERATED_BODY()

	float BeginningValue = 0.0f;
	float EndValue = 0.0f;
	EGavernUpgrades GavernUpgrade = SHIELD;
};

class UImage;
/**
 * 
 */
UCLASS()
class TINYTAVERNGAME_API UGavernWheel : public UTT_CommonActivatableWidget
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable)
	void WheelAppears(){PlayAnimation(WheelAppearsAnimation);}
	UFUNCTION(BlueprintCallable)
	void SetGavern(UNPCDialog* _Gavern);
	
protected:
	UPROPERTY(Transient, meta = (BindWidgetAnim))
	TObjectPtr<UWidgetAnimation> WheelAppearsAnimation;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (BindWidget))
	TObjectPtr<UImage> WheelImage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "TT| Gavern Wheel Details")
	int NumberOfSections = 3;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "TT| Gavern Wheel Details")
	float FirstLoopTime = 5;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "TT| Gavern Wheel Details")
	float LoopTime = 0.5f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "TT| Gavern Wheel Details")
	float StoppingTime = 3.0f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "TT| Gavern Wheel Details")
	FAlphaBlend StopWheelBlend;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "TT| Gavern Wheel Details")
	TObjectPtr<USoundBase> InitialSound;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "TT| Gavern Wheel Details")
	TObjectPtr<USoundBase> LoopSound;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "TT| Gavern Wheel Details")
	TObjectPtr<USoundBase> EndSound;

	//Used to back with Gamepad
	UPROPERTY(EditDefaultsOnly)
	FDataTableRowHandle StopWheelInputActionData;
	FUIActionBindingHandle StopWheelHandle;

	virtual void NativeConstruct() override;
	virtual void NativeDestruct() override;
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;

private:
	FAlphaBlend LoopBlend;
	bool FirstLoopDone = false;
	bool PlayerStoppedWheel = false;
	bool WheelStopped = false;
	TArray<FSectionOfWheel> WheelSections;
	FSectionOfWheel SectionItLanded;
	TObjectPtr<UNPCDialog> Gavern;
	TObjectPtr<UAudioComponent> LoopSoundComponent;
	
	UFUNCTION()
	void StopWheel();
	void CheckWheelSectionItLanded();
};
