// TinyTavern


#include "EnemyIndex.h"

#include "Components/Image.h"

void UEnemyIndex::SetSelected(bool Selected)
{
	if(Selected)
	{
		CompleteIndex.Get()->SetBrush(SelectedBrush);
		//PlayAnimation(HoverAnimation);
	}
	else
	{
		CompleteIndex.Get()->SetBrush(NormalBrush);
		SetColorAndOpacity(UnhoveredColor);
	}
	m_Selected = Selected;
}

void UEnemyIndex::NativeOnHovered()
{
	Super::NativeOnHovered();
	if(!m_Selected)
	{
		SetColorAndOpacity(HoveredColor);
	}
}

void UEnemyIndex::NativeOnUnhovered()
{
	Super::NativeOnUnhovered();
	if(!m_Selected)
	{
		SetColorAndOpacity(UnhoveredColor);
	}
}
