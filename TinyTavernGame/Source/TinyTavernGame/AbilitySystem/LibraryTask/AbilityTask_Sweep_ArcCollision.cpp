// TinyTavern


#include "AbilityTask_Sweep_ArcCollision.h"
#include "GameFramework/Character.h"
#include "ProfilingDebugging/StallDetector.h"
#include "TinyTavernGame/Character/Enemy/EnemyBaseClass/EnemyBase.h"



UAbilityTask_Sweep_ArcCollision::UAbilityTask_Sweep_ArcCollision(const FObjectInitializer& ObjectInitializer)
{
	bTickingTask = true;
}

UAbilityTask_Sweep_ArcCollision* UAbilityTask_Sweep_ArcCollision::SweepArcCollisions(UGameplayAbility* OwningAbility,
	FName TaskInstanceName,FName SocketName,float ArchWide,float LeftAngle, float RightAngle,float Distance,float Speed,float DelayFirstTime,bool DrawDebug)
{
	UAbilityTask_Sweep_ArcCollision* ArcCollisionsObj = NewAbilityTask<UAbilityTask_Sweep_ArcCollision>(OwningAbility, TaskInstanceName);

	ArcCollisionsObj->SocketName = SocketName;
	ArcCollisionsObj->ArchWide = ArchWide;
	ArcCollisionsObj->LeftAngle = LeftAngle;
	ArcCollisionsObj->RightAngle = RightAngle;
	ArcCollisionsObj->Distance = Distance;
	ArcCollisionsObj->Speed = Speed;
	ArcCollisionsObj->DelayFirstTime = DelayFirstTime;
	ArcCollisionsObj->DrawDebug = DrawDebug;
	ArcCollisionsObj->Enemy= Cast<AEnemyBase>(OwningAbility->GetActorInfo().OwnerActor.Get());
	

	return ArcCollisionsObj;
}

void UAbilityTask_Sweep_ArcCollision::Activate()
{
	Super::Activate();
	External_Radius = ArchWide;
	Internal_Radius = 0;
}


void UAbilityTask_Sweep_ArcCollision::TickTask(float DeltaTime)
{
	Super::TickTask(DeltaTime);
	External_Radius += DeltaTime * Speed;
	Internal_Radius += DeltaTime * Speed;
	timer += DeltaTime;

	//Waiting DelayTime for first Sweep
	if(!FirstTime)
	{
		if(timer > DelayFirstTime)
		{
			FirstTime = true;
			StartLocation = Enemy->GetMesh()->GetSocketLocation(SocketName);
			const double YawRotation = Enemy->GetMesh()->GetSocketRotation(SocketName).Yaw;
			LeftAngle += YawRotation;
			RightAngle += YawRotation;

		}
	}

	//Begin Sweep before DelayTime
	if(FirstTime)
	{
		if(External_Radius < Distance)
		{
			TArray<FHitResult> OutHits;
			FVector PlayerLocation = FVector::Zero();
			FCollisionQueryParams CollisionParams;
			CollisionParams.AddIgnoredActor(GetOwnerActor());

			bool isHit = GetWorld()->SweepMultiByChannel(OutHits, StartLocation, StartLocation+ 0.1, FQuat::Identity, ECollisionChannel::ECC_GameTraceChannel1, FCollisionShape::MakeSphere(External_Radius), CollisionParams);
			if(isHit)
			{
				for( auto& hit : OutHits)
				{
					if(hit.GetActor()->GetClass()->IsChildOf(ACharacter::StaticClass()))
					{
						PlayerLocation = hit.GetActor()->GetActorLocation();
					
						if (PlayerLocation != FVector::Zero())
						{
							float RelativeX = PlayerLocation.X - StartLocation.X;
							float RelativeY = PlayerLocation.Y - StartLocation.Y;
							//Check if Player is in ArchWide
							if(	(RelativeX * RelativeX) + (RelativeY *RelativeY) > Internal_Radius * Internal_Radius)
							{
								double ArcPosition = FMath::RadiansToDegrees(atan2(RelativeY,RelativeX));
								//Check if player is between the angles.
								if ((LeftAngle < ArcPosition) && (ArcPosition < RightAngle))
								{
									//The player is in Area
									OnHit.Broadcast(hit.GetActor());
							
								}
							}
							//The player is out of area
							OnHit.Broadcast(nullptr);
					
						}
					}
				}
			}
		}
		//Arc Arrive to Max Distance
		else
		{
			OnCompleted.Broadcast(nullptr);
			EndTask();
		}

		//Preview Collision Area
		if(DrawDebug)
		{
			DrawDebugCircle(GetWorld(), StartLocation, External_Radius, 12, FColor::Black, false, 0.1, 0, 0.5, FVector(1, 0, 0), FVector(0, 1, 0), false);
			DrawDebugCircle(GetWorld(), StartLocation, Internal_Radius, 12, FColor::Red, false, 0.1, 0, 0.5, FVector(1, 0, 0), FVector(0, 1, 0), false);

			FVector EndPoint = FVector (External_Radius * cos(FMath::DegreesToRadians(LeftAngle)),External_Radius * sin(FMath::DegreesToRadians(LeftAngle)),0) + StartLocation;
			FVector StartPoint = FVector (Internal_Radius * cos(FMath::DegreesToRadians(LeftAngle)),Internal_Radius * sin(FMath::DegreesToRadians(LeftAngle)),0) + StartLocation; 
			DrawDebugLine(GetWorld(),StartPoint,EndPoint,FColor::Black,false,-1,0,0.5);

			EndPoint = FVector (External_Radius * cos(FMath::DegreesToRadians(RightAngle)),External_Radius * sin(FMath::DegreesToRadians(RightAngle)),0) + StartLocation;
			StartPoint = FVector (Internal_Radius * cos(FMath::DegreesToRadians(RightAngle)),Internal_Radius * sin(FMath::DegreesToRadians(RightAngle)),0) + StartLocation;

			DrawDebugLine(GetWorld(),StartPoint,EndPoint,FColor::Black,false,-1,0,0.5) ;
		
		}
	}
}
	

	


void UAbilityTask_Sweep_ArcCollision::OnGameplayAbilityCancelled()
{
}