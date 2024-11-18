#pragma once

#include "CoreMinimal.h"
#include "Components/Widget.h"

class TINYTAVERNGAME_API SLoading : public SCompoundWidget 
{
public:
	
	SLATE_BEGIN_ARGS(SLoading)
		//	: _BackgroundBrush()
		{
		}
	//	SLATE_ARGUMENT(const FSlateBrush*, BackgroundBrush)
	SLATE_END_ARGS()
	
	void Construct(const FArguments& InArgs);
	
	virtual int32 OnPaint(const FPaintArgs& Args, const FGeometry& AllottedGeometry, const FSlateRect& MyCullingRect,
		FSlateWindowElementList& OutDrawElements, int32 LayerId, const FWidgetStyle& InWidgetStyle,
		bool bParentEnabled) const override;
	
//	void SetBackgroundBrush(const FSlateBrush* _BackgroundBrush){this->BackgroundBrush = _BackgroundBrush;}
private:
	//FCurveSequence Sequence;
//	FCurveHandle CurveHandle;

	//const FSlateBrush* BackgroundBrush;
	


};
