// TinyTavern

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "Engine/DataTable.h"
#include "Recipes_DataTable.generated.h"
class UImage;


USTRUCT(BlueprintType)
struct FRecipes_Struct : public FTableRowBase
{
	GENERATED_BODY()

	UPROPERTY(VisibleAnywhere)
	int ID = 0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite,meta = (Categories="Enemy"))
	FGameplayTag EnemyName_Tag;
	

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (Categories = "States.Cuts"))
	FGameplayTag Cut_Tag;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (Categories = "States.Cook"))
	FGameplayTag Cook_Tag;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (Categories = "States.Ingredients"))
	FGameplayTag Ingredient1_Tag;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (Categories = "States.Ingredients"))
	FGameplayTag Ingredient2_Tag;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (Categories = "States.Ingredients"))
	FGameplayTag Ingredient3_Tag;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<UTexture2D> RecipeSprite;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FText Name;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FText Description;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int Stars = 0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int Principal_Mission = 0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool Vip = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int Progression = 0;

	virtual void OnDataTableChanged(const UDataTable* InDataTable, const FName InRowName) override;
	
};
/**
 * 
 */
UCLASS()
class TINYTAVERNGAME_API URecipes_DataTable : public UDataTable
{
	GENERATED_BODY()

	
	
};
