// TinyTavern

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "Components/Image.h"
#include "TinyTavernGame/UI/Menus/Buttons/ButtonBase.h"
#include "MiniCommandUI.generated.h"

struct FCommandImage;
class UQuestBoard;
struct FLevelRow;
struct FRecipes_Struct;
class UCanvasPanel;
class UTT_GameInstance;
class UImage;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnCommandAcceptedSignature, UMiniCommandUI*, MiniCommandUI);
/**
 * 
 */
UCLASS()
class TINYTAVERNGAME_API UMiniCommandUI : public UButtonBase
{
	GENERATED_BODY()

public:
	//Delegate for when accepting a command
	UPROPERTY(BlueprintAssignable, Category = "TT| Mini Command Delegates")
	FOnCommandAcceptedSignature OnCommandAccepted;

	void SetPlateID(int _PlateID){ PlateID = _PlateID;}
	int GetPlateID(){return PlateID;}
	void EstablishCommand(int _PlateID, TObjectPtr<UQuestBoard> _QuestBoard);
	void SetIsAvailable(bool _IsAvailable);
	bool GetIsAvailable(){return IsAvailable;}
	void SetUnAvailable(bool _IsUnavailable){IsUnavailable = _IsUnavailable;}
	void SetInDissapearAnimation(bool _InDissapearAnimation){InDissapearAnimation = _InDissapearAnimation;}
	void SetNPCImage(FSlateBrush NPCImage, FGameplayTag _NPCTAG)
	{
		NPCBrush = NPCImage;
		NPCStampImage.Get()->SetBrush(NPCImage); NPCTAG = _NPCTAG;
	}
	FGameplayTag GetNPCTAG(){return NPCTAG;}
	void PlayAppearAnimation();
	void SetMiniCommandCompleted();
	void InvisibleMiniCommand();
	
protected:
	//Animation
	UPROPERTY(Transient, meta = (BindWidgetAnim))
	TObjectPtr<UWidgetAnimation> AppearAnimation;
	UPROPERTY(Transient, meta = (BindWidgetAnim))
	TObjectPtr<UWidgetAnimation> ShakeAnimation;
	//Widgets
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	TObjectPtr<UImage> NPCStampImage;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	TObjectPtr<UCanvasPanel> CommandInfoCanvas;
	//Stars
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	TObjectPtr<UImage> StarImage;
	//Completed Image
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	TObjectPtr<UImage> CompletedImage;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	TObjectPtr<UImage> CompletedImage2;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "TT| MiniCommand")
	FLinearColor CompletedColor = FLinearColor(9,9,9,1.0f);
	//Sounds
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "TT| MiniCommand| Sounds")
	TObjectPtr<USoundBase> ErrorSound;
	//Brushes
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "TT| MiniCommand| Brushes")
	FSlateBrush OneStarBrush;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "TT| MiniCommand| Brushes")
	FSlateBrush TwoStarBrush;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "TT| MiniCommand| Brushes")
	FSlateBrush ThreeStarBrush;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "TT| MiniCommand| Brushes")
	FSlateBrush FourStarBrush;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "TT| MiniCommand| Brushes")
	FSlateBrush FiveStarBrush;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "TT| MiniCommand| Brushes")
	FSlateBrush NormalBrush;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "TT| MiniCommand| Brushes")
	FSlateBrush ClickableBrush;
	
	virtual void NativeConstruct() override;
	virtual void NativeDestruct() override;
	virtual void NativeOnClicked() override;
	virtual void NativeOnHovered() override;
	virtual void NativeOnUnhovered() override;
	virtual void OnAnimationFinished_Implementation(const UWidgetAnimation* Animation);
	
private:
	//Game Instance
	TObjectPtr<UTT_GameInstance> TTGameInstance;
	TArray<FRecipes_Struct*> Orders;
	TArray<FLevelRow*> LevelsInfo;
	TArray<FCommandImage*> ImagesOfRecipe;
	TObjectPtr<UQuestBoard> QuestBoard;
	int PlateID = -1;
	FSlateBrush NPCBrush;
	//NPC It Belongs to
	FGameplayTag NPCTAG;
	bool IsAvailable = true;
	bool IsUnavailable = false;
	//Command is dissapear animation
	bool InDissapearAnimation = false;

	FSlateBrush GetAPartOfCommandImage(FGameplayTag TagToSearch);
};
