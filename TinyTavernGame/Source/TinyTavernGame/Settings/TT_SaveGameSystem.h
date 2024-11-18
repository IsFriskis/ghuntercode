// TinyTavern

#pragma once

#include "CoreMinimal.h"
#include "TT_GameInstance.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "TT_SaveGameSystem.generated.h"

class UTT_SaveGame;
/**
 * 
 */
UCLASS()
class TINYTAVERNGAME_API UTT_SaveGameSystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()
public:

	UTT_SaveGameSystem(const FObjectInitializer& ObjectInitializer);

	UFUNCTION(BlueprintCallable)
	bool CreateSaveGameInstance();
	bool CreateDefaultSaveGameInstance(UGameInstance* _GameInstance);

	UFUNCTION(BlueprintCallable)
	bool LoadSaveGameData();
	bool ApplyLoadToGameInstance(UTT_SaveGame* _SaveGameInstance);


	UFUNCTION(BlueprintCallable)
	bool SaveGameData(UGameInstance* _GameInstance);

	UFUNCTION(BlueprintCallable)
	bool DeleteSaveGame();
	
	UFUNCTION(BlueprintCallable)
	UTT_SaveGame* GetSaveGameInstance() {return SaveGameInstance;}

	UFUNCTION(BlueprintCallable)
	void SetSaveGameInstance(UTT_SaveGame* _SaveGame) {SaveGameInstance = _SaveGame;}

	UFUNCTION(BlueprintCallable)
	FString GetSlotToSaveName() const {return SlotToSaveName;}

	UFUNCTION(BlueprintCallable)
	int32 GetSlotToSaveUserIdIndex() const {return SlotToSaveUserIdIndex;}

protected:
	bool ParseAllGameInstanceToSaveGame(UGameInstance* _GameInstance);
	UPROPERTY()
	UTT_SaveGame* SaveGameInstance = nullptr;

private:

	FString SlotToSaveName = "SlotA";
	int32 SlotToSaveUserIdIndex = 1;
};

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnSaveCompleted);


class FSaveGameAsyncTask : public FNonAbandonableTask 
{
	friend class FAsyncTask<FSaveGameAsyncTask>;
public:

	FOnSaveCompleted OnCompleted;
	UTT_SaveGameSystem* SaveGameSystem;
	UTT_GameInstance* MyData;

	FSaveGameAsyncTask(UTT_SaveGameSystem* _SaveGameSystem,
		UTT_GameInstance* _MyData)
	 :
	SaveGameSystem(_SaveGameSystem),
	MyData(_MyData)

	{
	}

	void DoWork()
	{
		if(SaveGameSystem->SaveGameData(MyData))
		{
			OnCompleted.Broadcast();
		}
	}

	FORCEINLINE TStatId GetStatId() const
	{
		RETURN_QUICK_DECLARE_CYCLE_STAT(ExampleAutoDeleteAsyncTask, STATGROUP_ThreadPoolAsyncTasks);
	}
};

class FLoadGameAsyncTask : public FNonAbandonableTask 
{
	friend class FAsyncTask<FLoadGameAsyncTask>;
public:

	UTT_SaveGameSystem* SaveGameSystem;

	FLoadGameAsyncTask(UTT_SaveGameSystem* _SaveGameSystem)
	 :
	SaveGameSystem(_SaveGameSystem)
	{
	}

	void DoWork()
	{
		SaveGameSystem->LoadSaveGameData();
	}

	FORCEINLINE TStatId GetStatId() const
	{
		RETURN_QUICK_DECLARE_CYCLE_STAT(ExampleAutoDeleteAsyncTask, STATGROUP_ThreadPoolAsyncTasks);
	}
};

class FDeleteGameAsyncTask : public FNonAbandonableTask 
{
	friend class FAsyncTask<FDeleteGameAsyncTask>;
public:

	UTT_SaveGameSystem* SaveGameSystem;

	FDeleteGameAsyncTask(UTT_SaveGameSystem* _SaveGameSystem)
	 :
	SaveGameSystem(_SaveGameSystem)
	{
	}

	void DoWork()
	{
		SaveGameSystem->DeleteSaveGame();
	}

	FORCEINLINE TStatId GetStatId() const
	{
		RETURN_QUICK_DECLARE_CYCLE_STAT(ExampleAutoDeleteAsyncTask, STATGROUP_ThreadPoolAsyncTasks);
	}
};

class FCreateDefaultGameAsyncTask : public FNonAbandonableTask 
{
	friend class FAsyncTask<FCreateDefaultGameAsyncTask>;
public:

	UTT_SaveGameSystem* SaveGameSystem;
	UTT_GameInstance* MyData;

	FCreateDefaultGameAsyncTask(UTT_SaveGameSystem* _SaveGameSystem,
		UTT_GameInstance* _MyData)
	 :
	SaveGameSystem(_SaveGameSystem),
	MyData(_MyData)

	{
	}

	void DoWork()
	{
		SaveGameSystem->CreateDefaultSaveGameInstance(MyData);
	}

	FORCEINLINE TStatId GetStatId() const
	{
		RETURN_QUICK_DECLARE_CYCLE_STAT(ExampleAutoDeleteAsyncTask, STATGROUP_ThreadPoolAsyncTasks);
	}
};