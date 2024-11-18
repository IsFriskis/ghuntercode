// TinyTavern


#include "FirstTimeInTavernUI.h"

#include "Components/AudioComponent.h"
#include "TinyTavernGame/Settings/TT_GameInstance.h"
#include "TinyTavernGame/Settings/TT_UserSettings.h"
#include "TinyTavernGame/UI/Menus/Tavern/Tavern.h"

void UFirstTimeInTavernUI::NativeConstruct()
{
	Super::NativeConstruct();
	//First Time in the game
	PlayerCurrentMusicVolume = Cast<UTT_GameInstance>(GetGameInstance())->UserSettings.Get()->GetMusicVolume();
	PercentOfPlayerMusicVolume = PlayerCurrentMusicVolume * FinalPercentageOfFirstBlend;
	MusicBlend.SetBlendTime(SecondsOfFirstBlend);
	MusicBlend.SetValueRange(0, PercentOfPlayerMusicVolume);
	MusicBlend.Reset();
}

void UFirstTimeInTavernUI::AddTavern(TObjectPtr<UTavern> _Tavern, TObjectPtr<UAudioComponent> _TavernMusicComponent,
	TObjectPtr<UAudioComponent> _TavernAmbienceComponent)
{
	Tavern = _Tavern;
	TavernMusicComponent = _TavernMusicComponent;
	TavernMusicComponent.Get()->SetVolumeMultiplier(0.0f);
	TavernAmbienceComponent = _TavernAmbienceComponent;
	TavernAmbienceComponent.Get()->SetVolumeMultiplier(0.0f);
	FirstTimeInTavern = true;
}

void UFirstTimeInTavernUI::ShowTavern()
{
	DeactivateWidget();
	Tavern.Get()->AddWidgetToTavernStack(FTavernWidgets::GookerMenu);
}

void UFirstTimeInTavernUI::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);
	if(FirstTimeInTavern)
	{
		MusicBlend.Update(InDeltaTime);
		TavernMusicComponent.Get()->SetVolumeMultiplier(MusicBlend.GetBlendedValue());
		TavernAmbienceComponent.Get()->SetVolumeMultiplier(MusicBlend.GetBlendedValue());
		if(MusicBlend.IsComplete())
		{
			if(OneBlendCompleted)
			{
				FirstTimeInTavern = false;
			}
			else
			{
				MusicBlend.SetBlendTime(SecondsOfSecondBlend);
				MusicBlend.SetValueRange(PercentOfPlayerMusicVolume, PlayerCurrentMusicVolume);
				MusicBlend.Reset();
				OneBlendCompleted = true;
			}
		}
	}
}
