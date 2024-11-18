// TinyTavern


#include "AbilityTask_ArcCollisions.h"

#include "GameFramework/Character.h"


UAbilityTask_ArcCollisions::UAbilityTask_ArcCollisions(const FObjectInitializer& ObjectInitializer)
{
	bTickingTask = true;
}

UAbilityTask_ArcCollisions* UAbilityTask_ArcCollisions::ArcCollisions(UGameplayAbility* OwningAbility,
	FName TaskInstanceName, FVector StartLocation, FRotator StartRotation, float Radius, float RadiusStep, float AngleDegrees,
	int32 Segments)
{
	UAbilityTask_ArcCollisions* ArcCollisionsObj = NewAbilityTask<UAbilityTask_ArcCollisions>(OwningAbility, TaskInstanceName);
	ArcCollisionsObj->StartLocation = StartLocation;
	ArcCollisionsObj->StartRotation = StartRotation;
	ArcCollisionsObj->Radius = Radius;
	ArcCollisionsObj->RadiusStep = RadiusStep;
	ArcCollisionsObj->AngleDegrees = AngleDegrees;
	ArcCollisionsObj->Segments = Segments;

	return ArcCollisionsObj;
}


void UAbilityTask_ArcCollisions::Activate()
{
	Super::Activate();
	CurrentRadius = 0;
}



void UAbilityTask_ArcCollisions::TickTask(float DeltaTime)
{
	Super::TickTask(DeltaTime);
	CurrentRadius += RadiusStep;
	// Convertir el ángulo a radianes
	float AngleRadians = FMath::DegreesToRadians(AngleDegrees);

	//Tamaño del arco
	float ArcLength = AngleRadians * CurrentRadius;

	//Calcular el número de Segmentos en función del tamaño del Radio
	Segments = (ArcLength / 50) + 1;
	// Calcular el paso de ángulo entre cada segmento
	float AngleStep = AngleRadians / Segments;

	for (int32 i = 0; i <= Segments; i++)
	{
		// Calcular el ángulo actual
		float CurrentAngle = AngleStep * i;

		// Calcular la posición del punto en el arco
		FVector EndLocation = StartLocation + StartRotation.RotateVector(FVector(FMath::Cos(CurrentAngle) * CurrentRadius, FMath::Sin(CurrentAngle) * CurrentRadius, 0.0f));

		// Parámetros de colisión
		FCollisionQueryParams CollisionParams;
		CollisionParams.AddIgnoredActor(GetOwnerActor()); // Ignorar el actor que realiza el barrido

		// Resultado de la colisión
		TArray<FHitResult> OutHits;

		// Realizar el barrido
		GetWorld()->SweepMultiByChannel(OutHits, EndLocation, EndLocation+0.01, FQuat::Identity, ECollisionChannel::ECC_GameTraceChannel1, FCollisionShape::MakeSphere(50), CollisionParams);

		// Dibuja el barrido para depuración
		//DrawDebugLine(GetWorld(), StartLocation, EndLocation, FColor::Green, false, 1.0f, 0, 1.0f);
		DrawDebugSphere(GetWorld(),EndLocation,25,6,FColor::Red,false,-1,0,1);
		for (auto& Hit : OutHits)
		{
			if(Hit.GetActor()->GetClass()->IsChildOf(ACharacter::StaticClass()))
			{
				OnHit.Broadcast(Hit.GetActor());
			}
		}

	}

	if (CurrentRadius > Radius)
	{
		OnCompleted.Broadcast(nullptr);
		EndTask();
	}
}

void UAbilityTask_ArcCollisions::OnGameplayAbilityCancelled()
{
}