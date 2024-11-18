// TinyTavern

#pragma once

#include "CoreMinimal.h"
#include "Components/Image.h"
#include "TinyTavernGame/Recipes/Recipes_DataTable.h"
#include "TinyTavernGame/UI/Menus/Buttons/ButtonBase.h"
#include "CommandUI.generated.h"


class UOverlaySlot;
class UCommandRecipeRectangle;
class UOverlay;
class UQuestBoard;
class UCanvasPanelSlot;
class USizeBox;
struct FLevelRow;
class UScaleBox;
class UCommonButtonStyle;

USTRUCT()
struct FCommandImage : public FTableRowBase
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (Categories = "Command Tag"))
	FGameplayTag Tag;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (Categories = "Image of Tag"))
	FSlateBrush ImageOfTag;
};

class UCommonTextBlock;
class UProgressBar;
class UTT_GameInstance;
class UButtonBase;
class UImage;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnCommandClicked, UCommandUI*, CommandUI);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnCommandHover, UCommandUI*, CommandUI);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnCommandUnHover, UCommandUI*, CommandUI);
/**
 * 
 */
UCLASS()
class TINYTAVERNGAME_API UCommandUI : public UButtonBase
{
	GENERATED_BODY()

public:
	//Delegate for when accepting a command
	UPROPERTY(BlueprintAssignable, Category = "TT| Command Delegates")
	FOnCommandClicked OnCommandClicked;
	UPROPERTY(BlueprintAssignable, Category = "TT| Command Delegates")
	FOnCommandHover OnCommandHover;
	UPROPERTY(BlueprintAssignable, Category = "TT| Command Delegates")
	FOnCommandUnHover OnCommandUnHover;
	
	//Copy Information of a Command to Another
	void CopyCommandInfo(TObjectPtr<UCommandUI> OtherCommandUI);
	void SetCommand(int _PlateID);
	void SetPlateID(int _PlateID){PlateID = _PlateID;}
	int GetPlateID(){return PlateID;}
	void SetInPreview(bool _InPreview){InPreview = _InPreview;}
	bool IsInPreview(){return InPreview;}
	//TObjectPtr<UButtonBase> GetCommandButton(){return CommandButton;}
	void SetCommandIconVisibility(ESlateVisibility NewVisibility);
	bool IsControllable(){return CommandIsControllable;}
	//Sets if command is active or not and if not active, changes pressed image
	void SetIsActive(bool _IsActive);
	void SetIsCompleted(bool Completed);
	void LightCommand();
	bool IsCommandActive(){return IsActive;}
	bool IsCompleted(){return CompletedCommand;}
	void SetNPCImage(FSlateBrush NPCImage, FGameplayTag _NPCTAG);
	FGameplayTag GetNPCTAG(){return NPCTAG;}
	void MakeTornCommand();
	//Main Command Function
	void MakeMainCommand();
	void ExecuteAppearAnimation(bool ShouldAppear, bool InPreview = false);
	void EndPreviewAnimation();
	void SetQuestBoard(TObjectPtr<UQuestBoard> _QuestBoard)
	{
		QuestBoard = nullptr;
		QuestBoard = _QuestBoard;
	}
	void ResetCommand();
	void SetExtraStar(bool Appear = false);
	void SetNewColor(FLinearColor NewColor){CommandColor = NewColor;};
	void WhiteCommand(){SetColorAndOpacity(FLinearColor(1,1,1,1));}
	void SetCurrentColor(){SetColorAndOpacity(CommandColor);}
	void ShakeCommand(){PlayAnimation(ShakeAnimation);}
	
protected:
	//Animations
	UPROPERTY(Transient, meta = (BindWidgetAnim))
	UWidgetAnimation* AppearAnimation;
	UPROPERTY(Transient, meta = (BindWidgetAnim))
	UWidgetAnimation* PreviewAnimation;
	UPROPERTY(Transient, meta = (BindWidgetAnim))
	TObjectPtr<UWidgetAnimation> ShakeAnimation;
	//This animation shows when command is active
	UPROPERTY(Transient, meta = (BindWidgetAnim))
	UWidgetAnimation* ShowCommandAnimation;
	//Widgets
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	TObjectPtr<USizeBox> CommandSizeBox;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	TObjectPtr<UCommonTextBlock> CommandName;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	TObjectPtr<UImage> CommandCompletedImage;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	TObjectPtr<UCommonTextBlock> CompletedText;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	TObjectPtr<UImage> CommandSprite;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	TObjectPtr<UCommandRecipeRectangle> CommandRecipeTriangle;
	//NPC
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (BindWidget))
	TObjectPtr<UImage> NPCSprite;
	//Stars
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	TObjectPtr<USizeBox> StarSizeBox;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	TObjectPtr<UProgressBar> StarBar;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	TObjectPtr<UImage> ExtraStar;
	//Torn Command
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	TObjectPtr<UImage> TornCommand;
	//Styles for commands
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "TT| Command Sprites")
	TSubclassOf<UCommonButtonStyle> MainCommandStyle;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "TT| Command Sprites")
	TArray<TSubclassOf<UCommonButtonStyle>> ScrollStyles;
	//Brushes
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "TT| Command Sprites")
	FSlateBrush GoctorStars;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "TT| Command Sprites")
	FSlateBrush GoctorExtraStar;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "TT| Command Sprites")
	FSlateBrush GailorStars;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "TT| Command Sprites")
	FSlateBrush GailorExtraStar;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "TT| Command Sprites")
	FSlateBrush GmithStars;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "TT| Command Sprites")
	FSlateBrush GmithExtraStar;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "TT| Command Sprites")
	FSlateBrush GavernStars;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "TT| Command Sprites")
	FSlateBrush GavernExtraStar;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "TT| Command Sprites")
	TArray<float> DistanceToSubtractDependingOnAmountOfStars;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "TT| Command Sprites")
	TArray<float> PercentageToRemoveDependingOnStars;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "TT| Command Settings")
	FVector2D CommandRecipeTriangleScale = FVector2d::One();
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "TT| Command Settings")
	FMargin CommandRecipeTriangleMargin = FMargin(0, 0, 0, -130);
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "TT| Command Settings")
	FMargin CommandNameMargin = FMargin(75, 65, 75, 0);
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "TT| Command Settings")
	FMargin CompletedImageMargin = FMargin(0, 0, 0, 40);
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "TT| Command Settings")
	FMargin CompletedTextMargin = FMargin(0, 0, 0, 40);
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "TT| Command Settings")
	FVector2D CommandSpriteScale = FVector2d::One();
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "TT| Command Settings")
	FVector2D CompletedSpriteScale = FVector2d(3.5f, 3.5f);
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "TT| Command Settings")
	FVector2D CompletedTextScale = FVector2d(1.0f, 1.0f);
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "TT| Command Settings")
	bool ShouldWrapText = true;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "TT| Command Settings")
	bool ShouldCommandTrianglesStayInLeft = false;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "TT| Command Settings")
	bool ShouldCommandNameStayInLeft = false;
	
	//Sounds
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "TT| Command Sounds")
	TObjectPtr<USoundBase> ErrorSound;

	virtual void NativePreConstruct() override;
	virtual void NativeConstruct() override;
	virtual void NativeDestruct() override;
	virtual void OnAnimationFinished_Implementation(const UWidgetAnimation* Animation);
	virtual void NativeOnHovered() override;
	virtual void NativeOnUnhovered() override;
	virtual void NativeOnClicked() override;
	
private:
	//Game Instance
	TObjectPtr<UTT_GameInstance> TTGameInstance;
	TObjectPtr<UDataTable> RecipeDataTable;
	TArray<FRecipes_Struct*> Orders;
	TArray<FLevelRow*> LevelsInfo;
	TArray<FCommandImage*> ImagesOfRecipe;
	
	UPROPERTY(EditAnywhere, Category = "TT| Command Needed Info")
	TObjectPtr<UDataTable> CommandImagesTable;
	UPROPERTY(EditAnywhere, Category = "TT| Command Needed Info")
	TObjectPtr<UDataTable> LevelDataTable;

	TObjectPtr<UQuestBoard> QuestBoard;
	uint16_t NumberOfCommandsAvailable = 6;
	int PlateID;
	bool InPreview = false;
	FSlateBrush NPCBrush;
	bool CommandIsControllable = true;
	bool IsActive = false;
	bool CompletedCommand = false;
	bool Ingredient1 = false;
	bool Ingredient2 = false;
	bool Ingredient3 = false;
	FGameplayTag CookStyle;
	FGameplayTag CutStyle;
	TObjectPtr<UOverlaySlot> StarBoxOverlaySlot;
	//NPC It Belongs to
	FGameplayTag NPCTAG;
	//Canvas Panel Slot of Command Scale Box used for sizing the widget
	TObjectPtr<UCanvasPanelSlot> CommandScaleBoxPanelSlot;
	//Canvas Panel Slot of Commands Triangles and Command Name used for sizing the widget
	TObjectPtr<UOverlaySlot> CommandTrianglePanelSlot;
	TObjectPtr<UOverlaySlot> CommandNamePanelSlot;
	TObjectPtr<UOverlaySlot> CompletedImagePanelSlot;
	TObjectPtr<UOverlaySlot> CompletedTextPanelSlot;
	//Type of Enemy to cook, used for royal command
	FGameplayTag EnemyTag;
	//Command is dissapear animation
	bool InDissapearAnimation = false;
	//Command color used for when command is in finished or accepted orders
	FLinearColor CommandColor = FLinearColor(1,1,1,1);
	UFUNCTION(BlueprintCallable)
	void ClickedCommand();
	UFUNCTION(BlueprintCallable)
	void PlayHoverAnim();
	UFUNCTION(BlueprintCallable)
	void StopHoverAnim();
	//Show Stars
	void SetUpStars(int Amount);
	void ResetStars();
	FSlateBrush GetAPartOfCommandImage(FGameplayTag TagToSearch);
};
