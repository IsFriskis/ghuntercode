// TinyTavern


#include "MiniCommandUI.h"

#include "CommandUI.h"
#include "QuestBoard.h"
#include "Components/CanvasPanel.h"
#include "Components/Image.h"
#include "TinyTavernGame/Recipes/Recipes_DataTable.h"
#include "TinyTavernGame/Settings/TT_GameInstance.h"


void UMiniCommandUI::NativeConstruct()
{
	Super::NativeConstruct();

	//Reset PlateID
	PlateID = -1;
	IsAvailable = true;
	CommandInfoCanvas.Get()->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
	TTGameInstance = Cast<UTT_GameInstance>(GetGameInstance());
	TTGameInstance.Get()->RecipeDataTable->GetAllRows("",Orders);
	TTGameInstance.Get()->LevelDataTable->GetAllRows("", LevelsInfo);
	TTGameInstance.Get()->CommandImagesTable->GetAllRows("", ImagesOfRecipe);
	InDissapearAnimation = false;
	IsUnavailable = false;
	CompletedImage.Get()->SetVisibility(ESlateVisibility::Hidden);
	CompletedImage2.Get()->SetVisibility(ESlateVisibility::Hidden);
	CompletedImage.Get()->SetColorAndOpacity(FLinearColor(1,1,1,1));
	CompletedImage2.Get()->SetColorAndOpacity(FLinearColor(1,1,1,1));
	SetColorAndOpacity(FLinearColor(1,1,1,1));
	SetAnimationCurrentTime(AppearAnimation,0);
	//PlayAppearAnimation();
}

void UMiniCommandUI::NativeDestruct()
{
	Super::NativeDestruct();
	Orders.Empty();
	LevelsInfo.Empty();
	ImagesOfRecipe.Empty();
	QuestBoard = nullptr;
	NPCTAG = FGameplayTag::EmptyTag;
}

void UMiniCommandUI::NativeOnClicked()
{
	Super::NativeOnClicked();
	if(IsAvailable)
	{
		// int numberOfPendingOrders = TTGameInstance.Get()->AcceptedOrders.Num() + TTGameInstance.Get()->FinishedOrders.Num() + QuestBoard.Get()->GetNumberOfCommandsInSelectedRequests();
		int numberOfPendingOrders = QuestBoard.Get()->GetNumberOfCommandsInSelectedRequests();
		if(numberOfPendingOrders < QuestBoard.Get()->GetNumberOfCommandsAvailable())
		{
			if(QuestBoard.Get()->AddToSelectedRequests(this))
			{
				QuestBoard.Get()->HoverSatisfaction(this, false);
				SetIsAvailable(false);
				OnCommandAccepted.Broadcast(this);
			
				// InDissapearAnimation = true;
				//PlayAnimationReverse(AppearAnimation,1);
				//QuestBoard.Get()->RefreshMiniCommandTargetForGamepad();
			}
			else
			{
				//Error Sound
				PlaySound(ErrorSound);
			}
		}
		else
		{
			QuestBoard.Get()->NoMoreSelectedPlatesAnimation();
			PlayAnimation(ShakeAnimation);
			//Error Sound
			PlaySound(ErrorSound);
		}
	}
}

void UMiniCommandUI::NativeOnHovered()
{
	Super::NativeOnHovered();
	if(IsAvailable)
	{
		QuestBoard.Get()->HoverSatisfaction(this, true);
	}
	else
	{
		QuestBoard.Get()->HoverSatisfaction(this, true);
	}
}

void UMiniCommandUI::NativeOnUnhovered()
{
	Super::NativeOnUnhovered();
	if(IsAvailable)
	{
		QuestBoard.Get()->HoverSatisfaction(this, false);
	}
	else
	{
		QuestBoard.Get()->HoverSatisfaction(this, false);
	}
}

void UMiniCommandUI::OnAnimationFinished_Implementation(const UWidgetAnimation* Animation)
{
	Super::OnAnimationFinished_Implementation(Animation);
	if(Animation == AppearAnimation)
	{
		if(InDissapearAnimation)
		{
			SetIsAvailable(false);
			InDissapearAnimation = false;
		}
		else
		{
			if(!IsUnavailable)
			{
				SetIsAvailable(true);
			}
		}
	}
}

void UMiniCommandUI::EstablishCommand(int _PlateID, TObjectPtr<UQuestBoard> _QuestBoard)
{
	PlateID = _PlateID;
	QuestBoard = nullptr;
	QuestBoard = _QuestBoard;
	for (FRecipes_Struct* order : Orders)
	{
		if(order->ID == PlateID)
		{
			//int difficulty = TTGameInstance.Get()->MaxSatisfactionOfACommand / 5;
			int difficulty = order->Stars;
			if(difficulty == 1)
			{
				//1 Star
				StarImage.Get()->SetBrush(OneStarBrush);
			}
			else if(difficulty == 2)
			{
				//2 Stars
				StarImage.Get()->SetBrush(TwoStarBrush);
			}
			else if(difficulty == 3)
			{
				//3 Stars
				StarImage.Get()->SetBrush(ThreeStarBrush);
			}
			else if(difficulty == 4)
			{
				//4 Stars
				StarImage.Get()->SetBrush(FourStarBrush);
			}
			else if(difficulty == 5)
			{
				//5 Stars
				StarImage.Get()->SetBrush(FiveStarBrush);
			}
			break;			
		}
	}
}


void UMiniCommandUI::SetIsAvailable(bool _IsAvailable)
{
	IsAvailable = _IsAvailable;
	if(IsAvailable)
	{
		GetStyle()->NormalPressed = ClickableBrush;
		IsUnavailable = false;
	}
	else
	{
		//SetRenderOpacity(0.0f);
		//SetVisibility(ESlateVisibility::Hidden);
		IsUnavailable = true;
		StopAnimation(AppearAnimation);
		GetStyle()->NormalPressed = NormalBrush;
	}
}

void UMiniCommandUI::PlayAppearAnimation()
{
	SetVisibility(ESlateVisibility::Visible);
	PlayAnimation(AppearAnimation, GetAnimationCurrentTime(AppearAnimation), 1);
}

void UMiniCommandUI::SetMiniCommandCompleted()
{
	CompletedImage.Get()->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
	CompletedImage2.Get()->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
	CompletedImage.Get()->SetColorAndOpacity(CompletedColor);
	CompletedImage2.Get()->SetColorAndOpacity(CompletedColor);
}

void UMiniCommandUI::InvisibleMiniCommand()
{
	StopAnimation(AppearAnimation);
	SetVisibility(ESlateVisibility::Hidden);
}

FSlateBrush UMiniCommandUI::GetAPartOfCommandImage(FGameplayTag TagToSearch)
{
	for(FCommandImage* commandImage : ImagesOfRecipe)
	{
		if(commandImage->Tag == TagToSearch)
		{
			return commandImage->ImageOfTag;
		}
	}
	return FSlateBrush();
}
