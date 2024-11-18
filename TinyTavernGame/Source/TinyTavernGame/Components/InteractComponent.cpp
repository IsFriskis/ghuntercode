// TinyTavern


#include "InteractComponent.h"

#include "Components/SphereComponent.h"
#include "Components/WidgetComponent.h"
#include "TinyTavernGame/TinyTavernUtils.h"
#include "TinyTavernGame/Player/PlayerCharacter.h"
#include "TinyTavernGame/Player/TTPlayerController.h"


// Sets default values for this component's properties
UInteractComponent::UInteractComponent()
{
	//PrimaryComponentTick.bCanEverTick = true;

	// Initialize the collision sphere
	CollisionSphere = CreateDefaultSubobject<USphereComponent>(TEXT("SphereColliderInteract"));
	if (!CollisionSphere)
	{
		UE_LOG(LogTemp, Error, TEXT("Failed to create default subobject 'SphereComponent'"));
	}

	PointToSpawnDrop = CreateDefaultSubobject<USceneComponent>(TEXT("PointToSpawnDrop"));
	if (!CollisionSphere)
	{
		UE_LOG(LogTemp, Error, TEXT("Failed to create default subobject 'PointToSpawnDrop'"));
	}
	// Initialize the user widget
	UserWidget = CreateDefaultSubobject<UWidgetComponent>(TEXT("UserWidgetInteract"));
	if (!UserWidget)
	{
		UE_LOG(LogTemp, Error, TEXT("Failed to create default subobject 'UserWidget'"));
	}
}


// Called when the game starts
void UInteractComponent::BeginPlay()
{
	Super::BeginPlay();

	CollisionSphere->OnComponentBeginOverlap.AddDynamic(this, &UInteractComponent::OnOverlapBegin);
	CollisionSphere->OnComponentEndOverlap.AddDynamic(this, &UInteractComponent::OnOverlapEnd);

	UserWidget->SetVisibility(false);
}

void UInteractComponent::ShowUI()
{
		UserWidget->SetVisibility(true);
}

void UInteractComponent::HideUI()
{
	UserWidget->SetVisibility(false);
}


// Called every frame
void UInteractComponent::TickComponent(float DeltaTime, ELevelTick TickType,
                                       FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	
}

void UInteractComponent::Interact()
{
    print("Interact");
}

void UInteractComponent::OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	APlayerCharacter* Player = Cast<APlayerCharacter>(OtherActor);
	if(Player)
	{
		PlayerRef = Player;
		ShowUI();
		Cast<ATTPlayerController>(Player->GetController())->OnInteract.AddDynamic(this, &UInteractComponent::Interact);
		
	}
}

void UInteractComponent::OnOverlapEnd(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	APlayerCharacter* Player = Cast<APlayerCharacter>(OtherActor);
	if(Player)
	{
		PlayerRef = nullptr;
		HideUI();
		Cast<ATTPlayerController>(Player->GetController())->OnInteract.RemoveAll(this);
	}
}

