// TinyTavern


#include "QuestsOverlay.h"

#include "CommonActionWidget.h"
#include "Animation/WidgetAnimation.h"
#include "Components/SizeBox.h"
#include "Components/VerticalBox.h"
#include "Kismet/GameplayStatics.h"
#include "TinyTavernGame/TT_GameplayTags.h"
#include "TinyTavernGame/Settings/TT_GameInstance.h"
#include "TinyTavernGame/UI/HUD/AdventureUI/QuestsOverlay/RecipeTicket.h"

void UQuestsOverlay::NativeConstruct()
{
	Super::NativeConstruct();
	HoveredQuest = 0;
	
	//Bind to animations finish to show and collapse RecipeTickets
	OpenMenuAnimationEvent.BindDynamic(this,&UQuestsOverlay::OnMenuOpenAnimationFinished);
	BindToAnimationFinished(OpenMenuAnimation, OpenMenuAnimationEvent);
	CloseMenuAnimationEvent.BindDynamic(this, &UQuestsOverlay::OnMenuCloseAnimationFinished);
	BindToAnimationFinished(CloseMenuAnimation, CloseMenuAnimationEvent);
	//Fill MainOrders array reading the game Instance
	GameInstance = Cast<UTT_GameInstance>(UGameplayStatics::GetGameInstance(GetWorld()));

	//Hide QuestOverlay in Tutorial Levels
	if(GameInstance.Get()->CurrentLevel < 0)
	{
		SetVisibility(ESlateVisibility::Hidden);
	}
	else
	{
		SetVisibility(ESlateVisibility::HitTestInvisible);
	}

	//Fill MainOrdes array by reading the game Instance
	for(const TTuple<int, bool> Order : GameInstance->MainOrders)
	{
		URecipeTicket* Widget = CreateWidget<URecipeTicket>(GetWorld(),RecipeWidgetClass);
		MainOrders.Add(Widget);
		Widget->SetVisibility(ESlateVisibility::Collapsed);
		MainDishesVBox.Get()->AddChildToVerticalBox(Widget);
		Widget->SetRecipe(Order.Key, true, FGameplayTags::Get().NPC_Gmith);
		if(Order.Value)
		{
			Widget->MarkAsCompleted();
		}
	}

	for(FSecondaryCommand Order: GameInstance->FinishedOrders)
	{
		URecipeTicket* Widget = CreateWidget<URecipeTicket>(GetWorld(),RecipeWidgetClass);
		AcceptedOrders.Add(Widget);
		Widget->SetVisibility(ESlateVisibility::Collapsed);
		AcceptedDishesVBox.Get()->AddChildToVerticalBox(Widget);
		Widget->SetRecipe(Order.PlateID, false, Order.NPCTag);
		Widget->MarkAsCompleted();
	}
	
	//Fill AcceptedOrders array reading the game Instance
	for(FSecondaryCommand Order: GameInstance->AcceptedOrders)
	{
		URecipeTicket* Widget = CreateWidget<URecipeTicket>(GetWorld(),RecipeWidgetClass);
		AcceptedOrders.Add(Widget);
		Widget->SetVisibility(ESlateVisibility::Collapsed);
		AcceptedDishesVBox.Get()->AddChildToVerticalBox(Widget);
		Widget->SetRecipe(Order.PlateID, false, Order.NPCTag);
	}

	

	if(AcceptedOrders.IsEmpty())
	{
		AcceptedDishesBanner.Get()->SetVisibility(ESlateVisibility::Collapsed);
	}
	
	GameInstance->OnCompletedRecipe.AddDynamic(this, &UQuestsOverlay::OnRecipeCompleted);
	OpenTicketsAction.Get()->SetInputAction(LeftArrowAction);
}

void UQuestsOverlay::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);
	
}

void UQuestsOverlay::NativeDestruct()
{
	Super::NativeDestruct();
	MainOrders.Empty();
	AcceptedOrders.Empty();
	CloseMenuAnimationEvent.Unbind();
	OpenMenuAnimationEvent.Unbind();
}

void UQuestsOverlay::OpenQuestsMenu()
{
	if(!IsOpen && !OnAnimation)
	{
		OnAnimation = true;
		IsMainOrdersHovered = true;
		IsAnyOrderSelected = false;
		HoveredQuest = 0;
		AddRecipesToMenu();
		PlayAnimation(OpenMainQuestsAnimation);
		OpenTicketsAction.Get()->SetInputAction(RightArrowAction);
	}
}

void UQuestsOverlay::CloseQuestsMenu()
{
	if(IsOpen)
	{
		PlayAnimation(CloseMenuAnimation);
		if(IsMainOrdersHovered && !MainOrders.IsEmpty())
		{
			MainOrders[HoveredQuest].Get()->MarkAsUnhovered(true);
		}
		else if(!AcceptedOrders.IsEmpty())
		{
			AcceptedOrders[HoveredQuest].Get()->MarkAsUnhovered(true);
		}
		if(SelectedOrder)
		{
			SelectedOrder.Get()->MarkAsUnhovered(true);
		}
		HoveredQuest = 0;
		OpenTicketsAction.Get()->SetInputAction(LeftArrowAction);
	}
}

void UQuestsOverlay::HoverNextQuest()
{
	if(!MainOrders.IsEmpty() && IsOpen && !IsAnyOrderSelected)
	{
		if(IsMainOrdersHovered)
		{
			if(HoveredQuest < MainOrders.Num()-1)
			{
				MainOrders[HoveredQuest].Get()->MarkAsUnhovered(true);
				HoveredQuest++;
				MainOrders[HoveredQuest].Get()->MarkAsHovered();
			}
			else if(!AcceptedOrders.IsEmpty())
			{
				MainOrders[HoveredQuest].Get()->MarkAsUnhovered(true);
				IsMainOrdersHovered = false;
				HoveredQuest = 0;
				PlayAnimation(OpenAcceptedQuestsAnimation);
				PlayAnimation(CloseMainQuestsAnimation);
				AddRecipesToMenu();
				OnAnimation = true;
			}
		}
		else if(!AcceptedOrders.IsEmpty())
		{
			if(HoveredQuest < AcceptedOrders.Num()-1)
			{
				AcceptedOrders[HoveredQuest].Get()->MarkAsUnhovered(true);
				HoveredQuest++;
				AcceptedOrders[HoveredQuest].Get()->MarkAsHovered();
			}
			else if(!AcceptedOrders.IsEmpty())
			{
				AcceptedOrders[HoveredQuest].Get()->MarkAsUnhovered(true);
				IsMainOrdersHovered = true;
				HoveredQuest = 0;
				PlayAnimation(OpenMainQuestsAnimation);
				PlayAnimation(CloseAcceptedQuestsAnimation);
				AddRecipesToMenu();
				OnAnimation = true;
			}
		}
	}
}

void UQuestsOverlay::HoverLastQuest()
{
	if(!MainOrders.IsEmpty() && IsOpen && !IsAnyOrderSelected)
	{
		if(IsMainOrdersHovered)
		{
			if(HoveredQuest > 0)
			{
				MainOrders[HoveredQuest].Get()->MarkAsUnhovered(true);
				HoveredQuest--;
				MainOrders[HoveredQuest].Get()->MarkAsHovered();
			}
			else if(!AcceptedOrders.IsEmpty())
			{
				MainOrders[HoveredQuest].Get()->MarkAsUnhovered(true);
				IsMainOrdersHovered = false;
				HoveredQuest = AcceptedOrders.Num()-1;
				PlayAnimation(OpenAcceptedQuestsAnimation);
				PlayAnimation(CloseMainQuestsAnimation);
				AddRecipesToMenu();
				OnAnimation = true;
			}
		}
		else if(!AcceptedOrders.IsEmpty())
		{
			if(HoveredQuest > 0)
			{
				AcceptedOrders[HoveredQuest].Get()->MarkAsUnhovered(true);
				HoveredQuest--;
				AcceptedOrders[HoveredQuest].Get()->MarkAsHovered();
			}
			else if(!MainOrders.IsEmpty())
			{
				AcceptedOrders[HoveredQuest].Get()->MarkAsUnhovered(true);
				IsMainOrdersHovered = true;
				HoveredQuest = MainOrders.Num()-1;
				PlayAnimation(OpenMainQuestsAnimation);
				PlayAnimation(CloseAcceptedQuestsAnimation);
				AddRecipesToMenu();
				OnAnimation = true;
			}
		}	
	}
}

void UQuestsOverlay::SelectQuest()
{
	if(IsOpen)
	{
		if(MainOrders.IsEmpty() && AcceptedOrders.IsEmpty() || IsAnyOrderSelected)
		{
			CloseQuestsMenu();
		}
		else
		{
			if(IsMainOrdersHovered && !MainOrders.IsEmpty())
			{
				SelectedOrder = MainOrders[HoveredQuest];
			}
			else if(!AcceptedOrders.IsEmpty())
			{
				SelectedOrder = AcceptedOrders[HoveredQuest];
			}

			for(TObjectPtr<URecipeTicket> Order : MainOrders)
			{
				if(Order != SelectedOrder)
				{
					Order.Get()->SetVisibility(ESlateVisibility::Collapsed);
				}
			}
			for(TObjectPtr<URecipeTicket> Order : AcceptedOrders)
			{
				if(Order != SelectedOrder)
				{
					Order.Get()->SetVisibility(ESlateVisibility::Collapsed);
				}
			}
			MainDishesBanner.Get()->SetVisibility(ESlateVisibility::Collapsed);
			AcceptedDishesBanner.Get()->SetVisibility(ESlateVisibility::Collapsed);
			IsAnyOrderSelected = true;
		}
	}
}

void UQuestsOverlay::AddRecipesToMenu()
{
	if(!MainOrders.IsEmpty())
	{
		if(IsMainOrdersHovered)
		{
			for(URecipeTicket* Order : MainOrders)
			{
				Order->MakeOrderVisible();
				Order->MarkAsUnhovered(false);
			}
			
			if(!MainOrders[HoveredQuest].Get()->IsSelected)
			{
				MainOrders[HoveredQuest].Get()->MarkAsHovered();
			}
			if(ShouldHide)
			{
				for(URecipeTicket* Order : AcceptedOrders)
				{
					Order->MakeOrderVisible();
					Order->SetVisibility(ESlateVisibility::Collapsed);
				}
			}
		}
		else if(!AcceptedOrders.IsEmpty())
		{
			for(URecipeTicket* Order : AcceptedOrders)
			{
				Order->MakeOrderVisible();
				Order->MarkAsUnhovered(false);
			}
			
			if(!AcceptedOrders[HoveredQuest].Get()->IsSelected)
			{
				AcceptedOrders[HoveredQuest].Get()->MarkAsHovered();
			}
			if(ShouldHide)
			{
				for(URecipeTicket* Order : MainOrders)
				{
					Order->SetVisibility(ESlateVisibility::Collapsed);
				}
			}
		}
	}
	IsOpen = true;
	OnAnimation = false;
}

void UQuestsOverlay::OnRecipeCompleted(int ID, FGameplayTag NPCTag, bool PremiumCompleted)
{
	bool RecipeFound = false;
	for (URecipeTicket* Recipe : MainOrders)
	{
		//IfPremium
		if(PremiumCompleted && CheckPremium(ID, Recipe->GetID()))
		{
			Recipe->MarkAsCompleted();
			RecipeFound = true;
			break;
		}
		else if(Recipe->GetID() == ID)
		{
			Recipe->MarkAsCompleted();
			RecipeFound = true;
			break;
		}
	}
	if(!RecipeFound)
	{
		for (URecipeTicket* Recipe : AcceptedOrders)
		{
			if(Recipe->GetID() == ID && Recipe->GetNPCTag() == NPCTag )
			{
				Recipe->MarkAsCompleted();
			}
		}
	}
}

void UQuestsOverlay::HideRecipesInMenu()
{
	if(!MainOrders.IsEmpty())
	{
		for(URecipeTicket* Order : MainOrders)
		{
			Order->MarkAsUnhovered(false);
			Order->SetVisibility(ESlateVisibility::Collapsed);
		}
	}
	if(!AcceptedOrders.IsEmpty())
	{
		for(URecipeTicket* Order : AcceptedOrders)
		{
			Order->MarkAsUnhovered(false);
			Order->SetVisibility(ESlateVisibility::Collapsed);
		}
	}
	
	PlayAnimation(CloseAcceptedQuestsAnimation);
	PlayAnimation(CloseMainQuestsAnimation);
	IsMainOrdersHovered = true;
	IsAnyOrderSelected = false;
	HoveredQuest = 0;
	IsOpen = false;
	SelectedOrder = nullptr;
}

void UQuestsOverlay::OnMenuCloseAnimationFinished()
{
	HideRecipesInMenu();
	MainDishesBanner.Get()->SetVisibility(ESlateVisibility::HitTestInvisible);
	if(!AcceptedOrders.IsEmpty())
	{
		AcceptedDishesBanner.Get()->SetVisibility(ESlateVisibility::HitTestInvisible);
	}
	PlayAnimation(OpenMenuAnimation);
}

void UQuestsOverlay::OnMenuOpenAnimationFinished()
{
	OnAnimation = false;
}

bool UQuestsOverlay::CheckPremium(int PlateID, int RecipePlateID)
{
	for(int i = 0; i < GameInstance.Get()->PremiumMainOrders.Num(); i++)
	{
		if(RecipePlateID == GameInstance.Get()->PremiumMainOrders[i])
		{
			//Premium Information
			int enemyPremiumID = GameInstance.Get()->PremiumMainOrders[i] / 100000;
			int cutPremiumID = (GameInstance.Get()->PremiumMainOrders[i] - 100000) / 10000;
			int cookPremiumID = (GameInstance.Get()->PremiumMainOrders[i] - 110000) / 1000;
			//Plate Information
			int enemyPlateID = PlateID / 100000;
			int cutPlateID = (PlateID - 100000) / 10000;
			int cookPlateID = (PlateID - 110000) / 1000;
			//Compare
			if(enemyPremiumID == enemyPlateID &&
				cutPremiumID == cutPlateID &&
				cookPremiumID == cookPlateID)
			{
				return true;
			}
		}
	}

	return false;
}
