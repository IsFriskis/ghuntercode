// TinyTavern


#include "InventorySlot.h"

#include "CommonRichTextBlock.h"
#include "CommonTextBlock.h"
#include "Components/Image.h"
#include "Components/Overlay.h"
#include "Components/OverlaySlot.h"
#include "Components/SizeBox.h"
#include "Components/ProgressBar.h"
#include "TinyTavernGame/TT_GameplayTags.h"
#include "TinyTavernGame/Settings/TT_GameInstance.h"
#include "TinyTavernGame/UI/Menus/Tavern/QuestBoard/CommandRecipeRectangle.h"

void UInventorySlot::NativeConstruct()
{
	Super::NativeConstruct();

	SetIsEnabled(false);
	StatuePlateTextBlock.Get()->SetVisibility(ESlateVisibility::Hidden);
	//Invisible everything else
	NPCStamp.Get()->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
	PlateImage.Get()->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
	CommandName.Get()->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
	//Reset Stars
	StarBoxOverlaySlot = Cast<UOverlaySlot>(StarSizeBox.Get()->Slot);
	FMargin StarSizeBoxPadding = StarBoxOverlaySlot.Get()->GetPadding();
	StarSizeBoxPadding.Left = DistanceToSubtractDependingOnAmountOfStars[0];
	StarBoxOverlaySlot.Get()->SetPadding(StarSizeBoxPadding);
	StarBar.Get()->SetPercent(1.0f);
	ExtraStar.Get()->SetVisibility(ESlateVisibility::Collapsed);
	StatuePlateTitle.Get()->SetVisibility(ESlateVisibility::Hidden);
	MossImage.Get()->SetVisibility(ESlateVisibility::Hidden);
	CommandCompletedImage.Get()->SetVisibility(ESlateVisibility::Hidden);
	CompletedText.Get()->SetVisibility(ESlateVisibility::Hidden);
	//Set a random scroll for command
	SetStyle(ScrollStyles[rand() % (ScrollStyles.Num()-1)]);
}

void UInventorySlot::SetInventorySlot(int _PlateID, UTexture2D* Plate, bool Ingredient1, bool Ingredient2,
	bool Ingredient3, FGameplayTag EnemyTag, FGameplayTag CookTag, FGameplayTag CutTag, int NumberOfStars,
	FText _CommandName, TObjectPtr<UTT_GameInstance> _TTGameInstance, bool IsInGavernDay)
{
	PlateID = _PlateID;
	TTGameInstance = _TTGameInstance;
	PlateImage.Get()->SetBrushFromTexture(Plate);
	CommandName.Get()->SetText(_CommandName);
	SetStyle(ScrollStyles[rand() % (ScrollStyles.Num()-1)]);

	CommandRecipeTriangle.Get()->SetEnemyTag(EnemyTag);
	CommandRecipeTriangle.Get()->SetCookStyle(CookTag);
	CommandRecipeTriangle.Get()->SetCutStyle(CutTag);
	CommandRecipeTriangle.Get()->SetUpTriangles(Ingredient1, Ingredient2, Ingredient3);
	CommandRecipeTriangle.Get()->SetNPC(FGameplayTags::Get().NPC_Gavern);
	SetUpStars(NumberOfStars, IsInGavernDay);
}

void UInventorySlot::EnableInventorySlot()
{
	SetIsEnabled(true);
}

void UInventorySlot::MakeStatuePlate(FText PlateTitle, FText _PlateText,
	TSubclassOf<UCommonButtonStyle> StatuePlateStyle, TSubclassOf<UCommonTextStyle> StatuePlateTextStyle)
{
	StatuePlateTitle.Get()->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
	StatuePlateTextBlock.Get()->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
	StatuePlateTitle.Get()->SetText(PlateTitle);
	PlateText = _PlateText;
	StatuePlateTextBlock.Get()->SetText(BP_FormatTextForEnter());
	StatuePlateTitle.Get()->SetStyle(StatuePlateTextStyle);
	MossImage.Get()->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
	SetStyle(StatuePlateStyle);
	//Invisible everything else
	NPCStamp.Get()->SetVisibility(ESlateVisibility::Hidden);
	PlateImage.Get()->SetVisibility(ESlateVisibility::Hidden);
	CommandName.Get()->SetVisibility(ESlateVisibility::Hidden);
	CommandRecipeTriangle.Get()->SetVisibility(ESlateVisibility::Hidden);
	StarSizeBox.Get()->SetVisibility(ESlateVisibility::Hidden);
	ExtraStar.Get()->SetVisibility(ESlateVisibility::Hidden);
}

void UInventorySlot::StatuePlatePickedUp(TSubclassOf<UCommonButtonStyle> StatuePlateStyle,
	TSubclassOf<UCommonTextStyle> StatuePlateTextStyle, FText PlateTitle)
{
	SetStyle(StatuePlateStyle);
	NPCStamp.Get()->SetVisibility(ESlateVisibility::Hidden);
	StarSizeBox.Get()->SetVisibility(ESlateVisibility::Hidden);
	ExtraStar.Get()->SetVisibility(ESlateVisibility::Hidden);
	StatuePlateTitle.Get()->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
	StatuePlateTitle.Get()->SetText(PlateTitle);
	StatuePlateTitle.Get()->SetStyle(StatuePlateTextStyle);
	CommandName.Get()->SetStyle(StatuePlateTextStyle);
	CommandRecipeTriangle.Get()->SetNPC(FGameplayTags::Get().NPC_Gailor);
	CommandCompletedImage.Get()->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
	CompletedText.Get()->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
	PlateImage.Get()->SetRenderScale(StatuePlateScale);
}

void UInventorySlot::NativeOnClicked()
{
	Super::NativeOnClicked();

	OnInventorySlotClicked.Broadcast(this);
}

void UInventorySlot::SetUpStars(int Amount, bool IsGavernDay)
{
	FMargin StarSizeBoxPadding = StarBoxOverlaySlot.Get()->GetPadding();
	StarSizeBoxPadding.Left = DistanceToSubtractDependingOnAmountOfStars[5-Amount];
	StarBoxOverlaySlot.Get()->SetPadding(StarSizeBoxPadding);
	StarBar.Get()->SetPercent(PercentageToRemoveDependingOnStars[5-Amount]);
	if(IsGavernDay)
	{
		ExtraStar.Get()->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
	}
}
	