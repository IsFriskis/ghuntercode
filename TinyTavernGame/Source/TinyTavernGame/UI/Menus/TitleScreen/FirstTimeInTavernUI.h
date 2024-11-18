// TinyTavern

#pragma once

#include "CoreMinimal.h"
#include "TinyTavernGame/UI/TTUserWidget.h"
#include "TinyTavernGame/UI/Menus/TT_CommonActivatableWidget.h"
#include "FirstTimeInTavernUI.generated.h"

class UTavern;
/**
 * 
 */
UCLASS()
class TINYTAVERNGAME_API UFirstTimeInTavernUI : public UTT_CommonActivatableWidget
{
	GENERATED_BODY()

public:
	void AddTavern(TObjectPtr<UTavern> _Tavern, TObjectPtr<UAudioComponent> _TavernMusicComponent, TObjectPtr<UAudioComponent> _TavernAmbienceComponent);
	
protected:
	UPROPERTY(EditAnywhere, Category = "TT| First Time In Tavern")
	float SecondsOfFirstBlend = 14.25f;
	UPROPERTY(EditAnywhere, Category = "TT| First Time In Tavern")
	float SecondsOfSecondBlend = 2.5f;
	UPROPERTY(EditAnywhere, Category = "TT| First Time In Tavern")
	float FinalPercentageOfFirstBlend = 0.15f;
	UFUNCTION(BlueprintCallable)
	void ShowTavern();
	void NativeConstruct() override;
	void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;
	
private:
	TObjectPtr<UTavern> Tavern;
	bool FirstTimeInTavern = false;
	bool OneBlendCompleted = false;
	float PercentOfPlayerMusicVolume = 0.15f;
	float PlayerCurrentMusicVolume = 1.0f;
	FAlphaBlend MusicBlend;
	TObjectPtr<UAudioComponent> TavernMusicComponent;
	TObjectPtr<UAudioComponent> TavernAmbienceComponent;
};
