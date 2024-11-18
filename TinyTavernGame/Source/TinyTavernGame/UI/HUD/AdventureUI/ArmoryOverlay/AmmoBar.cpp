// TinyTavern


#include "AmmoBar.h"

#include "GameplayTagContainer.h"
#include "Components/ProgressBar.h"
#include "Components/TextBlock.h"
#include "TinyTavernGame/TT_GameplayTags.h"
#include "TinyTavernGame/UI/HUD/OverlayWidgetController.h"

void UAmmoBar::WidgetControllerSet_Implementation()
{
	UOverlayWidgetController* OverlayWidget = Cast<UOverlayWidgetController>(WidgetController);
	OverlayWidget->OnBoiledAmmoChange.AddDynamic(this,&UAmmoBar::SetBoilAmmo);
	OverlayWidget->OnMaxBoiledAmmoChange.AddDynamic(this,&UAmmoBar::SetMaxBoilAmmo);
	OverlayWidget->OnFriedAmmoChange.AddDynamic(this,&UAmmoBar::SetFryAmmo);
	OverlayWidget->OnMaxFriedAmmoChange.AddDynamic(this,&UAmmoBar::SetMaxFryAmmo);
	OverlayWidget->OnGrilledAmmoChange.AddDynamic(this,&UAmmoBar::SetGrillAmmo);
	OverlayWidget->OnMaxGrilledAmmoChange.AddDynamic(this,&UAmmoBar::SetMaxGrillAmmo);
}

void UAmmoBar::UpdateAmmoSource(FGameplayTag EquippedWeapon)
{
	//Update the Attribute the bar is displaying
	SelectedWeapon = EquippedWeapon;
	
	FProgressBarStyle Style = ProgressBar.Get()->GetWidgetStyle();
	if(SelectedWeapon.MatchesTagExact(FGameplayTags::Get().Weapon_Range_Boil))
	{
		Style.SetBackgroundImage(BoilImage);
		Style.SetFillImage(BoilFillImage);
		ProgressBar->SetFillColorAndOpacity(BoilFillImage.TintColor.GetSpecifiedColor());
		CurrentAmmo = BoilAmmo;
		CurrentMaxAmmo = MaxBoilAmmo;
	}
	else if(SelectedWeapon.MatchesTagExact(FGameplayTags::Get().Weapon_Range_Fry))
	{
		Style.SetBackgroundImage(FryImage);
		Style.SetFillImage(FryFillImage);
		ProgressBar->SetFillColorAndOpacity(BoilFillImage.TintColor.GetSpecifiedColor());
		CurrentAmmo = FryAmmo;
		CurrentMaxAmmo = MaxFryAmmo;
	}
	else if(SelectedWeapon.MatchesTagExact(FGameplayTags::Get().Weapon_Range_Grill))
	{
		Style.SetBackgroundImage(GrillImage);
		Style.SetFillImage(GrillFillImage);
		ProgressBar->SetFillColorAndOpacity(BoilFillImage.TintColor.GetSpecifiedColor());
		CurrentAmmo = GrillAmmo;
		CurrentMaxAmmo = MaxGrillAmmo;
	}
	ProgressBar->SetWidgetStyle(Style);
	UpdateAmmo();
}

void UAmmoBar::UpdateAmmo()
{
	if(SelectedWeapon.MatchesTagExact(FGameplayTags::Get().Weapon_Range_Boil))
	{
		ProgressBar.Get()->SetPercent(BoilAmmo/MaxBoilAmmo);
		CurrentAmmo = BoilAmmo;
		CurrentMaxAmmo = MaxBoilAmmo;
	}
	else if(SelectedWeapon.MatchesTagExact(FGameplayTags::Get().Weapon_Range_Fry))
	{
		ProgressBar.Get()->SetPercent(FryAmmo/MaxFryAmmo);
		CurrentAmmo = FryAmmo;
		CurrentMaxAmmo = MaxFryAmmo;
	}
	else if(SelectedWeapon.MatchesTagExact(FGameplayTags::Get().Weapon_Range_Grill))
	{
		ProgressBar.Get()->SetPercent(GrillAmmo/MaxGrillAmmo);
		CurrentAmmo = GrillAmmo;
		CurrentMaxAmmo = MaxGrillAmmo;
	}
	OnAmmoUpdate.Broadcast(CurrentAmmo,CurrentMaxAmmo);
}



float UAmmoBar::GetCurrentAmmo()
{
	return CurrentAmmo;
}

float UAmmoBar::GetCurrentMaxAmmo()
{
	return  CurrentMaxAmmo;
}

void UAmmoBar::SetBoilAmmo(float newAmmo)
{
	BoilAmmo = newAmmo;
	UpdateAmmo();
}

void UAmmoBar::SetMaxBoilAmmo(float newAmmo)
{
	MaxBoilAmmo = newAmmo;
	UpdateAmmo();
}

void UAmmoBar::SetFryAmmo(float newAmmo)
{
	FryAmmo = newAmmo;
	UpdateAmmo();
}

void UAmmoBar::SetMaxFryAmmo(float newAmmo)
{
	MaxFryAmmo = newAmmo;
	UpdateAmmo();
}

void UAmmoBar::SetGrillAmmo(float newAmmo)
{
	GrillAmmo = newAmmo;
	UpdateAmmo();
}

void UAmmoBar::SetMaxGrillAmmo(float newAmmo)
{
	MaxGrillAmmo = newAmmo;
	UpdateAmmo();
}

