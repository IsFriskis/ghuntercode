// TinyTavern

 // “MyNamespace”

#include "LoadingScreenWidget.h"

#define LOCTEXT_NAMESPACE "UMG"


void ULoadingScreenWidget::SynchronizeProperties()
{
	Super::SynchronizeProperties();
//	LoadingSlate->SetBackgroundBrush(&BackgroundBrush);
}
void ULoadingScreenWidget::ReleaseSlateResources(bool bReleaseChildren)
{
	//LoadingSlate.Reset();
}

TSharedRef<SWidget> ULoadingScreenWidget::RebuildWidget()
{/*
	LoadingSlate = SNew(SLoading)
		.BackgroundBrush(&BackgroundBrush);
	
	return LoadingSlate.ToSharedRef();
	*/
	return SNullWidget::NullWidget;
}
#if WITH_EDITOR
const FText ULoadingScreenWidget::GetPaletteCategory()
{
	return LOCTEXT("CustomPalletCategory", "TinyTavern");
}
#endif

