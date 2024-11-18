// TinyTavern


#include "CommandUI.h"

#include "CommandRecipeRectangle.h"
#include "CommonTextBlock.h"
#include "QuestBoard.h"
#include "Components/CanvasPanelSlot.h"
#include "Components/Image.h"
#include "Components/Overlay.h"
#include "Components/OverlaySlot.h"
#include "Components/ProgressBar.h"
#include "Components/ScaleBox.h"
#include "Components/SizeBox.h"
#include "TinyTavernGame/TT_GameplayTags.h"
#include "TinyTavernGame/Settings/TT_GameInstance.h"
#include "TinyTavernGame/UI/Menus/Buttons/ButtonBase.h"


void UCommandUI::NativePreConstruct()
{
	Super::NativePreConstruct();
	CommandRecipeTriangle.Get()->SetRenderScale(CommandRecipeTriangleScale);
	CommandTrianglePanelSlot = Cast<UOverlaySlot>(CommandRecipeTriangle.Get()->Slot);
	CommandTrianglePanelSlot.Get()->SetPadding(CommandRecipeTriangleMargin);
	CommandNamePanelSlot = Cast<UOverlaySlot>(CommandName.Get()->Slot);
	CommandNamePanelSlot.Get()->SetPadding(CommandNameMargin);
	CommandSprite.Get()->SetRenderScale(CommandSpriteScale);
	CompletedImagePanelSlot = Cast<UOverlaySlot>(CommandCompletedImage.Get()->Slot);
	CompletedImagePanelSlot.Get()->SetPadding(CompletedImageMargin);
	CommandCompletedImage.Get()->SetRenderScale(CompletedSpriteScale);
	CompletedTextPanelSlot = Cast<UOverlaySlot>(CompletedText.Get()->Slot);
	CompletedTextPanelSlot.Get()->SetPadding(CompletedTextMargin);
	CompletedText.Get()->SetRenderScale(CompletedTextScale);
	CommandName.Get()->SetAutoWrapText(ShouldWrapText);
	SetColorAndOpacity(FLinearColor(1,1,1,1));
	if(ShouldCommandTrianglesStayInLeft)
	{
		CommandTrianglePanelSlot.Get()->SetHorizontalAlignment(HAlign_Fill);
	}
	if(ShouldCommandNameStayInLeft)
	{
		CommandName.Get()->SetJustification(ETextJustify::Left);
	}
}

void UCommandUI::NativeConstruct()
{
	Super::NativeConstruct();
	
	ResetCommand();
}

void UCommandUI::NativeDestruct()
{
	Super::NativeDestruct();
	Orders.Empty();
	LevelsInfo.Empty();
	ImagesOfRecipe.Empty();
	QuestBoard = nullptr;
}

void UCommandUI::OnAnimationFinished_Implementation(const UWidgetAnimation* Animation)
{
	Super::OnAnimationFinished_Implementation(Animation);
	if(Animation == AppearAnimation)
	{
		SetIsEnabled(true);
		if(InDissapearAnimation)
		{
			InDissapearAnimation = false;
			if(PlateID == -1)
			{
				SetIsEnabled(false);
				SetVisibility(ESlateVisibility::Hidden);
				QuestBoard.Get()->UpdateSelectedRequests();
			}
		}
	}
}

void UCommandUI::NativeOnHovered()
{
	Super::NativeOnHovered();
	PlayHoverAnim();
}

void UCommandUI::NativeOnUnhovered()
{
	Super::NativeOnUnhovered();
	StopHoverAnim();
}

void UCommandUI::NativeOnClicked()
{
	Super::NativeOnClicked();
	ClickedCommand();
}

void UCommandUI::ClickedCommand()
{
	OnCommandClicked.Broadcast(this);
}

void UCommandUI::PlayHoverAnim()
{
	OnCommandHover.Broadcast(this);
}

void UCommandUI::StopHoverAnim()
{
	OnCommandUnHover.Broadcast(this);
}

void UCommandUI::SetUpStars(int Amount)
{
	FMargin StarSizeBoxPadding = StarBoxOverlaySlot.Get()->GetPadding();
	StarSizeBoxPadding.Left = DistanceToSubtractDependingOnAmountOfStars[5-Amount];
	StarBoxOverlaySlot.Get()->SetPadding(StarSizeBoxPadding);
	StarBar.Get()->SetPercent(PercentageToRemoveDependingOnStars[5-Amount]);
}

void UCommandUI::ResetStars()
{
	FMargin StarSizeBoxPadding = StarBoxOverlaySlot.Get()->GetPadding();
	StarSizeBoxPadding.Left = DistanceToSubtractDependingOnAmountOfStars[0];
	StarBoxOverlaySlot.Get()->SetPadding(StarSizeBoxPadding);
	StarBar.Get()->SetPercent(1.0f);
}

FSlateBrush UCommandUI::GetAPartOfCommandImage(FGameplayTag TagToSearch)
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

void UCommandUI::SetCommand(int _PlateID)
{
	PlateID = _PlateID;
	CommandRecipeTriangle.Get()->ResetTriangles();
	Ingredient1 = false;
	Ingredient2 = false;
	Ingredient3 = false;
	ResetStars();
	for(FRecipes_Struct* order : Orders)
	{
		if(order->ID == PlateID)
		{
			CommandName.Get()->SetText(order->Name);
			CommandSprite.Get()->SetBrushFromTexture(order->RecipeSprite);
			if(order->Ingredient1_Tag.IsValid())
			{
				Ingredient1 = true;
			}
			if(order->Ingredient2_Tag.IsValid())
			{
				Ingredient2 = true;
			}
			if(order->Ingredient3_Tag.IsValid())
			{
				Ingredient3 = true;
			}
			if(order->Cut_Tag.IsValid())
			{
				CommandRecipeTriangle.Get()->SetCutStyle(order->Cut_Tag);
				CutStyle = order->Cut_Tag;
			}
			if(order->Cook_Tag.IsValid())
			{
				CommandRecipeTriangle.Get()->SetCookStyle(order->Cook_Tag);
				CookStyle = order->Cook_Tag;
			}
			if(order->EnemyName_Tag.IsValid())
			{
				CommandRecipeTriangle.Get()->SetEnemyTag(order->EnemyName_Tag);
				EnemyTag = order->EnemyName_Tag;
			}
			int satisfaction = order->Stars;
			SetUpStars(satisfaction);
			break;
		}
	}
	//Set Triangle of Cut Syle that depends on amount of ingredients
	CommandRecipeTriangle.Get()->SetUpTriangles(Ingredient1, Ingredient2, Ingredient3);
}

void UCommandUI::SetCommandIconVisibility(ESlateVisibility NewVisibility)
{
	CommandCompletedImage.Get()->SetVisibility(NewVisibility);
}

void UCommandUI::SetIsActive(bool _IsActive)
{
	IsActive = _IsActive;
}

void UCommandUI::SetIsCompleted(bool Completed)
{
	CompletedCommand = Completed;
	if(CompletedCommand)
	{
		CommandCompletedImage.Get()->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
		CompletedText.Get()->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
	}
	else
	{
		CommandCompletedImage.Get()->SetVisibility(ESlateVisibility::Hidden);
		CompletedText.Get()->SetVisibility(ESlateVisibility::Hidden);
	}
}

void UCommandUI::LightCommand()
{
	StarBar.Get()->SetFillColorAndOpacity(FLinearColor(1,1,1,1));
	ExtraStar.Get()->SetColorAndOpacity(FLinearColor(1,1,1,1));
	NPCSprite.Get()->SetColorAndOpacity(FLinearColor(1,1,1,1));
	CommandSprite.Get()->SetColorAndOpacity(FLinearColor(1,1,1,1));
	CommandRecipeTriangle.Get()->SetColorAndOpacity(FLinearColor(1,1,1,1));
}

void UCommandUI::SetNPCImage(FSlateBrush NPCImage, FGameplayTag _NPCTAG)
{
	NPCBrush = NPCImage;
	NPCSprite.Get()->SetBrush(NPCImage);
	NPCTAG = _NPCTAG;
	CommandRecipeTriangle.Get()->SetNPC(NPCTAG);
	FProgressBarStyle npcBarFillStyle;
	FSlateBrush invisibleBrush;
	invisibleBrush.TintColor = FSlateColor(FLinearColor(0.0f, 0.0f, 0.0f, 0.0f));
	npcBarFillStyle.BackgroundImage = invisibleBrush;
	SetIsInteractionEnabled(false);
	// SetIsFocusable(false);
	//Extra Star Visibility
	if(TTGameInstance.Get()->SecondaryOrdersExtra.Contains(FSecondaryCommand(NPCTAG, PlateID)))
	{
		ExtraStar.Get()->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
	}
	//Set Stars
	if(NPCTAG == FGameplayTags::Get().NPC_Goctor)
	{
		npcBarFillStyle.FillImage = GoctorStars;
		ExtraStar.Get()->SetBrush(GoctorExtraStar);
	}
	else if(NPCTAG == FGameplayTags::Get().NPC_Gailor)
	{
		npcBarFillStyle.FillImage = GailorStars;
		ExtraStar.Get()->SetBrush(GailorExtraStar);
	}
	else if(NPCTAG == FGameplayTags::Get().NPC_Gmith)
	{
		npcBarFillStyle.FillImage = GmithStars;
		ExtraStar.Get()->SetBrush(GmithExtraStar);
	}
	else if(NPCTAG == FGameplayTags::Get().NPC_Gavern)
	{
		npcBarFillStyle.FillImage = GavernStars;
		ExtraStar.Get()->SetBrush(GavernExtraStar);
	}
	StarBar.Get()->SetWidgetStyle(npcBarFillStyle);
}

void UCommandUI::MakeTornCommand()
{
	//Set Invisible
	TornCommand.Get()->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
	
	IsActive = true;
	GetStyle()->NormalPressed = GetStyle()->NormalHovered;
}

void UCommandUI::MakeMainCommand()
{
	SetVisibility(ESlateVisibility::SelfHitTestInvisible);
	SetIsEnabled(false);
	SetStyle(MainCommandStyle);
	//Royal Images
	//Show if Premium
	if(TTGameInstance.Get()->PremiumMainOrders.Contains(PlateID))
	{
		CommandRecipeTriangle.Get()->MainCommand(true, false, false);
		CommandRecipeTriangle.Get()->IsPremium();
	}
	else
	{
		CommandRecipeTriangle.Get()->MainCommand(Ingredient1, Ingredient2, Ingredient3);
	}
	NPCSprite.Get()->SetVisibility(ESlateVisibility::Hidden);
	StarBar.Get()->SetVisibility(ESlateVisibility::Hidden);
	ExtraStar.Get()->SetVisibility(ESlateVisibility::Hidden);
	CommandRecipeTriangle.Get()->SetIconsToRemainToTheLeft();
	PlayAnimation(AppearAnimation, 0.5f);
}

void UCommandUI::ExecuteAppearAnimation(bool ShouldAppear, bool _InPreview)
{
	if(ShouldAppear)
	{
		StopAnimation(PreviewAnimation);
		SetRenderScale(FVector2d(1.0f, 1.0f));
		InDissapearAnimation = false;
		float time = GetAnimationCurrentTime(AppearAnimation);
		PlayAnimation(AppearAnimation, time, 1);
		if(_InPreview)
		{
			PlayAnimation(PreviewAnimation, 0, 0);
		}
	}
	else
	{
		PlayAnimationReverse(AppearAnimation,1);
		StopAnimation(PreviewAnimation);
		SetRenderScale(FVector2d(1.0f, 1.0f));
		InDissapearAnimation = true;
	}
}

void UCommandUI::EndPreviewAnimation()
{
	StopAnimation(PreviewAnimation);
	SetRenderScale(FVector2d(1.0f, 1.0f));
	InDissapearAnimation = false;
	SetRenderOpacity(1.0f);
}

void UCommandUI::ResetCommand()
{
	IsActive = false;
	CompletedCommand = false;
	InPreview = false;
	//GetGame Instance
	TTGameInstance = Cast<UTT_GameInstance>(GetGameInstance());
	//Recipe DataTable
	RecipeDataTable = TTGameInstance.Get()->RecipeDataTable;
	RecipeDataTable.Get()->GetAllRows("", Orders);
	//Level DataTable
	LevelDataTable.Get()->GetAllRows("", LevelsInfo);
	//Command Images DataTable
	CommandImagesTable->GetAllRows("", ImagesOfRecipe);
	//Reset PlateID
	PlateID = -1;
	Ingredient1 = false;
	Ingredient2 = false;
	Ingredient3 = false;
	//Hide Stars
	StarBoxOverlaySlot = Cast<UOverlaySlot>(StarSizeBox.Get()->Slot);
	ResetStars();
	ExtraStar.Get()->SetVisibility(ESlateVisibility::Hidden);
	//Torn Command Invisible
	TornCommand.Get()->SetVisibility(ESlateVisibility::Hidden);
	//Obtain Current Level Number of Commands
	if(TTGameInstance.Get()->CurrentLevel > 1 && TTGameInstance.Get()->CurrentLevel != 6)
	{
		NumberOfCommandsAvailable = LevelsInfo[TTGameInstance.Get()->CurrentLevel+1]->NumberOfPlayerAvailableCommands;
	}
	//Reset Completed
	CommandCompletedImage.Get()->SetVisibility(ESlateVisibility::Hidden);
	SetIsEnabled(false);
	//Set a random scroll for command
	SetStyle(ScrollStyles[rand() % (ScrollStyles.Num()-1)]);
	CommandColor = FLinearColor(1,1,1,1);
	WhiteCommand();
}

void UCommandUI::SetExtraStar(bool Appear)
{
	if(Appear)
	{
		ExtraStar.Get()->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
	}
}

void UCommandUI::CopyCommandInfo(TObjectPtr<UCommandUI> OtherCommandUI)
{
	OtherCommandUI.Get()->CommandCompletedImage = CommandCompletedImage;
	OtherCommandUI.Get()->CommandCompletedImage.Get()->SetVisibility(CommandCompletedImage.Get()->GetVisibility());
	
	OtherCommandUI.Get()->CommandSprite.Get()->SetBrush(CommandSprite.Get()->GetBrush());

	OtherCommandUI.Get()->SetCommand(PlateID);
	OtherCommandUI.Get()->Ingredient1 = Ingredient1;
	OtherCommandUI.Get()->Ingredient2 = Ingredient2;
	OtherCommandUI.Get()->Ingredient3 = Ingredient3;
	OtherCommandUI.Get()->EnemyTag = EnemyTag;
	OtherCommandUI.Get()->CookStyle = CookStyle;
	OtherCommandUI.Get()->CutStyle = CutStyle;
	OtherCommandUI.Get()->CommandRecipeTriangle.Get()->SetUpTriangles(Ingredient1, Ingredient2, Ingredient3);
	OtherCommandUI.Get()->CommandRecipeTriangle.Get()->SetEnemyTag(EnemyTag);
	OtherCommandUI.Get()->CommandRecipeTriangle.Get()->SetCookStyle(CookStyle);
	OtherCommandUI.Get()->CommandRecipeTriangle.Get()->SetCutStyle(CutStyle);
	OtherCommandUI.Get()->CommandRecipeTriangle.Get()->SetNPC(NPCTAG);
	Ingredient1 = false;
	Ingredient2 = false;
	Ingredient3 = false;
	OtherCommandUI.Get()->IsActive = IsActive;
	OtherCommandUI.Get()->CompletedCommand = CompletedCommand;
	OtherCommandUI.Get()->TornCommand.Get()->SetVisibility(ESlateVisibility::Hidden);
	OtherCommandUI.Get()->NPCTAG = NPCTAG;
	OtherCommandUI.Get()->NPCBrush = NPCBrush;
	OtherCommandUI.Get()->StarBar.Get()->SetWidgetStyle(StarBar.Get()->GetWidgetStyle());
	OtherCommandUI.Get()->ExtraStar.Get()->SetVisibility(ExtraStar.Get()->GetVisibility());
	OtherCommandUI.Get()->ExtraStar.Get()->SetBrush(ExtraStar.Get()->GetBrush());
	OtherCommandUI.Get()->NPCSprite.Get()->SetBrush(NPCBrush);
}

