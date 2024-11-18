// TinyTavern


#include "BookMap.h"
#include "TPButton.h"
#include "Animation/WidgetAnimation.h"
#include "Blueprint/WidgetBlueprintLibrary.h"
#include "Components/BackgroundBlur.h"
#include "Components/CanvasPanel.h"
#include "Components/Image.h"
#include "Input/CommonUIInputTypes.h"
#include "TinyTavernGame/TT_GameplayTags.h"
#include "TinyTavernGame/Actor/Structures/TPWithChoices.h"
#include "TinyTavernGame/Player/PlayerCharacter.h"
#include "TinyTavernGame/Settings/TT_GameInstance.h"

void UBookMap::NativeConstruct()
{
	Super::NativeConstruct();
	FryTotemImage.Get()->SetVisibility(ESlateVisibility::Hidden);
	FryTotemOutline.Get()->SetVisibility(ESlateVisibility::Hidden);
	BoilTotemImage.Get()->SetVisibility(ESlateVisibility::Hidden);
	BoilTotemOutline.Get()->SetVisibility(ESlateVisibility::Hidden);
	Blur.Get()->SetVisibility(ESlateVisibility::Hidden);
	
	GameInstance = Cast<UTT_GameInstance>(GetGameInstance());
	FadeInAnimationEvent.BindDynamic(this,&UBookMap::TeleportPlayer);
	BindToAnimationFinished(OnTpFadeInAnimation,FadeInAnimationEvent);
	FadeOutAnimationEvent.BindDynamic(this, &UBookMap::OnFadeOutAnimationFinished);
	BindToAnimationFinished(OnTpFadeOutAnimation,FadeOutAnimationEvent);
	if (GameInstance)
	{
		int cont = 0;
		int ID = GameInstance->GhunterMapPosition;
		MapPositionIcons.Empty();
		for( UWidget* GhunterPosition : GhunterPositionMap->GetAllChildren())
		{
			UImage* Image = Cast<UImage> (GhunterPosition);
			if(Image)
			{
				Image->SetVisibility(ESlateVisibility::Hidden);
				MapPositionIcons.Add(FMapPositionIcons(Image,cont));
				cont++;
			}
		}
		//Detect if player has obtained boil or fry in tavern but not access its totems
        if(GameInstance.Get()->UnlockedWeapons[FGameplayTags::Get().Weapon_CanObtainBoil] && !GameInstance.Get()->UnlockedWeapons[FGameplayTags::Get().Weapon_Range_Boil])
        {
        	BoilTotemImage.Get()->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
        	BoilTotemOutline.Get()->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
        	PlayAnimation(BoilTotemBlink,0,0);
        }
        if(GameInstance.Get()->UnlockedWeapons[FGameplayTags::Get().Weapon_CanObtainFry] && !GameInstance.Get()->UnlockedWeapons[FGameplayTags::Get().Weapon_Range_Fry])
        {
        	FryTotemImage.Get()->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
        	FryTotemOutline.Get()->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
        	PlayAnimation(FryTotemBlink,0,0);
        }
		//Show Ghunter in Map
		for(auto MapPositionIcon : MapPositionIcons)
		{
			if (MapPositionIcon.ID == ID)
			{
				if(MapPositionIcon.ImageMapPosition)
				{
					MapPositionIcon.ImageMapPosition->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
					return;
				}
			}
		}
	
	}
}

void UBookMap::NativeDestruct()
{
	Super::NativeDestruct();
	BackHandle.Unregister();
}

UWidget* UBookMap::NativeGetDesiredFocusTarget() const
{
	for (TObjectPtr<UTPButton> TPButton : TPButtons)
	{
		if(TPButton->IsUnlocked())
		{
			return TPButton;
		}
	}
	return Super::NativeGetDesiredFocusTarget();
}

void UBookMap::ShowButtons(int CurrentTPID)
{
	for (UWidget* Widget : CanvasPanel.Get()->GetAllChildren())
	{
		if(UTPButton* ButtonWidget = Cast<UTPButton>(Widget))
		{
			for (int TP : GameInstance->TPsOpened)
			{
				if(TP == ButtonWidget->GetID())
				{
					ButtonWidget->SetAsUnlocked();
					ButtonWidget->OnTpButtonClicked.AddUObject(this, &UBookMap::OnTPButtonClicked);
				}
				if(CurrentTPID == ButtonWidget->GetID())
				{
					ButtonWidget->SetAsCurrentTP();
				}
			}
			TPButtons.Add(ButtonWidget);
		}
	}
	RequestRefreshFocus();
	RecipeBookImage->SetVisibility(ESlateVisibility::Visible);
	Blur->SetVisibility(ESlateVisibility::Visible);
	SetNavigationOfTPButtons();
}

void UBookMap::SetShouldManageClose()
{
	//Back Handler
	BackHandle = RegisterUIActionBinding(FBindUIActionArgs(BackInputActionData, false, FSimpleDelegate::CreateUObject(this, &UBookMap::BackLogic)));
}

void UBookMap::OnTPButtonClicked(UTPButton* Button)
{
	CanvasPanel.Get()->SetVisibility(ESlateVisibility::Hidden);
	OnButtonClicked.Broadcast();
	PlayAnimation(OnTpFadeInAnimation);
	ClickedButton = Button;
}

void UBookMap::OnFadeOutAnimationFinished()
{
	
	RemoveFromParent();
}

void UBookMap::BackLogic()
{
	this->RemoveFromParent();
}

void UBookMap::TeleportPlayer()
{
	if(ClickedButton && GameInstance && !GameInstance->TPsInWorld.IsEmpty())
	{
		for (TObjectPtr<ATPWithChoices> TP : GameInstance->TPsInWorld)
		{
			if(TP.Get()->ID == ClickedButton.Get()->GetID())
			{
				GetOwningPlayerPawn()->SetActorLocation(TP.Get()->GetTPLocation());
				GetOwningPlayerPawn()->SetActorRotation(TP.Get()->GetTPLocation().Rotation());
				PlayAnimation(OnTpFadeOutAnimation);
				break;
			}
		}
	}
}
//----------------------------------------------------------------------------------------------------------------------------------
//-------------------------------------NAVIGATION-----------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------------------------------------------------

void UBookMap::SetNavigationOfTPButtons()
{
	TP1Navigation();
	TP2Navigation();
	TP3Navigation();
	TP4Navigation();
	TP5Navigation();
	TP6Navigation();
	TP7Navigation();
	TP8Navigation();
	TP9Navigation();
}

void UBookMap::TP1Navigation()
{
	//TP1 Taberna
	if(TP1_Taberna.Get()->IsVisible())
	{
		//First Check Up
		if(TP2_Cascada.Get()->IsVisible())
		{
			TP1_Taberna.Get()->SetNavigationRuleExplicit(EUINavigation::Up,TP2_Cascada);
		}
		else if(TP5_Mercado.Get()->IsVisible())
		{
			TP1_Taberna.Get()->SetNavigationRuleExplicit(EUINavigation::Up,TP5_Mercado);
		}
		//Check Right
		if(TP6_Pozo.Get()->IsVisible())
		{
			TP1_Taberna.Get()->SetNavigationRuleExplicit(EUINavigation::Right,TP6_Pozo);
		}
		else if(TP9_Laberinto.Get()->IsVisible())
		{
			TP1_Taberna.Get()->SetNavigationRuleExplicit(EUINavigation::Right,TP9_Laberinto);
		}
		else if(TP7_Zona3.Get()->IsVisible())
		{
			TP1_Taberna.Get()->SetNavigationRuleExplicit(EUINavigation::Right,TP7_Zona3);
		}
		else if(TP8_CentroZona3.Get()->IsVisible())
		{
			TP1_Taberna.Get()->SetNavigationRuleExplicit(EUINavigation::Right,TP8_CentroZona3);
		}
		//Check Left
		if(TP3_Cruce.Get()->IsVisible())
		{
			TP1_Taberna.Get()->SetNavigationRuleExplicit(EUINavigation::Left,TP3_Cruce);
		}
		else if(TP4_Campamento.Get()->IsVisible())
		{
			TP1_Taberna.Get()->SetNavigationRuleExplicit(EUINavigation::Left,TP4_Campamento);
		}
	}
}

void UBookMap::TP2Navigation()
{
	//TP2 Cascada
	if(TP2_Cascada.Get()->IsVisible())
	{
		//First Check Up
		if(TP5_Mercado.Get()->IsVisible())
		{
			TP2_Cascada.Get()->SetNavigationRuleExplicit(EUINavigation::Up,TP5_Mercado);
		}
		//Check Right
		if(TP6_Pozo.Get()->IsVisible())
		{
			TP2_Cascada.Get()->SetNavigationRuleExplicit(EUINavigation::Right,TP6_Pozo);
		}
		else if(TP9_Laberinto.Get()->IsVisible())
		{
			TP2_Cascada.Get()->SetNavigationRuleExplicit(EUINavigation::Right,TP9_Laberinto);
		}
		else if(TP8_CentroZona3.Get()->IsVisible())
		{
			TP2_Cascada.Get()->SetNavigationRuleExplicit(EUINavigation::Right,TP8_CentroZona3);
		}
		else if(TP7_Zona3.Get()->IsVisible())
		{
			TP2_Cascada.Get()->SetNavigationRuleExplicit(EUINavigation::Right,TP7_Zona3);
		}
		//Check Left
		if(TP3_Cruce.Get()->IsVisible())
		{
			TP2_Cascada.Get()->SetNavigationRuleExplicit(EUINavigation::Left,TP3_Cruce);
		}
		else if(TP4_Campamento.Get()->IsVisible())
		{
			TP2_Cascada.Get()->SetNavigationRuleExplicit(EUINavigation::Left,TP4_Campamento);
		}
		//Check Down
		if(TP1_Taberna.Get()->IsVisible())
		{
			TP2_Cascada.Get()->SetNavigationRuleExplicit(EUINavigation::Down,TP1_Taberna);
		}
	}
}

void UBookMap::TP3Navigation()
{
	//TP3 Cruce
	if(TP3_Cruce.Get()->IsVisible())
	{
		//First Check Up
		if(TP4_Campamento.Get()->IsVisible())
		{
			TP3_Cruce.Get()->SetNavigationRuleExplicit(EUINavigation::Up,TP4_Campamento);
		}
		else if(TP5_Mercado.Get()->IsVisible())
		{
			TP3_Cruce.Get()->SetNavigationRuleExplicit(EUINavigation::Up,TP5_Mercado);
		}
		//Check Right
		if(TP2_Cascada.Get()->IsVisible())
		{
			TP3_Cruce.Get()->SetNavigationRuleExplicit(EUINavigation::Right,TP2_Cascada);
		}
		else if(TP6_Pozo.Get()->IsVisible())
		{
			TP3_Cruce.Get()->SetNavigationRuleExplicit(EUINavigation::Right,TP6_Pozo);
		}
		else if(TP9_Laberinto.Get()->IsVisible())
		{
			TP3_Cruce.Get()->SetNavigationRuleExplicit(EUINavigation::Right,TP9_Laberinto);
		}
		else if(TP8_CentroZona3.Get()->IsVisible())
		{
			TP3_Cruce.Get()->SetNavigationRuleExplicit(EUINavigation::Right,TP8_CentroZona3);
		}
		else if(TP7_Zona3.Get()->IsVisible())
		{
			TP3_Cruce.Get()->SetNavigationRuleExplicit(EUINavigation::Right,TP7_Zona3);
		}
		//Check Down
		if(TP1_Taberna.Get()->IsVisible())
		{
			TP3_Cruce.Get()->SetNavigationRuleExplicit(EUINavigation::Down,TP1_Taberna);
		}
	}
}

void UBookMap::TP4Navigation()
{
	//TP4 Campamento
	if(TP4_Campamento.Get()->IsVisible())
	{
		//Check Right
		if(TP5_Mercado.Get()->IsVisible())
		{
			TP4_Campamento.Get()->SetNavigationRuleExplicit(EUINavigation::Right,TP5_Mercado);
		}
		else if(TP7_Zona3.Get()->IsVisible())
		{
			TP4_Campamento.Get()->SetNavigationRuleExplicit(EUINavigation::Right,TP7_Zona3);
		}
		else if(TP2_Cascada.Get()->IsVisible())
		{
			TP4_Campamento.Get()->SetNavigationRuleExplicit(EUINavigation::Right,TP2_Cascada);
		}
		else if(TP6_Pozo.Get()->IsVisible())
		{
			TP4_Campamento.Get()->SetNavigationRuleExplicit(EUINavigation::Right,TP6_Pozo);
		}
		else if(TP9_Laberinto.Get()->IsVisible())
		{
			TP4_Campamento.Get()->SetNavigationRuleExplicit(EUINavigation::Right,TP9_Laberinto);
		}
		else if(TP8_CentroZona3.Get()->IsVisible())
		{
			TP4_Campamento.Get()->SetNavigationRuleExplicit(EUINavigation::Right,TP8_CentroZona3);
		}
		//Check Down
		if(TP3_Cruce.Get()->IsVisible())
		{
			TP4_Campamento.Get()->SetNavigationRuleExplicit(EUINavigation::Down,TP3_Cruce);
		}
		else if(TP1_Taberna.Get()->IsVisible())
		{
			TP4_Campamento.Get()->SetNavigationRuleExplicit(EUINavigation::Down,TP1_Taberna);
		}
	}
}

void UBookMap::TP5Navigation()
{
	//TP5 Mercado
	if(TP5_Mercado.Get()->IsVisible())
	{
		//Check Right
		if(TP7_Zona3.Get()->IsVisible())
		{
			TP5_Mercado.Get()->SetNavigationRuleExplicit(EUINavigation::Right,TP7_Zona3);
		}
		else if(TP6_Pozo.Get()->IsVisible())
		{
			TP5_Mercado.Get()->SetNavigationRuleExplicit(EUINavigation::Right,TP6_Pozo);
		}
		else if(TP9_Laberinto.Get()->IsVisible())
		{
			TP5_Mercado.Get()->SetNavigationRuleExplicit(EUINavigation::Right,TP9_Laberinto);
		}
		else if(TP8_CentroZona3.Get()->IsVisible())
		{
			TP5_Mercado.Get()->SetNavigationRuleExplicit(EUINavigation::Right,TP8_CentroZona3);
		}
		//Check Left
		if(TP4_Campamento.Get()->IsVisible())
		{
			TP5_Mercado.Get()->SetNavigationRuleExplicit(EUINavigation::Left,TP4_Campamento);
		}
		else if(TP3_Cruce.Get()->IsVisible())
		{
			TP5_Mercado.Get()->SetNavigationRuleExplicit(EUINavigation::Left,TP3_Cruce);
		}
		//Check Down
		if(TP2_Cascada.Get()->IsVisible())
		{
			TP5_Mercado.Get()->SetNavigationRuleExplicit(EUINavigation::Down,TP2_Cascada);
		}
		else if(TP1_Taberna.Get()->IsVisible())
		{
			TP5_Mercado.Get()->SetNavigationRuleExplicit(EUINavigation::Down,TP1_Taberna);
		}
	}
}

void UBookMap::TP6Navigation()
{
	//TP6 Pozo
	if(TP6_Pozo.Get()->IsVisible())
	{
		//First Check Up
		if(TP5_Mercado.Get()->IsVisible())
		{
			TP6_Pozo.Get()->SetNavigationRuleExplicit(EUINavigation::Up,TP5_Mercado);
		}
		else if(TP7_Zona3.Get()->IsVisible())
		{
			TP6_Pozo.Get()->SetNavigationRuleExplicit(EUINavigation::Up,TP7_Zona3);
		}
		//Check Right
		if(TP9_Laberinto.Get()->IsVisible())
		{
			TP6_Pozo.Get()->SetNavigationRuleExplicit(EUINavigation::Right,TP9_Laberinto);
		}
		else if(TP8_CentroZona3.Get()->IsVisible())
		{
			TP6_Pozo.Get()->SetNavigationRuleExplicit(EUINavigation::Right,TP8_CentroZona3);
		}
		//Check Left
		if(TP2_Cascada.Get()->IsVisible())
		{
			TP6_Pozo.Get()->SetNavigationRuleExplicit(EUINavigation::Left,TP2_Cascada);
		}
		else if(TP3_Cruce.Get()->IsVisible())
		{
			TP6_Pozo.Get()->SetNavigationRuleExplicit(EUINavigation::Left,TP3_Cruce);
		}
		else if(TP4_Campamento.Get()->IsVisible())
		{
			TP6_Pozo.Get()->SetNavigationRuleExplicit(EUINavigation::Left,TP4_Campamento);
		}
		//Check Down
		if(TP1_Taberna.Get()->IsVisible())
		{
			TP6_Pozo.Get()->SetNavigationRuleExplicit(EUINavigation::Down,TP1_Taberna);
		}
	}
}

void UBookMap::TP7Navigation()
{
	//TP7 Zona3
	if(TP7_Zona3.Get()->IsVisible())
	{
		//Check Left
		if(TP5_Mercado.Get()->IsVisible())
		{
			TP7_Zona3.Get()->SetNavigationRuleExplicit(EUINavigation::Left,TP5_Mercado);
		}
		else if(TP4_Campamento.Get()->IsVisible())
		{
			TP7_Zona3.Get()->SetNavigationRuleExplicit(EUINavigation::Left,TP4_Campamento);
		}
		else if(TP6_Pozo.Get()->IsVisible())
		{
			TP7_Zona3.Get()->SetNavigationRuleExplicit(EUINavigation::Left,TP6_Pozo);
		}
		else if(TP2_Cascada.Get()->IsVisible())
		{
			TP7_Zona3.Get()->SetNavigationRuleExplicit(EUINavigation::Left,TP2_Cascada);
		}
		else if(TP3_Cruce.Get()->IsVisible())
		{
			TP7_Zona3.Get()->SetNavigationRuleExplicit(EUINavigation::Left,TP3_Cruce);
		}
		else if(TP1_Taberna.Get()->IsVisible())
		{
			TP7_Zona3.Get()->SetNavigationRuleExplicit(EUINavigation::Left,TP1_Taberna);
		}
		//Check Down
		if(TP9_Laberinto.Get()->IsVisible())
		{
			TP7_Zona3.Get()->SetNavigationRuleExplicit(EUINavigation::Down,TP9_Laberinto);
		}
		else if(TP8_CentroZona3.Get()->IsVisible())
		{
			TP7_Zona3.Get()->SetNavigationRuleExplicit(EUINavigation::Down,TP8_CentroZona3);
		}
	}
}

void UBookMap::TP8Navigation()
{
	//TP8 CentroZona3
	if(TP8_CentroZona3.Get()->IsVisible())
	{
		//First Check Up
		if(TP7_Zona3.Get()->IsVisible())
		{
			TP8_CentroZona3.Get()->SetNavigationRuleExplicit(EUINavigation::Up,TP7_Zona3);
		}
		//Check Left
		if(TP9_Laberinto.Get()->IsVisible())
		{
			TP8_CentroZona3.Get()->SetNavigationRuleExplicit(EUINavigation::Left,TP9_Laberinto);
		}
		else if(TP6_Pozo.Get()->IsVisible())
		{
			TP8_CentroZona3.Get()->SetNavigationRuleExplicit(EUINavigation::Left,TP6_Pozo);
		}
		else if(TP2_Cascada.Get()->IsVisible())
		{
			TP8_CentroZona3.Get()->SetNavigationRuleExplicit(EUINavigation::Left,TP2_Cascada);
		}
		else if(TP3_Cruce.Get()->IsVisible())
		{
			TP8_CentroZona3.Get()->SetNavigationRuleExplicit(EUINavigation::Left,TP3_Cruce);
		}
		else if(TP5_Mercado.Get()->IsVisible())
		{
			TP8_CentroZona3.Get()->SetNavigationRuleExplicit(EUINavigation::Left,TP5_Mercado);
		}
		else if(TP4_Campamento.Get()->IsVisible())
		{
			TP8_CentroZona3.Get()->SetNavigationRuleExplicit(EUINavigation::Left,TP4_Campamento);
		}
		//Check Down
		if(TP1_Taberna.Get()->IsVisible())
		{
			TP8_CentroZona3.Get()->SetNavigationRuleExplicit(EUINavigation::Down,TP1_Taberna);
		}
	}
}

void UBookMap::TP9Navigation()
{
	//TP9 Laberinto
	if(TP9_Laberinto.Get()->IsVisible())
	{
		//First Check Up
		if(TP7_Zona3.Get()->IsVisible())
		{
			TP9_Laberinto.Get()->SetNavigationRuleExplicit(EUINavigation::Up,TP7_Zona3);
		}
		//Check Right
		if(TP8_CentroZona3.Get()->IsVisible())
		{
			TP9_Laberinto.Get()->SetNavigationRuleExplicit(EUINavigation::Right,TP8_CentroZona3);
		}
		//Check Left
		if(TP6_Pozo.Get()->IsVisible())
		{
			TP9_Laberinto.Get()->SetNavigationRuleExplicit(EUINavigation::Left,TP6_Pozo);
		}
		else if(TP2_Cascada.Get()->IsVisible())
		{
			TP9_Laberinto.Get()->SetNavigationRuleExplicit(EUINavigation::Left,TP2_Cascada);
		}
		else if(TP3_Cruce.Get()->IsVisible())
		{
			TP9_Laberinto.Get()->SetNavigationRuleExplicit(EUINavigation::Left,TP3_Cruce);
		}
		else if(TP5_Mercado.Get()->IsVisible())
		{
			TP9_Laberinto.Get()->SetNavigationRuleExplicit(EUINavigation::Left,TP5_Mercado);
		}
		else if(TP4_Campamento.Get()->IsVisible())
		{
			TP9_Laberinto.Get()->SetNavigationRuleExplicit(EUINavigation::Left,TP4_Campamento);
		}
		//Check Down
		if(TP1_Taberna.Get()->IsVisible())
		{
			TP9_Laberinto.Get()->SetNavigationRuleExplicit(EUINavigation::Down,TP1_Taberna);
		}
	}
}
