// TinyTavern


#include "NewDishWidget.h"

#include "Components/CanvasPanel.h"
#include "Components/Image.h"
#include "Components/SizeBox.h"
#include "Components/TextBlock.h"
#include "Input/CommonUIInputTypes.h"
#include "Kismet/GameplayStatics.h"
#include "TinyTavernGame/TT_GameplayTags.h"
#include "TinyTavernGame/Recipes/Recipes_DataTable.h"
#include "TinyTavernGame/Settings/TT_GameInstance.h"
#include "TinyTavernGame/UI/Menus/Tavern/QuestBoard/CommandRecipeRectangle.h"
#include "TinyTavernGame/Player/PlayerCharacter.h"

void UNewDishWidget::NativeConstruct()
{
	// Super::NativeConstruct();
	
	GameInstance = Cast<UTT_GameInstance>(GetGameInstance());
	NewDishAnimationEvent.BindDynamic(this, &UNewDishWidget::Dissapear);
	BindToAnimationFinished(NewDishAnimation,NewDishAnimationEvent);
	NewDishDiscoveredAnimationEvent.BindDynamic(this, &UNewDishWidget::OnNewDishDiscoveredFinishAnimation);
	BindToAnimationFinished(NewDishDiscoveredAnimation,NewDishDiscoveredAnimationEvent);
	NewDishDiscoveredCloseAnimationEvent.BindDynamic(this, &UNewDishWidget::Dissapear);
	BindToAnimationFinished(NewDishDiscoveredCloseAnimation,NewDishDiscoveredCloseAnimationEvent);
	NewDishSizeBox.Get()->SetVisibility(ESlateVisibility::Hidden);
	NewDishDiscoveredSizeBox.Get()->SetVisibility(ESlateVisibility::Hidden);
	
	SetVisibility(ESlateVisibility::SelfHitTestInvisible);
	ActivateWidget();
}

void UNewDishWidget::NativeDestruct()
{
	// Super::NativeDestruct();
	BackHandle.Unregister();
	AcceptHandle.Unregister();
}

void UNewDishWidget::SetDishData(int RecipeID)
{
	TArray<FRecipes_Struct*> Orders;
	RecipesDataTable.Get()->GetAllRows("",Orders);
	for (FRecipes_Struct* Order : Orders)
	{
		if(Order->ID == RecipeID)
		{
			DishImage_1.Get()->SetBrushFromTexture(Order->RecipeSprite);
			DishImage.Get()->SetBrushFromTexture(Order->RecipeSprite);
			DishName.Get()->SetText(Order->Name);
			DishName_1.Get()->SetText(Order->Name);
			CurrentOrderName =  Order->Name;
			EmemyTag = Order->EnemyName_Tag;
			CookTag = Order->Cook_Tag;
			CheeseTag = Order->Ingredient1_Tag;
			JamTag = Order->Ingredient2_Tag;
			WineTag = Order->Ingredient3_Tag;
			CutTag = Order->Cut_Tag;
			break;
		}
	}
	for (FString DiscoveredPlate : GameInstance->DiscoveredPlates)
	{
		if(DiscoveredPlate == CurrentOrderName.ToString())
		{
			PlateDiscovered = true;
			break;
		}
	}

	if(CheeseTag.IsValid() && CheeseTag.MatchesTagExact(FGameplayTags::Get().States_Ingredient_Cheese))
	{
		HasCheese = true;
	}
	if(JamTag.IsValid() && JamTag.MatchesTagExact(FGameplayTags::Get().States_Ingredient_Jam))
	{
		HasJam = true;
	}
	if(WineTag.IsValid() && WineTag.MatchesTagExact(FGameplayTags::Get().States_Ingredient_Wine))
	{
		HasWine = true;
	}

	RecipeRectangle.Get()->SetEnemyTag(EmemyTag);
	RecipeRectangle.Get()->SetCutStyle(CutTag);
	RecipeRectangle.Get()->SetCookStyle(CookTag);
	RecipeRectangle.Get()->SetUpTriangles(HasCheese,HasJam,HasWine);
	
	RecipeRectangle_1.Get()->SetEnemyTag(EmemyTag);
	RecipeRectangle_1.Get()->SetCutStyle(CutTag);
	RecipeRectangle_1.Get()->SetCookStyle(CookTag);
	RecipeRectangle_1.Get()->SetUpTriangles(HasCheese,HasJam,HasWine);

	if(GameInstance->MainOrders.Contains(RecipeID))
	{
		if(GameInstance->PremiumMainOrders.Contains(RecipeID))
		{
			RecipeRectangle.Get()->MainCommand(true, false, false);
			RecipeRectangle.Get()->IsPremium();
		
			RecipeRectangle_1.Get()->MainCommand(true, false, false);
			RecipeRectangle_1.Get()->IsPremium();
		}
		else
		{
			RecipeRectangle.Get()->MainCommand(HasCheese,HasJam,HasWine);
			RecipeRectangle_1.Get()->MainCommand(HasCheese,HasJam,HasWine);
		}
	}
	else
	{
		RecipeRectangle.Get()->SetNPC(FGameplayTags::Get().NPC_Gavern);
		RecipeRectangle_1.Get()->SetNPC(FGameplayTags::Get().NPC_Gavern);
	}

	if(PlateDiscovered)
	{
		//If plate discover, do all this so that cursor does not interact with widget
		//and can still shoot
		NewDishCanvas.Get()->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
		DeactivateWidget();
		NewDishSizeBox.Get()->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
		PlayAnimation(NewDishAnimation);
		BackHandle.Unregister();
		AcceptHandle.Unregister();
	}
	else
	{
		//Hide HUD if new Plate Discovered
		if(!PlayerCharacter.Get()->GetHUDIsHidden())
		{
			PlayerCharacter.Get()->SetHUDVisibility(true);
		}
		PlaySound(NewDishDiscoveredSound);
		UGameplayStatics::SetGamePaused(GetWorld(), true);
		GameInstance->DiscoveredPlates.Add(CurrentOrderName.ToString());
		//All plates obtained achievement
		GameInstance.Get()->PlateDiscovered();
		NewDishDiscoveredSizeBox.Get()->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
		PlayAnimation(NewDishDiscoveredAnimation);
		InNewDishDiscoveredAnim = true;
	}
}

void UNewDishWidget::SetPlayer(TObjectPtr<APlayerCharacter> _PlayerCharacter)
{
	PlayerCharacter = _PlayerCharacter;
}

void UNewDishWidget::Dissapear()
{
	RemoveFromParent();
	UGameplayStatics::SetViewportMouseCaptureMode(GetWorld(), EMouseCaptureMode::CaptureDuringMouseDown);
	//First set game mode only to make the cursos dissapear
	FInputModeGameOnly InputModeGameOnly;
	FInputModeGameAndUI InputModeGameAndUI;
	InputModeGameAndUI.SetHideCursorDuringCapture(false);
	UGameplayStatics::GetPlayerController(GetWorld(),0)->SetInputMode(InputModeGameOnly);
	UGameplayStatics::GetPlayerController(GetWorld(),0)->SetInputMode(InputModeGameAndUI);
	UGameplayStatics::GetPlayerController(GetWorld(),0)->SetShowMouseCursor(false);
	//Check if all main orders game been completed
	if(GameInstance.Get()->CurrentLevel > 0  && GameInstance.Get()->CurrentLevel != 6)
	{
		GameInstance.Get()->CheckIfAllMainOrdersHaveBeenCompleted();
	}
}

void UNewDishWidget::OnNewDishDiscoveredFinishAnimation()
{
	InNewDishDiscoveredAnim = false;
	//Back Handler
	BackHandle = RegisterUIActionBinding(FBindUIActionArgs(BackInputActionData, false, FSimpleDelegate::CreateUObject(this, &UNewDishWidget::BackLogic)));
	AcceptHandle = RegisterUIActionBinding(FBindUIActionArgs(AcceptInputActionData, false, FSimpleDelegate::CreateUObject(this, &UNewDishWidget::BackLogic)));
}

void UNewDishWidget::BackLogic()
{
	if (PlayerCharacter)
	{
		if(PlayerCharacter.Get()->GetHUDIsHidden())
		{
			PlayerCharacter.Get()->SetHUDVisibility(false);
		}
	}
	if(!InNewDishDiscoveredAnim && !InNewDishDiscoveredCloseAnim)
	{
		InNewDishDiscoveredCloseAnim = true;
		if(UGameplayStatics::IsGamePaused(GetWorld()))
		{
			UGameplayStatics::SetGamePaused(GetWorld(), false);
			FInputModeGameOnly InputModeGameOnly;
			FInputModeGameAndUI InputModeGameAndUI;
			InputModeGameAndUI.SetHideCursorDuringCapture(true);
			UGameplayStatics::GetPlayerController(GetWorld(),0)->SetInputMode(InputModeGameOnly);
			UGameplayStatics::GetPlayerController(GetWorld(),0)->SetInputMode(InputModeGameAndUI);
		}
		PlayAnimation(NewDishDiscoveredCloseAnimation);
	}
}
