// TinyTavern


#include "RecipeBookTabList.h"

#include "CommonActionWidget.h"
#include "CommonButtonBase.h"
#include "Components/HorizontalBox.h"
#include "Components/HorizontalBoxSlot.h"
#include "Components/SizeBox.h"

void URecipeBookTabList::NativePreConstruct()
{
	Super::NativePreConstruct();
	
	PreviousTabInput.Get()->SetInputAction(PreviousTabInputActionData);
	NextTabInput.Get()->SetInputAction(NextTabInputActionData);
}

void URecipeBookTabList::NativeConstruct()
{
	Super::NativeConstruct();
	OnTabSelected.AddDynamic(this, &URecipeBookTabList::TabSelectedEvent);
	OnVisibilityChanged.AddDynamic(this, &URecipeBookTabList::TabVisibilityChangedEvent);
	
	RefreshNextPrevVisibility();
}

void URecipeBookTabList::NativeDestruct()
{
	Super::NativeDestruct();
	
	OnTabSelected.RemoveAll(this);
	OnVisibilityChanged.RemoveAll(this);
}

void URecipeBookTabList::HandleTabCreation_Implementation(FName TabNameID, UCommonButtonBase* TabButton)
{
	Super::HandleTabCreation_Implementation(TabNameID, TabButton);
	UpdateTabStyles(TabButton);
}

void URecipeBookTabList::HandleTabRemoval_Implementation(FName TabNameID, UCommonButtonBase* TabButton)
{
	Super::HandleTabRemoval_Implementation(TabNameID, TabButton);
	RefreshNextPrevVisibility();
}

void URecipeBookTabList::TabSelectedEvent(FName TabID)
{
	RefreshNextPrevVisibility();
}

void URecipeBookTabList::TabVisibilityChangedEvent(ESlateVisibility InVisibility)
{
	RefreshNextPrevVisibility();
}

void URecipeBookTabList::RefreshNextPrevVisibility()
{
	if(GetTabCount() <= 1)
	{
		PreviousTabWrapper.Get()->SetVisibility(ESlateVisibility::Hidden);
		NextTabWrapper.Get()->SetVisibility(ESlateVisibility::Hidden);
	}
	else
	{
		PreviousTabWrapper.Get()->SetVisibility(ESlateVisibility::Visible);
		NextTabWrapper.Get()->SetVisibility(ESlateVisibility::Visible);
	}
}

void URecipeBookTabList::UpdateTabStyles(UCommonButtonBase* TabButton)
{
	TObjectPtr<UHorizontalBoxSlot> newTabButtonBox = TabButtonBox.Get()->AddChildToHorizontalBox(Cast<UWidget>(TabButton));
	newTabButtonBox.Get()->SetPadding(TabButtonMargin);
	TabButton->SetMinDimensions(MinTabWidth,0);
	if(UseButtonStyle)
	{
		TabButton->SetStyle(TabButtonStyle);
	}
	RefreshNextPrevVisibility();
	
}
