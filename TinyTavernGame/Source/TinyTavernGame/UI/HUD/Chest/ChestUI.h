// TinyTavern

#pragma once

#include "CoreMinimal.h"
#include "TinyTavernGame/Actor/Structures/Chest.h"
#include "TinyTavernGame/UI/Menus/TT_CommonActivatableWidget.h"
#include "ChestUI.generated.h"

class UCommonTextBlock;
class UImage;
/**
 * 
 */
UCLASS()
class TINYTAVERNGAME_API UChestUI : public UTT_CommonActivatableWidget
{
	GENERATED_BODY()

public:
	void SetChestRelic(TObjectPtr<AChest> _Chest, FRelicInfoStruct RelicInfo);

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (BindWidget))
	TObjectPtr<UImage> StampImage;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (BindWidget))
	TObjectPtr<UImage> RelicImage;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (BindWidget))
	TObjectPtr<UCommonTextBlock> RelicName;
	UPROPERTY(Transient, meta = (BindWidgetAnim))
	UWidgetAnimation* RelicAppearAnimation;

	//Chest UI Info
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "TT| Chest UI Info")
	FSlateBrush DamselsSporesBrush;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "TT| Chest UI Info")
	FSlateBrush ZerroleanSilkBrush;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "TT| Chest UI Info")
	FSlateBrush GungstenBarBrush;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "TT| Chest UI Info")
	FSlateBrush GoctorStamp;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "TT| Chest UI Info")
	FSlateBrush GailorStamp;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "TT| Chest UI Info")
	FSlateBrush GmithStamp;
	
	//For Backing with Gamepad
	UPROPERTY(EditDefaultsOnly)
	FDataTableRowHandle BackInputActionData;
	FUIActionBindingHandle BackHandle;

	virtual void NativeConstruct() override;
	virtual void NativeDestruct() override;
	virtual void OnAnimationFinished_Implementation(const UWidgetAnimation* Animation) override;
	
private:
	TObjectPtr<AChest> Chest;
	bool InShowAnimation = true;
	
	UFUNCTION()
	void BackLogic();
};
