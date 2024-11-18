// TinyTavern


#include "IngredientType.h"

#include "Components/Image.h"
#include "Components/ScaleBox.h"
#include "TinyTavernGame/TT_GameplayTags.h"
#include "TinyTavernGame/UI/HUD/OverlayWidgetController.h"
#include "TinyTavernGame/UI/HUD/AdventureUI/ProgressBarPoint.h"

void UIngredientType::WidgetControllerSet_Implementation()
{
	UOverlayWidgetController* OverlayWidget = Cast<UOverlayWidgetController>(WidgetController);
	
	OverlayWidget->OnCheeseAmmoChange.AddDynamic(this, &UIngredientType::UpdateCheeseAmmo);
	OverlayWidget->OnWineAmmoChange.AddDynamic(this, &UIngredientType::UpdateWineAmmo);
	OverlayWidget->OnJamAmmoChange.AddDynamic(this, &UIngredientType::UpdateJamAmmo);
	OverlayWidget->OnMaxCheeseAmmoChange.AddDynamic(this, &UIngredientType::UpdateMaxCheeseAmmo);
	OverlayWidget->OnMaxWineAmmoChange.AddDynamic(this, &UIngredientType::UpdateMaxWineAmmo);
	OverlayWidget->OnMaxJamAmmoChange.AddDynamic(this, &UIngredientType::UpdateMaxJamAmmo);
}

void UIngredientType::NativeConstruct()
{
	Super::NativeConstruct();
	IngredientAmmoPoints.Add(AmmoSlot1);
	IngredientAmmoPoints.Add(AmmoSlot2);
	IngredientAmmoPoints.Add(AmmoSlot3);
	IngredientAmmoPoints.Add(AmmoSlot4);
	IngredientAmmoPoints.Add(AmmoSlot5);
	UpdateIngredientAmmoSource(SelectedIngredient);
	UpdateMaxIngredientAmmo(CurrentIngredientMaxAmmo);
	UpdateIngredientAmmo(CurrentIngredientAmmo);
}

void UIngredientType::UpdateIngredientAmmoSource(FGameplayTag EquippedIngredient)
{
	if(EquippedIngredient.MatchesTagExact(FGameplayTags::Get().Weapon_Ingredient_Cheese))
	{
		CurrentIngredientAmmo = CheeseAmmo;
		CurrentIngredientMaxAmmo = CheeseMaxAmmo;
		UpdateAmmoImages(FullCheeseImage);
	}
	else if (EquippedIngredient.MatchesTagExact(FGameplayTags::Get().Weapon_Ingredient_Jam))
	{
		CurrentIngredientAmmo = JamAmmo;
		CurrentIngredientMaxAmmo = JamMaxAmmo;
		UpdateAmmoImages(FullJamImage);
	}
	else if (EquippedIngredient.MatchesTagExact(FGameplayTags::Get().Weapon_Ingredient_Wine))
	{
		CurrentIngredientAmmo = WineAmmo;
		CurrentIngredientMaxAmmo = WineMaxAmmo;
		UpdateAmmoImages(FullWineImage);
	}
	SelectedIngredient = EquippedIngredient;
}

void UIngredientType::SetCanShowButton(bool CanShowButton)
{
	ShowButton = CanShowButton;
}

void UIngredientType::UpdateIngredientAmmo(float newAttributeValue)
{
	if(!IngredientAmmoPoints.IsEmpty())
	{
		//Update the last Ammo image to let player know he lost ammo
		if(CurrentIngredientAmmo > newAttributeValue)
		{
			IngredientAmmoPoints[CurrentIngredientAmmo-1]->UpdateImage(true);
			CurrentIngredientAmmo = newAttributeValue;
		}
		else if (CurrentIngredientAmmo < newAttributeValue)
		{
			CurrentIngredientAmmo++;
			IngredientAmmoPoints[CurrentIngredientAmmo-1]->UpdateImage(false);
		}
	}
	
	if(this->GetVisibility() == ESlateVisibility::Hidden && newAttributeValue != 0)
	{
		this->SetVisibility(ESlateVisibility::HitTestInvisible);
		if(ShowButton)
		{
			ButtonSizeBox.Get()->SetVisibility(ESlateVisibility::HitTestInvisible);
		}
	}
	
}

void UIngredientType::UpdateMaxIngredientAmmo(float newAttributeValue)
{
	if(!IngredientAmmoPoints.IsEmpty())
	{
		for (int i = 0; i < IngredientAmmoPoints.Num(); i++)
		{
			if(i > newAttributeValue-1)
			{
				IngredientAmmoPoints[i].Get()->SetVisibility(ESlateVisibility::Hidden);
			}
			else
			{
				IngredientAmmoPoints[i].Get()->SetVisibility(ESlateVisibility::HitTestInvisible);
			}
		}
		CurrentIngredientMaxAmmo = newAttributeValue;
	}
	switch (static_cast<int>(newAttributeValue))
	{
	case 4:
		BeltImage.Get()->SetBrushFromTexture(Belt4Ingredients);
		break;
	case 5:
		BeltImage.Get()->SetBrushFromTexture(Belt5Ingredients);
		break;
	default:
		BeltImage.Get()->SetBrushFromTexture(Belt3Ingredients);
		break;
	}
}

void UIngredientType::UpdateCheeseAmmo(float newAttributeValue)
{
	CheeseAmmo = newAttributeValue;
	if(SelectedIngredient.MatchesTagExact(FGameplayTags::Get().Weapon_Ingredient_Cheese) || !SelectedIngredient.IsValid())
	{
		UpdateIngredientAmmo(newAttributeValue);
	}
}

void UIngredientType::UpdateMaxCheeseAmmo(float newAttributeValue)
{
	CheeseMaxAmmo = newAttributeValue;
	if(SelectedIngredient.MatchesTagExact(FGameplayTags::Get().Weapon_Ingredient_Cheese)|| !SelectedIngredient.IsValid())
	{
		UpdateMaxIngredientAmmo(newAttributeValue);
	}
}

void UIngredientType::UpdateJamAmmo(float newAttributeValue)
{
	JamAmmo = newAttributeValue;
	if(SelectedIngredient.MatchesTagExact(FGameplayTags::Get().Weapon_Ingredient_Jam))
	{
		UpdateIngredientAmmo(newAttributeValue);
	}
}

void UIngredientType::UpdateMaxJamAmmo(float newAttributeValue)
{
	JamMaxAmmo = newAttributeValue;
	if(SelectedIngredient.MatchesTagExact(FGameplayTags::Get().Weapon_Ingredient_Jam))
	{
		UpdateMaxIngredientAmmo(newAttributeValue);
	}
}

void UIngredientType::UpdateWineAmmo(float newAttributeValue)
{
	WineAmmo = newAttributeValue;
	if(SelectedIngredient.MatchesTagExact(FGameplayTags::Get().Weapon_Ingredient_Wine))
	{
		UpdateIngredientAmmo(newAttributeValue);
	}
}

void UIngredientType::UpdateMaxWineAmmo(float newAttributeValue)
{
	WineMaxAmmo = newAttributeValue;
	if(SelectedIngredient.MatchesTagExact(FGameplayTags::Get().Weapon_Ingredient_Wine))
	{
		UpdateMaxIngredientAmmo(newAttributeValue);
	}
}

void UIngredientType::UpdateAmmoImages(UTexture2D* Texture)
{
	for(UProgressBarPoint* Point : IngredientAmmoPoints)
	{
		Point->SetFullImage(Texture);
		for(int i =0; i < CurrentIngredientMaxAmmo; i++)
		{
			if(i < CurrentIngredientAmmo)
			{
				IngredientAmmoPoints[i]->UpdateImage(false);
			}
			else
			{
				IngredientAmmoPoints[i]->UpdateImage(true);
			}
		}
	}
}