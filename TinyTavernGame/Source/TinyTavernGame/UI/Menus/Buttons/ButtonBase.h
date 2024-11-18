// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CommonButtonBase.h"
#include "ButtonBase.generated.h"

class UCommonTextBlock;
/**
 * 
 */
UCLASS()
class TINYTAVERNGAME_API UButtonBase : public UCommonButtonBase
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable)
	void SetTextOfButton(FText NewText);
	
protected:
	UPROPERTY(Transient, meta = (BindWidgetAnim))
	UWidgetAnimation* HoverAnimation;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (BindWidget))
	TObjectPtr<UCommonTextBlock> ButtonText;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "TT| Special Configuration")
	FText TextOfButton;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "TT| Special Configuration")
	bool HasAnimationOnHover = false;
	//0 to loop indefinitely
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "TT| Special Configuration")
	int HoverAnimationLoops;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "TT| Special Configuration")
	int ButtonIndex;
	virtual void NativePreConstruct() override;
	virtual void NativeOnHovered() override;
	virtual void NativeOnUnhovered() override;
};
