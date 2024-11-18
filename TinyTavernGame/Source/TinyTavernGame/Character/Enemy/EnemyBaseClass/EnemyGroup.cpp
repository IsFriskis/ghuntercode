// TinyTavern


#include "EnemyGroup.h"

#include "EnemyAAIController.h"
#include "EnemyBase.h"
#include "IA_EnemyManager.h"
#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Perception/AIPerceptionComponent.h"
#include "TinyTavernGame/TT_GameplayTags.h"
#include "TinyTavernGame/AbilitySystem/AttributeSet/TT_AttributeSet.h"
#include "TinyTavernGame/Character/Enemy/Gamusino/EnemyGamusino.h"
#include "TinyTavernGame/Character/Enemy/Gamusino/GamusinoGroup.h"
#include "TinyTavernGame/IA/MIscelaneous/ResetEnemyTrigger.h"


AEnemyGroup::AEnemyGroup()
{
	PrimaryActorTick.bCanEverTick = true;
}



void AEnemyGroup::BeginPlay()
{
	Super::BeginPlay();

	GetGameInstance()->GetSubsystem<UIA_EnemyManager>()->AddEnemyGroup(this);
	GI = Cast<UTT_GameInstance>(GetGameInstance());
	
	if(AllowedPremium)
	{
		if(GI->CurrentLevel<1)
		
			{
				PremiumEnemies = 1;
				
			}

		else
			{
				// Obtener la hora actual en ticks como semilla
				int64 CurrentTimeInTicks = FDateTime::Now().GetTicks();
				int32 Seed = static_cast<int32>(CurrentTimeInTicks & 0xFFFFFFFF); // Convertir a int32

				// Crear un FRandomStream con la semilla basada en el reloj
				FRandomStream RandomStream(Seed);
				int32 RandomNumber = FMath::RandRange(MinPercentage,MaxPercentage);
				PremiumEnemies = (RandomNumber * enemiesInGroupCount /100 ) +1  ;
				NormalEnemies = enemiesInGroupCount - PremiumEnemies;
			}
			NormalEnemies = enemiesInGroupCount - PremiumEnemies;
		
		SpawnEnemies(NormalEnemies,NormalEnemyType);
		SpawnEnemies(PremiumEnemies,PremiumEnemyType);
	}
	else
	{
		SpawnEnemies(enemiesInGroupCount,NormalEnemyType);
	}
	if(!WaypointsArray.IsEmpty())
	{
		SetWaypointInMap();
	}
	if(wanderFromGroup)
	{
		// Set the blackboard value to wander from the group
		for (AEnemyBase* enemy : GetEnemiesInGroup())
		{
			enemy->AIController->GetBlackboardComponent()->SetValueAsBool("WanderFromGroup", true);
		}
	}
	if(doesPatrol)
	{
		// Set the blackboard value to wander from the group
		for (AEnemyBase* enemy : GetEnemiesInGroup())
		{
			enemy->AIController->GetBlackboardComponent()->SetValueAsBool("DoesPatrol", true);
		}
	}
}

void AEnemyGroup::SpawnEnemies(int enemies,TSubclassOf<AEnemyBase> EnemyType)
{
	
	for (int32 i = 0; i < enemies; ++i)
	{
		bool isPositionValid = false;
		FVector spawnPosition;

		// Keep generating new positions until we find a valid one
		while (!isPositionValid)
		{
			// Generate random polar coordinates
			float angle = FMath::RandRange(0.0f, 2.0f * PI);
			float radius = FMath::Sqrt(FMath::RandRange(0.0f, 1.0f)) * spawnRadius; // Use square root to distribute points evenly

			// Convert to Cartesian coordinates
			float x = radius * FMath::Cos(angle);
			float y = radius * FMath::Sin(angle);

			// Calculate the potential spawn position
			spawnPosition = GetActorLocation() + FVector(x, y, 100);

			// Assume the position is valid until proven otherwise
			isPositionValid = true;

			// Check the distance to all previously spawned enemies
			for (AEnemyBase* enemy : enemiesInGroup)
			{
				if (FVector::Dist(spawnPosition, enemy->GetActorLocation()) < spawnDistance)
				{
					// The position is too close to an existing enemy, so it's not valid
					isPositionValid = false;
					break;
				}
			}
		}
		// Perform a raycast from 500 units above the spawn position to the ground
		FHitResult HitResult;
		FVector Start = spawnPosition + FVector(0, 0, 500);
		FVector End = spawnPosition;
		FCollisionQueryParams CollisionParams;

		if (GetWorld()->LineTraceSingleByChannel(HitResult, Start, End, ECC_Visibility, CollisionParams))
		{
			
			spawnPosition.Z = HitResult.Location.Z;
		}
		if(debugMode)
		{
			DrawDebugLine(
				GetWorld(),
				Start,
				End,
				FColor::Blue,  // Red color
				true,  // Persist over frames
				-1.0f,  // Infinite lifetime
				0,  // Depth priority
				1.0f  // Thickness
			);
		}

		FActorSpawnParameters SpawnParams;
		SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;
		SpawnParams.OverrideLevel = this->GetLevel();
		// Spawn the enemy at the calculated position
		AEnemyBase* newEnemy = GetWorld()->SpawnActor<AEnemyBase>(EnemyType, spawnPosition, FRotator::ZeroRotator, SpawnParams);

		if(newEnemy != nullptr)
		{
			newEnemy->SetGroup(this);
			enemiesInGroup.Add(newEnemy);
		}
	}
}



void AEnemyGroup::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if(doesRespawn)
	{
		if(enemiesInGroupCount<=0)
		{
			RespawnEnemies();
		}
	}

	if(debugMode)
	{
		//Debug sphere for the spawn radius
		DrawDebugSphere(
			GetWorld(), 
			GetActorLocation(), 
			spawnRadius, // radius
			12, // segments
			FColor::Red, 
			false, // persistent lines
			0.0f, // life time
			0, // depth priority
			1.0f // thickness
		);
	}
}

void AEnemyGroup::SetWaypointInMap()
{
	for (AWaypoint* Waypoint : WaypointsArray)
	{
		PatrolWaypointsMap.Add(Waypoint,nullptr);
	}
}

void AEnemyGroup::ResetEnemies()
{
	for (AEnemyBase* enemy : enemiesInGroup)
	{
		TObjectPtr<UTT_AttributeSet> EnemyAttributeSet = Cast<UTT_AttributeSet>(enemy->GetAttributeSet());
		FGameplayTagContainer EnemyTagContainer;
		FGameplayTagContainer FilterEnemyTagContainer;
		enemy->AIController->GetBlackboardComponent()->SetValueAsBool("ResetEnemy",true);
		enemy->GetAbilitySystemComponent()->GetOwnedGameplayTags(EnemyTagContainer);
		EnemyAttributeSet->SetCookbar(0);

		FGameplayTag EnemyTagBase = FGameplayTag::RequestGameplayTag(FName("Enemy"));
		// Recorrer todas las etiquetas en el contenedor
		for (const FGameplayTag& Tag : EnemyTagContainer)
		{
			// Verificar si la etiqueta coincide con "enemy" o es una sub-etiqueta de "enemy"
			if (Tag.MatchesTag(EnemyTagBase))
			{
				FilterEnemyTagContainer.AddTag(Tag);
				// Salir del bucle si se encuentra una etiqueta
				break;  
			}
		}
		/*if(EnemyTagContainer.HasTagExact(FGameplayTags::Get().Enemy_Gamusino))
		{
			FilterEnemyTagContainer.AddTag(FGameplayTags::Get().Enemy_Gamusino);
		}
		else if(EnemyTagContainer.HasTagExact(FGameplayTags::Get().Enemy_Basilisco))
		{
			FilterEnemyTagContainer.AddTag(FGameplayTags::Get().Enemy_Basilisco);
		}
		else if(EnemyTagContainer.HasTagExact(FGameplayTags::Get().Enemy_Cucafera))
		{
			FilterEnemyTagContainer.AddTag(FGameplayTags::Get().Enemy_Cucafera);
		}
		else
		{
			FilterEnemyTagContainer.AddTag(FGameplayTags::Get().Enemy_Cuelebre);
		}*/
		FilterEnemyTagContainer.AddTag(FGameplayTags::Get().States_Raw);
		FilterEnemyTagContainer.AddTag(FGameplayTags::Get().States_ResetEnemy);
		if(enemy->IsPremium)
		{
			FilterEnemyTagContainer.AddTag(FGameplayTags::Get().PremiumEnemy);
		}
		FGameplayTagContainer NewEnemyTagContainer = EnemyTagContainer.Filter(FilterEnemyTagContainer);
		enemy->GetAbilitySystemComponent()->RemoveLooseGameplayTags(EnemyTagContainer);
		enemy->GetAbilitySystemComponent()->AddLooseGameplayTags(FilterEnemyTagContainer);
		enemy->AIController->IsDetectingPlayer = false;
		enemy->AIController->CurrentAlertDetectionTime = enemy->AIController->MaxAlertDetectionTime;
		enemy->AIController->CurrentRemoveInvestigationStateTimer = 0;
		enemy->AIController->GetBlackboardComponent()->SetValueAsVector(FName("SpawnLocation"),enemy->SpawnPosition);
	}
}

void AEnemyGroup::RespawnEnemies()
{
	enemiesInGroupCount=numToSpawn;
	SpawnEnemies(numToSpawn,NormalEnemyType);
}
