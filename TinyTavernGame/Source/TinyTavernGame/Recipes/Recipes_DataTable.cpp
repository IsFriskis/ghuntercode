// TinyTavern


#include "Recipes_DataTable.h"

#include "TinyTavernGame/TT_GameplayTags.h"

void FRecipes_Struct::OnDataTableChanged(const UDataTable* InDataTable, const FName InRowName)
{
	FTableRowBase::OnDataTableChanged(InDataTable, InRowName);
	int NewID = 0;
	FRecipes_Struct* Row  = InDataTable->FindRow<FRecipes_Struct>(InRowName, TEXT(""), true);

	//Enemy Names
	if (Row->EnemyName_Tag == FGameplayTags::GameplayTags.Enemy_Gamusino)
	{
		NewID += 100000;
	}
	if (Row->EnemyName_Tag == FGameplayTags::GameplayTags.Enemy_Cucafera)
	{
		NewID += 200000;
	}
	if (Row->EnemyName_Tag == FGameplayTags::GameplayTags.Enemy_Cuelebre)
	{
		NewID += 300000;
	}
	if (Row->EnemyName_Tag == FGameplayTags::GameplayTags.Enemy_Basilisco)
	{
		NewID += 400000;
	}

	//Cuts
	if (Row->Cut_Tag == FGameplayTags::GameplayTags.States_Cuts_Sliced)
	{
		NewID += 10000;
	}
	if (Row->Cut_Tag == FGameplayTags::GameplayTags.States_Cuts_Smashed)
	{
		NewID += 20000;
	}
	if (Row->Cut_Tag == FGameplayTags::GameplayTags.States_Cuts_Skewered)
	{
		NewID += 30000;
	}
	//Cooks
	if (Row->Cook_Tag == FGameplayTags::GameplayTags.States_Cook_Grilled)
	{
		NewID += 1000;
	}
	else if (Row->Cook_Tag == FGameplayTags::GameplayTags.States_Cook_Boiled)
	{
		NewID += 2000;
	}
	else if (Row->Cook_Tag == FGameplayTags::GameplayTags.States_Cook_Fried)
	{
		NewID += 3000;
	}
	
	
	//Ingredients
	if (Row->Ingredient1_Tag == FGameplayTags::GameplayTags.States_Ingredient_Cheese)
	{
		NewID += 100;
	}
	if (Row->Ingredient2_Tag == FGameplayTags::GameplayTags.States_Ingredient_Jam)
	{
		NewID += 10;
	}
	if (Row->Ingredient3_Tag == FGameplayTags::GameplayTags.States_Ingredient_Wine)
	{
		NewID += 1;
	}
	Row->ID = NewID;
}
