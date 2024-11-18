// TinyTavern

#pragma once

#include "CoreMinimal.h"
#include "TinyTavernGame/UI/Menus/Buttons/ButtonBase.h"
#include "EnemyIndex.generated.h"

class UImage;
/**
 * 
 */
UCLASS()
class TINYTAVERNGAME_API UEnemyIndex : public UButtonBase
{
	GENERATED_BODY()
public:
	UFUNCTION(BlueprintCallable)
	void SetSelected(bool Selected);
	virtual void NativeOnHovered() override;
	virtual void NativeOnUnhovered() override;
	
protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	TObjectPtr<UImage> CompleteIndex;
	UPROPERTY(EditAnywhere, Category = "TT");
	FSlateBrush NormalBrush;
	UPROPERTY(EditAnywhere, Category = "TT");
	FSlateBrush SelectedBrush;
	UPROPERTY(EditAnywhere, Category = "TT");
	FLinearColor HoveredColor;
	UPROPERTY(EditAnywhere, Category = "TT");
	FLinearColor UnhoveredColor;
	UPROPERTY(BlueprintReadWrite);
	bool m_Selected;
};
