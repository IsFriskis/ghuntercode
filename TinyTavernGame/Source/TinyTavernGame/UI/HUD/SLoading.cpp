#include "SLoading.h"
#include "SlateOptMacros.h"

BEGIN_SLATE_FUNCTION_BUILD_OPTIMIZATION

void SLoading::Construct(const FArguments& InArgs)
{
/*
	BackgroundBrush = InArgs._BackgroundBrush;
	Sequence = FCurveSequence();

	ChildSlot
	[
		SNew(SOverlay)
		+ SOverlay::Slot()
		.HAlign(HAlign_Center)
		.VAlign(VAlign_Center)
		[
			SNew(SImage)
			.Image(BackgroundBrush)
		]
	];
	
	CurveHandle = Sequence.AddCurve(0.f, 60.f);
	Sequence.Play(AsShared(), true, 0.f, false);
	*/
}

int32 SLoading::OnPaint(const FPaintArgs& Args, const FGeometry& AllottedGeometry, const FSlateRect& MyCullingRect,
	FSlateWindowElementList& OutDrawElements, int32 LayerId, const FWidgetStyle& InWidgetStyle,
	bool bParentEnabled) const
{
	/*
	const FGeometry BackgroundImageGeometry = AllottedGeometry.MakeChild(
		BackgroundBrush->ImageSize,
		FSlateLayoutTransform(),
		FSlateRenderTransform(),
		FVector2D(0.5f, 0.5f)
		);

	FSlateDrawElement::MakeBox(
		OutDrawElements,
		LayerId,
		BackgroundImageGeometry.ToPaintGeometry(),
		BackgroundBrush,
		ESlateDrawEffect::None,
		InWidgetStyle.GetColorAndOpacityTint() * BackgroundBrush->GetTint(InWidgetStyle)
		);
	*/
	return LayerId;
}

END_SLATE_FUNCTION_BUILD_OPTIMIZATION