// TinyTavern

#pragma once

#include "CoreMinimal.h"
#include "TinyTavernGame/UI/Menus/Buttons/ButtonBase.h"
#include "TPButton.generated.h"

DECLARE_MULTICAST_DELEGATE_OneParam(FOnTPButtonClicked, class UTPButton* Button);

class UTexture2D;
class UImage;
/**
 * 
 */
UCLASS()
class TINYTAVERNGAME_API UTPButton : public UButtonBase
{
	GENERATED_BODY()
	
public:
	virtual void NativeConstruct() override;
	void SetAsUnlocked();
	void SetAsCurrentTP();
	bool IsUnlocked();
	int GetID();
	virtual void NativeOnClicked() override;
	
	FOnTPButtonClicked OnTpButtonClicked;
	
protected:
	UPROPERTY(VisibleAnywhere,BlueprintReadWrite, meta = (BindWidget))
	TObjectPtr<UImage> TPImage;
	
	UPROPERTY(EditAnywhere,Category = "TT")
	int ID;
	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category = "TT")
	TObjectPtr<UTexture2D> CurrentTPTexture;

	UPROPERTY(EditAnywhere,BlueprintReadWrite, Category = "TT")
	bool IsCurrentTP = false;
	
private:
	bool Unlocked = false;
};
