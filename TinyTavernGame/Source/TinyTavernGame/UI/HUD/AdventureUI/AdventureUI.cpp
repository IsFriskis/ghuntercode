// TinyTavern


#include "AdventureUI.h"

#include "CommonTextBlock.h"
#include "HealthOverlay/HealthOverlay.h"
#include "ArmoryOverlay/ArmoryOverlay.h"
#include "Components/Image.h"
#include "Kismet/GameplayStatics.h"
#include "QuestsOverlay/QuestsOverlay.h"
#include "TinyTavernGame/Settings/TT_GameInstance.h"
#include "TinyTavernGame/UI/HUD/OverlayWidgetController.h"
#include "TinyTavernGame/UI/Menus/RecipeBook/RecipeBook.h"
#include "Widgets/CommonActivatableWidgetContainer.h"

void UAdventureUI::NativeConstruct()
{
	Super::NativeConstruct();
	PlayAnimation(BeginPlayAnimation);
	TTGameInstance = Cast<UTT_GameInstance>(GetGameInstance());
	GavernUpgradeImage.Get()->SetVisibility(ESlateVisibility::Hidden);
	CheckGavernDay();
}

void UAdventureUI::WidgetControllerSet_Implementation()
{
	HealthOverlay->SetWidgetController(WidgetController);
	ArmoryOverlay->SetWidgetController(WidgetController);
	
	OverlayWidget = Cast<UOverlayWidgetController>(WidgetController);
	OverlayWidget.Get()->OnHealthChange.AddDynamic(this, &UAdventureUI::OnHealthChange);
	OverlayWidget.Get()->OnQuestMenu.AddDynamic(this, &UAdventureUI::OpenCloseQuestsMenu);
	OverlayWidget.Get()->OnNextOrLastQuestSelection.AddDynamic(this,&UAdventureUI::SelectNextOrLastQuest);
	OverlayWidget.Get()->OnPoisonResistanceChange.AddDynamic(this,&UAdventureUI::OnPoisonResistanceChange);
	OverlayWidget.Get()->OnMaxPoisonResistanceChange.AddDynamic(this,&UAdventureUI::OnMaxPoisonResistanceChange);
	OverlayWidget.Get()->OnHudVisibilityChange.AddDynamic(this,&UAdventureUI::HudVisibilityChange);
	OverlayWidget.Get()->OnInventoryFullChange.AddDynamic(this,&UAdventureUI::InventoryFull);
}

void UAdventureUI::OpenCloseQuestsMenu(bool Open)
{
	if(Open)
	{
		if(!QuestOverlayAlreadyOpen)
		{
			QuestsOverlay.Get()->OpenQuestsMenu();
			QuestOverlayAlreadyOpen = true;
		}
		else
		{
			QuestsOverlay.Get()->CloseQuestsMenu();
			QuestOverlayAlreadyOpen = false;
		}
	}
	else
	{
		QuestsOverlay.Get()->CloseQuestsMenu();
		QuestOverlayAlreadyOpen = false;
		//QuestsOverlay.Get()->SelectQuest();
	}
}

void UAdventureUI::SelectNextOrLastQuest(bool Next)
{
	if(Next)
	{
		QuestsOverlay->HoverNextQuest();
	}
	else
	{
		QuestsOverlay->HoverLastQuest();
	}
}

void UAdventureUI::SelectRecipe()
{
	QuestsOverlay.Get()->SelectQuest();
}

void UAdventureUI::OnPoisonResistanceChange(float newPoisonResistance)
{
	PoisonImage.Get()->SetOpacity(newPoisonResistance/MaxPoisonResistance);
}

void UAdventureUI::OnMaxPoisonResistanceChange(float newMaxPoisonResistance)
{
	MaxPoisonResistance = newMaxPoisonResistance;
}

void UAdventureUI::HudVisibilityChange(bool _ShoulHide)
{
	ShouldHide = _ShoulHide;
	if (ShouldHide)
	{
		SetVisibility(ESlateVisibility::Hidden);
	}
	else
	{
		SetVisibility(ESlateVisibility::SelfHitTestInvisible);
	}
	OverlayWidget.Get()->SetHUDIsHidden(ShouldHide);
}

void UAdventureUI::InventoryFull(bool IsInventoryFull)
{
	if (IsInventoryFull)
	{
		T_InventoryText.Get()->SetText(FText::FromString("Inventory Full"));
	}
	else
	{
		T_InventoryText.Get()->SetText(FText::FromString("Extra Plate +1"));
	}
	PlayAnimation(InventoryAnimation);
}

void UAdventureUI::CheckGavernDay()
{
	if(TTGameInstance.Get()->IsGavernDay)
	{
		switch(TTGameInstance.Get()->GavernUpgrade)
		{
		case NOUPGRADE:
			break;
		case SHIELD:
			GavernUpgradeImage.Get()->SetBrush(ShieldGavernPotion);
			break;
		case EXTRAREPUTATION:
			GavernUpgradeImage.Get()->SetBrush(ExtraReputationGavernPotion);
			break;
		case PUFFBALLRESISTANCE:
			GavernUpgradeImage.Get()->SetBrush(PuffballResistanceGavernPotion);
			break;
		}
		GavernUpgradeImage.Get()->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
	}
}

void UAdventureUI::OnHealthChange(float newHealth)
{
	if(!IsBeginPlay && CurrentHealth > newHealth)
	{
		PlayAnimation(LowHealthAnimation,0,1);
	}
	else
	{
		IsBeginPlay = false;
	}
	CurrentHealth = newHealth;
}

