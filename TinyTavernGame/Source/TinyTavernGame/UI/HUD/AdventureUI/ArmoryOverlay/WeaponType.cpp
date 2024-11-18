// TinyTavern


#include "WeaponType.h"
#include "GameplayTagContainer.h"
#include "Components/Image.h"
#include "TinyTavernGame/TT_GameplayTags.h"

void UWeaponType::UpdateImage(FGameplayTag newWeapon)
{
	for (FWeaponTextures Texture : Textures)
	{
		if(Texture.Tag == newWeapon)
		{
			SelectedWeapon->SetBrushFromTexture(Texture.Texture);
		}
	}
	
}
