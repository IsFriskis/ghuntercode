// TinyTavern


#include "TurnsInvisibleHoldButton.h"

void UTurnsInvisibleHoldButton::NativeConstruct()
{
	Super::NativeConstruct();
	SetRenderOpacity(0.0f);
}

void UTurnsInvisibleHoldButton::BeginHolding()
{
	Super::BeginHolding();
	SetRenderOpacity(1.0f);
}

void UTurnsInvisibleHoldButton::EndHolding()
{
	Super::EndHolding();
	SetRenderOpacity(0.0f);
}
