// TinyTavern


#include "Resource.h"

#include "MotionWarpingComponent.h"
#include "ResourceManager.h"
#include "Components/BoxComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Components/WidgetComponent.h"
#include "Components/SphereComponent.h"
#include "Kismet/KismetMaterialLibrary.h"
#include "TinyTavernGame/TT_GameplayTags.h"
#include "TinyTavernGame/Actor/EffectActor.h"
#include "TinyTavernGame/Settings/TT_GameInstance.h"

// Sets default values
AResource::AResource()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	ResourceTrigger = CreateDefaultSubobject<UBoxComponent>("Resource Trigger");
	RootComponent = ResourceTrigger.Get();

	ResourceSkeletalMesh = CreateDefaultSubobject<USkeletalMeshComponent>("Resource Skeletal Mesh");
	ResourceSkeletalMesh.Get()->SetupAttachment(ResourceTrigger);

	ResourceWidget = CreateDefaultSubobject<UWidgetComponent>("Resource Widget");
	ResourceWidget.Get()->SetupAttachment(ResourceTrigger);

	ResourceMeshCollision = CreateDefaultSubobject<UBoxComponent>("Resource Mesh Collision");
	ResourceMeshCollision.Get()->SetupAttachment(ResourceSkeletalMesh);

	MotionWarpingComponent = CreateDefaultSubobject<UMotionWarpingComponent>("Motion Warping Component");

	MotionWarpingLocation = CreateDefaultSubobject<USceneComponent>("Motion Warping Location2");
	MotionWarpingLocation.Get()->SetupAttachment(ResourceSkeletalMesh);
	
	
}

void AResource::SpawnDrop()
{
	GetWorld()->SpawnActor<AEffectActor>(DropClass, GetActorTransform());
	//TODO: after FP, don't destroy, leave a little mesh
	Destroy();
}

// Called when the game starts or when spawned
void AResource::BeginPlay()
{
	Super::BeginPlay();

	OffState();
	UTT_GameInstance* GI =  Cast<UTT_GameInstance>(GetGameInstance());

	if(ResourceType == FGameplayTags::Get().ResourceType_Boil)
	{
		if(GI->UnlockedWeapons[FGameplayTags::Get().Weapon_Range_Boil])
		{
			OnState();
		}
		GetGameInstance()->GetSubsystem<UResourceManager>()->AddBoilResource(this);
	}
	else if(ResourceType == FGameplayTags::Get().ResourceType_Fry)
	{
		if(GI->UnlockedWeapons[FGameplayTags::Get().Weapon_Range_Fry])
		{
			OnState();
		}
		GetGameInstance()->GetSubsystem<UResourceManager>()->AddFryResource(this);
	}
	else if (ResourceType == FGameplayTags::Get().ResourceType_Grill)
	{
		OnState();	
	}
	else
	{
		OnState();	
	}
	ResourceDissapearBlend.SetBlendTime(ResourceDissapearTime);
	ResourceDissapearBlend.SetValueRange(0,1);
	DynamicMat = UKismetMaterialLibrary::CreateDynamicMaterialInstance(GetWorld(), NoteDissolveMaterial);
	DynamicMat.Get()->SetScalarParameterValue("BurnDisappearance", 0.0f);
}

void AResource::OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	TObjectPtr<APlayerCharacter> player = Cast<APlayerCharacter>(OtherActor);
	if (player)
	{
		ResourceWidget.Get()->SetVisibility(true);
		ResourceSkeletalMesh.Get()->SetOverlayMaterial(HighlightMaterial.Get());
		PlayerInside = true;
	}
}	

void AResource::OnOverlapEnd(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	TObjectPtr<APlayerCharacter> player = Cast<APlayerCharacter>(OtherActor);
	if (player)
	{
		ResourceWidget.Get()->SetVisibility(false);
		ResourceSkeletalMesh.Get()->SetOverlayMaterial(nullptr);
		PlayerInside = false;
	}
}



void AResource::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);
	if(ResourceType == FGameplayTags::Get().ResourceType_Boil)
	{
		GetGameInstance()->GetSubsystem<UResourceManager>()->RemoveBoilResource(this);
	}
	else if(ResourceType == FGameplayTags::Get().ResourceType_Fry)
	{
		GetGameInstance()->GetSubsystem<UResourceManager>()->RemoveFryResource(this);
	}
	else if (ResourceType == FGameplayTags::Get().ResourceType_Grill)
	{
		
	}
}

// Called every frame
void AResource::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if(ShouldDissapear)
	{
		ResourceDissapearBlend.Update(DeltaTime);
		DynamicMat.Get()->SetScalarParameterValue("BurnDisappearance", ResourceDissapearBlend.GetBlendedValue());
		if(ResourceDissapearBlend.IsComplete())
		{
			Destroy();
		}
	}
}


void AResource::OnState()
{
	
	ResourceTrigger.Get()->OnComponentBeginOverlap.AddDynamic(this, &AResource::OnOverlapBegin);
	
	ResourceTrigger.Get()->OnComponentEndOverlap.AddDynamic(this, &AResource::OnOverlapEnd);
	
	ResourceTrigger->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	ResourceTrigger->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
	ResourceTrigger->SetCollisionResponseToChannel(ECollisionChannel::ECC_GameTraceChannel3, ECollisionResponse::ECR_Overlap);
	ResourceMeshCollision->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	ResourceMeshCollision->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Block);
	ResourceMeshCollision->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECollisionResponse::ECR_Ignore);
	SetActorHiddenInGame(false);
	
}

void AResource::OffState()
{
	if(ResourceTrigger.Get()->OnComponentBeginOverlap.IsBound())
	{
		ResourceTrigger.Get()->OnComponentBeginOverlap.RemoveDynamic(this, &AResource::OnOverlapBegin);
	}
	if(ResourceTrigger.Get()->OnComponentEndOverlap.IsBound())
	{
		ResourceTrigger.Get()->OnComponentEndOverlap.RemoveDynamic(this, &AResource::OnOverlapEnd);
	}
	ResourceWidget.Get()->SetVisibility(false);
	SetActorHiddenInGame(true);

	ResourceTrigger->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	ResourceMeshCollision->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	ResourceSkeletalMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}

void AResource::SetShouldDissapear(bool _ShouldDissapear)
{
	ShouldDissapear = _ShouldDissapear;
	ResourceSkeletalMesh.Get()->SetMaterial(0,DynamicMat);
	ResourceDissapearBlend.Reset();
}
