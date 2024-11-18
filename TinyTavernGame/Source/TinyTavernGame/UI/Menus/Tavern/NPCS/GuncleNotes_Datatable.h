// TinyTavern

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "GuncleNotes_Datatable.generated.h"

USTRUCT(BlueprintType)
struct FGuncleNotes_Struct : public FTableRowBase
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int ID = 0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FSlateBrush Note;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FText NoteTitle;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FText NoteText;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FText SignatureText;
	
};
/**
 * 
 */
UCLASS()
class TINYTAVERNGAME_API UGuncleNotes_Datatable : public UDataTable
{
	GENERATED_BODY()

private:
	int numberOfIDs = 0;
};
