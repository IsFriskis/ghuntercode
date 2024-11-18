// TinyTavern


#include "RecipeBook.h"

#include "CommonActivatableWidgetSwitcher.h"
#include "RecipeBookTabList.h"
#include "Components/Image.h"
#include "TinyTavernGame/Player/UIPlayer/UIPlayer.h"
#include "TinyTavernGame/Player/TTPlayerController.h"
#include "Input/CommonUIInputTypes.h"
#include "TinyTavernGame/Inventory/Inventory.h"
#include "TinyTavernGame/UI/Menus/Options/Options.h"
#include "Widgets/CommonActivatableWidgetContainer.h"
#include "CommonInputSubsystem.h"
#include "Kismet/GameplayStatics.h"

void URecipeBook::NativeConstruct()
{
	Super::NativeConstruct();
	
	//Try Get Player
	UIPlayer = Cast<AUIPlayer>(GetOwningPlayerPawn());
	Player = Cast<ATTPlayerController>(GetOwningPlayer());
	if(Player == nullptr)
	{
		LocalPlayer = UGameplayStatics::GetPlayerController(GetWorld(), 0)->GetLocalPlayer();
	}
	else
	{
		LocalPlayer = Player.Get()->GetLocalPlayer();
	}
	//Back Handler
	BackHandle = RegisterUIActionBinding(FBindUIActionArgs(BackInputActionData, true, FSimpleDelegate::CreateUObject(this, &URecipeBook::BackLogic)));

	//Previous and Next Index Input Handler
	PreviousInputHandle = RegisterUIActionBinding(FBindUIActionArgs(PreviousInputActionData,false,FSimpleDelegate::CreateUObject(this, &URecipeBook::PreviousIndex)));
	NextInputHandle = RegisterUIActionBinding(FBindUIActionArgs(NextInputActionData,false,FSimpleDelegate::CreateUObject(this, &URecipeBook::NextIndex)));
	
	UpdateIndexButtons(CurrentIndex);
	LBImage.Get()->SetVisibility(ESlateVisibility::Hidden);
	RBImage.Get()->SetVisibility(ESlateVisibility::Hidden);
}


void URecipeBook::NativeOnDeactivated()
{
	Super::NativeOnDeactivated();
	//Remove Bindings
	BackHandle.Unregister();
	PreviousInputHandle.Unregister();
	NextInputHandle.Unregister();
}

void URecipeBook::NativeDestruct()
{
	Super::NativeDestruct();
	//Remove Bindings
	BackHandle.Unregister();
	PreviousInputHandle.Unregister();
	NextInputHandle.Unregister();
}

void URecipeBook::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);
	
	if(UCommonInputSubsystem::Get(LocalPlayer)->GetCurrentInputType() == ECommonInputType::Gamepad)
	{
		LBImage.Get()->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
		RBImage.Get()->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
	}
	else
	{
		LBImage.Get()->SetVisibility(ESlateVisibility::Hidden);
		RBImage.Get()->SetVisibility(ESlateVisibility::Hidden);
	}
}

void URecipeBook::BackLogic()
{
	ClosePauseMenu();
}

void URecipeBook::PreviousIndex()
{
	PlaySound(PreviousPageSound);
	--CurrentIndex;
	if(CurrentIndex < 0)
	{
	CurrentIndex = RecipeBookSwitcher.Get()->GetNumWidgets()-1;
	}
	//CurrentIndex = FMath::Clamp(CurrentIndex,0,RecipeBookSwitcher.Get()->GetNumWidgets()-1);
	UpdateIndexButtons(CurrentIndex);
}

void URecipeBook::NextIndex()
{
	PlaySound(NextPageSound);
	++CurrentIndex;
	if(CurrentIndex >  RecipeBookSwitcher.Get()->GetNumWidgets()-1)
	{
		CurrentIndex = 0;
	}
	CurrentIndex = FMath::Clamp(CurrentIndex,0,RecipeBookSwitcher.Get()->GetNumWidgets()-1);
	UpdateIndexButtons(CurrentIndex);
}

void URecipeBook::ClosePauseMenu()
{
	UE_LOG(LogTemp, Warning, TEXT("Some warning message") );
	if(Player)
	{
		Player.Get()->OpenSettingsMenu();
	}
	else if(UIPlayer)
	{
		UIPlayer.Get()->OpenSettingsMenu();
	}
}

void URecipeBook::RecipeBookInTavern()
{
	//Show another confirm text in options of recipe book
	//if recipe book is in tavern
	Options.Get()->SetIsInTavern();
}

