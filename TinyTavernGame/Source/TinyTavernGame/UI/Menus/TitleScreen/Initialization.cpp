// TinyTavern


#include "Initialization.h"

#include "TitleScreen.h"

void UInitialization::StartInitialCinematic()
{
	TitleScreen.Get()->AddWidgetToTitleScreenStack(FTitleScreenWidgets::InitialCinematic);
}
