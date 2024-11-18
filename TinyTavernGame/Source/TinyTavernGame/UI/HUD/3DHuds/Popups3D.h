// TinyTavern

#pragma once

#include "CoreMinimal.h"
#include "TinyTavernGame/UI/TTUserWidget.h"
#include "GameplayTagContainer.h"
#include "Popups3D.generated.h"

class UPopUpsData;
struct FPopUpInfo;
class UImage;
class USizeBox;
/**
 * 
 */
UCLASS()
class TINYTAVERNGAME_API UPopups3D : public UTTUserWidget
{
	GENERATED_BODY()

public:
	virtual void NativeConstruct() override;
	virtual void WidgetControllerSet_Implementation() override;
	virtual void NativeDestruct() override;
	//Displays the widget depending on the Type (Weapon/Ingredient/Inventory)
	void ShowNotification(FGameplayTag NotificationType);
	void HideNotification();
	
protected:
	UFUNCTION()
	void HidePopUp();
	
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (BindWidget))
	TObjectPtr<UImage> NotificationImage;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (BindWidget))
	TObjectPtr<UImage> TimeIndicatorImage;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (BindWidget))
	TObjectPtr<USizeBox> SizeBox;

	//Animations
	UPROPERTY(Transient, meta = (BindWidgetAnim))
	TObjectPtr<UWidgetAnimation> FadeAwayAnimation;
	UPROPERTY(Transient, meta = (BindWidgetAnim))
	TObjectPtr<UWidgetAnimation> FullRenderOpacityAnimation;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "TT|Notifications")
	TObjectPtr<UPopUpsData> PopUpsData;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "TT|Notifications")
	bool TimeBased = false;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "TT|Notifications")
	FGameplayTag m_NotificationType;
	
private:
	FTimerHandle HidingTimer;
	FWidgetAnimationDynamicEvent FadeAwayAnimationEvent;
	FPopUpInfo GetNotificationInfo() const;
	float PoisonResistance = 0.0f;
	float MaxPoisonResistance = 0.0f;
	float Duration = 0.0f;
};
