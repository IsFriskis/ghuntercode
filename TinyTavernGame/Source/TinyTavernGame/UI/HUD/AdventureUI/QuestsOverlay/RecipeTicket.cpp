// TinyTavern


#include "RecipeTicket.h"

#include "CommonActionWidget.h"
#include "Blueprint/WidgetTree.h"
#include "Components/Button.h"
#include "Components/Image.h"
#include "TinyTavernGame/TT_GameplayTags.h"
#include "TinyTavernGame/Recipes/Recipes_DataTable.h"
#include "TinyTavernGame/Settings/TT_GameInstance.h"
#include "TinyTavernGame/UI/Menus/Tavern/QuestBoard/CommandRecipeRectangle.h"


void URecipeTicket::NativeConstruct()
{
	Super::NativeConstruct();
	FButtonStyle Style = Ticket->GetStyle();
	NormalBrush = Style.Normal;
	HoveredBrush = Style.Hovered;
	PressedBrush = Style.Pressed;
	Check->SetVisibility(ESlateVisibility::Hidden);
	UpDownTicketsAction.Get()->SetVisibility(ESlateVisibility::Hidden);
}

void URecipeTicket::SetRecipe(int ID, bool IsMain, FGameplayTag _NPCTag)
{
	m_ID = ID;
	
	TArray<FName> RowNames = RecipesData->GetRowNames();
	for(const FName Row : RowNames)
	{
		FRecipes_Struct* RecipeData = RecipesData->FindRow<FRecipes_Struct>(Row,"");
		if(RecipeData && RecipeData->ID == ID)
		{
			EmemyTag = RecipeData->EnemyName_Tag;
			CookTag = RecipeData->Cook_Tag;
			CheeseTag = RecipeData->Ingredient1_Tag;
			JamTag = RecipeData->Ingredient2_Tag;
			WineTag = RecipeData->Ingredient3_Tag;
			CutTag = RecipeData->Cut_Tag;
			break;
		}
	}
	
	if(CheeseTag.IsValid() && CheeseTag.MatchesTagExact(FGameplayTags::Get().States_Ingredient_Cheese))
	{
		HasCheese = true;
		NumberOfIngredients++;
	}
	if(JamTag.IsValid() && JamTag.MatchesTagExact(FGameplayTags::Get().States_Ingredient_Jam))
	{
		
		HasJam = true;
		NumberOfIngredients++;
	}
	if(WineTag.IsValid() && WineTag.MatchesTagExact(FGameplayTags::Get().States_Ingredient_Wine))
	{
		HasWine = true;
		NumberOfIngredients++;
	}
	
	UTT_GameInstance* GameInstance = Cast<UTT_GameInstance>(GetGameInstance());
	
	RecipeRectangle.Get()->SetEnemyTag(EmemyTag);
	RecipeRectangle.Get()->SetCutStyle(CutTag);
	RecipeRectangle.Get()->SetCookStyle(CookTag);
	RecipeRectangle.Get()->SetUpTriangles(HasCheese,HasJam,HasWine);
	if(IsMain)
	{
		if(GameInstance->PremiumMainOrders.Contains(ID))
		{
			RecipeRectangle.Get()->MainCommand(true,false,false);
			RecipeRectangle.Get()->IsPremium();
			NumberOfIngredients = 2;
		}
		else
		{
			RecipeRectangle.Get()->MainCommand(HasCheese,HasJam,HasWine);
		}
	}
	else
	{
		// bool IdFound = false;
		// for(TTuple<FGameplayTag, FNPCInformationStruct> Element: GameInstance->NPCInformationMap)
		// {
		// 	for(const int npcPlateID : Element.Value.NPCOrders)
		// 	{
		// 		int x = npcPlateID;
		// 		if(npcPlateID == ID)
		// 		{
		// 			NPCTag = Element.Key;
		// 			IdFound = true;
		// 			goto endSearch;	
		// 		}
		// 	}
		// 	if(IdFound)
		// 	{
		// 		endSearch:
		// 	break;
		// 	}
		// }
		NPCTag = _NPCTag;
		RecipeRectangle.Get()->SetNPC(NPCTag);
	}
}

void URecipeTicket::MarkAsHovered()
{
	switch (NumberOfIngredients)
	{
	case 1:
		PlayAnimation(OnHoveredAnim1Ing);
		break;
	case 2:
		PlayAnimation(OnHoveredAnim2Ing);
		break;
	case 3:
		PlayAnimation(OnHoveredAnim3Ing);
		break;
	default:
		PlayAnimation(OnHoveredAnim);
		break;
	}
	SetUpDownAction(true);
	IsSelected = true;
	FButtonStyle Style;
	Style.Normal = HoveredBrush;
	Ticket->SetStyle(Style);
}

void URecipeTicket::MarkAsUnhovered(bool ShouldPlayAnimation)
{
	if(ShouldPlayAnimation)
	{
		switch (NumberOfIngredients)
		{
			case 1:
				PlayAnimation(OnUnHoveredAnim1Ing);
				break;
			case 2:
				PlayAnimation(OnUnHoveredAnim2Ing);
				break;
			case 3:
				PlayAnimation(OnUnHoveredAnim3Ing);
			break;
			default:
				PlayAnimation(OnUnHoveredAnim);
				break;
		}
		
	}
	SetUpDownAction(false);
	IsSelected = false;
	FButtonStyle Style;
	Style.Normal = NormalBrush;
	Ticket->SetStyle(Style);
}

int URecipeTicket::GetID() const
{
	return m_ID;
}

void URecipeTicket::MarkAsCompleted()
{
	Check->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
}

void URecipeTicket::MakeOrderVisible()
{
	this->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
}

void URecipeTicket::SetUpDownAction(bool Show)
{
	if(Show)
	{
		UpDownTicketsAction.Get()->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
	}
	else
	{
		UpDownTicketsAction.Get()->SetVisibility(ESlateVisibility::Hidden);
	}
}
	