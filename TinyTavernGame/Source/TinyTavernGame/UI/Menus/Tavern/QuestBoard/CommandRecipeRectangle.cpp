// TinyTavern


#include "CommandRecipeRectangle.h"

#include "Components/Image.h"
#include "Components/Overlay.h"
#include "Components/WrapBox.h"
#include "Components/WrapBoxSlot.h"
#include "TinyTavernGame/TT_GameplayTags.h"

void UCommandRecipeRectangle::NativeConstruct()
{
	Super::NativeConstruct();
	ResetTriangles();
	CutOverlayWrapBoxSlot = Cast<UWrapBoxSlot>(CutOverlay.Get()->Slot);
	PremiumOverlay.Get()->SetVisibility(ESlateVisibility::Collapsed);
}

void UCommandRecipeRectangle::SetUpTriangles(bool Ingredient1, bool Ingredient2, bool Ingredient3)
{
	//Normal Padding for CutOverlay
	FMargin weaponBoxPadding = CutOverlayWrapBoxSlot.Get()->GetPadding();
	weaponBoxPadding.Left = NormalSeparationIfOnlyCutAndCookStyle;
	CutOverlayWrapBoxSlot.Get()->SetPadding(weaponBoxPadding);
	
	if(Ingredient1)
	{
		Ingredient1Overlay.Get()->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
		TriangleIngredient1.Get()->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
		TriangleIngredient1.Get()->SetBrush(CheeseUpImage);
		UpTriangleForWeapon = false;
		if(Ingredient2)
		{
			Ingredient2Overlay.Get()->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
			TriangleIngredient2.Get()->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
			TriangleIngredient2.Get()->SetBrush(JamDownImage);
			UpTriangleForWeapon = true;
			if(Ingredient3)
			{
				Ingredient3Overlay.Get()->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
				TriangleIngredient3.Get()->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
				TriangleIngredient3.Get()->SetBrush(WineUpImage);
				UpTriangleForWeapon = false;
			}
		}
		else if(Ingredient3)
		{
			Ingredient2Overlay.Get()->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
			TriangleIngredient2.Get()->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
			TriangleIngredient2.Get()->SetBrush(WineDownImage);
			Ingredient3Overlay.Get()->SetVisibility(ESlateVisibility::Collapsed);
			TriangleIngredient3.Get()->SetVisibility(ESlateVisibility::Collapsed);
			UpTriangleForWeapon = true;
		}
		
	}
	else if(Ingredient2)
	{
		Ingredient1Overlay.Get()->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
		TriangleIngredient1.Get()->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
		TriangleIngredient1.Get()->SetBrush(JamUpImage);
		Ingredient2Overlay.Get()->SetVisibility(ESlateVisibility::Collapsed);
		TriangleIngredient2.Get()->SetVisibility(ESlateVisibility::Collapsed);
		UpTriangleForWeapon = false;
		if(Ingredient3)
		{
			Ingredient2Overlay.Get()->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
			TriangleIngredient2.Get()->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
			TriangleIngredient2.Get()->SetBrush(WineDownImage);
			Ingredient3Overlay.Get()->SetVisibility(ESlateVisibility::Collapsed);
			TriangleIngredient3.Get()->SetVisibility(ESlateVisibility::Collapsed);
			UpTriangleForWeapon = true;
		}
	}
	else if(Ingredient3)
	{
		Ingredient1Overlay.Get()->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
		TriangleIngredient1.Get()->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
		TriangleIngredient1.Get()->SetBrush(WineUpImage);
		Ingredient2Overlay.Get()->SetVisibility(ESlateVisibility::Collapsed);
		TriangleIngredient2.Get()->SetVisibility(ESlateVisibility::Collapsed);
		Ingredient3Overlay.Get()->SetVisibility(ESlateVisibility::Collapsed);
		TriangleIngredient3.Get()->SetVisibility(ESlateVisibility::Collapsed);
		UpTriangleForWeapon = false;
	}
	else
	{
		//Only Cook And Cut Style Visible
		weaponBoxPadding.Left = MinSeparationIfOnlyCutAndCookStyle;
		CutOverlayWrapBoxSlot.Get()->SetPadding(weaponBoxPadding);
		UpTriangleForWeapon = true;
	}
}

void UCommandRecipeRectangle::SetNPC(FGameplayTag NPCName)
{
	if (NPCName == FGameplayTags::Get().NPC_Goctor)
	{
		SetEnemyCutAndCookStyle(GoctorCommandImages);
	}
	else if (NPCName == FGameplayTags::Get().NPC_Gailor)
	{
		SetEnemyCutAndCookStyle(GailorCommandImages);
	}
	else if (NPCName == FGameplayTags::Get().NPC_Gmith)
	{
		SetEnemyCutAndCookStyle(GmithCommandImages);
	}
	else if (NPCName == FGameplayTags::Get().NPC_Gavern)
	{
		SetEnemyCutAndCookStyle(GavernCommandImages);
	}
}

void UCommandRecipeRectangle::MainCommand(bool Ingredient1, bool Ingredient2, bool Ingredient3)
{
	if(Ingredient1)
	{
		TriangleIngredient1.Get()->SetBrush(RoyalCheeseUpImage);
		UpTriangleForWeapon = false;
		if(Ingredient2)
		{
			TriangleIngredient2.Get()->SetBrush(RoyalJamDownImage);
			UpTriangleForWeapon = true;
			if(Ingredient3)
			{
				TriangleIngredient3.Get()->SetBrush(RoyalWineUpImage);
				UpTriangleForWeapon = false;
			}
		}
		else if(Ingredient3)
		{
			TriangleIngredient3.Get()->SetBrush(RoyalWineDownImage);
			UpTriangleForWeapon = true;
		}
	}
	else if(Ingredient2)
	{
		TriangleIngredient1.Get()->SetBrush(RoyalJamUpImage);
		TriangleIngredient1.Get()->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
		UpTriangleForWeapon = false;
		if(Ingredient3)
		{
			TriangleIngredient2.Get()->SetBrush(RoyalWineDownImage);
			TriangleIngredient2.Get()->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
			UpTriangleForWeapon = true;
		}
	}
	else if(Ingredient3)
	{
		TriangleIngredient1.Get()->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
		TriangleIngredient1.Get()->SetBrush(RoyalWineUpImage);
		UpTriangleForWeapon = false;
	}
	else
	{
		UpTriangleForWeapon = true;
	}
	SetEnemyCutAndCookStyle(MainCommandImages);
}

void UCommandRecipeRectangle::IsPremium()
{
	PremiumOverlay.Get()->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
	Ingredient1Overlay.Get()->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
	TriangleIngredient1.Get()->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
	TriangleIngredient1.Get()->SetBrush(RoyalWhateverIngredientImage);
}

void UCommandRecipeRectangle::ResetTriangles()
{
	Ingredient1Overlay.Get()->SetVisibility(ESlateVisibility::Collapsed);
	TriangleIngredient1.Get()->SetVisibility(ESlateVisibility::Collapsed);
	Ingredient2Overlay.Get()->SetVisibility(ESlateVisibility::Collapsed);
	TriangleIngredient2.Get()->SetVisibility(ESlateVisibility::Collapsed);
	Ingredient3Overlay.Get()->SetVisibility(ESlateVisibility::Collapsed);
	TriangleIngredient3.Get()->SetVisibility(ESlateVisibility::Collapsed);
}

void UCommandRecipeRectangle::SetIconsToRemainToTheLeft()
{
	TriangleBox.Get()->SetHorizontalAlignment(HAlign_Left);
}

void UCommandRecipeRectangle::SetEnemyCutAndCookStyle(FNPCTriangleCommandInfo CommandInfo)
{
	//Enemy
	if(EnemyTag == FGameplayTags::Get().Enemy_Gamusino)
	{
		TriangleEnemy.Get()->SetBrush(CommandInfo.GamusinoImage);
	}
	else if(EnemyTag == FGameplayTags::Get().Enemy_Cucafera)
	{
		TriangleEnemy.Get()->SetBrush(CommandInfo.CucaferaImage);
	}
	else if(EnemyTag == FGameplayTags::Get().Enemy_Basilisco)
	{
		TriangleEnemy.Get()->SetBrush(CommandInfo.BasiliscoImage);
	}
	
	//Set Cut Style
	if(WeaponStyle == FGameplayTags::Get().States_Cuts_Sliced)
	{
		if(UpTriangleForWeapon)
		{
			TriangleWeapon.Get()->SetBrush(CommandInfo.SlicedImageUp);
		}
		else
		{
			TriangleWeapon.Get()->SetBrush(CommandInfo.SlicedImageDown);
		}
	}
	else if(WeaponStyle == FGameplayTags::Get().States_Cuts_Smashed)
	{
		if(UpTriangleForWeapon)
		{
			TriangleWeapon.Get()->SetBrush(CommandInfo.SmashedImageUp);
		}
		else
		{
			TriangleWeapon.Get()->SetBrush(CommandInfo.SmashedImageDown);
		}
	}
	else if(WeaponStyle == FGameplayTags::Get().States_Cuts_Skewered)
	{
		if(UpTriangleForWeapon)
		{
			TriangleWeapon.Get()->SetBrush(CommandInfo.SkeweredImageUp);
		}
		else
		{
			TriangleWeapon.Get()->SetBrush(CommandInfo.SkeweredImageDown);
		}
	}

	//Set Cook Style
	if(CookStyle == FGameplayTags::Get().States_Cook_Grilled)
	{
		TriangleCookStyle.Get()->SetBrush(CommandInfo.GrillImage);
	}
	else if(CookStyle == FGameplayTags::Get().States_Cook_Boiled)
	{
		TriangleCookStyle.Get()->SetBrush(CommandInfo.BoilImage);
	}
	else if(CookStyle == FGameplayTags::Get().States_Cook_Fried)
	{
		TriangleCookStyle.Get()->SetBrush(CommandInfo.FryImage);
	}
}
