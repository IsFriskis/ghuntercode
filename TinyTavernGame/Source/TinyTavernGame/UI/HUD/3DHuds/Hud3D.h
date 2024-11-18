// TinyTavern

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "TinyTavernGame/UI/TTUserWidget.h"
#include "Hud3D.generated.h"

class UPopups3D;
class UImage;
class UEnemyCookBar;
class UOverlay;
/**
 * 
 */
UCLASS()
class TINYTAVERNGAME_API UHud3D : public UTTUserWidget
{
	GENERATED_BODY()
	
public:

	//Updates value of progress bar
	void UpdateProgressBar(float Value);
	//Resets the value of the progress bar and disables visibility
	void ResetProgressBar();
	//A spinning stun image for X duration
	void ShowStun();
	void HideStun();
	//Get Cook Bar, player will have on invisible
	TObjectPtr<UTTUserWidget> GetCookBar(){ return CookBar; }
	//
	void ShowNotificationPopUp(FGameplayTag NotificationTag);
	
protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (BindWidget))
	TObjectPtr<UTTUserWidget> CookBar;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (BindWidget))
	TObjectPtr<class UProgressBar> ProgressBar3D;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (BindWidget))
	TObjectPtr<UImage> Image3D;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (BindWidget))
	TObjectPtr<UOverlay> PopUpOverlay;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "TT|PopUps")
	TSubclassOf<UPopups3D> PopUpClass;
	//Max Instances of PopUps of same type than can be added 
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "TT|PopUps")
	int AllowedInstancesOfSamePopUp = 3;
	//Time to wait before adding a PopUp Instance of a type that has exceeded AllowedInstancesOfSamePopUp 
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "TT|PopUps")
	int PopUpReloadTime = 2;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "TT|HUD3D")
	float StunRotationSpeed = 64.0f;

	virtual void NativeConstruct() override;
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;

private:
	UFUNCTION()
	void RestartAllowedInstancesOfSamePopUp();
	
	FGameplayTag LastGameplayTag;
	int TimesSamePopUp = 0;
	FTimerHandle Timer;
	float m_Time = 0;
	bool m_StunActive = false;
	float m_StunAngle = 0.0f;
	bool ReloadingPopUp = false;
};
